#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"

#include <openssl/rsa.h>


namespace {
  const int KEY_SIZE = 512;

  /*
   * Takes an OpenSLL BIGNUM and return an equivalent LLVM APInt.
   */
  llvm::APInt ConvertBignum(const BIGNUM *bn) {
    // Get bytes in big-endian order
    int size = BN_num_bytes(bn);
    std::unique_ptr<uint8_t[]> bytes(new uint8_t[size]);
    BN_bn2bin(bn, bytes.get());

    // Reconstruct a LLVM APInt from the big num
    // There are better ways to do this, but We Do Not Care (TM)
    llvm::APInt out(KEY_SIZE * 2, 0, false);
    for (int i=0; i<size; i++) {
      out <<= 8;
      out  |= bytes[i];
    }

    return out;
  }

  /*
   * Generate a random RSA key and does `pow(target, e, n)`.
   * `e` is fixed to 65537, so this is actually `pow(target, 65537, n)`.
   * The private key is only used through `n`.
   *
   * See:
   * https://stackoverflow.com/questions/5927164/how-to-generate-rsa-private-key-using-openssl
   * https://www.codepool.biz/how-to-use-openssl-generate-rsa-keys-cc.html
   */
  llvm::APInt MakeRandomKey(llvm::APInt& n, llvm::APInt &e, const llvm::APInt& target) {
    // bne = 65537
    BIGNUM *bne = BN_new();
    assert(BN_set_word(bne, RSA_F4) == 1);

    // Generate and get modulus and public exponent
    const BIGNUM *pMod, *pPubExp, *pPvtExp;
    RSA *rsa = RSA_new();
    RSA_generate_key_ex(rsa, KEY_SIZE, bne, nullptr);
    RSA_get0_key(rsa, &pMod, &pPubExp, &pPvtExp);

    // Convert target to OpenSSL bignum
    BIGNUM *bn_target = BN_new();
    assert(BN_set_word(bn_target, target.getLimitedValue(UINT64_MAX)) == 1);

    // Super secure RSA
    // result = target ^ pubExp % mod
    BIGNUM *bn_result = BN_new();
    BN_CTX *bnCtx = BN_CTX_new();
    BN_mod_exp(bn_result, bn_target, pPubExp, pMod, bnCtx);
    BN_CTX_free(bnCtx);

    // Convert the encrypted value back to LLVM types
    llvm::APInt newTarget = ConvertBignum(bn_result);

    // Convert public mod/exp to LLVM
    llvm::APInt mod    = ConvertBignum(pMod);
    llvm::APInt pubExp = ConvertBignum(pPubExp);
    n = mod;
    e = pubExp;

    // Cleanup
    RSA_free(rsa);
    BN_free(bne);
    BN_free(bn_target);
    BN_free(bn_result);

    return newTarget;
  }

  struct DumboPass : public llvm::FunctionPass {
    static char ID;
    DumboPass() : llvm::FunctionPass(ID) {}

    virtual bool runOnFunction(llvm::Function &F) {
      bool modified = false;
      llvm::errs() << "Running on \"" << F.getName() << "\"\n";

      // A function is made of basic blocks...
			for (llvm::BasicBlock& B : F) {
        // ...which are made out of instructions...
				for (llvm::Instruction& I : B) {
          // ...and we only want `icmp` instructions...
          if (auto op = llvm::dyn_cast<llvm::ICmpInst>(&I)) {
            // ...which use the equality operator
            if (op->getPredicate() != llvm::ICmpInst::ICMP_EQ) {
              continue;
            }

            // We only obfuscate constants.
            // We only handle them in the right-hand side but it's trivial to
            // add support for lhs.
            llvm::ConstantInt* rhs = llvm::dyn_cast<llvm::ConstantInt>(op->getOperand(1));
            if (rhs == nullptr) {
              continue;
            }
  
            // Print some debug info
            llvm::errs() << "Found ICMP: " << *op << "\n";

            // Create an IRBuilder to make things easier
            llvm::IRBuilder<> builder(op);

            // New operand size
            unsigned opSize = KEY_SIZE * 2;
            llvm::Type* opType = llvm::Type::getIntNTy(F.getContext(), opSize);

            // Get the non-constant operand on the left
            llvm::Value* lhs = op->getOperand(0);
            // Extend it to the new operand size
            lhs = builder.CreateZExt(lhs, opType);

            // Create _modexp FunctionType
            std::vector<llvm::Type*> args;
            args.push_back(opType); // base
            args.push_back(opType); // exponent
            args.push_back(opType); // mod
            llvm::FunctionType* fccType = llvm::FunctionType::get(
              opType,
              args,
              false
            );
            // Declare it in the current module (or get a reference to it)
            llvm::Constant *_modexp = F.getParent()->getOrInsertFunction("_modexp", fccType);

            // encryptedRhs = pow(n, e, rhs)
            llvm::APInt n, e;
            llvm::APInt encryptedRhs = MakeRandomKey(n, e, rhs->getValue());

            // encryptedLhs = _modexp(lhs, e, n)
            std::vector<llvm::Value*> concreteArgs;
            concreteArgs.push_back(lhs);
            concreteArgs.push_back(llvm::ConstantInt::get(opType, e));
            concreteArgs.push_back(llvm::ConstantInt::get(opType, n));
            llvm::Value* encryptedLhs = builder.CreateCall(fccType, _modexp, concreteArgs);

            // Replace the original comparison with the encrypted one
            llvm::Value *newCmp = builder.CreateICmpEQ(
                encryptedLhs,
                llvm::ConstantInt::get(opType, encryptedRhs)
            );
            op->replaceAllUsesWith(newCmp);

            modified = true;
          }
        }
      }

      return modified;
    }
  };
} // namespace

char DumboPass::ID = 0;

static llvm::RegisterPass<DumboPass> X(
  "dumbo",                  // Command line argument
  "Dumb Obfuscator Pass",   // Command line desciption
  false,                    // Only looks at CFG
  false                     // Analysis Pass
);


#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
  struct DumboPass : public FunctionPass {
    static char ID;
    DumboPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
      errs() << "I saw a function called " << F.getName() << "!\n";
      return false;
    }
  };
}

char DumboPass::ID = 0;

static RegisterPass<DumboPass> X(
  "dumbo",                  // Command line argument
  "Dumb Obfuscator Pass",   // Command line desciption
  false,                    // Only looks at CFG
  false                     // Analysis Pass
);


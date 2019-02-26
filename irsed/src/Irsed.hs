module Irsed where

import Data.Generics
import Data.Word
import qualified LLVM.AST as A
import LLVM.AST.Constant as AC
import LLVM.AST.Type
import LLVM.Context
import LLVM.Module

{-
babush: this would be nicer if llvm-hs would use Type for ConstantInt.
        Unfortunately it does not, so we need to differentiate between
        constant and non-constant integers.
        I've read SYB some years ago so I don't remember much T.T
        Anyway this works and is probably kinda slow.
-}

widenIntFromTo      :: Word32 -> Word32 -> A.Global -> A.Global
widenIntFromTo m n = everywhere (mkT replace) where
    replace x@(IntegerType bits)    = if bits == m then IntegerType n else x
    replace x                       = x

widenConstIntFromTo :: Word32 -> Word32 -> A.Global -> A.Global
widenConstIntFromTo m n = everywhere (mkT replace) where
    replace x@(A.ConstantOperand (AC.Int bits val)) = if bits == m then A.ConstantOperand (AC.Int (fromIntegral n) val) else x
    replace x                                       = x

widenAllIntsFromTo  :: Word32 -> Word32 -> A.Global -> A.Global
widenAllIntsFromTo m n = widenConstIntFromTo m n . widenIntFromTo m n

-- Don't widen global metadata stuff we don't care about
widenModuleFromTo   :: Word32 -> Word32 -> A.Module -> A.Module
widenModuleFromTo m n = everywhere (mkT replace) where
    replace x@A.Function{}          = widenAllIntsFromTo m n x
    replace x@A.GlobalVariable{}    = widenAllIntsFromTo m n x
    replace x                       = x
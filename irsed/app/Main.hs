module Main where

import qualified Data.Text.Lazy.IO as T
import qualified LLVM.AST as A
import LLVM.Context
import LLVM.Module
import LLVM.Pretty (ppllvm)
import System.Environment

import Irsed


widenModule :: A.Module -> A.Module
widenModule = widenModuleFromTo 32 1024

main :: IO ()
main = do
    args <- getArgs
    if args == [] then do
        prog <- getProgName
        putStrLn $ "Usage: " ++ prog ++ " <file.ll>"
    else do
        mod  <- withContext $
                    \ctx -> withModuleFromLLVMAssembly ctx (File $ args !! 0) $ fmap widenModule . moduleAST
        T.putStrLn . ppllvm $ mod
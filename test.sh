#!/bin/sh

cd test
echo "====> Compiling LLVM IR..."
clang-9 -S -emit-llvm test.c
echo "====> Obfuscated LLVM IR:"
opt-9 -load=../build/dumbo/DumboPass.so --dumbo ./test.ll -S -o test_obfuscated.ll
cat test_obfuscated.ll
echo "====> Compiling final executable..."
clang-9 -flto=full -march=native -O3 -Wl,-strip-all -o test test_obfuscated.ll ../helper/modexp_wide.ll
echo "====> Running final executable..."
./test


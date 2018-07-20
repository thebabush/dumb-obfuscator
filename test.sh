#!/bin/sh

cd test
../llvm_build/bin/clang -S -emit-llvm test.c
../llvm_build/bin/opt -load=../build/dumbo/DumboPass.so --dumbo ./test.ll -S


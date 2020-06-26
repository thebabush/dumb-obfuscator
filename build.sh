#!/bin/sh

mkdir -p build
cd build

cmake ../
echo "====> Making the LLVM pass..."
cmake --build .

cd ..
cd helper
echo "====> Making the helper..."
clang-9 -S -emit-llvm -o modexp.ll modexp.c
echo "====> Extending the helper from i32 to i1024..."
sed 's/i32/i1024/g' modexp.ll > modexp_wide.ll
echo "====> Fixing wide char stuff..."
sed -i '/!0/d' modexp_wide.ll


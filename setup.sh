#!/bin/sh

echo -n "Downloading LLVM..."
wget "http://releases.llvm.org/6.0.1/llvm-6.0.1.src.tar.xz"
tar -xf llvm-6.0.1.src.tar.xz
echo " OK"

echo -n "Downloading clang..."
wget "http://releases.llvm.org/6.0.1/cfe-6.0.1.src.tar.xz"
tar -xf cfe-6.0.1.src.tar.xz
echo " OK"

echo -n "Preparing for LLVM build..."
mv llvm-6.0.1.src llvm_src
mv cfe-6.0.1.src llvm_src/tools/clang

mkdir -p llvm_build
cd llvm_build
echo " OK"

echo "Running CMake..."
cmake ../llvm_src/ -DLLVM_TARGETS_TO_BUILD="X86" -DLLVM_INCLUDE_EXAMPLES=OFF -DLLVM_INCLUDE_TESTS=OFF
echo "Building LLVM, please drink some Prosecco in the mean time or make -j 8 if you have enough RAM..."
cmake --build .

echo "-----------------------------------------------------"
echo ""
echo ""
echo "If nothing failed so far, you should be good to go :)"
echo "Try ./build.sh and then ./test.sh"


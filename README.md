
## At a glance ##

A step-by-step tutorial for building an out-of-source LLVM pass based on Adrian Sampson's "LLVM for Grad Students"

## Setup ##

LLVM is an umbrella project for building compilers
and code transformation tools. We consider in this tutorial:
- Building LLVM from source
- Building a trivial out-of-source LLVM pass.

We will be building LLVM v`6.0.1` which is the latest as of this writing.
We assume that you have a working compiler toolchain (GCC or LLVM) and that CMake is installed (minimum version 3.4.3).
We also assume that you are working un a Linux system.

With a working Internet connection, run `./setup.sh` and pray the Linux Gods
that nothing fails.
This will download LLVM and clang and then compile them.

## Building the Pass ##

Run `./build.sh`.

This will basically create `./build/` and run cmake on it.

## Testing the Pass ##

Run `./test.sh`.

This will compile `./test/test.c` into human-readable LLVM IR, run the pass on
it and show the final IR.

### Further resources

This tutorial is based on the following resources

- Adrian Sampson's blog entry "LLVM for Grad Students" ([link](http://adriansampson.net/blog/llvm.html))
- LLVM documentation: Writing an LLVM pass ([link](http://llvm.org/docs/WritingAnLLVMPass.html))
- LLVM documentation: Building LLVM with CMake ([link](http://llvm.org/docs/CMake.html#cmake-out-of-source-pass))

# dumb-obfuscator

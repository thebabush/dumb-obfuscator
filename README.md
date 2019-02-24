# Dumb Obfuscator

The answer to "why shouldn't people put RSA computations everywhere in their programs?".

If you want to read the step-by-step tutorial you can read the
[post on my blog](https://thebabush.github.io/dumbo-llvm-based-dumb-obfuscator.html).

## Setup ##

You should really read my blog post if you want this to work out of the box.
Basically though, you just need to add LLVM's official apt repositories to your
Ubuntu machine and install `clang-7`.

Anyway, you can pretty much change LLVM version by changing every reference to
`clang-7` to whatever clang you want to use.
You need to change `llvm-config-7` references too.

## Building the Pass ##

Run `./build.sh`.

This will basically create `./build/` and run cmake on it.

## Testing the Pass ##

Run `./test.sh`.

This will compile `./test/test.c` into human-readable LLVM IR, run the pass on
it, show the final IR and run the obfuscated program.


#!/bin/bash

stack run ../helper/modexp.ll | tee >(cat 1>&2) | clang-7 -x ir -c -o /dev/null -


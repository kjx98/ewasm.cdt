#! /bin/bash

clang-format --version
find libc include rt examples -name '*.hpp' -o -name '*.cpp' -o -name '*.h' -o -name '*.c' | xargs clang-format -i

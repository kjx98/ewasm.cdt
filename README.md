# ewasm.cdt
C/C++ development toolkit for EWASM

[![Build Status](https://travis-ci.org/kjx98/ewasm.cdt.svg?branch=master)](
https://travis-ci.org/kjx98/ewasm.cdt)

## Quick start
### download and build, requires clang/llvm 8+
*	git clone https://github.com/kjx98/ewasm.cdt
*	cd ewasm.cdt
*	mkdir build
*	cd build
*	cmake -DCMAKE_INSTALL_PREFIX=${HOME}/opt ..
*	make
*	make install

* ewasm CDT installed under ~/opt/ewasm
* ~/bin/bldewasm.sh is sample compile and link script for C/C++

### tested OS
*	fedora 31+
*	ubuntu 18.04+

### rt library
* support __int128, multiply/divide/mod/shift etc
* only support integer (NO float yet!)
* only malloc/memcpy/memset/memcmp/strlen builtins

### libc library
* Work In Progress
*	bldewasm.sh is a sample compile/link script
*	C/C++ without standard library shoud work, C++ template and clang builtins should work
* TODO string/exit of libc

## Examples
*	testfib.cpp  example C++ smart contract calc fibonacci number
*	testO.c example C smart contract

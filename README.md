# ewasm.cdt
C/C++ development toolkit for EWASM

[![Build Status](https://travis-ci.org/kjx98/ewasm.cdt.svg?branch=master)](
https://travis-ci.org/kjx98/ewasm.cdt)

## Quick start
### download and build
*	git clone https://github.com/kjx98/ewasm.cdt
*	cd ewasm.cdt
*	mkdir build
*	cd build
*	cmake -DCMAKE_INSTALL_PREFIX=${HOME}/opt ..
*	make
*	make install

* ewasm CDT installed under ~/opt/ewasm
* ~/bin/bldewasm.sh is sample compile and link script for C/C++

### rt library
* only support integer (NO float yet!)
* only memcpy/memset/memcmp/strlen builtins

### libc library
Work in progress
C/C++ no use functions from standard library shoud work
*TODO string/exit of libc

## Examples
*	testfib.cpp  example C++ smart contract calc fibonacci number
*	testO.c example C smart contract

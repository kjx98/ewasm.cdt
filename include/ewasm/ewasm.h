/*
    Copyright 2020 Jesse Kuang <jkuang@21cn.com>

    This file is part of ewasm.cdt.

    ewasm.cdt is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ewasm.cdt is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ewasm.cdt.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once

#ifndef __EWASM_H__
#define __EWASM_H__

#include <stddef.h>
#include <stdint.h>

#ifdef	__cplusplus
using uint128_t = unsigned __int128;
using int128_t	= __int128;
using byte = uint8_t;
#else
typedef unsigned __int128 uint128_t;
typedef __int128	int128_t;
typedef uint8_t		byte;
#endif

#ifdef	__GNUC__
#define forceinline __inline__ __attribute__((always_inline))
#else
#define forceinline __inline__
#endif

//////////////////////////
// Types For Wasm Stuff //
//////////////////////////

typedef int32_t i32; // same as i32 in WebAssembly
typedef int64_t i64; // same as i64 in WebAssembly


//////////////////////////////
// Types for Ethereum Stuff //
//////////////////////////////

//typedef uint8_t* bytes; // an array of bytes with unrestricted length
typedef uint8_t bytes32[32]; // an array of 32 bytes
typedef struct	eth_bytes32
{
	uint8_t	bytes[32];
} eth_bytes32;
typedef struct eth_bytes32	eth_uint256be;
typedef struct	eth_address
{
	uint8_t bytes[20]; // an array of 20 bytes
} eth_address;
typedef uint32_t u32;
typedef uint64_t u64;
typedef unsigned __int128 u128; // a 128 bit number, represented as a 16 bytes long little endian unsigned integer in memory, not sure if this works
//typedef uint256_t u256; // a 256 bit number, represented as a 32 bytes long little endian unsigned integer in memory, doesn't work
typedef uint32_t i32ptr; // same as i32 in WebAssembly, but treated as a pointer to a WebAssembly memory offset
// ethereum interface functions

// builtin and host interface function as C declarations
#ifdef	__cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif
// define for memory op, no string.h with clang wasm32 target
void *memcpy(void *dts, const void *src, size_t);
void *memmove(void *dts, const void *src, size_t);
void *memset(void *s, int c, size_t);
int	memcmp(const void *, const void *, size_t);
size_t  strlen(const char *);

void *malloc(size_t);
void free(void *);
void *realloc(void *, size_t);

// bswap32, bswap64 already builtin
forceinline uint128_t bswap128(uint128_t ml) {
	uint128_t ret;
	u64	*ss=(u64 *)&ml;
	u64	*dp=(u64 *)&ret;
	dp[0] = __builtin_bswap64(ss[1]);
	dp[1] = __builtin_bswap64(ss[0]);
	return ret;
}

////////////////////////////
// EEI Method Declaration //
////////////////////////////
#define DECL_IMPORT(name, args) eth_##name args \
    __attribute__((import_module("ethereum"),import_name( #name )))

void DECL_IMPORT(useGas, (i64 gas));
void DECL_IMPORT(getAddress, (eth_address* res));
void DECL_IMPORT(getExternalBalance, (eth_address* acct, u128 *bal));
void DECL_IMPORT(storageStore, (bytes32 key, bytes32 value));
void DECL_IMPORT(storageLoad, (bytes32 key, bytes32 value));
void DECL_IMPORT(getCaller, (eth_address* acct));
void DECL_IMPORT(getCallValue, (u128 *val));
void DECL_IMPORT(getTxOrigin, (eth_address* acct));

u32	DECL_IMPORT(getCallDataSize, () );
void DECL_IMPORT(callDataCopy, (void *res, u32 dOff, u32 dLen));

void DECL_IMPORT(codeCopy, (void *res, u32 codeOff, u32 codeLen));
u32 DECL_IMPORT(getCodeSize, () );

u64 DECL_IMPORT(getGasLeft, () );
u64 DECL_IMPORT(getBlockGasLimit, () );
void DECL_IMPORT(getTxGasPrice, (u128 *prc));
u64 DECL_IMPORT(getBlockNumber, () );
u64 DECL_IMPORT(getBlockTimestamp, () );

void DECL_IMPORT(log, (void* dat, u32 dLen, u32 numTopics, bytes32 to1, bytes32 to2, bytes32 to3, bytes32 to4));

void DECL_IMPORT(finish, (void* _off, u32 _len));
void DECL_IMPORT(revert, (void* _off, u32 _len));
void DECL_IMPORT(selfDestruct, (eth_address* selfAddr));


///////////////////////////////////////////////////
// Useful Intrinsics, Not Including Memory Stuff //
///////////////////////////////////////////////////

extern void __builtin_unreachable();			// wasm unreachable opcode
extern int __builtin_ctz(unsigned int); 		// wasm i32.ctz opcode
extern int __builtin_ctzll(unsigned long long); 	// wasm i64.ctz opcode
// there are many more like this

void forceinline exit(int i){ __builtin_unreachable(); }

// follow should move to internal of memory.c
////////////////////////////
// Memory Managment Stuff //
////////////////////////////

extern unsigned char __heap_base;	// heap_base is immutable position where their model of heap grows down from, can ignore
extern unsigned char __data_end;	// data_end is immutable position in memory up to where statically allocated things are
extern unsigned long __builtin_wasm_memory_grow(int, unsigned long);	// first arg is mem idx 0, second arg is pages
extern unsigned long __builtin_wasm_memory_size(int);	// arg must be zero until more memory instances are available

// sample uses:
//  unsigned char* heap_base = &__heap_base;
//  unsigned char* data_end = &__data_end;
//  unsigned int memory_size = __builtin_wasm_memory_size(0);
#ifdef	__cplusplus
}
#endif

// ethereum ABI
#ifdef	__cplusplus
class	bytes {
public:
	bytes() = default;
	bytes(const bytes&) = default;
	bytes(void *dp, size_t siz) : _data((uint8_t *)dp), _size(siz) {}
	bool operator==(const bytes &bs) const noexcept {
		if (_size != bs._size) return false;
		return memcmp(_data, bs._data, _size) == 0;
	}
	const size_t size() const noexcept { return _size; }
	uint8_t *data() const noexcept { return _data;}
private:
	uint8_t	*_data=nullptr;
	uint32_t	_size=0;
};

using string = bytes;
#endif

enum	eth_argType {
	UINT32 = 0,
	UINT64 = 1,
	UINT128= 2,
	UINT256= 3,
	INT32  = 4,
	INT64  = 5,
	INT128 = 6,
	INT256 = 7,
	UINT160= 8,
	STRING = 9,
	BYTES  = 10,
};

typedef struct eth_argument
{
	i32		_type;
	void	*pValue;	// for int/uint 128, 256 and bytes
	u64		_nValue;
}	eth_argument;

typedef struct eth_method
{
	char	*Name;	// name of method
	uint32_t	Id;		// uint32be ID of method, 0 for Constructor
	int		nParams;
	int		nResults;
	eth_argument	*inputs;
	eth_argument	*outputs;
}	eth_method;

typedef struct eth_ABI
{
	int		nMethods;	// >0, at least constructor
	eth_method	*methods;	// the first method MUST BE constructor
}	eth_ABI;

// ethereum ABI
#ifdef	__cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif
//extern u32 getCallMethodID();
//extern int decodeParam(eth_argument *, int);
extern	eth_ABI	__Contract_ABI;
extern int encodeResult(eth_argument *, int);

#ifdef	__cplusplus
}
#endif


///////////
// Other //
///////////

/*
Below are some things which you can use to give LLVM hints to do certain things.

__attribute__((import_module("ethereum")))	- work after llvm 8
__attribute__((import_name("funcname")))	- work after llvm 8
__attribute__((visibility("default"))) 		- make function exported
__attribute__((visibility("hidden")))		- make thing not exported
__attribute__((visibility("used")))		- make variable const global and exported
__attribute__((noinline))			- don't inline this function, since llvm tries to inline it since code size is less important for LLVM
__attribute__((always_inline))
__attribute__((noreturn))

e.g. create a global variable (but it will be immutable, don't know how to create a mutable one):
   __attribute__((used))
   static int heap_ptr=0;

*/


//
// for testing, can export start fuction
//void _start(){  _main(); }

#endif

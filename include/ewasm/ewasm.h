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

#if	__clang_major__ > 8
#include <stddef.h>
#include <stdint.h>
#else
typedef	unsigned char	uint8_t;
typedef	unsigned short	uint16_t;
typedef	unsigned int	uint32_t;
typedef	unsigned long long	uint64_t;
typedef	unsigned long	size_t;
typedef	short	int16_t;
typedef	int	int32_t;
typedef	long long	int64_t;
#endif

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
//typedef uint8_t bytes32[32]; // an array of 32 bytes
typedef struct	ewasm_bytes32
{
	uint8_t	bytes[32];
} ewasm_bytes32;
typedef struct ewasm_bytes32	ewasm_uint256be;
typedef struct ewasm_address
{
	uint8_t bytes[20]; // an array of 20 bytes
} ewasm_address;
typedef struct ewasm_bytes
{
	void	*_data;
	uint32_t	_size;
} ewasm_bytes;
typedef uint32_t u32;
typedef uint64_t u64;
typedef unsigned __int128 u128; // a 128 bit number, represented as a 16 bytes long little endian unsigned integer in memory, not sure if this works
//typedef uint256_t u256; // a 256 bit number, represented as a 32 bytes long little endian unsigned integer in memory, doesn't work
typedef uint32_t i32ptr; // same as i32 in WebAssembly, but treated as a pointer to a WebAssembly memory offset

// builtin and host interface function as C declarations
#ifdef	__cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif
// define for memory op, no string.h with clang wasm32 target
//void *memmove(void *dts, const void *src, size_t);
void *memcpy(void *dst, const void *src, size_t);
void *memrcpy(void *dst, const void *src, size_t);
void *memset(void *s, int c, size_t);
int	memcmp(const void *, const void *, size_t);
size_t strlen(const char *);

void *malloc(size_t);
void *calloc(size_t count, size_t size);
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

// ethereum interface functions
////////////////////////////
// EEI Method Declaration //
////////////////////////////
#define DECL_IMPORT(name, args) eth_##name args \
    __attribute__((import_module("ethereum"),import_name( #name )))
#define DEBUG_IMPORT(name, args) debug_##name args \
    __attribute__((import_module("debug"),import_name( #name )))

#ifdef	__cplusplus
namespace	ewasm {
struct	address;
struct	bytes32;
};
using	namespace ewasm;
void DECL_IMPORT(getAddress, (address* res));
void DECL_IMPORT(getExternalBalance, (address* acct, u128 *bal));
void DECL_IMPORT(storageStore, (bytes32* key, bytes32* value));
void DECL_IMPORT(storageLoad, (bytes32* key, bytes32* value));
void DECL_IMPORT(getCaller, (address* acct));
void DECL_IMPORT(getTxOrigin, (address* acct));
void DECL_IMPORT(log, (void* dat, u32 dLen, u32 numTopics, bytes32* to1, bytes32* to2, bytes32* to3, bytes32* to4));
void DECL_IMPORT(selfDestruct, (address* selfAddr));
#ifndef	NDEBUG
void DEBUG_IMPORT(printStorage, (bytes32 *));
void DEBUG_IMPORT(printStorageHex, (bytes32 *));
#endif
#else
void DECL_IMPORT(getAddress, (void* res));
void DECL_IMPORT(getExternalBalance, (void* acct, u128 *bal));
void DECL_IMPORT(storageStore, (void* key, void* value));
void DECL_IMPORT(storageLoad, (void* key, void* value));
void DECL_IMPORT(getCaller, (void* acct));
void DECL_IMPORT(getTxOrigin, (void* acct));
void DECL_IMPORT(log, (void* dat, u32 dLen, u32 numTopics, void* to1, void* to2, void* to3, void* to4));
void DECL_IMPORT(selfDestruct, (void* selfAddr));
#ifndef	NDEBUG
void DEBUG_IMPORT(printStorage, (void *));
void DEBUG_IMPORT(printStorageHex, (void *));
#endif
#endif

#ifndef	NDEBUG
void DEBUG_IMPORT(print, (void*, u32));
void DEBUG_IMPORT(printMem, (void*, u32));
void DEBUG_IMPORT(printMemHex, (void*, u32));
#else
#define	debug_print(x, y)	(void *)0
#define	debug_printMem(x, y)	(void *)0
#define	debug_printMemHex(x, y)	(void *)0
#define	debug_printStorage(x)	(void *)0
#define	debug_printStorageHex(x)	(void *)0
#endif

void DECL_IMPORT(useGas, (i64 gas));
void DECL_IMPORT(getCallValue, (u128 *val));
u32	DECL_IMPORT(getCallDataSize, () );
void DECL_IMPORT(callDataCopy, (void *res, u32 dOff, u32 dLen));

void DECL_IMPORT(codeCopy, (void *res, u32 codeOff, u32 codeLen));
u32 DECL_IMPORT(getCodeSize, () );

u64 DECL_IMPORT(getGasLeft, () );
u64 DECL_IMPORT(getBlockGasLimit, () );
void DECL_IMPORT(getTxGasPrice, (u128 *prc));
u64 DECL_IMPORT(getBlockNumber, () );
u64 DECL_IMPORT(getBlockTimestamp, () );

void DECL_IMPORT(finish, (void* _off, u32 _len));
void DECL_IMPORT(revert, (void* _off, u32 _len));

#define	ewasm_print(x) debug_print(x, strlen(x))

///////////////////////////////////////////////////
// Useful Intrinsics, Not Including Memory Stuff //
///////////////////////////////////////////////////

extern void __builtin_unreachable();			// wasm unreachable opcode
extern int __builtin_ctz(unsigned int); 		// wasm i32.ctz opcode
extern int __builtin_ctzll(unsigned long long); 	// wasm i64.ctz opcode
// there are many more like this

void forceinline exit(int i){ __builtin_unreachable(); }

#ifdef	__cplusplus
}
#endif

// ethereum ABI
enum	ewasm_argType {
	UINT16	= 0,
	UINT32	= 1,
	UINT64	= 2,
	BOOL	= 3,
	INT16	= 4,
	INT32	= 5,
	INT64	= 6,
	UINT128	= 7,
	UINT256	= 8,
	INT128	= 9,
	INT256	= 10,
	UINT160	= 11,
	STRING	= 12,
	BYTES	= 13,
};

typedef struct ewasm_argument
{
	i32		_type;
	ewasm_bytes	pValue;	// for int/uint 128, 256 and bytes
	u64		_nValue;
}	ewasm_argument;

typedef struct ewasm_method
{
	char	*Name;	// name of method
	u32		Id;		// uint32be ID of method, 0 for Constructor
	int		nParams;
	int		nResults;
	ewasm_argument	*inputs;
	ewasm_argument	*outputs;
}	ewasm_method;

typedef struct ewasm_ABI
{
	uint32_t		nMethods;	// >0, at least constructor
	ewasm_method	*methods;	// the first method MUST BE constructor
}	ewasm_ABI;

#define	DECL_ABI(x) { sizeof(x)/sizeof(x[0]), x }

// ethereum ABI
#ifdef	__cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif
//extern u32 getCallMethodID();
//extern int decodeParam(ewasm_argument *, int);
extern	ewasm_ABI	__Contract_ABI;
extern	__attribute__((weak))
void ewasm_main(const u32 Id, const ewasm_method *);
extern int encodeResult(ewasm_argument *, const int);

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

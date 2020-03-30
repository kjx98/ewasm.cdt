#pragma once
#ifndef _BYTESWAP_H
#define _BYTESWAP_H

#include <stdint.h>

#ifdef __GNUC__
#define __bswap_16 __builtin_bswap16
#define __bswap_32 __builtin_bswap32
#define __bswap_64 __builtin_bswap64
#else
static __inline uint16_t __bswap_16(uint16_t __x) {
  return (__x << 8) | (__x >> 8);
}

static __inline uint32_t __bswap_32(uint32_t __x) {
  return __x >> 24 | ((__x >> 8) & 0xff00) | ((__x << 8) & 0xff0000) |
         __x << 24;
}

static __inline uint64_t __bswap_64(uint64_t __x) {
  return (uint64_t)(__bswap_32(__x)) << 32 | __bswap_32(__x >> 32);
}
#endif

#define bswap_16(x) __bswap_16(x)
#define bswap_32(x) __bswap_32(x)
#define bswap_64(x) __bswap_64(x)

#endif

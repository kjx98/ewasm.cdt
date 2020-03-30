#pragma once
#include <stdint.h>

using uint128_t = unsigned __int128;
using int128_t = __int128;

extern "C" {
__int128 ___fixdfti(uint64_t);
__int128 ___fixsfti(uint32_t);
unsigned __int128 ___fixunsdfti(uint64_t);
unsigned __int128 ___fixunssfti(uint32_t);
}

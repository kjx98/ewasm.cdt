#include "compiler_builtins.hpp"

extern "C" {
void __lshlti3(__int128& ret, uint64_t low, uint64_t high, uint32_t shift) {
   uint128_t i = high;
   i <<= 64;
   i |= low;
   i <<= shift;
   ret = (unsigned __int128)i;
}

void __lshrti3(__int128& ret, uint64_t low, uint64_t high, uint32_t shift) {
   uint128_t i = high;
   i <<= 64;
   i |= low;
   i >>= shift;
   ret = (unsigned __int128)i;
}

}

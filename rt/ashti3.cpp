#include "compiler_builtins.hpp"

extern "C" {
void __ashlti3(__int128& ret, uint64_t low, uint64_t high, uint32_t shift) {
   uint128_t i = high;
   i <<= 64;
   i |= low;
   i <<= shift;
   ret = i;
}

void __ashrti3(__int128& ret, uint64_t low, uint64_t high, uint32_t shift) {
   // retain the signedness
   ret = high;
   ret <<= 64;
   ret |= low;
   ret >>= shift;
}

}

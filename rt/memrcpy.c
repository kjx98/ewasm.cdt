#include <ewasm/ewasm.h>

__attribute__ ((noinline))
void* memrcpy(void* __restrict destination, const void* __restrict source, size_t len) {
  uint8_t* destination_ptr = (uint8_t*) destination;
  uint8_t* source_ptr = (uint8_t*) source +(len-1);
  // copy rest 1-byte at a time
  while (len-- > 0)
    *destination_ptr++ = *source_ptr--;
  return destination;
}

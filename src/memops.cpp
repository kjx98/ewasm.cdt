#include "ewasm.hpp"

// already builtin
#ifdef	ommit
__attribute__ ((noinline)) extern "C"
void* memcpy(void* restrict destination, const void* restrict source, size_t len) {
  uint8_t* destination_ptr = (uint8_t*) destination;
  uint8_t* source_ptr = (uint8_t*) source;
  // copy 8-byte chunks
  while (len > 7) {
    *(uint64_t*)destination_ptr = *(uint64_t*)source_ptr;
    destination_ptr+=8;
    source_ptr+=8;
    len-=8;
  }
  // copy rest 1-byte at a time
  while (len-- > 0)
    *destination_ptr++ = *source_ptr++;
  return destination;
}

__attribute__ ((noinline)) extern "C"
void* memset(void* restrict in, int c, size_t len) {
  uint8_t* in_ptr = (uint8_t*)in;
  if (len>8){
    // set in 8-byte chunks
    uint64_t c64 = c + (c<<8) + (c<<16) + (c<<24) + (((uint64_t)c)<<32) + (((uint64_t)c)<<40) + (((uint64_t)c)<<56);
    while (len > 7) {
      *(uint64_t*)in_ptr = c64;
      in_ptr+=8;
      len-=8;
    }
  }
  // set rest 1-byte at a time
  while (len-- > 0) {
    *in_ptr++ = c;
  }
  return in_ptr;
}
#endif

//__attribute__ ((noinline))
extern "C" int memcmp ( const void * in1, const void * in2, size_t num ){
  // TODO: do this in 64-bit chunks, then 8-bit chunks at end
  uint8_t* in1_ptr = (uint8_t*) in1;
  uint8_t* in2_ptr = (uint8_t*) in2;
  int ret=0;
  for (int i=0;i<num;++i){
    if (in1_ptr[i]!=in2_ptr[i]){
		ret = (in1_ptr[i] > in2_ptr[i])?1:-1;
		break;
    }
  }
  return ret;
}

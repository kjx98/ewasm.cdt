#include <ewasm/ewasm.hpp>

//__attribute__ ((noinline))
extern "C" int memcmp ( const void * in1, const void * in2, size_t num )
{
  // TODO: do this in 64-bit chunks, then 8-bit chunks at end
  uint8_t* in1_ptr = (uint8_t*) in1;
  uint8_t* in2_ptr = (uint8_t*) in2;
  for (int i=0;i<num;++i,++in1_ptr,++in2_ptr) {
	if (*in1_ptr > *in2_ptr)
		return 1;
	else if (*in1_ptr < *in2_ptr)
		return -1;
  }
  return 0;
}

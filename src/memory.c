#include <stdint.h>
#include "ewasm.h"

#ifdef	not_work
void* malloc(const size_t size){
  //  Our malloc is naive: we only append to the end of the previous allocation, starting at data_end. This is tuned for short runtimes where memory management cost is expensive, not many things are allocated, or not many things are freed.
  //  It seems (in May 2019) that LLVM->Wasm starts data_end at 1024 plus anything that is statically stored in memory at compile-time. So our heap starts at data_end and grows upwards.
  // LLVM backend uses the following conventions:
  //   - Static memory is in byte 0 through __data_end.
  //   - There is a "shadow stack" which "shadows" function calls and stores local variables if there are too many
  //     This shadow stack grows down from __heap_base
  //   - The optional heap grows up from __heap_base, may eventually need memory.grow
  //   - LLVM stores these memory offsets as globals. For example:
  //        (global (;0;) (mut i32) (i32.const 66576))	;; current heap pointer
  //        (global (;1;) i32 (i32.const 66576))	;; __heap_base
  //        (global (;2;) i32 (i32.const 1032))		;; static memory from byte 0 through 1032
  //        (export "__heap_base" (global 1))		;; allow memory management externally
  //        (export "__data_end" (global 2))		;; allow memory management externally
  // WARNING: There is a bug. The shadow stack has a limit, and can collide with static memory. It may to precompute the maximum shadow stack size needed, and give it to `wasm-ld` with `-z stack-size=10000`.
  // One last thing. `wasm-ld` will take argument `--stack-first` to put the shadow stack before the static memory.

  // this heap pointer starts at heap_base and always increments upward
  static uint8_t* heap_ptr = &__heap_base;

  uint32_t total_bytes_needed = ((uint32_t)heap_ptr)+size;
  // check whether we have enough memory, and handle if we don't
  if (total_bytes_needed > __builtin_wasm_memory_size(0)*PAGE_SIZE){ // if exceed current memory size
    #if GROWABLE_MEMORY==true
      uint32_t total_pages_needed = (total_bytes_needed/PAGE_SIZE) + ((total_bytes_needed%PAGE_SIZE)?1:0);
      __builtin_wasm_memory_grow(0, total_pages_needed - __builtin_wasm_memory_size(0));
      // note: memory grow is non-deterministic, could fail at different total memory for different implementations
    #else
      // for conciseness, we do nothing here, but if we access memory outside of bounds, then it will trap
    #endif
  }

  heap_ptr = (uint8_t*)total_bytes_needed;
  return (void*)(heap_ptr-size);
}
#endif

// already builtin
#ifdef	ommit
__attribute__ ((noinline))
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

__attribute__ ((noinline))
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
int memcmp ( const void * in1, const void * in2, size_t num ){
  // TODO: do this in 64-bit chunks, then 8-bit chunks at end
  uint8_t* in1_ptr = (uint8_t*) in1;
  uint8_t* in2_ptr = (uint8_t*) in2;
  int ret=0;
  for (int i=0;i<num;++i){
    if (in1_ptr[i]!=in2_ptr[i]){
		ret = (in1_ptr[i] - in2_ptr[i]);
      break;
    }
  }
  return ret;
}

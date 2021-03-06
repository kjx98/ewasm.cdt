#include <stdarg.h>
#include <stdbool.h>
//#include <alloca.h>
#include <assert.h>
#include <byteswap.h>
#include <endian.h>
#include <ewasm/ewasm.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>

#define PAGE_SIZE 65536
#define GROWABLE_MEMORY                                                        \
  true // whether we want memory to be growable; true/false

// follow should move to internal of memory.c
////////////////////////////
// Memory Managment Stuff //
////////////////////////////

extern unsigned char
    *__heap_base; // heap_base is immutable position where their model of heap
                  // grows down from, can ignore
extern unsigned char *__data_end; // data_end is immutable position in memory up
                                  // to where statically allocated things are
extern unsigned long __builtin_wasm_memory_grow(
    int, unsigned long); // first arg is mem idx 0, second arg is pages
extern unsigned long __builtin_wasm_memory_size(
    int); // arg must be zero until more memory instances are available
static uint8_t *heap_ptr;

void malloc_init() {
  // __heap_base is mutable constant, point to heap base
  // this heap pointer starts at heap_base and always increments upward
  heap_ptr = (uint8_t *)__heap_base;
}

// sample uses:
//  unsigned char* heap_base = &__heap_base;
//  unsigned char* data_end = &__data_end;
//  unsigned int memory_size = __builtin_wasm_memory_size(0);
void *malloc(const size_t size) {
  //  Our malloc is naive: we only append to the end of the previous allocation,
  //  starting at data_end. This is tuned for short runtimes where memory
  //  management cost is expensive, not many things are allocated, or not many
  //  things are freed. It seems (in May 2019) that LLVM->Wasm starts data_end
  //  at 1024 plus anything that is statically stored in memory at compile-time.
  //  So our heap starts at data_end and grows upwards.
  // LLVM backend uses the following conventions:
  //   - Static memory is in byte 0 through __data_end.
  //   - There is a "shadow stack" which "shadows" function calls and stores
  //   local variables if there are too many
  //     This shadow stack grows down from __heap_base
  //   - The optional heap grows up from __heap_base, may eventually need
  //   memory.grow
  //   - LLVM stores these memory offsets as globals. For example:
  //        (global (;0;) (mut i32) (i32.const 66576))	;; current heap pointer
  //        (global (;1;) i32 (i32.const 66576))	;; __heap_base
  //        (global (;2;) i32 (i32.const 1032))		;; static memory from
  //        byte 0 through 1032 (export "__heap_base" (global 1))
  //        ;; allow memory management externally (export "__data_end" (global
  //        2))		;; allow memory management externally
  // WARNING: There is a bug. The shadow stack has a limit, and can collide with
  // static memory. It may to precompute the maximum shadow stack size needed,
  // and give it to `wasm-ld` with `-z stack-size=10000`. One last thing.
  // `wasm-ld` will take argument `--stack-first` to put the shadow stack before
  // the static memory.

  uint32_t total_bytes_needed = ((uint32_t)heap_ptr) + size;
  // check whether we have enough memory, and handle if we don't
  if (total_bytes_needed > __builtin_wasm_memory_size(0) *
                               PAGE_SIZE) { // if exceed current memory size
#if GROWABLE_MEMORY == true
    uint32_t total_pages_needed = (total_bytes_needed / PAGE_SIZE) +
                                  ((total_bytes_needed % PAGE_SIZE) ? 1 : 0);
    __builtin_wasm_memory_grow(0, total_pages_needed -
                                      __builtin_wasm_memory_size(0));
    // note: memory grow is non-deterministic, could fail at different total
    // memory for different implementations
#else
    // for conciseness, we do nothing here, but if we access memory outside of
    // bounds, then it will trap
#endif
  }

  heap_ptr = (uint8_t *)total_bytes_needed;
  return (void *)(heap_ptr - size);
}

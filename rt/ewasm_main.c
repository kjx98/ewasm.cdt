#include <ewasm/ewasm.h>

#include "abi.h"
#include "ewasm_main.h"

#ifdef USE_MALLOC_ABI
byte *__abiBuff; //[MAX_ABI_INPUT];
#else
byte __abiBuff[MAX_ABI_INPUT];
#endif
static byte ret[32] = {0, 0, 0, 0, 0, 0, 0, 10};

#pragma clang diagnostic ignored "-Wmain-return-type"
// __attribute__((export_name("main")))
__attribute__((noreturn)) void main() {
  u32 in_len;
  // constructor with null input while no arguments
  // constructor called w/out method ID?
  u32 param_off = 0;
#ifdef USE_MALLOC_ABI
  __abiBuff = malloc(MAX_ABI_INPUT);
  assert(__abiBuff != nullptr);
#endif
  const ewasm_method *mtdPtr = __Contract_ABI.methods;
  if (((in_len = eth_getCallDataSize()) & 0x1f) == 0) {
    // Constructor
    if (__Contract_ABI.nMethods == 0) {
      // no Constructor, for special test
      eth_finish(ret, 8);
    }
    if (__Contract_ABI.methods == 0) {
      ewasm_print("WARN! no method in ABI");
      eth_revert(0, 0);
      // invalid, no Constructor
    }
  } else {
    param_off = 4; // Offset ID, 4 bytes
    if (in_len < param_off || (((in_len - param_off) & 0x1f) != 0)) {
      ewasm_print("WARN! input length invalid");
      eth_revert(0, 0);
      // invalid, param length
    }
    u32 met;
    eth_callDataCopy(&met, 0, 4);
    met = __builtin_bswap32(met);
    int i;
    ++mtdPtr; // sacn from 2nd, first is constructor
    for (i = 1; i < __Contract_ABI.nMethods; ++i, ++mtdPtr) {
      if (mtdPtr->Id == met)
        break;
    }
    if (i >= __Contract_ABI.nMethods) {
      ewasm_print("WARN! no found method with ID");
      debug_printMemHex(&met, 4);
      eth_revert(0, 0);
      // invalid, no such method
    }
  }
  // we should decode param here
  if (decodeParam(mtdPtr->inputs, mtdPtr->nParams, in_len)) {
    // decode inputs error
    eth_revert(0, 0);
    // invalid, not enough params
  }
  ewasm_main(mtdPtr->Id, mtdPtr);
  // we should encode result and eth_finish
  returnResult(mtdPtr->outputs, mtdPtr->nResults);
  __builtin_unreachable();
}

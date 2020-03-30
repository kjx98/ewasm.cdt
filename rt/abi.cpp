#include "abi.h"
#include "ewasm_main.h"
#include <assert.h>
#include <ewasm/ewasm.hpp>

//#include <type_traits>

//__attribute__((uninitialized))
#define paramBuff __abiBuff

extern "C" int decodeParam(ewasm_argument *args, u32 argc, u32 in_len) {
  u32 paramOff = 0;
  if (argc == 0)
    return 0;
  if ((in_len & 0x1f) != 0) {
    in_len -= 4; // strip medthod ID
    paramOff = 4;
  }
  if (in_len > MAX_ABI_INPUT)
    in_len = MAX_ABI_INPUT;
  u32 minLen = argc * 32;
  if (in_len < minLen) {
    eth_revert(0, 0);
    // invalid, not enough params
  }
  eth_callDataCopy(paramBuff, paramOff, in_len);
  u32 sliceOff = argc * 32;
  u32 prOff = 0;
  int i;
  for (i = 0; i < argc; ++i, prOff += 32) {
    i32 pType = args[i]._type;
    switch (pType) {
    case STRING:
    case BYTES: {
      // decode string, bytes
      u32 rOff = u32From256(paramBuff + prOff);
      if (rOff < sliceOff)
        return -1; // decode invalid offset
      u32 rLen = u32From256(paramBuff + rOff);
      rOff += 32;
      if (rOff + rLen > in_len)
        return -1; // decode invalid length
      args[i].pValue._data = paramBuff + rOff;
      args[i].pValue._size = rLen;
    } break;
    case UINT128:
    case INT128:
    case UINT256:
    case INT256:
      args[i].pValue._data = paramBuff + prOff;
      args[i].pValue._size = 32;
      break;
    case UINT160: // address
      args[i].pValue._data = paramBuff + prOff;
      args[i].pValue._size = 32;
      break;
    default:
      switch (pType) {
      case UINT64:
      case INT64:
        args[i]._nValue = u64From256(paramBuff + prOff);
        break;
      default:
        args[i]._nValue = u32From256(paramBuff + prOff);
      }
    }
  }
  return 0; // success
}

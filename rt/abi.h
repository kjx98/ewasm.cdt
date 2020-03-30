#pragma once

// ABI input/output codec
#ifdef __cplusplus
extern "C" {
#endif

int decodeParam(ewasm_argument *args, u32 argc, u32 in_len);
__attribute__((noreturn)) void returnResult(ewasm_argument *args, u32 nRet);
#ifdef __cplusplus
}
#endif

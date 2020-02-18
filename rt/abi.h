#pragma once

// ABI input/output codec

#ifdef	_cplusplus
extern "C" {
#endif
int decodeParam(ewasm_argument *args, int argc, u32 in_len);
__attribute__((noreturn)) void returnResult(ewasm_argument *args, int nRet);
#ifdef	_cplusplus
}
#endif

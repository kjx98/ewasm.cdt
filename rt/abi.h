#pragma once

// ABI input/output codec

#ifdef	_cplusplus
extern "C" {
#endif
int decodeParam(ewasm_argument *args, int argc, u32 in_len);
#ifdef	_cplusplus
void returnResult(ewasm_argument *args, int nRet);
}
#endif

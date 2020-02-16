#include <ewasm/ewasm.h>

static	byte	ret[32]={0,0,0,0, 0,0,0,10};
static	byte	paramBuff[512];
#pragma clang diagnostic ignored "-Wmain-return-type"
void main() // __attribute__((export_name("main")))
{
	u32	in_len;
	// constructor with null input while no arguments
	// constructor called w/out method ID?
	u32	param_off=0;
	ewasm_method*	mtdPtr = __Contract_ABI.methods;
	if ( ((in_len=eth_getCallDataSize())  & 0x1f) == 0 ) {
		// Constructor
		if (__Contract_ABI.nMethods == 0) {
			eth_finish(ret, 8);
		}
		if (__Contract_ABI.methods == 0) {
			eth_revert(0, 0);
			// invalid, no Constructor
		}
	} else {
		param_off = 4;	// Offset ID, 4 bytes
		if ( in_len < param_off || ( ((in_len-param_off)&0x1f) != 0 ) ) {
			eth_revert(0, 0);
			// invalid, param length
		}
		u32 	met;
		eth_callDataCopy(&met, 0, 4);
		met = __builtin_bswap32(met);
		u32	i;
		for(i=1; i<__Contract_ABI.nMethods; ++i, ++mtdPtr) {
			if (mtdPtr->Id == met) break;
		}
		if (i >= __Contract_ABI.nMethods) {
			eth_revert(0, 0);
			// invalid, no such method
		}
	}
	// we should decode param here
	u32	minLen = mtdPtr->nParams * 32;
	if (in_len < minLen) {
			eth_revert(0, 0);
			// invalid, not enough params
	}
	if (in_len > sizeof(paramBuff)+param_off) in_len = sizeof(paramBuff);
	eth_callDataCopy(paramBuff, param_off, in_len);
	ewasm_main(mtdPtr->Id, paramBuff, in_len, mtdPtr);
}

#include <ewasm/ewasm.h>

static	byte	ret[32]={0,0,0,0, 0,0,0,10};
static	byte	paramBuff[512];
static	int decodeParam(ewasm_argument *args, int argc, u32 in_len);

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
	in_len -= param_off;
	if (in_len > sizeof(paramBuff)+param_off) in_len = sizeof(paramBuff);
	u32	minLen = mtdPtr->nParams * 32;
	if (in_len < minLen) {
		eth_revert(0, 0);
		// invalid, not enough params
	}
	if (decodeParam(mtdPtr->inputs, mtdPtr->nParams, in_len)) {
		// decode inputs error
		eth_revert(0, 0);
		// invalid, not enough params
	}
	eth_callDataCopy(paramBuff, param_off, in_len);
	ewasm_main(mtdPtr->Id, mtdPtr);
}

static int decodeParam(ewasm_argument *args, int argc, u32 in_len)
{
	int	i;
	u32	sliceOff=argc * 32;
	u32	prOff=0;
	for (i=0; i<argc; ++i, prOff+= 32) {
		i32	pType = args[i]._type;
		switch (pType) {
		case STRING:
		case BYTES:
		{
			// decode string, bytes
			u32	rOff = u32From256(paramBuff + prOff);
			if (rOff < sliceOff)
				return -1;	// decode invalid offset
			u32 rLen = u32From256(paramBuff + rOff);
			rOff += 32;
			if (rOff + rLen > in_len)
				return -1;	// decode invalid length
			args[i].pValue._data = paramBuff+rOff;
			args[i].pValue._size = rLen;
		}
			break;
		case UINT128:
		case INT128:
			break;
		case UINT256:
		case INT256:
			break;
		case UINT160:	// address
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
	return 0;	// success
}

#include <ewasm/ewasm.hpp>
#include <assert.h>
#include "ewasm_main.h"
//#include <type_traits>

//__attribute__((uninitialized))
#define	paramBuff	__abiBuff

int decodeParam(ewasm_argument *args, int argc, u32 in_len)
{
	u32	paramOff=0;
	if (argc == 0) return 0;
	if ((in_len & 0x1f) != 0) {
		in_len -= 4;	// strip medthod ID
		paramOff = 4;
	}
	if (in_len > MAX_ABI_INPUT) in_len = MAX_ABI_INPUT;
	u32	minLen = argc * 32;
	if (in_len < minLen) {
		eth_revert(0, 0);
		// invalid, not enough params
	}
	eth_callDataCopy(paramBuff, paramOff, in_len);
	u32	sliceOff=argc * 32;
	u32	prOff=0;
	int	i;
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
		case UINT256:
		case INT256:
			args[i].pValue._data = paramBuff+prOff;
			args[i].pValue._size = 32;
			break;
		case UINT160:	// address
			args[i].pValue._data = paramBuff+prOff;
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
	return 0;	// success
}


//__attribute__((uninitialized))
#define	resBuff	__abiBuff

__attribute__((noreturn))
void returnResult(ewasm_argument *args, int nRet)
{
	if (nRet == 0) eth_finish(nullptr, 0);
	assert(args != nullptr);
	u32	resLen=0;
	u32	sliceOff=nRet * 32;
	u32	prOff=0;
	int	i;
	for (i=0; i<nRet; ++i, prOff+= 32) {
		i32	pType = args[i]._type;
		switch (pType) {
		case STRING:
		case BYTES:
		{
			// decode string, bytes
			u32To256(resBuff + prOff, sliceOff);
			u32 rLen = args[i].pValue._size;
			if (sliceOff +32 + rLen > MAX_ABI_INPUT)
				eth_revert(0, 0);
			u32To256(resBuff + sliceOff, rLen);
			sliceOff += 32;
			memcpy(resBuff+sliceOff, args[i].pValue._data, rLen);
			sliceOff += (rLen + 31) & 0xffe0;
		}
			break;
		case UINT128:
		case INT128:
		case UINT256:
		case INT256:
			memcpy(resBuff+prOff, args[i].pValue._data, 32);
			break;
		case UINT160:	// address
			memcpy(resBuff+prOff, args[i].pValue._data, 32);
			break;
		default:
			switch (pType) {
			case UINT64:
				u64To256(resBuff+prOff, args[i]._nValue);
				break;
			case INT64:
				i64To256(resBuff+prOff, args[i]._nValue);
				break;
			case INT16:
				// extend int16 to int64
				args[i]._nValue = (int64_t)((int16_t)args[i]._nValue);
				i64To256(resBuff+prOff, args[i]._nValue);
				break;
			case INT32:
				// extend int32 to int64
				args[i]._nValue = (int64_t)((int32_t)args[i]._nValue);
				i64To256(resBuff+prOff, args[i]._nValue);
				break;
			default:
				u32To256(resBuff+prOff, args[i]._nValue);
			}
		}
	}
	resLen = sliceOff;
	// code result
	eth_finish(resBuff, resLen);
	__builtin_unreachable();
}

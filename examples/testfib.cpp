#include <ewasm/ewasm.hpp>

static u32 fib(u32 n) {
	if (n < 2) return n;
	return fib(n-1)+fib(n-2);
}

static ewasm_argument	arg1{UINT64};
ewasm_method	_methods[]={
	{(char *)"constructor", 0, 0, 0,},
	{(char *)"fib", 0x73181a7b, 1, 0, &arg1,},
	{(char *)"owner", 0x8da5cb5b, 0, 0,},
};

extern "C" ewasm_ABI __Contract_ABI={3, _methods};

using namespace	ewasm;

static	byte	ret[32]={0,0,0,0, 0,0,0,10};
static	bytes32	key0(1), val32;
extern "C" void ewasm_main(const u32 Id, const byte* pBuff, const u32 pLen,
			const ewasm_method *mtdPtr)
{
	u32 n = 10;
	switch (Id) {
	case 0x73181a7b:
		if (pLen >= 32) {
			u32	met;
			// should be call FibValue(uint32) with Sig 0x73181a7b
			memcpy(&met, pBuff+28, 4);
			n = __builtin_bswap32(met);
		} else {
			eth_revert(ret, 8);
		}
		break;
	case 0x8da5cb5b:
	{
		// should be call owner() with Sig 0x8da5cb5b
		eth_storageLoad(&key0, &val32);
		eth_finish(&val32,32);
		return;
	}
		break;
	case 0x861731d5:
		// Constructor
		address	sender;
		eth_getCaller(&sender);
		bytes32	val32(sender);
		eth_storageStore(&key0, &val32);
		eth_finish(ret, 0);
		return;
	}
	u32 res = __builtin_bswap32(fib(n));
	*(u32 *)(ret+4) = 0;
	*(u32 *)(ret+28) = res;
	eth_finish(ret,32);
}

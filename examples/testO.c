#include <ewasm/ewasm.h>

typedef __int128_t i128;
typedef	unsigned int	u32;
typedef	unsigned short	u16;
typedef unsigned long long u64;

#ifdef ommit
#define	DECL_IMPORT(name, args) eth_##name args \
	__attribute__((import_module("ethereum"),import_name( #name )));
void DECL_IMPORT(finish, (char* _off, i32 _len))
//void eth_finish(char* _off, i32 _len) __attribute__((import_module("ethereum"), import_name("finish")));
i32  eth_getCallDataSize() __attribute__((import_module("ethereum"),import_name("getCallDataSize")));
void eth_callDataCopy(void *res, i32 dOff, i32 dLen) __attribute__((import_module("ethereum"),import_name("callDataCopy")));
#endif
//size_t	strlen(const char *);
char*	strcpy(char *__restrict, const char*__restrict);
char*	strncpy(char *__restrict, const char*__restrict, size_t);
char*	strchr(const char *, int);
int	strncmp(const char *, const char*, size_t);

__int128_t max(__int128_t a, __int128_t b) {
   return a*b; //a > b ? a: b;
}

__int128_t my_bswap128(__int128_t ml) {
	__int128_t ret;
	u64	ss[2];
	memcpy(&ss, &ml, sizeof(ml));
	void *dd = (void *)&ss; //malloc(sizeof(ml));
	memcmp(&ss, dd, sizeof(ml));
	memmove(dd, &ml, sizeof(ml));
	u64	*ptr=(u64 *)&ret;
	ptr[0] = __builtin_bswap64(ss[1]);
	ptr[1] = __builtin_bswap64(ss[0]);
	return ret;
}

// cpurchase   0xd6960697
// creceived	0x73fac6f0
// refund		0x590e1ae3
static	uint8_t	ret[32]={0,0,0,0, 0,0,0,10};
static	uint8_t	ret1[8]={0,0,0,20};
#pragma clang diagnostic ignored "-Wmain-return-type"
void main() // __attribute__((export_name("main")))
{
	if (eth_getCallDataSize() < 4) eth_finish(ret, 8);
	u32 	met;
	eth_callDataCopy(&met, 0, 4);
	u16		ms = __builtin_bswap16(met);
	ms++;
	ms = strlen("test only");
	//strcpy((char *)ret, "test only");
	__int128_t	ml;
	eth_callDataCopy(&ml, 0, 16);
	ml = bswap128(ml);
	ml = my_bswap128(ml);
	switch (__builtin_bswap32(met)) {
	case 0xd6960697:
	case 0x73fac6f0:
	case 0x590e1ae3:
		eth_finish(ret1,4);
	}
}

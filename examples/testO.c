#include <ewasm/ewasm.h>

typedef __int128_t i128;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned long long u64;

// size_t	strlen(const char *);
char *strcpy(char *__restrict, const char *__restrict);
char *strncpy(char *__restrict, const char *__restrict, size_t);
char *strchr(const char *, int);
int strncmp(const char *, const char *, size_t);

__attribute__((used)) __int128_t max(__int128_t a, __int128_t b) {
  return a * b; // a > b ? a: b;
}

__attribute__((used)) __int128_t my_bswap128(__int128_t ml) {
  __int128_t ret;
  u64 ss[2];
  memcpy(&ss, &ml, sizeof(ml));
  void *dd = (void *)&ss; // malloc(sizeof(ml));
  memcmp(&ss, dd, sizeof(ml));
  memcpy(dd, &ml, sizeof(ml));
  u64 *ptr = (u64 *)&ret;
  ptr[0] = __builtin_bswap64(ss[1]);
  ptr[1] = __builtin_bswap64(ss[0]);
  return ret;
}

// cpurchase   0xd6960697
// creceived	0x73fac6f0
// refund		0x590e1ae3
static uint8_t ret[32] = {0, 0, 0, 0, 0, 0, 0, 10};
static uint8_t ret1[8] = {0, 0, 0, 20};
#pragma clang diagnostic ignored "-Wmain-return-type"
void main() // __attribute__((export_name("main")))
{
  if (eth_getCallDataSize() < 4)
    eth_finish(ret, 8);
  u32 met;
  eth_callDataCopy(&met, 0, 4);
  u16 ms = __builtin_bswap16(met);
  ms++;
  ms = strlen("test only");
  // strcpy((char *)ret, "test only");
  memcpy(ret, "test only", ms);
  __int128_t ml;
  eth_callDataCopy(&ml, 0, 16);
  ml = bswap128(ml);
  ml = my_bswap128(ml);
  switch (__builtin_bswap32(met)) {
  case 0xd6960697:
  case 0x73fac6f0:
  case 0x590e1ae3:
    eth_finish(ret1, 4);
  }
}

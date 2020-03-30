typedef int i32;
typedef long long i64;
typedef __int128_t i128;
typedef unsigned char byte;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef __uint128_t u128;

void eth_finish(void *_off, i32 _len)
    __attribute__((__import_module__("ethereum"), __import_name__("finish")));
i32 eth_getCallDataSize()
    __attribute__((import_module("ethereum"), import_name("getCallDataSize")));
void eth_callDataCopy(void *res, i32 dOff, i32 dLen)
    __attribute__((import_module("ethereum"), import_name("callDataCopy")));

static u64 fib(u32 n) {
  if (n < 2)
    return n;
  u64 result = 2;
  u64 pre = 1;
  u64 next = 1;
  for (u32 i = 2; i < n; ++i) {
    result = pre + next;
    pre = next;
    next = result;
  }
  return result;
}

static byte ret[32] = {0, 0, 0, 0, 0, 0, 0, 10};
#pragma clang diagnostic ignored "-Wmain-return-type"
void main() // __attribute__((export_name("main")))
{
  i32 in_len;
  if ((in_len = eth_getCallDataSize()) < 4)
    eth_finish(ret, 8);
  i32 met;
  i32 n = 10;
  if (in_len >= 36) {
    // should be call FibValue(uint32) with Sig 0x73181a7b
    eth_callDataCopy(&met, 32, 4);
    n = __builtin_bswap32(met);
  } else if (in_len == 4) {
    // should be call owner() with Sig 0x8da5cb5b
    ret[7] = 0;
    ret[19] = 0xfe;
    eth_finish(ret, 32);
    return;
  }
  i64 res = __builtin_bswap64(fib(n));
  *(i64 *)(ret + 4) = 0;
  *(i64 *)(ret + 24) = res;
  eth_finish(ret, 32);
}

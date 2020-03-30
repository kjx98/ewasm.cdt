#include <ewasm/ewasm.h>

size_t strlen(const char *s) {
  if (s == 0)
    return 0;
  size_t n = 0;
  for (; *s != 0; ++s, ++n)
    ;
  return n;
}

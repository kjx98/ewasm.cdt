#include <ewasm/ewasm.hpp>

extern "C" size_t strlen(const char *s)
{
	size_t	n=0;
	if (s == nullptr) return n;
	for (; *s != 0; ++s, ++n);
	return n;
}

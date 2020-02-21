#pragma once

// ABI input/output codec

#ifdef	USE_MALLOC_ABI
#define	MAX_ABI_INPUT	1024
#else
#define	MAX_ABI_INPUT	512
#endif

#ifdef	USE_MALLOC_ABI
extern	byte	*__abiBuff;
#else
extern	byte	__abiBuff[];
#endif

#ifndef _STD_H
#define _STD_H 1

#include "types.h"

/** 
 * A bit of fancyness that allows us to sterotype functions and reuse the signatre, allowing us to
 * safely swap in and out functions at runtime.
*/
#define Fn_putchar(className) int className ## _putchar(uchar ch)
typedef int (*PFn_putchar)(uchar);

extern PFn_putchar putchar;

int printf(const PChar restrict format, ...);
int puts(const PChar str);

Pointer memmove(Pointer dstptr, const Pointer srcptr, uint size);
int memcmp(const Pointer aptr, const Pointer bptr, uint size);
Pointer memcpy(Pointer restrict dstptr, const Pointer restrict srcptr, uint size);
Pointer memset(Pointer bufptr, u8 value, uint size);

uint strlen(const PChar str);
PChar strcpy(PChar dest, const PChar src);

void abort(void);

u32 getAsciiKey(void);


/** CHeck if a bit is set in an array*/
#define Bt(array, bit_index) \
    ((array[(bit_index) / (sizeof(char) * 8)] & (1 << ((bit_index) % (sizeof(char) * 8)))) != 0)
/** if clause to do basic array bounds check */    
#define ifInside(index, size) if (index >= 0 && index < size)
#endif
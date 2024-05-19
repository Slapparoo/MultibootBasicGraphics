#include "vga.h"
#include "types.h"
#include "stdarg.h" // @TODO implement header

static const PChar hex = "0123456789ABCDEF";

int puts(const PChar str) {
    for (int i = 0; str[i] != 0; putchar(str[i++]));
}


void putInt$(int num) {
    // Buffer to hold the digits of the number in reverse order
    char buffer[12]; // Enough to hold all digits of a 32-bit integer including the sign
    int i = 0;

    // Handle zero case explicitly
    if (num == 0) {
        putchar('0');
        return;
    }

    // Handle negative numbers
    if (num < 0) {
        putchar('-');
        num = -num;
    }

    // Extract digits and store them in reverse order
    while (num != 0) {
        buffer[i++] = (num % 10) + '0'; // Convert digit to character and store
        num /= 10;
    }

    // Print the digits in correct order
    while (i > 0) {
        putchar(buffer[--i]);
    }
}

/**
 * @TODO not fully featured
 * @TODO lacking in error control
*/
int printf(const PChar restrict format, ...) {
    va_list args;
    va_start(args, format);

    PChar fmt = format;
    char ch;

    while ((ch = *fmt)) {
        if (ch == '%') {
            fmt++;
            switch (*fmt) {
                case 'c':
                    putchar(va_arg(args, int));
                    break;
                case 's':
                    puts(va_arg(args, PChar));
                    break;
                case 'x': // hex
                    puts("0x");
                    u32 value = va_arg(args, int);

                    int ix = 28;
                    for (int i = 0; i < 8; i++, ix -= 4) {
                        putchar(hex[(value >> ix) & 0xF]);
                    }
                    break;
                case 'B': // boolean
                    int res = va_arg(args, int);
                    if (res) {
                        puts("true");
                    } else {
                        puts("false");
                    }
                    break;
                case 'b': // binary
                    puts("0b");
                    u32 valueb = va_arg(args, int);
                    for (int i = 31; i >= 0; i--) {
                        putchar((valueb & (1 << i)) ? '1' : '0');
                    }
                    break;
                case 'i': //integer
                    putInt$(va_arg(args, int));
                    break;    
                case 'd': //decimal    
                default:
                    putchar('%');
                    putchar(*fmt);
                    break;
            }
        } else {
            putchar(ch);
        }
        fmt++;
    }
    va_end(args);
}

Pointer memmove(Pointer dstptr, const Pointer srcptr, uint size) {
	PChar dst = (PChar) dstptr;
	const PChar src = (const PChar) srcptr;
	if (dst < src) {
		for (uint i = 0; i < size; i++)
			dst[i] = src[i];
	} else {
		for (uint i = size; i != 0; i--)
			dst[i-1] = src[i-1];
	}
	return dstptr;
}

int memcmp(const Pointer aptr, const Pointer bptr, uint size) {
	const PChar a = (const PChar) aptr;
	const PChar b = (const PChar) bptr;
	for (uint i = 0; i < size; i++) {
		if (a[i] < b[i])
			return -1;
		else if (b[i] < a[i])
			return 1;
	}
	return 0;
}

Pointer memcpy(Pointer restrict dstptr, const Pointer restrict srcptr, uint size) {
	PChar dst = (PChar) dstptr;
	const PChar src = (const PChar) srcptr;
	for (uint i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}

Pointer memset(Pointer bufptr, u8 value, uint size) {
	PChar buf = (PChar) bufptr;
	for (uint i = 0; i < size; i++)
		buf[i] = value;
	return bufptr;
}

uint strlen(const PChar str) {
	uint len = 0;
	while (str[len])
		len++;
	return len;
}

PChar strcpy(PChar dest, const PChar src) {
    if (dest == null) {
        return null; // Return if no memory is allocated to the destination
    }

    if (src == dest) {
        return dest;
    }

    PChar ptr = dest; // Pointer to the beginning of the destination string
    PChar sptr = src; // Pointer to the beginning of the destination string

    while (*sptr != 0) {
        *dest++ = *sptr++; // Copy character from source to destination
    }

    *dest = 0; // Include the terminating null character
    return ptr; // Return the pointer to the destination
}

__attribute__((__noreturn__))
void abort(void) {
	// TODO: Abnormally terminate the process as if by SIGABRT.
	printf("abort()\n");
	while (1) { }
    	__builtin_unreachable();
}

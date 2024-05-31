#include "types.h"
#include "stdlibc.h"
#include <sys/io.h>



/** scancode to ascii map*/ /* incomplete but works for ascii*/
static char *keymap = "\0`1234567890-=\0\tqwertyuiop[]\n\0asdfghjkl;'\0\0\\zxcvbnm,./\0\0\0 \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
static char *keymapUppercase = "\0~!@#$%^&*()_+\0\tQWERTYUIOP{}\n\0ASDFGHJKL:\"\0\0\\ZXCVBNM<>?\0\0\0 \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

static boolean shift = false, ctrl = false, alt = false;
static u32 last = 0;

/**
 * Poll to see if anything has changed since last time we checked
 * Only retursn change on key up
*/
u32 getAsciiKey() {
    // init 
    if (last == 0) {
        last = inb(0x60);
        return 0;
    }
    /*
    keymap:
        db	0
        db	'1234567890-=', bspace
        db	tab,'qwertyuiop[]',enter_key
        db	ctrl_key,'asdfghjkl;',39,'`',lshift
        db	'\','zxcvbnm,./',rshift,prnscr,alt_key,' '
        db	caps,f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,numlock
        db	scroll,home,arrowup,pgup,num_sub,arrowleft,center5,arrowright
        db	num_plus,_end,arrowdown,pgdn,_ins,del
    */

    u32 rawkeyScancode = inb(0x60);  // read byte from post 0x60
    u32 keyScancode = rawkeyScancode & 0x7f; // ignore the up flag
    boolean up = (rawkeyScancode & 0x80); // just the key up

    if (rawkeyScancode != last) {
        last = rawkeyScancode;
        if (keyScancode == 0x2A || keyScancode == 0x36) { // left and right shift
            shift = !up;
        } else if (keyScancode == 0x38 || keyScancode == 0x3a) { // left and right control
            ctrl = !up;
        } else if (keyScancode == 0x71 || keyScancode == 0x72) { // alt
            alt = !up;
        } else if (keyScancode == 0x60) { // cursor keys?
            // cursor key? always seem to be an up event?
            
        } else if (up) {
            return shift ? keymapUppercase[keyScancode] : keymap[keyScancode];
        }
    }
    return 0;
}
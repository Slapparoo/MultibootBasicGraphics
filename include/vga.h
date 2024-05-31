/*

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef __MYOS__DRIVERS__VGA_H
#define __MYOS__DRIVERS__VGA_H

#include "multiboot2.h"
#include "types.h"
#include "stdlibc.h"

typedef struct VideoGraphicsArray
{
    /* data */
    u32 width;
    u32 height;
    u32 indexMax;

    u32 *buffer;
    u32 *screen;
    u32 *dest;
} VideoGraphicsArray;

typedef struct VideoGraphicsArray * PVideoGraphicsArray;
extern PVideoGraphicsArray videoGraphicsArray;
extern struct VgaConsole* vgaConsole;

void boot_vga_init(PVideoGraphicsArray pvideoGraphicsArray, const PMultibootHeader boot_header, u32 *buffer);
void boot_vga_putPixel(i32 x, i32 y, u32 color);
void boot_vga_putChar(uchar ch, i32 x, i32 y, u32 fgColor, u32 bgColor);
void boot_vga_putStr(PChar ch, i32 x, i32 y, u32 fgColor, u32 bgColor);
void boot_vga_fillRectangle(i32 x, i32 y, u32 w, u32 h, u32 color);
void boot_vga_bufferToScreen();
void boot_vga_window(i32 x, i32 y, u32 w, u32 h);
void boot_vga_init_window_console();

extern u64 FONT[256];
extern u8 sys_font_std_8x12[256*12];

#define CLR_MED_GREY 0x909090
#define CLR_LIGHT_GREY_1 0xC0C0C0
#define CLR_LIGHT_GREY_2 0xD0D0D0
#define CLR_LIGHT_GREY_3 0xE0E0E0
#define CLR_DARK_GREY_1 0x101010
#define CLR_DARK_GREY_2 0x404040
#define CLR_DARK_BLUE_1 0x2040E0
#define CLR_DARK_BLUE_2 0x002080
#define CLR_BLUE 0x1030A0
#define CLR_WHITE 0xFFFFFF
#define CLR_YELLOW 0xFFFF10
#define CLR_GREEN 0x10FF10
#define CLR_RED 0xFF1010
#define CLR_BLACK 0x000000

typedef struct VgaConsole {
    u32 col; // track current position
    u32 row; // track current position
    u32 fgColor;
    u32 bgColor;
    u32 width;  // in chars
    u32 height; // in chars
    u32 xpos; // in pixels
    u32 ypos; // in pixels
    u32 fontWidth;
    u32 fontHeight;
    PChar font;
    PChar buffer;
    boolean cursor;
} VgaConsole;

typedef struct CrtConsole {
    u32 col; // track current position
    u32 row; // track current position
    u8 color;
    PChar buffer;
} CrtConsole;

Fn_putchar(vga_boot_console);
Fn_putchar(crt_boot_console);
Fn_putchar(log_boot_console);


#endif

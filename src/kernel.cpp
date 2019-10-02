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
#include <vga.h>

VideoGraphicsArray *vgap;

#define width 1024 // hard coded - not good please change
#define height 768 // hard coded - not good please change

u32 buffer[width * height]; // hard coded - not good please change
static const char* hex = "0123456789ABCDEF";

void printfHex(u8 key, i32 x, i32 y, u32 color) {
    char* foo = "00";
    
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    vgap->PutStr(foo, x, y, color);
}

void printfHex32(u32 key, i32 x, i32 y, u32 color) {
    printfHex((key >> 24) & 0xFF, x, y, color);
    printfHex((key >> 16) & 0xFF, x+16, y, color);
    printfHex((key >> 8) & 0xFF, x+32, y, color);
    printfHex( key & 0xFF, x+48, y, color);
}

void printf(char* str, u32 key, i32 x, i32 y, u32 color) {
    vgap->PutStr(str, x, y, color);
    
    int l = 0;
    for (; str[l] != 0; l++);
    printfHex32(key, x + (l*8), y, color);
}

/**
 Draw something that looks like a window

*/
void window(i32 x, i32 y, u32 w, u32 h) {
    
    vgap->FillRectangle(x, y, w, h, 0x909090); // full
    vgap->FillRectangle(x+2, y+2, w-4, 2, 0x2040E0); // top bar
    vgap->FillRectangle(x+2, y+4, w-4, 8, 0x1030A0); // top bar
    vgap->FillRectangle(x+2, y+12, w-4, 2, 0x002080); // top bar
    vgap->FillRectangle(x+2, y+14, w-4, h-28, 0xE0E0E0); // content
    vgap->FillRectangle(x+2, y+h-12, w-4, 10, 0xC0C0C0); // bottom bar
    vgap->FillRectangle(x+w-37, y+3, 10, 10, 0xD0D0D0); // button back
    vgap->FillRectangle(x+w-25, y+3, 10, 10, 0xD0D0D0); // button back
    vgap->FillRectangle(x+w-13, y+3, 10, 10, 0xD0D0D0); // button back
    vgap->PutStr("Look a window", x+(w/2)-(13*8)/2  , y+4, 0xFFFFFF);
    
    vgap->PutChar(14, x+4, y+4, 0xFF1010);  // cirle
    vgap->PutChar(14, x+14, y+4, 0x10FF10); // cirle
    vgap->PutChar(14, x+24, y+4, 0xFFFF10); // cirle
    vgap->PutChar('_', x+w-36, y+4, 0x101010); 
    vgap->FillRectangle(x+w-24, y+4, 8, 8, 0x101010);
    vgap->FillRectangle(x+w-23, y+5, 6, 6, 0xD0D0D0);
    vgap->PutChar('X', x+w-12, y+4, 0x101010);   
    
    vgap->PutStr("Status bar | with | interesting | stuff ", x+4, y+h-10, 0x404040);
}

/**

entry point, this is what gets called by the ASM loader.s
you don't need stack pointer its just there to check whats going on

*/

extern "C" void kernelMain(u32 stackPointer , const multiboot_header* multiboot_structure, u32 /*multiboot_magic*/) {
    VideoGraphicsArray vga(multiboot_structure, buffer);
    vgap = &vga;

    window(100, 200, 800, 300);
    window(299, 99, 400, 500);
    
    u32 yOffs = 116;
    
	// dump some runtme information
	
    printf("multiboot_structure    : 0x", (int)multiboot_structure, 303, yOffs, 0); yOffs+=8;
    printf("stackPointer           : 0x", stackPointer, 303, yOffs, 0); yOffs+=8;
    printf("instructionPointer     : 0x", (int)&kernelMain, 303, yOffs, 0); yOffs+=8;
    printf("screen buffer          : 0x", (int)buffer, 303, yOffs, 0); yOffs+=8;
    printf("screen buffer[1024*768]: 0x", (int)&buffer[1024*768], 303, yOffs, 0); yOffs+=8;
    printf("FONT                   : 0x", (int)vgap->FONT, 303, yOffs, 0); yOffs+=8;
    printf("buffer size            : 0x", 1024*768*4, 303, yOffs, 0); yOffs+=8;
    printf("vga                    : 0x", (int)vgap, 303, yOffs, 0); yOffs+=8;
    
    yOffs+=4;
//    printf("multiboot_header
    printf("u32 flags              : 0x", (int)multiboot_structure->flags, 303, yOffs, 0); yOffs+=8;
    printf("u32 mem_lower          : 0x", (int)multiboot_structure->mem_lower, 303, yOffs, 0); yOffs+=8;
    printf("u32 mem_upper          : 0x", (int)multiboot_structure->mem_upper, 303, yOffs, 0); yOffs+=8;
    printf("u32 boot_device        : 0x", (int)multiboot_structure->boot_device, 303, yOffs, 0); yOffs+=8;
    printf("u32 cmdline            : 0x", (int)multiboot_structure->cmdline, 303, yOffs, 0); yOffs+=8;
    printf("u32 mods_count         : 0x", (int)multiboot_structure->mods_count, 303, yOffs, 0); yOffs+=8;
    printf("u32 mods_addr          : 0x", (int)multiboot_structure->mods_addr, 303, yOffs, 0); yOffs+=8;
    printf("u32 syms1              : 0x", (int)multiboot_structure->syms1, 303, yOffs, 0); yOffs+=8;
    printf("u32 syms2              : 0x", (int)multiboot_structure->syms2, 303, yOffs, 0); yOffs+=8;
    printf("u32 syms3              : 0x", (int)multiboot_structure->syms3, 303, yOffs, 0); yOffs+=8;
    printf("u32 mmap_length        : 0x", (int)multiboot_structure->mmap_length, 303, yOffs, 0); yOffs+=8;
    printf("u32 mmap_addr          : 0x", (int)multiboot_structure->mmap_addr, 303, yOffs, 0); yOffs+=8;
    printf("u32 drives_length      : 0x", (int)multiboot_structure->drives_length, 303, yOffs, 0); yOffs+=8;
    printf("u32 drives_addr        : 0x", (int)multiboot_structure->drives_addr, 303, yOffs, 0); yOffs+=8;
    printf("u32 config_table       : 0x", (int)multiboot_structure->config_table, 303, yOffs, 0); yOffs+=8;
    printf("u32 boot_loader_name   : 0x", (int)multiboot_structure->boot_loader_name, 303, yOffs, 0); yOffs+=8;
    printf("u32 apm_table          : 0x", (int)multiboot_structure->apm_table, 303, yOffs, 0); yOffs+=8;
    printf("u32 vbe_control_info   : 0x", (int)multiboot_structure->vbe_control_info, 303, yOffs, 0); yOffs+=8;
    printf("u32 vbe_mode_info      : 0x", (int)multiboot_structure->vbe_mode_info, 303, yOffs, 0); yOffs+=8;
    printf("u16 vbe_mode           : 0x", (int)multiboot_structure->vbe_mode, 303, yOffs, 0); yOffs+=8;
    printf("u16 vbe_interface_seg  : 0x", (int)multiboot_structure->vbe_interface_seg, 303, yOffs, 0); yOffs+=8;
    printf("u32 vbe_interface_off  : 0x", (int)multiboot_structure->vbe_interface_off, 303, yOffs, 0); yOffs+=8;
    printf("u32 vbe_interface_len  : 0x", (int)multiboot_structure->vbe_interface_len, 303, yOffs, 0); yOffs+=8;
    printf("u64 framebuffer_addr   : 0x", (long)multiboot_structure->framebuffer_addr, 303, yOffs, 0); yOffs+=8;
    printf("u32 framebuffer_pitch  : 0x", (int)multiboot_structure->framebuffer_pitch, 303, yOffs, 0); yOffs+=8;
    
    printf("u32 framebuffer_width  : 0x", (int)multiboot_structure->framebuffer_width, 303, yOffs, 0); yOffs+=8;
    printf("u32 framebuffer_height : 0x", (int)multiboot_structure->framebuffer_height, 303, yOffs, 0); yOffs+=8;
    printf("u8 framebuffer_bpp     : 0x", (int)multiboot_structure->framebuffer_bpp, 303, yOffs, 0); yOffs+=8;
    printf("u8 framebuffer_type    : 0x", (int)multiboot_structure->framebuffer_type, 303, yOffs, 0); yOffs+=8;
    printf("u8 color_info[5]       : 0x", (int)multiboot_structure->color_info, 303, yOffs, 0); yOffs+=8;

    
    vga.bufferToScreen();
}

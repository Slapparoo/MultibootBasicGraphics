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
#include "vga.h"
#include "stdarg.h"
#include "stdlibc.h"
#include "multiboot2.h"

#define CHECK_FLAG(flags,bit) ((flags) & (1 << (bit)))

#define SCREEN_WIDTH 1024 // hard coded - not good please change
#define SCREEN_HEIGHT 768 // hard coded - not good please change

u32 buffer[SCREEN_WIDTH * SCREEN_HEIGHT]; // hard coded - not good please change

struct VideoGraphicsArray vga;
PMultibootHeader multibootHeader;
PFn_putchar putchar;

/**
 * print the flags we set in the application header given to Grub Multiboot
*/
int requestFlags(u32 requestFlags) {
  printf ("request flags:\n  pageAlign(%B), memInfo(%B), videoMode(%B)\n", 
    requestFlags & MULTIBOOT_PAGE_ALIGN,
    requestFlags & MULTIBOOT_MEMORY_INFO,
    requestFlags & MULTIBOOT_VIDEO_MODE);
}

int validateBoot(u32 magic, u32 flags) {
        /* Am I booted by a Multiboot-compliant boot loader? */
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf ("Invalid magic number: %x, expecting %x\n", magic, MULTIBOOT_BOOTLOADER_MAGIC);
        return 1;
    }

    /* Bits 4 and 5 are mutually exclusive! */
    if ((flags & MULTIBOOT_INFO_AOUT_SYMS) && (flags & MULTIBOOT_INFO_ELF_SHDR)) {
        printf ("Both bits 4 and 5 are set.\n");
        return 2;
    }
    return 0;
}

void bootFlags(u32 flags) {
    /* Print out the flags. */
    printf ("flags:\n");

    printf ("  mem(%B), ", flags & MULTIBOOT_INFO_MEMORY);
    printf ("boot_device(%B), ", flags & MULTIBOOT_INFO_BOOTDEV);
    printf ("cmdline(%B), ", flags & MULTIBOOT_INFO_CMDLINE);
    printf ("mods(%B)\n", flags & MULTIBOOT_INFO_MODS);
    printf ("  aout(%B), ", flags & MULTIBOOT_INFO_AOUT_SYMS);
    printf ("elf(%B), ", flags & MULTIBOOT_INFO_ELF_SHDR);
    printf ("mmap(%B), ", flags & MULTIBOOT_INFO_MEM_MAP);
    printf ("drive info(%B), ", flags & MULTIBOOT_INFO_DRIVE_INFO);
    printf ("Config Table(%B)\n", flags & MULTIBOOT_INFO_CONFIG_TABLE);
    printf ("  Boot Loader(%B), ", flags & MULTIBOOT_INFO_BOOT_LOADER_NAME);
    printf ("APM(%B), ", flags & MULTIBOOT_INFO_APM_TABLE);
    printf ("VBE(%B), ", flags & MULTIBOOT_INFO_VBE_INFO);
    printf ("framebuffer(%B)\n", flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO);
}

extern u32 loader;
extern u32* multiboot;
u32 *mbPtr = &multiboot;
extern u32 bootresponse;
extern u32 kernel_stack;



/**

entry point, this is what gets called by the ASM loader.s
you don't need stack pointer its just there to check whats going on

*/

void main() {};

void kernelMain() {
    u32 *bootRes = &bootresponse;
    u32 magic = bootRes[0];
    multibootHeader = bootRes[1];
    u32 c1, c2, c3;

    putchar = &crt_boot_console_putchar;
    u32 mbRequestFlags = mbPtr[1];


    if (mbRequestFlags & MULTIBOOT_VIDEO_MODE) {
        boot_vga_init(&vga, multibootHeader, buffer);
        putchar = &vga_boot_console_putchar;
    } else {
        crt_clear_screen(0);
    }

    if (validateBoot(magic, multibootHeader->flags)) {
        return 1;
    }

    if (mbRequestFlags & MULTIBOOT_VIDEO_MODE) {
        boot_vga_window(100, 200, 100, 30);
        boot_vga_window(120, 40, 80, 50);
    }

    requestFlags(mbRequestFlags);
    bootFlags(multibootHeader->flags);

	// dump some runtme information

    printf("multiboot request flags: %x, %b\n", mbRequestFlags, mbRequestFlags);
    printf("x bootresponse  addr     : %x\n", bootRes);
    printf("x magic                  : %x\n", magic);
    printf("x multibootHeader        : %x\n", bootRes[1]);
    printf("x multibootRequest offest: %x\n", mbPtr);
    printf("x stackPointer top       : %x\n", &kernel_stack);
    printf("instructionPointer     : %x\n", &kernelMain);
    if (mbRequestFlags & MULTIBOOT_VIDEO_MODE) {
        printf("screen buffer          : %x\n", buffer);
        printf("screen buffer end      : %x\n", &buffer[multibootHeader->framebuffer_width*multibootHeader->framebuffer_height]);
        printf("buffer size            : %x\n", multibootHeader->framebuffer_width*multibootHeader->framebuffer_height*(multibootHeader->framebuffer_bpp / 8));
        printf("vga                    : %x\n", &vga);
    }
    
    printf("u32 flags              : %x, %b\n", multibootHeader->flags, multibootHeader->flags);
    printf("u32 mem_lower          : %x\n", multibootHeader->mem_lower);
    printf("u32 mem_upper          : %x\n", multibootHeader->mem_upper);
    printf("u32 boot_device        : %x\n", multibootHeader->boot_device);
    printf("u32 cmdline            : %x, %s \n", multibootHeader->cmdline, multibootHeader->cmdline);
    if (multibootHeader->flags & MULTIBOOT_INFO_MODS) {
        printf("u32 mods_count         : %x\n", multibootHeader->mods_count);
        printf("u32 mods_addr          : %x\n", multibootHeader->mods_addr);
    }

    if (multibootHeader->flags & MULTIBOOT_INFO_ELF_SHDR) {
        printf("u32 u.elf_sec.addr     : %x\n", multibootHeader->u.elf_sec.addr);
        printf("u32 u.elf_sec.num      : %x\n", multibootHeader->u.elf_sec.num);
        // printf("u32 u.elf_sec.shndx    : %x\n", (int)multibootHeader->u.elf_sec.shndx);
        printf("u32 u.elf_sec.size     : %x\n", multibootHeader->u.elf_sec.size);
    }
    printf("u32 mmap_length        : %x\n", multibootHeader->mmap_length);
    printf("u32 mmap_addr          : %x\n", multibootHeader->mmap_addr);
    if (multibootHeader->flags & MULTIBOOT_INFO_DRIVE_INFO) {
        printf("u32 drives_length      : %x\n", multibootHeader->drives_length);
        printf("u32 drives_addr        : %x\n", multibootHeader->drives_addr);
    }
    if (multibootHeader->flags & MULTIBOOT_INFO_CONFIG_TABLE) { 
        printf("u32 config_table       : %x\n", multibootHeader->config_table);
    }
    printf("u32 boot_loader_name   : %x\n", multibootHeader->boot_loader_name);
    if (multibootHeader->flags & MULTIBOOT_INFO_APM_TABLE) {
        printf("u32 apm_table          : %x\n", multibootHeader->apm_table);
    }
    if (mbRequestFlags & MULTIBOOT_VIDEO_MODE) {
        printf("u32 vbe_control_info   : %x\n", multibootHeader->vbe_control_info);
        printf("u32 vbe_mode_info      : %x\n", multibootHeader->vbe_mode_info);
        printf("u16 vbe_mode           : %x\n", multibootHeader->vbe_mode);
        printf("u16 vbe_interface_seg  : %x\n", multibootHeader->vbe_interface_seg);
        printf("u32 vbe_interface_off  : %x\n", multibootHeader->vbe_interface_off);
        printf("u32 vbe_interface_len  : %x\n", multibootHeader->vbe_interface_len);
        printf("u64 framebuffer_addr   : %x\n", multibootHeader->framebuffer_addr);
        printf("u32 framebuffer_pitch  : %x\n", multibootHeader->framebuffer_pitch);
        
        printf("u32 framebuffer_(width,height,bpp)  :%ix%ix%i\n", multibootHeader->framebuffer_width, multibootHeader->framebuffer_height, multibootHeader->framebuffer_bpp);
        printf("u8 framebuffer_type    : %x\n", multibootHeader->framebuffer_type);
    }

    for (int i = 0; i < 256; i++) {
        printf("%c", i);
    }

    printf("\n (root)> ");

    vgaConsole->cursor = true;
    crt_boot_console_cursor();
    while (true) {
        char code = getAsciiKey();
        if (code != 0) {
            if (code == '\n') {
                printf("\nerror: not found\n (root)> "); // nothing is ever found
            } else {
                printf("%c", code);
            }
        }
    }
}

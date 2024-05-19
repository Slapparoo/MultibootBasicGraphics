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

/*

Boot loader this is the boot entry point, it calls the kernelMain in kernel.cpp


#define MULTIBOOT_HEADER_FLAGS  MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_VIDEO_MODE | AOUT_KLUDGE

*/
.att_syntax

// .set FLAGS, 7 = graphics, 3 = crt   
.set REQUEST_FLAGS, 7

.set MAGIC, 0x1badb002
.set FLAGS, REQUEST_FLAGS  /* crt mode */
.set CHECKSUM, -(MAGIC + FLAGS)
.set MODE_TYPE, 0
.set WIDTH, 1024  /* requested width */
.set HEIGHT, 768  /* requested height */
.set DEPTH, 32    /* requested bits per pixel BPP */
.set HEADER_ADDR, 0
.set LOAD_ADDR, 0
.set LOAD_END_ADDR, 0
.set BSS_END_ADDR, 0
.set ENTRY_ADDR, 0

/**
from https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#OS-image-format 

0	u32	magic	required
4	u32	flags	required
8	u32	checksum	required
12	u32	header_addr	if flags[16] is set
16	u32	load_addr	if flags[16] is set
20	u32	load_end_addr	if flags[16] is set
24	u32	bss_end_addr	if flags[16] is set
28	u32	entry_addr	if flags[16] is set
32	u32	mode_type	if flags[2] is set
36	u32	width	if flags[2] is set
40	u32	height	if flags[2] is set
44	u32	depth	if flags[2] is set

*/

.section .multiboot
.global multiboot
multiboot:
    .long MAGIC
    .long FLAGS
    .long CHECKSUM
    .long HEADER_ADDR
    .long LOAD_ADDR
    .long LOAD_END_ADDR
    .long BSS_END_ADDR
    .long ENTRY_ADDR
    .long MODE_TYPE
    .long WIDTH
    .long HEIGHT
    .long DEPTH
    /* enough space for the returned header */
    .space 4 * 13

.section .text
.extern kernelMain
.extern callConstructors
.global loader

loader:
    mov $kernel_stack, %esp
    mov $kernel_stack, %ecx

/*    .byte 0x50 - sneaky machine code for push eax */
    push %eax  # Magic
    push %ebx  # Pointer to multiboot
    push %ecx  # Stack pointer
    call kernelMain

_stop:
    cli
    hlt
    jmp _stop


.section .bss
.space 200*1024*1024; # 200 MiB
kernel_stack:


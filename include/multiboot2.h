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
#ifndef __MULTIBOOT2__H
#define __MULTIBOOT2__H

#include "types.h"

/* How many bytes from the start of the file we search for the header. */
#define MULTIBOOT_SEARCH                        8192
#define MULTIBOOT_HEADER_ALIGN                  4
/* The magic field should contain this. */
#define MULTIBOOT_HEADER_MAGIC                  0x1BADB002
/* This should be in %eax. */
#define MULTIBOOT_BOOTLOADER_MAGIC              0x2BADB002
/* Alignment of multiboot modules. */
#define MULTIBOOT_MOD_ALIGN                     0x00001000
/* Alignment of the multiboot info structure. */
#define MULTIBOOT_INFO_ALIGN                    0x00000004
/* Flags set in the ’flags’ member of the multiboot header. */
/* Align all boot modules on i386 page (4KB) boundaries. */
#define MULTIBOOT_PAGE_ALIGN                    0x00000001
/* Must pass memory information to OS. */
#define MULTIBOOT_MEMORY_INFO                   0x00000002
/* Must pass video information to OS. */
#define MULTIBOOT_VIDEO_MODE                    0x00000004
/* This flag indicates the use of the address fields in the header. */
#define MULTIBOOT_AOUT_KLUDGE                   0x00010000
/* Flags to be set in the ’flags’ member of the multiboot info structure. */
/* is there basic lower/upper memory information? */
#define MULTIBOOT_INFO_MEMORY                   0x00000001
/* is there a boot device set? */
#define MULTIBOOT_INFO_BOOTDEV                  0x00000002
/* is the command-line defined? */
#define MULTIBOOT_INFO_CMDLINE                  0x00000004
/* are there modules to do something with? */
#define MULTIBOOT_INFO_MODS                     0x00000008
/* These next two are mutually exclusive */
/* is there a symbol table loaded? */
#define MULTIBOOT_INFO_AOUT_SYMS                0x00000010
/* is there an ELF section header table? */
#define MULTIBOOT_INFO_ELF_SHDR                 0X00000020
/* is there a full memory map? */
#define MULTIBOOT_INFO_MEM_MAP                  0x00000040
/* Is there drive info? */
#define MULTIBOOT_INFO_DRIVE_INFO               0x00000080
/* Is there a config table? */
#define MULTIBOOT_INFO_CONFIG_TABLE             0x00000100
/* Is there a boot loader name? */
#define MULTIBOOT_INFO_BOOT_LOADER_NAME         0x00000200
/* Is there a APM table? */
#define MULTIBOOT_INFO_APM_TABLE                0x00000400
/* Is there video information? */
#define MULTIBOOT_INFO_VBE_INFO                 0x00000800
#define MULTIBOOT_INFO_FRAMEBUFFER_INFO         0x00001000



/* The symbol table for a.out. */
struct MultibootAoutSymbolTable
{
  u32 tabsize;
  u32 strsize;
  u32 addr;
//   u32 reserved;
};
typedef struct MultibootAoutSymbolTable MultibootAoutSymbolTable_t;

/* The section header table for ELF. */
struct MultibootElfSectionHeaderTable
{
  u32 num;
  u32 size;
  u32 addr;
//   u32 shndx;
};
typedef struct MultibootElfSectionHeaderTable MultibootElfSectionHeaderTable_t;
/**
 * The multiboot section which is passed in by grub at boot up with the following fields
 * populated. 
 * for more information see:
 * https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Boot-information-format
 * 
 * 
 * 0       | flags             |    (required)
        +-------------------+
4       | mem_lower         |    (present if flags[0] is set)
8       | mem_upper         |    (present if flags[0] is set)
        +-------------------+
12      | boot_device       |    (present if flags[1] is set)
        +-------------------+
16      | cmdline           |    (present if flags[2] is set)
        +-------------------+
20      | mods_count        |    (present if flags[3] is set)
24      | mods_addr         |    (present if flags[3] is set)
        +-------------------+
28 - 40 | syms              |    (present if flags[4] or
        |                   |                flags[5] is set)
        +-------------------+
44      | mmap_length       |    (present if flags[6] is set)
48      | mmap_addr         |    (present if flags[6] is set)
        +-------------------+
52      | drives_length     |    (present if flags[7] is set)
56      | drives_addr       |    (present if flags[7] is set)
        +-------------------+
60      | config_table      |    (present if flags[8] is set)
        +-------------------+
64      | boot_loader_name  |    (present if flags[9] is set)
        +-------------------+
68      | apm_table         |    (present if flags[10] is set)
        +-------------------+
72      | vbe_control_info  |    (present if flags[11] is set)
76      | vbe_mode_info     |
80      | vbe_mode          |
82      | vbe_interface_seg |
84      | vbe_interface_off |
86      | vbe_interface_len |
        +-------------------+
88      | framebuffer_addr  |    (present if flags[12] is set)
96      | framebuffer_pitch |
100     | framebuffer_width |
104     | framebuffer_height|
108     | framebuffer_bpp   |
109     | framebuffer_type  |
110-115 | color_info        |
        +-------------------+
 * 
 */
typedef struct MultibootHeader {
    u32 flags;
    u32 mem_lower;
    u32 mem_upper;
    u32 boot_device;
    u32 cmdline;
    u32 mods_count;
    u32 mods_addr;
    union
    {
        MultibootAoutSymbolTable_t aout_sym;
        MultibootElfSectionHeaderTable_t elf_sec;
    } u;
    u32 mmap_length;
    u32 mmap_addr;
    u32 drives_length;
    u32 drives_addr;
    u32 config_table;
    u32 boot_loader_name;
    u32 apm_table;
    u32 vbe_control_info;
    u32 vbe_mode_info;
    u16 vbe_mode;
    u16 vbe_interface_seg;
    u32 vbe_interface_off;
    u32 vbe_interface_len;
    u64 framebuffer_addr;
    u32 framebuffer_pitch;
    
    u32 framebuffer_width;
    u32 framebuffer_height;
    u8 framebuffer_bpp;
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED      0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB          1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT     2
   u8 framebuffer_type;
   union
   {
     struct
     {
       u32 framebuffer_palette_addr;
       u16 framebuffer_palette_num_colors;
     };
     struct
     {
       u8 framebuffer_red_field_position;
       u8 framebuffer_red_mask_size;
       u8 framebuffer_green_field_position;
       u8 framebuffer_green_mask_size;
       u8 framebuffer_blue_field_position;
       u8 framebuffer_blue_mask_size;
     };
   };
} MultibootHeader;
typedef struct MultibootHeader * PMultibootHeader;

typedef struct MultibootMmapEntry
{
  u32 size;
  u64 addr;
  u64 len;
#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5
  u32 type;
} __attribute__((packed)) MultibootMmapEntry;

typedef struct MultibootModList
{
  /* the memory used goes from bytes ’mod_start’ to ’mod_end-1’ inclusive */
  u32 mod_start;
  u32 mod_end;

  /* Module command line */
  u32 cmdline;

  /* padding to take it to 16 bytes (must be zero) */
  u32 pad;
} MultibootModList;

/* APM BIOS info. */
typedef struct MultibootApmInfo
{
  u16 version;
  u16 cseg;
  u32 offset;
  u16 cseg_16;
  u16 dseg;
  u16 flags;
  u16 cseg_len;
  u16 cseg_16_len;
  u16 dseg_len;
} MultibootApmInfo;

typedef struct MultibootApmInfo * PMultibootApmInfo;

#endif

ENTRY(loader)
/* OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386) */

SECTIONS
{
  . = 1M;

  .text :
  {
  . = ALIGN(8);
  KEEP(*(.multiboot))
    *(.text*)
    *(.rodata)
  }

  .data  :
  {
    start_ctors = .;
    KEEP(*( .init_array ));
    KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
    end_ctors = .;

    *(.data)
  }

  .bss  :
  {
    *(.bss)
  }

  /DISCARD/ : { *(.fini_array*) *(.comment) }
}

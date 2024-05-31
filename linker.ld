ENTRY(loader)

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
    *(.data)
  }

  .bss  :
  {
    *(.bss)
  }
}

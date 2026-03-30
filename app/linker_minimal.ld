
MEMORY
{
  FLASH (rx) : ORIGIN = 0x08008000, LENGTH = 512K
  RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 128K
}

_estack =  ORIGIN(RAM) + LENGTH(RAM);  /* = 0x20020000 */

SECTIONS
{
    .isr_vector :
  {
    KEEP(*(.isr_vector))
  } > FLASH

    .text :
    {
      *(.text*)
    } > FLASH

    .bss :
    {
      _sbss = .;
      *(.bss*)
      *(COMMON)
      _ebss = .;
    } > RAM

  _sidata = LOADADDR(.data);   /* FLASH start */
  
  .data :
  {
    _sdata = .;    /* RAM start */
    *(.data*)
    _edata = .;
  } > RAM AT    > FLASH
}


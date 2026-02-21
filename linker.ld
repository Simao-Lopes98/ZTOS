/* linked.ld
Linker script file
*/


/* See src/startup.S */
ENTRY(Reset_Handler)

/* See: RM0008, sections 3.3.1 and 3.3.3 */
MEMORY
{
  FLASH (rx)  :ORIGIN = 0x08000000,LENGTH =64K
  SRAM  (rwx) :ORIGIN = 0x20000000,LENGTH =20K
}

SECTIONS
{

/* Define _estack as the highest address in the SRAM region. */
_estack = ORIGIN(SRAM) + LENGTH(SRAM);

.isr_vector :
  {
    KEEP(*(.isr_vector));
    . = ALIGN(4);
  }> FLASH

.text :
  {
    *(.text)
    *(.text.*)
    *(.rodata)
    *(.rodata.*)
    . = ALIGN(4);
    _etext = .;
  }> FLASH

  .ARM.extab : {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
   } > FLASH

  .ARM.exidx : {
    PROVIDE (__exidx_start = .);
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    PROVIDE (__exidx_end = .);
  } > FLASH

/* This loads the memory address of .data */
_la_data = LOADADDR(.data);


.data :
  {
    _sdata = .;
    *(.data)
    *(.data.*)
    . = ALIGN(4);
    _edata = .;
  }> SRAM AT> FLASH


.bss :
  {
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss.*)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
    . = ALIGN(4);
    end = .;
    __end__ = .;
  }> SRAM
}
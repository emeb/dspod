/*
 * linker script for application code in SPRAM
 * ROM  remapped RAM1/RAM2 at location 0
 * RAM1 reserved for loadable code.
 * RAM2 as data/bss/heap/stack
 */

MEMORY
{
	ROM  (rx)    : ORIGIN = 0x00000000, LENGTH = 0x8000
    RAM1 (rx)    : ORIGIN = 0x10000000, LENGTH = 0x8000
    RAM2 (xrw)   : ORIGIN = 0x10008000, LENGTH = 0x8000
}
SECTIONS {
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        *(.srodata)
        *(.srodata*)
        . = ALIGN(4);
        _etext = .;
        _sidata = _etext;
    } >ROM
    .data : AT ( _sidata )
    {
        . = ALIGN(4);
        _sdata = .;
        _ram_start = .;
        . = ALIGN(4);
        *(.data)
        *(.data*)
        *(.sdata)
        *(.sdata*)
        . = ALIGN(4);
        _edata = .;
    } >RAM2
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(.sbss)
        *(.sbss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >RAM2
    .heap :
    {
        . = ALIGN(4);
        _heap_start = .;
    } >RAM2
}

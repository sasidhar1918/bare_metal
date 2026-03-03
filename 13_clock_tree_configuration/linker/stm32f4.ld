/* Entry Point */
ENTRY(Reset_Handler)

/* Specify the memory areas  */
MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 2048K
    SRAM (xrw) : ORIGIN = 0x20000000, LENGTH = 256K
}

/* Define sections */
SECTIONS
{
    /* The program code and other data goes into FLASH */
    .text :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector)) /* Vector table */
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .; 
    } > FLASH

    /* Initialized data sections go into RAM, load address in FLASH */
    .data : 
    {
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > SRAM AT > FLASH

    /* Uninitialized data section into RAM */
    .bss :
    {
        _sbss = .;
        *(.bss)
        *(.bss*)
        . = ALIGN(4);
        _ebss = .;
    } > SRAM

    /* User_heap_stack section, used to check that there is enough RAM left */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        . = . + 0x2000; /* 8KB of stack */
        _estack = .;
    } > SRAM
}

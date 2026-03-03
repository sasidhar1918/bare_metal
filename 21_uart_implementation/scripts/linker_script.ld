/* Entry Point: The first function to run (usually in startup.c) */
ENTRY(Reset_Handler)

/* Specify the memory areas for STM32F4 */
MEMORY
{
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM   (xrw)     : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Define sections */
SECTIONS
{
    /* 1. Vector Table and Code (Flash) */
    .text :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector)) /* The Interrupt Vector Table MUST be first */
        *(.text)             /* All C code */
        *(.text*)            /* All C code sub-sections */
        *(.rodata)           /* Constant data (strings, etc.) */
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;          /* End of code marker */
    } > FLASH

    /* 2. Initialized Data (Stored in Flash, moved to RAM at boot) */
    _sidata = LOADADDR(.data);
    .data :
    {
        . = ALIGN(4);
        _sdata = .;          /* Start of data in RAM */
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;          /* End of data in RAM */
    } > RAM AT > FLASH

    /* 3. Uninitialized Data (BSS) - Zeroed out at boot */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;          /* Start of BSS */
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;          /* End of BSS */
        __bss_end__ = _ebss;
    } > RAM

    /* 4. User Stack Space (At the very top of RAM) */
    ._user_stack :
    {
        . = ALIGN(8);
        . = . + 0x1000;    /* Reserve 4KB for stack */
        _estack = .;
    } > RAM

    /* Remove information from the standard libraries */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }
}

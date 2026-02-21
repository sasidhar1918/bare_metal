/* Entry Point */
ENTRY(Reset_Handler)

/* Specify Memory Regions */
MEMORY
{
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (xrw)     : ORIGIN = 0x20000000, LENGTH = 96K
}

/* Define Sections */
SECTIONS
{
    /* Vector Table and Code go into FLASH */
    .text :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector)) /* Keep the interrupt vector table */
        *(.text)             /* All code */
        *(.text*)            /* All sub-sections of code */
        *(.rodata)           /* Constant data (Read-only) */
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;          /* Define end of text section */
    } > FLASH

    /* Used by the startup code to initialize data in RAM */
    _sidata = LOADADDR(.data);

    /* Initialized data section goes into RAM, but stored in FLASH */
    .data : 
    {
        . = ALIGN(4);
        _sdata = .;        /* Start of .data */
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;        /* End of .data */
    } > RAM AT > FLASH

    /* Uninitialized data */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;         /* Start of .bss */
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
       . = ALIGN(4);
        _ebss = .;         /* End of .bss */
        __bss_end__ = _ebss;
    } > RAM

    /* User Stack pointer (Top of RAM) */
    _estack = ORIGIN(RAM) + LENGTH(RAM);
}

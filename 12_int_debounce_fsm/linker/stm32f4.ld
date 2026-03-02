/* Entry Point */
ENTRY(Reset_Handler)

/* Specify the memory areas */
MEMORY
{
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Define sections */
SECTIONS
{
    /* 1. The Startup code and Interrupt Vector Table go at the start of Flash */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector)) /* Startup code looks for this specific name */
        . = ALIGN(4);
    } > FLASH

    /* 2. The program code and other data goes into Flash */
    .text :
    {
        . = ALIGN(4);
        *(.text)           /* .text sections (code) */
        *(.text*)          /* .text* sections (subsidiary code) */
        *(.glue_7)         /* glue arm to thumb code */
        *(.glue_7t)        /* glue thumb to arm code */
        *(.eh_frame)

        KEEP (*(.init))
        KEEP (*(.fini))

        . = ALIGN(4);
        _etext = .;        /* define a global symbol at end of code */
    } > FLASH

    /* 3. Constant data goes into Flash */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    /* 4. Used by the startup to initialize data */
    _sidata = LOADADDR(.data);

    /* 5. Initialized data sections go into RAM, but are stored in FLASH */
    .data : 
    {
        . = ALIGN(4);
        _sdata = .;        /* create a global symbol at data start */
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;        /* create a global symbol at data end */
    } > RAM AT > FLASH

    /* 6. Uninitialized data section (Zero-filled) */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;         /* start of .bss */
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;         /* end of .bss */
        __bss_end__ = _ebss;
    } > RAM

    /* 7. User_heap_stack section, used to check that there is enough RAM left */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + 0x200;    /* Min amount of heap */
        . = . + 0x400;    /* Min amount of stack */
        . = ALIGN(8);
    } > RAM

    /* Set stack top to end of RAM */
    _estack = ORIGIN(RAM) + LENGTH(RAM);

    /* Remove information from the standard libraries */
    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }
}

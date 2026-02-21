#include <stdint.h>
#include "stm32f4xx.h"

/* Symbols defined in the Linker Script */
extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss, _estack;

/* Prototypes for Handlers */
void Reset_Handler(void);
void Default_Handler(void) { while(1); }

/* Weak Aliases for all Interrupts */
void SysTick_Handler(void) __attribute__ ((weak, alias("Default_Handler")));


/* Vector Table */
__attribute__ ((section(".isr_vector")))
uint32_t *vector_table[] = {
    (uint32_t *)&_estack,         /* 0: Stack Pointer */
    (uint32_t *)Reset_Handler,    /* 1: Reset Handler */
    0, 0, 0, 0, 0, 0, 0, 0, 0,    /* 2-10: Reserved */
    0, 0, 0, 0,                   /* 11-14: SVCall, PendSV... */
    (uint32_t *)SysTick_Handler   /* 15: SysTick */
};


void Reset_Handler(void) {
    /* Copy .data from FLASH to RAM */
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* Zero out .bss */
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    /* Call SystemInit (CMSIS) and main */
    extern void SystemInit(void);
    extern int main(void);
    
    SystemInit();
    main();

    while(1); 
}

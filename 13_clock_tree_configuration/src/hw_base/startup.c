#include <stdint.h>

/* Symbols from linker script */
extern uint32_t _etext, _sdata, _edata, _sbss, _ebss, _estack;

/* Function prototypes */
void Reset_Handler(void);
void Default_Handler(void);
extern int main(void);

/* Hardware Specific Handlers */
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

/* Vector Table [cite: 3, 4] */
__attribute__((section(".isr_vector")))
uint32_t *vector_table[] = {
    (uint32_t *)&_estack,         /* Top of Stack */
    (uint32_t *)Reset_Handler,    /* Reset Handler */
    (uint32_t *)Default_Handler,  /* NMI */
    (uint32_t *)Default_Handler,  /* HardFault */
    0, 0, 0, 0, 0, 0, 0,          /* Reserved */
    (uint32_t *)Default_Handler,  /* SVC */
    0, 0,                         /* Reserved */
    (uint32_t *)Default_Handler,  /* PendSV */
    (uint32_t *)SysTick_Handler   /* SysTick */
};

/**
 * @brief Initialize memory and call main
 */
void Reset_Handler(void) {
    /* Copy .data from FLASH to SRAM */
    uint32_t *pSrc = &_etext;
    uint32_t *pDest = &_sdata;
    while (pDest < &_edata) {
        *pDest++ = *pSrc++;
    }

    /* Zero out .bss */
    pDest = &_sbss;
    while (pDest < &_ebss) {
        *pDest++ = 0;
    }

    /* Call the application entry point */
    main();

    /* Infinite loop if main returns */
    while (1);
}

void Default_Handler(void) {
    while (1);
}

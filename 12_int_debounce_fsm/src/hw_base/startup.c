#include <stdint.h>

/* Symbols defined in the linker script */
extern uint32_t _sidata; /* Start of .data in FLASH */
extern uint32_t _sdata;  /* Start of .data in RAM */
extern uint32_t _edata;  /* End of .data in RAM */
extern uint32_t _sbss;   /* Start of .bss in RAM */
extern uint32_t _ebss;   /* End of .bss in RAM */
extern uint32_t _estack; /* Top of Stack */

/* Function prototypes */
void Reset_Handler(void);
void Default_Handler(void);

/* Weak prototypes for handlers you will implement in main.c */
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI0_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void EXTI1_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

/* Standard Cortex-M4 Handlers */
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));

/* Vector Table */
__attribute__((section(".isr_vector")))
uint32_t *vector_table[] = {
    (uint32_t *)&_estack,      /* Main Stack Pointer */
    (uint32_t *)Reset_Handler, /* Reset Handler */
    (uint32_t *)NMI_Handler,
    (uint32_t *)HardFault_Handler,
    (uint32_t *)MemManage_Handler,
    (uint32_t *)BusFault_Handler,
    (uint32_t *)UsageFault_Handler,
    0,
    0,
    0,
    0, /* Reserved */
    (uint32_t *)SVC_Handler,
    (uint32_t *)DebugMon_Handler,
    0, /* Reserved */
    (uint32_t *)PendSV_Handler,
    (uint32_t *)SysTick_Handler,

    /* External Interrupts (STM32F4 Specific) */
    0,
    0,
    0,
    0,
    0,
    0,                            /* WWDG, PVD, etc. (Omitted for brevity) */
    (uint32_t *)EXTI0_IRQHandler, /* EXTI Line 0 */
    (uint32_t *)EXTI1_IRQHandler, /* EXTI Line 1 */

};

/**
 * @brief  Hardware Reset Entry Point
 */
void Reset_Handler(void)
{
    /* 1. Copy .data section from FLASH to RAM */
    uint32_t size = (uint32_t)&_edata - (uint32_t)&_sdata;
    uint8_t *pDst = (uint8_t *)&_sdata;
    uint8_t *pSrc = (uint8_t *)&_sidata;

    for (uint32_t i = 0; i < size; i++)
    {
        *pDst++ = *pSrc++;
    }

    /* 2. Initialize .bss section to zero */
    size = (uint32_t)&_ebss - (uint32_t)&_sbss;
    pDst = (uint8_t *)&_sbss;

    for (uint32_t i = 0; i < size; i++)
    {
        *pDst++ = 0;
    }

    /* 3. Call the application's entry point */
    extern int main(void);
    main();

    /* Should not reach here */
    while (1)
        ;
}

/**
 * @brief  Fallback for unimplemented interrupts
 */
void Default_Handler(void)
{
    while (1)
        ;
}

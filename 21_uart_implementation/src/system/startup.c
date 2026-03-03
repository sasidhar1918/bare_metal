#include <stdint.h>

/* Symbols defined in the Linker Script */
extern uint32_t _estack;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sidata;
extern uint32_t _sbss;
extern uint32_t _ebss;

/* Function prototypes from system_stm32.c */
void SystemInit(void);
void System_ConfigureUARTPins(void);
extern int main(void);

/* --- Standard Cortex-M4 Exceptions --- */
void Reset_Handler(void);
void Default_Handler(void);
void NMI_Handler(void)          __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler(void)    __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler(void)    __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler(void)     __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler(void)   __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler(void)          __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler(void)     __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler(void)       __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler(void)      __attribute__ ((weak, alias("Default_Handler")));

/* --- STM32F4 Specific Interrupts --- */
void DMA2_Stream2_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler"))); // RX
void USART1_IRQHandler(void)       __attribute__ ((weak, alias("Default_Handler"))); // Global
void DMA2_Stream7_IRQHandler(void) __attribute__ ((weak, alias("Default_Handler"))); // TX

/* The Vector Table */
__attribute__ ((section(".isr_vector")))
uint32_t *vector_table[] = {
    (uint32_t *)&_estack,             /* 0: Initial Stack Pointer */
    (uint32_t *)Reset_Handler,        /* 1: Reset Handler */
    (uint32_t *)NMI_Handler,
    (uint32_t *)HardFault_Handler,
    (uint32_t *)MemManage_Handler,
    (uint32_t *)BusFault_Handler,
    (uint32_t *)UsageFault_Handler,
    0, 0, 0, 0,                       /* Reserved */
    (uint32_t *)SVC_Handler,
    (uint32_t *)DebugMon_Handler,
    0,                                /* Reserved */
    (uint32_t *)PendSV_Handler,
    (uint32_t *)SysTick_Handler,
    
    /* External Interrupts (Offsets 16+) */
    [16 + 56] = (uint32_t *)DMA2_Stream2_IRQHandler,
    [16 + 37] = (uint32_t *)USART1_IRQHandler,
    [16 + 70] = (uint32_t *)DMA2_Stream7_IRQHandler
};

/**
 * @brief The Reset Handler
 * Logic: Power -> Memory Setup -> Pin Setup -> Main
 */
void Reset_Handler(void) {
    /* 1. Basic Hardware Init (Enable Clocks/Power) */
    SystemInit();

    /* 2. Copy .data from FLASH to RAM */
    uint32_t *pSrc = &_sidata;
    uint32_t *pDst = &_sdata;
    while (pDst < &_edata) {
        *pDst++ = *pSrc++;
    }

    /* 3. Zero out .bss (Uninitialized variables) */
    pDst = &_sbss;
    while (pDst < &_ebss) {
        *pDst++ = 0;
    }

    /* 4. Configure GPIO Pins for UART (Physical Layer) */
    System_ConfigureUARTPins();

    /* 5. Launch Application */
    main();

    /* Safety loop */
    while(1);
}

void Default_Handler(void) {
    /* Trap for unhandled interrupts */
    while(1);
}

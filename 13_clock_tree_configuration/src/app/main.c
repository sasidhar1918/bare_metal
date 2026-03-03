#include "common_defs.h"

/* External functions from our BSP layers */
extern void RCC_Init_168MHz(void);
extern void MCO1_Init(void);

/* Global tick counter */
static volatile uint32_t g_ms_ticks = 0;

/**
 * @brief SysTick ISR
 */
void SysTick_Handler(void) {
    g_ms_ticks++;
}

/**
 * @brief Configures SysTick based on the current system clock
 * @param freq The current CPU frequency in Hz
 */
void SysTick_Update_Freq(uint32_t freq) {
    volatile uint32_t *STK_LOAD = (volatile uint32_t *)0xE000E014;
    volatile uint32_t *STK_VAL  = (volatile uint32_t *)0xE000E018;
    volatile uint32_t *STK_CTRL = (volatile uint32_t *)0xE000E010;

    /* Calculate ticks for 1ms: (Freq / 1000) */
    *STK_LOAD = (freq / 1000UL) - 1;
    *STK_VAL  = 0;
    /* Enable Counter, Interrupt, and Processor Clock */
    *STK_CTRL = 0x07; 
}

/**
 * @brief Simple blocking delay using SysTick
 */
void Delay_Ms(uint32_t ms) {
    uint32_t start = g_ms_ticks;
    while ((g_ms_ticks - start) < ms);
}

int main(void) {
    /* 1. Start with HSI (16MHz) default logic */
    /* Initialize SysTick for 16MHz initially */
    SysTick_Update_Freq(HSI_VALUE);

    /* 2. Configure MCO1 (PA8) */
    /* This allows us to see the clock transition on the pin */
    MCO1_Init();

    /* 3. Reconfigure Clock Tree: HSI -> HSE -> PLL (168MHz) */
    /* This function handles Flash Latency and Bus Prescalers */
    RCC_Init_168MHz();

    /* 4. Validate Peripheral Timing Impact */
    /* IMPORTANT: We must update SysTick for the new 168MHz speed 
       If we didn't do this, Delay_Ms(100) would finish ~10x faster */
    SysTick_Update_Freq(SYSCLOCK_FREQ);

    /* 5. Heartbeat Loop */
    /* We use a GPIO (e.g., PD12) to visually confirm 1Hz toggle */
    volatile uint32_t *RCC_AHB1ENR = (volatile uint32_t *)(0x40023830);
    volatile uint32_t *GPIOD_MODER  = (volatile uint32_t *)(0x40020C00);
    volatile uint32_t *GPIOD_ODR    = (volatile uint32_t *)(0x40020C14);

    *RCC_AHB1ENR |= (1 << 3);           /* Enable GPIOD Clock */
    *GPIOD_MODER |= (1 << (12 * 2));    /* PD12 Output */

    while (1) {
        *GPIOD_ODR ^= (1 << 12);        /* Toggle LED */
        Delay_Ms(500);                  /* 500ms delay */
    }

    return 0;
}

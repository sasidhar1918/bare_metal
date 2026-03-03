#include "common_defs.h"

/**
 * @brief RCC Register Structure
 * Based on the STM32F4 Memory Map at 0x40023800
 */
typedef struct {
    volatile uint32_t CR;            /* 0x00: Clock control register */
    volatile uint32_t PLLCFGR;       /* 0x04: PLL configuration register */
    volatile uint32_t CFGR;          /* 0x08: Clock configuration register */
    volatile uint32_t CIR;           /* 0x0C: Clock interrupt register */
    volatile uint32_t AHB1RSTR;      /* 0x10: AHB1 peripheral reset register */
    // ... skipping offsets to Flash interface ...
    uint32_t reserved[10];
    volatile uint32_t APB1ENR;       /* 0x40: APB1 peripheral clock enable */
    volatile uint32_t APB2ENR;       /* 0x44: APB2 peripheral clock enable */
} RCC_Regs;

#define RCC ((RCC_Regs *)RCC_BASE)

/**
 * @brief Flash Interface Register Structure
 * Necessary for high-speed clocking (Wait States)
 */
typedef struct {
    volatile uint32_t ACR;           /* 0x00: Access control register */
} Flash_Regs;

#define FLASH ((Flash_Regs *)FLASH_CNTRL_BASE)

void RCC_Init_168MHz(void) {
    /* 1. Enable HSE (External Crystal) */
    RCC->CR |= (1 << 16); // HSEON
    while (!(RCC->CR & (1 << 17))); // Wait for HSERDY

    /* 2. Configure Flash Latency 
     * Required for 168MHz at 3.3V (5 Wait States) 
     * Address: 0x40023C00
     */
    FLASH->ACR = (1 << 8) | (1 << 9) | (1 << 10) | 5; // Prefetch, Instruction/Data Cache, 5WS

    /* 3. Configure Bus Prescalers */
    RCC->CFGR |= (0 << 4);  /* AHB  = SYSCLK / 1 (168MHz) */
    RCC->CFGR |= (5 << 10); /* APB1 = AHB    / 4 (42MHz Max) */
    RCC->CFGR |= (4 << 13); /* APB2 = AHB    / 2 (84MHz Max) */

    /* 4. Configure PLL 
     * Target: 168MHz
     * Formula: PLL_VCO = (HSE / M) * N
     * SYSCLK = PLL_VCO / P
     * Settings: M=8, N=336, P=2 => (8MHz / 8) * 336 / 2 = 168MHz
     */
    RCC->PLLCFGR = (8 << 0)          /* PLLM = 8 */
                 | (336 << 6)        /* PLLN = 336 */
                 | (0 << 16)         /* PLLP = 2 (00 binary) */
                 | (1 << 22)         /* PLL Source = HSE */
                 | (7 << 24);        /* PLLQ = 7 (for USB 48MHz) */

    /* 5. Enable PLL */
    RCC->CR |= (1 << 24); // PLLON
    while (!(RCC->CR & (1 << 25))); // Wait for PLLRDY

    /* 6. Switch System Clock to PLL */
    RCC->CFGR |= (2 << 0); // SW = PLL
    while (((RCC->CFGR >> 2) & 0x3) != 0x2); // Wait for SWS to show PLL
}

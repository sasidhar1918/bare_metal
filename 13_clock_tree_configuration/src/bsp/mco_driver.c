#include "common_defs.h"

/**
 * @brief GPIO Register Structure
 * Based on gpioPortA @ 0x40020000 from .repl
 */
typedef struct {
    volatile uint32_t MODER;    /* Mode register */
    volatile uint32_t OTYPER;   /* Output type register */
    volatile uint32_t OSPEEDR;  /* Output speed register */
    volatile uint32_t PUPDR;    /* Pull-up/pull-down register */
    volatile uint32_t IDR;      /* Input data register */
    volatile uint32_t ODR;      /* Output data register */
    volatile uint32_t BSRR;     /* Bit set/reset register */
    volatile uint32_t LCKR;     /* Configuration lock register */
    volatile uint32_t AFR[2];   /* Alternate function registers [0]=Low, [1]=High */
} GPIO_Regs;

#define GPIOA ((GPIO_Regs *)GPIOA_BASE)
#define RCC_CFGR *(volatile uint32_t *)(RCC_BASE + 0x08)

/**
 * @brief Configure PA8 as MCO1 to output the PLL clock
 */
void MCO1_Init(void) {
    /* 1. Enable GPIOA Clock (RCC AHB1ENR bit 0) */
    volatile uint32_t *RCC_AHB1ENR = (volatile uint32_t *)(RCC_BASE + 0x30);
    *RCC_AHB1ENR |= (1 << 0);

    /* 2. Configure PA8 for Alternate Function Mode (Mode 10) */
    GPIOA->MODER &= ~(3 << (8 * 2));
    GPIOA->MODER |=  (2 << (8 * 2));

    /* 3. Set High Speed for the pin (Speed 11) */
    GPIOA->OSPEEDR |= (3 << (8 * 2));

    /* 4. Map PA8 to AF0 (MCO1 is typically AF0 on STM32F4) */
    /* PA8 is in High Register (AFR[1]), pin index 0 (8-8) */
    GPIOA->AFR[1] &= ~(0xF << (0 * 4)); 
    GPIOA->AFR[1] |=  (0 << (0 * 4)); 

    /* 5. Select PLL as MCO1 source and set prescaler to 4 */
    /* MCO1 Source: PLL (binary 11) bits 21-22 */
    /* MCO1 Prescaler: Div 4 (binary 110) bits 24-26 */
    RCC_CFGR &= ~((0x3 << 21) | (0x7 << 24));
    RCC_CFGR |=  ((0x3 << 21) | (0x6 << 24));
}

#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

// Base Addresses for STM32F407 (Adjust if using a different F4)
#define RCC_BASE      0x40023800
#define GPIOA_BASE    0x40020000
#define GPIOD_BASE    0x40020C00
#define SYSCFG_BASE   0x40013800

// Register Offsets
#define RCC_AHB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB2ENR   (*(volatile uint32_t *)(RCC_BASE + 0x44))

typedef struct {
    volatile uint32_t MODER;    /* Mode register */
    volatile uint32_t OTYPER;   /* Output type register */
    volatile uint32_t OSPEEDR;  /* Output speed register */
    volatile uint32_t PUPDR;    /* Pull-up/pull-down register */
    volatile uint32_t IDR;      /* Input data register */
    volatile uint32_t ODR;      /* Output data register */
    volatile uint32_t BSRR;     /* Bit set/reset register */
} GPIO_Regs;

#define GPIOA ((GPIO_Regs *)GPIOA_BASE)
#define GPIOD ((GPIO_Regs *)GPIOD_BASE)

// Function Prototypes
void GPIO_Init(void);
void LED_Toggle(void);
uint8_t Button_Read(void);

#endif

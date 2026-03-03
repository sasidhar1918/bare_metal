#include <stdint.h>

/* Register Base Addresses for STM32F407 */
#define RCC_BASE      0x40023800
#define GPIOA_BASE    0x40020000
#define USART1_BASE   0x40011000
#define DMA2_BASE     0x40026400

/* RCC (Reset and Clock Control) Registers */
#define RCC_AHB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB2ENR   (*(volatile uint32_t *)(RCC_BASE + 0x44))

/**
 * @brief  Initializes the System Clock and Peripheral Clocks.
 * In Renode, we usually stick to the internal RC oscillator 
 * to keep the simulation simple, but we must enable peripheral power.
 */
void SystemInit(void) {
    /* 1. Enable GPIOA Clock (UART1 pins PA9=TX, PA10=RX) */
    RCC_AHB1ENR |= (1 << 0); 

    /* 2. Enable USART1 Clock */
    RCC_APB2ENR |= (1 << 4);

    /* 3. Enable DMA2 Clock (Required for DMA-driven API) */
    RCC_AHB1ENR |= (1 << 22);
}

/**
 * @brief  Configures the GPIO pins for UART1 functionality.
 * This belongs in the system layer as it's part of the board setup.
 */
void System_ConfigureUARTPins(void) {
    /* Pointer to GPIOA Moder and Alternate Function registers */
    volatile uint32_t *GPIOA_MODER = (uint32_t *)(GPIOA_BASE + 0x00);
    volatile uint32_t *GPIOA_AFRH  = (uint32_t *)(GPIOA_BASE + 0x24);

    /* Set PA9 and PA10 to Alternate Function mode (bits 10) */
    *GPIOA_MODER &= ~((3 << 18) | (3 << 20)); // Clear
    *GPIOA_MODER |=  ((2 << 18) | (2 << 20)); // Set to AF

    /* Set AF7 (USART1) for PA9 and PA10 */
    // Each pin gets 4 bits in AFRH. PA9 is index 1, PA10 is index 2.
    *GPIOA_AFRH &= ~((0xF << 4) | (0xF << 8)); // Clear
    *GPIOA_AFRH |=  ((0x7 << 4) | (0x7 << 8)); // Set AF7
}

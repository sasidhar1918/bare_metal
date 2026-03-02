#include "gpio.h"

void GPIO_Init(void) {
    // 1. Enable Clocks for GPIOA (Button) and GPIOD (LED)
    RCC_AHB1ENR |= (1 << 0); // GPIOA EN
    RCC_AHB1ENR |= (1 << 3); // GPIOD EN
    
    // 2. Configure PD12 as Output (LED)
    GPIOD->MODER &= ~(3 << (12 * 2)); // Clear bits
    GPIOD->MODER |=  (1 << (12 * 2)); // Set as General Purpose Output
    
    // 3. Configure PA0 as Input (Button)
    GPIOA->MODER &= ~(3 << (0 * 2));  // 00 = Input Mode
    GPIOA->PUPDR &= ~(3 << (0 * 2));  // Clear pull-up/down (assume external pull-down)
    // Note: If your button floats, use: GPIOA->PUPDR |= (2 << (0 * 2)); // Pull-down

    // 4. Configure EXTI for PA0
    // Enable SYSCFG clock to manage interrupts
    RCC_APB2ENR |= (1 << 14); 

    // Link PA0 to EXTI0 Line in SYSCFG_EXTICR1
    volatile uint32_t *EXTICR1 = (volatile uint32_t *)(SYSCFG_BASE + 0x08);
    *EXTICR1 &= ~(0xF << 0); // Map EXTI0 to Port A
    
    // Configure EXTI Registers (Direct Access)
    volatile uint32_t *EXTI_IMR  = (volatile uint32_t *)(0x40013C00 + 0x00);
    volatile uint32_t *EXTI_RTSR = (volatile uint32_t *)(0x40013C00 + 0x08);
    
    *EXTI_IMR  |= (1 << 0);  // Unmask EXTI0
    *EXTI_RTSR |= (1 << 0);  // Trigger on Rising Edge
    
    // 5. Enable Interrupt in NVIC (Interrupt ID for EXTI0 is 6)
    volatile uint32_t *NVIC_ISER0 = (volatile uint32_t *)(0xE000E100);
    *NVIC_ISER0 |= (1 << 6);
}

void LED_Toggle(void) {
    GPIOD->ODR ^= (1 << 12);
}

uint8_t Button_Read(void) {
    return (GPIOA->IDR & (1 << 0)) ? 1 : 0;
}

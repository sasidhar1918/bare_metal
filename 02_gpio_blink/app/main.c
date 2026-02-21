#include <stdint.h>

/* Register Base Addresses - STM32F4 Series */
#define RCC_BASE      0x40023800UL
#define GPIOA_BASE    0x40020000UL

/* Register Offsets */
#define RCC_AHB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define GPIOA_MODER   (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_OTYPER  (*(volatile uint32_t *)(GPIOA_BASE + 0x04))
#define GPIOA_ODR     (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

#define GPIOAEN       (1UL << 0)
#define LED_PIN       (1UL << 5) // PA5 is the standard Netduino LED

int main(void) {
    // 1. Enable GPIOA clock
    RCC_AHB1ENR |= GPIOAEN;

    // 2. Set PA5 to Output (01)
    GPIOA_MODER &= ~(3UL << (5 * 2)); 
    GPIOA_MODER |=  (1UL << (5 * 2));

    // 3. Set PA5 to Push-Pull
    GPIOA_OTYPER &= ~(1UL << 5);

    while (1) {
        // Toggle LED
        GPIOA_ODR ^= LED_PIN;

        // Delay - marked volatile so the emulator doesn't skip it
        for (volatile int i = 0; i < 100000; i++);
    }

    return 0; // Standard C compliance
}

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "debounce_fsm.h"

/* Global System Tick Counter */
static volatile uint32_t g_system_ticks = 0;

/**
 * @brief SysTick Interrupt Handler
 * Triggered every 1ms (if configured correctly)
 */
void SysTick_Handler(void) {
    g_system_ticks++;
}

/**
 * @brief EXTI Line 0 Interrupt Handler (PA0 Button)
 */
void EXTI0_IRQHandler(void) {
    // Check if the interrupt pending bit for Line 0 is set
    volatile uint32_t *EXTI_PR = (volatile uint32_t *)(0x40013C00 + 0x14);
    
    if (*EXTI_PR & (1 << 0)) {
        // Clear the pending bit by writing a 1 to it
        *EXTI_PR |= (1 << 0);
        
        // Signal the FSM that an edge was detected
        g_button_interrupt_occurred = true;
    }
}

/**
 * @brief Simple SysTick Configuration
 * Configures the system to interrupt every 1ms
 */
void SysTick_Config(uint32_t ticks) {
    volatile uint32_t *STK_LOAD = (volatile uint32_t *)0xE000E014;
    volatile uint32_t *STK_VAL  = (volatile uint32_t *)0xE000E018;
    volatile uint32_t *STK_CTRL = (volatile uint32_t *)0xE000E010;

    *STK_LOAD = ticks - 1;
    *STK_VAL  = 0;
    // Enable Counter, Interrupt, and use Processor Clock
    *STK_CTRL = 0x07; 
}

int main(void) {
    // 1. Hardware Setup
    GPIO_Init();
    
    // 2. Initialize System Timer (Assuming 16MHz internal clock for 1ms tick)
    // 16,000,000 / 1000 = 16,000
    SysTick_Config(16000);

    // 3. Initialize Software State Machine
    Debounce_Init();

    /* Infinite Loop */
    while (1) {
        // Run the Debounce FSM continuously
        // It uses the non-blocking g_system_ticks for timing
        if (Debounce_Update(g_system_ticks)) {
            // This block only executes once per VALID button press
            LED_Toggle();
        }

        // You can perform other non-blocking tasks here
        // The LED will only toggle when the FSM confirms a clean press
    }

    return 0;
}

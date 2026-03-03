#include <stdint.h>
#include <string.h>
#include "uart_driver.h"

/* * 3-Bit Control Variable (Simulated "DIP Switch" for Renode)
 * 000: Blocking
 * 001: Interrupt RX
 * 100: DMA RX
 * etc.
 */
volatile uint8_t CONFIG_MODE = MODE_BLOCKING; 

void delay(uint32_t count) {
    while(count--) { __asm("nop"); }
}

int main(void) {
    char msg[] = "UART Driver Started. Testing Mode...\r\n";
    uint8_t rx_byte;
    char overflow_msg[] = "!!! OVERFLOW DETECTED !!!\r\n";

    /* Initialize UART with the selected mode */
    UART_Init(115200, (UART_Mode_t)CONFIG_MODE);

    /* Initial Transmission */
    UART_Transmit((uint8_t*)msg, strlen(msg));

    while(1) {
        /* * Logic: Echo back whatever is received.
         * If in Interrupt/DMA mode, this reads from the Ring Buffer.
         * If in Blocking mode, this waits for a byte.
         */
        if (UART_Receive(&rx_byte, 1) > 0) {
            // Echo the byte back to show it worked
            UART_Transmit(&rx_byte, 1);
        }

        /* Skill: Check for Overflow Handling */
        if (UART_CheckOverflow()) {
            UART_Transmit((uint8_t*)overflow_msg, strlen(overflow_msg));
        }

        /* Optional: Small delay to prevent CPU saturation in simulation */
        delay(1000);
    }

    return 0;
}

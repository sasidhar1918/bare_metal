#include "stm32f4_uart.h"

/**
 * @brief Basic UART configuration.
 * For STM32F4 @ 16MHz (typical Renode default clock)
 */
void HAL_UART_Init(uint32_t baudrate) {
    /* 1. Reset CR1 (Stops UART) */
    USART1->CR1 = 0;

    /* 2. Configure Baud Rate 
     * Formula: BRR = PCLK / (16 * Baudrate)
     * For 115200 @ 16MHz, BRR ~ 0x8B
     */
    if (baudrate == 115200) {
        USART1->BRR = 0x8B; 
    } else {
        USART1->BRR = 0x683; // 9600 @ 16MHz (Fallback)
    }

    /* 3. Enable Transmitter, Receiver and UART Peripheral */
    USART1->CR1 |= (UART_CR1_TE | UART_CR1_RE | UART_CR1_UE);
}

void HAL_UART_WriteByte(uint8_t data) {
    USART1->DR = data;
}

uint8_t HAL_UART_ReadByte(void) {
    return (uint8_t)(USART1->DR & 0xFF);
}

bool HAL_UART_IsTxEmpty(void) {
    return (USART1->SR & UART_SR_TXE);
}

bool HAL_UART_IsRxReady(void) {
    return (USART1->SR & UART_SR_RXNE);
}

/**
 * @brief Helper to turn Hardware Interrupts on/off
 */
void HAL_UART_EnableInterrupts(bool rx, bool tx) {
    if (rx) USART1->CR1 |= UART_CR1_RXNEIE;
    else    USART1->CR1 &= ~UART_CR1_RXNEIE;

    if (tx) USART1->CR1 |= UART_CR1_TXEIE;
    else    USART1->CR1 &= ~UART_CR1_TXEIE;
}

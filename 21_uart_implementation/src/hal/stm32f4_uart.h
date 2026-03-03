#ifndef STM32F4_UART_HAL_H
#define STM32F4_UART_HAL_H

#include <stdint.h>
#include <stdbool.h>

/* USART1 Register Map Structure (STM32F407) */
typedef struct {
    volatile uint32_t SR;    /* Status register           (Offset 0x00) */
    volatile uint32_t DR;    /* Data register             (Offset 0x04) */
    volatile uint32_t BRR;   /* Baud rate register        (Offset 0x08) */
    volatile uint32_t CR1;   /* Control register 1        (Offset 0x0C) */
    volatile uint32_t CR2;   /* Control register 2        (Offset 0x10) */
    volatile uint32_t CR3;   /* Control register 3        (Offset 0x14) */
    volatile uint32_t GTPR;  /* Guard time/prescaler reg  (Offset 0x18) */
} USART_Regs_t;

/* Memory address of USART1 on APB2 bus */
#define USART1 ((USART_Regs_t *)0x40011000)

/* Bit Definitions for SR */
#define UART_SR_RXNE (1 << 5)  /* Read Data Register Not Empty */
#define UART_SR_TC   (1 << 6)  /* Transmission Complete */
#define UART_SR_TXE  (1 << 7)  /* Transmit Data Register Empty */
#define UART_SR_ORE  (1 << 3)  /* Overrun Error */

/* Bit Definitions for CR1 */
#define UART_CR1_UE     (1 << 13) /* UART Enable */
#define UART_CR1_TE     (1 << 3)  /* Transmitter Enable */
#define UART_CR1_RE     (1 << 2)  /* Receiver Enable */
#define UART_CR1_RXNEIE (1 << 5)  /* RXNE Interrupt Enable */
#define UART_CR1_TXEIE  (1 << 7)  /* TXE Interrupt Enable */

/* HAL Functions */
void HAL_UART_Init(uint32_t baudrate);
void HAL_UART_WriteByte(uint8_t data);
uint8_t HAL_UART_ReadByte(void);
bool HAL_UART_IsTxEmpty(void);
bool HAL_UART_IsRxReady(void);
void HAL_UART_EnableInterrupts(bool rx, bool tx);

#endif

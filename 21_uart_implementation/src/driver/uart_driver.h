#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "ring_buffer.h"

/* 3-Bit Control Definitions */
typedef enum {
    MODE_BLOCKING = 0,   /* 000: TX Block, RX Block */
    MODE_INT_RX   = 1,   /* 001: TX Block, RX Interrupt */
    MODE_INT_TX   = 2,   /* 010: TX Interrupt, RX Block */
    MODE_INT_FULL = 3,   /* 011: TX Interrupt, RX Interrupt */
    MODE_DMA_RX   = 4,   /* 100: TX Block, RX DMA */
    MODE_DMA_TX   = 5,   /* 101: TX DMA, RX Block */
    MODE_DMA_FULL = 7    /* 111: TX DMA, RX DMA */
} UART_Mode_t;

/* Public API */
void UART_Init(uint32_t baudrate, UART_Mode_t mode);
void UART_Transmit(uint8_t *data, uint16_t size);
uint16_t UART_Receive(uint8_t *data, uint16_t size);
bool UART_CheckOverflow(void);

#endif

#include "uart_driver.h"
#include "stm32f4_uart.h"
#include "stm32f4_dma.h"

/* Private Driver State */
static RingBuffer_t rx_rb;
static UART_Mode_t current_mode;
static uint8_t *tx_ptr;
static uint16_t tx_remaining = 0;

void UART_Init(uint32_t baudrate, UART_Mode_t mode) {
    current_mode = mode;
    RingBuffer_Init(&rx_rb);
    HAL_UART_Init(baudrate);

    /* RX Setup based on mode */
    if (mode & 0x01) { // Interrupt-driven RX (Bits 0 or 1 set)
        HAL_UART_EnableInterrupts(true, false);
    } 
    
    if (mode & 0x04) { // DMA-driven RX (Bit 2 set)
        HAL_DMA_Init_RX((uint32_t)&USART1->DR, (uint32_t)rx_rb.storage, RING_BUFFER_SIZE);
        USART1->CR3 |= (1 << 6); // DMAR: DMA enable receiver
    }
}

/**
 * @brief TRANSMIT DISPATCHER
 * Decides whether to use Blocking, IT, or DMA based on current_mode
 */
void UART_Transmit(uint8_t *data, uint16_t size) {
    // 1. DMA Method (Highest priority in this logic)
    if (current_mode & 0x04) { 
        USART1->CR3 |= (1 << 7); // DMAT: DMA enable transmitter
        HAL_DMA_Init_TX((uint32_t)&USART1->DR, (uint32_t)data, size);
        HAL_DMA_Start_TX(size);
        while(!HAL_DMA_IsTxComplete()); // Wait for DMA to finish
        HAL_DMA_ClearTxFlags();
    }
    // 2. Interrupt Method
    else if (current_mode & 0x02) {
        tx_ptr = data;
        tx_remaining = size;
        HAL_UART_EnableInterrupts(false, true); // Enable TXEIE
        while(tx_remaining > 0); // Wait for ISR to empty the buffer
    }
    // 3. Blocking Method
    else {
        for(uint16_t i = 0; i < size; i++) {
            while(!HAL_UART_IsTxEmpty());
            HAL_UART_WriteByte(data[i]);
        }
    }
}

/**
 * @brief RECEIVE DISPATCHER
 */
uint16_t UART_Receive(uint8_t *data, uint16_t size) {
    uint16_t count = 0;

    // If using Background RX (Interrupt or DMA), read from Ring Buffer
    if (current_mode & 0x05) { 
        while (count < size) {
            if (RingBuffer_Get(&rx_rb, &data[count])) {
                count++;
            } else {
                break; // Buffer empty
            }
        }
    } 
    // Blocking RX
    else {
        for(uint16_t i = 0; i < size; i++) {
            while(!HAL_UART_IsRxReady());
            data[i] = HAL_UART_ReadByte();
            count++;
        }
    }
    return count;
}

/**
 * @brief INTERRUPT SERVICE ROUTINE (ISR)
 * This is called by the startup.c vector table
 */
void USART1_IRQHandler(void) {
    // Check RX Interrupt
    if (HAL_UART_IsRxReady()) {
        uint8_t val = HAL_UART_ReadByte();
        RingBuffer_Put(&rx_rb, val); // Skill: Overflow handled inside Put
    }

    // Check TX Interrupt (TXE - Transmit Data Register Empty)
    if ((USART1->SR & UART_SR_TXE) && (USART1->CR1 & UART_CR1_TXEIE)) {
        if (tx_remaining > 0) {
            HAL_UART_WriteByte(*tx_ptr++);
            tx_remaining--;
        } else {
            HAL_UART_EnableInterrupts(false, false); // Done, disable TXEIE
        }
    }
}

bool UART_CheckOverflow(void) {
    bool status = rx_rb.overflow;
    rx_rb.overflow = false; // Reset after reading
    return status;
}

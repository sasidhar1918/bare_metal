#include "stm32f4_dma.h"

/**
 * @brief Configures DMA2 Stream 7 for USART1 TX
 */
void HAL_DMA_Init_TX(uint32_t pAddr, uint32_t mAddr, uint16_t size) {
    DMA2_S7->CR &= ~DMA_CR_EN; // Disable to configure
    while(DMA2_S7->CR & DMA_CR_EN); // Wait for disable

    DMA2_S7->PAR  = pAddr; // Destination: USART1->DR
    DMA2_S7->M0AR = mAddr; // Source: Your RAM Buffer
    DMA2_S7->NDTR = size;

    /* Channel 4, Memory Increment, Memory-to-Peripheral, TC Interrupt */
    DMA2_S7->CR = DMA_CR_CH_4 | DMA_CR_MINC | DMA_CR_DIR_M2P | DMA_CR_TCIE;
}

/**
 * @brief Configures DMA2 Stream 2 for USART1 RX (Circular Mode)
 */
void HAL_DMA_Init_RX(uint32_t pAddr, uint32_t mAddr, uint16_t size) {
    DMA2_S2->CR &= ~DMA_CR_EN;
    while(DMA2_S2->CR & DMA_CR_EN);

    DMA2_S2->PAR  = pAddr; // Source: USART1->DR
    DMA2_S2->M0AR = mAddr; // Destination: Ring Buffer Array
    DMA2_S2->NDTR = size;

    /* Channel 4, Memory Increment, Peripheral-to-Memory, Circular Mode */
    DMA2_S2->CR = DMA_CR_CH_4 | DMA_CR_MINC | DMA_CR_DIR_P2M | DMA_CR_CIRC;
    
    DMA2_S2->CR |= DMA_CR_EN; // RX DMA usually stays on
}

void HAL_DMA_Start_TX(uint16_t size) {
    DMA2_S7->CR &= ~DMA_CR_EN;
    while(DMA2_S7->CR & DMA_CR_EN);
    
    DMA2_S7->NDTR = size;
    DMA2_S7->CR |= DMA_CR_EN;
}

bool HAL_DMA_IsTxComplete(void) {
    return (DMA2->HISR & (1 << 27)); // TCIF7 bit in High ISR
}

void HAL_DMA_ClearTxFlags(void) {
    DMA2->HIFCR |= (1 << 27); // Clear TCIF7
}

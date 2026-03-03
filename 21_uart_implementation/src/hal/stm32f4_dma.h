#ifndef STM32F4_DMA_HAL_H
#define STM32F4_DMA_HAL_H

#include <stdint.h>
#include <stdbool.h>

/* DMA Stream Register Map */
typedef struct {
    volatile uint32_t CR;   /* Control register */
    volatile uint32_t NDTR; /* Number of data register */
    volatile uint32_t PAR;  /* Peripheral address register */
    volatile uint32_t M0AR; /* Memory 0 address register */
    volatile uint32_t M1AR; /* Memory 1 address register (for double buffer) */
    volatile uint32_t FCR;  /* FIFO control register */
} DMA_Stream_Regs_t;

/* DMA Base Registers for Interrupt Status/Clear */
typedef struct {
    volatile uint32_t LISR;  /* Low interrupt status register */
    volatile uint32_t HISR;  /* High interrupt status register */
    volatile uint32_t LIFCR; /* Low interrupt flag clear register */
    volatile uint32_t HIFCR; /* High interrupt flag clear register */
} DMA_Base_Regs_t;

#define DMA2      ((DMA_Base_Regs_t *)0x40026400)
#define DMA2_S2   ((DMA_Stream_Regs_t *)0x40026410) /* USART1_RX */
#define DMA2_S7   ((DMA_Stream_Regs_t *)0x400264A0) /* USART1_TX */

/* Bit Definitions for DMA CR */
#define DMA_CR_EN     (1 << 0)  /* Stream Enable */
#define DMA_CR_TCIE   (1 << 4)  /* Transfer Complete Interrupt Enable */
#define DMA_CR_DIR_M2P (1 << 6) /* Direction: Memory-to-Peripheral (TX) */
#define DMA_CR_DIR_P2M (0 << 6) /* Direction: Peripheral-to-Memory (RX) */
#define DMA_CR_MINC   (1 << 10) /* Memory Increment Mode */
#define DMA_CR_CIRC   (1 << 8)  /* Circular Mode */
#define DMA_CR_CH_4   (4 << 25) /* Channel 4 Select */

/* HAL Functions */
void HAL_DMA_Init_TX(uint32_t pAddr, uint32_t mAddr, uint16_t size);
void HAL_DMA_Init_RX(uint32_t pAddr, uint32_t mAddr, uint16_t size);
void HAL_DMA_Start_TX(uint16_t size);
bool HAL_DMA_IsTxComplete(void);
void HAL_DMA_ClearTxFlags(void);

#endif

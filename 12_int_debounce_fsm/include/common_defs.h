#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H

#include <stdint.h>
#include <stdbool.h>

/** * @brief System Clock Constants 
 * Based on the .repl nvic configuration.
 */
#define SYS_CLOCK_HZ      72000000UL  /* Systick frequency set in .repl  */
#define MS_TICK_VAL       (SYS_CLOCK_HZ / 1000UL) 

/** * @brief Memory Map Constants
 * Derived from the STM32F4 peripheral and memory layout.
 */
#define FLASH_START       0x08000000UL /* Base address of Flash [cite: 1] */
#define SRAM_START        0x20000000UL /* Base address of SRAM [cite: 1] */
#define EXTI_BASE         0x40013C00UL /* EXTI controller base  */
#define NVIC_BASE         0xE000E000UL /* NVIC controller base  */

/** * @brief Interrupt Vector Mapping
 * Matches the EXTI to NVIC routing in the .repl file.
 */
#define EXTI0_IRQ_NUMBER  6            /* EXTI0 maps to NVIC IRQ 6  */

/** * @brief Application Logic Constants 
 */
#define DEBOUNCE_TIME_MS  50           /* Software debounce threshold */
#define UNUSED(x)         (void)(x)    /* Macro to suppress unused variable warnings */

/**
 * @brief Standard Function Return Codes
 */
typedef enum {
    STATUS_OK = 0,
    STATUS_ERROR,
    STATUS_BUSY,
    STATUS_TIMEOUT
} Status_t;

#endif /* COMMON_DEFS_H */

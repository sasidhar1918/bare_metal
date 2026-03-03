#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H

#include <stdint.h>
#include <stdbool.h>

/* Clock Source Constants */
#define HSE_VALUE          8000000UL   /* External crystal frequency (8MHz) */
#define HSI_VALUE          16000000UL  /* Internal RC oscillator (16MHz) */

/* Target Frequencies after PLL reconfiguration */
#define SYSCLOCK_FREQ      168000000UL /* Max freq for STM32F407 */
#define AHB_PRESCALER      1
#define APB1_PRESCALER     4           /* 42MHz Max */
#define APB2_PRESCALER     2           /* 84MHz Max */

/* Peripheral Base Addresses from .repl */
#define RCC_BASE           0x40023800UL
#define FLASH_CNTRL_BASE   0x40023C00UL
#define GPIOA_BASE         0x40020000UL

/* Useful Macros */
#define UNUSED(x) (void)(x)

#endif

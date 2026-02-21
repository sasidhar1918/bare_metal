#ifndef SYSTICK_H
#define SYSTICK_H

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdbool.h>

void SysTick_Init(uint32_t system_tick_hz);

/* Blocking: Halts execution */
void SysTick_DelayMs(uint32_t ms);

/* Non-blocking: Periodic check */
typedef uint32_t tick_t;
tick_t SysTick_GetTick(void);
bool SysTick_HasElapsed(tick_t start_tick, uint32_t duration_ms);

#endif

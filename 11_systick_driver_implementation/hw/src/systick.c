#include<systick.h>

#include<stm32f4xx.h>

static volatile uint32_t g_system_ticks =0;


void SysTick_Init(uint32_t system_tick_hz){

    if (SysTick_Config(system_tick_hz/1000)) while(1);
}

void SysTick_Handler(void){
    g_system_ticks++;
}

uint32_t SysTick_GetTick(void){
    return g_system_ticks;
}

void SysTick_DelayMs(uint32_t ms){
    uint32_t start = g_system_ticks;
    while((g_system_ticks-start) < ms);
}

bool SysTick_HasElapsed(uint32_t start_tick, uint32_t duration_ms){
    return (uint32_t)(g_system_ticks-start_tick) >= duration_ms;
}

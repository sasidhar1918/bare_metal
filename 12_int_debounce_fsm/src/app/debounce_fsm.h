#ifndef DEBOUNCE_FSM_H
#define DEBOUNCE_FSM_H

#include <stdint.h>
#include <stdbool.h>

#define DEBOUNCE_THRESHOLD_MS 50  // Minimum stable time to confirm press

typedef enum {
    STATE_IDLE,
    STATE_DEBOUNCING,
    STATE_PRESSED,
    STATE_WAIT_RELEASE
} DebounceState_t;

/* Global flag set by the EXTI ISR */
extern volatile bool g_button_interrupt_occurred;

/* Function Prototypes */
void Debounce_Init(void);
bool Debounce_Update(uint32_t current_tick);

#endif

#include "debounce_fsm.h"
#include "gpio.h"

/* Volatile because it's modified in the ISR */
volatile bool g_button_interrupt_occurred = false;

static DebounceState_t current_state = STATE_IDLE;
static uint32_t start_tick = 0;

void Debounce_Init(void) {
    current_state = STATE_IDLE;
    g_button_interrupt_occurred = false;
}

/**
 * @brief  State Machine to validate button press
 * @param  current_tick: The current system time in ms
 * @return true if a valid "click" was detected, false otherwise
 */
bool Debounce_Update(uint32_t current_tick) {
    bool event_detected = false;

    switch (current_state) {
        case STATE_IDLE:
            if (g_button_interrupt_occurred) {
                g_button_interrupt_occurred = false;
                start_tick = current_tick;
                current_state = STATE_DEBOUNCING;
            }
            break;

        case STATE_DEBOUNCING:
            if ((current_tick - start_tick) >= DEBOUNCE_THRESHOLD_MS) {
                // Check if button is STILL pressed after the noise window
                if (Button_Read() == 1) { 
                    current_state = STATE_PRESSED;
                } else {
                    current_state = STATE_IDLE; // It was just noise
                }
            }
            break;

        case STATE_PRESSED:
            // This is where we confirm the "Click" event
            event_detected = true;
            current_state = STATE_WAIT_RELEASE;
            break;

        case STATE_WAIT_RELEASE:
            // Wait until the user physically lets go of the button
            if (Button_Read() == 0) {
                current_state = STATE_IDLE;
            }
            break;
    }

    return event_detected;
}

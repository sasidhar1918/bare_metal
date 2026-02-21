#include "systick.h"


int main(void) {
    /* Standard CMSIS function to initialize clock system */
    SystemInit();
    SystemCoreClockUpdate();

    /* Initializing the driver */
    SysTick_Init(16000000); // in Hz

    uint32_t last_heartbeat = SysTick_GetTick();

    while (1) {
        /* Non-blocking: Run every 500ms */
        if (SysTick_HasElapsed(last_heartbeat, 500)) {
            // Do something (e.g., Toggle GPIO)
            last_heartbeat = SysTick_GetTick();
        }

        /* Blocking Delay */
        // SysTick_DelayMs(10); 
    }
}

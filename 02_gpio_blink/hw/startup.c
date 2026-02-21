#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

int main(void); // Changed to int

void Reset_Handler(void) {
    // Copy data from Flash to RAM
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    // Zero out BSS
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    main();

    while (1);
}

/* Minimal Vector Table for Emulator */
__attribute__((section(".isr_vector")))
uint32_t vectors[] = {
    (uint32_t)&_estack,
    (uint32_t)Reset_Handler
};

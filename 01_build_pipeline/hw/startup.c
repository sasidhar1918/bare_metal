#include <stdint.h>

extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss, _estack;

void Reset_Handler(void) {
    // 1. Copy .data from Flash to RAM
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    // 2. Initialize .bss to zero
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    // 3. Call Application entry
    extern int main(void);
    main();

    while (1); // Safety loop
}

// Minimal Vector Table
__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) = {
    (void (*)(void))&_estack,
    Reset_Handler
};

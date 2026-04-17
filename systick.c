// systick.c

#include "registers.h"

static volatile unsigned int tick_count = 0; 

void SysTick_Handler(void) { tick_count++; }

unsigned int get_ticks(void) { return tick_count; }

void SysTick_Init(void) {
    SYST_RVR = 16000 - 1; // 16 Mhz -> 16000 cycles = 1ms
    SYST_CVR = 0;     // Clear current value
    SYST_CSR = 0b111;  // use processor clock, enable systick exception, enable counter
}
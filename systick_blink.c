// blink led using systick for delays

#include "registers.h"

void SysTick_Init(void) {
    *(volatile unsigned int*)SYST_RVR = 16000 - 1; // 16 Mhz -> 16000 cycles = 1ms
    *(volatile unsigned int*)SYST_CVR = 0;     // Clear current value
    *(volatile unsigned int*)SYST_CSR = 0x7;  // use processor clock, enable systick exception, enable counter
}

volatile unsigned int tick_count = 0; // goes to .bss
void SysTick_Handler(void) {
    tick_count++;
}


int main(void) {
    RCC_AHB1ENR |= (1 << 0);   // Enable GPIOA clock

    GPIOA_MODER |= (1 << 10);   // General purpose output mode for PA5 (01)
    GPIOA_MODER &= ~(1 << 11);
    GPIOA_OTYPER &= ~(1 << 5); // push-pull for PA5
    
    SysTick_Init();

    unsigned int last_tick = 0; // tick_count at last toggle on/off
    int led_state = 0; // led on/off
    int delay = 2000; // delay in ms

    while (1) {

        if (led_state == 0 && tick_count - last_tick >= delay) {   // if led off and tick_count is >= delay from last turn toggle
            last_tick = tick_count;
            GPIOA_BSRR = (1 << 5); // turn on
            led_state = 1;
        }
        if (led_state == 1 && tick_count - last_tick >= delay) {    // if led on and tick count is >= delay from last 
            last_tick = tick_count;
            GPIOA_BSRR = (1 << (16 + 5)); // turn off
            led_state = 0;
            
        }
    }
}



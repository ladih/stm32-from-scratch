// led blink, systick, usart

#include "registers.h"
#include "led.h"
#include "systick.h"
#include "uart.h"

int main(void) {

    led_init();
    SysTick_Init();
    uart_init();

    unsigned int last_tick = 0; // tick_count at last toggle on/off
    unsigned int current_tick;
    int led_state = 0; // led on/off
    int delay = 10000; // delay in ms

    while (1) {

        current_tick = get_ticks();
        if (current_tick - last_tick >= delay) {
            last_tick = current_tick;

            if (led_state == 0) {
                GPIOA_BSRR = (1 << 5); // turn on
                uart_print("GPIOA_BSRR = (1 << 5)\n\r");
            }
            else {
                GPIOA_BSRR = (1 << (16 + 5)); // turn off
                uart_print("GPIOA_BSRR = (1 << (16 + 5))\n\r");
            }
            led_state = !led_state;
        }
    }
}
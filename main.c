// main.c
// led control and temperature sensing with UART, EXTI, TIM2, SysTick, ADC

#include "main.h"

led_state_t g_led_state = {0, 0, 100, 20, 50, 50, 0, 1, 0};

int main(void) {
    
    led_init();
    SysTick_Init();
    uart_init();
    button_init();  
    tim2_init();
    adc_init();

    // Welcome text
    led_on();
    uart_print("\n\n\n\rWelcome to led blink!\r\n");
    uart_print("Commands:\r\n");
    uart_print("   led on\r\n");
    uart_print("   led off\r\n");
    uart_print("   b <ms> \r\n");
    uart_print("   dim <delay> <duty>\r\n");
    uart_print("   dim2 <num>\r\n");
    uart_print("   b\r\n");
    uart_print("   t\r\n");
    uart_print("> ");

    unsigned int current_tick;

    while (1) {
        
        if (button_pressed) {
            button_pressed = 0;
            g_led_state.dim_flag = 0;
            g_led_state.blink_flag = 1;
            pa5_to_gpio();
            uart_print("***Button press: ");
            if (g_led_state.button_blink_increase == 1) {
                g_led_state.delay *= 2;
                if (g_led_state.delay >= MAX_DELAY) {
                    g_led_state.delay = MAX_DELAY;
                    g_led_state.button_blink_increase = !g_led_state.button_blink_increase;
                }
                uart_print("blink delay increased to ");
                uart_print_int(g_led_state.delay);
            }
            else {
                g_led_state.delay /= 2;
                if (g_led_state.delay <= MIN_DELAY) {
                    g_led_state.delay = MIN_DELAY;
                    g_led_state.button_blink_increase = !g_led_state.button_blink_increase;
                }
                uart_print("blink delay decreased to ");
                uart_print_int(g_led_state.delay);
            }            
            uart_print("ms***\r\n> ");
        }

        if (line_ready) {
            line_ready = 0;
            if (rx_buf[0] == '\0') {
                    uart_print("\r\n> ");
                    continue;
            }
            char buf[BUF_SIZE];
            scpy(buf, (char *)rx_buf); // copy volatile data into non-volatile buffer
            if (!process_command(buf))
                uart_print("\r\nError: invalid command\r\n");
            uart_print("> ");
        }

        if (g_led_state.blink_flag) {
            current_tick = get_ticks();
            if (current_tick - g_led_state.last_tick >= g_led_state.delay) {            // x - y = x + (~y + 1)
                g_led_state.last_tick = current_tick;
                if (g_led_state.led_is_on == 0) {
                    led_on();
                }
                else {
                    led_off();
                }
                g_led_state.led_is_on = !g_led_state.led_is_on;
            }
        }

        if (g_led_state.dim_flag) {
            current_tick = get_ticks();
            if (current_tick - g_led_state.last_tick >= g_led_state.off_time && g_led_state.led_is_on == 0) {
                g_led_state.last_tick = current_tick;
                g_led_state.led_is_on = !g_led_state.led_is_on;
                led_on();
            }
            else if (current_tick - g_led_state.last_tick >= g_led_state.on_time && g_led_state.led_is_on == 1) {
                g_led_state.last_tick = current_tick;
                g_led_state.led_is_on = !g_led_state.led_is_on;
                led_off();
            }
        }
    }
}
// led blink, systick, usart

#include "registers.h"
#include "led.h"
#include "systick.h"
#include "uart.h"

int is_led_command(volatile const char *s) {

    if (s[0] != 'l' || s[1] != 'e' || s[2] != 'd' || s[3] != ' ') {
        return 0;
    }
    if (s[4] >= '0' && s[4] <= '9') {
        return 1;
    }
    return 0;
}

int stoi(const char *s) {
    int val = 0;
    while (*s != '\0') {
        val = 10 * val + (*s - '0');
        s++;
    }
    return val;

}

int get_delay(volatile const char *s) {

    char delay[5];
    s += 4;
    int i = 0;
    while (*s >= '0' && *s <= '9') {
        delay[i] = *s;
        s++;
        i++;
    }
    delay[i] = '\0';

    return stoi(delay);
}


int scmp(volatile char *s1, const char *s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 == *s2;
}

int main(void) {

    led_init();
    SysTick_Init();
    uart_init();

    uart_print("\n\n\nWelcome to led blink!\r\n");
    uart_print("Commands:\r\n");
    uart_print("led on\r\n");
    uart_print("led off\r\n");
    uart_print("led [delay] \r\n\n");

    int blink_flag = 0;

    unsigned int last_tick = 0; // tick_count at last toggle on/off
    unsigned int current_tick;
    int led_state = 0; // led on/off
    int delay = 50; // delay in ms

    while (1) {

        if (line_ready) {

            // newline to write "led turned on" etc on.
            while (!(USART2_SR & (1 << 7))); // wait for TXE (Transmit data register empty)
            USART2_DR = '\n';                   // write to DR

            if (scmp(rx_buf, "led on")) {
                blink_flag = 0;
                GPIOA_BSRR = (1 << 5); // turn on
                uart_print("***Led turned on***\r\n");
            }
            else if (scmp(rx_buf, "led off")) {
                blink_flag = 0;
                GPIOA_BSRR = (1 << (16 + 5)); // turn off
                uart_print("***Led turned off***\r\n");
            }
            else if (is_led_command(rx_buf)) {
                delay = get_delay(rx_buf);
                blink_flag = 1;
                uart_print("***Blinking with ");
                uart_print_int(delay);
                uart_print(" ms delay***\r\n");

            }
            else {
                uart_print("---Invalid command---\r\n");
            }
            line_ready = 0;
        }

        if (blink_flag) {
            current_tick = get_ticks();
            if (current_tick - last_tick >= delay) {
                last_tick = current_tick;
                if (led_state == 0) {
                    GPIOA_BSRR = (1 << 5); // turn on
                }
                else {
                    GPIOA_BSRR = (1 << (16 + 5)); // turn off
                }
                led_state = !led_state;
            }
        }
    }
}
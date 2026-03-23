// led blink, systick, usart, EXTI

#include "registers.h"
#include "led.h"
#include "systick.h"
#include "uart.h"
#include "button.h"

#define MIN_DELAY 20
#define MAX_DELAY 2000

int is_led_command(volatile const char *s);
int get_delay(volatile const char *s);
int scmp(volatile char *s1, const char *s2);
int stoi(const char *s);

int main(void) {

    led_init();
    SysTick_Init();
    uart_init();
    button_init();

    uart_print("\n\n\n\rWelcome to led blink!\r\n");
    uart_print("Commands:\r\n");
    uart_print("led on\r\n");
    uart_print("led off\r\n");
    uart_print("led [delay] \r\n\n");
    uart_print("> ");

    int blink_flag = 0;

    unsigned int last_tick = 0; // tick_count at last toggle on/off
    unsigned int current_tick;
    int led_state = 0; // led on/off
    int delay = 100; // delay in ms

    int button_blink_increase = 0;

    while (1) {
        if (button_pressed) {
            button_pressed = 0;

            if (blink_flag == 0) blink_flag = 1;

            uart_print("***Button press: ");
            if (button_blink_increase == 1) {
                delay *= 2;
                if (delay >= MAX_DELAY) {
                    delay = MAX_DELAY;
                    button_blink_increase = !button_blink_increase;
                }
                uart_print("blink delay increased to ");
                uart_print_int(delay);
            }
            else {
                delay /= 2;
                if (delay <= MIN_DELAY) {
                    delay = MIN_DELAY;
                    button_blink_increase = !button_blink_increase;
                }
                uart_print("blink delay decreased to ");
                uart_print_int(delay);
            }            
            uart_print(" ms***\r\n> ");
        }

        if (line_ready) {
            line_ready = 0;
            if (rx_buf[0] == '\0') {
                    line_ready = 0;
                    uart_print("\r\n> ");
                    continue;
            }

            uart_print("\r\n");

            if (scmp(rx_buf, "led on")) {
                blink_flag = 0;
                led_on();
                uart_print("***Led turned on***\r\n");
            }
            else if (scmp(rx_buf, "led off")) {
                blink_flag = 0;
                led_off();
                uart_print("***Led turned off***\r\n");
            }
            else if (is_led_command(rx_buf)) {
                delay = get_delay(rx_buf);
                if (delay < MIN_DELAY) delay = MIN_DELAY;
                blink_flag = 1;
                uart_print("***Blinking with ");
                uart_print_int(delay);
                uart_print(" ms delay***\r\n");

            }
            else {
                uart_print("Error: invalid command\r\n");
            }
            uart_print("> ");
        }

        if (blink_flag) {
            current_tick = get_ticks();
            if (current_tick - last_tick >= delay) {
                last_tick = current_tick;
                if (led_state == 0) {
                    led_on();
                }
                else {
                    led_off();
                }
                led_state = !led_state;
            }
        }
    }
}


// functions
int is_led_command(volatile const char *s) {
    // check if input string is a valid led command
    if (s[0] != 'l' || s[1] != 'e' || s[2] != 'd' || s[3] != ' ') {
        return 0;
    }
    if (s[4] >= '0' && s[4] <= '9') {
        return 1;
    }
    return 0;
}

int stoi(const char *s) {  // parameter if non-volatile because "delay" from get_delay is local
    // integer string to int
    int val = 0;
    while (*s != '\0') {
        val = 10 * val + (*s - '0');
        s++;
    }
    return val;
}

int get_delay(volatile const char *s) {
    // input starts with "led i", where "i" is a number 0 to 9.
    int max_num_chars = 6;
    char delay[max_num_chars];
    s += 4; // skip "led "
    int i = 0;
    while (*s >= '0' && *s <= '9' && i < max_num_chars - 1) {
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

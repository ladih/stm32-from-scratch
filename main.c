// led blink, systick, usart, EXTI

#include "registers.h"
#include "led.h"
#include "systick.h"
#include "uart.h"
#include "button.h"

#define MIN_DELAY 20
#define MAX_DELAY 2000


int starts_with(volatile const char *s, const char *prefix);
int is_led_command(volatile const char *s);
int parse_int(volatile const char *s);
int scmp(volatile char *s1, const char *s2);
int stoi(const char *s);


void tim2_init() {
    RCC_APB1ENR |= (1 << 0);
    TIM2_PSC = 15;
    TIM2_ARR = 999;
    TIM2_CCR1 = 500;

    TIM2_CCMR1 |= 0b110 << 4;
    TIM2_CCMR1 |= 1 << 3;
    TIM2_CCER |= 1 << 0;
    TIM2_CR1 |= 1 << 7;
    TIM2_CR1 |= 1 << 0;
}


void pa5_to_af(void) {
    GPIOA_MODER &= ~(0b11 << 10);
    GPIOA_MODER |= (0b10 << 10); // alternate function

    GPIOA_AFRL  &= ~(0xF << 20);   // clear AF bits for PA5
    GPIOA_AFRL  |=  (0x1 << 20);   // AF1 = TIM2
}

void pa5_to_gpio(void) {
    GPIOA_MODER &= ~(0b11 << 10);
    GPIOA_MODER |= (0b01 << 10); // output
}



int main(void) {

    led_init();
    SysTick_Init();
    uart_init();
    button_init();  
    tim2_init();

    uart_print("\n\n\n\rWelcome to led blink!\r\n");
    uart_print("Commands:\r\n");
    uart_print("   led on\r\n");
    uart_print("   led off\r\n");
    uart_print("   led <ms> \r\n");
    uart_print("   dim <delay> <duty>\r\n");
    uart_print("   b\r\n");
    uart_print("   status\r\n\n");
    uart_print("> ");

    int blink_flag = 0; // LED blinks if blink_flag == 1

    unsigned int last_tick = 0; // tick_count at last toggle on/off
    unsigned int current_tick;

    int led_state = 0; // led on/off
    int delay = 100; // time in ms the LED stays in each led_state

    int button_blink_increase = 0;


    int dim_flag = 0;
    int duty = 20;
    int on_time = delay * duty / 100;
    int off_time = delay - on_time;


    while (1) {
        if (button_pressed) {
            button_pressed = 0;
            dim_flag = 0;

            pa5_to_gpio();

            if (blink_flag == 0) blink_flag = 1; // start blinking if currently no blink when button is pressed (e.g at reset)

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
            uart_print("ms***\r\n> ");
        }

        if (line_ready) {
            line_ready = 0;
            if (rx_buf[0] == '\0') {
                    uart_print("\r\n> ");
                    continue;
            }

            uart_print("\r\n");

            if (scmp(rx_buf, "led on"))  { pa5_to_gpio(); blink_flag=0; dim_flag=0; led_on(); uart_print("***Led turned on***\r\n");}
            else if (scmp(rx_buf, "led off"))  { pa5_to_gpio(); blink_flag=0; dim_flag=0; led_off(); uart_print("***Led turned off***\r\n");}

            else if (starts_with(rx_buf, "led ") && rx_buf[4] >= '0' && rx_buf[4] <= '9') {
                pa5_to_gpio();
                dim_flag = 0;
                blink_flag = 1;
                delay = parse_int(rx_buf + 4);
                if (delay < MIN_DELAY) delay = MIN_DELAY;
                uart_print("***Blinking with ");
                uart_print_int(delay);
                uart_print("ms delay***\r\n");
            }

            else if (starts_with(rx_buf, "dim2 ") && rx_buf[5] >= '0' && rx_buf[5] <= '9') {
                dim_flag = 0;
                blink_flag = 0;
                pa5_to_af();
                int CCR1 = parse_int(rx_buf + 5);
                TIM2_CCR1 = CCR1;
            }

            else if (starts_with(rx_buf, "dim ") && rx_buf[4] >= '0' && rx_buf[4] <= '9') {
                pa5_to_gpio();
                const char *p = (const char *)(rx_buf + 4);
                delay = parse_int(p);
                while (*p >= '0' && *p <= '9') p++;
                while (*p == ' ') p++;
                duty = parse_int(p);

                if (duty <= 0) {
                    blink_flag = 0;
                    dim_flag = 0;
                    led_off();
                } else if (duty >= 100) {
                    blink_flag = 0;
                    dim_flag = 0;
                    led_on();
                } else {
                    on_time = delay * duty / 100;
                    off_time = delay - on_time;
                    blink_flag = 0;
                    dim_flag = 1;
                }

                uart_print("Delay\tDuty\ton_time\toff_time\r\n");
                uart_print_int(delay);
                uart_print("\t");
                uart_print_int(duty);
                uart_print("\t");
                uart_print_int(on_time);
                uart_print("\t");
                uart_print_int(off_time);
                uart_print("\r\n");
            }

            else if (scmp(rx_buf, "b")) blink_flag = 1;
            else if (scmp(rx_buf, "status")) {
                uart_print("   Blink delay: ");
                uart_print_int(delay);
                uart_print("ms\r\n");
            }
            else {
                uart_print("Error: invalid command\r\n");
            }
            uart_print("> ");
        }

        if (blink_flag) {
            current_tick = get_ticks();
            if (current_tick - last_tick >= delay) {            // x - y = x + (~y + 1)
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

        if (dim_flag) {
            //pa5_to_af();
            current_tick = get_ticks();
            if (current_tick - last_tick >= off_time && led_state == 0) {
                last_tick = current_tick;
                led_state = !led_state;
                led_on();
            }
            else if (current_tick - last_tick >= on_time && led_state == 1) {
                last_tick = current_tick;
                led_state = !led_state;
                led_off();
            }
        }
    }
}


// functions

int stoi(const char *s) {  // parameter if non-volatile because "delay" from get_delay is local
    // integer string to int
    int val = 0;
    while (*s != '\0') {
        val = 10 * val + (*s - '0');
        s++;
    }
    return val;
}

int parse_int(volatile const char *s) {
    // return "ms" as int from string with form "blink <ms>"
    int max_num_chars = 6;
    char delay[max_num_chars];
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


int starts_with(volatile const char *s, const char *prefix) {
    while (*prefix) {
        if (*s != *prefix) return 0;
        s++;
        prefix++;
    }
    return 1;
}

// main.c
// led control and temperature sensing with UART, EXTI, TIM2, SysTick, ADC

#include "registers.h"
#include "led.h"
#include "systick.h"
#include "uart.h"
#include "button.h"
#include "adc.h"
#include "tim2.h"

#define MIN_DELAY 20
#define MAX_DELAY 1000
#define NUM_COMMANDS (sizeof(commands) / sizeof(commands[0]))

int scmp(const char *s1, const char *s2);
int starts_with(const char *s, const char *prefix);
int parse_int(const char *s);
void scpy(char *s1, const char *s2);
int slen(const char *s);

typedef struct {
    int blink_flag;
    int dim_flag;
    int delay;
    int duty;
    int on_time;
    int off_time;
} led_state_t;
led_state_t led_state = {0, 0, 100, 20, 50, 50};

void cmd_temp(char *args) { //todo: take avareage of many readings to get more stable reading
    int t = read_temp_celsius();
    int whole = t / 10;
    int fraction = t % 10;
    uart_print("\r\n***The temperature is ");
    uart_print_int(whole);
    uart_print(".");
    uart_print_int(fraction);
    uart_print(" degrees Celsius.***\r\n");
}

void cmd_dim(char *args) {
    if (*args < '0' || *args > '9') {
        uart_print("\r\nError: Invalid first argument of dim command\r\n");
        return;
    }
    int temp_delay = parse_int(args);
    while (*args >= '0' && *args <= '9') args++;
    while (*args == ' ') args++;
    if (*args < '0' || *args > '9') {
        uart_print("\r\nError: Invalid second argument of dim command\r\n");
        return;
    }
    pa5_to_gpio();
    led_state.delay = temp_delay;
    led_state.duty = parse_int(args);
    if (led_state.duty <= 0) {
        led_state.blink_flag = 0;
        led_state.dim_flag = 0;
        led_off();
        uart_print("\r\nTurning LED off (duty <= 0)\r\n");
    } else if (led_state.duty >= 100) {
        led_state.blink_flag = 0;
        led_state.dim_flag = 0;
        led_on();
        uart_print("\r\nTurning LED on (duty >= 100)\r\n");
    } else {
        led_state.on_time = led_state.delay * led_state.duty / 100;
        led_state.off_time = led_state.delay - led_state.on_time;
        led_state.blink_flag = 0;
        led_state.dim_flag = 1;
        uart_print("\r\nDelay\tDuty\ton_time\toff_time\r\n");
        uart_print_int(led_state.delay);
        uart_print("\t");
        uart_print_int(led_state.duty);
        uart_print("\t");
        uart_print_int(led_state.on_time);
        uart_print("\t");
        uart_print_int(led_state.off_time);
        uart_print("\r\n");
    }
}

void cmd_led_on(char *args) {
    pa5_to_gpio();
    led_state.blink_flag = 0;
    led_state.dim_flag = 0;
    led_on();
    uart_print("\r\n***Led turned on***\r\n");
}

void cmd_led_off(char *args) {
    pa5_to_gpio();
    led_state.dim_flag = 0;
    led_state.blink_flag = 0;
    led_off();
    uart_print("\r\n***Led turned off***\r\n");
}

void cmd_b_ms(char *args) {
    if (*args < '0' || *args > '9') {
        uart_print("\r\nError: Bad led command\r\n");
        return;
    }
    pa5_to_gpio();
    led_state.dim_flag = 0;
    led_state.blink_flag = 1;
    led_state.delay = parse_int(args);
    if (led_state.delay < MIN_DELAY) led_state.delay = MIN_DELAY;
    uart_print("\r\n***Blinking with ");
    uart_print_int(led_state.delay);
    uart_print("ms delay***\r\n");
}

void cmd_dim2(char *args) {
    if (*args < '0' || *args > '9') {
        uart_print("\r\nError: Bad dim2 command\r\n");
        return;
    }
    led_state.dim_flag = 0;
    led_state.blink_flag = 0;
    pa5_to_af();
    int CCR1 = parse_int(args);
    TIM2_CCR1 = CCR1;
    uart_print("\r\n***CCR1 changed to ");
    uart_print_int(TIM2_CCR1);
    uart_print("***\r\n");
}

void cmd_blink(char *args) {
    pa5_to_gpio();
    led_state.blink_flag = !led_state.blink_flag;
    led_state.dim_flag = 0;
    led_off();
    if (led_state.blink_flag == 1) {
        uart_print("\r\n***Blinking started with delay ");
        uart_print_int(led_state.delay);
        uart_print("ms***\r\n");
    }
    else uart_print("\r\n***Blinking turned off***\r\n");
}

typedef void (*cmd_handler_t)(char *args);
typedef struct {
    const char *name;
    cmd_handler_t handler;
    int prefix_match;
} command_t;

const command_t commands[] = {
    {"led on", cmd_led_on, 0},
    {"led off", cmd_led_off, 0},
    {"b ", cmd_b_ms, 1},
    {"dim ", cmd_dim, 1},
    {"dim2 ", cmd_dim2, 1},
    {"b", cmd_blink, 0},
    {"t", cmd_temp, 0},
};

int process_command(char *input) {
    for (int i = 0; i < NUM_COMMANDS; i++) {
        if (commands[i].prefix_match) {
            if (starts_with(input, commands[i].name)) {
                char *args = input + slen(commands[i].name);
                commands[i].handler(args);
                return 1;
            }
        }
        else if (scmp(input, commands[i].name) == 0) {
                commands[i].handler("");
                return 1;
        }
    }
    return 0;
}

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

    unsigned int last_tick = 0; // tick_count at last toggle on/off
    unsigned int current_tick;
    int button_blink_increase = 1;
    int led_is_on = 0;

    while (1) {
        
        if (button_pressed) {
            button_pressed = 0;
            led_state.dim_flag = 0;
            led_state.blink_flag = 1;
            pa5_to_gpio();
            uart_print("***Button press: ");
            if (button_blink_increase == 1) {
                led_state.delay *= 2;
                if (led_state.delay >= MAX_DELAY) {
                    led_state.delay = MAX_DELAY;
                    button_blink_increase = !button_blink_increase;
                }
                uart_print("blink delay increased to ");
                uart_print_int(led_state.delay);
            }
            else {
                led_state.delay /= 2;
                if (led_state.delay <= MIN_DELAY) {
                    led_state.delay = MIN_DELAY;
                    button_blink_increase = !button_blink_increase;
                }
                uart_print("blink delay decreased to ");
                uart_print_int(led_state.delay);
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

        if (led_state.blink_flag) {
            current_tick = get_ticks();
            if (current_tick - last_tick >= led_state.delay) {            // x - y = x + (~y + 1)
                last_tick = current_tick;
                if (led_is_on == 0) {
                    led_on();
                }
                else {
                    led_off();
                }
                led_is_on = !led_is_on;
            }
        }

        if (led_state.dim_flag) {
            current_tick = get_ticks();
            if (current_tick - last_tick >= led_state.off_time && led_is_on == 0) {
                last_tick = current_tick;
                led_is_on = !led_is_on;
                led_on();
            }
            else if (current_tick - last_tick >= led_state.on_time && led_is_on == 1) {
                last_tick = current_tick;
                led_is_on = !led_is_on;
                led_off();
            }
        }
    }
}

// functions

int parse_int(const char *s) {
    if (*s < '0' || *s > '9') return -1;
    int result = 0;
    int i = 0;
    while (*s >= '0' && *s <= '9' && i < 6) {
        result = result * 10 + (*s - '0');
        s++;
        i++;
    }
    return result;
}

int starts_with(const char *s, const char *prefix) {
    while (*prefix) {
        if (*s != *prefix) return 0;
        s++;
        prefix++;
    }
    return 1;
}

void scpy(char* s1, const char* s2) {
    // copy s2 to s1
    while (*s2 != '\0') {
        *s1 = *s2;
        s1++;
        s2++;
    }
    *s1 = '\0';
}

int slen(const char *s) {
    // return len of s
    int i = 0;
    while (*s != '\0') {
        s++;
        i++;
    }
    return i;
}

int scmp(const char *s1, const char *s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return !(*s1 == *s2);
}


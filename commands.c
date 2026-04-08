#include "main.h"

typedef void (*cmd_handler_t)(char *args);

typedef struct {
    const char *name;
    cmd_handler_t handler;
    int prefix_match;
} command_t;

static const command_t commands[] = {
    {"s", cmd_s, 0},          // led on/off switch
    {"b ", cmd_b_ms, 1},      // blink <ms>
    {"dim ", cmd_dim, 1},     // software pwm
    {"dim2 ", cmd_dim2, 1},   // hardware pwm
    {"b", cmd_blink, 0},      // blink toggle on/off
    {"t", cmd_temp, 0},       // display temperature
};

#define NUM_COMMANDS (sizeof(commands) / sizeof(commands[0]))

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

void cmd_temp(char *args) {
    int t = read_temp_celsius();
    int whole = t / 10;
    int fraction = t % 10;
    uart_print("\r\n***The temperature is ");
    uart_print_int(whole);
    uart_print(".");
    uart_print_int(fraction);
    uart_print(" degrees Celsius***\r\n");
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
    g_led_state.delay = temp_delay;
    g_led_state.duty = parse_int(args);
    g_led_state.last_tick = get_ticks();
    if (g_led_state.duty <= 0) {
        g_led_state.blink_flag = 0;
        g_led_state.dim_flag = 0;
        led_off();
        uart_print("\r\nTurning LED off (duty <= 0)\r\n");
    } else if (g_led_state.duty >= 100) {
        g_led_state.blink_flag = 0;
        g_led_state.dim_flag = 0;
        led_on();
        uart_print("\r\nTurning LED on (duty >= 100)\r\n");
    } else {
        g_led_state.on_time = g_led_state.delay * g_led_state.duty / 100;
        g_led_state.off_time = g_led_state.delay - g_led_state.on_time;
        g_led_state.blink_flag = 0;
        g_led_state.dim_flag = 1;
        uart_print("\r\nDelay\tDuty\ton_time\toff_time\r\n");
        uart_print_int(g_led_state.delay);
        uart_print("\t");
        uart_print_int(g_led_state.duty);
        uart_print("\t");
        uart_print_int(g_led_state.on_time);
        uart_print("\t");
        uart_print_int(g_led_state.off_time);
        uart_print("\r\n");
    }
}

void cmd_s(char *args) {
    pa5_to_gpio();
    g_led_state.blink_flag = 0;
    g_led_state.dim_flag = 0;
    if (g_led_state.led_is_on == 1) {
        led_off();
        uart_print("\r\n***Led turned off***\r\n");
    }
    else {
        led_on();
        uart_print("\r\n***Led turned on***\r\n");
    }
    g_led_state.led_is_on = !g_led_state.led_is_on;
}


void cmd_dim2(char *args) {
    if (*args < '0' || *args > '9') {
        uart_print("\r\nError: Bad dim2 command\r\n");
        return;
    }
    g_led_state.dim_flag = 0;
    g_led_state.blink_flag = 0;
    pa5_to_af();
    int CCR1 = parse_int(args);
    TIM2_CCR1 = CCR1;
    uart_print("\r\n***CCR1 changed to ");
    uart_print_int(TIM2_CCR1);
    uart_print("***\r\n");
}


void cmd_b_ms(char *args) {
    if (*args < '0' || *args > '9') {
        uart_print("\r\nError: Bad led command\r\n");
        return;
    }
    pa5_to_gpio();
    g_led_state.dim_flag = 0;
    g_led_state.blink_flag = 1;
    g_led_state.delay = parse_int(args);
    if (g_led_state.delay < MIN_DELAY) g_led_state.delay = MIN_DELAY;
    uart_print("\r\n***Blinking with ");
    uart_print_int(g_led_state.delay);
    uart_print("ms delay***\r\n");
}


void cmd_blink(char *args) {
    pa5_to_gpio();
    g_led_state.blink_flag = !g_led_state.blink_flag;
    g_led_state.dim_flag = 0;
    if (g_led_state.blink_flag == 1) {
        g_led_state.last_tick = get_ticks();
        uart_print("\r\n***Blinking started with delay ");
        uart_print_int(g_led_state.delay);
        uart_print("ms***\r\n");
    }
    else {
         led_off();
         g_led_state.led_is_on = 0;
         uart_print("\r\n***Blinking turned off***\r\n");
    }
}
#include "main.h"

// at the top of commands.c, after the includes

typedef void (*cmd_handler_t)(char *args);

typedef struct {
    const char *name;
    cmd_handler_t handler;
    int prefix_match;
} command_t;

static const command_t commands[] = {
    {"s",    cmd_s,     0},
    {"b ",   cmd_b_ms,  1},
    {"dim ", cmd_dim,   1},
    {"dim2 ",cmd_dim2,  1},
    {"b",    cmd_blink, 0},
    {"t",    cmd_temp,  0},
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

// pa5 mode helpers — called under mutex by whoever needs them
static void ensure_pa5_gpio_locked(void) {
    if (g_led_state.pa5_mode != PA5_MODE_GPIO) {
        led_off();          // drive pin low before switching mode
        pa5_to_gpio();
        g_led_state.pa5_mode = PA5_MODE_GPIO;
    }
}

static void ensure_pa5_af_locked(void) {
    if (g_led_state.pa5_mode != PA5_MODE_AF) {
        pa5_to_af();
        g_led_state.pa5_mode = PA5_MODE_AF;
    }
}

void cmd_s(char *args) {
    xSemaphoreTake(g_state_mutex, portMAX_DELAY);
    ensure_pa5_gpio_locked();

    if (g_led_state.mode != LED_MODE_STATIC) {
        // was blinking/dimming 
        g_led_state.mode = LED_MODE_STATIC;
        g_led_state.static_on = 0;  // start with led off 
    } else {
        // already static -> toggle
        g_led_state.static_on = !g_led_state.static_on;
    }

    g_led_state.generation++;
    uint32_t is_on = g_led_state.static_on;
    xSemaphoreGive(g_state_mutex);

    if (is_on) uart_print("\r\n***Led turned on***\r\n");
    else        uart_print("\r\n***Led turned off***\r\n");
}

void cmd_blink(char *args) {
    xSemaphoreTake(g_state_mutex, portMAX_DELAY);
    ensure_pa5_gpio_locked();
    if (g_led_state.mode == LED_MODE_BLINK) {
        g_led_state.mode = LED_MODE_STATIC;
        g_led_state.static_on = 0;
        g_led_state.generation++;
        xSemaphoreGive(g_state_mutex);
        uart_print("\r\n***Blinking turned off***\r\n");
    } else {
        g_led_state.mode = LED_MODE_BLINK;
        g_led_state.generation++;
        uint32_t delay = g_led_state.delay;
        xSemaphoreGive(g_state_mutex);
        uart_print("\r\n***Blinking started with delay ");
        uart_print_int(delay);
        uart_print("ms***\r\n");
    }
}

void cmd_b_ms(char *args) {
    if (*args < '0' || *args > '9') {
        uart_print("\r\nError: Bad led command\r\n");
        return;
    }
    uint32_t d = parse_int(args);
    if (d < MIN_DELAY) d = MIN_DELAY;

    xSemaphoreTake(g_state_mutex, portMAX_DELAY);
    ensure_pa5_gpio_locked();
    g_led_state.mode = LED_MODE_BLINK;
    g_led_state.delay = d;
    g_led_state.generation++;
    xSemaphoreGive(g_state_mutex);

    uart_print("\r\n***Blinking with ");
    uart_print_int(d);
    uart_print("ms delay***\r\n");
}

void cmd_dim(char *args) {
    if (*args < '0' || *args > '9') {
        uart_print("\r\nError: Invalid first argument of dim command\r\n");
        return;
    }
    uint32_t on = parse_int(args);
    while (*args >= '0' && *args <= '9') args++;
    while (*args == ' ') args++;
    if (*args < '0' || *args > '9') {
        uart_print("\r\nError: Invalid second argument of dim command\r\n");
        return;
    }
    uint32_t off = parse_int(args);

    xSemaphoreTake(g_state_mutex, portMAX_DELAY);
    ensure_pa5_gpio_locked();
    g_led_state.mode = LED_MODE_DIM;
    g_led_state.on_time = on;
    g_led_state.off_time = off;
    g_led_state.generation++;
    xSemaphoreGive(g_state_mutex);

    uart_print("\r\non_time\toff_time\r\n");
    uart_print_int(on);
    uart_print("\t");
    uart_print_int(off);
    uart_print("\r\n");
}

void cmd_dim2(char *args) {
    if (*args < '0' || *args > '9') {
        uart_print("\r\nError: Bad dim2 command\r\n");
        return;
    }
    uint32_t ccr = parse_int(args);

    xSemaphoreTake(g_state_mutex, portMAX_DELAY);
    ensure_pa5_af_locked();
    g_led_state.mode = LED_MODE_STATIC;
    g_led_state.static_on = 1;  // TIM2 is driving it, consider it "on"
    g_led_state.generation++;
    xSemaphoreGive(g_state_mutex);

    TIM2_CCR1 = ccr;
    uart_print("\r\n***CCR1 changed to ");
    uart_print_int(ccr);
    uart_print("***\r\n");
}

void cmd_temp(char *args) {
    int t = read_temp_celsius();
    uart_print("\r\n***The temperature is ");
    uart_print_int(t / 10);
    uart_print(".");
    uart_print_int(t % 10);
    uart_print(" degrees Celsius***\r\n");
}
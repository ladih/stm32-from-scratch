#ifndef MAIN_H
#define MAIN_H
#include "registers.h"
#include "led.h"
#include "uart.h"
#include "button.h"
#include "adc.h"
#include "tim2.h"
#include "utils.h"
#include "commands.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdint.h>

#define MIN_DELAY 20
#define MAX_DELAY 1000

typedef enum { PA5_MODE_NONE, PA5_MODE_GPIO, PA5_MODE_AF } pa5_mode_t;
typedef enum { LED_MODE_STATIC, LED_MODE_BLINK, LED_MODE_DIM } led_mode_t;

typedef struct {
    led_mode_t  mode;
    uint32_t    static_on;      // for LED_MODE_STATIC: 1=on, 0=off
    uint32_t    delay;          // for LED_MODE_BLINK
    uint32_t    on_time;        // for LED_MODE_DIM
    uint32_t    off_time;       // for LED_MODE_DIM
    uint32_t    button_blink_increase;
    uint32_t    generation;
    pa5_mode_t  pa5_mode;
} led_state_t;

extern led_state_t g_led_state;
extern SemaphoreHandle_t g_state_mutex;

#endif
// main.h

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
#include <stdint.h>


#define MIN_DELAY 20 // minimum LED blink delay in ms - Global floor; "b" command and button will never go below this
#define MAX_DELAY 1000 // maximum LED blink delay in ms - Cap for button-triggered cycles only; "b" command can exceed this

typedef struct {
    uint32_t blink_flag;
    uint32_t dim_flag;
    uint32_t delay;
    uint32_t duty;
    uint32_t on_time;
    uint32_t off_time;
    uint32_t last_tick;
    uint32_t button_blink_increase;
    uint32_t led_is_on;
} led_state_t;

extern led_state_t g_led_state;  // declaration only

#endif
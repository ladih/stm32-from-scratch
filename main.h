// main.h

#ifndef MAIN_H
#define MAIN_H

#include "registers.h"
#include "led.h"
#include "systick.h"
#include "uart.h"
#include "button.h"
#include "adc.h"
#include "tim2.h"
#include "utils.h"
#include "commands.h"

#define MIN_DELAY 20 // minimum LED blink delay in ms - Global floor; "b" command and button will never go below this
#define MAX_DELAY 1000 // maximum LED blink delay in ms - Cap for button-triggered cycles only; "b" command can exceed this

typedef struct {
    int blink_flag;
    int dim_flag;
    int delay;
    int duty;
    int on_time;
    int off_time;
    unsigned int last_tick;
    int button_blink_increase;
    int led_is_on;
} led_state_t;

extern led_state_t g_led_state;  // declaration only

#endif
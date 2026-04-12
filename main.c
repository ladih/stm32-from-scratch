// main.c
// led control and temperature sensing using UART, EXTI, TIM2, SysTick, ADC

#include "main.h"

// led_states: blink_flag, dim_flag, delay, duty, on_time, off_time, last_tick, button_blink_increase, led_is_on
led_state_t g_led_state = {0, 0, 100, 20, 50, 50, 0, 1, 1};

void button_task(void *pvParameters) {
    while(1) {
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
    vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void led_task(void *pvParameters) {
    while(1) {
        if (g_led_state.blink_flag) {
            led_on();
            g_led_state.led_is_on = 1;
            vTaskDelay(pdMS_TO_TICKS(g_led_state.delay));
            led_off();
            g_led_state.led_is_on = 0;
            vTaskDelay(pdMS_TO_TICKS(g_led_state.delay));
        }
        else if (g_led_state.dim_flag) {
            led_on();
            g_led_state.led_is_on = 1;
            vTaskDelay(pdMS_TO_TICKS(g_led_state.on_time));
            led_off();
            g_led_state.led_is_on = 0;
            vTaskDelay(pdMS_TO_TICKS(g_led_state.off_time));
        }
        else {
            vTaskDelay(pdMS_TO_TICKS(10)); // nothing to do, sleep
        }
    }
}

void cmd_task(void *pvParameters) {
    while(1) {
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
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}


int main(void) {
    
    led_init();
    uart_init();
    button_init();  
    tim2_init();
    adc_init();

    // Welcome text
    led_on();
    uart_print("\n\n\n\rWelcome to led blink!\r\n");
    uart_print("Commands:\r\n");
    uart_print("   s\r\n");
    uart_print("   b <ms> \r\n");
    uart_print("   dim <delay> <duty>\r\n");
    uart_print("   dim2 <num>\r\n");
    uart_print("   b\r\n");
    uart_print("   t\r\n");
    uart_print("> ");

    xTaskCreate(led_task, "LED", 128, NULL, 1, NULL);
    xTaskCreate(button_task, "BUTTON", 128, NULL, 1, NULL);
    xTaskCreate(cmd_task, "CMD", 128, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1);

}
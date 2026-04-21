// main.c

#include "main.h"

led_state_t g_led_state = {
    .mode                   = LED_MODE_STATIC,
    .static_on              = 1,
    .delay                  = 100,
    .on_time                = 50,
    .off_time               = 50,
    .button_blink_increase  = 1,
    .generation             = 0,
    .pa5_mode               = PA5_MODE_NONE,
};

SemaphoreHandle_t g_state_mutex;
SemaphoreHandle_t test_mutex;

// led_task
void led_task(void *pvParameters) {
    uart_print("\r\nled_task started\r\n");

    while (1) {

        uint32_t gen; 
        gen = g_led_state.generation;

        if (g_led_state.mode == LED_MODE_STATIC) {
            if (g_led_state.static_on) led_on(); else led_off();
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        else if (g_led_state.mode == LED_MODE_BLINK) {
            led_on();
            vTaskDelay(pdMS_TO_TICKS(g_led_state.delay));

            if (gen != g_led_state.generation) continue;
            
            led_off();
            vTaskDelay(pdMS_TO_TICKS(g_led_state.delay));
        }
        else if (g_led_state.mode == LED_MODE_DIM) {
            led_on();
            vTaskDelay(pdMS_TO_TICKS(g_led_state.on_time));

            if (gen != g_led_state.generation) continue;

            led_off();
            vTaskDelay(pdMS_TO_TICKS(g_led_state.off_time));
        }
    }
}


// button_task
void button_task(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(40));
    uart_print("button_task started\r\n");

    while (1) {
        if (button_pressed) {
            button_pressed = 0;

            uint32_t delay = 0;
            if (1) { //xSemaphoreTake(g_state_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {

                g_led_state.mode = LED_MODE_BLINK;
                if (g_led_state.button_blink_increase) {
                    g_led_state.delay *= 2;
                    if (g_led_state.delay >= MAX_DELAY) {
                        g_led_state.delay = MAX_DELAY;
                        g_led_state.button_blink_increase = 0;
                    }
                } else {
                    g_led_state.delay /= 2;
                    if (g_led_state.delay <= MIN_DELAY) {
                        g_led_state.delay = MIN_DELAY;
                        g_led_state.button_blink_increase = 1;
                    }
                }
                delay = g_led_state.delay;
                g_led_state.generation++;
                //xSemaphoreGive(g_state_mutex);
            }
            else {
                uart_print("xSemaphoreTake failed for button press\r\n");
                continue;
            }
            uart_print("***Button press: blink delay set to ");
            uart_print_int(delay);
            uart_print("ms***\r\n> ");
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

// cmd_task
void cmd_task(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(80));
    uart_print("cmd_task started\r\n");

    while (1) {
        if (line_ready) {
            line_ready = 0;
            if (rx_buf[0] == '\0') {
                uart_print("\r\n> ");
                continue;
            }
            char buf[BUF_SIZE];
            scpy(buf, (char *)rx_buf);
            if (!process_command(buf))
                uart_print("\r\nError: invalid command\r\n");
            uart_print("> ");
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}


void intro_task(void *pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(120));

    uart_print("intro_task started");

    uart_print("\n\n\n\rWelcome to led blink!\r\n");
    uart_print("Commands:\r\n");
    uart_print("   s\r\n");
    uart_print("   b <ms>\r\n");
    uart_print("   dim <on time> <off time>\r\n");
    uart_print("   dim2 <num>\r\n");
    uart_print("   b\r\n");
    uart_print("   t\r\n");
    uart_print("> ");

    vTaskDelete(NULL);
}


// main
int main(void) {

    led_init();
    uart_init();
    button_init();
    tim2_init();
    adc_init();

    xTaskCreate(led_task,    "LED",    128, NULL, 1, NULL);
    xTaskCreate(button_task, "BUTTON", 128, NULL, 1, NULL);
    xTaskCreate(cmd_task,    "CMD",    128, NULL, 1, NULL);
    xTaskCreate(intro_task,  "INTRO",  128, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1);

}


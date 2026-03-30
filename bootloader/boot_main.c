#include "registers.h"
#include <stdint.h>
#include "led.h"
#include "uart.h"

#define APP_START_ADDRESS 0x08008000UL
typedef void (*app_entry_t)(void);

static inline void disable_irq(void) { __asm volatile ("cpsid i"); }
static inline void set_msp(uint32_t topOfStack) { __asm volatile ("msr msp, %0" : : "r"(topOfStack)); }
#define SCB_VTOR (*(volatile uint32_t*)0xE000ED08)

void jump_to_app(void) {
    uint32_t app_stack = *((volatile uint32_t*)APP_START_ADDRESS);
    uint32_t app_reset = *((volatile uint32_t*)(APP_START_ADDRESS + 4));

    disable_irq();
    SCB_VTOR = APP_START_ADDRESS;
    set_msp(app_stack);

    app_entry_t app = (app_entry_t)app_reset;
    app();
}

void delay(volatile uint32_t count) {
    while (count--);
}

int main(void) {
    led_init();
    uart_init();
    uart_print("\n\n\r...Bootloader started, press any key to continue.\r\n");

    while (1) {
        if (line_ready) {
            RCC_APB1RSTR |= 1 << 17;
            for (volatile int i = 0; i < 10000; i++);
            RCC_APB1RSTR &= ~(1 << 17);
            break;
        }
    }

    jump_to_app();
}


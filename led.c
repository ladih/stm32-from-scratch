// led.c

#include "registers.h"

void led_init(void) {
    // PA 5 
    RCC_AHB1ENR |= (1 << 0); // RCC AHB1 peripheral clock enable register, Bit 0 GPIOAEN: IO port A clock enable
    GPIOA_MODER |= (1 << 10); // bit 10 = 1 
    GPIOA_MODER &= ~(1 << 11); // bit 11 = 0         (01 = General purpose output mode)
    GPIOA_OTYPER &= ~(1 << 5); // GPIO port output type register.  PA5, Output open-drain (0)
}

void led_on(void) {
    GPIOA_BSRR |= (1 << 5);
}

void led_off(void) {
    GPIOA_BSRR |= (1 << (16 + 5));
}
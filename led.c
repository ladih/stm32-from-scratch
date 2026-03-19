// led.c

#include "registers.h"

void led_init(void) {
    RCC_AHB1ENR |= (1 << 0);
    GPIOA_MODER |= (1 << 10);
    GPIOA_MODER &= ~(1 << 11);
    GPIOA_OTYPER &= ~(1 << 5);
}
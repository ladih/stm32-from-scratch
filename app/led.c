// led.c

#include "registers.h"

void led_init(void) {
    // PA 5 
    RCC_AHB1ENR |= (1 << 0); // RCC AHB1 peripheral clock enable register, Bit 0 GPIOAEN: IO port A clock enable

    GPIOA_MODER &= ~(0b11 << 10);
    GPIOA_MODER |= (0b01 << 10); // general purpose output 
    GPIOA_OTYPER &= ~(1 << 5); // push-pull
}

void led_on(void) {
    GPIOA_BSRR |= (1 << 5);
}

void led_off(void) {
    GPIOA_BSRR |= (1 << (16 + 5));
}

void pa5_to_af(void) {
    GPIOA_MODER &= ~(0b11 << 10);
    GPIOA_MODER |= (0b10 << 10); // alternate function

    GPIOA_AFRL  &= ~(0xF << 20);   // clear AF bits for PA5
    GPIOA_AFRL  |=  (0x1 << 20);   // AF1 = TIM2
}

void pa5_to_gpio(void) {
    GPIOA_MODER &= ~(0b11 << 10);
    GPIOA_MODER |= (0b01 << 10); // output
}
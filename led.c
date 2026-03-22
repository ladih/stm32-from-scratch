// led.c

#include "registers.h"

void led_init(void) {
    RCC_AHB1ENR |= (1 << 0); // RCC AHB1 peripheral clock enable register, Bit 0 GPIOAEN: IO port A clock enable
    GPIOA_MODER |= (1 << 10); // bit 10 = 1 
    GPIOA_MODER &= ~(1 << 11); // bit 11 = 0         (General purpose output mode for PA5)
    GPIOA_OTYPER &= ~(1 << 5); // GPIO port output type register.  PA5, Output open-drain (0)
}
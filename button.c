// button.c

#include "registers.h"

volatile int button_pressed = 0; // for use in main.c

void button_init() {

    // "B1 USER: the user button is connected to the I/O PC13 (pin 2) of the STM32 microcontroller."

    RCC_AHB1ENR |= (1 << 2); // enable clock for GPIOC
    GPIOC_MODER &= ~(0b11 << 26); // 00 = input  (PC 13)
    RCC_APB2ENR |= (1 << 14); // 1: System configuration controller clock enabled
    SYSCFG_EXTICR4 |= (0b0010 << 4); // map EXTI13 to port C  (0010 = C)
    EXTI_IMR |= (1 << 13);   // unmask interrupt on line 13

    EXTI_FTSR |= (1 << 13);  // enable falling edge (button press if pull-up)
    // EXTI->RTSR |=  (1 << 13); // enable rising edge (trigger on release)

    NVIC_ISER1 |= (1 << 8);     // Enable NVIC for EXTI_15 to EXTI_10.
}


void EXTI15_10_IRQHandler(void) {
    if (EXTI_PR & (1 << 13)) {   // check pending bit
        button_pressed = 1;
        EXTI_PR |= (1 << 13);    // clear by writing 1    
    }
}
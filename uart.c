// uart on PA2 

#include "registers.h"

void uart_init(void) {
    // PA2 pin config
    RCC_AHB1ENR |= (1 << 0);
    GPIOA_MODER &= ~(3 << 4);
    GPIOA_MODER |= (2 << 4);
    GPIOA_AFRL |= (7 << 8);

    // USART config
    RCC_APB1ENR |= (1 << 17); // enable USART2 clock
    USART2_CR1 |= (1 << 13); // enable USART2
    // USART2_CR1 bit 12 = 0  -> 8 data bits used
    USART2_BRR = 1667; // set baud rate 16 MHz / 9600 = 1667
    USART2_CR1 |= (1 << 3); // enable transmitter
    // USART2_CR1 |= 1 << 2; // enable reciever

}

void uart_print(const char *s) {
    while (*s) {
        while (!(USART2_SR & (1 << 7)));
        USART2_DR = *s++;
    }
    while (!(USART2_SR & (1 << 6)));
}
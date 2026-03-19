
// USART

#include "registers.h"


int main(void) {

    // GPIO
    RCC_AHB1ENR |= (1 << 0); // enable clock for GPIOA
    GPIOA_MODER |= (2 << 4); // alternate function mode for PA2 (10)
    GPIOA_AFRL |= (7 << 8); // alternate function = 7 = 0111 for PA2

    // USART
    RCC_APB1ENR |= (1 << 17); // enable USART2 clock

    USART2_CR1 |= (1 << 13); // enable USART2
    // USART2_CR1 bit 12 = 0  -> 8 data bits used
    USART2_BRR = 139; // set baud rate
    USART2_CR1 |= (1 << 3); // enable transmitter
    // USART2_CR1 |= 1 << 2; // enable reciever

    while (1);

}
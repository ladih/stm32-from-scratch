// uart on PA2 

#include "registers.h"

volatile char rx_buf[50];
volatile int rx_idx = 0;
volatile int line_ready = 0;

void USART2_IRQHandler(void) {

    if (USART2_SR & (1 << 5)) {       // if RXNE (Read data register not empty)
        char c = USART2_DR;
        if (c == '\r' || c == '\n') {
            rx_buf[rx_idx] = '\0';
            line_ready = 1;
            rx_idx = 0;
        }
        else {
            rx_buf[rx_idx++] = c;
        }
        
        while (!(USART2_SR & (1 << 7))); // wait for TXE (Transmit data register empty)
        USART2_DR = c;                   // write to DR
    }
}

void uart_init(void) {
    // PA2 pin config
    RCC_AHB1ENR |= (1 << 0);
    GPIOA_MODER &= ~(3 << 4);
    GPIOA_MODER |= (2 << 4);
    GPIOA_AFRL |= (7 << 8);

    // PA3 pin config
    GPIOA_MODER &= ~(3 << 6);
    GPIOA_MODER |= (2 << 6);
    GPIOA_AFRL |= (7 << 12);

    // USART config
    RCC_APB1ENR |= (1 << 17); // enable USART2 clock
    USART2_CR1 |= (1 << 13); // enable USART2
    // USART2_CR1 bit 12 = 0  -> 8 data bits used
    USART2_BRR = 1667; // set baud rate 16 MHz / 9600 = 1667
    USART2_CR1 |= (1 << 3); // enable transmitter
    USART2_CR1 |= 1 << 2; // enable reciever
    USART2_CR1 |= 1 << 5; // RXNEIE: RXNE interrupt enable
    NVIC_ISER1 |= 1 << 6; // IRQ 38 (32 + 6)... Interrupt Set-Enable Registers USART2
}

void uart_print(const char *s) {
    while (*s) {
        while (!(USART2_SR & (1 << 7)));
        USART2_DR = *s++;
    }
    while (!(USART2_SR & (1 << 6)));
}

void uart_print_int(int n) {
    char buf[12]; // enough for -2147483648 + null terminator
    char *p = buf + sizeof(buf) - 1;
    int negative = n < 0;

    *p = '\0';

    if (negative) n = -n;

    // Build digits in reverse
    do {
        *--p = '0' + (n % 10);
        n /= 10;
    } while (n);

    if (negative) *--p = '-';

    uart_print(p);
}
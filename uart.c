// uart on PA2 

#include "registers.h"
#include "uart.h"

volatile char rx_buf[BUF_SIZE];
volatile int rx_idx = 0;
volatile int line_ready = 0;

void uart_init(void) {
    // PA2 pin config (using PA2 for TX)
    RCC_AHB1ENR |= (0b1 << 0);        // IO port A clock enable
    GPIOA->MODER &= ~(0b11 << 4);     // Reset PA2 moder config
    GPIOA->MODER |= (0b10 << 4);      // Alternate function mode (10) for pin 2
    GPIOA->AFR[0] |= (0b0111 << 8);   // AF7 (USART2 TX) for PA2

    // PA3 pin config
    GPIOA->MODER &= ~(0b11 << 6);      // Reset moder
    GPIOA->MODER |= (0b10 << 6);       // Alternate function mode
    GPIOA->AFR[0] |= (0b0111 << 12);   // AF7 (USART_RX)

    // USART config
    RCC_APB1ENR |= (0b1 << 17); // enable USART2 clock
    USART2->CR1 |= (0b1 << 13); // enable USART2
    // USART2->CR1 bit 12 = 0  -> 8 data bits used
    // baud rate register (USART_BRR) - 12-bit mantissa and 4-bit fraction.
    USART2->BRR = 0x683; //  baud = f_c / (16 * USARTDIV)   =>  USARTDIV = f_c / (16 * baud).  USARTDIV_9600 = 16 000 000 / (16 * 9600) = 104.166666667... first four bits are fractional part => plug in 16 * USARTDIV = 1667 = 0x683
    USART2->CR1 |= (0b1 << 3); // enable transmitter
    USART2->CR1 |= (0b1 << 2); // enable reciever
    USART2->CR1 |= (0b1 << 5); // RXNEIE: RXNE interrupt enable.    1: An USART interrupt is generated whenever ORE=1 or RXNE=1 in the USART_SR register
    NVIC_ISER1 |= (0b1 << 6); // Enable IRQ 38 (32 + 6) (USART2_IRQHandler)   via   Interrupt Set-Enable Register (ISER)
}

void send_char(char c) {
    while (!(USART2->SR & (0b1 << 7))); // wait for TXE
    USART2->DR = c;
}

void USART2_IRQHandler(void) {
    if (USART2->SR & (0b1 << 5)) {         // if Read data register not empty (RXNE)
        char c = USART2->DR;               // read character

        if (c == '\r' || c == '\n') {
            rx_buf[rx_idx] = '\0';
            line_ready = 1;
            rx_idx = 0;
        }
        else {
            if (c == '\x7F') { // backspace
                if (rx_idx > 0) {
                    rx_idx--;
                    uart_print("\b \b");
                }
            }
            else if (rx_idx < BUF_SIZE - 1) {  // leave room for null terminator
                rx_buf[rx_idx++] = c;
                send_char(c); // echo saved character
            }
        }
    }
}

void uart_print(const char *s) {
    while (*s) {
        while (!(USART2->SR & (0b1 << 7))); // Wait for content of TDR register to be transferred into the shift register
        USART2->DR = *s++;  // write, then increment
    }
    while (!(USART2->SR & (0b1 << 6)));  // wait for Transmission complete (TC)
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
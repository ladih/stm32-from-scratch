#ifndef REGISTERS_H
#define REGISTERS_H

// GPIOA registers
#define GPIOA_BASE  0x40020000UL
#define GPIOA_MODER (*((volatile unsigned int *)(GPIOA_BASE + 0x00))) // GPIOA port mode register
#define GPIOA_OTYPER (*((volatile unsigned int *)(GPIOA_BASE + 0x04))) // GPIOA port output type register
#define GPIOA_IDR     (*(volatile unsigned int *)(GPIOA_BASE + 0x10)) // GPIOA port input data register
#define GPIOA_ODR   (*((volatile unsigned int *)(GPIOA_BASE + 0x14))) // GPIOA port output data register
#define GPIOA_BSRR (*(volatile unsigned int *)(GPIOA_BASE + 0x18)) // GPIOA port bit set/reset register
#define GPIOA_AFRL   (*(volatile unsigned int *)(GPIOA_BASE + 0x20)) // alternate function low

// Reset and clock control registers
#define RCC_BASE 0x40023800UL
#define RCC_CFGR (*(volatile unsigned int *)(RCC_BASE + 0x08)) // RCC clock configuration register (RCC_CFGR): Offset: 0x08
#define RCC_AHB1ENR (*(volatile unsigned int *)(RCC_BASE + 0x30)) // RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)
#define RCC_APB1ENR (*(volatile unsigned int *)(RCC_BASE + 0x40)) // RCC APB1 peripheral clock enable register (RCC_APB1ENR)

// universal synchronous asynchronous receiver transmitter
#define USART2_BASE 0x40004400UL
#define USART2_SR (*(volatile unsigned int *)(USART2_BASE + 0x00))    // Status register
#define USART2_DR (*(volatile unsigned int *)(USART2_BASE + 0x04))    // Data register
#define USART2_BRR (*(volatile unsigned int *)(USART2_BASE + 0x08))   // Baud rate register (USART_BRR)
#define USART2_CR1 (*(volatile unsigned int *)(USART2_BASE + 0x0C))   // Control register 1 (USART_CR1)

// SysTick registers
#define SYST_RVR  (*(volatile unsigned int*)0xE000E014UL)
#define SYST_CVR  (*(volatile unsigned int*)0xE000E018UL)
#define SYST_CSR  (*(volatile unsigned int*)0xE000E010UL)

#define NVIC_ISER1 (* (volatile unsigned int *)0xE000E104)   //  Interrupt Set-Enable Register

#endif



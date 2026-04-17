#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

// GPIO registers
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;   // 0x4
    volatile uint32_t OSPEEDR;  // 0x8
    volatile uint32_t PUPDR;    // 0xC
    volatile uint32_t IDR;      // 0x10
    volatile uint32_t ODR;      // 0x14
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIO_TypeDef;

#define GPIOA ((GPIO_TypeDef*) 0x40020000)
#define GPIOC ((GPIO_TypeDef*) 0x40020800)

// Reset and clock control registers
#define RCC_BASE 0x40023800UL
#define RCC_CFGR (*(volatile uint32_t *)(RCC_BASE + 0x08))     // RCC clock configuration register (RCC_CFGR): Offset: 0x08
#define RCC_AHB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x30))  // RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)
#define RCC_APB1ENR (*(volatile uint32_t *)(RCC_BASE + 0x40))  // RCC APB1 peripheral clock enable register (RCC_APB1ENR)
#define RCC_APB2ENR (*(volatile uint32_t *)(RCC_BASE + 0x44))  //RCC APB2 peripheral clock enable register (RCC_APB2ENR)

// ADC1
#define ADC1_BASE 0x40012000UL
#define ADC1_SR      (*(volatile uint32_t*)(ADC1_BASE + 0x00))
#define ADC1_CR1     (*(volatile uint32_t*)(ADC1_BASE + 0x04))
#define ADC1_CR2     (*(volatile uint32_t*)(ADC1_BASE + 0x08))
#define ADC1_SMPR1   (*(volatile uint32_t*)(ADC1_BASE + 0x0C))
#define ADC1_SQR3    (*(volatile uint32_t*)(ADC1_BASE + 0x34))
#define ADC1_DR      (*(volatile uint32_t*)(ADC1_BASE + 0x4C))
#define ADC_CCR      (*(volatile uint32_t*)(ADC1_BASE + 0x300 + 0x04))

// SYSCFG
#define SYSCFG_BASE 0x40013800UL
#define SYSCFG_EXTICR4 (*(volatile uint32_t *)(SYSCFG_BASE + 0x14))       // SYSCFG external interrupt configuration register 4

// EXTI registers
#define EXTI_BASE 0x40013C00UL
#define EXTI_IMR (*(volatile uint32_t *)(EXTI_BASE + 0x00))       // Interrupt mask register (EXTI_IMR)
#define EXTI_FTSR (*(volatile uint32_t *)(EXTI_BASE + 0x0C))      // Falling trigger selection register (EXTI_FTSR)
#define EXTI_PR (*(volatile uint32_t *)(EXTI_BASE + 0x14))        // Pending register (EXTI_PR)

// Universal Synchronous Asynchronous Receiver Transmitter (USART) registers
typedef struct {
    volatile uint32_t SR;   // Status register   offset 0x00
    volatile uint32_t DR;   // Data register     offset 0x04
    volatile uint32_t BRR;  // Baud rate register       0x08
    volatile uint32_t CR1;  // Control reguster 1       0x0C
} USART_TypeDef;

#define USART2 ((USART_TypeDef *) 0x40004400)

// SysTick registers
#define SYST_CSR  (*(volatile uint32_t *)(0xE000E010UL)) // SysTick Control and Status Register
#define SYST_RVR  (*(volatile uint32_t *)(0xE000E014UL)) // SysTick Reload Value Register
#define SYST_CVR  (*(volatile uint32_t *)(0xE000E018UL)) // SysTick Current Value Register

// Interrupt Set-Enable Registers, NVIC_ISER0-NVIC_ISER15
#define NVIC_ISER1 (*(volatile uint32_t *)(0xE000E104UL))   //     63 - 32
#define NVIC_ISER2 (*(volatile uint32_t *)(0xE000E108UL))   //     95 - 64

// 32-bit General-purpose timers; TIM2 
#define TIM2_BASE 0x40000000UL
#define TIM2_CR1 (*(volatile uint32_t *)(TIM2_BASE + 0x00))    // TIMx control register 1 (TIMx_CR1)
#define TIM2_PSC (*(volatile uint32_t *)(TIM2_BASE + 0x28))    // TIMx prescaler (TIMx_PSC)
#define TIM2_ARR (*(volatile uint32_t *)(TIM2_BASE + 0x2C))    // TIMx auto-reload register (TIMx_ARR)
#define TIM2_CCR1 (*(volatile uint32_t *)(TIM2_BASE + 0x34))   // TIMx capture/compare register 1 (TIMx_CCR1)
#define TIM2_CCMR1 (*(volatile uint32_t *)(TIM2_BASE + 0x18))  // TIMx capture/compare mode register 1 (TIMx_CCMR1)
#define TIM2_CCER (*(volatile uint32_t *)(TIM2_BASE + 0x20))   // TIMx capture/compare enable register (TIMx_CCER)

#endif



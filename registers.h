

#ifndef REGISTERS_H
#define REGISTERS_H

// GPIOA registers
#define GPIOA_BASE  0x40020000UL
#define GPIOA_MODER (*((volatile unsigned int *)(GPIOA_BASE + 0x00))) // GPIOA port mode register
#define GPIOA_OTYPER (*((volatile unsigned int *)(GPIOA_BASE + 0x04)))
#define GPIOA_IDR     (*(volatile unsigned int *)(GPIOA_BASE + 0x10)) // GPIOA port input data register
#define GPIOA_ODR   (*((volatile unsigned int *)(GPIOA_BASE + 0x14))) // GPIOA port output data register
#define GPIOA_BSRR (*(volatile unsigned int *)(GPIOA_BASE + 0x18)) // GPIOA port bit set/reset register

// Reset and clock control registers
#define RCC_BASE 0x40023800UL
#define RCC_CFGR (*(volatile unsigned int *)(RCC_BASE + 0x08)) // RCC clock configuration register (RCC_CFGR): Offset: 0x08
#define RCC_AHB1ENR (*(volatile unsigned int *)(RCC_BASE + 0x30)) // RCC_AHB1ENR: RCC + 0x30 = 0x40023830

// SysTick registers
#define SYST_CSR 0xE000E010UL // SysTick Control and Status Register
#define SYST_RVR 0xE000E014UL // SysTick Reload Value Register
#define SYST_CVR 0xE000E018UL // SysTick Current Value Register
#define SYST_CALIB 0xE000E01CUL // SysTick Calibration value Register

#endif
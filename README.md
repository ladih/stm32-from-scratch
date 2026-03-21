# STM32F446RE Bare-Metal 
STM32F446RE project using direct register access (no HAL, CMSIS, or RTOS).

## Features
- LED blink (GPIO)
- UART input/output
- Button input
- SysTick timer

## Hardware
- Board: Nucleo-F446RE
- LED: PA5 (on-board LED)
- Button: PC13 (user button)
- UART: USART2 (PA2 TX, PA3 RX)

## Build
```sh
make
```

## Flash
```
make flash
```

## Requirements
- arm-none-eabi-gcc
- make
- st-flash (from stlink)
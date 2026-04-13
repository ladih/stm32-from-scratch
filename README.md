LED control and temperature sensing on the STM32F446RE.
No HAL, no CMSIS — registers only.

Features:
- FreeRTOS tasks for LED, button, and UART command handling
- Mutex-protected shared state with generation-based stale cycle detection
- UART CLI with commands for static, blink, software PWM, and hardware PWM dim modes
- Hardware PWM via TIM2 with direct CCR register control
- ADC temperature reading
- Bare-metal version available in the bare-metal branch
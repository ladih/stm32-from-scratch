// tim2.c

#include "registers.h"

void tim2_init(void) {
    RCC_APB1ENR |= (1 << 0); // TIM2 clock enable
    TIM2_PSC = 15; // pre scaler 16_000_000 / 16 = 1_000_000
    TIM2_ARR = 999; // auto reload register 1_000_000 / 1000 = 1000
    TIM2_CCR1 = 500; // capture/compare register 1

    TIM2_CCMR1 |= 0b110 << 4; // Capture/compare mode register; OC1M = PWM mode 1
    TIM2_CCMR1 |= 1 << 3;  // OC1PE: Output compare 1 preload enable, Preload register on TIMx_CCR1 enabled
    TIM2_CCER |= 1 << 0;
    TIM2_CR1 |= 1 << 7;
    TIM2_CR1 |= 1 << 0;
}
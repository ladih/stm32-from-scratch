// adc.c

#include "registers.h"

void adc_init(void)
{
    RCC_AHB1ENR |= (1 << 2); // GPIOC clock
    RCC_APB2ENR |= (1 << 8); // ADC1 clock
    GPIOC_MODER |=  (0b11 << 0); // PC0 Analog mode
    GPIOC_PUPDR &= ~(0b11 << 0); // PC0 no pull-up/down
    ADC_CCR &= ~(0b11 << 16); // clock freq. for ADC = PCLK2 / 2 
    ADC1_CR1 &= ~(0b11 << 24); // 00 = 12 bit resolution
    ADC1_CR1 &= ~(1 << 8); // 0: Scan mode disabled
    ADC1_CR2 &= ~(1 << 1); // 0: Single conversion mode
    ADC1_CR2 &= ~(0b11 << 28); // 00: Trigger detection disabled
    ADC1_CR2 &= ~(0b1 << 30); // no conversion start
    ADC1_CR2 &= ~(1 << 10); // 0: The EOC bit is set at the end of each sequence of regular conversions.
    ADC1_SQR3 = 10; // 1st conversion in sequence = channel 10 (PC0)
    ADC1_SMPR1 |= (0b111 << 0); // sampling time: 480 cycles
    ADC1_CR2 |= (1 << 0); // 1: Enable ADC
}

int read_temp_celsius(void)
{
    ADC1_CR2 |= (1 << 30); // start conversion
    while (!(ADC1_SR & (1 << 1))); // wait for conversion complete
    int raw = ADC1_DR; // read ADC result
    int mv = raw * 3300 / 4095; // normalize to get mV
    int temp_times_10 = mv - 500; // 500 mV = 0 degrees, 10mV = 1 degree

    return temp_times_10;
}



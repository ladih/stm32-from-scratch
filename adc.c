// adc.c

#include "registers.h"
#include <stdint.h>
#include "uart.h"

void adc_init(void)
{
    RCC_AHB1ENR |= (1 << 2); // GPIOC clock
    RCC_APB2ENR |= (1 << 8); // ADC1 clock
    GPIOC->MODER |=  (0b11 << 0); // PC0 Analog mode
    GPIOC->PUPDR &= ~(0b11 << 0); // PC0 no pull-up/down
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

    const uint16_t VCC_MV = 3300;        // supply voltage in mV
    const uint16_t ADC_VREF_MV = 3300;   // ADC reference voltage in mV
    const uint16_t ADC_MAX = 4095;
    const uint16_t SENSOR_OFFSET = 500; // 500 mV = 0 degrees

    const uint16_t n_readings = 50;
    uint32_t sum = 0;

    for (uint16_t i = 0; i < n_readings; i++) {        
        ADC1_CR2 |= (1 << 30); // start conversion
        while (!(ADC1_SR & (0b1 << 1))); // wait for conversion complete (Bit 1: EOC)
        sum += ADC1_DR; // sum up ADC result
    }

    uint16_t mean = sum / n_readings;
    uint16_t mv = (uint32_t)mean * ADC_VREF_MV / ADC_MAX; // normalize to get mV
    int16_t temp_times_10 = (int16_t)mv - SENSOR_OFFSET; // TMP36: 500mV = 0°C, 10mV per °C. Result is temp × 10.

    return temp_times_10;
}
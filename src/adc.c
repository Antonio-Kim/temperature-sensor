#include "stm32l4xx.h"
#include "adc.h"

#define ADCEN			(1U<<13)
#define ADC1_CH1		(1U<<6)
#define GPIOCEN			(1U<<2)
#define ISR_EOC			(1U<<2)
#define CFGR_CONT		(1U<<3)

void pc0_adc_init(void) {
	// Configure the ADC GPIO Pin
	// 1. Enable clock access to GPIOC
	RCC->AHB2ENR |= GPIOCEN;
	// 2. Set the mode of PC0 to analog
	GPIOC->MODER |= (1U<<0);
	GPIOC->MODER |= (1U<<1);

	// Configure the ADC Module
	// 1. Enable Clock access to ADC
	RCC->AHB2ENR |= ADCEN;
	// 2. Configure adc parameters
	// 		- Conversion Sequence start
	// 		- Conversion Sequence length
	// 		- Enable ADC Module
	ADC1->SQR1 = ADC1_CH1;

	ADC1->SQR1 &= ~(1U<<0);
	ADC1->SQR1 &= ~(1U<<1);
	ADC1->SQR1 &= ~(1U<<2);
	ADC1->SQR1 &= ~(1U<<3);

	ADC1->CR |= (1U<<0);
}

void start_adc_conversion(void) {
	// Enable continuous conversion
	ADC1->CFGR |= CFGR_CONT;
	ADC1->ISR |= (1U<<0);
}

uint32_t temperature_reading(void) {
	// Using TMP36 as a temperature Sensor. Using different
	// sensors will need to be reconfigured in the formula,
	// potentially different signals as well
	// You can choose 5V reading, but here the conversion is
	// used for 3.3V
	float voltage_offset = 0.5;
	float voltage_reading = 3.3;

	// Wait for conversion to complete
	while (!(ADC1->ISR & ISR_EOC)){}

	float voltage = (ADC1->DR * voltage_reading)/1024;
	float temperature_in_celsius = (voltage - voltage_offset) * 100;
	return temperature_in_celsius;
}

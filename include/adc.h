#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void pc0_adc_init(void);
void start_adc_conversion(void);
uint32_t temperature_reading(void);

#endif

#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "adc.h"

uint32_t sensor_value;

int main(void) {

	uart2_tx_init();
	pc0_adc_init();
	start_adc_conversion();
	while (1) {
		sensor_value = temperature_reading();
		printf("Temperature(C): %d\n\r", (int)sensor_value);
	}
}

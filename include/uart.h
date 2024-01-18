#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "stm32l4xx.h"

void uart2_tx_init(void);
char uart2_read(void);
void uart2_rxtx_init(void);

#endif

#include "uart.h"

#define GPIOAEN				(1U<<0)
#define USART2EN			(1U<<17)
#define CR1_TE				(1U<<3)
#define CR1_UE				(1U<<0)
#define CR1_RE				(1U<<2)
#define ISR_TXE				(1U<<7)
#define ISR_RXNE			(1U<<5)
#define SYS_FREQ			16000000
#define APB1_CLK			SYS_FREQ
#define UART_BAUDRATE		115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);


void uart2_write(int ch);
int __io_putchar(int ch) {
	uart2_write(ch);
	return ch;
}

void uart2_rxtx_init(void) {
	// Configure UART GPIO pins
	// 1. Enable Clock Access to GPIOA
	RCC->AHB2ENR|= GPIOAEN;
	// 2. Set PA2 mode to alternate function mode
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= (1U<<5);
	// 3. Set PA2 Alternate function type to UART_TX (AF7)
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	// 4. Set PA3 Mode to alternate function mode
	GPIOA->MODER &= ~(1U<<6);
	GPIOA->MODER |= (1U<<7);
	// 5. Set PA3 alternate function type to UART_RX
	GPIOA->AFR[0] |= (1U<<12);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &= ~(1U<<15);


	// Configure UART Module
	// 1. Enable clock access to UART2
	RCC->APB1ENR1 |= USART2EN;
	// 2. Configure Baud Rate
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);
	// 3. Configure the transfer direction ****NB This might cause some issues. Upper registers are not reserved
	USART2->CR1 = CR1_TE | RE;
	// 4. Enable UART Module
	USART2->CR1 |= CR1_UE;

}

void uart2_tx_init(void) {
	// Configure UART GPIO pins
	// 1. Enable Clock Access to GPIOA
	RCC->AHB2ENR|= GPIOAEN;
	// 2. Set PA2 mode to alternate function mode
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= (1U<<5);
	// 3. Set PA2 Alternate function type to UART_TX (AF7)
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	// Configure UART Module
	// 1. Enable clock access to UART2
	RCC->APB1ENR1 |= USART2EN;
	// 2. Configure Baud Rate
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);
	// 3. Configure the transfer direction ****NB This might cause some issues. Upper registers are not reserved
	USART2->CR1 = CR1_TE;
	// 4. Enable UART Module
	USART2->CR1 |= CR1_UE;

}

char uart2_read(void) {
	// 1. Make sure receive data register is empty
	while(!(USART2->ISR & ISR_RXNE)){}
	// 2. Read data
	return USART2->DR;
}

void uart2_write(int ch) {
	// 1. Make sure transmit data register is empty
	while(!(USART2->ISR & ISR_TXE)){}
	// 2. Write to transmit data register
	USART2->TDR = ch & 0xFF;
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate) {
	USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate) {
	return (PeriphClk + (BaudRate / 2U)) / BaudRate;
}

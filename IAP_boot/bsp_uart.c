#include "bsp_uart.h"

void init_uart(){
	REG_SET(RCC_AHB1ENR, RCC_AHB1ENR_GPIOAEN);			// 开GPIOA的时钟
	REG_SET(RCC_APB2ENR, RCC_APB2ENR_USART1EN);			// 开USART1的时钟
	
	// 配置PA9和PA10的复用，复用成UART1
	REG_SET_VALUE(GPIOA_AFRH, (0x0FU << (UART1_RX_GPIO_PIN - 8) * 4), (GPIOA_AF_UART1 << (UART1_RX_GPIO_PIN - 8) * 4));
	REG_SET_VALUE(GPIOA_AFRH, (0x0FU << (UART1_TX_GPIO_PIN - 8) * 4), (GPIOA_AF_UART1 << (UART1_TX_GPIO_PIN - 8) * 4));
	
	// 配置PA9
	REG_SET_VALUE(GPIOA_MODER, (0x03 << UART1_TX_GPIO_PIN * 2), (0x02 << UART1_TX_GPIO_PIN * 2));
	REG_SET_VALUE(GPIOA_OTYPER, (0x01 << UART1_TX_GPIO_PIN), (0x00 << UART1_TX_GPIO_PIN));
	REG_SET_VALUE(GPIOA_OSPEEDR, (0x03 << UART1_TX_GPIO_PIN * 2), (0x03 << UART1_TX_GPIO_PIN * 2));
	REG_SET_VALUE(GPIOA_PUPDR, (0x03 << UART1_TX_GPIO_PIN * 2), (0x00 << UART1_TX_GPIO_PIN * 2));
	
	// 配置PA10
	REG_SET_VALUE(GPIOA_MODER, (0x03 << UART1_RX_GPIO_PIN * 2), (0x02 << UART1_RX_GPIO_PIN * 2));
	REG_SET_VALUE(GPIOA_OTYPER, (0x01 << UART1_RX_GPIO_PIN), (0x00 << UART1_RX_GPIO_PIN));
	REG_SET_VALUE(GPIOA_OSPEEDR, (0x03 << UART1_RX_GPIO_PIN * 2), (0x03 << UART1_RX_GPIO_PIN * 2));
	REG_SET_VALUE(GPIOA_PUPDR, (0x03 << UART1_RX_GPIO_PIN * 2), (0x01 << UART1_RX_GPIO_PIN * 2));
	
	// 配置USART1
	REG_SET_VALUE(USART1_CR1, (USART_CR1_RE | USART_CR1_TE), (USART_CR1_RE | USART_CR1_TE));
	REG_SET_VALUE(USART1_CR2, (0x3000U), (USART_CR2_STOP_1));
	REG_SET_VALUE(USART1_BRR, (0xFFFFU), 0x02D9);		// 配置波特率为115200
	REG_SET(USART1_CR1, USART_CR1_UE);					// 使能UART1
}

uint8_t uart1_rec_byte(uint32_t timout){
	while( !(REG32_VALUE(USART1_SR) & USART_SR_RXNE) && timout != 1 ){
		if(timout != 0){
			timout--;
		}
	}
	return REG8_VALUE(USART1_DR);
}

void uart1_send_byte(uint8_t value){
	while(!(REG32_VALUE(USART1_SR) & USART_SR_TXE));
	REG32_VALUE(USART1_DR) = value;
	while(!(REG32_VALUE(USART1_SR) & USART_SR_TC));
}

void uart1_send_msg(uint8_t* msg){
	while(*msg != '\0'){
		// 等待发送完成
		uart1_send_byte(*msg);
		msg++;
	}
}

int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		uart1_send_byte((uint8_t) ch);
		return (ch);
}


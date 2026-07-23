#ifndef _BSP_UART_H_
#define _BSP_UART_H_
#include "stm32f4xx.h"

#define UART1_RX_GPIO_PIN 10
#define UART1_TX_GPIO_PIN 9

void init_uart(void);
uint8_t uart1_rec_byte(uint32_t timout);
void uart1_send_byte(uint8_t value);
void uart1_send_msg(uint8_t* msg);

#endif


#ifndef __USER_HAL_CALLBACK_H
#define __USER_HAL_CALLBACK_H
#include "stm32f4xx.h"
#include "stm32f4xx_hal_uart.h"
#include "bsp_console.h"
#include "bsp_wifi.h"

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
HAL_StatusTypeDef User_UARTEx_ReceiveToIdle_DMA(UART_HandleTypeDef* huart, uint8_t *pData, uint32_t size);


#endif


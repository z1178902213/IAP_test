#ifndef __BSP_WIFI_H
#define __BSP_WIFI_H

#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "main.h"
#include "user_hal_callback.h"

#define WIFI_MAX_BUFFER_SIZE 0xff

typedef enum{
	WIFI_OK 			= 0x00U,
	WIFI_ERROR 	= 0x01U,
	WIFI_BUSY 		= 0x02U,
	WIFI_TIMEOUT = 0x03U
}WIFI_Status_Type;

void WIFI_Enable(void);
WIFI_Status_Type WIFI_Send_Command(uint8_t *pdata, uint16_t size);
WIFI_Status_Type WIFI_Receive_Message(void);

#endif


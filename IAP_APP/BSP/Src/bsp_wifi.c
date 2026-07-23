#include "bsp_wifi.h"

extern UART_HandleTypeDef huart3;



uint8_t wifi_tx_complete = 0;
uint8_t wifi_rx_buffer[WIFI_MAX_BUFFER_SIZE] = {0};
uint8_t wifi_rx_complete = 0;
uint32_t wifi_rx_buf_size = 0;

void WIFI_Enable(){
	HAL_GPIO_WritePin(WIFI_RST_GPIO_Port, WIFI_RST_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin, GPIO_PIN_SET);
}

// WIFI指令发送，向WiFi模块发送指令，目前最大仅支持接收256个字符，即512字节。
WIFI_Status_Type WIFI_Send_Command(uint8_t *pdata, uint16_t size){
	HAL_StatusTypeDef wStatus;
	wStatus = HAL_UART_Transmit_DMA(&huart3, (uint8_t *)pdata, size);
	return (WIFI_Status_Type)wStatus;
}

// WiFi指令接收，接收WiFi模块发回的数据
WIFI_Status_Type WIFI_Receive_Message(){
	HAL_StatusTypeDef wStatus;
	wStatus = User_UARTEx_ReceiveToIdle_DMA(&huart3, (uint8_t*)wifi_rx_buffer, WIFI_MAX_BUFFER_SIZE);
	return (WIFI_Status_Type)wStatus;
}



#include "user_hal_callback.h"

uint32_t tmp_wifi_rx_len = 0;
uint32_t tmp_console_rx_len = 0;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;

extern uint8_t console_tx_complete;
extern uint8_t console_rx_complete;
extern uint32_t console_rx_buf_size;

extern uint8_t wifi_tx_complete;
extern uint8_t wifi_rx_complete;
extern uint32_t wifi_rx_buf_size;

// 数据发送完成的回调函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(huart == &huart1){
		console_tx_complete = 1;
	}else if(huart == &huart3){
		wifi_tx_complete = 1;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // DMA满接收完成（收到最大长度）
    if(huart->Instance == USART1)
    {
				console_rx_buf_size = CONSOLE_MAX_BUFFER_SIZE;
				console_rx_complete = 1;
    }else if(huart->Instance == USART3)
    {
				wifi_rx_buf_size = CONSOLE_MAX_BUFFER_SIZE;
				wifi_rx_complete = 1;
    }
}


HAL_StatusTypeDef User_UARTEx_ReceiveToIdle_DMA(UART_HandleTypeDef* huart, uint8_t *pData, uint32_t Size){
    // 启动DMA接收（普通DMA，循环/正常模式自选，这里用Normal）
    if(HAL_UART_Receive_DMA(huart, pData, Size) != HAL_OK)
    {
        return HAL_ERROR;
    }
    // 使能空闲中断
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    return HAL_OK;
}

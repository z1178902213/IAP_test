#include "bsp_console.h"

extern UART_HandleTypeDef huart1;

uint8_t console_tx_complete = 0;
uint8_t console_rx_buffer[CONSOLE_MAX_BUFFER_SIZE] = {0};
uint8_t console_rx_complete = 0;
uint32_t console_rx_buf_size = 0;


// =====================================
// 私有函数的实现
// =====================================

// 控制台输出，向上位机发送消息，目前最大仅支持接收256个字符，即512字节。
Console_Status_Type Console_Send_Data(uint8_t *pdata, uint16_t size){
	HAL_StatusTypeDef txStatus;
	uint16_t data_length = 0;
	if(size == 0){
		while(data_length < CONSOLE_MAX_BUFFER_SIZE && *(pdata + data_length) != '\0'){
			data_length++;
		}
	}else{
		data_length = size;
	}
	
	txStatus = HAL_UART_Transmit_DMA(&huart1, (uint8_t *)pdata, data_length);
	return (Console_Status_Type)txStatus;
}

// 控制台输入，等待上位机发送消息
Console_Status_Type Console_Readlines(){
	HAL_StatusTypeDef rxStatus = User_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*)console_rx_buffer, CONSOLE_MAX_BUFFER_SIZE);
	return (Console_Status_Type)rxStatus;
}

// 解析指令类型
Console_Command_Type Console_Parse_Command(){
	if(console_rx_buffer[0] == 'A' && console_rx_buffer[1] == 'T'){
		return Console_Command_WiFi;
	}else if(console_rx_buffer[0] == 'E' && console_rx_buffer[1] == 'X' && console_rx_buffer[2] == 'I' && console_rx_buffer[3] == 'T'){
		return Console_Command_Exit;
	}else{
		return Console_Command_Unknown;
	}
}


///重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口DEBUG_USART */
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}

///重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		
	int ch;
	HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 10);	
	return (ch);
}


#ifndef __USER_TASK_H
#define __USER_TASK_H

#include "stm32f4xx.h"
#include <lwip/opt.h>
#include <lwip/arch.h>
#include "lwip/tcpip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/sio.h"
#include "ethernetif.h"

typedef struct{
	uint32_t Console_Level;
	uint32_t Buffer_Size;
} Console_Log_Task_Params_Type;

void User_Start_Tasks(void);
void Wait_Tx_Rx(uint8_t *flag, uint32_t delayTime);
void Wait_Tx_Rx_Forever(uint8_t *flag);
void TCPIP_Init(void);
#endif


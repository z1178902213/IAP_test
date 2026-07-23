#include "bsp_led.h"
#include "bsp_console.h"
#include "bsp_wifi.h"
#include "user_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#define CONSOLE_LEVEL_INFO (0x01 << 0)
#define CONSOLE_LEVEL_WARNING (0x01 << 1)
#define CONSOLE_LEVEL_ERROR (0x01 << 2)

TaskHandle_t APP_Create_Task_Handler = NULL;
TaskHandle_t LED_Red_Blink_Task_Handler = NULL;
TaskHandle_t TCPIO_ECHO_Task_Handler = NULL;
TaskHandle_t UART1_Test_Tx_Task_Handler = NULL;
TaskHandle_t Console_Wait_Command_Task_Handler = NULL;
TaskHandle_t Console_Log_Task_Handler = NULL;
Console_Log_Task_Params_Type Console_Log_Task_Param;

static void APP_Create_Task(void);
static void LED_Red_Blink_Task(void);
static void Console_Wait_Command_Task(void);
static void tcpecho_thread(void *arg);

extern UART_HandleTypeDef huart1;
extern uint8_t console_tx_complete;
extern uint8_t console_rx_buffer[];
extern uint8_t console_rx_complete;
extern uint32_t console_rx_buf_size;
extern uint8_t wifi_tx_complete;
extern uint8_t wifi_rx_buffer[];
extern uint8_t wifi_rx_complete;
extern uint32_t wifi_rx_buf_size;


void User_Start_Tasks(){
	xTaskCreate(
							(TaskFunction_t) APP_Create_Task,
							"APP_Create_Task",
							512,
							NULL,
							4,
							&APP_Create_Task_Handler);
	
	vTaskStartScheduler();
}

static void APP_Create_Task(){
	TCPIP_Init();
	
	xTaskCreate(
							(TaskFunction_t) tcpecho_thread,
							"tcpecho_thread",
							512,
							NULL,
							4,
							&TCPIO_ECHO_Task_Handler);
	
	// 创建LED闪烁的任务
	xTaskCreate(
							(TaskFunction_t) LED_Red_Blink_Task,
							"LED_Red_Blink_Task",
							configMINIMAL_STACK_SIZE,
							NULL,
							2,
							&LED_Red_Blink_Task_Handler);
	
	// 创建获取控制台输入的任务
	xTaskCreate(
							(TaskFunction_t) Console_Wait_Command_Task,
							"Console_Wait_Command_Task",
							configMINIMAL_STACK_SIZE,
							NULL,
							3,
							&Console_Wait_Command_Task_Handler);
	
	// 删除创建任务的任务
	vTaskDelete(APP_Create_Task_Handler);
}


static void LED_Red_Blink_Task(){
	while(1){
		LED_Toggle(LED_R_GPIO_Port, LED_R_Pin);
		vTaskDelay(500);
		LED_Toggle(LED_R_GPIO_Port, LED_R_Pin);
		vTaskDelay(500);
		LED_Toggle(LED_G_GPIO_Port, LED_G_Pin);
		vTaskDelay(500);
		LED_Toggle(LED_G_GPIO_Port, LED_G_Pin);
		vTaskDelay(500);
		LED_Toggle(LED_B_GPIO_Port, LED_B_Pin);
		vTaskDelay(500);
		LED_Toggle(LED_B_GPIO_Port, LED_B_Pin);
		vTaskDelay(500);
	}
}



// ===============================================================================================
// 功能：Console_Wait_Command_Task任务，通过控制台接收上位机发送的命令
// 使用方式：无需手动调用，该任务每隔100ms就会检查上位机是否有指令发送，并且会自动匹配指令的相关功能
// ===============================================================================================
static void Console_Wait_Command_Task(){
	Console_Command_Type cmdType;		// 收到的指令类型
	Console_Status_Type cStatus; 
	Console_Command_Mode_Type cmdMode = COMMAND_NORMAL_MODE;
	uint8_t wifi_rx_flag = 0;
	char *msg;											// 通过控制台输出的消息
	while(1){
		if(cmdMode == COMMAND_NORMAL_MODE){
			printf("==> 一般指令模式：\n");
		}else if(cmdMode == COMMAND_WIFI_MODE){
			printf("==> WiFi指令模式：\n");
		}
		
		cStatus = Console_Readlines();		// 开启接收的监听
		if(cStatus == CONSOLE_OK){
			Wait_Tx_Rx(&console_rx_complete, 100);		// 看看是否接收到数据，如果没收到就将任务阻塞100ms
			
			// 代码能到这里就说明收到数据了
			cmdType = Console_Parse_Command();
			if(cmdMode == COMMAND_NORMAL_MODE){
				if(cmdType == Console_Command_WiFi){
					printf("==> 切换到WiFi指令模式：\n");
					cmdMode = COMMAND_WIFI_MODE;
					
					// 把控制台收到的数据原封不动转发给WiFi模块
					WIFI_Send_Command(console_rx_buffer, console_rx_buf_size);
					Wait_Tx_Rx_Forever(&wifi_tx_complete);		// 和WiFi有关的操作，不知道为什么不能让任务调度，一调度就不能用，所以在这里死等
					
					// 等待接收WiFi模块返回的消息
					WIFI_Receive_Message();
					Wait_Tx_Rx_Forever(&wifi_rx_complete);		// 和WiFi有关的操作，不知道为什么不能让任务调度，一调度就不能用，所以在这里死等
					
					// 把WiFi返回的消息输出到给控制台
					Console_Send_Data(wifi_rx_buffer, wifi_rx_buf_size);
					Wait_Tx_Rx(&console_tx_complete, 100);
					
				}else if(cmdType == Console_Command_Unknown){
					msg = "==> error: 未知指令，无操作\n";
					Console_Send_Data((uint8_t*)msg, 0);
					Wait_Tx_Rx(&console_tx_complete, 100);
				}
			}else if(cmdMode == COMMAND_WIFI_MODE){
				// 当前指令模式是WiFi模式，那就是收到什么就转发什么，除非遇到了EXIT指令，那就退出WiFi模式
				if(cmdType == Console_Command_Exit){
					printf("==> 切换到一般指令模式：");
					cmdMode = COMMAND_NORMAL_MODE;
				} else {
					// 把控制台收到的数据原封不动转发给WiFi模块
					WIFI_Send_Command(console_rx_buffer, console_rx_buf_size);
					Wait_Tx_Rx_Forever(&wifi_tx_complete);		// 和WiFi有关的操作，不知道为什么不能让任务调度，一调度就不能用
					
					// 等待接收WiFi模块返回的消息
					WIFI_Receive_Message();
					Wait_Tx_Rx_Forever(&wifi_rx_complete);		// 和WiFi有关的操作，不知道为什么不能让任务调度，一调度就不能用
					
					// 把WiFi返回的消息输出到给控制台
					Console_Send_Data(wifi_rx_buffer, wifi_rx_buf_size);
					Wait_Tx_Rx(&console_tx_complete, 100);
				}
			}
		}else{
			vTaskDelay(100);
		}
	}
}

// 等待发送或者接收，完事会清除标志位
void Wait_Tx_Rx(uint8_t *flag, uint32_t delayTime){
	while(*flag != 1){
		vTaskDelay(delayTime);
	}
	*flag = 0;
}
// 一直死循环，等待发送或者接收，完事会清除标志位
void Wait_Tx_Rx_Forever(uint8_t *flag){
	while(*flag != 1);
	*flag = 0;
}

static void 
tcpecho_thread(void *arg)
{
  struct netconn *conn, *newconn;
  err_t err;
  LWIP_UNUSED_ARG(arg);

  /* Create a new connection identifier. */
  /* Bind connection to well known port number 7. */
#if LWIP_IPV6
  conn = netconn_new(NETCONN_TCP_IPV6);
  netconn_bind(conn, IP6_ADDR_ANY, LOCAL_PORT);
#else /* LWIP_IPV6 */
  conn = netconn_new(NETCONN_TCP);
  netconn_bind(conn, IP_ADDR_ANY, 5001);
#endif /* LWIP_IPV6 */
  LWIP_ERROR("tcpecho: invalid conn", (conn != NULL), return;);

  printf("本地端口号是%d\n\n", 5001);
  
  /* Tell connection to go into listening mode. */
  netconn_listen(conn);

  while (1) {

    /* Grab new connection. */
    err = netconn_accept(conn, &newconn);
    /*printf("accepted new connection %p\n", newconn);*/
    /* Process the new connection. */
    if (err == ERR_OK) {
      struct netbuf *buf;
      void *data;
      u16_t len;
      
      while ((err = netconn_recv(newconn, &buf)) == ERR_OK) {
        /*printf("Recved\n");*/
        do {
             netbuf_data(buf, &data, &len);
             err = netconn_write(newconn, data, len, NETCONN_COPY);
#if 0
            if (err != ERR_OK) {
              printf("tcpecho: netconn_write: error \"%s\"\n", lwip_strerr(err));
            }
#endif
        } while (netbuf_next(buf) >= 0);
        netbuf_delete(buf);
      }
      /*printf("Got EOF, looping\n");*/ 
      /* Close connection and discard connection identifier. */
      netconn_close(newconn);
      netconn_delete(newconn);
    }
  }
}


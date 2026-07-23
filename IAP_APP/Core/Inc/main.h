/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_led.h"
#include "bsp_wifi.h"
#include "bsp_console.h"
#include "bsp_eth.h"
#include "user_task.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
//void TCPIP_Init(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define WIFI_EN_Pin GPIO_PIN_2
#define WIFI_EN_GPIO_Port GPIOE
#define SW3_Pin GPIO_PIN_13
#define SW3_GPIO_Port GPIOC
#define LED_R_Pin GPIO_PIN_6
#define LED_R_GPIO_Port GPIOF
#define LED_G_Pin GPIO_PIN_7
#define LED_G_GPIO_Port GPIOF
#define LED_B_Pin GPIO_PIN_8
#define LED_B_GPIO_Port GPIOF
#define ETH_MDC_USER_Pin GPIO_PIN_1
#define ETH_MDC_USER_GPIO_Port GPIOC
#define SW1_Pin GPIO_PIN_0
#define SW1_GPIO_Port GPIOA
#define ETH_MDIO_USER_Pin GPIO_PIN_2
#define ETH_MDIO_USER_GPIO_Port GPIOA
#define ETH_RXD0_USER_Pin GPIO_PIN_4
#define ETH_RXD0_USER_GPIO_Port GPIOC
#define ETH_RXD1_Pin GPIO_PIN_5
#define ETH_RXD1_GPIO_Port GPIOC
#define WIFI_RX_Pin GPIO_PIN_10
#define WIFI_RX_GPIO_Port GPIOB
#define WIFI_TX_Pin GPIO_PIN_11
#define WIFI_TX_GPIO_Port GPIOB
#define UART_TX_Pin GPIO_PIN_9
#define UART_TX_GPIO_Port GPIOA
#define UART_RX_Pin GPIO_PIN_10
#define UART_RX_GPIO_Port GPIOA
#define ETH_TX_EN_Pin GPIO_PIN_11
#define ETH_TX_EN_GPIO_Port GPIOG
#define ETH_TXD0_Pin GPIO_PIN_13
#define ETH_TXD0_GPIO_Port GPIOG
#define ETH_TXD1_Pin GPIO_PIN_14
#define ETH_TXD1_GPIO_Port GPIOG
#define WIFI_RST_Pin GPIO_PIN_15
#define WIFI_RST_GPIO_Port GPIOG

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

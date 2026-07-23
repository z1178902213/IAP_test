#ifndef __BSP_LED_H
#define __BSP_LED_H
#include "stm32f4xx.h"
#include "main.h"
#include "stm32f4xx_hal_gpio.h"

#define LED_ON GPIO_PIN_RESET
#define LED_OFF GPIO_PIN_SET

void LED_Turn_On(GPIO_TypeDef* LED_Color, uint16_t LED_Pin);
void LED_Turn_Off(GPIO_TypeDef* LED_Color, uint16_t LED_Pin);
void LED_Toggle(GPIO_TypeDef* LED_Color, uint16_t LED_Pin);


#endif


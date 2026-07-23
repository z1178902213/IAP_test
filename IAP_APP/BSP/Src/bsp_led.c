#include "bsp_led.h"

// µãÁÁLED
void LED_Turn_On(GPIO_TypeDef* LED_Color, uint16_t LED_Pin){
	HAL_GPIO_WritePin(LED_Color, LED_Pin, LED_ON);
}

// Ï¨ÃðLED
void LED_Turn_Off(GPIO_TypeDef* LED_Color, uint16_t LED_Pin){
	HAL_GPIO_WritePin(LED_Color, LED_Pin, LED_OFF);
}

// ·­×ªLED×´Ì¬
void LED_Toggle(GPIO_TypeDef* LED_Color, uint16_t LED_Pin){
	HAL_GPIO_TogglePin(LED_Color, LED_Pin);
}


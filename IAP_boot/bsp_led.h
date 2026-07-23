#ifndef _BSP_LED_H_
#define _BSP_LED_H_
#include "stm32f4xx.h"

#define LED_RED_GPIO_PIN 6
#define LED_GREEN_GPIO_PIN 7
#define LED_BLUE_GPIO_PIN 8

void init_led(void);
void toggle_led(uint32_t LED_COLOR_GPIO_PIN);

#endif


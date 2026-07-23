#include "app.h"

void delay(){
	uint32_t i = 0x7fffff;
	for(; i > 0; i--){}
}

void Default_App(){
	toggle_led(LED_RED_GPIO_PIN);
	delay();
	toggle_led(LED_RED_GPIO_PIN);
	toggle_led(LED_BLUE_GPIO_PIN);
	delay();
	toggle_led(LED_BLUE_GPIO_PIN);
	toggle_led(LED_GREEN_GPIO_PIN);
	delay();
	toggle_led(LED_GREEN_GPIO_PIN);
}


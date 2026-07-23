#include "stm32f4xx.h"
#include "user_rcc.h"

void reg_set_value(uint32_t *__REG__, uint32_t __MASK__, uint32_t __VALUE__){
		*__REG__ &= ~__MASK__;
		*__REG__ |= __VALUE__;
}

void user_rcc_osc_init(){
	volatile uint32_t rcc_cr = 0x0000U;
	uint32_t timeout = 0xfffff;
	uint32_t test = 0xFFFFFFFF;
	uint32_t test2 = 0x000FF000;
	uint32_t test3 = 0x00018000;
	
	reg_set_value(&test, test2, test3);
	
	// 使能HSE
	REG_SET_VALUE(RCC_CR, RCC_CR_HSEON, RCC_CR_HSEON);
	
	// 检测HSE振荡器是否稳定
	while(((REG32_VALUE(RCC_CR) & RCC_CR_HSERDY) != RCC_CR_HSERDY) && timeout > 0){
		timeout--;
	}
	
	// 先关PLL
	REG_RESET(RCC_CR, RCC_CR_PLLON);
	// 配置PLL - 设置HSE为时钟源
	REG_SET_VALUE(RCC_PLLCFGR, RCC_PLLCFGR_PLLSRC, 1 << 22);
	// 配置PLL - M=25、N=336、P=2、Q=7
	REG_SET_VALUE(RCC_PLLCFGR, RCC_PLLCFGR_PLLM, 25 << 0);
	REG_SET_VALUE(RCC_PLLCFGR, RCC_PLLCFGR_PLLN, 336 << 6);
	// 这里PLLP配置出错，想当然把2左移16位，实际上PLLP的2分频应该给0才对。
	// 导致PLLCLK时钟有问题，总是被重置成HSI的时钟，导致时钟频率出错。
	REG_SET_VALUE(RCC_PLLCFGR, RCC_PLLCFGR_PLLP, 0 << 16);
	REG_SET_VALUE(RCC_PLLCFGR, RCC_PLLCFGR_PLLQ, 7 << 24);
	// 使能PLL
	REG_SET(RCC_CR, RCC_CR_PLLON);
	timeout = 0xfffff;
	while(((REG32_VALUE(RCC_CR) & RCC_CR_PLLRDY) != RCC_CR_PLLRDY) && timeout > 0){
		timeout --;
	}
}

void user_rcc_clock_init(){
	uint32_t timeout = 0xfffff;
	uint32_t FATENCY = 0x00000000;
	
	// 开启电源域时钟
	REG_SET(RCC_APB1ENR, RCC_APB1ENR_PWREN);
	// 切换到VOS1最高性能电压
	REG_SET_VALUE(PWR_CR, PWR_CR_VOS, PWR_CR_VOS);
	// 等待电压稳定就绪
	while( !(REG32_VALUE(PWR_CSR) & PWR_CSR_VOSRDY) );
	
	// 注意：修改系统时钟之前，要先把FLASH时钟调整到合适的频率，免得FLASH无法访问
	FATENCY = REG32_VALUE(FLASH_ACR) & FLASH_ACR_LATENCY;
	if(FATENCY < (uint32_t)0x05){
		// REG_SET_VALUE(FLASH_ACR, 0xFFFFU, 0x05U | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN);
		REG_SET_VALUE(FLASH_ACR, 0xFFFFU, 0x05U);
	}
	
	// 注意，需要先配置几个总线时钟，然后才将PLL切换到SYSCLK
	REG_SET_VALUE(RCC_CFGR, RCC_CFGR_PPRE1, 0x05U << 10);		// 配置APB1时钟
	REG_SET_VALUE(RCC_CFGR, RCC_CFGR_PPRE2, 0x04U << 13);		// 配置APB2时钟
	// 好像要先配置完APB1和APB2时钟，才能配置AHB时钟
	REG_SET_VALUE(RCC_CFGR, RCC_CFGR_HPRE, 0x00U << 4);			// 配置AHB时钟
	
	// 配置PLLCLK为系统时钟SYSCLK
	REG_SET_VALUE(RCC_CFGR, RCC_CFGR_SW, 0x02 << 0);
	
	for(; timeout > 0; timeout --);
	timeout = 0xfffff;
	
	// 检测PLL是否切换成功
	while(((REG32_VALUE(RCC_CFGR) & RCC_CFGR_SWS) != (0x02 << 2)) && timeout > 0){
		timeout--;
	}
	
}


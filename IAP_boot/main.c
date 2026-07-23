#include "stm32f4xx.h"
#include "user_rcc.h"
#include "bsp_led.h"
#include "bsp_uart.h"
#include "app.h"
#include "iflash.h"
#include "loader.h"
#include "app2.h"

#define APP_ADDR 0x08040000UL
typedef void (*AppResetHandler)(void);

const uint8_t msg[] = "请选择启动方式：\n1.烧录模式\n2.恢复出厂设置\n3.进入系统（5秒后自动进入）\n";

void jumpAPP(void);

void init_rcc(){
	user_rcc_osc_init();
	user_rcc_clock_init();
}

int main(){
	uint8_t chs;
	init_rcc();
	init_led();
	init_uart();
	
	printf((const char *)msg);
	chs = uart1_rec_byte(0x2FFFFFF);
	
	
	if(chs == '1' || chs == 1){
		printf("==> 烧录模式\n");
		loader();
	}else if(chs == '2' || chs == 2){
		printf("==> 恢复出厂设置\n");
		factory();
	}else{
		printf("==> 启动系统\n");
	}
	
	jumpAPP();
	
	while(1){

	}
}

void SystemInit(){
	REG32_VALUE(RCC_CR) |= (uint32_t)0x00000001;
	REG32_VALUE(RCC_CFGR) = 0x00000000;
	REG32_VALUE(RCC_CR) &= (uint32_t)0xFEF6FFFF;
	REG32_VALUE(RCC_PLLCFGR) = 0x24003010;
	REG32_VALUE(RCC_CR) &= (uint32_t)0xFFFBFFFF;
	REG32_VALUE(RCC_CIR) = 0x00000000;
}

__asm void IAP_JumpToApp(uint32_t ulAppMSP, uint32_t ulAppResetEntry)
{
/* *INDENT-OFF* */
    PRESERVE8
		CPSID I

    ; R0 = ulAppMSP
    ; R1 = ulAppResetEntry
    MSR     MSP, R0 
    BX      R1 
/* *INDENT-ON* */
}

void jumpAPP(){
	uint32_t app_sp = *(volatile uint32_t *)APP_ADDR;
	uint32_t app_reset_entry = *(volatile uint32_t *)(APP_ADDR + 4);
    // ========== 校验APP镜像合法性 ==========
    // F4 SRAM范围 0x20000000 ~ 0x2002FFFF(192KB)
    if ((app_sp < 0x20000000UL) || (app_sp > 0x20030000UL))
    {
        // APP无效，直接返回，不跳转
        return;
    }
    if (app_reset_entry < APP_ADDR || app_reset_entry > 0x080FFFFFUL)
    {
        return;
    }
		
    // 1. 开启HSI
		REG_SET(RCC_CR, RCC_CR_HSION);
    while( (REG32_VALUE(RCC_CR) & RCC_CR_HSIRDY) != RCC_CR_HSIRDY );

		uint32_t FATENCY = REG32_VALUE(FLASH_ACR) & FLASH_ACR_LATENCY;
		if(FATENCY >= (uint32_t)0x05){
			// REG_SET_VALUE(FLASH_ACR, 0xFFFFU, 0x05U | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN);
			REG_SET_VALUE(FLASH_ACR, 0xFFFFU, 0x00U);
		}
		
    // 2. 系统时钟切换为 HSI
		REG_SET_VALUE(RCC_CFGR, RCC_CFGR_SW, 0x00);
    while( (REG32_VALUE(RCC_CFGR) & RCC_CFGR_SWS) != 0 );

    // 3. 关闭 PLL
		REG_SET_VALUE(RCC_CR, RCC_CR_PLLON, 0);
    while( REG32_VALUE(RCC_CR) & RCC_CR_PLLRDY );

    // 4. 关闭HSE
		REG_RESET(RCC_CR, RCC_CR_HSEON);
		
		
    // 5. 载入APP主栈指针 MSP
    IAP_JumpToApp(app_sp, app_reset_entry);

    // 6. 跳转至APP复位服务函数
    ((AppResetHandler)app_reset_entry)();

    // 理论永远执行不到这里
    while(1);
}



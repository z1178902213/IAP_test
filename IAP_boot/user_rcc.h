#ifndef _USER_RCC_H_
#define _USER_RCC_H_
#define FLASH_ACR_PRFTEN (1U << 8)
#define FLASH_ACR_ICEN   (1U << 9)
#define FLASH_ACR_DCEN   (1U << 10)

void user_rcc_osc_init(void);
void user_rcc_clock_init(void);

#endif


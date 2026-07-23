#ifndef __IFLASH_H
#define __IFLASH_H
#include "stm32f4xx.h"
#include "bsp_uart.h"

#define FLASH_KEY1 ((uint32_t)0x45670123)
#define FLASH_KEY2 ((uint32_t)0xCDEF89AB)

void FLASH_Unlock(void);
void FLASH_Lock(void);
void FLASH_Erase_Sector(uint8_t choose);
void FLASH_ProgramWord(uint32_t Address, uint32_t Data);

#endif


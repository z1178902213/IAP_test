#include "iflash.h"
/* 内部FLASH操作的代码
*/

// 解锁内部FLASH
void FLASH_Unlock(){
	if((REG32_VALUE(FLASH_CR) & FLASH_CR_LOCK) == FLASH_CR_LOCK){
		REG_SET_VALUE(FLASH_KEYR, FLASH_KEYR_KSY, FLASH_KEY1);
		REG_SET_VALUE(FLASH_KEYR, FLASH_KEYR_KSY, FLASH_KEY2);
	}
}

// 上锁内部FLASH
void FLASH_Lock(){
	REG_SET(FLASH_CR, FLASH_CR_LOCK);
}

// 让FLASH擦除指定扇区，传入参数choose：0擦除RUN区，1擦除APP区
void FLASH_Erase_Sector(uint8_t choose){
	uint8_t i = 0;
	const uint8_t target0[3] = {6, 7, 8};
	const uint8_t target1[3] = {9, 10, 11};
	
	// 如果处于繁忙的状态就等待
	printf("==> 正在检查FLASH是否空闲\n");
	while((REG32_VALUE(FLASH_SR) & FLASH_SR_BSY) == FLASH_SR_BSY);
	
	// 配置成32位编程模式
	REG_SET_VALUE(FLASH_CR, FLASH_CR_PSIZE, 2 << 8);
	
	if(choose == 0){
		printf("==> 正在擦除RUN扇区：");
		for(i = 0; i < 3; i++){
			printf("%d", target0[i]);
			REG_SET(FLASH_CR, FLASH_CR_SER);
			REG_SET_VALUE(FLASH_CR, FLASH_CR_SNB, target0[i] << 3);
			REG_SET(FLASH_CR, FLASH_CR_STRT);
			// 等待擦除完成
			while((REG32_VALUE(FLASH_SR) & FLASH_SR_BSY) == FLASH_SR_BSY);
		}
	}else if(choose == 1){
		printf("==> 正在擦除APP扇区：");
		for(i = 0; i < 3; i++){
			printf("%d", target1[i]);
			REG_SET(FLASH_CR, FLASH_CR_SER);
			REG_SET_VALUE(FLASH_CR, FLASH_CR_SNB, target1[i] << 3);
			REG_SET(FLASH_CR, FLASH_CR_STRT);
			// 等待擦除完成
			while((REG32_VALUE(FLASH_SR) & FLASH_SR_BSY) == FLASH_SR_BSY);
		}
	}
	
	// 全部完成以后清除SER，失能擦除
	REG_RESET(FLASH_CR, FLASH_CR_SER);
}

// 按字向FLASH写入数据
void FLASH_ProgramWord(uint32_t Address, uint32_t Data){
	while((REG32_VALUE(FLASH_SR) & FLASH_SR_BSY) == FLASH_SR_BSY);
	
	// 配置成32位编程模式
	REG_SET_VALUE(FLASH_CR, FLASH_CR_PSIZE, 2 << 8);
	REG_SET(FLASH_CR, FLASH_CR_PG);
	
	*(volatile uint32_t*)Address = Data;
	
	REG_RESET(FLASH_CR, FLASH_CR_PG);
	while((REG32_VALUE(FLASH_SR) & FLASH_SR_BSY) == FLASH_SR_BSY);
}


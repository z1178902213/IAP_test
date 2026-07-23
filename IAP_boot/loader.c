#include "bsp_uart.h"
#include "iflash.h"
#include "loader.h"

void loader(void){
	uint32_t i = 0;
	uint8_t ch = 0;
	uint32_t app2_entry = 0x080A0000;
	uint8_t tmp[200] = {0};
	uint32_t rec_buffer = 0;
	
	printf("==> 进入烧录模式，请输入BIN文件大小：\n");
	
	uint32_t file_size = 0;
	
	for(i=0; i< 4; i++){
		ch = uart1_rec_byte(0); 
		file_size |= (ch << ( 8 * ( 3 - i )));
	}
	
	FLASH_Unlock();
	FLASH_Erase_Sector(1);
	printf("==> 文件大小%d，请发送bin文件...\n", file_size);
	
	for(i=0; i< file_size; i++){
		ch = uart1_rec_byte(0);
		if(i < 200){
			tmp[i] = ch;
		}
		rec_buffer |= (ch << ( 8 * ( i % 4 )));
		if((i + 1) % 4 == 0){
			FLASH_ProgramWord((app2_entry + (i/4)*4), rec_buffer);
			rec_buffer = 0;
		}
	}
	printf("==> 内存前200个字节数据：\n");
	for(i=0; i< 200; i++){
		printf("0x%02X ", tmp[i]);
		if((i + 1)%10 == 0)
			printf("\n");
	}

	printf("==> ROM中前200的数据：\n");
	for(i=0; i< 200; i++){
		printf("0x%02X ", *(uint8_t*)(app2_entry+i));
		if((i + 1)%10 == 0)
			printf("\n");
	}
	
	FLASH_Lock();
	
	printf("==> 文件烧录完成，正在校验...\n");
	factory();
}

void factory(){
	uint32_t app_entry = 0x08040000;
	uint32_t app_backup = 0x080A0000;
	uint32_t i = 0;
	uint32_t sector_size = 0x60000;
	
	printf("==> 解锁FLASH...\n");
	FLASH_Unlock();
	FLASH_Erase_Sector(0);
	
	
	printf("==> 将固件恢复到运行区...\n");
	for(i = 0; i< sector_size; i+=4){
		FLASH_ProgramWord(app_entry + i, *((uint32_t *)(app_backup+i)));
		if(i % 4000 == 0){
			printf("==> 当前进度：%d/%d\n", i, sector_size);
		}
	}
	printf("==> 将固件恢复到运行区完成，即将启动系统...\n");
	
	FLASH_Lock();
}


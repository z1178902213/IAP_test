#include "bsp_spi.h"

void init_spi1(){
	REG_SET(RCC_AHB1ENR, RCC_AHB1ENR_GPIOBEN);			// 开GPIOB的时钟
	REG_SET(RCC_AHB1ENR, RCC_AHB1ENR_GPIOGEN);			// 开GPIOG的时钟，PG6线选信号要用到
	REG_SET(RCC_APB2ENR, RCC_APB2ENR_SPI1EN);			// 开SPI1的时钟
	
	// 配置PB3、PB4、PB5的复用，复用成SPI1。配置PG6为系统默认。
	REG_SET_VALUE(GPIOB_AFRL, (0x0FU << SPI1_CLK_GPIO_PIN * 4), (GPIOB_AF_SPI1 << SPI1_CLK_GPIO_PIN * 4));
	REG_SET_VALUE(GPIOB_AFRL, (0x0FU << SPI1_DO_GPIO_PIN * 4), (GPIOB_AF_SPI1 << SPI1_DO_GPIO_PIN * 4));
	REG_SET_VALUE(GPIOB_AFRL, (0x0FU << SPI1_DI_GPIO_PIN * 4), (GPIOB_AF_SPI1 << SPI1_DI_GPIO_PIN * 4));
	REG_SET_VALUE(GPIOG_AFRL, (0x0FU << SPI1_CS_GPIO_PIN * 4), (GPIOG_AF_NONE << SPI1_CS_GPIO_PIN * 4));
	
	// 配置PB3
	REG_SET_VALUE(GPIOB_MODER, (0x03 << SPI1_CLK_GPIO_PIN * 2), (0x02 << SPI1_CLK_GPIO_PIN * 2));
	REG_SET_VALUE(GPIOB_OTYPER, (0x01 << SPI1_CLK_GPIO_PIN), (0x00 << SPI1_CLK_GPIO_PIN));
	REG_SET_VALUE(GPIOB_OSPEEDR, (0x03 << SPI1_CLK_GPIO_PIN * 2), (0x03 << SPI1_CLK_GPIO_PIN * 2));
	REG_SET_VALUE(GPIOB_PUPDR, (0x03 << SPI1_CLK_GPIO_PIN * 2), (0x00 << SPI1_CLK_GPIO_PIN * 2));

	// 配置PB4 
	REG_SET_VALUE(GPIOB_MODER, (0x03 << SPI1_DO_GPIO_PIN * 2), (0x02 << SPI1_DO_GPIO_PIN * 2));
	REG_SET_VALUE(GPIOB_OTYPER, (0x01 << SPI1_DO_GPIO_PIN), (0x00 << SPI1_DO_GPIO_PIN));
	REG_SET_VALUE(GPIOB_OSPEEDR, (0x03 << SPI1_DO_GPIO_PIN * 2), (0x03 << SPI1_DO_GPIO_PIN * 2));
	REG_SET_VALUE(GPIOB_PUPDR, (0x03 << SPI1_DO_GPIO_PIN * 2), (0x00 << SPI1_DO_GPIO_PIN * 2));
																
	// 配置PB5                    
	REG_SET_VALUE(GPIOB_MODER, (0x03 << SPI1_DI_GPIO_PIN * 2), (0x02 << SPI1_DI_GPIO_PIN * 2));
	REG_SET_VALUE(GPIOB_OTYPER, (0x01 << SPI1_DI_GPIO_PIN), (0x00 << SPI1_DI_GPIO_PIN));
	REG_SET_VALUE(GPIOB_OSPEEDR, (0x03 << SPI1_DI_GPIO_PIN * 2), (0x03 << SPI1_DI_GPIO_PIN * 2));
	REG_SET_VALUE(GPIOB_PUPDR, (0x03 << SPI1_DI_GPIO_PIN * 2), (0x00 << SPI1_DI_GPIO_PIN * 2));
																
	// 配置PG6                    
	REG_SET_VALUE(GPIOG_MODER, (0x03 << SPI1_CS_GPIO_PIN * 2), (0x01 << SPI1_CS_GPIO_PIN * 2));
	REG_SET_VALUE(GPIOG_OTYPER, (0x01 << SPI1_CS_GPIO_PIN), (0x00 << SPI1_CS_GPIO_PIN));
	REG_SET_VALUE(GPIOG_OSPEEDR, (0x03 << SPI1_CS_GPIO_PIN * 2), (0x03 << SPI1_CS_GPIO_PIN * 2));
	REG_SET_VALUE(GPIOG_PUPDR, (0x03 << SPI1_CS_GPIO_PIN * 2), (0x01 << SPI1_CS_GPIO_PIN * 2));
	
	// 配置SPI1：8位数据帧
	// 如果要把SSM配置成软件控制，那SSI也要配置成1，如果是0会有问题
	REG_SET_VALUE(SPI1_CR1, (0x0FFF), (SPI_CR1_CPHA | SPI_CR1_CPOL | SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BR_128));
	REG_SET(SPI1_CR1, SPI_CR1_SPE);					// 使能SPI1
}

uint8_t spi1_wait_busy(){
	uint32_t timeout = 0xfffff;
	// 等待不忙
	while((REG32_VALUE(SPI1_SR) & SPI_SR_BSY) == SPI_SR_BSY && timeout > 0){
		timeout--;
	}
	if(timeout == 0){
		return 0;
	}else{
		return 1;
	}
}

void spi1_flash_cs_on(){
	REG_SET(GPIOG_BSSR, (0x01 << (6+16)));
}

void spi1_flash_cs_off(){
	REG_SET(GPIOG_BSSR, (0x01 << 6));
}

uint8_t spi1_send_byte(uint8_t data){
	uint32_t timeout = 0xfffff;
	while(((REG32_VALUE(SPI1_SR) & SPI_SR_TXE) != SPI_SR_TXE) && timeout > 0){
		timeout--;
	}
	if(timeout > 0){
		timeout = 0xfffff;
	}else{
		return 0;
	}
	REG_SET_VALUE(SPI1_DR, 0x00FF, data);
	
	while(((REG32_VALUE(SPI1_SR) & SPI_SR_RXNE) != SPI_SR_RXNE) && timeout > 0){
		timeout--;
	}
	if(timeout > 0){
		timeout = 0xfffff;
	}else{
		return 0;
	}
	return REG8_VALUE(SPI1_DR);
}

uint8_t spi1_receive_byte(){
	return spi1_send_byte(0xFF);
}

uint32_t spi1_flash_read_id(){
	uint32_t ret = 0x00000000;
	uint16_t waittt = 0xFFFF;
	
	spi1_flash_cs_on();
	
	for(;waittt > 0; waittt--);
	spi1_send_byte(SPI_FLASH_JEDECID);
	ret |= spi1_send_byte(0xFF);
	ret |= spi1_send_byte(0xFF)<<8;
	ret |= spi1_send_byte(0xFF)<<16;
	
	spi1_flash_cs_off();
	return ret;
}

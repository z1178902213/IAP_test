#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_
#include "stm32f4xx.h"

#define SPI1_CLK_GPIO_PIN 3
#define SPI1_DO_GPIO_PIN 4
#define SPI1_DI_GPIO_PIN 5
#define SPI1_CS_GPIO_PIN 6

#define SPI_FLASH_JEDECID 0x9F

void init_spi1(void);
uint8_t spi1_wait_busy(void);
uint8_t spi1_send_byte(uint8_t data);
uint8_t spi1_receive_byte(void);
uint32_t spi1_flash_read_id(void);
void	spi1_flash_cs_off(void);

#endif


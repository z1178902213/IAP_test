#include <stdint.h>
#include <stdio.h>

#ifndef _STM32F4XX_H_
#define _STM32F4XX_H_

// 这个REG要传入变量，会修改变量值，比如要给低4位改成0110，其他不变，mask=0x0000000F，value=0x00000006
#define REG_SET_VALUE(__REG__, __MASK__, __VALUE__) \
    do                                              \
    {   uint32_t tmp = REG32_VALUE(__REG__);        \
        tmp &= ~__MASK__;                       		\
        tmp |= __VALUE__;                       		\
				REG32_VALUE(__REG__) = tmp;									\
    } while (0)

// 寄存器置位，直接传入寄存器变量和寄存器掩码，就是要置位的那几个位，就可以置位了
#define REG_SET(__REG__, __MASK__) \
    do                              \
    {   uint32_t tmp = REG32_VALUE(__REG__);        \
        tmp &= ~__MASK__;        \
        tmp |= __MASK__;        \
				REG32_VALUE(__REG__) = tmp;									\
    } while (0)

// 寄存器复位，直接传入寄存器变量和寄存器掩码，就是要复位的那几个位，就可以复位了
#define REG_RESET(__REG__, __MASK__) \
    do                              \
    {   uint32_t tmp = REG32_VALUE(__REG__);        \
        tmp &= ~__MASK__;        \
				REG32_VALUE(__REG__) = tmp;									\
    } while (0)

#define REG_GET_BIT(__REG__, __MASK__) (__REG__ & __MASK__)

#define FLASH_BASE 0x40023C00
#define FLASH_ACR (FLASH_BASE + 0x00)
#define FLASH_ACR_LATENCY (0x07U << 0)
#define FLASH_KEYR (FLASH_BASE + 0x04)
#define FLASH_KEYR_KSY (0xFFFFFFFF)
		
#define FLASH_SR (FLASH_BASE + 0x0C)
#define FLASH_SR_BSY (0x01U << 16)
		
#define FLASH_CR (FLASH_BASE + 0x10)
#define FLASH_CR_PG (0x01U << 0)
#define FLASH_CR_SER (0x01U << 1)
#define FLASH_CR_MER (0x01U << 2)
#define FLASH_CR_SNB (0x0FU << 3)
#define FLASH_CR_PSIZE (0x03U << 8)
#define FLASH_CR_STRT (0x01U << 16)
#define FLASH_CR_LOCK (0x01U << 31)

#define RCC_BASE 0x40023800
#define RCC_CR (RCC_BASE + 0x00)
#define RCC_CR_HSION (0x01 << 0)
#define RCC_CR_HSIRDY (0x01 << 1)
#define RCC_CR_HSEON (0x01 << 16)
#define RCC_CR_HSERDY (0x01U << 17)
#define RCC_CR_PLLON (0x01 << 24)
#define RCC_CR_PLLRDY (0x01 << 25)

#define RCC_PLLCFGR (RCC_BASE + 0x04)
#define RCC_PLLCFGR_PLLQ 0x0F000000
#define RCC_PLLCFGR_PLLSRC 0x00400000
#define RCC_PLLCFGR_PLLP 0x00030000
#define RCC_PLLCFGR_PLLN 0x00007FC0
#define RCC_PLLCFGR_PLLM 0x0000003F

#define RCC_CFGR (RCC_BASE + 0x08)
#define RCC_CFGR_SW (0x03 << 0)
#define RCC_CFGR_SWS (0x03 << 2)
#define RCC_CFGR_HPRE (0x0F << 4)
#define RCC_CFGR_PPRE1 (0x07 << 10)
#define RCC_CFGR_PPRE2 (0x07 << 13)
#define RCC_CFGR_RTCPRE (0x1F << 16)

#define RCC_CIR (RCC_BASE + 0x0C)

#define RCC_AHB1RSTR (RCC_BASE + 0x10)
#define RCC_AHB1RSTR_GPIOARST (0x01 << 0)
#define RCC_AHB1RSTR_GPIOBRST (0x01 << 1)
#define RCC_AHB1RSTR_GPIOFRST (0x01 << 5)

#define RCC_AHB2RSTR (RCC_BASE + 0x14)
#define RCC_AHB3RSTR (RCC_BASE + 0x18)
#define RCC_APB1RSTR (RCC_BASE + 0x20)
#define RCC_APB2RSTR (RCC_BASE + 0x24)

#define RCC_AHB1ENR (RCC_BASE + 0x30)
#define RCC_AHB1ENR_GPIOAEN (0x01 << 0)
#define RCC_AHB1ENR_GPIOBEN (0x01 << 1)
#define RCC_AHB1ENR_GPIOCEN (0x01 << 2)
#define RCC_AHB1ENR_GPIODEN (0x01 << 3)
#define RCC_AHB1ENR_GPIOEEN (0x01 << 4)
#define RCC_AHB1ENR_GPIOFEN (0x01 << 5)
#define RCC_AHB1ENR_GPIOGEN (0x01 << 6)
#define RCC_AHB1ENR_GPIOHEN (0x01 << 7)
#define RCC_AHB1ENR_GPIOIEN (0x01 << 8)

#define RCC_AHB2ENR (RCC_BASE + 0x34)
#define RCC_AHB3ENR (RCC_BASE + 0x38)

#define RCC_APB1ENR (RCC_BASE + 0x40)
#define RCC_APB1ENR_PWREN (0x01 << 28)

#define RCC_APB2ENR (RCC_BASE + 0x44)
#define RCC_APB2ENR_USART1EN (0x01 << 4)
#define RCC_APB2ENR_SPI1EN (0x01 << 12)

#define GPIOA_BASE 0x40020000
#define GPIOA_MODER (GPIOA_BASE + 0x00)
#define GPIOA_OTYPER (GPIOA_BASE + 0x04)
#define GPIOA_OSPEEDR (GPIOA_BASE + 0x08)
#define GPIOA_PUPDR (GPIOA_BASE + 0x0C)
#define GPIOA_IDR (GPIOA_BASE + 0x10)
#define GPIOA_ODR (GPIOA_BASE + 0x14)
#define GPIOA_BSSR (GPIOA_BASE + 0x18)
#define GPIOA_LCKR (GPIOA_BASE + 0x1C)
						
#define GPIOA_AFRL (GPIOA_BASE + 0x20)
#define GPIOA_AFRH (GPIOA_BASE + 0x24)
#define GPIOA_AF_NONE (0x00U)
#define GPIOA_AF_SPI1 (0x05U)
#define GPIOA_AF_UART1 (0x07U)

#define GPIOB_BASE 0x40020400
#define GPIOB_MODER (GPIOB_BASE + 0x00)
#define GPIOB_OTYPER (GPIOB_BASE + 0x04)
#define GPIOB_OSPEEDR (GPIOB_BASE + 0x08)
#define GPIOB_PUPDR (GPIOB_BASE + 0x0C)
#define GPIOB_IDR (GPIOB_BASE + 0x10)
#define GPIOB_ODR (GPIOB_BASE + 0x14)
#define GPIOB_BSSR (GPIOB_BASE + 0x18)
#define GPIOB_LCKR (GPIOB_BASE + 0x1C)
						
#define GPIOB_AFRL (GPIOB_BASE + 0x20)
#define GPIOB_AFRH (GPIOB_BASE + 0x24)
#define GPIOB_AF_NONE (0x00)
#define GPIOB_AF_SPI1 (0x05)
#define GPIOB_AF_UART1 (0x07)
						
#define GPIOF_BASE 0x40021400
#define GPIOF_MODER (GPIOF_BASE + 0x00)
#define GPIOF_OTYPER (GPIOF_BASE + 0x04)
#define GPIOF_OSPEEDR (GPIOF_BASE + 0x08)
#define GPIOF_PUPDR (GPIOF_BASE + 0x0C)
#define GPIOF_IDR (GPIOF_BASE + 0x10)
#define GPIOF_ODR (GPIOF_BASE + 0x14)
#define GPIOF_BSSR (GPIOF_BASE + 0x18)
#define GPIOF_LCKR (GPIOF_BASE + 0x1C)

#define GPIOF_AFRL (GPIOF_BASE + 0x20)
#define GPIOF_AFRH (GPIOF_BASE + 0x24)
#define GPIOF_AF_NONE (0x00U)
#define GPIOF_AF_SPI1 (0x05U)
#define GPIOF_AF_UART1 (0x07U)

#define GPIOG_BASE 0x40021800
#define GPIOG_MODER (GPIOG_BASE + 0x00)
#define GPIOG_OTYPER (GPIOG_BASE + 0x04)
#define GPIOG_OSPEEDR (GPIOG_BASE + 0x08)
#define GPIOG_PUPDR (GPIOG_BASE + 0x0C)
#define GPIOG_IDR (GPIOG_BASE + 0x10)
#define GPIOG_ODR (GPIOG_BASE + 0x14)
#define GPIOG_BSSR (GPIOG_BASE + 0x18)
#define GPIOG_LCKR (GPIOG_BASE + 0x1C)
						
#define GPIOG_AFRL (GPIOG_BASE + 0x20)
#define GPIOG_AFRH (GPIOG_BASE + 0x24)
#define GPIOG_AF_NONE (0x00)
#define GPIOG_AF_SPI1 (0x05)
#define GPIOG_AF_UART1 (0x07)

#define USART1_BASE 0x40011000
#define USART1_SR (USART1_BASE + 0x00)
#define USART1_DR (USART1_BASE + 0x04)
#define USART1_BRR (USART1_BASE + 0x08)
#define USART1_CR1 (USART1_BASE + 0x0C)
#define USART1_CR2 (USART1_BASE + 0x10)
#define USART1_CR3 (USART1_BASE + 0x14)
#define USART1_GTPR (USART1_BASE + 0x18)

#define USART_SR_IDLE (0x01U << 4)
#define USART_SR_RXNE (0x01U << 5)
#define USART_SR_TC (0x01U << 6)
#define USART_SR_TXE (0x01U << 7)

#define USART_CR1_OVER8 (0x01U << 15)
#define USART_CR1_UE (0x01U << 13)
#define USART_CR1_M (0x01U << 12)
#define USART_CR1_WAKE (0x01U << 11)
#define USART_CR1_PCE (0x01U << 10)
#define USART_CR1_PS (0x01U << 9)
#define USART_CR1_TE (0x01U << 3)
#define USART_CR1_RE (0x01U << 2)
#define USART_CR2_STOP_1 (0x00U << 12)
#define USART_CR2_STOP_0_5 (0x01U << 12)
#define USART_CR2_STOP_2 (0x02U << 12)
#define USART_CR2_STOP_1_5 (0x03U << 12)

#define SPI1_BASE 0x40013000
#define SPI1_CR1 (SPI1_BASE + 0x00)
#define SPI1_CR2 (SPI1_BASE + 0x04)
#define SPI1_SR (SPI1_BASE + 0x08)
#define SPI1_DR (SPI1_BASE + 0x0C)

#define SPI_CR1_CPHA (0x01 << 0)
#define SPI_CR1_CPOL (0x01 << 1)
#define SPI_CR1_MSTR (0x01 << 2)
#define SPI_CR1_BR (0x03 << 3)
#define SPI_CR1_BR_128 (6 << 3)
#define SPI_CR1_SPE (0x01 << 6)
#define SPI_CR1_LSBFIRST (0x01 << 7)
#define SPI_CR1_SSI (0x01 << 8)
#define SPI_CR1_SSM (0x01 << 9)
#define SPI_CR1_RXONLY (0x01 << 10)
#define SPI_CR1_DFF (0x01 << 11)
#define SPI_CR1_CRCNEXT (0x01 << 12)
#define SPI_CR1_CRCEN (0x01 << 13)
#define SPI_CR1_BIDIOE (0x01 << 14)
#define SPI_CR1_BIDIMODE (0x01 << 15)
#define SPI_SR_BSY (0x01 << 7)
#define SPI_SR_TXE (0x01 << 1)
#define SPI_SR_RXNE (0x01 << 0)

#define PWR_BASE 0x40007000
#define PWR_CR (PWR_BASE + 0x00)
#define PWR_CSR (PWR_BASE + 0x04)
#define PWR_CR_VOS (0x01 << 14)
#define PWR_CSR_VOSRDY (0x01 << 14)

// 这里有坑，对于要==>【访问外设寄存器】<==读取值啥的，一定要加volatile，告诉CPU要去寄存器重新拿值，不要有缓存优化。
#define REG32_VALUE(__REG__) (*(volatile uint32_t *)(__REG__))
#define REG16_VALUE(__REG__) (*(volatile uint16_t *)(__REG__))
#define REG8_VALUE(__REG__) (*(volatile uint8_t *)(__REG__))
#endif


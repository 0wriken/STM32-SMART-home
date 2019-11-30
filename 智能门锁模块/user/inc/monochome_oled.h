#ifndef __MONOCHOME_OLED_H__
#define __MONOCHOME_OLED_H__

#include "stm32f10x.h"


/******************************************OLED屏接口定义开始******************************************/
#define OLED_RES_GPIOX        GPIOA
#define OLED_RES_GPIOX_PIN    12
#define OLED_RES_GPIOX_PORT   ((OLED_RES_GPIOX_PIN >= 8)?(OLED_RES_GPIOX_PIN - 8):(OLED_RES_GPIOX_PIN))
#define OLED_RES_RCC_GPIO     RCC->APB2ENR
#define OLED_RES_RCC_GPIOX    2	

#define OLED_CS_GPIOX        GPIOA
#define OLED_CS_GPIOX_PIN    15
#define OLED_CS_GPIOX_PORT   ((OLED_CS_GPIOX_PIN >= 8)?(OLED_CS_GPIOX_PIN - 8):(OLED_CS_GPIOX_PIN))
#define OLED_CS_RCC_GPIO     RCC->APB2ENR
#define OLED_CS_RCC_GPIOX    2	

#define OLED_DC_GPIOX        GPIOA
#define OLED_DC_GPIOX_PIN    11
#define OLED_DC_GPIOX_PORT   ((OLED_DC_GPIOX_PIN >= 8)?(OLED_DC_GPIOX_PIN - 8):(OLED_DC_GPIOX_PIN))
#define OLED_DC_RCC_GPIO     RCC->APB2ENR
#define OLED_DC_RCC_GPIOX    2	

#define OLED_CLK_GPIOX        GPIOB
#define OLED_CLK_GPIOX_PIN    13
#define OLED_CLK_GPIOX_PORT   ((OLED_CLK_GPIOX_PIN >= 8)?(OLED_CLK_GPIOX_PIN - 8):(OLED_CLK_GPIOX_PIN))
#define OLED_CLK_RCC_GPIO     RCC->APB2ENR
#define OLED_CLK_RCC_GPIOX    3	

#define OLED_DIN_GPIOX        GPIOB
#define OLED_DIN_GPIOX_PIN    15
#define OLED_DIN_GPIOX_PORT   ((OLED_DIN_GPIOX_PIN >= 8)?(OLED_DIN_GPIOX_PIN - 8):(OLED_DIN_GPIOX_PIN))
#define OLED_DIN_RCC_GPIO     RCC->APB2ENR
#define OLED_DIN_RCC_GPIOX    3	



#define OLED_GPIO_AFIO_EN
#ifdef OLED_GPIO_AFIO_EN
#define OLED_GPIO_AFIO    AFIO
#define OLED_GPIO_AFIO_BIT    24		//具体哪一个位
#define OLED_GPIO_AFIO_RCC    RCC->APB2ENR
#define OLED_GPIO_AFIO_RCCX   0
#endif

/******************************************OLED屏接口定义结束******************************************/


/******************************************SPI控制器定义开始******************************************/
#ifdef OLED_SPI_AFIO_EN
#define OLED_SPI_AFIO    AFIO
#define OLED_SPI_AFIO_BIT    X		//具体哪一个位    
#endif

#define OLED_RCC_SPIX	RCC->APB1ENR
#define OLED_RCC_SPIX_BIT	14
#define OLED_SPIX    SPI2

/* 
* SPIX通信速度设置
* FPCLK    72MHz
* 000    FPCLK/2 
* 001    FPCLK/4   
* 010    FPCLK/8  
* 011    FPCLK/16  
* 100    FPCLK/32  
* 101    FPCLK/64 
* 110    FPCLK/128
* 111    FPCLK/256
*/
#define OLED_SPIX_BAUD_RATE    0X1

#define OLED_SPIX_MODE_ZERO    0
#define OLED_SPIX_MODE_ONE     1
#define OLED_SPIX_MODE_TWO     2
#define OLED_SPIX_MODE_THREE   3
#define OLED_SPIX_MODE    OLED_SPIX_MODE_ZERO
/******************************************SPI控制器定义结束******************************************/


/****************************************OLED屏电平状态设置开始****************************************/
#define OLED_RES_HIGH()		{OLED_RES_GPIOX->ODR |=  (0X1u << OLED_RES_GPIOX_PIN);}
#define OLED_RES_LOW()		{OLED_RES_GPIOX->ODR &= ~(0X1u << OLED_RES_GPIOX_PIN);}

#define OLED_CS_HIGH()		{OLED_CS_GPIOX->ODR |=  (0X1u << OLED_CS_GPIOX_PIN);}
#define OLED_CS_LOW()		{OLED_CS_GPIOX->ODR &= ~(0X1u << OLED_CS_GPIOX_PIN);}

#define OLED_DC_HIGH()		{OLED_DC_GPIOX->ODR |=  (0X1u << OLED_DC_GPIOX_PIN);}
#define OLED_DC_LOW()		{OLED_DC_GPIOX->ODR &= ~(0X1u << OLED_DC_GPIOX_PIN);}
/****************************************OLED屏电平状态设置结束****************************************/

void oled_init(void);
void oled_display_clear(void);
void oled_display_image(uint8_t display_page, uint8_t display_column, const uint8_t *display_memory);
void oled_display_arbitrary_string(uint8_t display_page, uint8_t display_column, uint8_t *display_string);

#endif

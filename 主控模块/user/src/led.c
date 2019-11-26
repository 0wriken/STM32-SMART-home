#include "led.h"

/*
**************************************************************************
* 函数功能: LED初始化
* 函数形参: None
* 函数返回值: None
* 备注: None   PF6
 **************************************************************************
*/
void led_init(void)
{
	RCC->AHB1ENR |= 0X1 << 2;
	
	GPIOC->MODER &= ~(0X3 << 0);
	GPIOC->MODER |= 0X1 << 0;
	GPIOC->OTYPER &= ~(0X1 << 0);
	GPIOC->OSPEEDR &= ~(0X3 << 0);
	GPIOC->PUPDR &= ~(0X3 << 0);
	
	GPIOC->ODR |= 0X0 << 0;
	
	RCC->AHB1ENR |= 0X1 << 5;
	
	GPIOF->MODER &= ~(0X3 << 2*6);
	GPIOF->MODER |= 0X1 << 2*6;
	GPIOF->OTYPER &= ~(0X1 << 6);
	GPIOF->OSPEEDR &= ~(0X3 << 2*6);
	GPIOF->PUPDR &= ~(0X3 << 2*6);
	
	GPIOF->ODR |= 0X0 << 6;
}
//PE2 3 4
void key_init(void)
{
	RCC->AHB1ENR |= 0X1 << 4;
	GPIOE->MODER&=~(0X3<<2*2);
	GPIOE->PUPDR &=~(0x3<<2*2);
	
	GPIOE->MODER&=~(0X3<<2*3);
	GPIOE->PUPDR &=~(0x3<<2*3);
	
	GPIOE->MODER&=~(0X3<<2*4);
	GPIOE->PUPDR &=~(0x3<<2*4);
	
	
	
}
u8 key_status(void)
{
	u8 key_res = KEY_ERROR_RES;
	static u8 key_static = 0;
	if(KEY_ONE_SCAN==1 && key_static==0)
	{
		if(KEY_ONE_SCAN==1)
		{
			key_res = KEY_ONE_RES;
			key_static = 1;
		}
	}
	else if(KEY_ONE_SCAN == 0)
	{
		key_static = 0;
	}
	
	if(!KEY_TWO_SCAN==1 && key_static==0)
	{
		if(!KEY_TWO_SCAN)
		{
			key_res = KEY_TWO_RES;
			key_static = 1;
		}
		
	}
	else if(!KEY_TWO_SCAN == 0)
	{
		key_static = 0;
	}
	
	if(!KEY_THREE_SCAN==1 && key_static==0)
	{
		if(!KEY_THREE_SCAN)
		{
			key_res = KEY_THREE_RES;
			key_static = 1;
		}
	}
	else if(!KEY_THREE_SCAN == 0)
	{
		key_static = 0;
	}
	
	if(!KEY_FOUR_SCAN==1 && key_static==0)
		{
		if(!KEY_FOUR_SCAN)
		{
			key_res = KEY_FOUR_RES;
			key_static = 1;
		}
		
	}
	else if(!KEY_FOUR_SCAN == 0)
	{
		key_static = 0;
	}
	
	return key_res;
}



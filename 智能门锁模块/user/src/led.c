#include "led.h"
/*
***********************************************************************************************
* 函数功能:LED灯初始化
* 函数形参: None
* 函数返回值: None
* 备注: None
* 作者: wriken
* 时间: 2019-08-21
* 修改作者: None
* 修改时间: None
***********/
void led_init(void)
{
	RCC->APB2ENR|=(0X1<<3);
	
	GPIOB->CRL=0;
	
	GPIOB->CRL|=(0X2<<24);
	
	GPIOB->CRH=0;
	
	GPIOB->CRH|=(0X2<<0); //PB8  PB6 推娩输出
	
	GPIOB->ODR|=(0x5<<6);//初始为灭
	
	
}

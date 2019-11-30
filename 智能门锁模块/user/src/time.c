#include "time.h"
/*
***********************************************************************************************
* 函数功能:定时器初始化
* 函数形参: 延时时间毫秒
* 函数返回值: None
* 备注: None
* 作者: wriken
* 时间: 2019-08-21
* 修改作者: None
* 修改时间: None
***********/
void time_init(u32 nms)
{	
	RCC->APB1ENR|=(0x1<<2);//PB6  
	
	TIM4->SMCR&=~(0X7<<0);		//清除
	TIM4->SMCR&=~(0X1<<14);//时钟源选择系统时钟源，禁止外部时钟模式 2
	
	TIM4->PSC=7200-1;//分频器10KHZ
	TIM4->ARR=nms-1;//定时时间为0.n毫秒
	TIM4->CNT=0;    //清除计数数值
	TIM4->EGR|=(0X1<<0);  //打开计数器使能
	TIM4->SR&=~(0X1<<0);	//清除标志位
	
	
	TIM4->CR1=0;					//清除

	TIM4->CR1|=(0X1<<0);	//打开定时使能
	TIM4->CR1&=~(0x1<<1); //配置1次UG位
	TIM4->CR1|=(0X1<<3);  //定时时间到不会停止、
	while(!(TIM4->SR&(0x1<<0)))
	{	
			;
	}
		TIM4->CR1 &= ~(0X1 << 0);			//开启计数器
}




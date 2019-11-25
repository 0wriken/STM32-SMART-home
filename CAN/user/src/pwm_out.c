#include "pwm_out.h"
//pa6 PUT
u32 a=10;
u32 flag1=0;
void pwm_io_init(void)
{
	RCC->AHB1ENR|=(0X1<<0);
	GPIOA->MODER|=(0X2<<2*6);
	GPIOA->OTYPER &= ~(0X1 << 6);
	GPIOA->OSPEEDR |= (0X1<< 2*6);
	GPIOA->PUPDR &= ~(0X3 << 2*6);
	GPIOA->AFR[0]|=(0X2<<4*6);
	
}

void pwm_out_init(void)
{
	RCC->APB1ENR|=(0X1<<1);
	TIM3->SMCR=0;
	TIM3->PSC|=83;
	TIM3->ARR=1000-1;
	TIM3->CNT=0;
	TIM3->EGR|=(0x1<<3);
	TIM3->SR&=~(0X1<<0);
	TIM3->CR1=0;
	TIM3->CR1&=~(0X1<<1);
	TIM3->DIER|=(0X1<<0);
	TIM3->CR1|=(0X1<<0);
	NVIC_SetPriorityGrouping(7-2);//抢占位
	NVIC_EncodePriority(7-2,0, 0);//抢占与响应优先级
	NVIC_SetPriority(TIM3_IRQn, NVIC_EncodePriority(7-2,0, 0));//中断类型
	NVIC_EnableIRQ(TIM3_IRQn);	 //打开该类型中断使能	
	
}
void TIM3_IRQHandler ()
{
	if(TIM3->SR&(0X1<<0)) //判断定时中断标志位
	{
		if(a<1000&&flag1==0)   //亮度最亮到最暗
		{
			TIM3->SR&=~(0X1<<0); //定时标志位清0
			TIM3->CCR1=a-1;//占空比	
			a++;
			flag1=0;
			if(a==1000)
				flag1=1;
		}
		if(a<1001&&flag1==1)   //亮度由最暗到最亮
		{
			TIM3->SR&=~(0X1<<0);
			TIM3->CCR1=a-1;//占空比
			a--;
			flag1=1;
			if(a==0)
				flag1=0;  //亮灭标志位
		}
		TIM3->CCMR1&=~(0Xff<<0);
		TIM3->CCMR1|=(0X6<<4);
		TIM3->CCER&=~(0XF<<0);
		TIM3->CCER|=(0x1<<0);
		
	}
}







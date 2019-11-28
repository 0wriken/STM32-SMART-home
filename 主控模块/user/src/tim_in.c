#include "tim_in.h"
//PA8 TIM1_CH1
u32 data;
u8 rec_ok;
u8 flagg;
void tim_in_init(void)
{
	RCC->AHB1ENR|=(0X1<<0);
	GPIOA->MODER&=~(0x3<<2*8);
	GPIOA->MODER|=(0X2<<2*8);
	GPIOA->PUPDR&=~(0X3<<2*8);
	GPIOA->AFR[1]&=~(0XF<<2*0);
	GPIOA->AFR[1]|=(0X1<<2*0);//复用为TIM1
	
	
	RCC->APB2ENR|=(0X1<<16);
	TIM9->CCMR1|=(0x1<<0);  //输入TI1  1C1F0011
	TIM9->CCMR1&=~(0X3<<2);//每次输入上升捕获
	TIM9->CCMR1|=(0xF<<4);          //70K滤波
	
	TIM9->CR1=0;
	TIM9->CR1|=(0X1<<8);
	TIM9->CR1|=(0X1<<2);    //84/
//	TIM9->SMCR|=(0x5<<4);//TS 101  从100
	TIM9->SMCR&=~(0X7<<0);//
	
	TIM1->PSC|=840-1;//840分频 10us 
	TIM1->ARR|=12000-1;//12ms计数
	TIM1->CNT=0;
	TIM1->SR&=~(0x1<<0);//清除标志位
	TIM1->EGR|=(0x1<<0);//更新
	TIM1->DIER|=(0x1<<1);//产生中断
	TIM1->DIER|=(0X1<<0);//更新中断
	NVIC_SetPriorityGrouping(7-2);//抢占位
	NVIC_SetPriority(TIM1_CC_IRQn, NVIC_EncodePriority(7-2,0, 0));//中断类型
	NVIC_EnableIRQ(TIM1_CC_IRQn);
	TIM9->CCER&=~(0X3<<1);//上升沿触发
	TIM9->CCER|=(0x1<<0);     //使能捕获
	TIM9->CR1|=(0X1<<0);
	
}
//TIM_SR  1位上升  CC1F
void   TIM1_CC_IRQHandler()
{
		if((TIM1->SR&(0X1<<1)))
		{
			TIM1->SR&=~(0X1<<0);
			while(TIM1->SR&(0X1<<1));
			TIM1->CNT=0;
			if(TIM1->CCR1>1300&&TIM1->CCR1<1400)
			{
				flagg=1;
			}
			else if(flagg==1)
			{
			if(TIM9->CCR1>220&&TIM9->CCR1<230)
			{
				data>>=1;
				data|=0x80000000;
			}
			else if(TIM1->CCR1>105&&TIM1->CCR1<120)
			data>>=1;
			}
		}
		if((TIM1->SR&(0X1<<9)))
		{
			TIM1->SR&=~(0X1<<9);
			while(TIM1->SR&(0X1<<9));
			rec_ok=1;
		}
}
u8 Get_key(void)
{
	u8 add_z=0;
	u8 add_f=0;
	u8 key_z=0;
	u8 key_f=0;
	if(rec_ok==1)
	{
		rec_ok=0;
		add_z=(data&(0x000000ff));
		add_f=(data&(0x0000ff00))>>8;
		key_z=(data*(0x00ff0000))>>16;
		key_f=(data&(0xff000000))>>24;
	}
	if(key_z==~(key_f))
		return key_z;
	else 
		return 0xff;
}

#include "stm32f4xx.h"
#include "delay.h"
void Delay_init(void)
{
    RCC->APB2ENR|=(0X1<<0);//打开时钟使能
    TIM1->CR1|=(0X1<<7);
    TIM1->CR1|=(0X1<<3);
    TIM1->CR1|=(0X1<<4);
    TIM1->CR1|=(0X1<<1);//禁止更新源

}
void Delay_us(int nus)
{
    TIM1->ARR=84-1;//如何查看时间频率
    TIM1->PSC=2*nus-1;
    TIM1->EGR&=~(0X1<<0);
    TIM1->SR&=~(0X1<<0);
    TIM1->CR1|=(0X1<<0);
    while(!(TIM1->SR&(0X1<<0)));
}   
void Delay_ms(int nms)
{
    TIM1->ARR=84*1000-1;
    TIM1->PSC=2*nms-1;
    TIM1->EGR&=~(0X1<<0);
    TIM1->SR&=~(0X1<<0);
    TIM1->CR1|=(0X1<<0);
    while(!(TIM1->SR&(0X1<<0)));
}

void delay_mms(int ms)
{   

   u16 i=0;  
   while(ms--)
   {
      i=12000;  //自己定义
      while(i--) ;    
   }

}
void delay_ms(int ms)
{
	
   u16 i=0;  
   while(ms--)
   {
      i=12000;  //自己定义
      while(i--) ;    
   }
}
void delay_us(int time)
{    
   u32 i=0;  
   while(time--)
   {
      i=12;  //自己定义
      while(i--) ;    
   }

 }



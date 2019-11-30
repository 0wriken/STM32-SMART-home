#include "stm32f10x.h"
#include "delay.h"

void DelayInit(void)
{
	RCC->APB2ENR |=0x01<<11;//定时器1时钟使能---72Mhz
	
	TIM1->CR1 &=~(0x01<<0);//关闭计数器
	TIM1->CR1 |=0x01<<7;//TIMx_ARR 寄存器进行缓冲
	TIM1->CR1 |=0x01<<3;//单次模式
	TIM1->CR1 &=~(0x03<<1);//更新源选择
}



////延时nus微秒
////参数 nus :延时多少微秒
////返回值：0 正常  1异常(提前结束了延时)
u8 Delayus(u32 nus)
{
	TIM1->ARR=36-1;
	TIM1->PSC=2*nus-1;
	TIM1->EGR |=0x01<<0;//（软件生产更新事件---让PSC和ARR值加载到其影子寄存器中，计数器值清零）
	TIM1->SR &=~(0x01<<0);//6.清除更新标志
	TIM1->CR1 |=0x01<<0;//开启计数
	while(!(TIM1->SR&(0x01<<0)));//等待时间到
}


////延时ms微秒
////参数 ms :延时多少微秒
////返回值：0 正常  1异常(提前结束了延时)
u8 Delayms(u16 nms)
{
	TIM1->ARR=36*1000-1;
	TIM1->PSC=2*nms-1;
	TIM1->EGR |=0x01<<0;//（软件生产更新事件---让PSC和ARR值加载到其影子寄存器中，计数器值清零）
	TIM1->SR &=~(0x01<<0);//6.清除更新标志
	TIM1->CR1 |=0x01<<0;//开启计数
	while(!(TIM1->SR&(0x01<<0)));//等待时间到
}


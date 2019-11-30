#include "stm32f10x.h"
#include "uart.h"
#include "delay.h"
#include "stdio.h"
#include "iic.h"
#include "at24c02.h"
#include "MPR121.h"
#include "spi.h"
#include "oled.h"
#include "ucos_ii.h"
#include "task.h"
#include "led.h"
#include "monochome_oled.h"
#include "oled_memory.h"
#include "MG200.h"
extern u8 buf[100];


//函数功能：系统滴答中断-ms
//参数说明：nms延时时长
//返回值：无
//注意事项:nms最大值798.915
//时间：2018/11/20 
void Systick_Interrupt(u16 nms)
{
	SysTick->CTRL &=~(0x01<<2);//选择时钟源（9M）
	SysTick->LOAD=9*1000*nms;//设置重装载值（LOAD）
	SysTick->VAL=0;//写VAL（清零VAL，COUNTFLAG清零，加载LOAD值到VAL）
	SysTick->CTRL |=0x01<<1;//开滴答中断-模块级
		//配置NVIC优先级
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority (7-2, 2, 2));
	//系统级中断使能--内核中断不需要NVIC使能
	//NVIC_EnableIRQ(SysTick_IRQn);
	
	SysTick->CTRL |=0x01<<0;//开启定时器
}

void SysTick_Handler(void)
{
	OSIntEnter();
	
	OSTimeTick( );///操作系统调度的服务函数
	
	OSIntExit();
}



OS_EVENT *OS_MBOX;
OS_EVENT *OSQ;

int main(void)
{
	INT8U ret;
	NVIC_SetPriorityGrouping(7-2);
	
	//硬件初始化
	DelayInit( );//定时器1延时功能初始化
	Usart1Init(72,115200);
	led_init();
	IICPinInit( );
	Usart2Init(36,115200);
	MPR121Init();
	MG200Init();
	SPIPinInit( );
//	OLEDInit( );
	GPIO_voice_init();
	GPIO_key_init();
	oled_init();
	
	
	Systick_Interrupt(1000/OS_TICKS_PER_SEC);//5ms产生系统调度
	
	OSInit();//初始化 UCOS-II 内核相关函数。
	

	//至少要创建一个任务
	ret=OSTaskCreate(StartTask,NULL,&StratStack[StratStackLen-1],StartTaskPrio);
//	if(ret!=OS_ERR_NONE)
//		printf("OSTaskCreate err!!\r\n");
//	
//	printf("OS Start!!!!!\r\n");
 	OSStart( );//启动 uC/OS-II 的多任务环境。
	
	
	//启动了操作系统后，下面的代码就不会被执行
	while(1)
	{
		printf("||||||||||||||||||||||||||\r\n");
		
	}
}

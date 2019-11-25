#include "stm32f4xx.h"
#include "os.h"
#include "start_task.h"
#include "led.h"
#include "uart.h"
#include "tim_in.h"
#include "delay.h"
#include "ili9486.h"
#include "pic.h"
#include "touch.h"
/*
**************************************************************************
* 函数功能: 系统滴答定时器初始化
* 函数形参: nms
* 函数返回值: None
* 备注: None
**************************************************************************
*/
void systick_init(uint32_t nms)
{
	SysTick->CTRL = 0;	//清空配置
	SysTick->VAL = 0;	//清空计数器
	SysTick->LOAD = 21000 * nms;	//设置nms的计数值
	SysTick->CTRL |= 0X1 << 1;		//开启中断使能
	SysTick->CTRL |= 0X1 << 0;		//开启系统滴答定时器
}


void SysTick_Handler(void)
{
	OSIntEnter();	//告诉操作系统进中断
	
	OSTimeTick();	//调用心跳节拍函数
	
	OSIntExit();	//告诉操作系统出中断
}


/*
**************************************************************************
* 函数功能: 主函数
* 函数形参: None
* 函数返回值: None
* 备注: None
**************************************************************************
*/
int main(void)
{
	led_init();		//初始化LED灯接口
	key_init();
	uart1_init(84000000, 9600);
		printf("coming statr\n");

	LCD_Port_Init();
	LCD_Init();
	Delay_init();
	
	//display_line(300,20,30,0,RED);
	
	init_touch_gpio();
	systick_init(1000/OS_TICKS_PER_SEC);	//设置心跳节拍
	
	tim_in_init();
	OSInit();	//初始化操作系统

	int k=0;
//	LCD_Clear(0,319,0,479,0xfff);
	while(1)
	{
		
		LCD_display_pic(0,150,(u8*)gImage_11);
		play_Square(40,100,RED);
		while ( t_para.sta.loosen == 0 )//等待松手标志置1
		{
			delay_ms(10);//等待按下触摸屏
		}
		t_para.sta.loosen = 0;
		if(t_para.sx <120&&t_para.sx >40&&t_para.sy>100&&t_para.sx < 150)
			printf("调试成功!!!\n");
	}
	/* 创建一个起始任务 */
	OSTaskCreateExt(start_task,
					0u,
					&start_task_stk[START_TASK_STK_SIZE - 1u],
					START_TASK_PRIO,
					START_TASK_PRIO,
					&start_task_stk[0u],
					START_TASK_STK_SIZE,
					0u,
					(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
	
	OSStart();	//启动操作心跳
	
	/* 防止操作系统启动失败，程序跑飞 */
	while(1)
	{
		
	}
}

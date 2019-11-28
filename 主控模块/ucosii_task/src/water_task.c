#include "start_task.h"
#include "led.h"
#include "uart.h"
#include "string.h"
#include "rain.h"
u8 error4;
u8 re1_buf[100];
u32 g1id;
u8 set_task_point(int x,int y,u8 *buf);
void delay_mms(int ms);

//是否有雨水
void water_task(void *arg)
{
						OSFlagPend (flag, 
												1<<2,
												OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,
												0,
											 &error4);
	LCD_Clear(0,319,0,479,0xfff);
	while(1)
	{
		if(can1_receive_msg(re1_buf,&g1id))
		{
			LCD_Clear(80,200,240,430,0xff);//清空
			if(g1id==0xAD)
			{
				if(strcpy((char *)re1_buf,"rain"))
				play_string(80,200, RED,WHITE,rain_buf,5);
				if(strcpy((char *)re1_buf,"no_rain"))
				play_string(80,200, RED,WHITE,norain_buf,8);
			}
		}	
		delay_mms(1000);//每一秒检测一次
		if(set_task_point(200,400,(u8 *)"quit")==1)
		{
					OSTaskSuspend(9);//挂起自己回到主界面
		}
}
}

#include "start_task.h"
#include "led.h"
#include "uart.h"
#include "string.h"
#include "sun.h"
u8 error3;
u8 re2_buf[100];
u32 g2id;
u8 set_task_point(int x,int y,u8 *buf);
void delay_mms(int ms);
void sun_task(void *arg)
{
						OSFlagPend (flag, 
												1<<3,
												OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,
												0,
											 &error3);
	LCD_Clear(0,319,0,479,0xfff);
	LCD_display_pic(200,250,(u8 *)gImage2_back);
	while(1)
	{
		if(can1_receive_msg(re2_buf,&g2id))
		{
			LCD_Clear(80,200,240,430,0xff);//清空
			if(g2id==0xAC)
			{
				if(strcpy((char *)re2_buf,"light"))
				play_string(80,200, RED,WHITE,li_buf,6);
				if(strcpy((char *)re2_buf,"dark"))
				play_string(80,200, RED,WHITE,d_buf,4);
			}
		}	
		delay_mms(1000);//每一秒检测一次
		if(set_task_point(200,400,(u8 *)"quit")==1)
		{
					OSTaskSuspend(10);//挂起自己回到主界面
		}
}
}

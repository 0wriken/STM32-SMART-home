#include "start_task.h"
#include "led.h"
#include "uart.h"
#include "delay.h"
#include "string.h"
//#include "pic.h"
#include "play.h"
u8 error1;
u16 play_flag=0;
u8 play_buf[256];
u8 re_buf[100];
u32 gid;
u8 set_task_point(int x,int y,u8 *buf);
void play_task(void *arg)
{
				OSFlagPend (flag, 
												1<<1,
												OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,
												0,
											 &error1);
	LCD_Clear(0,319,0,479,0xfff);
	//等待接收的标志位显示数据
	LCD_display_pic(200,250,(u8 *)gImage1_back);
	while(1)
	{
		if(can1_receive_msg(re_buf,&gid))
		{
			LCD_Clear(80,200,240,430,0xff);//清空
			if(gid==0xAF)
			{
			if(strcpy((char *)re_buf,"vibra"))
			{
				//play_string(80,200, RED,WHITE,v_buf,5);
				LCD_Clear(0,319,0,479,0xfff);//清屏
				LCD_display_pic(200,250,(u8 *)gImage1_back);
				LCD_display_pic(200,250,(u8 *)gImage_zhen);
			}
				
			if(strcpy((char *)re_buf,"no_vibra"))
			{
			LCD_Clear(0,319,0,479,0xfff);//清屏
			LCD_display_pic(200,250,(u8 *)gImage1_back);
			LCD_display_pic(200,250,(u8 *)gImage_sea);
			//play_string(80,200, RED,WHITE,v2_buf,8);
			}
		}	
		delay_mms(2000);//每一秒检测一次
		if(set_task_point(200,250,(u8 *)"quit")==1)
		{
					OSTaskSuspend(7);//挂起自己回到主界面
		}
	}
}
}


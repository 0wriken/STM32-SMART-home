#include "start_task.h"
#include "led.h"
#include "uart.h"
//#include "pic.h"
u8 error;
u32 getid;
u8 rec_buf[100]={0};
u8 set_task_point(u16 x,u16 y,u8 *buf)
{
		play_Square(x,y,RED);
	if(x>240||y>430)
	{
		printf("设置位置有误\n");
		return 0;		
	}
	if(t_para.sx>x&&t_para.sx<(x+80)&&t_para.sy<(y+50)&&t_para.sy>y)
		{
			printf("准确按下\n");
			return 1;
		}
		else return 0;
}
void lcd_task(void *arg)
{
//		LCD_display_pic(0,150,(u8*)gImage_11);
//		play_Square(40,100,RED);//ok位置
//		

//	while(1)
//	{

//	  can1_send_msg(0x45,(u8 *)"66666",8);
//		can1_receive_msg(rec_buf,&getid);
//		printf("rec:%s  getid:%d\n",rec_buf,getid);
//		can1_send_msg((u32)0XAE,(u8 *)"LED_ON",8);
//		printf("send_ok");
//		delay_mws(2000);
//			//测试CAN
//	}
	//开机动画
	
	
	//OK键按下
	printf("coming\n");
	//灯光控制
//	OSTaskResume(7);
//	OSTaskResume(8);
//	OSTaskResume(9);
//	OSTaskResume(10);  
	if(set_task_point(40,100,(u8 *)NULL)==1)
	{
		OSFlagPost (flag, 
								1<<0,
								OS_FLAG_SET,
							 &error);
	}
	printf("灯光开启\n");
		if(set_task_point(80,200,(u8 *)"检测震动")==1)
	{
		OSFlagPost (flag, 
								1<<1,
								OS_FLAG_SET,
							 &error);
	}
		printf("震动开启\n");
		if(set_task_point(40,100,(u8 *)"雨水")==1)
	{
		OSFlagPost (flag, 
								1<<2,
								OS_FLAG_SET,
							 &error);
	}
		printf("光敏开启\n");
		if(set_task_point(40,100,(u8 *)"光敏")==1)
	{
		OSFlagPost (flag, 
								1<<3,
								OS_FLAG_SET,
							 &error);
	}
	
	
	
}

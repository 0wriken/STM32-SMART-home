#include "start_task.h"
#include "led.h"
#include "stdlib.h"
#include "uart.h"
#include "check.h"
u8 error2;
u8 rx_buf[100];
u32 id;
u8 send_buf[100];
dis display={
	{0xaf},{100},{200}
};
extern u8 flag1;
u8 set_task_point(int x,int y,u8 *buf);
void send_wait(u32 id,u8 * buf,u8 len)
{
	 u8 re_buf[100]={0};
	 u32 gid=0;
		while(1)
		{
			can1_send_msg(id,buf,len);
			printf("%s",buf);
			can1_receive_msg(re_buf,&gid);
			if(gid==0xAE)
			{
			printf("应答：%s\n",re_buf);
			if(strcmp((char *)re_buf,"OK")==0)
			{	
				printf("接收到应答\n");
				break;
			}
			}
			//OSTimeDly(200);
		}
}	
void check_task(void *arg)
{
			OSFlagPend (flag, 
												1<<0,
												OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,
												0,
											 &error2);
	LCD_Clear(0,319,0,479,0xfff);//清屏
	printf("进入灯光控制\n");
	///显示界面
	LCD_display_pic(200,400,(u8 *)gImage3_back);
	LCD_display_pic(40,100,(u8 *)gImage_jieni);
	LCD_display_pic(40,200,(u8 *)gImage_red);
	LCD_display_pic(40,300,(u8 *)gImage_green);
	LCD_display_pic(150,300,(u8 *)gImage_close);
	LCD_display_pic(150,100,(u8 *)gImage_close);
	LCD_display_pic(150,200,(u8 *)gImage_close);
	
	while(1)
	{
		int i=0;
		//等待进入显示界面

		//t_para.sx=0;
		//t_para.sy=0;
		//等待接收CAN的值
//		can1_receive_msg(rx_buf,&id);
//		for(i=0;i<9;i++)
//		{
//			if(display.check_id[i]==id)
//				break;
//		}
		//play_string((u16)display.x[i],(u16)display.y[i],
		//WHITE,RED,rx_buf,10);	
		
		//检测触摸屏是否按下发送信号至CAN
		t_para.sx=0;
		t_para.sy=0;
		if(set_task_point(40,100,(u8 *)"LED1_ON")==1)
		{
			send_wait((u32 )0XAE,(u8 *)"LED1_ON",8);
			//OSTaskResume(11);
		}
		if(set_task_point(150,100,(u8 *)"LED1_OFF")==1)
		{
					send_wait((u32 )0XAE,(u8 *)"LED1_OF",8);
		}
		
		if(set_task_point(40,200,(u8 *)"LED2_ON")==1)
		{
				send_wait((u32 )0XAE,(u8 *)"LED2_ON",8);
		}
		
		if(set_task_point(150,200,(u8 *)"LED2_OFF")==1)
		{
		send_wait((u32 )0XAE,(u8 *)"LED2_OF",8);
		}
	
		if(set_task_point(40,300,(u8 *)"LED3_ON")==1)
		{
			send_wait((u32 )0XAE,(u8 *)"LED3_ON",8);
		}
		
		if(set_task_point(150,300,(u8 *)"LED3_OFF")==1)
		{
			send_wait((u32 )0XAE,(u8 *)"LED3_OF",8);
		}
		if(set_task_point(200,400,(u8 *)"quit")==1)
		{	
			flag1=1;
					OSTaskSuspend(8);//挂起自己回到主界面
		
		}

}
}


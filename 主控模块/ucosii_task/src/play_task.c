#include "start_task.h"
#include "led.h"
#include "uart.h"
#include "string.h"
#include "pic.h"
u8 error1;
u16 play_flag=0;
u8 play_buf[256];
u8 re_buf[100];
u32 gid;
u8 set_task_point(int x,int y,u8 *buf);
void delay_mms(int ms)
{   
   u32 i=0;  
   while(ms--)
   {
      i=12000;  //�Լ�����
      while(i--) ;    
   }

}
void play_task(void *arg)
{
				OSFlagPend (flag, 
												1<<1,
												OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,
												0,
											 &error1);
	LCD_Clear(0,319,0,479,0xfff);
	//�ȴ����յı�־λ��ʾ����
	while(1)
	{
		if(can1_receive_msg(re_buf,&gid))
		{
			LCD_Clear(80,200,240,430,0xff);//���
			if(gid==0xAF)
			{
				if(strcpy((char *)re_buf,"vibra"))
				play_string(80,200, RED,WHITE,v_buf,5);
				if(strcpy((char *)re_buf,"no_vibra"))
				play_string(80,200, RED,WHITE,v2_buf,8);
			}
		}	
		delay_mms(1000);//ÿһ����һ��
		if(set_task_point(200,400,(u8 *)"quit")==1)
		{
					OSTaskSuspend(7);//�����Լ��ص�������
		}
	}
}


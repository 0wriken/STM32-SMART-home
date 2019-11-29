#include "start_task.h"
#include "led.h"
#include "uart.h"
#include "pic.h"
//unsigned char gImage_11[115208];
u8 error;
u32 getid;
u8 rec_buf[100]={0};
extern u8 flag1;
u8 set_task_point(u16 x,u16 y,u8 *buf)
{
		//play_Square(x,y,RED);
	if(x>240||y>430)
	{
		printf("����λ������\n");
		return 0;		
	}
	if(t_para.sx>x&&t_para.sx<(x+120)&&t_para.sy<(y+100)&&t_para.sy>y)
		{
			printf("׼ȷ����\n");
			return 1;
		}
		else return 0;
}
void lcd_task(void *arg)
{

	//��������
	
	
	//OK������
	printf("coming\n");
	LCD_Clear(0,319,0,479,0xfff);//����
	OSTaskResume(7);
	OSTaskResume(8);
	OSTaskResume(9);
	OSTaskResume(10);
LCD_display_pic(40,100,(u8 *)gImage_rgb); 
LCD_display_pic(200,100,(u8 *)gImage_light);
LCD_display_pic(40,250,(u8 *)gImage_rain);
LCD_display_pic(200,250,(u8 *)gImage_zhendong);
//LCD_display_pic(0,0,(u8 *)gImage_desk);
	while(1)
	{
		if(flag1==1)
		{
		LCD_Clear(0,319,0,479,0xfff);//����
	LCD_display_pic(40,100,(u8 *)gImage_rgb); 
LCD_display_pic(200,100,(u8 *)gImage_light);
LCD_display_pic(40,250,(u8 *)gImage_rain);
LCD_display_pic(200,250,(u8 *)gImage_zhendong);
			flag1=0;
		}
	if(set_task_point(40,100,(u8 *)NULL)==1)
	{
		OSFlagPost (flag, 
								1<<0,
								OS_FLAG_SET,
							 &error);
	}
	//printf("�ƹ⿪��\n");
		if(set_task_point(200,250,(u8 *)"�����")==1)
	{
		OSFlagPost (flag, 
								1<<1,
								OS_FLAG_SET,
							 &error);
	}
		//printf("�𶯿���\n");
		if(set_task_point(40,250,(u8 *)"��ˮ")==1)
	{
		OSFlagPost (flag, 
								1<<2,
								OS_FLAG_SET,
							 &error);
	}
		//printf("��������\n");
		if(set_task_point(200,100,(u8 *)"����")==1)
	{
		OSFlagPost (flag, 
								1<<3,
								OS_FLAG_SET,
							 &error);
	}
	
	
}
}

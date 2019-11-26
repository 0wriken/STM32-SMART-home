#include "start_task.h"
#include "led.h"
#include "uart.h"
#include "pic.h"
u8 error;
u32 getid;
u8 rec_buf[100]={0};
void lcd_task(void *arg)
{
		LCD_display_pic(0,150,(u8*)gImage_11);
		play_Square(40,100,RED);//okÎ»ÖÃ
		
	while(1)
	{
		 can1_send_msg(0x45,(u8 *)"66666",8);
		can1_receive_msg(rec_buf,&getid);
		printf("rec:%s  getid:%d\n",rec_buf,getid);
		delay_ms(2000);
	}
	//OK¼ü°´ÏÂ
		OSFlagPost (flag, 
								1<<0,
								OS_FLAG_SET,
							 &error);
	
	
	
	
	
}
#include "start_task.h"
#include "led.h"
#include "uart.h"
u8 error2;
u8 rx_buf[100];
u32 id;
dis display;
void check_task(void *arg)
{
	
	while(1)
	{
		int i=0;
		//等待进入显示界面
		OSFlagPend (flag, 
												1<<0,
												OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,
												0,
											 &error2);
		
		//等待接收CAN的值
		can1_receive_msg(rx_buf,&id);
		for(i=0;i<9;i++)
		{
			if(display.check_id[i]==id)
				break;
		}
		play_string((u16)display.x[i],(u16)display.y[i],
		WHITE,RED,rx_buf);	
		
		//检测触摸屏是否按下发送信号至CAN
	
}
}


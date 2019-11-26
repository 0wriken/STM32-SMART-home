#include "start_task.h"
#include "led.h"
#include "uart.h"
u8 error1;
u16 play_flag=0;
u8 play_buf[100];
void play_task(void *arg)
{
	//等待接收的标志位显示数据
		strcpy((char *)play_buf,(char*)OSMboxPend (QSM,
										0,
                   &error1));
		printf("play is%s\n",(char *)play_buf);
		
}


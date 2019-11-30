#include "task.h"
#include "ucos_ii.h"
#include "uart.h"
#include "stdio.h"
#include "delay.h"
extern u8 *buff;
extern u32 data;
char *message[5]={"led1_open","led2_open","led1_close","led2_close"};
OS_STK UartStack[UartStackLen];

void UartTask(void *pd)
{

	printf("Uart coming!!\r\n");
	
	while(1)
	{
		if(USART1->SR & (0X01<<5)) 
		{
			data=USART1->DR;
			
			switch(data)
			{
				case '1'://OSMboxPost (OSMbox,&message1);break;
					OSQPost(OSQ,(u8 *)message[0]);break;
				case '2'://OSMboxPost (OSMbox,&message2);break;
				OSQPost(OSQ,(u8 *)message[1]);break;
				case '3'://OSMboxPost (OSMbox,&message2);break;
				OSQPost(OSQ,(u8 *)message[2]);break;
				case '4'://OSMboxPost (OSMbox,&message2);break;
				OSQPost(OSQ,(u8 *)message[3]);break;
				case '5'://OSMboxPost (OSMbox,&message2);break;
				OSQPost(OSQ,(u8 *)message[4]);break;
			}
	}
}
}
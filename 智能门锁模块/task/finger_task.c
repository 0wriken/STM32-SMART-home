#include "task.h"
#include "ucos_ii.h"
#include "stdio.h"
#include "uart.h"
#include "delay.h"
char *sbuf;
u8 error;
u8 *buff;
OS_STK FingerStack[FingerStackLen];
u8 finger_error;
u8 finger_flag;
void FingerTask(void *pd)  //指纹模块
{

	
	//指纹的配置，录入，删除，删除所有
	if(finger_flag==1)
		if(finger_flag==2)
			if(finger_flag==3)
			{
					
			}
}

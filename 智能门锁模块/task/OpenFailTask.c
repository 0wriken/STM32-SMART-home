#include "task.h"
#include "ucos_ii.h"
#include "stdio.h"
#include "uart.h"
#include "delay.h"
#include "monochome_oled.h"
#include "funtion.h"
u8 fail_flag;
u8 error11,error12,error13;
void OpenFailTask(void *pd)
{
	while(1)
	{
		fail_flag=OSFlagPend (OSFLAGFAIL, 
                      1<<2|1<<1|1<<0,
											OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,
                      0,
                     &error11);
	if(fail_flag==1)
	{
		oled_display_arbitrary_string(4,0,(u8 *)"ÃÜÂë´íÎó");
	}
		if(fail_flag==2)
	{
		oled_display_arbitrary_string(4,0,(u8 *)"Ö¸ÎÆ´íÎó");
	}
		if(fail_flag==4)
	{
		oled_display_arbitrary_string(4,0,(u8 *)"Ë¢¿¨Ê§°Ü");
	}
}
}



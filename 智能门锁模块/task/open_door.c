#include "task.h"
#include "ucos_ii.h"
#include "stdio.h"
#include "uart.h"
#include "delay.h"
#include "monochome_oled.h"
#include "MG200.h"
#include "funtion.h"
#include "time.h"
u8 error8,error9,error10;
u8 open_flag;
void OpendoorTask(void *pd)
{
	while(1)
	{
	open_flag=OSFlagPend (OSFLAGCHEK, 
                      1<<2|1<<1|1<<0,
											OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,
                      0,
                     &error8);
	//开门
	if(open_flag==1)
	{
		oled_display_clear();
		oled_display_arbitrary_string(4,0,(u8 *)"密码正确");
		 voice_data(0x18);
		open_dor();
	}
		if(open_flag==2)
	{
		oled_display_clear();
		oled_display_arbitrary_string(4,0,(u8 *)"指纹正确");
	}
		if(open_flag==4)
	{
		oled_display_clear();
		oled_display_arbitrary_string(4,0,(u8 *)"刷卡成功");
	}
	
	
	
	
	
}
	
}


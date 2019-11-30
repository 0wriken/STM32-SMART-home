#include "task.h"
#include "ucos_ii.h"
#include "stdio.h"
#include "uart.h"
#include "delay.h"
#include "monochome_oled.h"
#include "MG200.h"
#include "funtion.h"
#include "time.h"
#include "at24c02.h"
u8 error1,error2,error3;
extern u32 i;
u8 key_buf[9];
void GetPwTask(void *pd)   //获取开门信号
{
	while(1)
	{
	loop :
		
		oled_display_clear();
		i=0;
		oled_display_arbitrary_string(0,0,(u8 *)"请输入开机密码");
		memset((char *)key_buf,0,strlen((char *)key_buf));
		oled_display_arbitrary_string(6,0,(u8 *)"#确认");
	
		oled_display_arbitrary_string(6,86,(u8 *)"*退出");   
			while(1)
			{
		if(!(GPIOB->IDR&(0X1<<3)))  //当有按键按下时
		{
			memset((char *)key_buf,0,strlen((char *)key_buf));
				int j=0;
				u8 get;
				while(1)
				{
					get=get_key();
					
				if(get!=0)
				{
					key_buf[j++]=get;
//					printf("输入的密码为:%d\r\n",(u32)get);
					if(j>9||key_buf[j-1]=='#')   
						break;
					if(key_buf[j-1]=='*')   
					{
						goto loop;
					}
				}
			}
				printf("j=:%d\r\n",j);
				printf("输入的密码为:%s\r\n",(char *)key_buf);
							
				OSFlagPost (OSFLAGGET, 
											1<<0,    
											OS_FLAG_SET,
                     &error1);     //0位为密码开锁
		}
	}                             

		
//		oled_display_clear();
//		oled_display_arbitrary_string(0,0,(u8 *)"请输入指纹");
		
		
			if(!Match1N())         //采集指纹，0为无指纹信息
			{
			printf("采集的到指纹\r\n");
				finger_mesg=Match1N();

				EraseOne(Match1N());
				EraseAll();
				OSFlagPost (OSFLAGGET, 
                      1<<1,    
											OS_FLAG_SET,
                     &error2);  //1位指纹
			}
			
			
				OSFlagPost (OSFLAGGET, 
                      1<<2,    
											OS_FLAG_SET,
                     &error3);  //2位刷卡
		
			
		}
}

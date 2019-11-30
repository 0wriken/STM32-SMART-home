#include "funtion.h"
#include "time.h"
#include "MPR121.h"
#include "monochome_oled.h"
#include "oled_memory.h"
#include "MG200.h"
#include "stdio.h"
#include "uart.h"
#include "string.h"
u32 i=0;
u32 at=0;
//各种模块的初始化 功能实现
u8 voice_flag=0;
void delay(u32 nms)
{
		nms*=18;
	while(nms--);
}
void nms_delay(u32 nms)
{
		nms*=18000;
	while(nms--);
}
void GPIO_voice_init()
{
	RCC->APB2ENR|=(0X1<<3);
	GPIOB->CRL&=~(0XFF<<0);
	GPIOB->CRL&=~(0XFF<<4);
	GPIOB->CRL|=(0X1<<0);
	GPIOB->CRL|=(0X4<<4);
}
void voice_synchronization()
{
	GPIO_voice_init();
	GPIOB->ODR|=(0X1<<0);
	time_init(80);
	GPIOB->ODR&=~(0X1<<0);
	time_init(10);
}
void voice_high()
{
	GPIO_voice_init();
	GPIOB->ODR|=(0X1<<0);
	time_init(15);
	GPIOB->ODR&=~(0X1<<0);
	time_init(5);
}
void voice_low()
{
	GPIOB->ODR|=(0X1<<0);
	time_init(5);
	GPIOB->ODR&=~(0X1<<0);
	time_init(15);
}
//发送语音数据
void voice_data(u8 data)
{
	if(voice_flag==0)
	{
	voice_synchronization();
	for(u8 i=0;i<8;i++)
	{
		if(data&(0x80>>i))
			voice_high();
		else
			voice_low();
	}
	}
	voice_flag=1;
}
void voice_play()
{
	while(!!(GPIOB->IDR&(0X1<<1)));
		voice_flag=0;
}

//150ms~200ms开门
//PA1防止撬锁
//KEY1
//PA4,PA5
void GPIO_key_init()
{
	RCC->APB2ENR|=(0X1<<2);
	GPIOA->CRL &=~(0xff<<16);
	GPIOA->CRL|=(0X2<<16);
	GPIOA->CRL|=(0X2<<20);
}
void open_dor()
{
	GPIOA->ODR&=~(0x1<<5);
	GPIOA->ODR|=(0X1<<4);
	time_init(2000);
	GPIOA->ODR|=(0X1<<5);
	GPIOA->ODR|=(0X1<<4);
	time_init(1500);
	GPIOA->ODR&=~(0x1<<4);
	GPIOA->ODR|=(0X1<<5);
	time_init(2000);
}
u8 get_key()
{
	if(!(GPIOB->IDR&(0X1<<3)))
	{ 
				i+=4;
			i%=72; 
		if('1'==GetMPR121())
		{
			oled_display_arbitrary_string(2,i,(u8 *)"1");
//			printf("输出：%c",(char)'1');
			return '1';
		}
		if('2'==GetMPR121())
		{
		oled_display_arbitrary_string(2,i,(u8 *)"2");
			return '2';
		}
		if('3'==GetMPR121())
		{
		oled_display_arbitrary_string(2,i,(u8 *)"3");
		return '3';
		}
		if('4'==GetMPR121())
		{
		oled_display_arbitrary_string(2,i,(u8 *)"4");
		return '4';}
		if('5'==GetMPR121())
		{
		oled_display_arbitrary_string(2,i,(u8 *)"5");
		return '5';}
		if('6'==GetMPR121())
		{
		oled_display_arbitrary_string(2,i,(u8 *)"6");
		return '6';}
		if('7'==GetMPR121())
		{
		oled_display_arbitrary_string(2,i,(u8 *)"7");
		return '7';}
		if('8'==GetMPR121())
		{
		oled_display_arbitrary_string(2,i,(u8 *)"8");
	return '8';	}
		if('9'==GetMPR121())
		{
		oled_display_arbitrary_string(2,i,(u8 *)"9");
	return '9';	}	
		if('0'==GetMPR121())
		{
		oled_display_arbitrary_string(2,i,(u8 *)"0");
		return '0';}
			if('*'==GetMPR121())
			{
			oled_display_arbitrary_string(2,i,(u8 *)"*");
			return '*';
			}
		if('#'==GetMPR121())
		{
		oled_display_arbitrary_string(2,i,(u8 *)"#");
		return '#';}
	}
	return 0;
}
u32 get_key_buf(u8 *p)
{
	loop2 :
	oled_display_clear();
	oled_display_arbitrary_string(0,0,(u8 *)"请修改开机密码");
	oled_display_arbitrary_string(6,0,(u8 *)"#确认");
	i=0;
	oled_display_arbitrary_string(6,86,(u8 *)"*退出");  
	memset((char *)p,0,strlen((char *)p));
	int j=0;
	u8 get;
	while(1)
	{
		get=get_key();
	if(get!=0)
	{
		p[j++]=get;
//					printf("输入的密码为:%d\r\n",(u32)get);
		if(j>9||p[j-1]=='#')   
			break;
		if(p[j-1]=='*')   
		{
			goto loop2;
		}
	}
}
	return 1;

}
u32 root_get_key()
{
	if(!(GPIOB->IDR&(0X1<<3)))
	{ 
		return ((u32)get_key()-48);
//				if('1'==get_key())
//		{
//			return 1;
//		}
//		if('2'==get_key())
//		{
//			return 2;
//		}
//		if('3'==get_key())
//		{
//		return 3;
//		}
//		if('4'==get_key())
//		{
//		return 4;
//		}
//		if('5'==get_key())
//		{
//		return  5;
//		}
//		if('6'==get_key())
//		{
//		return 6;}
//		if('7'==get_key())
//		{
//		return 7;}
//		if('8'==get_key())
//		{
//	return 8;	}
//		if('9'==get_key())
//		{
//	return 9;	}	
//		if('0'==get_key())
//		{
//		return 0;}
//			if('*'==get_key())
//			{
//			return 10;
//			}
//		if('#'==get_key())
//		{
//		return 20;
//		}
}
	return 88;
}
//格式化函数
void format()
{
	     at=0;
//			At24c02WriteBytes(at,8,(u8 *)"11111111#");
//			At24c02PageWrite(16,8,(u8 *)"88888888");
//			At24c02WriteBytes(16+at,9,(u8 *)"88888888#");//地址16位存放开门密码传入数组中

//			EraseAll();
}
u8 * change_buf(u8 *p)
{
	u8 *b=NULL;
	for(u8 i=0;i<8;i++)
	{
		*b++=*p++;
	}
	return b;
}
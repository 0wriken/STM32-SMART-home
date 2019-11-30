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
void GetPwTask(void *pd)   //��ȡ�����ź�
{
	while(1)
	{
	loop :
		
		oled_display_clear();
		i=0;
		oled_display_arbitrary_string(0,0,(u8 *)"�����뿪������");
		memset((char *)key_buf,0,strlen((char *)key_buf));
		oled_display_arbitrary_string(6,0,(u8 *)"#ȷ��");
	
		oled_display_arbitrary_string(6,86,(u8 *)"*�˳�");   
			while(1)
			{
		if(!(GPIOB->IDR&(0X1<<3)))  //���а�������ʱ
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
//					printf("���������Ϊ:%d\r\n",(u32)get);
					if(j>9||key_buf[j-1]=='#')   
						break;
					if(key_buf[j-1]=='*')   
					{
						goto loop;
					}
				}
			}
				printf("j=:%d\r\n",j);
				printf("���������Ϊ:%s\r\n",(char *)key_buf);
							
				OSFlagPost (OSFLAGGET, 
											1<<0,    
											OS_FLAG_SET,
                     &error1);     //0λΪ���뿪��
		}
	}                             

		
//		oled_display_clear();
//		oled_display_arbitrary_string(0,0,(u8 *)"������ָ��");
		
		
			if(!Match1N())         //�ɼ�ָ�ƣ�0Ϊ��ָ����Ϣ
			{
			printf("�ɼ��ĵ�ָ��\r\n");
				finger_mesg=Match1N();

				EraseOne(Match1N());
				EraseAll();
				OSFlagPost (OSFLAGGET, 
                      1<<1,    
											OS_FLAG_SET,
                     &error2);  //1λָ��
			}
			
			
				OSFlagPost (OSFLAGGET, 
                      1<<2,    
											OS_FLAG_SET,
                     &error3);  //2λˢ��
		
			
		}
}

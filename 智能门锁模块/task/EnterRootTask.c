#include "task.h"
#include "ucos_ii.h"
#include "stdio.h"
#include "uart.h"
#include "delay.h"
#include "monochome_oled.h"
#include "funtion.h"
u8 root_error;
u8 fution_error;
void EnterRootTask(void *pd)
{
	while(1)
	{
		OSFlagPend (OSFLAGCHEK, 
                      1<<3,
											OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,
                      0,
					                      &root_error);
		oled_display_clear();
		oled_display_arbitrary_string(4,0,(u8 *)"�ѽ������Աģʽ");
	//��������ʵ�ֹ���

////		u8 i=root_get_key();//�������빦��
//		OSFlagPost (OSFLAGRootFution, 
//                      1<<i,    
//											OS_FLAG_SET,
//                     &fution_error);  //9������������
	
}
}


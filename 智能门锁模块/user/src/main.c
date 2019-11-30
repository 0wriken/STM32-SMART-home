#include "stm32f10x.h"
#include "uart.h"
#include "delay.h"
#include "stdio.h"
#include "iic.h"
#include "at24c02.h"
#include "MPR121.h"
#include "spi.h"
#include "oled.h"
#include "ucos_ii.h"
#include "task.h"
#include "led.h"
#include "monochome_oled.h"
#include "oled_memory.h"
#include "MG200.h"
extern u8 buf[100];


//�������ܣ�ϵͳ�δ��ж�-ms
//����˵����nms��ʱʱ��
//����ֵ����
//ע������:nms���ֵ798.915
//ʱ�䣺2018/11/20 
void Systick_Interrupt(u16 nms)
{
	SysTick->CTRL &=~(0x01<<2);//ѡ��ʱ��Դ��9M��
	SysTick->LOAD=9*1000*nms;//������װ��ֵ��LOAD��
	SysTick->VAL=0;//дVAL������VAL��COUNTFLAG���㣬����LOADֵ��VAL��
	SysTick->CTRL |=0x01<<1;//���δ��ж�-ģ�鼶
		//����NVIC���ȼ�
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority (7-2, 2, 2));
	//ϵͳ���ж�ʹ��--�ں��жϲ���ҪNVICʹ��
	//NVIC_EnableIRQ(SysTick_IRQn);
	
	SysTick->CTRL |=0x01<<0;//������ʱ��
}

void SysTick_Handler(void)
{
	OSIntEnter();
	
	OSTimeTick( );///����ϵͳ���ȵķ�����
	
	OSIntExit();
}



OS_EVENT *OS_MBOX;
OS_EVENT *OSQ;

int main(void)
{
	INT8U ret;
	NVIC_SetPriorityGrouping(7-2);
	
	//Ӳ����ʼ��
	DelayInit( );//��ʱ��1��ʱ���ܳ�ʼ��
	Usart1Init(72,115200);
	led_init();
	IICPinInit( );
	Usart2Init(36,115200);
	MPR121Init();
	MG200Init();
	SPIPinInit( );
//	OLEDInit( );
	GPIO_voice_init();
	GPIO_key_init();
	oled_init();
	
	
	Systick_Interrupt(1000/OS_TICKS_PER_SEC);//5ms����ϵͳ����
	
	OSInit();//��ʼ�� UCOS-II �ں���غ�����
	

	//����Ҫ����һ������
	ret=OSTaskCreate(StartTask,NULL,&StratStack[StratStackLen-1],StartTaskPrio);
//	if(ret!=OS_ERR_NONE)
//		printf("OSTaskCreate err!!\r\n");
//	
//	printf("OS Start!!!!!\r\n");
 	OSStart( );//���� uC/OS-II �Ķ����񻷾���
	
	
	//�����˲���ϵͳ������Ĵ���Ͳ��ᱻִ��
	while(1)
	{
		printf("||||||||||||||||||||||||||\r\n");
		
	}
}

#include "stm32f4xx.h"
#include "os.h"
#include "start_task.h"
#include "led.h"
#include "uart.h"
#include "tim_in.h"
#include "delay.h"
#include "ili9486.h"
#include "pic.h"
#include "touch.h"
/*
**************************************************************************
* ��������: ϵͳ�δ�ʱ����ʼ��
* �����β�: nms
* ��������ֵ: None
* ��ע: None
**************************************************************************
*/
void systick_init(uint32_t nms)
{
	SysTick->CTRL = 0;	//�������
	SysTick->VAL = 0;	//��ռ�����
	SysTick->LOAD = 21000 * nms;	//����nms�ļ���ֵ
	SysTick->CTRL |= 0X1 << 1;		//�����ж�ʹ��
	SysTick->CTRL |= 0X1 << 0;		//����ϵͳ�δ�ʱ��
}


void SysTick_Handler(void)
{
	OSIntEnter();	//���߲���ϵͳ���ж�
	
	OSTimeTick();	//�����������ĺ���
	
	OSIntExit();	//���߲���ϵͳ���ж�
}


/*
**************************************************************************
* ��������: ������
* �����β�: None
* ��������ֵ: None
* ��ע: None
**************************************************************************
*/
int main(void)
{
	led_init();		//��ʼ��LED�ƽӿ�
	key_init();
	uart1_init(84000000, 9600);
		printf("coming statr\n");

	LCD_Port_Init();
	LCD_Init();
	Delay_init();
	
	//display_line(300,20,30,0,RED);
	
	init_touch_gpio();
	systick_init(1000/OS_TICKS_PER_SEC);	//������������
	
	tim_in_init();
	OSInit();	//��ʼ������ϵͳ

	int k=0;
//	LCD_Clear(0,319,0,479,0xfff);
	while(1)
	{
		
		LCD_display_pic(0,150,(u8*)gImage_11);
		play_Square(40,100,RED);
		while ( t_para.sta.loosen == 0 )//�ȴ����ֱ�־��1
		{
			delay_ms(10);//�ȴ����´�����
		}
		t_para.sta.loosen = 0;
		if(t_para.sx <120&&t_para.sx >40&&t_para.sy>100&&t_para.sx < 150)
			printf("���Գɹ�!!!\n");
	}
	/* ����һ����ʼ���� */
	OSTaskCreateExt(start_task,
					0u,
					&start_task_stk[START_TASK_STK_SIZE - 1u],
					START_TASK_PRIO,
					START_TASK_PRIO,
					&start_task_stk[0u],
					START_TASK_STK_SIZE,
					0u,
					(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
	
	OSStart();	//������������
	
	/* ��ֹ����ϵͳ����ʧ�ܣ������ܷ� */
	while(1)
	{
		
	}
}

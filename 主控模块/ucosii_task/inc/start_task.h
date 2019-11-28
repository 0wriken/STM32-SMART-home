#ifndef __START_TASK_H__
#define __START_TASK_H__

#include "stm32f4xx.h"
#include "os.h"
#include "led.h"
#include "uart.h"
#include "Can.h"
#include "tim_in.h"
#include "string.h"
#include "delay.h"
#include "ili9486.h"
#include "touch.h"

typedef struct box
{ 
	u32 check_id[9];//���id�����
	u16 x[9];
	u16 y[9];
}dis;
extern dis display ;
extern OS_EVENT *Sem;

extern OS_EVENT *Sem1;

extern OS_EVENT *Sem2;

extern OS_EVENT *OSQ;

extern OS_EVENT *QSM;


extern OS_EVENT  *mutex;



extern OS_FLAG_GRP  *flag;
#define START_TASK_STK_SIZE    256
#define START_TASK_PRIO    15
extern OS_STK start_task_stk[START_TASK_STK_SIZE];

void start_task(void *p_arg);


/////////�û�����
#define LCD_TASK_STK_SIZE    256
#define START_LCD_PRIO    18  
extern OS_STK lcd_task_stk[LCD_TASK_STK_SIZE];

void lcd_task(void *p_arg);


/////////�ƹ����
#define CHECK_TASK_STK_SIZE    256
#define START_CHECK_PRIO    8  
extern OS_STK check_task_stk[CHECK_TASK_STK_SIZE];

void check_task(void *p_arg);



////////�����
#define PLAY_TASK_STK_SIZE    256
#define START_PLAY_PRIO    7 
extern OS_STK play_task_stk[PLAY_TASK_STK_SIZE];

void play_task(void *p_arg);


////////��ˮ���
#define WATER_TASK_STK_SIZE    256
#define START_WATER_PRIO    9 
extern OS_STK water_task_stk[WATER_TASK_STK_SIZE];

void water_task(void *p_arg);

//////////////�������
////////��ˮ���
#define SUN_TASK_STK_SIZE    256
#define START_SUN_PRIO    10 
extern OS_STK sun_task_stk[SUN_TASK_STK_SIZE];

void sun_task(void *p_arg);



#endif

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
	u32 check_id[9];//存放id做检查
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
#define START_TASK_STK_SIZE    128
#define START_TASK_PRIO    10  
extern OS_STK start_task_stk[START_TASK_STK_SIZE];

void start_task(void *p_arg);


/////////用户界面
#define LCD_TASK_STK_SIZE    128
#define START_LCD_PRIO    16  
extern OS_STK lcd_task_stk[LCD_TASK_STK_SIZE];

void lcd_task(void *p_arg);


/////////检测CAN
#define CHECK_TASK_STK_SIZE    128
#define START_CHECK_PRIO    15  
extern OS_STK check_task_stk[CHECK_TASK_STK_SIZE];

void check_task(void *p_arg);



////////显示收到的数据或触发的数据
#define PLAY_TASK_STK_SIZE    128
#define START_PLAY_PRIO    14 
extern OS_STK play_task_stk[PLAY_TASK_STK_SIZE];

void play_task(void *p_arg);

#endif

#ifndef __START_TASK_H__
#define __START_TASK_H__

#include "stm32f4xx.h"
#include "os.h"
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


//////////////////
#define LED_TASK_STK_SIZE    128
#define START_LED_PRIO    12  
extern OS_STK led_task_stk[LED_TASK_STK_SIZE];

void led_task(void *p_arg);


//////////////////
#define KEY_TASK_STK_SIZE    128
#define START_KEY_PRIO    15  
extern OS_STK key_task_stk[KEY_TASK_STK_SIZE];

void key_task(void *p_arg);

/////////////////////////
#define LED1_TASK_STK_SIZE    128
#define START_LED1_PRIO     13
extern OS_STK led1_task_stk[LED1_TASK_STK_SIZE];

void led1_task(void *p_arg);

//////////////////////
#define LED2_TASK_STK_SIZE    128
#define START_LED2_PRIO    14 
extern OS_STK led2_task_stk[LED2_TASK_STK_SIZE];

void led2_task(void *p_arg);

#endif

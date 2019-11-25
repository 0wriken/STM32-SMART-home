#include "start_task.h"
#include "led.h"
#include "tim_in.h"

OS_STK start_task_stk[START_TASK_STK_SIZE];

/*
**************************************************************************
* 函数功能: 启动任务函数
* 函数形参: void *p_arg    传入该函数数据的指针
* 函数返回值: None
* 备注: None
**************************************************************************
*/
OS_EVENT *Sem;

OS_EVENT *Sem1;

OS_EVENT *Sem2;

OS_EVENT *OSQ;

OS_EVENT *QSM;

OS_EVENT  *mutex;

OS_FLAG_GRP  *flag;


INT8U  perr;
void *Mes[100];
OS_STK start_task_stk[START_TASK_STK_SIZE];
 OS_STK key_task_stk[KEY_TASK_STK_SIZE];
 OS_STK led_task_stk[LED_TASK_STK_SIZE];
 OS_STK led1_task_stk[LED1_TASK_STK_SIZE];
 OS_STK led2_task_stk[LED2_TASK_STK_SIZE];
void start_task(void *p_arg)
{
		Sem=OSSemCreate(0);
		Sem1=OSSemCreate(0);
		Sem2=OSSemCreate(0); 
	
		OSQ=OSQCreate(Mes,100);//
		
		QSM=OSMboxCreate ((void *)0);
	
		mutex=OSMutexCreate (5,
                          &perr);
		flag=OSFlagCreate (0,
                            &perr);

	
		OSTaskCreateExt(led_task,
					0u,
					&led_task_stk[LED_TASK_STK_SIZE-1u],
					START_LED_PRIO ,
					START_LED_PRIO ,
					&led_task_stk[0u],
					LED_TASK_STK_SIZE,
					0u,
					(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

	
	OSTaskCreateExt(led1_task,
					0u,
					&led1_task_stk[LED1_TASK_STK_SIZE-1u],
					START_LED1_PRIO ,
					START_LED1_PRIO ,
					&led1_task_stk[0u],
					LED1_TASK_STK_SIZE,
					0u,
					(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
	
	OSTaskCreateExt(led2_task,
					0u,
					&led2_task_stk[LED2_TASK_STK_SIZE-1u],
					START_LED2_PRIO ,
					START_LED2_PRIO ,
					&led2_task_stk[0u],
					LED2_TASK_STK_SIZE,
					0u,
					(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
						
	OSTaskCreateExt(key_task,
					0u,
					&key_task_stk[KEY_TASK_STK_SIZE-1u],
					START_KEY_PRIO ,
					START_KEY_PRIO ,
					&key_task_stk[0u],
					KEY_TASK_STK_SIZE,
					0u,
					(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));


}

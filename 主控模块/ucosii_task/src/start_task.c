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
OS_STK lcd_task_stk[LCD_TASK_STK_SIZE];
OS_STK check_task_stk[CHECK_TASK_STK_SIZE];
OS_STK play_task_stk[PLAY_TASK_STK_SIZE];
OS_STK sun_task_stk[SUN_TASK_STK_SIZE];
OS_STK water_task_stk[WATER_TASK_STK_SIZE];
void start_task(void *p_arg)
{
//		Sem=OSSemCreate(0);
//		Sem1=OSSemCreate(0);
//		Sem2=OSSemCreate(0); 
	
		//OSQ=OSQCreate(Mes,100);//
		
		QSM=OSMboxCreate ((void *)0);
	
		mutex=OSMutexCreate (5,
                          &perr);
		flag=OSFlagCreate (0,
                            &perr);

	OSTaskCreate(lcd_task,NULL,
	&lcd_task_stk[LCD_TASK_STK_SIZE-1u],
	START_LCD_PRIO);
//		OSTaskCreateExt(lcd_task,
//					0u,
//					&lcd_task_stk[LCD_TASK_STK_SIZE-1u],
//					START_LCD_PRIO ,
//					START_LCD_PRIO ,
//					&lcd_task_stk[0u],
//					LCD_TASK_STK_SIZE,
//					0u,
//					(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
		OSTaskCreate(check_task,NULL,
	&check_task_stk[CHECK_TASK_STK_SIZE-1u],
	START_CHECK_PRIO);
	
	OSTaskCreate(play_task,NULL,
			&play_task_stk[PLAY_TASK_STK_SIZE-1u],
	START_PLAY_PRIO);	
					
	OSTaskCreate(water_task,NULL,
			&water_task_stk[WATER_TASK_STK_SIZE-1u],
	START_WATER_PRIO);	
	
		OSTaskCreate(sun_task,NULL,
			&sun_task_stk[SUN_TASK_STK_SIZE-1u],
	START_SUN_PRIO);
					
//	OSTaskCreateExt(check_task,
//					0u,
//					&check_task_stk[CHECK_TASK_STK_SIZE-1u],
//					START_CHECK_PRIO ,
//					START_CHECK_PRIO ,
//					&check_task_stk[0u],
//					CHECK_TASK_STK_SIZE,
//					0u,
//					(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
	
//	OSTaskCreateExt(play_task,
//					0u,
//					&play_task_stk[PLAY_TASK_STK_SIZE-1u],
//					START_PLAY_PRIO ,
//					START_PLAY_PRIO ,
//					&play_task_stk[0u],
//					PLAY_TASK_STK_SIZE,
//					0u,
//					(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
//					while(1)
//					{
//						;
//					}
	while(1)
	{
		
		OSTimeDly(1);
	}



}

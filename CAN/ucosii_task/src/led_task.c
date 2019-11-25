#include "start_task.h"
#include "led.h"
#include "uart.h"
void led_task(void *arg)
{
	INT8U   err;
	 OS_STK_DATA  data={0};
	while(1)
	{
//			OSSemPend(Sem, 0, &err);
//		OSTaskStkChk (START_LED_PRIO ,
//                     &data);
//			LED4_TOR();
//			printf("%d\n",(data.OSUsed*100)/(data.OSFree+data.OSUsed));
//		OSTimeDlyHMSM(0u, 0u, 0u, 500u);
//		/*-printf("%s\n",(char *)OSQPend (OSQ,
//                0,
//                &err));*/
//		printf("%s\n",(char *)OSMboxPend (QSM,
//                   0,
//                   &err));

//		OSTimeDlyHMSM(0u, 0u, 0u, 200u);
//		
//		OSMutexPend (mutex,
//                   0,
//                   &err);
//		printf("coming 2\n");
//		LED3_ON();
//		OSTimeDlyHMSM(0u, 0u, 0u, 2000u);
//		LED3_OFF();
//		OSMutexPost (mutex);
//		
LED3_OFF();
		OSFlagPend (flag,
                      1<<1,
                      OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,
                      0,
                      &err);
		LED3_ON();
		printf("coming 2\n");
		OSTimeDlyHMSM(0u, 0u, 0u, 2000u);
		LED3_OFF();

	}
}

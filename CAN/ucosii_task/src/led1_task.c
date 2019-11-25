#include "start_task.h"
#include "led.h"
#include "uart.h"
void led1_task(void *arg)
{
	INT8U err;
	 OS_STK_DATA  data={0};
	while(1)
	{
//			OSSemPend(Sem1, 0, &err);
//		printf("come sem1 \n");
//			LED4_TOR();
//		OSTimeDlyHMSM(0u, 0u, 0u, 1000u);
//		OSMutexPend (mutex,
//                   0,
//                   &err); 
//		LED4_ON();
//		printf("coming 1\n");
//		OSTimeDlyHMSM(0u, 0u, 0u, 2000u);
//		LED4_OFF();
//		OSMutexPost (mutex);
		LED4_OFF();
		OSFlagPend (flag,
                      0x1<<0,
                      OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,
                      0,
                      &err);
		LED4_ON();
		printf("coming 1\n");
		OSTimeDlyHMSM(0u, 0u, 0u, 2000u);
		LED4_OFF();
		
	}
	OSTimeDlyHMSM(0u, 0u, 0u, 200u);
}
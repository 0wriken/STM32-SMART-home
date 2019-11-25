#include "start_task.h"
#include "led.h"
#include "uart.h"
void led2_task(void *arg)
{
	INT8U err;
	while(1)
	{
			
			OSSemPend(Sem2, 0, &err);
			LED4_TOR();
		OSTimeDlyHMSM(0u, 0u, 0u, 200u);
	}
}


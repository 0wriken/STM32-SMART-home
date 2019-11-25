#include "start_task.h"
#include "led.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
void key_task(void *arg)
{
	INT8U err;
	u8 i=0;
	u8 *data=(u8*)malloc(sizeof(u8)*10);
	while(1)
	{
		i=key_status();
		switch(i)
		{
			case 2:
//			err = OSSemPost(Sem);
			OSFlagPost (flag,
                      1<<0,
                      OS_FLAG_SET,
                      &err);
			break;
			case 3:
//			err=OSSemPost(Sem1);
			OSFlagPost (flag,
                      1<<1,
                      OS_FLAG_SET,
                      &err);
			break;
			case 4:
			//err=OSSemPost(Sem2);
			OSFlagPost (flag,
                      3<<0,
                      OS_FLAG_SET,
                      &err);
			break;
		}
//		i=key_status();
//		switch(i)
//		{
//			case 2:
//					sprintf((char *)data,"%d,%s",2,"KEY2");
//			OSQPost (OSQ,
//                (void *)data);
//			OSMboxPost (QSM,(void *)data);
//			break;
//			case 3:
//					sprintf((char *)data,"%d,%s",3,"KEY3");
//			OSQPost (OSQ,
//                (void *)data);
//			OSMboxPost (QSM,(void *)data);
//			break;
//			case 4:
//					sprintf((char *)data,"%d,%s",4,"KEY4");
//			OSQPost (OSQ,
//                (void *)data);
//			
//			OSMboxPost (QSM,(void *)data);
//			break;
//			default :continue;
//			
//		}
		
		OSTimeDlyHMSM(0u, 0u, 0u, 500u);
	}
}

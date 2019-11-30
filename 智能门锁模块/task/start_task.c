#include "task.h"
#include "ucos_ii.h"
#include "stdio.h"
#include "uart.h"
#include "delay.h"
#include "monochome_oled.h"
u8 perr;
OS_STK StratStack[StratStackLen];
OS_STK PassWordStack[PassWordStackLen];
OS_STK GetPwStack[GetPwStacklen];
OS_STK CheckPwStack[CheckPwStacklen];
OS_STK OpendoorStack[OpendoorStacklen];
OS_STK OpenFailStack[OpenFailStackLen];
//OS_STK EnterRootStack[EnterRootStackLen];
OS_STK RootFutionStack[RootFutionStackLen];
OS_FLAG_GRP *OSFLAGCHEK;

OS_FLAG_GRP *OSFLAGGET;

OS_FLAG_GRP *OSFLAGFAIL;

OS_FLAG_GRP *OSFLAGRootFution;
void StartTask(void *pd)
{

	printf("the startrtask\n\r");
	OSQ=OSQCreate(NULL,0);//
	OSFLAGGET=OSFlagCreate (0,&perr);
	
	OSFLAGCHEK=OSFlagCreate (0,&perr);

	OSFLAGFAIL=OSFlagCreate (0,&perr);	

	OSFLAGRootFution=OSFlagCreate (0,&perr);		



	
	OSTaskCreate(OpendoorTask,NULL,&OpendoorStack[OpendoorStacklen-1],Opendoortaskprio);
	
	OSTaskCreate(GetPwTask,NULL,&GetPwStack[GetPwStacklen-1],GetPwtaskprio);
	
	
	OSTaskCreate(CheckPwTask,NULL,& CheckPwStack[CheckPwStacklen-1],CheckPwtaskprio);
	
	OSTaskCreate(OpendoorTask,NULL,& OpendoorStack[OpendoorStacklen-1],Opendoortaskprio);
	
	OSTaskCreate(OpenFailTask,NULL,& OpenFailStack[OpenFailStackLen-1],OpenFailtaskPrio);
	
//	OSTaskCreate(EnterRootTask,NULL,& EnterRootStack[EnterRootStackLen-1],EnterRootTaskPrio );  //关闭进入进入管理员模式
	
	OSTaskCreate(RootFutionTask,NULL,& RootFutionStack[RootFutionStackLen-1],RootFutionTaskPrio );
	format(); //出厂设置

	
	
	
	while(1)
	{
		
		OSTimeDly(1);
	}

}

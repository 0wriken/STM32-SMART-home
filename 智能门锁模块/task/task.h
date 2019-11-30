#ifndef _TASK_H_
#define _TASK_H_

#include "stm32f10x.h"
#include "ucos_ii.h"
#include "MPR121.h"
#include "funtion.h"
#include "at24c02.h"
#include <stdio.h>
#include <string.h>

extern OS_EVENT *OSQ;
extern OS_FLAG_GRP *OSFLAGCHEK;

extern OS_FLAG_GRP *OSFLAGGET;

extern OS_FLAG_GRP *OSFLAGFAIL;

extern OS_FLAG_GRP *OSFLAGRootFution;
extern u8 finger_mesg;
//-----------------------------------------------
#define StartTaskPrio 5
#define StratStackLen 128
extern OS_STK StratStack[StratStackLen];
void StartTask(void *pd);

//-----------------------------------------------
//#define FingerTaskPrio 8     //ָ��ģ��
//#define FingerStackLen 128
//extern OS_STK FingerStack[FingerStackLen];
//void FingerTask(void *pd);


//-----------------------------------------------
#define PassWordTaskPrio 18   //����ģ��
#define PassWordStackLen 128
extern OS_STK PassWordStack[PassWordStackLen];
void PassWordTask(void *pd);



//-----------------------------------------------
//#define UartTaskPrio 18   //����
//#define UartStackLen 128
//extern OS_STK UartStack[UartStackLen];
//void UartTask(void *pd);


//-----------------------------------------------
#define GetPwtaskprio 30    //��ȡ���루ָ�Ƶȣ�
#define GetPwStacklen 128
extern OS_STK GetPwStack[GetPwStacklen];
void GetPwTask(void *pd);



//-----------------------------------------------
#define CheckPwtaskprio 25    //������루ָ�Ƶȣ�
#define CheckPwStacklen 128
extern OS_STK CheckPwStack[CheckPwStacklen];
void CheckPwTask(void *pd);


//-----------------------------------------------
#define Opendoortaskprio 20     //����
#define OpendoorStacklen 128
extern OS_STK OpendoorStack[OpendoorStacklen];
void OpendoorTask(void *pd);




//-----------------------------------------------
#define OpenFailtaskPrio 19    //��ʾ����ʧ��
#define OpenFailStackLen 128
extern OS_STK OpenFailStack[OpenFailStackLen];
void OpenFailTask(void *pd);


//-----------------------------------------------
//#define EnterRootTaskPrio 16  //�������Աģʽ
//#define EnterRootStackLen 128
//extern OS_STK EnterRootStack[EnterRootStackLen];
//void EnterRootTask(void *pd);


//-----------------------------------------------
#define RootFutionTaskPrio 11  //����Ա�޸�
#define RootFutionStackLen 128
extern OS_STK RootFutionStack[RootFutionStackLen];
void RootFutionTask(void *pd);





#endif


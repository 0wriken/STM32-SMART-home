#ifndef _MPR121_H_
#define _MPR121_H_

#include "stm32f10x.h"
#include "io_bit.h"

#define MPR_IRQ PBin(3)

#define MPR121ADDR    0x5A
#define MPR121ADDR_W  MPR121ADDR<<1
#define MPR121ADDR_R (MPR121ADDR<<1)|1

//Register Address         Register Name 
//AN3891 A
#define  MHD_Rising          0x2B 
#define  NHD_Amount_Rising   0x2C 
#define  NCL_Rising          0x2D
#define  FDL_Rising          0x2E

#define  MHD_Falling         0x2F 
#define  NHD_Amount_Falling  0x30 
#define  NCL_Falling         0x31 
#define  FDL_Falling          0x32 
//AN3892 C
#define   ELE0_Touch_Threshold     0x41
#define   ELE0_Release_Threshold   0x42
#define   ELE1_Touch_Threshold     0x43
#define 	ELE1_Release_Threshold   0x44
#define   ELE2_Touch_Threshold     0x45
#define   ELE2_Release_Threshold   0x46
#define 	ELE3_Touch_Threshold     0x47
#define   ELE3_Release_Threshold   0x48 
#define   ELE4_Touch_Threshold     0x49
#define   ELE4_Release_Threshold	 0x4A 
#define   ELE5_Touch_Threshold 		 0x4B
#define   ELE5_Release_Threshold 	 0x4C
#define   ELE6_Touch_Threshold     0x4D
#define   ELE6_Release_Threshold   0x4E
#define   ELE7_Touch_Threshold     0x4F
#define   ELE7_Release_Threshold   0x50
#define   ELE8_Touch_Threshold     0x51
#define   ELE8_Release_Threshold   0x52 
#define   ELE9_Touch_Threshold     0x53
#define   ELE9_Release_Threshold   0x54 
#define   ELE10_Touch_Threshold    0x55
#define   ELE10_Release_Threshold  0x56 
#define   ELE11_Touch_Threshold    0x57 
#define   ELE11_Release_Threshold  0x58
//AN3890 D
#define   Filter_Configuration     0x5D
//AN3890 E
#define   Electrode_Configuration  0x5E
//AN3890 F
#define  AUTO_CONFIG_Control       0x7B
#define  AUTO_CONFIG_USL           0x7D 
#define  AUTO_CONFIG_LSL           0x7E
#define  AUTO_CONFIG_Target Level  0x7F

void MPR121Init(void);
u8 GetMPR121(void);

#endif


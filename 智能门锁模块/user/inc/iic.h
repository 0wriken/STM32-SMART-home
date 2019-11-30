#ifndef _IIC_H_
#define _IIC_H_

#include "stm32f10x.h"
#include "io_bit.h"

#define IIC_SCL     PBout(4)
#define IIC_SDA_OUT PBout(5)
#define IIC_SDA_IN  PBin(5)


void IICPinInit(void);
void IICStart(void);
void IICStop(void);
u8 IICSendByte(u8 data) ;
u8 IICReadByte(u8 ack) ;

#endif


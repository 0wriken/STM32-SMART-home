#ifndef _AT24C02_H_
#define _AT24C02_H_

#include "stm32f10x.h"


#define AT24C02ADDR    0x50//01010000
#define AT24C02ADDR_W  0xA0//10100000
#define AT24C02ADDR_R  0xA1//10100001


#define WRITE_ERR1 1
#define WRITE_ERR2 2
#define WRITE_ERR3 3
#define WRITE_ERR4 4
#define WRITE_ERR5 5
#define WRITE_ERR6 6
#define WRITE_ERR7 7
#define WRITE_ERR8 8

#define READ_ERR1 11
#define READ_ERR2 22
#define READ_ERR3 33
#define READ_ERR4 44
#define READ_ERR5 55

#define NOTSAMEPAGE 66

u8 At24c02WriteByte(u8 addr,u8 data);
u8 At24c02ReadBytes(u8 addr,u16 num,u8* p);
u8 At24c02PageWrite(u8 addr,u8 num,u8* p);
u8 At24c02WriteBytes(u8 addr,u16 num,u8* p);
#endif


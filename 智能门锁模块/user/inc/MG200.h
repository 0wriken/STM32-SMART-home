#ifndef _MG200_H_
#define _MG200_H_

#include "stm32f10x.h"
#include "io_bit.h"

#define MG200_PWR     PBout(9)
#define MG200_DETECT  PCin(13)

#define STARTCODE 0x6C//起始码 表示发送包的开始字节，固定为 6Ch
#define SENDADDR  0x63//发送地址 表示-发送地址（主控单元 MCU 地址） (63h)
#define REVADDR   0x62//接收地址 表示接收地址（指纹模块地址） (62h)
#define RESERVE   0x00//预留 预留字节， 后续扩展使用；目前未使用，默认为 00h

#define NO_ERROR 0
#define RECEIVE_PARAMETER_ERROR  1
#define RECEIVE_CHECKSUM_ERROR   2

#define ACK_ERR 0XAB

//常见的错误返回值:
//80h : 指纹模块接收到无效命令
//81h : 指纹模块接收到错误校验和
//83h : 指纹模块接收到无效参数
//84h : 指纹模块遇到内存错误

void MG200Init(void);
u8 CaptureAndExtract(u8 param);
u8 Enroll(void);
u8 Match1N(void);
u8 EraseOne(u8 ID);
u8 EraseAll(void);
u8  GetUserNum(void);
#endif


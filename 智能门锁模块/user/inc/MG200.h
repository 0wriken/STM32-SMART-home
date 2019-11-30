#ifndef _MG200_H_
#define _MG200_H_

#include "stm32f10x.h"
#include "io_bit.h"

#define MG200_PWR     PBout(9)
#define MG200_DETECT  PCin(13)

#define STARTCODE 0x6C//��ʼ�� ��ʾ���Ͱ��Ŀ�ʼ�ֽڣ��̶�Ϊ 6Ch
#define SENDADDR  0x63//���͵�ַ ��ʾ-���͵�ַ�����ص�Ԫ MCU ��ַ�� (63h)
#define REVADDR   0x62//���յ�ַ ��ʾ���յ�ַ��ָ��ģ���ַ�� (62h)
#define RESERVE   0x00//Ԥ�� Ԥ���ֽڣ� ������չʹ�ã�Ŀǰδʹ�ã�Ĭ��Ϊ 00h

#define NO_ERROR 0
#define RECEIVE_PARAMETER_ERROR  1
#define RECEIVE_CHECKSUM_ERROR   2

#define ACK_ERR 0XAB

//�����Ĵ��󷵻�ֵ:
//80h : ָ��ģ����յ���Ч����
//81h : ָ��ģ����յ�����У���
//83h : ָ��ģ����յ���Ч����
//84h : ָ��ģ�������ڴ����

void MG200Init(void);
u8 CaptureAndExtract(u8 param);
u8 Enroll(void);
u8 Match1N(void);
u8 EraseOne(u8 ID);
u8 EraseAll(void);
u8  GetUserNum(void);
#endif


#ifndef _UART_H_
#define _UART_H_

#include "stm32f10x.h"
#include <string.h>
#include <stdio.h>

typedef struct
{
	u8 RevBuf[10];
	u8 RevLen;
	u8 RevOver;
}TYPE_U2;

extern TYPE_U2 MG200rev;


void Usart1Init(u32 apb2clk,u32 bound);
void Usart2Init(u32 apb2clk,u32 bound);
void uart2_send_string(u8 *p);
u8  *OS_MBOX_masg(void);




#endif


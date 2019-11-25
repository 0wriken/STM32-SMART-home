#ifndef _UART_H_
#define _UART_H_

#include "stm32f4xx.h"
#include <string.h>
#include <stdio.h>

typedef struct
{
	u8 RevBuf[10];
	u8 RevLen;
	u8 RevOver;
}TYPE_U2;

extern TYPE_U2 MG200rev;


void uart1_init(uint32_t pclk2, uint32_t bound);
u8 rev_byte(void);
void send_byte(u8 val);




#endif


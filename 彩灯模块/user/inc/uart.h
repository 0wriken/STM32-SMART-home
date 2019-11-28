#ifndef __UART_H__
#define __UART_H__

#include "stm8s.h"

void uart1_Init(void);
void send_string(u8 *str);

#endif

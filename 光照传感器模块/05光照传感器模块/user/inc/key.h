#ifndef __KEY_H__
#define __KEY_H__

#include "stm8s.h"

#define KEY1 (!!(GPIOD->IDR & 1<<2))
#define KEY2 (!!(GPIOD->IDR & 1<<3))


u8 key_scan(void);
void init_key(void);

#endif 


#ifndef _FUNTION_H_
#define _FUNTION_H_
#include "stm32f10x.h"
#include "at24c02.h"
void delay(u32 nms); 
void GPIO_voice_init(void);
void voice_synchronization(void);
void voice_high(void);
void voice_low(void);
void voice_data(u8 data);
void voice_play(void);
void GPIO_key_init(void);
void open_dor(void);
void nms_delay(u32 nms);
u8 get_key(void);
u32 get_key_buf(u8 *p);
u32 root_get_key(void);
void format(void);
u8 * change_buf(u8 *p);
#endif


#ifndef __CANBUS_H__
#define __CANBUS_H__

#include "stm32f4xx.h"

u8 init_can1(u8 brp, u8 tbs1, u8 tbs2, u8 tsjw, u8 mode);
u8 can1_send_msg( u32 id,u8* msg, u8 len);
u8 can1_receive_msg(u8 *buf,u32 *id);
u8 can1_tx_msg(u32 id,u8 ide,u8 rtr,u8 len, u8 *p_dat);

#endif 


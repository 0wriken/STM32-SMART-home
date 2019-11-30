#ifndef _OLED_H_
#define _OLED_H_

#include "stm32f10x.h"
#include "io_bit.h"

void OLEDInit(void);

#define OLED_CS  PAout(15)
#define OLED_DC  PAout(11)
#define OLED_RES PAout(12)

#endif


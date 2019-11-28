#ifndef RGB_H_
#define RGB_H_

#include "stm8s.h"

void rgb_gpio_init(void);
void rgb_red_on(void);
void rgb_gree_on(void);
void rgb_blue_on(void);

void rgb_red_off(void);
void rgb_gree_off(void);
void rgb_blue_off(void);

void unknown_color(u32 time1,u32 time2,u32 time3);
void rgb_delay(u32 time);

#endif
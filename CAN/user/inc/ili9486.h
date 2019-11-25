#ifndef __ILI9486_H__
#define __ILI9486_H__

#include <stm32f4xx.h>
#include "delay.h"

void LCD_ILI9486_CMD(u16 common);
void LCD_Port_Init(void);
void LCD_ILI9486_Parameter(u16 data);
void LCD_Clear(u16 SX, u16 EX, u16 SY, u16 EY, u16 Color);
void LCD_Init(void);
void LCD_display_pic(u16 X,u16 Y,u8 *pic_data);
void play_string(u16 x,u16 y,u16 t_color,u16 b_color,u8*buff);
void display_line(u16 x,u16 y,u16 lenth,u16 angle,u16 color);

#endif


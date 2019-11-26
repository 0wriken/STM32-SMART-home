#ifndef _TOUCH_H_
#define _TOUCH_H_

#include "stm32f4xx.h"
#include "delay.h"
#include "ili9486.h"

#define LCD_WIDTH 	320
#define LCD_HEIGHT 	480

typedef struct 
{
	struct 
	{
		vu8 hold:1;	//���±��ֱ�־
		vu8 loosen:1;	//���ֱ�־
		vu8 press:1;	//���±�־
		vu8 reserved:5;	//����λ
	}sta;
	vu16 sx;	//��һ�ΰ���ʱ������
	vu16 sy;
	vu16 x;		//��ǰ����
	vu16 y;
}T_DEV_TYPE;


typedef struct 
{
	float xfac;
	float yfac;
	float xoff;
	float yoff;
	u32 flag;   //Ϊ0����־��ǰ����û��У׼��
}T_ADJUST_TYPE;


extern T_DEV_TYPE t_para;//����״̬&����
extern T_ADJUST_TYPE t_adjust_para;//У׼����



#define T_CS_LOW       (GPIOC->ODR &=~(1<<13))
#define T_CS_HIGH      (GPIOC->ODR |=(1<<13))
#define T_MOSI_HIGH 	(GPIOF->ODR |=(1<<11))
#define T_MOSI_LOW  	(GPIOF->ODR &=~(1<<11))
#define T_CLK_HIGH     (GPIOB->ODR |=(1<<0))
#define T_CLK_LOW      (GPIOB->ODR &=~(1<<0))
#define T_MISO         (GPIOB->IDR &(1<<2))

#define RED 0xf8f8
#define WHITE 0Xffff
//#define T_PEN          ((GPIOB->IDR &(1<<1)) && 1)
#define T_PEN          (!!(GPIOB->IDR &(1<<1)))


#define  X_CMD  0XD0
#define  Y_CMD  0X90
#define  T_ADJUST_OK  0xa5

void init_touch_gpio(void);
void send_touch_cmd(u8 cmd);
u16 rev_touch_data(void);
u16 return_xy_adval(u8 cmd);
u8 get_xyad_aveval(u16 *xvol,u16 *yvol);
void touch_adjust(void);
void init_timer6(void);
void lcd_dis_adjust_point(u16 x,u16 y,u16 color);

#endif


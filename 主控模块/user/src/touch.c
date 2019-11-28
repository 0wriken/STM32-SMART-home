#include "touch.h"

#include <stdlib.h>
#include <stdio.h>


T_ADJUST_TYPE t_adjust_para;//У׼����
T_DEV_TYPE t_para;//����״̬&����


/*
������:init_touch_gpio
������������
��������ֵ����
�������ܣ�ʵ�ִ�����оƬ(XTP2046)�Ĺܽų�ʼ��
XTP2046    LCD��     MCUIO��
CLK       T_CLK      PB0 ���
CS        T_CS       PC13 ���
DIN       T_MOSI     PF11 ���
DOUT      T_MISO     PB2 ����
PEN       T_PEN      PB1 ����
*/
void init_touch_gpio(void)
{
	//��PB PC PFʱ��
	RCC->AHB1ENR |= (1<<1)|(1<<2)|(1<<5);
	
	//CLK--PB0 ͨ������ 50M 
	/* ģʽ���� */
	GPIOB->MODER &=~(0X3<<0);  //����
	GPIOB->MODER |=(0X1<<0);  //���ó�ͨ�����  
	
	/*����������� */
	GPIOB->OTYPER &=~(1<<0);  //���ó��������
	
	/* ����������� */
	GPIOB->OSPEEDR &=~(0X3<<0);  //����
	GPIOB->OSPEEDR |=(0X2<<0);   //���ó��������50M
	
	//CS--PC13 ͨ������ 50M 
	/* ģʽ���� */
	GPIOC->MODER &= ~(0X3<<26);  //����
	GPIOC->MODER |= (0X1<<26);  //���ó�ͨ�����  
	
	/*����������� */
	GPIOC->OTYPER &= ~(1<<13);  //���ó��������
	
	/* ����������� */
	GPIOC->OSPEEDR &=~(0X3<<26);  //����
	GPIOC->OSPEEDR |=(0X2<<26);   //���ó��������50M
	
	//DIN--MOSI PF11 ͨ������ 50M 
	/* ģʽ���� */
	GPIOF->MODER &= ~(0X3<<22);  //����
	GPIOF->MODER |= (0X1<<22);  //���ó�ͨ�����  
	
	/*����������� */
	GPIOF->OTYPER &= ~(1<<11);  //���ó��������
	
	/* ����������� */
	GPIOF->OSPEEDR &= ~(0X3<<22);  //����
	GPIOF->OSPEEDR |= (0X2<<22);   //���ó��������50M
	
	/* DOUT--T_MISO--PB2 ���� */
	/* �ܽ�ģʽ */
	GPIOB->MODER &= ~(0X3<<4);  //����
	GPIOB->MODER |= (0X0<<4);   //���ó�����ģʽ
	
	/* �ܽ�������ģʽ */ 
	GPIOB->PUPDR &=~(0X3<<4);  //���ó���������-����
	
	/* PEN--T_PEN--PB1 ���� */
	/* �ܽ�ģʽ */
	GPIOB->MODER &= ~(0X3<<2);  //����
	GPIOB->MODER |= (0X0<<2);   //���ó�����ģʽ
	
	/* �ܽ�������ģʽ */ 
	GPIOB->PUPDR &= ~(0X3<<2);  //���ó���������-����
	
	//��У��ϵ����У����־��ȡ����----���ⲿ�洢����
			
	init_timer6();//��ʱɨ��T_PEN
		
	if(t_adjust_para.flag != T_ADJUST_OK)//û��У������У��
	{
			touch_adjust();
	}
	
}

/*
�������� return_xy_adval
���������� 
   u8 cmd ת������
��������ֵ;
   u16 date:ת���õ�������
�������ܣ�
  1����ģ�������䵽ADоƬ��
  2���ȴ�ADоƬת�����
  3����ADоƬ�ж�����������MCU��
*/
u16 return_xy_adval(u8 cmd)
{
	u16 date;
	//CS����
	T_CS_LOW;
	//��ģ�������䵽XTP2046��DIN�ܽ�
	send_touch_cmd(cmd);
	//�ȴ�ADת�����
	
	T_CLK_LOW;//�������ڡ�
	delay_us(1);
	T_CLK_HIGH;
	delay_us(1);
	
	//��ת���õ���������XTP2046��DOUT�ܽŶ���MCU��
	date = rev_touch_data();//16bit
	date >>= 4;
	//����Ƭѡ
	T_CS_HIGH;
	
	return date;
}

/*
��������T_MISO_cmd
��������;
u8 cmd �����������
��������ֵ;��
�������ܣ���ģ�������䵽ADоƬ��
*/
void send_touch_cmd(u8 cmd)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		
		if(cmd &(0x80 >> i) )//��׼������������
		{
			T_MOSI_HIGH;//����1
		}
		else
		{
			T_MOSI_LOW;//����0
		}
		T_CLK_LOW;//ʱ����׼��Ϊ��
		delay_us(1);
		T_CLK_HIGH;	//ʱ�Ӳ��������أ���������
		delay_us(1);
	}
}


/*
��������xtp2046_rev_data
������������
��������ֵ;
u16 date: ������������
�������ܣ���ADоƬ�ж�����������MCU��
*/
u16 rev_touch_data(void)
{
	u16 date =0;
	u8 i;
	
	for(i=0; i<16; i++)
	{
		delay_us(1);
		T_CLK_LOW;	
		delay_us(1);
		T_CLK_HIGH;//ʱ��������
		
		if (T_MISO)//���յ�1
		{
			date |= 0x8000>>i;
		}
		else //���յ�0
		{
			date &=~(0x8000>>i);
		}
	}
	return date;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

/*
��������get_xyad_aveval
�������ܣ��Դ���һ��ֵ���ж�β�������ƽ��ֵ
�Դ���ֵ���㣬�˲�
*/
u8 get_xyad_aveval(u16 *xvol,u16 *yvol)
{
	u16 x1,y1,x2,y2;
	
	x1 = return_xy_adval(X_CMD);
	y1 = return_xy_adval(Y_CMD);//��ɵ�һ�βɼ�	
	x2 = return_xy_adval(X_CMD);
	y2 = return_xy_adval(Y_CMD);//��ɵڶ��βɼ�	

	if (abs(x1 - x2) > 20 || abs(y1 - y2) > 20)    
	{
		return 0;//�����
	}

	*xvol = (x1 + x2) / 2;
	*yvol = (y1 + y2) / 2;//adcԭʼ����

	if (t_adjust_para.flag == T_ADJUST_OK)//����Ѿ�У׼�ˣ���ת������
	{
		*xvol = *xvol * t_adjust_para.xfac + t_adjust_para.xoff;
		*yvol = *yvol * t_adjust_para.yfac + t_adjust_para.yoff;//ת��LCD����ϵ
		
		*xvol = (*xvol >= LCD_WIDTH) ? LCD_WIDTH - 1 : *xvol;//��ֹ���LCD����ϵ
		*yvol = (*yvol >= LCD_HEIGHT) ? LCD_HEIGHT - 1 : *yvol;
	}
	printf("���º�x:%d\n,y:%d\n",*xvol,*yvol);
	return 1;
}


/*
��������touch_adjust
������������
��������ֵ����
�������ܣ�ʵ�ֶԴ����õ���������ֵУ׼
*/
void touch_adjust(void)
{
	u8 i;
	const u16 lcd_table[4][2]={{20,20},{300,20},{20,460},{300,460}};//LCD����ʵ���꣬ʮ�ֵ�����  оƬ�ڲ�����flash 1M
	u16 touch_table[4][2];//��Ű���ʮ�����ĵõ��Ĵ�������
	
	t_adjust_para.flag = 0;//û��У��
AA:	 
	//ѭ����ʾʮ�֣���ȡ4����������
	for(i=0; i<4; i++)
	{
		//��ʾʮ��
	lcd_dis_adjust_point(lcd_table[i][0],lcd_table[i][1], RED);
		while ( t_para.sta.loosen == 0 )//�ȴ����ֱ�־��1
		{
			delay_ms(10);//�ȴ����´�����
		}
		t_para.sta.loosen = 0;
		
		touch_table[i][0] = t_para.sx;
		touch_table[i][1] = t_para.sy;//��ȡ��Ӧ�Ĵ�����ԭʼ����
		
		printf("У������x:%d\r\n", t_para.sx);
		printf("У������y:%d\r\n", t_para.sy);
		lcd_dis_adjust_point(lcd_table[i][0],lcd_table[i][1],WHITE);//������һ��ʮ��
	}
	if (abs(touch_table[0][1] - touch_table[1][1]) > 500 //�жϴ��������λ�ã��Ƿ���ʮ�ּ��ϡ�
	 || abs(touch_table[2][1] - touch_table[3][1]) > 500
	 || abs(touch_table[0][0] - touch_table[2][0]) > 500
	 || abs(touch_table[1][0] - touch_table[3][0]) > 500)
	{
		goto AA;//û�е�����ȷλ�ã����»�ȡ����
	}

	//����У׼����
	t_adjust_para.xfac = (float)(lcd_table[0][0] - lcd_table[1][0]) 
							/ (touch_table[0][0] - touch_table[1][0]);
	t_adjust_para.xoff = ( LCD_WIDTH 
							- t_adjust_para.xfac * (touch_table[0][0] + touch_table[1][0])) / 2;
	
	t_adjust_para.yfac = (float)(lcd_table[2][1] - lcd_table[0][1]) 
							/ (touch_table[2][1] - touch_table[0][1]);
	t_adjust_para.yoff = ( LCD_HEIGHT
							- t_adjust_para.yfac * (touch_table[2][1] + touch_table[0][1])) / 2;
	
    t_adjust_para.flag = T_ADJUST_OK;
    
    //��У����ϵ����У����־��ŵ��ⲿ�洢����
    
	printf("t_adjust_para.xfac = %f\r\n",t_adjust_para.xfac);
	printf("t_adjust_para.xoff = %f\r\n",t_adjust_para.xoff);
	printf("t_adjust_para.yfac = %f\r\n",t_adjust_para.yfac);
	printf("t_adjust_para.yoff = %f\r\n",t_adjust_para.yoff);
	
	t_para.sta.hold = 0;
	t_para.sta.loosen = 0;//����״̬����
	t_para.sta.press = 0;
}
void init_timer6()
{
	RCC->APB1ENR |= 1<<4;//TIM6ʱ��ʹ��
	TIM6->PSC = 83;		//84��Ƶ  ������ʱ�ӣ�1MHZ
	TIM6->ARR = 800 - 1;     // ��ʱ 800uS
	
	TIM6->CR1 |= 1;		//������ʹ��
	
	TIM6->DIER |= 1;		//�����ж�ʹ��
	NVIC_EnableIRQ(TIM6_DAC_IRQn);//NVIC ��Ӧ���ж�ͨ��ʹ��
}
void TIM6_DAC_IRQHandler()
{
	static u8 t_pen_buf = 0XFF;//����T_PEN��ÿ��800us��״̬
	u16 tempx,tempy;
	
	if (TIM6->SR & 1)//800us
	{
		TIM6->SR &= ~1;
		
		t_pen_buf <<= 1;
		t_pen_buf |= T_PEN;
		
		if (t_pen_buf == 0)//ȷ���ȶ�����
		{
			if ( get_xyad_aveval(&tempx, &tempy) == 0)//��ȡ����
			{
				return ;
			}
			
			if (t_para.sta.hold == 0)//˵�������ϴ����ȶ��ɿ���״̬
			{
				t_para.sta.press = 1;//��ѹ��־00
				t_para.x = t_para.sx = tempx;//t_para.x----����ÿ�ΰ��µ�x����ֵ
				t_para.y = t_para.sy = tempy;
			}
			else
			{
				t_para.x = tempx;
				t_para.y = tempy;
			}
			
			t_para.sta.hold = 1;//�������±�־
		}
		else if (t_pen_buf == 0xff)//ȷ���ȶ��ɿ�
		{
			if (t_para.sta.hold == 1)
			{
				t_para.sta.loosen = 1;//���ֱ�־
			}
					
			t_para.sta.hold = 0;//�������±�־����
		}
	}
}
void lcd_dis_adjust_point(u16 x,u16 y,u16 color)
{
	display_line(x,y,30,0,color);
	display_line(x,y,30,180,color);
	display_line(x,y,30,90,color);
	display_line(x,y,30,270,color);
	//printf("x:%d,y:%d\n",x,y);

}










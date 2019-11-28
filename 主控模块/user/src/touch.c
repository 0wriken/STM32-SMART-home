#include "touch.h"

#include <stdlib.h>
#include <stdio.h>


T_ADJUST_TYPE t_adjust_para;//校准参数
T_DEV_TYPE t_para;//触屏状态&坐标


/*
函数名:init_touch_gpio
函数参数：无
函数返回值：无
函数功能：实现触摸屏芯片(XTP2046)的管脚初始化
XTP2046    LCD屏     MCUIO口
CLK       T_CLK      PB0 输出
CS        T_CS       PC13 输出
DIN       T_MOSI     PF11 输出
DOUT      T_MISO     PB2 输入
PEN       T_PEN      PB1 输入
*/
void init_touch_gpio(void)
{
	//打开PB PC PF时钟
	RCC->AHB1ENR |= (1<<1)|(1<<2)|(1<<5);
	
	//CLK--PB0 通用推挽 50M 
	/* 模式配置 */
	GPIOB->MODER &=~(0X3<<0);  //清零
	GPIOB->MODER |=(0X1<<0);  //配置成通用输出  
	
	/*输出类型配置 */
	GPIOB->OTYPER &=~(1<<0);  //配置成输出推挽
	
	/* 输出速率配置 */
	GPIOB->OSPEEDR &=~(0X3<<0);  //清零
	GPIOB->OSPEEDR |=(0X2<<0);   //配置成输出速率50M
	
	//CS--PC13 通用推挽 50M 
	/* 模式配置 */
	GPIOC->MODER &= ~(0X3<<26);  //清零
	GPIOC->MODER |= (0X1<<26);  //配置成通用输出  
	
	/*输出类型配置 */
	GPIOC->OTYPER &= ~(1<<13);  //配置成输出推挽
	
	/* 输出速率配置 */
	GPIOC->OSPEEDR &=~(0X3<<26);  //清零
	GPIOC->OSPEEDR |=(0X2<<26);   //配置成输出速率50M
	
	//DIN--MOSI PF11 通用推挽 50M 
	/* 模式配置 */
	GPIOF->MODER &= ~(0X3<<22);  //清零
	GPIOF->MODER |= (0X1<<22);  //配置成通用输出  
	
	/*输出类型配置 */
	GPIOF->OTYPER &= ~(1<<11);  //配置成输出推挽
	
	/* 输出速率配置 */
	GPIOF->OSPEEDR &= ~(0X3<<22);  //清零
	GPIOF->OSPEEDR |= (0X2<<22);   //配置成输出速率50M
	
	/* DOUT--T_MISO--PB2 输入 */
	/* 管脚模式 */
	GPIOB->MODER &= ~(0X3<<4);  //清零
	GPIOB->MODER |= (0X0<<4);   //配置成输入模式
	
	/* 管脚上下拉模式 */ 
	GPIOB->PUPDR &=~(0X3<<4);  //配置成无上下拉-浮空
	
	/* PEN--T_PEN--PB1 输入 */
	/* 管脚模式 */
	GPIOB->MODER &= ~(0X3<<2);  //清零
	GPIOB->MODER |= (0X0<<2);   //配置成输入模式
	
	/* 管脚上下拉模式 */ 
	GPIOB->PUPDR &= ~(0X3<<2);  //配置成无上下拉-浮空
	
	//将校正系数、校正标志读取出来----从外部存储器中
			
	init_timer6();//定时扫描T_PEN
		
	if(t_adjust_para.flag != T_ADJUST_OK)//没有校正，则校正
	{
			touch_adjust();
	}
	
}

/*
函数名： return_xy_adval
函数参数： 
   u8 cmd 转换命令
函数返回值;
   u16 date:转换好的数字量
函数功能：
  1、把模拟量传输到AD芯片中
  2、等待AD芯片转换完成
  3、从AD芯片中读出数字量到MCU中
*/
u16 return_xy_adval(u8 cmd)
{
	u16 date;
	//CS拉低
	T_CS_LOW;
	//把模拟量传输到XTP2046的DIN管脚
	send_touch_cmd(cmd);
	//等待AD转换完成
	
	T_CLK_LOW;//空闲周期。
	delay_us(1);
	T_CLK_HIGH;
	delay_us(1);
	
	//把转换好的数字量从XTP2046的DOUT管脚读到MCU中
	date = rev_touch_data();//16bit
	date >>= 4;
	//拉高片选
	T_CS_HIGH;
	
	return date;
}

/*
函数名：T_MISO_cmd
函数参数;
u8 cmd ：传入的命令
函数返回值;无
函数功能：把模拟量传输到AD芯片中
*/
void send_touch_cmd(u8 cmd)
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		
		if(cmd &(0x80 >> i) )//先准备数据线数据
		{
			T_MOSI_HIGH;//发送1
		}
		else
		{
			T_MOSI_LOW;//发送0
		}
		T_CLK_LOW;//时钟线准备为低
		delay_us(1);
		T_CLK_HIGH;	//时钟产生上升沿，发出数据
		delay_us(1);
	}
}


/*
函数名：xtp2046_rev_data
函数参数：无
函数返回值;
u16 date: 读到的数字量
函数功能：从AD芯片中读出数字量到MCU中
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
		T_CLK_HIGH;//时钟上升沿
		
		if (T_MISO)//接收到1
		{
			date |= 0x8000>>i;
		}
		else //接收到0
		{
			date &=~(0x8000>>i);
		}
	}
	return date;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

/*
函数名：get_xyad_aveval
函数功能：对触摸一次值进行多次采样，求平均值
对触摸值计算，滤波
*/
u8 get_xyad_aveval(u16 *xvol,u16 *yvol)
{
	u16 x1,y1,x2,y2;
	
	x1 = return_xy_adval(X_CMD);
	y1 = return_xy_adval(Y_CMD);//完成第一次采集	
	x2 = return_xy_adval(X_CMD);
	y2 = return_xy_adval(Y_CMD);//完成第二次采集	

	if (abs(x1 - x2) > 20 || abs(y1 - y2) > 20)    
	{
		return 0;//错误点
	}

	*xvol = (x1 + x2) / 2;
	*yvol = (y1 + y2) / 2;//adc原始数据

	if (t_adjust_para.flag == T_ADJUST_OK)//如果已经校准了，则转换坐标
	{
		*xvol = *xvol * t_adjust_para.xfac + t_adjust_para.xoff;
		*yvol = *yvol * t_adjust_para.yfac + t_adjust_para.yoff;//转到LCD坐标系
		
		*xvol = (*xvol >= LCD_WIDTH) ? LCD_WIDTH - 1 : *xvol;//防止溢出LCD坐标系
		*yvol = (*yvol >= LCD_HEIGHT) ? LCD_HEIGHT - 1 : *yvol;
	}
	printf("按下后x:%d\n,y:%d\n",*xvol,*yvol);
	return 1;
}


/*
函数名：touch_adjust
函数参数：无
函数返回值：无
函数功能：实现对触摸得到的坐标数值校准
*/
void touch_adjust(void)
{
	u8 i;
	const u16 lcd_table[4][2]={{20,20},{300,20},{20,460},{300,460}};//LCD屏真实坐标，十字的中心  芯片内部闪存flash 1M
	u16 touch_table[4][2];//存放按下十字中心得到的触摸坐标
	
	t_adjust_para.flag = 0;//没有校正
AA:	 
	//循环显示十字，获取4个触摸坐标
	for(i=0; i<4; i++)
	{
		//显示十字
	lcd_dis_adjust_point(lcd_table[i][0],lcd_table[i][1], RED);
		while ( t_para.sta.loosen == 0 )//等待松手标志置1
		{
			delay_ms(10);//等待按下触摸屏
		}
		t_para.sta.loosen = 0;
		
		touch_table[i][0] = t_para.sx;
		touch_table[i][1] = t_para.sy;//读取对应的触摸屏原始坐标
		
		printf("校正触摸x:%d\r\n", t_para.sx);
		printf("校正触摸y:%d\r\n", t_para.sy);
		lcd_dis_adjust_point(lcd_table[i][0],lcd_table[i][1],WHITE);//消除上一个十字
	}
	if (abs(touch_table[0][1] - touch_table[1][1]) > 500 //判断触摸屏点的位置，是否在十字架上。
	 || abs(touch_table[2][1] - touch_table[3][1]) > 500
	 || abs(touch_table[0][0] - touch_table[2][0]) > 500
	 || abs(touch_table[1][0] - touch_table[3][0]) > 500)
	{
		goto AA;//没有点中正确位置，重新获取坐标
	}

	//计算校准参数
	t_adjust_para.xfac = (float)(lcd_table[0][0] - lcd_table[1][0]) 
							/ (touch_table[0][0] - touch_table[1][0]);
	t_adjust_para.xoff = ( LCD_WIDTH 
							- t_adjust_para.xfac * (touch_table[0][0] + touch_table[1][0])) / 2;
	
	t_adjust_para.yfac = (float)(lcd_table[2][1] - lcd_table[0][1]) 
							/ (touch_table[2][1] - touch_table[0][1]);
	t_adjust_para.yoff = ( LCD_HEIGHT
							- t_adjust_para.yfac * (touch_table[2][1] + touch_table[0][1])) / 2;
	
    t_adjust_para.flag = T_ADJUST_OK;
    
    //将校正的系数、校正标志存放到外部存储器中
    
	printf("t_adjust_para.xfac = %f\r\n",t_adjust_para.xfac);
	printf("t_adjust_para.xoff = %f\r\n",t_adjust_para.xoff);
	printf("t_adjust_para.yfac = %f\r\n",t_adjust_para.yfac);
	printf("t_adjust_para.yoff = %f\r\n",t_adjust_para.yoff);
	
	t_para.sta.hold = 0;
	t_para.sta.loosen = 0;//触屏状态清零
	t_para.sta.press = 0;
}
void init_timer6()
{
	RCC->APB1ENR |= 1<<4;//TIM6时钟使能
	TIM6->PSC = 83;		//84分频  计数器时钟：1MHZ
	TIM6->ARR = 800 - 1;     // 定时 800uS
	
	TIM6->CR1 |= 1;		//计数器使能
	
	TIM6->DIER |= 1;		//更新中断使能
	NVIC_EnableIRQ(TIM6_DAC_IRQn);//NVIC 对应的中断通道使能
}
void TIM6_DAC_IRQHandler()
{
	static u8 t_pen_buf = 0XFF;//保存T_PEN，每过800us的状态
	u16 tempx,tempy;
	
	if (TIM6->SR & 1)//800us
	{
		TIM6->SR &= ~1;
		
		t_pen_buf <<= 1;
		t_pen_buf |= T_PEN;
		
		if (t_pen_buf == 0)//确认稳定按下
		{
			if ( get_xyad_aveval(&tempx, &tempy) == 0)//读取坐标
			{
				return ;
			}
			
			if (t_para.sta.hold == 0)//说明触屏上次是稳定松开的状态
			{
				t_para.sta.press = 1;//下压标志00
				t_para.x = t_para.sx = tempx;//t_para.x----保存每次按下的x坐标值
				t_para.y = t_para.sy = tempy;
			}
			else
			{
				t_para.x = tempx;
				t_para.y = tempy;
			}
			
			t_para.sta.hold = 1;//触屏按下标志
		}
		else if (t_pen_buf == 0xff)//确认稳定松开
		{
			if (t_para.sta.hold == 1)
			{
				t_para.sta.loosen = 1;//松手标志
			}
					
			t_para.sta.hold = 0;//触屏按下标志清零
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










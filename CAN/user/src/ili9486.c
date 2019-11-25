#include "ILI9486.h"

/******************** LCD引脚初始化函数 *******************
LCD显示屏硬件管脚对应关系：
数据线：
DB0---PD14	DB1---PD15	DB2---PD0	DB3---PD1
DB4---PE7	DB5---PE8	DB6---PE9	DB7---PE10
DB8---PE11	DB9---PE12	DB10---PE13	DB11---PE14
DB12---PE15	DB13---PD8	DB14---PD9	DB15---PD10
控制线：
WR---PD5	RD---PD4	CS---PG12	RS---PF12	BL--PB15
**********************************************************/
void LCD_Port_Init(void)
{
	/* 使能LCD硬件管脚时钟 */
	RCC->AHB1ENR |= (1<<3);//开PD
	RCC->AHB1ENR |= (1<<4);//开PE
	RCC->AHB1ENR |= (1<<6);//开PG
	RCC->AHB1ENR |= (1<<1);//开PB
	RCC->AHB1ENR |= (1<<5);//开PF
	
	
	/* 设置LCD硬件管脚模式 */
	/* DB0~DB15数据线：推挽输出  时钟50M */
	GPIOD->MODER |=(0X1<<(2*14))|(0X1<<(2*15));   //配置成通用输出  
	GPIOD->OSPEEDR |=(0X2<<(2*14))|(0X2ul<<(2*15)); //配置成输出速率50M 		
	
	GPIOD->MODER |=(0X1<<(2*0))|(0X1<<(2*1));     //配置成通用输出  
	GPIOD->OSPEEDR |=(0X2<<(2*0))|(0X2<<(2*1));   //配置成输出速率50M
	
	GPIOE->MODER |=0x55554000;                    //配置成通用输出  
	GPIOE->OTYPER |= 0;                           //配置成输出推挽
	GPIOE->OSPEEDR |=0x55554000;                  //配置成输出速率50M

	GPIOD->MODER |=(0X1<<(2*8))|(0X1<<(2*9))|(0X1<<(2*10));  //配置成通用输出  
	GPIOD->OSPEEDR |=(0X2<<(2*1))|(0X2<<(2*1))|(0X2<<(2*1));   //配置成输出速率50M
	
	/* 控制线：推挽输出  时钟50M 空闲为高电平 */
	GPIOD->MODER |=(0X1<<(2*4))|(0X1<<(2*5));  //配置成通用输出  
	GPIOD->OSPEEDR |=(0X2<<(2*4))|(0X2<<(2*5));
	GPIOD->ODR |= (1<<4)|(1<<5);
	
	GPIOG->MODER |=(0X1<<(2*12));              //配置成通用输出  
	GPIOG->OSPEEDR |=(0X2<<(2*12));
	GPIOG->ODR |= (1<<12);

	GPIOF->MODER |=(0X1<<(2*12));              //配置成通用输出  
	GPIOF->OSPEEDR |=(0X2<<(2*12));
	GPIOF->ODR |= (1<<12);
	
	GPIOB->MODER |=(0X1<<(2*15));               //配置成通用输出  
	GPIOB->OSPEEDR |=(0X2ul<<(2*15));
	GPIOB->ODR &= ~(1<<15);                     //关闭背光，在LCD初始化完后再打开
}

/******************* LCD写命令函数 *********************************
参数：需要输入的命令
********************************************************************/
void LCD_ILI9486_CMD(u16 common)
{
	GPIOG->ODR &= ~(1 << 12);    //CS拉低
	GPIOF->ODR &= ~(1 << 12);    //D/C拉低
	GPIOD->ODR &= ~(1 << 5);     //WR拉低
	//LCD_Output_Data(cmd);        //输出命令数据
	/* 把命令发送到数据总线 */
	if(common&0x01)   GPIOD->BSRRL = (1<<14); else GPIOD->BSRRH = (1<<14);
	if(common&0x02)   GPIOD->BSRRL = (1<<15); else GPIOD->BSRRH = (1<<15);
	if(common&0x04)   GPIOD->BSRRL = (1<<0);  else GPIOD->BSRRH = (1<<0);
	if(common&0x08)   GPIOD->BSRRL = (1<<1);  else GPIOD->BSRRH = (1<<1);
	if(common&0x10)   GPIOE->BSRRL = (1<<7);  else GPIOE->BSRRH = (1<<7);
	if(common&0x20)   GPIOE->BSRRL = (1<<8);  else GPIOE->BSRRH = (1<<8);
	if(common&0x40)   GPIOE->BSRRL = (1<<9);  else GPIOE->BSRRH = (1<<9);
	if(common&0x80)   GPIOE->BSRRL = (1<<10); else GPIOE->BSRRH = (1<<10);
	if(common&0x0100) GPIOE->BSRRL = (1<<11); else GPIOE->BSRRH = (1<<11);
	if(common&0x0200) GPIOE->BSRRL = (1<<12); else GPIOE->BSRRH = (1<<12);
	if(common&0x0400) GPIOE->BSRRL = (1<<13); else GPIOE->BSRRH = (1<<13);
	if(common&0x0800) GPIOE->BSRRL = (1<<14); else GPIOE->BSRRH = (1<<14);
	if(common&0x1000) GPIOE->BSRRL = (1<<15); else GPIOE->BSRRH = (1<<15);
	if(common&0x2000) GPIOD->BSRRL = (1<<8);  else GPIOD->BSRRH = (1<<8);
	if(common&0x4000) GPIOD->BSRRL = (1<<9);  else GPIOD->BSRRH = (1<<9);
	if(common&0x8000) GPIOD->BSRRL = (1<<10); else GPIOD->BSRRH = (1<<10);
	GPIOD->ODR |= 1 << 5;        //WR拉高
	GPIOG->ODR |= 1 << 12;       //拉高CS
}

/******************* LCD写数据函数 *********************************
参数：需要输入的命令
********************************************************************/
void LCD_ILI9486_Parameter(u16 data)
{
	GPIOG->ODR &= ~(1 << 12);    //CS拉低
	GPIOF->ODR |= 1 << 12;       //D/C拉高
	GPIOD->ODR &= ~(1 << 5);     //WR拉低
	//LCD_Output_Data(data);        //输出命令数据
	/* 把数据发送到数据总线 */
	if(data&0x01)   GPIOD->BSRRL = (1<<14); else GPIOD->BSRRH = (1<<14);
	if(data&0x02)   GPIOD->BSRRL = (1<<15); else GPIOD->BSRRH = (1<<15);
	if(data&0x04)   GPIOD->BSRRL = (1<<0);  else GPIOD->BSRRH = (1<<0);
	if(data&0x08)   GPIOD->BSRRL = (1<<1);  else GPIOD->BSRRH = (1<<1);
	if(data&0x10)   GPIOE->BSRRL = (1<<7);  else GPIOE->BSRRH = (1<<7);
	if(data&0x20)   GPIOE->BSRRL = (1<<8);  else GPIOE->BSRRH = (1<<8);
	if(data&0x40)   GPIOE->BSRRL = (1<<9);  else GPIOE->BSRRH = (1<<9);
	if(data&0x80)   GPIOE->BSRRL = (1<<10); else GPIOE->BSRRH = (1<<10);
	if(data&0x0100) GPIOE->BSRRL = (1<<11); else GPIOE->BSRRH = (1<<11);
	if(data&0x0200) GPIOE->BSRRL = (1<<12); else GPIOE->BSRRH = (1<<12);
	if(data&0x0400) GPIOE->BSRRL = (1<<13); else GPIOE->BSRRH = (1<<13);
	if(data&0x0800) GPIOE->BSRRL = (1<<14); else GPIOE->BSRRH = (1<<14);
	if(data&0x1000) GPIOE->BSRRL = (1<<15); else GPIOE->BSRRH = (1<<15);
	if(data&0x2000) GPIOD->BSRRL = (1<<8);  else GPIOD->BSRRH = (1<<8);
	if(data&0x4000) GPIOD->BSRRL = (1<<9);  else GPIOD->BSRRH = (1<<9);
	if(data&0x8000) GPIOD->BSRRL = (1<<10); else GPIOD->BSRRH = (1<<10);
	GPIOD->ODR |= 1 << 5;        //WR拉高
	GPIOG->ODR |= 1 << 12;       //拉高CS
}

/********************** LCD清屏函数 ******************************************
参数：
SX、EX：X轴的起始坐标和结束坐标
SY、EY：Y轴的起始坐标和结束坐标
Color： 屏幕颜色
******************************************************************************/
void LCD_Clear(u16 SX, u16 EX, u16 SY, u16 EY, u16 Color)
{
	u32 i;
	
	LCD_ILI9486_CMD(0x2A);                      //发送0x2A命令，设置X轴坐标
	LCD_ILI9486_Parameter((SX & 0xff00) >> 8);  //发送X轴起始坐标的高8位
	LCD_ILI9486_Parameter((SX & 0x00ff));       //发送X轴起始坐标的低8位
	LCD_ILI9486_Parameter((EX & 0xff00) >> 8);  //发送X轴结束坐标的高8位
	LCD_ILI9486_Parameter((EX & 0x00ff));       //发送X轴结束坐标的低8位
	
	LCD_ILI9486_CMD(0x2B);                      //发送0x2B命令，设置Y轴坐标
	LCD_ILI9486_Parameter((SY & 0xff00) >> 8);  //发送Y轴起始坐标的高8位
	LCD_ILI9486_Parameter((SY & 0x00ff));       //发送Y轴起始坐标的低8位
	LCD_ILI9486_Parameter((EY & 0xff00) >> 8);  //发送Y轴结束坐标的高8位
	LCD_ILI9486_Parameter((EY & 0x00ff));       //发送Y轴结束坐标的低8位
	
	LCD_ILI9486_CMD(0x2C);                      //发送0x2A命令，设置LCD显示颜色
	for(i = 0; i < (EX - SX + 1) * (EY - SY + 1); i++)  //循环清屏区域的每一个像素点
	{
		LCD_ILI9486_Parameter(Color);
	}
}

/********************** LCD初始化函数 *******************************************/
void LCD_Init(void)
{
	LCD_Port_Init();
	//************* Reset LCD Driver ****************//
	delay_ms(120); // Delay 120 ms
	//************* Start Initial Sequence **********//
	LCD_ILI9486_CMD(0XF2);
	LCD_ILI9486_Parameter(0x18);
	LCD_ILI9486_Parameter(0xA3);
	LCD_ILI9486_Parameter(0x12);
	LCD_ILI9486_Parameter(0x02);
	LCD_ILI9486_Parameter(0XB2);
	LCD_ILI9486_Parameter(0x12);
	LCD_ILI9486_Parameter(0xFF);
	LCD_ILI9486_Parameter(0x10);
	LCD_ILI9486_Parameter(0x00);
	LCD_ILI9486_CMD(0XF8);
	LCD_ILI9486_Parameter(0x21);
	LCD_ILI9486_Parameter(0x04);
	LCD_ILI9486_CMD(0XF9);
	LCD_ILI9486_Parameter(0x00);
	LCD_ILI9486_Parameter(0x08);
	LCD_ILI9486_CMD(0x36);
	LCD_ILI9486_Parameter(0x08);   //设置RGB,含排线的屏
//	LCD_ILI9486_Parameter(0x00);   //设置RGB，不含排线的屏
	LCD_ILI9486_CMD(0x3A);
	LCD_ILI9486_Parameter(0x05);   //设置16位BPP
	LCD_ILI9486_CMD(0xB4);
	LCD_ILI9486_Parameter(0x01);//0x00
	LCD_ILI9486_CMD(0xB6);
	LCD_ILI9486_Parameter(0x02);
	LCD_ILI9486_Parameter(0x22);
	LCD_ILI9486_CMD(0xC1);
	LCD_ILI9486_Parameter(0x41);
	LCD_ILI9486_CMD(0xC5);
	LCD_ILI9486_Parameter(0x00);
	LCD_ILI9486_Parameter(0x07);//0X18
	LCD_ILI9486_CMD(0xE0);
	LCD_ILI9486_Parameter(0x0F);
	LCD_ILI9486_Parameter(0x1F);
	LCD_ILI9486_Parameter(0x1C);
	LCD_ILI9486_Parameter(0x0C);
	LCD_ILI9486_Parameter(0x0F);
	LCD_ILI9486_Parameter(0x08);
	LCD_ILI9486_Parameter(0x48);
	LCD_ILI9486_Parameter(0x98);
	LCD_ILI9486_Parameter(0x37);
	LCD_ILI9486_Parameter(0x0A);
	LCD_ILI9486_Parameter(0x13);
	LCD_ILI9486_Parameter(0x04);
	LCD_ILI9486_Parameter(0x11);
	LCD_ILI9486_Parameter(0x0D);
	LCD_ILI9486_Parameter(0x00);
	LCD_ILI9486_CMD(0xE1);
	LCD_ILI9486_Parameter(0x0F);
	LCD_ILI9486_Parameter(0x32);
	LCD_ILI9486_Parameter(0x2E);
	LCD_ILI9486_Parameter(0x0B);
	LCD_ILI9486_Parameter(0x0D);
	LCD_ILI9486_Parameter(0x05);
	LCD_ILI9486_Parameter(0x47);
	LCD_ILI9486_Parameter(0x75);
	LCD_ILI9486_Parameter(0x37);
	LCD_ILI9486_Parameter(0x06);
	LCD_ILI9486_Parameter(0x10);
	LCD_ILI9486_Parameter(0x03);
	LCD_ILI9486_Parameter(0x24);
	LCD_ILI9486_Parameter(0x20);
	LCD_ILI9486_Parameter(0x00);
	LCD_ILI9486_CMD(0x11);   //退出睡眠
	delay_ms(120);
	LCD_ILI9486_CMD(0x29);   //开启显示
	
	/* 以下由用户按实际编写 */
	LCD_Clear(0,319,0,479,0xffff);  //把屏幕清成白色
	GPIOB->ODR |= 1 << 15;          //开LCD背光灯
}
void LCD_display_pic(u16 X,u16 Y,u8 *pic_data)
{
	u32 i;
	u16 w = 0, h = 0;
	
	w = (pic_data[2] << 8) | pic_data[3];
	h = (pic_data[4] << 8) | pic_data[5];
	
	LCD_ILI9486_CMD(0x2A);                      //发送0x2A命令，设置X轴坐标
	LCD_ILI9486_Parameter((X & 0xff00) >> 8);  //发送X轴起始坐标的高8位
	LCD_ILI9486_Parameter((X & 0x00ff));       //发送X轴起始坐标的低8位
	LCD_ILI9486_Parameter(((X + w - 1) & 0xff00) >> 8);  //发送X轴结束坐标的高8位
	LCD_ILI9486_Parameter(((X + w - 1) & 0x00ff));       //发送X轴结束坐标的低8位
	
	LCD_ILI9486_CMD(0x2B);                      //发送0x2B命令，设置Y轴坐标
	LCD_ILI9486_Parameter((Y & 0xff00) >> 8);  //发送Y轴起始坐标的高8位
	LCD_ILI9486_Parameter((Y & 0x00ff));       //发送Y轴起始坐标的低8位
	LCD_ILI9486_Parameter(((Y + h - 1) & 0xff00) >> 8);  //发送Y轴结束坐标的高8位
	LCD_ILI9486_Parameter(((Y + h - 1) & 0x00ff));       //发送Y轴结束坐标的低8位
	
	LCD_ILI9486_CMD(0x2C);                      //发送0x2A命令，设置LCD显示颜色
	for(i = 0; i < w * h; i++)  //循环清屏区域的每一个像素点
	{
		LCD_ILI9486_Parameter((pic_data[2 * i + 8] << 8) | pic_data[2 * i + 1 + 8]);
	}
}


void dispaly_point(u16 x,u16 y,u16 color)
{
		LCD_ILI9486_CMD(0x2A);                      //发送0x2A命令，设置X轴坐标
	LCD_ILI9486_Parameter((x & 0xff00) >> 8);  //发送X轴起始坐标的高8位
	LCD_ILI9486_Parameter((x & 0x00ff));       //发送X轴起始坐标的低8位
	LCD_ILI9486_Parameter((x & 0xff00) >> 8);  //发送X轴结束坐标的高8位
	LCD_ILI9486_Parameter((x & 0x00ff));       //发送X轴结束坐标的低8位
	
	LCD_ILI9486_CMD(0x2B);                      //发送0x2B命令，设置Y轴坐标
	LCD_ILI9486_Parameter((y & 0xff00) >> 8);  //发送Y轴起始坐标的高8位
	LCD_ILI9486_Parameter((y & 0x00ff));       //发送Y轴起始坐标的低8位
	LCD_ILI9486_Parameter((y & 0xff00) >> 8);  //发送Y轴结束坐标的高8位
	LCD_ILI9486_Parameter((y & 0x00ff));       //发送Y轴结束坐标的低8位
	
	LCD_ILI9486_CMD(0x2C); 
	LCD_ILI9486_Parameter(color);	     //发送y轴起始坐标的低8位
}
void display_line(u16 x,u16 y,u16 lenth,u16 angle,u16 color)
{
	u16 nx=0;
	u16 ny=0;
	u16 i=0;
	if(angle==0) //水平线开始
	{
		for(i=0;i<lenth;i++)
		{
			dispaly_point(x+i,y,color);
		}
	}
	if(angle==90)
	{
		for(i=0;i<lenth;i++)
		{
			dispaly_point(x,y+i,color);
		}
	}
	if(angle==180)
	{
			for(i=0;i<lenth;i++)
		{
			dispaly_point(x-i,y,color);
		}
	}
	if(angle==270)
	{
		for(i=0;i<lenth;i++)
		{
			dispaly_point(x,y-i,color);
		}
	}
	//任意角度
	else 
	{
		for(i=0;i<lenth;i++)
		{
			nx=x+cos(angle*3.14/180);
			ny=y-sin(angle*3.14/180);
			dispaly_point(nx,ny,color);
		}
	}
}
void lcd_dis_txt(u16 x,u16 y,u16 t_color,u16 b_color,u8*txt_buf)
{

	u16 txt_data=0;
	for(u8 i=0;i<16;i++)
	{
		txt_data=(txt_buf[2*i]<<8|txt_buf[2*i]);
		for(u8 j=0;j<16;j++)
		{
			if(txt_data&(0x8000>>j))
				dispaly_point(x+j,y+i,t_color);
			else 
				dispaly_point(x+j,y+i,b_color);
		}
	}
}
void play_string(u16 x,u16 y,u16 t_color,u16 b_color,u8*buff)
{
		u8 txt_data=0;
	for(u8 i=0;i<16;i++)
	{
		txt_data=buff[i];
		for(u8 j=0;j<8;j++)
		{
			if(txt_data&(0x80>>j))
				dispaly_point(x+j,y+i,t_color);
			else 
				dispaly_point(x+j,y+i,b_color);
		}
	}
}






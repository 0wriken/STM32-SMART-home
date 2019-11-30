#include "monochome_oled.h"
#include "oled_memory.h"


/*
**********************************************************************************
* 函数功能: OLED屏接口初始化
* 函数形参: None
* 函数返回值: None
* 备注: None
**********************************************************************************
*/
void oled_port_init(void)
{
#ifdef OLED_GPIO_AFIO_EN
	OLED_GPIO_AFIO_RCC  |= 0x1 << OLED_GPIO_AFIO_RCCX; 
	OLED_GPIO_AFIO->MAPR &= ~(0x7 << OLED_GPIO_AFIO_BIT);
	OLED_GPIO_AFIO->MAPR |= 0x2 << OLED_GPIO_AFIO_BIT;
#endif
	
	/*开启OLED屏RES的接口时钟*/
	OLED_RES_RCC_GPIO |= (0X1u << OLED_RES_RCC_GPIOX);
	/*配置OLED屏RES的接口模式*/
	if(OLED_RES_GPIOX_PIN >= 8)
	{
		OLED_RES_GPIOX->CRH &= ~(0XFu << (OLED_RES_GPIOX_PORT * 4));
		OLED_RES_GPIOX->CRH |=  (0X3u << (OLED_RES_GPIOX_PORT * 4));
	}
	else
	{
		OLED_RES_GPIOX->CRL &= ~(0XFu << (OLED_RES_GPIOX_PORT * 4));
		OLED_RES_GPIOX->CRL |=  (0X3u << (OLED_RES_GPIOX_PORT * 4));
	}
	/*OLED屏RES初始化状态为高*/
	OLED_RES_HIGH();
	
	/*开启OLED屏CS的接口时钟*/
	OLED_CS_RCC_GPIO |= (0X1u << OLED_CS_RCC_GPIOX);
	/*配置OLED屏CS的接口模式*/
	if(OLED_CS_GPIOX_PIN >= 8)
	{
		OLED_CS_GPIOX->CRH &= ~(0XFu << (OLED_CS_GPIOX_PORT * 4));
		OLED_CS_GPIOX->CRH |=  (0X3u << (OLED_CS_GPIOX_PORT * 4));
	}
	else
	{
		OLED_CS_GPIOX->CRL &= ~(0XFu << (OLED_CS_GPIOX_PORT * 4));
		OLED_CS_GPIOX->CRL |=  (0X3u << (OLED_CS_GPIOX_PORT * 4));
	}
	/*OLED屏CS初始化状态为高*/
	OLED_CS_HIGH();
	
	/*开启OLED屏DC的接口时钟*/
	OLED_DC_RCC_GPIO |= (0X1u << OLED_DC_RCC_GPIOX);
	/*配置OLED屏DC的接口模式*/
	if(OLED_DC_GPIOX_PIN >= 8)
	{
		OLED_DC_GPIOX->CRH &= ~(0XFu << (OLED_DC_GPIOX_PORT * 4));
		OLED_DC_GPIOX->CRH |=  (0X3u << (OLED_DC_GPIOX_PORT * 4));
	}
	else
	{
		OLED_DC_GPIOX->CRL &= ~(0XFu << (OLED_DC_GPIOX_PORT * 4));
		OLED_DC_GPIOX->CRL |=  (0X3u << (OLED_DC_GPIOX_PORT * 4));
	}
	/*OLED屏DC初始化状态为低*/
	OLED_DC_LOW();
	
	/*开启OLED屏CLK的接口时钟*/
	OLED_CLK_RCC_GPIO |= (0X1u << OLED_CLK_RCC_GPIOX);
	/*配置OLED屏CLK的接口模式*/
	if(OLED_CLK_GPIOX_PIN >= 8)
	{
		OLED_CLK_GPIOX->CRH &= ~(0XFu << (OLED_CLK_GPIOX_PORT * 4));
		OLED_CLK_GPIOX->CRH |=  (0XBu << (OLED_CLK_GPIOX_PORT * 4));
	}
	else
	{
		OLED_CLK_GPIOX->CRL &= ~(0XFu << (OLED_CLK_GPIOX_PORT * 4));
		OLED_CLK_GPIOX->CRL |=  (0XBu << (OLED_CLK_GPIOX_PORT * 4));
	}
	
	/*开启OLED屏DIN的接口时钟*/
	OLED_DIN_RCC_GPIO |= (0X1u << OLED_DIN_RCC_GPIOX);
	/*配置OLED屏DIN的接口模式*/
	if(OLED_DIN_GPIOX_PIN >= 8)
	{
		OLED_DIN_GPIOX->CRH &= ~(0XFu << (OLED_DIN_GPIOX_PORT * 4));
		OLED_DIN_GPIOX->CRH |=  (0XBu << (OLED_DIN_GPIOX_PORT * 4));
	}
	else
	{
		OLED_DIN_GPIOX->CRL &= ~(0XFu << (OLED_DIN_GPIOX_PORT * 4));
		OLED_DIN_GPIOX->CRL |=  (0XBu << (OLED_DIN_GPIOX_PORT * 4));
	}
}



/*
**********************************************************************************
* 函数功能: OLED屏SPI外设初始化
* 函数形参: None
* 函数返回值: None
* 备注: None
**********************************************************************************
*/
void oled_spi_init(void)
{
	
#ifdef OLED_SPI_AFIO_EN
	OLED_SPI_AFIO->MAPR |= 0X1 << OLED_SPI_AFIO_BIT;   
#endif
	
	/* 开启OLED屏SPI外设时钟 */
	OLED_RCC_SPIX |= 0X1 << OLED_RCC_SPIX_BIT;
	
	/* 清空OLED屏SPI外设控制寄存器1 */
	OLED_SPIX->CR1 = 0X0;	
	
	/* 配置OLED屏SPI外设控制寄存器1 */
	OLED_SPIX->CR1 |= 0X3 << 8;		//配置软件从器件管理，不选择片选
	OLED_SPIX->CR1 |= 0X1 << 6;		//使能SPI控制器
	OLED_SPIX->CR1 |= OLED_SPIX_BAUD_RATE << 3;		//设置SPI总线速度
	OLED_SPIX->CR1 |= 0X1 << 2;		//MCU的SPI控制器为主设备
	
	switch(OLED_SPIX_MODE)
	{
		case OLED_SPIX_MODE_ZERO:
			OLED_SPIX->CR1 |= OLED_SPIX_MODE_ZERO;
			break;
		case OLED_SPIX_MODE_ONE:
			OLED_SPIX->CR1 |= OLED_SPIX_MODE_ONE;
			break;
		case OLED_SPIX_MODE_TWO:
			OLED_SPIX->CR1 |= OLED_SPIX_MODE_TWO;
			break;
		case OLED_SPIX_MODE_THREE:
			OLED_SPIX->CR1 |= OLED_SPIX_MODE_THREE;
			break;
	}
}


/*
**********************************************************************************
* 函数功能: OLED屏SPI外设读写字节数据
* 函数形参: wbyte    写入的数据
* 函数返回值: SPI外设接收的数据
* 备注: None
**********************************************************************************
*/
uint8_t oled_spi_read_write_byte(uint8_t wbyte)
{
	while(!(OLED_SPIX->SR & (0X1 << 1)))	//等待发送缓冲区空
	{
		
	}
	OLED_SPIX->DR = wbyte;
	
	while(!(OLED_SPIX->SR & (0X1 << 0)))	//等待接收缓冲区非空
	{
	
	}
	return OLED_SPIX->DR;
}


/*
**********************************************************************************
* 函数功能: OLED屏SPI外设发送命令
* 函数形参: command    写入的命令
* 函数返回值: None
* 备注: None
**********************************************************************************
*/
void oled_spi_write_command(uint8_t command)
{
	OLED_CS_LOW();	//选择OLED屏工作
	OLED_DC_LOW();	//选择当前是命令模式
	
	oled_spi_read_write_byte(command);
	
	OLED_CS_HIGH();	//不选择OLED屏工作
}


/*
**********************************************************************************
* 函数功能: OLED屏SPI外设发送数据
* 函数形参: ram_data    写入的数据
* 函数返回值: None
* 备注: None
**********************************************************************************
*/
void oled_spi_write_data(uint8_t ram_data)
{
	OLED_CS_LOW();	//选择OLED屏工作
	OLED_DC_HIGH();	//选择当前是数据模式
	
	oled_spi_read_write_byte(ram_data);
	
	OLED_CS_HIGH();	//不选择OLED屏工作
}


/*
**********************************************************************************
* 函数功能: OLED屏设置显示位置
* 函数形参: page    页
*           column  列
* 函数返回值: None
* 备注: None
**********************************************************************************
*/
void oled_set_display_location(uint8_t page, uint8_t column)
{
	oled_spi_write_command(0XB0 + page);
	
	oled_spi_write_command(0X10 | (column >> 4));	//高位
	oled_spi_write_command(0x00 | (column & 0X0F));	//低位
}



/*
**********************************************************************************
* 函数功能: OLED屏清空显示
* 函数形参: None
* 函数返回值: None
* 备注: None
**********************************************************************************
*/
void oled_display_clear(void)
{
	for(uint8_t page_count = 0; page_count < 8; page_count++)
	{
		oled_set_display_location(page_count, 0);	//设置坐标
		
		for(uint8_t column_count = 0; column_count < 128; column_count++)
		{
			oled_spi_write_data(0x00);	//循环给列坐标设置显示点
		}
	}
}


/*
**********************************************************************************
* 函数功能: OLED屏显示图片
* 函数形参: display_page     显示起始页
*           display_column   显示起始列
*           *display_memory  显示数据的首地址
* 函数返回值: None
* 备注: 显示数据缓冲区前两个字节需要填写图片宽度与图片高度
**********************************************************************************
*/
void oled_display_image(uint8_t display_page, uint8_t display_column, const uint8_t *display_memory)
{
	uint8_t display_page_number = 0;		//显示需要的页数
	uint8_t display_column_number = 0;	    //显示需要的列数
	uint8_t display_column_value = 0;	    //显示列数的计数值
	uint8_t display_page_value = 0;		    //显示页数的计数值
	
	display_page_number = (display_memory[1] % 8 == 0)?(display_memory[1] / 8):(display_memory[1] / 8 + 1);
	display_column_number = display_memory[0];
	
	if(display_page_number + display_page > 8)
	{
		return ;
	}
	
	if(display_column_number + display_column > 128)
	{
		return ;
	}
	
	for(display_page_value = 0; display_page_value < display_page_number; display_page_value++)
	{
		oled_set_display_location(display_page + display_page_value,display_column);
		
		for(display_column_value = 0; display_column_value < display_column_number; display_column_value++)
		{
			oled_spi_write_data(display_memory[display_column_number * display_page_value + display_column_value + 2]);
		}
	}
	
}


/*
**********************************************************************************
* 函数功能: OLED屏显示英文字符
* 函数形参: display_page     显示起始页
*           display_column   显示起始列
*           *display_memory  显示数据的首地址
* 函数返回值: None
* 备注: 字体大小要求16*16
**********************************************************************************
*/
void oled_display_english_character(uint8_t display_page, uint8_t display_column, uint8_t *display_memory)
{
	uint8_t page_value = 0;
	uint8_t column_value = 0;
	
	for(page_value = 0; page_value < 2; page_value++)
	{
		oled_set_display_location(display_page + page_value, display_column);
		
		for(column_value = 0; column_value < 8; column_value++)
		{
			oled_spi_write_data(*display_memory);
			display_memory++;
		}
	}
}

/*
**********************************************************************************
* 函数功能: OLED屏显示中文字符
* 函数形参: display_page     显示起始页
*           display_column   显示起始列
*           *display_memory  显示数据的首地址
* 函数返回值: None
* 备注: 字体大小要求16*16
**********************************************************************************
*/
void oled_display_chinese_character(uint8_t display_page, uint8_t display_column, uint8_t *display_memory)
{
	uint8_t page_value = 0;		//页计数值
	uint8_t column_value = 0;	//列计数值
	
	for(page_value = 0; page_value < 2; page_value++)
	{
		/* 设置显示坐标 */
		oled_set_display_location(display_page + page_value, display_column);
		
		for(column_value = 0; column_value < 16; column_value++)
		{
			oled_spi_write_data(*display_memory);
			display_memory++;
		}
	}
}
/*
**********************************************************************************
* 函数功能: OLED屏显示字符串
* 函数形参: display_page     显示起始页
*           display_column   显示起始列
*           *display_memory  显示数据的首地址
* 函数返回值: None
* 备注: 字体大小要求16*16
**********************************************************************************
*/
void oled_display_arbitrary_string(uint8_t display_page, uint8_t display_column, uint8_t *display_string)
{
	uint16_t chinese_characters_number = 0;
	
	/* 如果当前字符不是'\0'说明需要显示字符 */
	while(*display_string != '\0')
	{
		if(*display_string > 0XA0)		//当前需要显示的字符是中文
		{
			chinese_characters_number = 0;
			while(1)//查找是第几个字
			{
				if((*display_string == chinese_character_code_list[2 * chinese_characters_number]) && (*(display_string + 1) == chinese_character_code_list[2 * chinese_characters_number + 1]))
				{
					break;		//已经找到该字了
				}
				chinese_characters_number++;
				if(chinese_character_code_list[2 * chinese_characters_number] == '\0')	//当前没有这个中文字
				{
					break;
				}
			}
			if(chinese_character_code_list[2 * chinese_characters_number] != '\0')//有这个字才显示
			{
				if((128 - display_column) < 16)//自动换行
				{
					display_column = 0; 
					display_page += 2;
				}
				oled_display_chinese_character(display_page,display_column,(u8 *)&chinese_character_buff[chinese_characters_number * 32]);
				display_column += 16;
				display_string += 2;
			}
		}
		else		//当前需要显示的是英文
		{
			if((128 - display_column) < 8)//自动换行
			{
				display_column = 0; 
				display_page += 2;
			}
			chinese_characters_number = *display_string - ' ';
			
			oled_display_english_character(display_page,display_column,(u8 *)&english_character_buff[chinese_characters_number * 16]);
			
			display_column += 8;
			display_string += 1;
		}
	}
}


/*
**********************************************************************************
* 函数功能: OLED屏软延时
* 函数形参: microsecond    微秒值
* 函数返回值: None
* 备注: None
**********************************************************************************
*/
void oled_delay_microsecond(uint32_t microsecond)
{
	microsecond *= 18;
	
	for(uint32_t i = 0; i < microsecond; i++)
	{
		;
	}
}


/*
**********************************************************************************
* 函数功能: OLED屏初始化
* 函数形参: None
* 函数返回值: None
* 备注: None
**********************************************************************************
*/
void oled_init(void)
{
	oled_port_init();
	oled_spi_init();
	
	OLED_RES_HIGH();	//不让OLED复位
	oled_delay_microsecond(100000);
	OLED_RES_LOW();		//复位OLED屏
	oled_delay_microsecond(100000);
	OLED_RES_HIGH();	//不让OLED复位
	oled_delay_microsecond(100000);
	
	oled_spi_write_command(0xAE); //关闭显示
	oled_spi_write_command(0xD5); //设置时钟分频因子,震荡频率
	oled_spi_write_command(80);   //[3:0],分频因子;[7:4],震荡频率
	oled_spi_write_command(0xA8); //设置驱动路数
	oled_spi_write_command(0X3F); //默认0X3F(1/64)
	oled_spi_write_command(0xD3); //设置显示偏移
	oled_spi_write_command(0X00); //默认为0

	oled_spi_write_command(0x40); //设置显示开始行 [5:0],行数.

	oled_spi_write_command(0x8D); //电荷泵设置
	oled_spi_write_command(0x14); //bit2，开启/关闭
	oled_spi_write_command(0x20); //设置内存地址模式
	oled_spi_write_command(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	oled_spi_write_command(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
	oled_spi_write_command(0xC8); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	oled_spi_write_command(0xDA); //设置COM硬件引脚配置
	oled_spi_write_command(0x12); //[5:4]配置

	oled_spi_write_command(0x81); //对比度设置
	oled_spi_write_command(0xEF); //1~255;默认0X7F (亮度设置,越大越亮)
	oled_spi_write_command(0xD9); //设置预充电周期
	oled_spi_write_command(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
	oled_spi_write_command(0xDB); //设置VCOMH 电压倍率
	oled_spi_write_command(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	oled_spi_write_command(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	oled_spi_write_command(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示
	oled_spi_write_command(0xAF); //开启显示
	
	oled_display_clear();
}

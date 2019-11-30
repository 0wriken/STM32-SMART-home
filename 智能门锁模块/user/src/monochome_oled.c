#include "monochome_oled.h"
#include "oled_memory.h"


/*
**********************************************************************************
* ��������: OLED���ӿڳ�ʼ��
* �����β�: None
* ��������ֵ: None
* ��ע: None
**********************************************************************************
*/
void oled_port_init(void)
{
#ifdef OLED_GPIO_AFIO_EN
	OLED_GPIO_AFIO_RCC  |= 0x1 << OLED_GPIO_AFIO_RCCX; 
	OLED_GPIO_AFIO->MAPR &= ~(0x7 << OLED_GPIO_AFIO_BIT);
	OLED_GPIO_AFIO->MAPR |= 0x2 << OLED_GPIO_AFIO_BIT;
#endif
	
	/*����OLED��RES�Ľӿ�ʱ��*/
	OLED_RES_RCC_GPIO |= (0X1u << OLED_RES_RCC_GPIOX);
	/*����OLED��RES�Ľӿ�ģʽ*/
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
	/*OLED��RES��ʼ��״̬Ϊ��*/
	OLED_RES_HIGH();
	
	/*����OLED��CS�Ľӿ�ʱ��*/
	OLED_CS_RCC_GPIO |= (0X1u << OLED_CS_RCC_GPIOX);
	/*����OLED��CS�Ľӿ�ģʽ*/
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
	/*OLED��CS��ʼ��״̬Ϊ��*/
	OLED_CS_HIGH();
	
	/*����OLED��DC�Ľӿ�ʱ��*/
	OLED_DC_RCC_GPIO |= (0X1u << OLED_DC_RCC_GPIOX);
	/*����OLED��DC�Ľӿ�ģʽ*/
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
	/*OLED��DC��ʼ��״̬Ϊ��*/
	OLED_DC_LOW();
	
	/*����OLED��CLK�Ľӿ�ʱ��*/
	OLED_CLK_RCC_GPIO |= (0X1u << OLED_CLK_RCC_GPIOX);
	/*����OLED��CLK�Ľӿ�ģʽ*/
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
	
	/*����OLED��DIN�Ľӿ�ʱ��*/
	OLED_DIN_RCC_GPIO |= (0X1u << OLED_DIN_RCC_GPIOX);
	/*����OLED��DIN�Ľӿ�ģʽ*/
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
* ��������: OLED��SPI�����ʼ��
* �����β�: None
* ��������ֵ: None
* ��ע: None
**********************************************************************************
*/
void oled_spi_init(void)
{
	
#ifdef OLED_SPI_AFIO_EN
	OLED_SPI_AFIO->MAPR |= 0X1 << OLED_SPI_AFIO_BIT;   
#endif
	
	/* ����OLED��SPI����ʱ�� */
	OLED_RCC_SPIX |= 0X1 << OLED_RCC_SPIX_BIT;
	
	/* ���OLED��SPI������ƼĴ���1 */
	OLED_SPIX->CR1 = 0X0;	
	
	/* ����OLED��SPI������ƼĴ���1 */
	OLED_SPIX->CR1 |= 0X3 << 8;		//�������������������ѡ��Ƭѡ
	OLED_SPIX->CR1 |= 0X1 << 6;		//ʹ��SPI������
	OLED_SPIX->CR1 |= OLED_SPIX_BAUD_RATE << 3;		//����SPI�����ٶ�
	OLED_SPIX->CR1 |= 0X1 << 2;		//MCU��SPI������Ϊ���豸
	
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
* ��������: OLED��SPI�����д�ֽ�����
* �����β�: wbyte    д�������
* ��������ֵ: SPI������յ�����
* ��ע: None
**********************************************************************************
*/
uint8_t oled_spi_read_write_byte(uint8_t wbyte)
{
	while(!(OLED_SPIX->SR & (0X1 << 1)))	//�ȴ����ͻ�������
	{
		
	}
	OLED_SPIX->DR = wbyte;
	
	while(!(OLED_SPIX->SR & (0X1 << 0)))	//�ȴ����ջ������ǿ�
	{
	
	}
	return OLED_SPIX->DR;
}


/*
**********************************************************************************
* ��������: OLED��SPI���跢������
* �����β�: command    д�������
* ��������ֵ: None
* ��ע: None
**********************************************************************************
*/
void oled_spi_write_command(uint8_t command)
{
	OLED_CS_LOW();	//ѡ��OLED������
	OLED_DC_LOW();	//ѡ��ǰ������ģʽ
	
	oled_spi_read_write_byte(command);
	
	OLED_CS_HIGH();	//��ѡ��OLED������
}


/*
**********************************************************************************
* ��������: OLED��SPI���跢������
* �����β�: ram_data    д�������
* ��������ֵ: None
* ��ע: None
**********************************************************************************
*/
void oled_spi_write_data(uint8_t ram_data)
{
	OLED_CS_LOW();	//ѡ��OLED������
	OLED_DC_HIGH();	//ѡ��ǰ������ģʽ
	
	oled_spi_read_write_byte(ram_data);
	
	OLED_CS_HIGH();	//��ѡ��OLED������
}


/*
**********************************************************************************
* ��������: OLED��������ʾλ��
* �����β�: page    ҳ
*           column  ��
* ��������ֵ: None
* ��ע: None
**********************************************************************************
*/
void oled_set_display_location(uint8_t page, uint8_t column)
{
	oled_spi_write_command(0XB0 + page);
	
	oled_spi_write_command(0X10 | (column >> 4));	//��λ
	oled_spi_write_command(0x00 | (column & 0X0F));	//��λ
}



/*
**********************************************************************************
* ��������: OLED�������ʾ
* �����β�: None
* ��������ֵ: None
* ��ע: None
**********************************************************************************
*/
void oled_display_clear(void)
{
	for(uint8_t page_count = 0; page_count < 8; page_count++)
	{
		oled_set_display_location(page_count, 0);	//��������
		
		for(uint8_t column_count = 0; column_count < 128; column_count++)
		{
			oled_spi_write_data(0x00);	//ѭ����������������ʾ��
		}
	}
}


/*
**********************************************************************************
* ��������: OLED����ʾͼƬ
* �����β�: display_page     ��ʾ��ʼҳ
*           display_column   ��ʾ��ʼ��
*           *display_memory  ��ʾ���ݵ��׵�ַ
* ��������ֵ: None
* ��ע: ��ʾ���ݻ�����ǰ�����ֽ���Ҫ��дͼƬ�����ͼƬ�߶�
**********************************************************************************
*/
void oled_display_image(uint8_t display_page, uint8_t display_column, const uint8_t *display_memory)
{
	uint8_t display_page_number = 0;		//��ʾ��Ҫ��ҳ��
	uint8_t display_column_number = 0;	    //��ʾ��Ҫ������
	uint8_t display_column_value = 0;	    //��ʾ�����ļ���ֵ
	uint8_t display_page_value = 0;		    //��ʾҳ���ļ���ֵ
	
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
* ��������: OLED����ʾӢ���ַ�
* �����β�: display_page     ��ʾ��ʼҳ
*           display_column   ��ʾ��ʼ��
*           *display_memory  ��ʾ���ݵ��׵�ַ
* ��������ֵ: None
* ��ע: �����СҪ��16*16
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
* ��������: OLED����ʾ�����ַ�
* �����β�: display_page     ��ʾ��ʼҳ
*           display_column   ��ʾ��ʼ��
*           *display_memory  ��ʾ���ݵ��׵�ַ
* ��������ֵ: None
* ��ע: �����СҪ��16*16
**********************************************************************************
*/
void oled_display_chinese_character(uint8_t display_page, uint8_t display_column, uint8_t *display_memory)
{
	uint8_t page_value = 0;		//ҳ����ֵ
	uint8_t column_value = 0;	//�м���ֵ
	
	for(page_value = 0; page_value < 2; page_value++)
	{
		/* ������ʾ���� */
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
* ��������: OLED����ʾ�ַ���
* �����β�: display_page     ��ʾ��ʼҳ
*           display_column   ��ʾ��ʼ��
*           *display_memory  ��ʾ���ݵ��׵�ַ
* ��������ֵ: None
* ��ע: �����СҪ��16*16
**********************************************************************************
*/
void oled_display_arbitrary_string(uint8_t display_page, uint8_t display_column, uint8_t *display_string)
{
	uint16_t chinese_characters_number = 0;
	
	/* �����ǰ�ַ�����'\0'˵����Ҫ��ʾ�ַ� */
	while(*display_string != '\0')
	{
		if(*display_string > 0XA0)		//��ǰ��Ҫ��ʾ���ַ�������
		{
			chinese_characters_number = 0;
			while(1)//�����ǵڼ�����
			{
				if((*display_string == chinese_character_code_list[2 * chinese_characters_number]) && (*(display_string + 1) == chinese_character_code_list[2 * chinese_characters_number + 1]))
				{
					break;		//�Ѿ��ҵ�������
				}
				chinese_characters_number++;
				if(chinese_character_code_list[2 * chinese_characters_number] == '\0')	//��ǰû�����������
				{
					break;
				}
			}
			if(chinese_character_code_list[2 * chinese_characters_number] != '\0')//������ֲ���ʾ
			{
				if((128 - display_column) < 16)//�Զ�����
				{
					display_column = 0; 
					display_page += 2;
				}
				oled_display_chinese_character(display_page,display_column,(u8 *)&chinese_character_buff[chinese_characters_number * 32]);
				display_column += 16;
				display_string += 2;
			}
		}
		else		//��ǰ��Ҫ��ʾ����Ӣ��
		{
			if((128 - display_column) < 8)//�Զ�����
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
* ��������: OLED������ʱ
* �����β�: microsecond    ΢��ֵ
* ��������ֵ: None
* ��ע: None
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
* ��������: OLED����ʼ��
* �����β�: None
* ��������ֵ: None
* ��ע: None
**********************************************************************************
*/
void oled_init(void)
{
	oled_port_init();
	oled_spi_init();
	
	OLED_RES_HIGH();	//����OLED��λ
	oled_delay_microsecond(100000);
	OLED_RES_LOW();		//��λOLED��
	oled_delay_microsecond(100000);
	OLED_RES_HIGH();	//����OLED��λ
	oled_delay_microsecond(100000);
	
	oled_spi_write_command(0xAE); //�ر���ʾ
	oled_spi_write_command(0xD5); //����ʱ�ӷ�Ƶ����,��Ƶ��
	oled_spi_write_command(80);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	oled_spi_write_command(0xA8); //��������·��
	oled_spi_write_command(0X3F); //Ĭ��0X3F(1/64)
	oled_spi_write_command(0xD3); //������ʾƫ��
	oled_spi_write_command(0X00); //Ĭ��Ϊ0

	oled_spi_write_command(0x40); //������ʾ��ʼ�� [5:0],����.

	oled_spi_write_command(0x8D); //��ɱ�����
	oled_spi_write_command(0x14); //bit2������/�ر�
	oled_spi_write_command(0x20); //�����ڴ��ַģʽ
	oled_spi_write_command(0x02); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	oled_spi_write_command(0xA1); //���ض�������,bit0:0,0->0;1,0->127;
	oled_spi_write_command(0xC8); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	oled_spi_write_command(0xDA); //����COMӲ����������
	oled_spi_write_command(0x12); //[5:4]����

	oled_spi_write_command(0x81); //�Աȶ�����
	oled_spi_write_command(0xEF); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	oled_spi_write_command(0xD9); //����Ԥ�������
	oled_spi_write_command(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
	oled_spi_write_command(0xDB); //����VCOMH ��ѹ����
	oled_spi_write_command(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	oled_spi_write_command(0xA4); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	oled_spi_write_command(0xA6); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
	oled_spi_write_command(0xAF); //������ʾ
	
	oled_display_clear();
}

#include "spi.h"


/*
SPI_SCK:PB13   �������
SPI_MOSI:PB15  �������
*/
void SPIPinInit(void)
{
	RCC->APB2ENR |=0x01<<3;
	
	GPIOB->CRH &=~(0x0F<<20 |0x0F<<28);
	GPIOB->CRH |=(0x03<<20 |0x03<<28);
	
	SPI_SCK=0;//���е�ƽ
}

/*
SPI��������һ���ֽ�
MODE0/3:�½��ط��ͷ����ͣ������ؽ��շ����� 
��λ��ǰ
*/
void SPISendByte(u8 data)
{
	u8 i;
	
	SPI_SCK=1;
	for(i=0;i<8;i++)
	{
		SPI_SCK=0;
		if((data<<i)&0x80)
			SPI_MOSI=1;
		else
			SPI_MOSI=0;
		SPI_SCK=1;
	}
	
}


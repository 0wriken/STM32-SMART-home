#include "spi.h"


/*
SPI_SCK:PB13   推挽输出
SPI_MOSI:PB15  推挽输出
*/
void SPIPinInit(void)
{
	RCC->APB2ENR |=0x01<<3;
	
	GPIOB->CRH &=~(0x0F<<20 |0x0F<<28);
	GPIOB->CRH |=(0x03<<20 |0x03<<28);
	
	SPI_SCK=0;//空闲电平
}

/*
SPI主机发送一个字节
MODE0/3:下降沿发送方发送，上升沿接收方接收 
高位在前
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


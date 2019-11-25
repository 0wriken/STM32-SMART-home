#include "spi.h"
void spi_init()
{
	
}
void send_data(u8 data)
{
	SPI_SCK=1;
	for(int i=0;i<8;i++)
	{
		SPI_SCK=0;
		if((data<<i)&(0x80))
		MOSI=1;
		else 
			MOSI=0;
		SPI_SCK=1;
	}
}	
void send_com(u8 data)
{
	CS=0;
	DC=0;
	send_data(data);
	CS=1;
}
void send_DATA(u8 data)
{
	CS=0;
	DC=1;
	send_data(data);
	CS=1;
}

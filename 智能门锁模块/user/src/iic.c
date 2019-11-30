#include "iic.h"
#include "delay.h"

//通信速度400Khz

/*
函数功能:IIC接口初始化
SCL---PB4---开漏输出
SDA---PB5---开漏输出
*/
void IICPinInit(void)
{
	RCC->APB2ENR |=0x01<<0;//AFIO时钟使能
	AFIO->MAPR &=~(0x07<<24);
	AFIO->MAPR |=(0x02<<24);//把PB3,4切换普通IO功能
	
	RCC->APB2ENR |=0x01<<3;
	GPIOB->CRL &=~(0xFF<<16);
	GPIOB->CRL|=(0X4<<12);
	GPIOB->CRL |=(0x77<<16);//PB4和PB5配置成开漏输出
	
	//总线空闲状态
	IIC_SDA_OUT=1;
	IIC_SCL=1;    
	
}

/*
起始条件
*/
void IICStart(void)
{	
	IIC_SCL=1;
	IIC_SDA_OUT=1;
	Delayus(2);//延时--起始条件建立时间
	IIC_SDA_OUT=0;
	Delayus(1);//延时--起始条件保持时间
	IIC_SCL=0; //---一个完整的周期
}

/*
停止条件
*/
void IICStop(void)
{
	IIC_SCL=0;
	IIC_SDA_OUT=0;
	IIC_SCL=1;
	Delayus(1);//延时--停止条件的建立时间
	IIC_SDA_OUT=1;
	Delayus(2);//延时--本次停止到下一开始的时间
}

/*
函数功能：主机发送应答信号
参数说明：待发送的应答信号 0有应答 1非应答
*/
void IICSendAck(u8 ack)
{
	IIC_SCL=0;
	if(ack)//主机准备数据
		IIC_SDA_OUT=1;
	else
		IIC_SDA_OUT=0;
	Delayus(2);//延时（数据稳定在数据线上）
	IIC_SCL=1;//（从机在时钟线上升沿从SDA上采集数据）
	Delayus(1);//延时（给时间从机读取数据）
}


/*
函数功能：主机读取从机的应答信号
返回值：读取到的应答信号  0有应答  1非应答
*/
u8 IICReadAck(void)
{
	u8 ack=0;
	
	IIC_SCL=0;//（从机准备数据）
	IIC_SDA_OUT=1;//读模式，为了是IO口的输出电路给引脚断开！！！！！！！！！
	Delayus(2);//延时（给时间从机准备数据并且数据稳定在数据线上）
	IIC_SCL=1;
	Delayus(1);//延时 (给时间主机读取数据)
	if(IIC_SDA_IN)//主机读取SDA线上的数据
		ack=1;
	
	IIC_SCL=0;//为了方便后续操作，防止意外出现停止条件
	
	return ack;
}


/*
函数功能：主机给从机发送一个字节数据并且读取从机应答
参数说明：待发送的一个字节数据
返回值：应答信号
*/
u8 IICSendByte(u8 data) 
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		if((data<<i)&0x80)//主机准备数据
			IIC_SDA_OUT=1;
		else
			IIC_SDA_OUT=0;
		Delayus(2);//延时（数据稳定在数据线上）
		IIC_SCL=1;//（从机在时钟线上升沿从SDA上采集数据）
		Delayus(1);//延时（给时间从机读取数据）
	}
	
	IIC_SCL=0;//为了方便后续操作，防止意外出现停止条件
	
	return IICReadAck( );
}

/*
函数功能：主机读取从机一个字节数据并且发送应答信号
参数说明：待发送的应答信号
返回值：读取到的一个字节数据
*/
u8 IICReadByte(u8 ack) 
{
	u8 i;
	u8 data=0;
	
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;//（从机准备数据）
		IIC_SDA_OUT=1;//读模式，为了是IO口的输出电路给引脚断开！！！！！！！！！
		Delayus(2);//延时（给时间从机准备数据并且数据稳定在数据线上）
		IIC_SCL=1;
		Delayus(1);//延时 (给时间主机读取数据)
		data<<=1;//空出最低位
		if(IIC_SDA_IN)//主机读取SDA线上的数据
			data |=1;
	}
	
	IIC_SCL=0;//为了方便后续操作，防止意外出现停止条件
	
	IICSendAck(ack);
	
	return data;
}




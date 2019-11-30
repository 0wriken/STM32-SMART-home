#include "iic.h"
#include "delay.h"

//ͨ���ٶ�400Khz

/*
��������:IIC�ӿڳ�ʼ��
SCL---PB4---��©���
SDA---PB5---��©���
*/
void IICPinInit(void)
{
	RCC->APB2ENR |=0x01<<0;//AFIOʱ��ʹ��
	AFIO->MAPR &=~(0x07<<24);
	AFIO->MAPR |=(0x02<<24);//��PB3,4�л���ͨIO����
	
	RCC->APB2ENR |=0x01<<3;
	GPIOB->CRL &=~(0xFF<<16);
	GPIOB->CRL|=(0X4<<12);
	GPIOB->CRL |=(0x77<<16);//PB4��PB5���óɿ�©���
	
	//���߿���״̬
	IIC_SDA_OUT=1;
	IIC_SCL=1;    
	
}

/*
��ʼ����
*/
void IICStart(void)
{	
	IIC_SCL=1;
	IIC_SDA_OUT=1;
	Delayus(2);//��ʱ--��ʼ��������ʱ��
	IIC_SDA_OUT=0;
	Delayus(1);//��ʱ--��ʼ��������ʱ��
	IIC_SCL=0; //---һ������������
}

/*
ֹͣ����
*/
void IICStop(void)
{
	IIC_SCL=0;
	IIC_SDA_OUT=0;
	IIC_SCL=1;
	Delayus(1);//��ʱ--ֹͣ�����Ľ���ʱ��
	IIC_SDA_OUT=1;
	Delayus(2);//��ʱ--����ֹͣ����һ��ʼ��ʱ��
}

/*
�������ܣ���������Ӧ���ź�
����˵���������͵�Ӧ���ź� 0��Ӧ�� 1��Ӧ��
*/
void IICSendAck(u8 ack)
{
	IIC_SCL=0;
	if(ack)//����׼������
		IIC_SDA_OUT=1;
	else
		IIC_SDA_OUT=0;
	Delayus(2);//��ʱ�������ȶ����������ϣ�
	IIC_SCL=1;//���ӻ���ʱ���������ش�SDA�ϲɼ����ݣ�
	Delayus(1);//��ʱ����ʱ��ӻ���ȡ���ݣ�
}


/*
�������ܣ�������ȡ�ӻ���Ӧ���ź�
����ֵ����ȡ����Ӧ���ź�  0��Ӧ��  1��Ӧ��
*/
u8 IICReadAck(void)
{
	u8 ack=0;
	
	IIC_SCL=0;//���ӻ�׼�����ݣ�
	IIC_SDA_OUT=1;//��ģʽ��Ϊ����IO�ڵ������·�����ŶϿ�������������������
	Delayus(2);//��ʱ����ʱ��ӻ�׼�����ݲ��������ȶ����������ϣ�
	IIC_SCL=1;
	Delayus(1);//��ʱ (��ʱ��������ȡ����)
	if(IIC_SDA_IN)//������ȡSDA���ϵ�����
		ack=1;
	
	IIC_SCL=0;//Ϊ�˷��������������ֹ�������ֹͣ����
	
	return ack;
}


/*
�������ܣ��������ӻ�����һ���ֽ����ݲ��Ҷ�ȡ�ӻ�Ӧ��
����˵���������͵�һ���ֽ�����
����ֵ��Ӧ���ź�
*/
u8 IICSendByte(u8 data) 
{
	u8 i;
	
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		if((data<<i)&0x80)//����׼������
			IIC_SDA_OUT=1;
		else
			IIC_SDA_OUT=0;
		Delayus(2);//��ʱ�������ȶ����������ϣ�
		IIC_SCL=1;//���ӻ���ʱ���������ش�SDA�ϲɼ����ݣ�
		Delayus(1);//��ʱ����ʱ��ӻ���ȡ���ݣ�
	}
	
	IIC_SCL=0;//Ϊ�˷��������������ֹ�������ֹͣ����
	
	return IICReadAck( );
}

/*
�������ܣ�������ȡ�ӻ�һ���ֽ����ݲ��ҷ���Ӧ���ź�
����˵���������͵�Ӧ���ź�
����ֵ����ȡ����һ���ֽ�����
*/
u8 IICReadByte(u8 ack) 
{
	u8 i;
	u8 data=0;
	
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;//���ӻ�׼�����ݣ�
		IIC_SDA_OUT=1;//��ģʽ��Ϊ����IO�ڵ������·�����ŶϿ�������������������
		Delayus(2);//��ʱ����ʱ��ӻ�׼�����ݲ��������ȶ����������ϣ�
		IIC_SCL=1;
		Delayus(1);//��ʱ (��ʱ��������ȡ����)
		data<<=1;//�ճ����λ
		if(IIC_SDA_IN)//������ȡSDA���ϵ�����
			data |=1;
	}
	
	IIC_SCL=0;//Ϊ�˷��������������ֹ�������ֹͣ����
	
	IICSendAck(ack);
	
	return data;
}




#include "at24c02.h"
#include "iic.h"
#include "delay.h"

/*
�������ܣ��ֽ�д
����˵����addr��д��ĵ�ַ  data��д�������
����ֵ��0д��ɹ�  ������������
*/
u8 At24c02WriteByte(u8 addr,u8 data)
{
	IICStart( );//��ʼ����
	
	
	if( IICSendByte(AT24C02ADDR_W) )//����������ַ+д����
	{
		IICStop( );
		return WRITE_ERR1;
	}
	
	if( IICSendByte(addr) )//�����ڲ���ַ
	{
		IICStop( );
		return WRITE_ERR2;
	}
	
	if( IICSendByte(data) )//��������
	{
		IICStop( );
		return WRITE_ERR3;
	}
	
	IICStop( );//ֹͣ����
	
	
	Delayms(5);//д����
	
	return 0;
}


/*
�������ܣ�ҳд
����˵����addr��д��ĵ�ַ  num��д������ݸ���   p��ָ���д������ݻ���ǰ
����ֵ��0д��ɹ�  ������������
ע�������ʼ��ַ�ͽ�����ַҪ��һ��
*/
u8 At24c02PageWrite(u8 addr,u8 num,u8* p)
{
	//��ʼд���ַ��addr         ����ҳ��addr/8
	//����д���ַ��addr+num-1   ����ҳ����addr+num-1��/8
	
	if( addr/8 != (addr+num-1)/16 )
			return NOTSAMEPAGE;
	
	IICStart( );//��ʼ����
	
	
	if( IICSendByte(AT24C02ADDR_W) )//����������ַ+д����
	{
		IICStop( );
		return WRITE_ERR4;
	}
	
	if( IICSendByte(addr) )//�����ڲ���ַ
	{
		IICStop( );
		return WRITE_ERR5;
	}
	
	while(num--)
	{
		if( IICSendByte(*p++) )//��������
		{
			IICStop( );
			return WRITE_ERR6;
		}
	}
	
	
	IICStop( );//ֹͣ����
	
	
	Delayms(5);//д����
	
	return 0;
}


/*
�������ܣ����д
����˵����addr�ڲ���ַ   num��д������ݸ���1~256  pָ�����ݻ�����
����ֵ��0д��ɹ�  ������������
*/
//u8 At24c02WriteBytes(u8 addr,u16 num,u8* p)
//{
//	u8 ret=0;
//		u8 less_addr=0;//��ǰҳ��ʣ���ٸ��ֽڿռ�
//	
//	while(1)
//	{
//		less_addr=8-addr%8;
//		if(less_addr>=num){//��ǰҳ��ȫ��д��
//			ret=At24c02PageWrite(addr,num,p);
//			return ret;
//		}
//		else{//��ǰҳд����num������д�굱ǰҳless_addr��
//			ret=At24c02PageWrite(addr,less_addr,p);
//			if(ret)//д��ʧ��
//				return ret;
//		}
//		
//		addr=addr+less_addr;
//		num=num-less_addr;
//		p=p+less_addr;
//	}
//	
//}
u8 At24c02WriteBytes(u8 addr,u16 num,u8* p)
{
	u8 ret=0;
		u8 less_addr=0;//��ǰҳ��ʣ���ٸ��ֽڿռ�
	
	while(1)
	{
		less_addr=16-addr%8;
		if(less_addr>=num){//��ǰҳ��ȫ��д��
			less_addr=num;
		}
		ret=At24c02PageWrite(addr,less_addr,p);
		if(ret)//д��ʧ��
			return ret;
		
		if(less_addr==num)
			return 0;
		
		
		addr=addr+less_addr;
		num=num-less_addr;
		p=p+less_addr;
	}
}

/*
�������ܣ������
����˵����addr�ڲ���ַ   num����ȡ�����ݸ���1~256  pָ��������ݻ�����
����ֵ��0��ȡ�ɹ�  ������������
*/
u8 At24c02ReadBytes(u8 addr,u16 num,u8* p)
{
	
	IICStart( );//��ʼ����
	
	
	if( IICSendByte(AT24C02ADDR_W) )//����������ַ+д����
	{
		IICStop( );
		return READ_ERR1;
	}
	
	if( IICSendByte(addr) )//�����ڲ���ַ
	{
		IICStop( );
		return READ_ERR2;
	}
	
	IICStart( );//�ظ���ʼ����
	
	if( IICSendByte(AT24C02ADDR_R) )//����������ַ+������
	{
		IICStop( );
		return READ_ERR3;
	}
	
	//������ȡ
	while(num) 
	{
		num--;//������λ�ʣ���ٸ�����
		if(0==num)
		{
			*p= IICReadByte(1) ;
			break;
		}
			
		*p++= IICReadByte(0) ;
	}
	
	IICStop( );//ֹͣ����
	
	return 0;
}


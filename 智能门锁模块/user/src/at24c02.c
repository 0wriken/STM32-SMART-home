#include "at24c02.h"
#include "iic.h"
#include "delay.h"

/*
函数功能：字节写
参数说明：addr待写入的地址  data待写入的数据
返回值：0写入成功  其他：错误码
*/
u8 At24c02WriteByte(u8 addr,u8 data)
{
	IICStart( );//起始条件
	
	
	if( IICSendByte(AT24C02ADDR_W) )//发送器件地址+写方向
	{
		IICStop( );
		return WRITE_ERR1;
	}
	
	if( IICSendByte(addr) )//发送内部地址
	{
		IICStop( );
		return WRITE_ERR2;
	}
	
	if( IICSendByte(data) )//发送数据
	{
		IICStop( );
		return WRITE_ERR3;
	}
	
	IICStop( );//停止条件
	
	
	Delayms(5);//写周期
	
	return 0;
}


/*
函数功能：页写
参数说明：addr待写入的地址  num待写入的数据个数   p待指向待写入的数据换从前
返回值：0写入成功  其他：错误码
注意事项：起始地址和结束地址要在一样
*/
u8 At24c02PageWrite(u8 addr,u8 num,u8* p)
{
	//起始写入地址：addr         所在页：addr/8
	//结束写入地址：addr+num-1   所在页：（addr+num-1）/8
	
	if( addr/8 != (addr+num-1)/16 )
			return NOTSAMEPAGE;
	
	IICStart( );//起始条件
	
	
	if( IICSendByte(AT24C02ADDR_W) )//发送器件地址+写方向
	{
		IICStop( );
		return WRITE_ERR4;
	}
	
	if( IICSendByte(addr) )//发送内部地址
	{
		IICStop( );
		return WRITE_ERR5;
	}
	
	while(num--)
	{
		if( IICSendByte(*p++) )//发送数据
		{
			IICStop( );
			return WRITE_ERR6;
		}
	}
	
	
	IICStop( );//停止条件
	
	
	Delayms(5);//写周期
	
	return 0;
}


/*
函数功能：随机写
参数说明：addr内部地址   num待写入的数据个数1~256  p指向数据缓冲区
返回值：0写入成功  其他：错误码
*/
//u8 At24c02WriteBytes(u8 addr,u16 num,u8* p)
//{
//	u8 ret=0;
//		u8 less_addr=0;//当前页还剩多少个字节空间
//	
//	while(1)
//	{
//		less_addr=8-addr%8;
//		if(less_addr>=num){//当前页能全部写完
//			ret=At24c02PageWrite(addr,num,p);
//			return ret;
//		}
//		else{//当前页写不完num个，先写完当前页less_addr个
//			ret=At24c02PageWrite(addr,less_addr,p);
//			if(ret)//写入失败
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
		u8 less_addr=0;//当前页还剩多少个字节空间
	
	while(1)
	{
		less_addr=16-addr%8;
		if(less_addr>=num){//当前页能全部写完
			less_addr=num;
		}
		ret=At24c02PageWrite(addr,less_addr,p);
		if(ret)//写入失败
			return ret;
		
		if(less_addr==num)
			return 0;
		
		
		addr=addr+less_addr;
		num=num-less_addr;
		p=p+less_addr;
	}
}

/*
函数功能：随机读
参数说明：addr内部地址   num待读取的数据个数1~256  p指向接收数据缓冲区
返回值：0读取成功  其他：错误码
*/
u8 At24c02ReadBytes(u8 addr,u16 num,u8* p)
{
	
	IICStart( );//起始条件
	
	
	if( IICSendByte(AT24C02ADDR_W) )//发送器件地址+写方向
	{
		IICStop( );
		return READ_ERR1;
	}
	
	if( IICSendByte(addr) )//发送内部地址
	{
		IICStop( );
		return READ_ERR2;
	}
	
	IICStart( );//重复起始条件
	
	if( IICSendByte(AT24C02ADDR_R) )//发送器件地址+读方向
	{
		IICStop( );
		return READ_ERR3;
	}
	
	//连续读取
	while(num) 
	{
		num--;//读完这次还剩多少个数据
		if(0==num)
		{
			*p= IICReadByte(1) ;
			break;
		}
			
		*p++= IICReadByte(0) ;
	}
	
	IICStop( );//停止条件
	
	return 0;
}


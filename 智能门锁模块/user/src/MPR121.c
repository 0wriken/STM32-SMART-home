#include "MPR121.h"
#include "iic.h"
#include "stdio.h"


/*
函数功能：写寄存器
参数说明：regaddr寄存器地址    data写入的值 
*/
u8 MPR121WriteDate(u8 regaddr,u8 data)
{
	
	IICStart( );
	
	if( IICSendByte(MPR121ADDR_W) )
	{
		IICStop();
		printf("1111111111111\r\n");
		return 0xFF;
	}
	
	if( IICSendByte(regaddr) )
	{
		IICStop();
		printf("222222222222\r\n");
		return 0xFF;
	}
	
	if( IICSendByte(data) )
	{
		IICStop();
		printf("3333333333333\r\n");
		return 0xFF;
	}
	
	IICStop();
	
	return 0;
}


/*
函数功能：读寄存器
参数说明：regaddr寄存器地址  
返回值：读到的值
*/
u8 MPR121ReadDate(u8 regaddr)
{
	IICStart( );
	
	if( IICSendByte(MPR121ADDR_W) )
	{
		IICStop();
		return 0;
	}
	
	if( IICSendByte(regaddr) )
	{
		IICStop();
		return 0;
	}
	
	IICStart( );
	if( IICSendByte(MPR121ADDR_R) )
	{
		IICStop();
		return 0;
	}
	
	return IICReadByte(1) ;
	
}


//MPR121初始化序列
void MPR121Init(void)
{
	//AN3891 A
	MPR121WriteDate(MHD_Rising,0x01);
	MPR121WriteDate(NHD_Amount_Rising,0x01);
	MPR121WriteDate(NCL_Rising,0);
	MPR121WriteDate(FDL_Rising,0);
	//AN3891 B
	MPR121WriteDate(MHD_Falling,1);
	MPR121WriteDate(NHD_Amount_Falling,1);
	MPR121WriteDate(NCL_Falling,0xFF);
	MPR121WriteDate(FDL_Falling,0x02);

//AN3892 C
	  MPR121WriteDate(ELE0_Touch_Threshold,0x0F);
	MPR121WriteDate(ELE0_Release_Threshold,0x0A);
	  MPR121WriteDate(ELE1_Touch_Threshold,0x0F);
	MPR121WriteDate(ELE1_Release_Threshold,0x0A);
	  MPR121WriteDate(ELE2_Touch_Threshold,0x0F);
	MPR121WriteDate(ELE2_Release_Threshold,0x0A);
	  MPR121WriteDate(ELE3_Touch_Threshold,0x0F);
	MPR121WriteDate(ELE3_Release_Threshold,0x0A);
	  MPR121WriteDate(ELE4_Touch_Threshold,0x0F);
	MPR121WriteDate(ELE4_Release_Threshold,0x0A);
	  MPR121WriteDate(ELE5_Touch_Threshold,0x0F);
	MPR121WriteDate(ELE5_Release_Threshold,0x0A);
	  MPR121WriteDate(ELE6_Touch_Threshold,0x0F);
	MPR121WriteDate(ELE6_Release_Threshold,0x0A);
	  MPR121WriteDate(ELE7_Touch_Threshold,0x0F);
	MPR121WriteDate(ELE7_Release_Threshold,0x0A);
	  MPR121WriteDate(ELE8_Touch_Threshold,0x0F);
	MPR121WriteDate(ELE8_Release_Threshold,0x0A);
  	MPR121WriteDate(ELE9_Touch_Threshold,0x0F);
	MPR121WriteDate(ELE9_Release_Threshold,0x0A);
   MPR121WriteDate(ELE10_Touch_Threshold,0x0F);
	MPR121WriteDate(ELE10_Release_Threshold,0x0A);
   MPR121WriteDate(ELE11_Touch_Threshold,0x0F);
	MPR121WriteDate(ELE11_Release_Threshold,0x0A);


	//AN3890 D
	MPR121WriteDate(Filter_Configuration,0x04);

	//AN3890 E
	MPR121WriteDate(Electrode_Configuration,0x0C);
	
}



u8 GetMPR121(void)
{
	u16 data=0;
	
	data = MPR121ReadDate(0x01)<<8;
	data|= MPR121ReadDate(0x00);
	
	if(data)
	{
		switch(data)
		{
			case 0x0001:return '1';break;
			case 0x0002:return '4';break;
			case 0x0004:return '7';break;
			case 0x0008:return '*';break;
			case 0x0010:return '5';break;
			case 0x0020:return '2';break;
			case 0x0040:return '8';break;
			case 0x0080:return '0';break;
			case 0x0100:return '#';break;
			case 0x0200:return '9';break;
			case 0x0400:return '6';break;
			case 0x0800:return '3';break;
		}
	}
		
	
	return 0;
	
}





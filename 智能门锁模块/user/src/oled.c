#include "oled.h"
#include "spi.h"
#include "delay.h"

/* 
OLED_CS  PAout(15)
OLED_DC  PAout(11)
OLED_RES PAout(12)
*/
void OLEDPinInit(void)
{
	RCC->APB2ENR |=(0x01<<0 | 0x01<<2);//AFIO GPIOA时钟使能
	
	AFIO->MAPR &=~(0x07<<24);
	AFIO->MAPR |=(0x02<<24);//把PA15切换普通IO功能
	
	GPIOA->CRH &=~(0xFF<<12 | 0x0F<<28);
	GPIOA->CRH |=(0x33<<12 | 0x03<<28);
	
	OLED_CS=1;
	OLED_RES=1;
	
}




//发送命令
void OLEDSendCmd(u8 cmd)
{
	OLED_CS=0;//片选
	OLED_DC=0;//命令
	
	SPISendByte(cmd);
	OLED_CS=1;//取消片选
}

//发送显存数据
void OLEDSendData(u8 data)
{
	OLED_CS=0;//片选
	OLED_DC=1;//数据
	
	SPISendByte(data);
	OLED_CS=1;//取消片选
}


void OLEDClear(void)
{
	u8 i,j;
	
	for(i=0;i<8;i++)
	{
		OLEDSendCmd(0xB0+i);//设置页地址：
		//设置列地址:
		OLEDSendCmd(0x00);//设置列地址低四位 
		OLEDSendCmd(0x10);//设置列地址高四位 
		for(j=0;j<128;j++)
			OLEDSendData(0xFF);
	}
}


void OLEDInit(void)
{
	OLEDPinInit( );
	
	OLED_RES=1;
  Delayms(100);
  OLED_RES=0;
  Delayms(100);
  OLED_RES=1;
  Delayms(100); 
  OLEDSendCmd(0xAE); //关闭显示
  OLEDSendCmd(0xD5); //设置时钟分频因子,震荡频率
  OLEDSendCmd(80);   //[3:0],分频因子;[7:4],震荡频率
  OLEDSendCmd(0xA8); //设置驱动路数
  OLEDSendCmd(0X3F); //默认0X3F(1/64)
  OLEDSendCmd(0xD3); //设置显示偏移
  OLEDSendCmd(0X00); //默认为0

  OLEDSendCmd(0x40); //设置显示开始行 [5:0],行数.

  OLEDSendCmd(0x8D); //电荷泵设置
  OLEDSendCmd(0x14); //bit2，开启/关闭
  OLEDSendCmd(0x20); //设置内存地址模式
  OLEDSendCmd(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
  OLEDSendCmd(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
  OLEDSendCmd(0xC8); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
  OLEDSendCmd(0xDA); //设置COM硬件引脚配置
  OLEDSendCmd(0x12); //[5:4]配置

  OLEDSendCmd(0x81); //对比度设置
  OLEDSendCmd(0xEF); //1~255;默认0X7F (亮度设置,越大越亮)
  OLEDSendCmd(0xD9); //设置预充电周期
  OLEDSendCmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
  OLEDSendCmd(0xDB); //设置VCOMH 电压倍率
  OLEDSendCmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

  OLEDSendCmd(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
  OLEDSendCmd(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示
  OLEDSendCmd(0xAF); //开启显示
	
	
	OLEDClear( );

}


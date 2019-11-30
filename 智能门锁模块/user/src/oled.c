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
	RCC->APB2ENR |=(0x01<<0 | 0x01<<2);//AFIO GPIOAʱ��ʹ��
	
	AFIO->MAPR &=~(0x07<<24);
	AFIO->MAPR |=(0x02<<24);//��PA15�л���ͨIO����
	
	GPIOA->CRH &=~(0xFF<<12 | 0x0F<<28);
	GPIOA->CRH |=(0x33<<12 | 0x03<<28);
	
	OLED_CS=1;
	OLED_RES=1;
	
}




//��������
void OLEDSendCmd(u8 cmd)
{
	OLED_CS=0;//Ƭѡ
	OLED_DC=0;//����
	
	SPISendByte(cmd);
	OLED_CS=1;//ȡ��Ƭѡ
}

//�����Դ�����
void OLEDSendData(u8 data)
{
	OLED_CS=0;//Ƭѡ
	OLED_DC=1;//����
	
	SPISendByte(data);
	OLED_CS=1;//ȡ��Ƭѡ
}


void OLEDClear(void)
{
	u8 i,j;
	
	for(i=0;i<8;i++)
	{
		OLEDSendCmd(0xB0+i);//����ҳ��ַ��
		//�����е�ַ:
		OLEDSendCmd(0x00);//�����е�ַ����λ 
		OLEDSendCmd(0x10);//�����е�ַ����λ 
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
  OLEDSendCmd(0xAE); //�ر���ʾ
  OLEDSendCmd(0xD5); //����ʱ�ӷ�Ƶ����,��Ƶ��
  OLEDSendCmd(80);   //[3:0],��Ƶ����;[7:4],��Ƶ��
  OLEDSendCmd(0xA8); //��������·��
  OLEDSendCmd(0X3F); //Ĭ��0X3F(1/64)
  OLEDSendCmd(0xD3); //������ʾƫ��
  OLEDSendCmd(0X00); //Ĭ��Ϊ0

  OLEDSendCmd(0x40); //������ʾ��ʼ�� [5:0],����.

  OLEDSendCmd(0x8D); //��ɱ�����
  OLEDSendCmd(0x14); //bit2������/�ر�
  OLEDSendCmd(0x20); //�����ڴ��ַģʽ
  OLEDSendCmd(0x02); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
  OLEDSendCmd(0xA1); //���ض�������,bit0:0,0->0;1,0->127;
  OLEDSendCmd(0xC8); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
  OLEDSendCmd(0xDA); //����COMӲ����������
  OLEDSendCmd(0x12); //[5:4]����

  OLEDSendCmd(0x81); //�Աȶ�����
  OLEDSendCmd(0xEF); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
  OLEDSendCmd(0xD9); //����Ԥ�������
  OLEDSendCmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
  OLEDSendCmd(0xDB); //����VCOMH ��ѹ����
  OLEDSendCmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

  OLEDSendCmd(0xA4); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
  OLEDSendCmd(0xA6); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
  OLEDSendCmd(0xAF); //������ʾ
	
	
	OLEDClear( );

}


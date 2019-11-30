#include "MG200.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "oled_memory.h"
TYPE_U2 MG200rev;
void MG200Init(void)
{
	//PC13--Detect����Ӧ�ϵ��źţ���ָ����ָ�ƴ�����ʱ����ߵ�ƽ��
	//PB9---MG200_PWR
	
	RCC->APB2ENR |=0x03<<3;
	
	GPIOB->CRH &=~(0x0F<<4);
	GPIOB->CRH |=(0x03<<4);
	GPIOC->CRH &=~(0x0F<<20);
	GPIOC->CRH |=(0x04<<20);
	MG200_PWR=1;
	//GPIOB->ODR |=0x01<<9;
}

/*
�������ܣ���MG200����һ���ֽ�����
����˵���������͵�����
����ֵ��
ע�����
*/
void MG200SendByte(u8 data)
{
	while((USART2->SR & (0X01<<7))==0);  //�ȴ����ͻ�����Ϊ��
	USART2->DR=data;
}

/*
�������ܣ�MCU ��ָ��ģ�鷢��ָ��
����˵����cmd-ָ����   parm-����
����ֵ����
ע�����
*/
void MG200SendCommond(u8 cmd,u8 param)
{
	u8 checksum=0;//У���
	
	//У��� Ϊ��ȷ�Ϸ������ݰ�����ȷ�Զ��裬�ǳ�ȥ��ʼ��֮��������ֽڰ���
	//8bit ��λ��ӵĽ����������ֽ������ӣ�
	checksum =(SENDADDR+REVADDR+cmd+param+RESERVE)&0xFF;

	MG200SendByte(STARTCODE);//��ʼ��
	MG200SendByte(SENDADDR);//���͵�ַ
	MG200SendByte(REVADDR);//���յ�ַ
	MG200SendByte(cmd);//ָ���� ��ʾָ������
	MG200SendByte(param);//���� ����ָ����Ĳ�ͬ�����ܴ��ڵĲ��������ݣ�
	MG200SendByte(RESERVE);//Ԥ���ֽڣ� ������չʹ�ã�Ŀǰδʹ�ã�Ĭ��Ϊ 00h
	MG200SendByte(checksum);//У���

}


/*
�������ܣ�ָ��ģ���ָ��Ļظ���
����˵����cmd-���ظ���ָ����   
����ֵ����
ע�����
*/
u8 MG200Ack(u8 cmd,u8* result,u8* param)
{
	u8 checksum=0;//У���
	while(!MG200rev.RevOver);//�ȴ�U2�������
	MG200rev.RevOver=0;
	/*
��ָ��ģ����յ�Ӧ������ص�Ԫ��Ҫ��������յ����ݰ��Ƿ���ȷ��������
�����������ж�Ϊ���յ���ȷ�ظ���������ʼλ���շ���ַ�Ƿ���ȷ��ָ�����Ƿ�
�뷢�͵�ָ����һ�¡�У����Ƿ���ȷ��
	*/
	if(MG200rev.RevBuf[0]!=STARTCODE)
		return RECEIVE_PARAMETER_ERROR;
	
	if(MG200rev.RevBuf[1]!=0x62)
		return RECEIVE_PARAMETER_ERROR;
	
	if(MG200rev.RevBuf[2]!=0x63)
		return RECEIVE_PARAMETER_ERROR;
	
	if(MG200rev.RevBuf[3]!=cmd)
		return RECEIVE_PARAMETER_ERROR;
	
	checksum =(0x62+0x63+cmd+MG200rev.RevBuf[4]+MG200rev.RevBuf[5]+RESERVE)&0xFF;
	if(MG200rev.RevBuf[7]!=(checksum))
		return RECEIVE_CHECKSUM_ERROR;

	*result =MG200rev.RevBuf[4];
	*param=MG200rev.RevBuf[5];
	
	memset(MG200rev.RevBuf,0,sizeof(MG200rev.RevBuf));
	
	return NO_ERROR;
}


/*
�������ܣ�ץȡָ��ͼ��ͬʱ��ȡ������
����˵���� param-���� 00h, 01h, 02h, 03h, 04h ������ 3~5 ��֮�䣬 �Զ���ѡ��ע��ָ�ƵĴ�����
����ֵ����
ע�����
*/
u8 CaptureAndExtract(u8 param)
{
	u8 result;
	u8 ackparam;
	
	MG200SendCommond(0x51,param);
	if(NO_ERROR!=MG200Ack(0x51,&result,&ackparam))
		return ACK_ERR;
/*	
����ֵresult��	
00h: ץȡָ��ͼ����������ȡ�ɹ�
B1h : ָ�ƹ�С(small finger)
B2h : ��ָ�� (no finger)
B3h : ָ��λ�ù��ڿ�����Ҫ����ָ���ұ�������left finger��
B4h : ָ��λ�ù��ڿ��ң���Ҫ����ָ�����������right finger��
B5h : ָ��λ�ù��ڿ��ϣ���Ҫ����ָ���±�������up finger��
B6h : ָ��λ�ù��ڿ��£���Ҫ����ָ���ϱ����� (down finger��
B7h : ʪ��ָ ��ָ�Ʊ����������ˮ�չ��ࣩ (wet finger)
B8h : ������ָ(dry finger)
C0h: ������ȡ����
C2h: ������ȡ���� (ͼ��������)
����: ץȡָ��ͼ����������ȡ����
*/
//	switch(result)
//	{
//		case 0x00:printf("ץȡָ��ͼ����������ȡ�ɹ�\r\n");break;
//		case 0xB1:printf("small finger\r\n");break;
//		case 0xB2:printf("no finger\r\n");break;
//		case 0xB3:printf("left finger\r\n");break;
//		case 0xB4:printf("right finger\r\n");break;
//		case 0xB5:printf("up finger\r\n");break;
//		case 0xB6:printf("down finger\r\n");break;
//		case 0xB7:printf("wet finger\r\n");break;
//		case 0xB8:printf("dry finger\r\n");break;
//		case 0xC0:printf("������ȡ����\r\n");break;
//		case 0xC2:printf("������ȡ���� (ͼ��������)\r\n");break;
//		default :printf("ץȡָ��ͼ����������ȡ����\r\n");break;
//	}
	return result;
}




/*
�������ܣ�ע����ָ���û�
����˵���� param-���� 00h, 01h, 02h, 03h, 04h ������ 3~5 ��֮�䣬 �Զ���ѡ��ע��ָ�ƵĴ�����
����ֵ��
00h: ע��ɹ�
83h: ID ����(ID < 0 ���� ID > ����û���)����ͨ�Ŵ���
91h: ע��ʧ��(�û���������)
93h: �Ѿ�ע��� ID
94h: ָ����ȡ���� < 3
ע��������ص�Ԫ MCU ��ʹ�ø�ע���û�ָ��ǰ������ִ�� 3~5 �Σ�ִ�д����ɸ����û�
					�����ƣ� ReqCaptureAndExtract ָ���ȡҪע���û�ָ�Ƶ���������
*/
u8 Enroll(void)
{
	u8 cnt=5;
	u8 result;
	u8 ackparam;
	
	//ץȡָ�Ƶ�����3��
	//printf("��һ��¼�룬�������ָ\r\n");
	oled_display_arbitrary_string(4 ,0, (u8*)"First finger");
	while(!MG200_DETECT);//�ȴ�����
	result=CaptureAndExtract(0x00);//��һ��ץȡָ�Ƶ�����-����0x00
	while( result!=0x00 )   
	{
		while(!MG200_DETECT);
		result=CaptureAndExtract(0x00);
	}
//	oled_display_arbitrary_string(4 ,0, (u8*)"              ");
		oled_display_clear();
	//printf("�ڶ���¼�룬�������ָ\r\n");
	oled_display_arbitrary_string(4 ,0, (u8*)"Second finger");
	while(!MG200_DETECT);//�ȴ�����
	result=CaptureAndExtract(0x01);//�ڶ���ץȡָ�Ƶ�����-����0x01
	while( result!=0x00 )
	{
		while(!MG200_DETECT);
		result=CaptureAndExtract(0x01);
	}
	oled_display_arbitrary_string(4 ,0, (u8*)"              ");
	
	//printf("������¼�룬�������ָ\r\n");
	oled_display_arbitrary_string(4 ,0, (u8*)"Third finger");
	while(!MG200_DETECT);//�ȴ�����
	result=CaptureAndExtract(0x02);//������ץȡָ�Ƶ�����-����0x02
	while( result!=0x00 )
	{
		while(!MG200_DETECT);
		result=CaptureAndExtract(0x02);
	}
	oled_display_arbitrary_string(4 ,0, (u8*)"              ");
	
	
	do
	{
		MG200SendCommond(0x7F,0x00);//ע��� ID �ţ�ID ��Χ 1~100������û���Ϊ 100�� ���ò���Ϊ 00h ʱ��ģ��ע��ɹ��� ID ����ģ���ڲ��Զ�����ģ�
		if(NO_ERROR==MG200Ack(0x7F,&result,&ackparam))
			break;
	}while(cnt--);
	
	if(cnt==0)
		result=ACK_ERR;
	
	switch(result)
	{
		case 0x00:printf("ע��ɹ�\r\n");break; 
		case 0x83:printf("ID ����(ID < 0 ���� ID > ����û���)����ͨ�Ŵ���\r\n");break;
		case 0x91:printf("ע��ʧ��(�û���������)\r\n");break;
		case 0x93:printf("�Ѿ�ע��� ID\r\n");break;
		case 0x94:printf("ָ����ȡ���� < 3\r\n");break;
		
	}

	return result;
	
}

/*
�������ܣ��ɼ�ָ�ƽ��� 1�� N �ȶ� (0x71)
����˵���� 
����ֵ�����زɼ�ָ�ƽ��� 1�� N �ȶԵĽ��
				0-û�и�ָ��  1~100�ȶԳɹ����û�ָ�� ID ��  0xFFʧ��
ע�����
*/
u8 Match1N(void)
{
	u8 cnt=5;
	u8 result;
	u8 ackparam;
	while(!MG200_DETECT);//�ȴ�����
	result=CaptureAndExtract(0x00);//ץȡָ�Ƶ�����-����0x00
	while( result!=0x00 )
	{
		while(!MG200_DETECT);
		result=CaptureAndExtract(0x00);
	}
	
	do{
		MG200SendCommond(0x71,0x00);
		if(NO_ERROR==MG200Ack(0x71,&result,&ackparam))
			break;
	}while(cnt--);
	
	if(cnt==0)
		result=ACK_ERR;
	
	if(0x00==result)
	{
		oled_display_clear();
		oled_display_arbitrary_string(4,0,(u8 *)"�ȶԳɹ�");
		printf("�ȶԳɹ�\r\n");
		return ackparam;//ID
	}
	else if(0x92==result)
	{
		oled_display_clear();
		oled_display_arbitrary_string(4,0,(u8 *)"û�и�ָ��");//�ȶ�ʧ��
		printf("û�и�ָ��\r\n");
		return 0;//ID==0
	}
	else 
	{
		oled_display_clear();
		oled_display_arbitrary_string(4,0,(u8 *)"ʧ��");
		printf("ʧ��\r\n");
		return 0xFF;
	}
}


/*
�������ܣ�ɾ��ָ�� ID �û�(0x73)
����˵����ID-ָ��ɾ���� ID �ţ��û� ID �� 1~100��������ֵΪ 01h~64h)
����ֵ��
00h�� SUCCESS
83h: ��������
90h: δע����û�
FFh: д�� ROM ����
ע�����
*/
u8 EraseOne(u8 ID)
{
	u8 cnt=5;
	
	u8 result;
	u8 ackparam;
	
	do
	{
		MG200SendCommond(0x73,ID);
		if(NO_ERROR==MG200Ack(0x73,&result,&ackparam))
			break;
	}while(cnt--);
	
	if(cnt==0)
		result=ACK_ERR;
	
	switch(result)
	{
		case 0x00:printf("ɾ���ɹ�\r\n");break;
		case 0x83:printf("��������\r\n");break;
		case 0x90:printf("δע����û�\r\n");break;
		case 0xFF:printf("δע����û�\r\n");break;
//				case 0x00:oled_display_clear(); oled_display_arbitrary_string(4 ,0, (u8*)"ɾ���ɹ�\r\n");break;
//				case 0x83:oled_display_clear();oled_display_arbitrary_string(4 ,0, (u8*)"ɾ��ʧ��\r\n");break;
//				case 0x90:oled_display_clear();oled_display_arbitrary_string(4 ,0, (u8*)"δע����û�\r\n");break;
//				case 0xFF:oled_display_clear();oled_display_arbitrary_string(4 ,0, (u8*)"δע����û�\r\n");break;
		
	}
	
	return result;
}

/*
�������ܣ�ɾ�������û�(0x54)
����˵���� 
����ֵ��
00h: SUCCESS
90h: ɾ��ʧ��
��ע������Ϊ 0��
������ɾ��ʧ��
ע�����
*/
u8 EraseAll(void)
{
	u8 cnt=5;
	u8 result;
	u8 ackparam;
	printf("����ɾ��\r\n");
	do{
		MG200SendCommond(0x54,0x00);
		if(NO_ERROR==MG200Ack(0x54,&result,&ackparam))
		break;
	}while(cnt--);
	
	if(cnt==0)
	{
		result=ACK_ERR;
	}
	
	
	switch(result)
	{
		case 0x00:printf("ɾ���ɹ�\r\n");break;
		case 0x90:printf("ɾ��ʧ�ܣ�ע������Ϊ 0��\r\n");break;
		default:printf("ɾ��ʧ��\r\n");break;
		
	}
	printf("ɾ������\r\n");
	return result;
}


/*
�������ܣ���ȡ��ע���û�����(0x55)
����˵���� 
����ֵ��
00h: ʧ��
������ָ������
ע�����
*/
u8  GetUserNum(void)
{
	u8 cnt=5;
	u8 result;
	u8 ackparam;
	
//	MG200SendCommond(0x55,0x00);
//	if(NO_ERROR!=MG200Ack(0x55,&result,&ackparam))
//		return ACK_ERR;
	do
	{
		MG200SendCommond(0x55,0x00);
		if(NO_ERROR==MG200Ack(0x55,&result,&ackparam))
			break;
	}while(cnt--);
	
	if(cnt==0)
		ackparam=ACK_ERR;
	
	return ackparam;


}

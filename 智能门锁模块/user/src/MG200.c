#include "MG200.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "oled_memory.h"
TYPE_U2 MG200rev;
void MG200Init(void)
{
	//PC13--Detect：感应上电信号（手指触摸指纹传感器时输出高电平）
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
函数功能：给MG200发送一个字节数据
参数说明：待发送的数据
返回值：
注意事项：
*/
void MG200SendByte(u8 data)
{
	while((USART2->SR & (0X01<<7))==0);  //等待发送缓冲区为空
	USART2->DR=data;
}

/*
函数功能：MCU 向指纹模块发送指令
参数说明：cmd-指令码   parm-参数
返回值：无
注意事项：
*/
void MG200SendCommond(u8 cmd,u8 param)
{
	u8 checksum=0;//校验和
	
	//校验和 为了确认发送数据包的正确性而设，是除去起始码之外的所有字节按照
	//8bit 单位相加的结果（溢出部分将被无视）
	checksum =(SENDADDR+REVADDR+cmd+param+RESERVE)&0xFF;

	MG200SendByte(STARTCODE);//起始码
	MG200SendByte(SENDADDR);//发送地址
	MG200SendByte(REVADDR);//接收地址
	MG200SendByte(cmd);//指令码 表示指令类型
	MG200SendByte(param);//参数 按照指令码的不同，可能存在的参数（数据）
	MG200SendByte(RESERVE);//预留字节， 后续扩展使用；目前未使用，默认为 00h
	MG200SendByte(checksum);//校验和

}


/*
函数功能：指纹模块对指令的回复包
参数说明：cmd-所回复的指令码   
返回值：无
注意事项：
*/
u8 MG200Ack(u8 cmd,u8* result,u8* param)
{
	u8 checksum=0;//校验和
	while(!MG200rev.RevOver);//等待U2接收完成
	MG200rev.RevOver=0;
	/*
从指纹模块接收到应答后，主控单元需要检查所接收的数据包是否正确，如果各项都
符合条件就判断为接收到正确回复。即：起始位、收发地址是否正确，指令码是否
与发送的指令码一致、校验和是否正确等
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
函数功能：抓取指纹图像并同时提取特征量
参数说明： param-参数 00h, 01h, 02h, 03h, 04h （可在 3~5 次之间， 自定义选择注册指纹的次数）
返回值：无
注意事项：
*/
u8 CaptureAndExtract(u8 param)
{
	u8 result;
	u8 ackparam;
	
	MG200SendCommond(0x51,param);
	if(NO_ERROR!=MG200Ack(0x51,&result,&ackparam))
		return ACK_ERR;
/*	
返回值result：	
00h: 抓取指纹图像及特征量提取成功
B1h : 指纹过小(small finger)
B2h : 无指纹 (no finger)
B3h : 指纹位置过于靠左，需要将手指靠右边摁按（left finger）
B4h : 指纹位置过于靠右，需要将手指靠左边摁按（right finger）
B5h : 指纹位置过于靠上，需要将手指靠下边摁按（up finger）
B6h : 指纹位置过于靠下，需要将手指靠上边摁按 (down finger）
B7h : 湿手指 （指纹表面出汗或者水渍过多） (wet finger)
B8h : 干燥手指(dry finger)
C0h: 特征提取错误
C2h: 特征提取错误 (图像质量差)
其他: 抓取指纹图像及特征量提取错误
*/
//	switch(result)
//	{
//		case 0x00:printf("抓取指纹图像及特征量提取成功\r\n");break;
//		case 0xB1:printf("small finger\r\n");break;
//		case 0xB2:printf("no finger\r\n");break;
//		case 0xB3:printf("left finger\r\n");break;
//		case 0xB4:printf("right finger\r\n");break;
//		case 0xB5:printf("up finger\r\n");break;
//		case 0xB6:printf("down finger\r\n");break;
//		case 0xB7:printf("wet finger\r\n");break;
//		case 0xB8:printf("dry finger\r\n");break;
//		case 0xC0:printf("特征提取错误\r\n");break;
//		case 0xC2:printf("特征提取错误 (图像质量差)\r\n");break;
//		default :printf("抓取指纹图像及特征量提取错误\r\n");break;
//	}
	return result;
}




/*
函数功能：注册新指纹用户
参数说明： param-参数 00h, 01h, 02h, 03h, 04h （可在 3~5 次之间， 自定义选择注册指纹的次数）
返回值：
00h: 注册成功
83h: ID 错误(ID < 0 或者 ID > 最大用户数)或者通信错误
91h: 注册失败(用户区域已满)
93h: 已经注册的 ID
94h: 指纹提取次数 < 3
注意事项：主控单元 MCU 在使用该注册用户指令前必须先执行 3~5 次（执行次数可根据用户
					需求定制） ReqCaptureAndExtract 指令，提取要注册用户指纹的特征量；
*/
u8 Enroll(void)
{
	u8 cnt=5;
	u8 result;
	u8 ackparam;
	
	//抓取指纹的特征3次
	//printf("第一次录入，请放下手指\r\n");
	oled_display_arbitrary_string(4 ,0, (u8*)"First finger");
	while(!MG200_DETECT);//等待触摸
	result=CaptureAndExtract(0x00);//第一次抓取指纹的特征-参数0x00
	while( result!=0x00 )   
	{
		while(!MG200_DETECT);
		result=CaptureAndExtract(0x00);
	}
//	oled_display_arbitrary_string(4 ,0, (u8*)"              ");
		oled_display_clear();
	//printf("第二次录入，请放下手指\r\n");
	oled_display_arbitrary_string(4 ,0, (u8*)"Second finger");
	while(!MG200_DETECT);//等待触摸
	result=CaptureAndExtract(0x01);//第二次抓取指纹的特征-参数0x01
	while( result!=0x00 )
	{
		while(!MG200_DETECT);
		result=CaptureAndExtract(0x01);
	}
	oled_display_arbitrary_string(4 ,0, (u8*)"              ");
	
	//printf("第三次录入，请放下手指\r\n");
	oled_display_arbitrary_string(4 ,0, (u8*)"Third finger");
	while(!MG200_DETECT);//等待触摸
	result=CaptureAndExtract(0x02);//第三次抓取指纹的特征-参数0x02
	while( result!=0x00 )
	{
		while(!MG200_DETECT);
		result=CaptureAndExtract(0x02);
	}
	oled_display_arbitrary_string(4 ,0, (u8*)"              ");
	
	
	do
	{
		MG200SendCommond(0x7F,0x00);//注册的 ID 号（ID 范围 1~100，最大用户数为 100； 当该参数为 00h 时，模块注册成功的 ID 号是模块内部自动分配的）
		if(NO_ERROR==MG200Ack(0x7F,&result,&ackparam))
			break;
	}while(cnt--);
	
	if(cnt==0)
		result=ACK_ERR;
	
	switch(result)
	{
		case 0x00:printf("注册成功\r\n");break; 
		case 0x83:printf("ID 错误(ID < 0 或者 ID > 最大用户数)或者通信错误\r\n");break;
		case 0x91:printf("注册失败(用户区域已满)\r\n");break;
		case 0x93:printf("已经注册的 ID\r\n");break;
		case 0x94:printf("指纹提取次数 < 3\r\n");break;
		
	}

	return result;
	
}

/*
函数功能：采集指纹进行 1： N 比对 (0x71)
参数说明： 
返回值：返回采集指纹进行 1： N 比对的结果
				0-没有该指纹  1~100比对成功的用户指纹 ID 号  0xFF失败
注意事项：
*/
u8 Match1N(void)
{
	u8 cnt=5;
	u8 result;
	u8 ackparam;
	while(!MG200_DETECT);//等待触摸
	result=CaptureAndExtract(0x00);//抓取指纹的特征-参数0x00
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
		oled_display_arbitrary_string(4,0,(u8 *)"比对成功");
		printf("比对成功\r\n");
		return ackparam;//ID
	}
	else if(0x92==result)
	{
		oled_display_clear();
		oled_display_arbitrary_string(4,0,(u8 *)"没有该指纹");//比对失败
		printf("没有该指纹\r\n");
		return 0;//ID==0
	}
	else 
	{
		oled_display_clear();
		oled_display_arbitrary_string(4,0,(u8 *)"失败");
		printf("失败\r\n");
		return 0xFF;
	}
}


/*
函数功能：删除指定 ID 用户(0x73)
参数说明：ID-指定删除的 ID 号（用户 ID 号 1~100，参数数值为 01h~64h)
返回值：
00h： SUCCESS
83h: 参数错误
90h: 未注册的用户
FFh: 写入 ROM 错误
注意事项：
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
		case 0x00:printf("删除成功\r\n");break;
		case 0x83:printf("参数错误\r\n");break;
		case 0x90:printf("未注册的用户\r\n");break;
		case 0xFF:printf("未注册的用户\r\n");break;
//				case 0x00:oled_display_clear(); oled_display_arbitrary_string(4 ,0, (u8*)"删除成功\r\n");break;
//				case 0x83:oled_display_clear();oled_display_arbitrary_string(4 ,0, (u8*)"删除失败\r\n");break;
//				case 0x90:oled_display_clear();oled_display_arbitrary_string(4 ,0, (u8*)"未注册的用户\r\n");break;
//				case 0xFF:oled_display_clear();oled_display_arbitrary_string(4 ,0, (u8*)"未注册的用户\r\n");break;
		
	}
	
	return result;
}

/*
函数功能：删除所有用户(0x54)
参数说明： 
返回值：
00h: SUCCESS
90h: 删除失败
（注册数量为 0）
其他：删除失败
注意事项：
*/
u8 EraseAll(void)
{
	u8 cnt=5;
	u8 result;
	u8 ackparam;
	printf("进入删除\r\n");
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
		case 0x00:printf("删除成功\r\n");break;
		case 0x90:printf("删除失败（注册数量为 0）\r\n");break;
		default:printf("删除失败\r\n");break;
		
	}
	printf("删除结束\r\n");
	return result;
}


/*
函数功能：获取已注册用户数量(0x55)
参数说明： 
返回值：
00h: 失败
其他：指纹数量
注意事项：
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

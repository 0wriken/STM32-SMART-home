
#include "MCP2515.H"




/*******************************************************************************
* 函数名  : Delay_Nms
* 描述    : 通过软件延时约nms(不准确)
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 此方式延时时间是不准确的,准确延时建议用定时器
*******************************************************************************/
void Delay_Nms(unsigned int x)
{
	unsigned int y;

	for (;x>0;x--)
		for (y=0;y<100;y++);
}


u8 SPI_Receive_SendByte(u8 byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_GetFlagStatus( SPI_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_SendData(byte);

  /* Wait to receive a byte */
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_ReceiveData();	 
}


/*******************************************************************************
* 函数名  : MCP2515_WriteByte
* 描述    : 通过SPI向MCP2515指定地址寄存器写1个字节数据
* 输入    : addr:MCP2515寄存器地址,dat:待写入的数据
* 输出    : 无
* 返回值  : 无
* 说明    : 无
*******************************************************************************/
void MCP2515_WriteByte(unsigned char addr,unsigned char dat)
{
	MCP2515_CS(L);				//置MCP2515的CS为低电平
	SPI_Receive_SendByte(CAN_WRITE);	//发送写命令
	SPI_Receive_SendByte(addr);			//发送地址
	SPI_Receive_SendByte(dat);			//写入数据
	MCP2515_CS(H);				//置MCP2515的CS为高电平 
}

/*******************************************************************************
* 函数名  : MCP2515_ReadByte
* 描述    : 通过SPI从MCP2515指定地址寄器读1个字节数据
* 输入    : addr:MCP2515寄存器地址
* 输出    : 无
* 返回值  : rByte:读取到寄存器的1个字节数据
* 说明    : 无
*******************************************************************************/
unsigned char MCP2515_ReadByte(unsigned char addr)
{
	unsigned char rByte;
	
	MCP2515_CS(L);				//置MCP2515的CS为低电平
	SPI_Receive_SendByte(CAN_READ);		//发送读命令
	SPI_Receive_SendByte(addr);			//发送地址
	rByte = SPI_Receive_SendByte(0);		//读取数据
	MCP2515_CS(H);				//置MCP2515的CS为高电平
	return rByte;				//返回读到的一个字节数据
}

/*******************************************************************************
* 函数名  : MCP2515_Reset
* 描述    : 发送复位指令软件复位MCP2515
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 将内部寄存器复位为缺省状态,并将器件设定为配置模式
*******************************************************************************/
void MCP2515_Reset(void)
{
    MCP2515_CS(L);				//置MCP2515的CS为低电平
    SPI_Receive_SendByte(CAN_RESET);	//发送寄存器复位命令
    MCP2515_CS(H);				//置MCP2515的CS为高电平
}

/*******************************************************************************
* 函数名  : MCP2515_Init
* 描述    : MCP2515初始化配置
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 初始化包括：软件复位、工作波特率设置、标识符相关配置等。
*******************************************************************************/
void MCP2515_Init(void)
{
	unsigned char temp=0;

   SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_8,  SPI_MODE_MASTER,\
       SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE,  \
       SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
   SPI_Cmd(ENABLE);
   GPIO_Init(GPIOA , GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);//spi_cs

    
	Delay_Nms(2000);        //通过软件延时约nms(不准确)
	MCP2515_Reset();	//发送复位指令软件复位MCP2515
	Delay_Nms(2);		//通过软件延时约nms(不准确)

	//设置波特率为125Kbps ----16个TQ
	//set CNF1,SJW=00,长度为1TQ,TQ=[2*(BRP+1)]/Fsoc=2*2/8M=0.5US      
	MCP2515_WriteByte(CNF1,1);    
	Delay_Nms(2);		//通过软件延时约nms(不准确)
	//set CNF2
	MCP2515_WriteByte(CNF2,0x80|PHSEG1_4TQ|PRSEG_7TQ);
	Delay_Nms(2);		//通过软件延时约nms(不准确)
	//set CNF3,PHSEG2=(2+1)TQ=3TQ,同时当CANCTRL.CLKEN=1时设定CLKOUT引脚为时间输出使能位
	MCP2515_WriteByte(CNF3,PHSEG2_4TQ);
	Delay_Nms(2);		//通过软件延时约nms(不准确)
    
    
	//发送ID配置----ID：0XAE                                                                                                                                          
	MCP2515_WriteByte(TXB0SIDH,0x15);//发送缓冲器0标准标识符高位
	Delay_Nms(2);		//通过软件延时约nms(不准确)
	MCP2515_WriteByte(TXB0SIDL,0xC0);//发送缓冲器0标准标识符低位
	Delay_Nms(2);		//通过软件延时约nms(不准确)
	
    
	MCP2515_WriteByte(RXB0SIDH,0x00);//清空接收缓冲器0的标准标识符高位
	Delay_Nms(2);		//通过软件延时约nms(不准确)
	MCP2515_WriteByte(RXB0SIDL,0x00);//清空接收缓冲器0的标准标识符低位
	Delay_Nms(2);		//通过软件延时约nms(不准确)
	MCP2515_WriteByte(RXB0CTRL,0x20);//仅仅接收标准标识符的有效信息
	Delay_Nms(2);		//通过软件延时约nms(不准确)
	MCP2515_WriteByte(RXB0DLC,DLC_8);//设置接收数据的长度为8个字节
	Delay_Nms(2);		//通过软件延时约nms(不准确)
	
    //接收ID配置----ID：0XAE
	MCP2515_WriteByte(RXF0SIDH,0x15);//配置验收滤波寄存器n标准标识符高位
	Delay_Nms(2);		//通过软件延时约nms(不准确)
	MCP2515_WriteByte(RXF0SIDL,0xC0);//配置验收滤波寄存器n标准标识符低位
	Delay_Nms(2);		//通过软件延时约nms(不准确)
	MCP2515_WriteByte(RXM0SIDH,0xFF);//配置验收屏蔽寄存器n标准标识符高位
	Delay_Nms(2);		//通过软件延时约nms(不准确)
	MCP2515_WriteByte(RXM0SIDL,0xE0);//配置验收屏蔽寄存器n标准标识符低位
	Delay_Nms(2);		//通过软件延时约nms(不准确)
	
	MCP2515_WriteByte(CANINTF,0x00);//清空CAN中断标志寄存器的所有位(必须由MCU清空)
	Delay_Nms(2);		//通过软件延时约nms(不准确)
	MCP2515_WriteByte(CANINTE,0x01);//配置CAN中断使能寄存器的接收缓冲器0满中断使能,其它位禁止中断
	Delay_Nms(2);		//通过软件延时约nms(不准确)
	
	MCP2515_WriteByte(CANCTRL,REQOP_NORMAL|CLKOUT_ENABLED);//将MCP2515设置为正常模式,退出配置模式
       // MCP2515_WriteByte(CANCTRL,REQOP_LOOPBACK|CLKOUT_ENABLED);//将MCP2515设置为环回模式,退出配置模式
	Delay_Nms(2);		//通过软件延时约nms(不准确)
	
	temp=MCP2515_ReadByte(CANSTAT);//读取CAN状态寄存器的值
	Delay_Nms(2);		//通过软件延时约nms(不准确)
	if(OPMODE_NORMAL!=(temp&0xE0))//判断MCP2515是否已经进入正常模式
	{
            MCP2515_WriteByte(CANCTRL,REQOP_NORMAL|CLKOUT_ENABLED);//再次将MCP2515设置为正常模式,退出配置模式
	}
}

/*******************************************************************************
* 函数名  : CAN_Send_Buffer
* 描述    : CAN发送指定长度的数据
* 输入    : *CAN_TX_Buf(待发送数据缓冲区指针),len(待发送数据长度)
* 输出    : 无
* 返回值  : 无
* 说明    : 无
*******************************************************************************/
void CAN_Send_Buffer(unsigned char *CAN_TX_Buf,unsigned char len)
{
	unsigned char j,dly,count;

	count=0;
	while(count<len)
	{
		dly=0;
		while((MCP2515_ReadByte(TXB0CTRL)&0x08) && (dly<50))//快速读某些状态指令,等待TXREQ标志清零
		{
			Delay_Nms(1);//通过软件延时约nms(不准确)
			dly++;
		}
				
		for(j=0;j<8;)
		{
			MCP2515_WriteByte(TXB0D0+j,CAN_TX_Buf[count++]);//将待发送的数据写入发送缓冲寄存器
			j++;
			if(count>=len) break;
		}
		MCP2515_WriteByte(TXB0DLC,j);//将本帧待发送的数据长度写入发送缓冲器0的发送长度寄存器
                Delay_Nms(2);
		MCP2515_CS(L);
		MCP2515_WriteByte(TXB0CTRL,0x08);//请求发送报文
		MCP2515_CS(H);
        Delay_Nms(2);
                
        CAN_TX_Buf[0] ^= CAN_TX_Buf[1];
        CAN_TX_Buf[1] ^= CAN_TX_Buf[0];
        CAN_TX_Buf[0] ^= CAN_TX_Buf[1];
	}
}

/*******************************************************************************
* 函数名  : CAN_Receive_Buffer
* 描述    : CAN接收一帧数据
* 输入    : *CAN_TX_Buf(待接收数据缓冲区指针)
* 输出    : 无
* 返回值  : len(接收到数据的长度,0~8字节)
* 说明    : 无
*******************************************************************************/
unsigned char CAN_Receive_Buffer(unsigned char *CAN_RX_Buf)
{
	unsigned char i=0,len=0,temp=0;

	temp = MCP2515_ReadByte(CANINTF);
	if(temp & 0x01)
	{
		len=MCP2515_ReadByte(RXB0DLC);//读取接收缓冲器0接收到的数据长度(0~8个字节)
		while(i<len)
		{	
			CAN_RX_Buf[i]=MCP2515_ReadByte(RXB0D0+i);//把CAN接收到的数据放入指定缓冲区
			i++;
		}
	}
	MCP2515_WriteByte(CANINTF,0);//清除中断标志位(中断标志寄存器必须由MCU清零)
	return len;
}




#include "MCP2515.H"




/*******************************************************************************
* ������  : Delay_Nms
* ����    : ͨ�������ʱԼnms(��׼ȷ)
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : �˷�ʽ��ʱʱ���ǲ�׼ȷ��,׼ȷ��ʱ�����ö�ʱ��
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
* ������  : MCP2515_WriteByte
* ����    : ͨ��SPI��MCP2515ָ����ַ�Ĵ���д1���ֽ�����
* ����    : addr:MCP2515�Ĵ�����ַ,dat:��д�������
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void MCP2515_WriteByte(unsigned char addr,unsigned char dat)
{
	MCP2515_CS(L);				//��MCP2515��CSΪ�͵�ƽ
	SPI_Receive_SendByte(CAN_WRITE);	//����д����
	SPI_Receive_SendByte(addr);			//���͵�ַ
	SPI_Receive_SendByte(dat);			//д������
	MCP2515_CS(H);				//��MCP2515��CSΪ�ߵ�ƽ 
}

/*******************************************************************************
* ������  : MCP2515_ReadByte
* ����    : ͨ��SPI��MCP2515ָ����ַ������1���ֽ�����
* ����    : addr:MCP2515�Ĵ�����ַ
* ���    : ��
* ����ֵ  : rByte:��ȡ���Ĵ�����1���ֽ�����
* ˵��    : ��
*******************************************************************************/
unsigned char MCP2515_ReadByte(unsigned char addr)
{
	unsigned char rByte;
	
	MCP2515_CS(L);				//��MCP2515��CSΪ�͵�ƽ
	SPI_Receive_SendByte(CAN_READ);		//���Ͷ�����
	SPI_Receive_SendByte(addr);			//���͵�ַ
	rByte = SPI_Receive_SendByte(0);		//��ȡ����
	MCP2515_CS(H);				//��MCP2515��CSΪ�ߵ�ƽ
	return rByte;				//���ض�����һ���ֽ�����
}

/*******************************************************************************
* ������  : MCP2515_Reset
* ����    : ���͸�λָ�������λMCP2515
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ���ڲ��Ĵ�����λΪȱʡ״̬,���������趨Ϊ����ģʽ
*******************************************************************************/
void MCP2515_Reset(void)
{
    MCP2515_CS(L);				//��MCP2515��CSΪ�͵�ƽ
    SPI_Receive_SendByte(CAN_RESET);	//���ͼĴ�����λ����
    MCP2515_CS(H);				//��MCP2515��CSΪ�ߵ�ƽ
}

/*******************************************************************************
* ������  : MCP2515_Init
* ����    : MCP2515��ʼ������
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��ʼ�������������λ���������������á���ʶ��������õȡ�
*******************************************************************************/
void MCP2515_Init(void)
{
	unsigned char temp=0;

   SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_8,  SPI_MODE_MASTER,\
       SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE,  \
       SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
   SPI_Cmd(ENABLE);
   GPIO_Init(GPIOA , GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);//spi_cs

    
	Delay_Nms(2000);        //ͨ�������ʱԼnms(��׼ȷ)
	MCP2515_Reset();	//���͸�λָ�������λMCP2515
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)

	//���ò�����Ϊ125Kbps ----16��TQ
	//set CNF1,SJW=00,����Ϊ1TQ,TQ=[2*(BRP+1)]/Fsoc=2*2/8M=0.5US      
	MCP2515_WriteByte(CNF1,1);    
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
	//set CNF2
	MCP2515_WriteByte(CNF2,0x80|PHSEG1_4TQ|PRSEG_7TQ);
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
	//set CNF3,PHSEG2=(2+1)TQ=3TQ,ͬʱ��CANCTRL.CLKEN=1ʱ�趨CLKOUT����Ϊʱ�����ʹ��λ
	MCP2515_WriteByte(CNF3,PHSEG2_4TQ);
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
    
    
	//����ID����----ID��0XAE                                                                                                                                          
	MCP2515_WriteByte(TXB0SIDH,0x15);//���ͻ�����0��׼��ʶ����λ
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
	MCP2515_WriteByte(TXB0SIDL,0xC0);//���ͻ�����0��׼��ʶ����λ
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
	
    
	MCP2515_WriteByte(RXB0SIDH,0x00);//��ս��ջ�����0�ı�׼��ʶ����λ
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
	MCP2515_WriteByte(RXB0SIDL,0x00);//��ս��ջ�����0�ı�׼��ʶ����λ
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
	MCP2515_WriteByte(RXB0CTRL,0x20);//�������ձ�׼��ʶ������Ч��Ϣ
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
	MCP2515_WriteByte(RXB0DLC,DLC_8);//���ý������ݵĳ���Ϊ8���ֽ�
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
	
    //����ID����----ID��0XAE
	MCP2515_WriteByte(RXF0SIDH,0x15);//���������˲��Ĵ���n��׼��ʶ����λ
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
	MCP2515_WriteByte(RXF0SIDL,0xC0);//���������˲��Ĵ���n��׼��ʶ����λ
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
	MCP2515_WriteByte(RXM0SIDH,0xFF);//�����������μĴ���n��׼��ʶ����λ
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
	MCP2515_WriteByte(RXM0SIDL,0xE0);//�����������μĴ���n��׼��ʶ����λ
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
	
	MCP2515_WriteByte(CANINTF,0x00);//���CAN�жϱ�־�Ĵ���������λ(������MCU���)
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
	MCP2515_WriteByte(CANINTE,0x01);//����CAN�ж�ʹ�ܼĴ����Ľ��ջ�����0���ж�ʹ��,����λ��ֹ�ж�
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
	
	MCP2515_WriteByte(CANCTRL,REQOP_NORMAL|CLKOUT_ENABLED);//��MCP2515����Ϊ����ģʽ,�˳�����ģʽ
       // MCP2515_WriteByte(CANCTRL,REQOP_LOOPBACK|CLKOUT_ENABLED);//��MCP2515����Ϊ����ģʽ,�˳�����ģʽ
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
	
	temp=MCP2515_ReadByte(CANSTAT);//��ȡCAN״̬�Ĵ�����ֵ
	Delay_Nms(2);		//ͨ�������ʱԼnms(��׼ȷ)
	if(OPMODE_NORMAL!=(temp&0xE0))//�ж�MCP2515�Ƿ��Ѿ���������ģʽ
	{
            MCP2515_WriteByte(CANCTRL,REQOP_NORMAL|CLKOUT_ENABLED);//�ٴν�MCP2515����Ϊ����ģʽ,�˳�����ģʽ
	}
}

/*******************************************************************************
* ������  : CAN_Send_Buffer
* ����    : CAN����ָ�����ȵ�����
* ����    : *CAN_TX_Buf(���������ݻ�����ָ��),len(���������ݳ���)
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void CAN_Send_Buffer(unsigned char *CAN_TX_Buf,unsigned char len)
{
	unsigned char j,dly,count;

	count=0;
	while(count<len)
	{
		dly=0;
		while((MCP2515_ReadByte(TXB0CTRL)&0x08) && (dly<50))//���ٶ�ĳЩ״ָ̬��,�ȴ�TXREQ��־����
		{
			Delay_Nms(1);//ͨ�������ʱԼnms(��׼ȷ)
			dly++;
		}
				
		for(j=0;j<8;)
		{
			MCP2515_WriteByte(TXB0D0+j,CAN_TX_Buf[count++]);//�������͵�����д�뷢�ͻ���Ĵ���
			j++;
			if(count>=len) break;
		}
		MCP2515_WriteByte(TXB0DLC,j);//����֡�����͵����ݳ���д�뷢�ͻ�����0�ķ��ͳ��ȼĴ���
                Delay_Nms(2);
		MCP2515_CS(L);
		MCP2515_WriteByte(TXB0CTRL,0x08);//�����ͱ���
		MCP2515_CS(H);
        Delay_Nms(2);
                
        CAN_TX_Buf[0] ^= CAN_TX_Buf[1];
        CAN_TX_Buf[1] ^= CAN_TX_Buf[0];
        CAN_TX_Buf[0] ^= CAN_TX_Buf[1];
	}
}

/*******************************************************************************
* ������  : CAN_Receive_Buffer
* ����    : CAN����һ֡����
* ����    : *CAN_TX_Buf(���������ݻ�����ָ��)
* ���    : ��
* ����ֵ  : len(���յ����ݵĳ���,0~8�ֽ�)
* ˵��    : ��
*******************************************************************************/
unsigned char CAN_Receive_Buffer(unsigned char *CAN_RX_Buf)
{
	unsigned char i=0,len=0,temp=0;

	temp = MCP2515_ReadByte(CANINTF);
	if(temp & 0x01)
	{
		len=MCP2515_ReadByte(RXB0DLC);//��ȡ���ջ�����0���յ������ݳ���(0~8���ֽ�)
		while(i<len)
		{	
			CAN_RX_Buf[i]=MCP2515_ReadByte(RXB0D0+i);//��CAN���յ������ݷ���ָ��������
			i++;
		}
	}
	MCP2515_WriteByte(CANINTF,0);//����жϱ�־λ(�жϱ�־�Ĵ���������MCU����)
	return len;
}



#include "Can.h"
	    
/*
�������ܣ���ʼ��CAN1
�����βΣ�brp����Ƶ����tbs1��ʱ���1��tbs2��ʱ���2��tsjw��ͬ����ת��mode��ģʽ
*/
u8 init_can1(u8 brp, u8 tbs1, u8 tbs2, u8 tsjw, u8 mode)
{
	u16 cnt=0;

	RCC->AHB1ENR|=1<<0;  	//ʹ��PORTA��ʱ�� 
	
	GPIOA->MODER &= ~(3<<22);
	GPIOA->MODER |= (2<<22); //PA11 ���ù���
	GPIOA->MODER &= ~(3<<24);
	GPIOA->MODER |= (2<<24); //PA12 ���ù��� 
	
	GPIOA->OTYPER &= ~(3 << 11);//����  ����
	
	GPIOA->OSPEEDR &= ~(0xf<< 22);//�ٶ�  50M
	GPIOA->OSPEEDR |= 0xa << 22;  
	
	GPIOA->AFR[1] &= ~(0XF << 12);
	GPIOA->AFR[1] |= 9 << 12;  //PA11  can1����   AF9
	GPIOA->AFR[1] &= ~(0XF << 16);
	GPIOA->AFR[1] |= 9 << 16;  //PA12  can1����   AF9
	
	
	RCC->APB1ENR |= 1<<25;//ʹ��CAN1ʱ�� CAN1ʹ�õ���APB1��ʱ��(max:42M)
	
	CAN1->MCR = 0;		//�˳�˯��ģʽ(ͬʱ��������λΪ0)
	/* 	
		��ʱ�䴥��ͨ��ģʽ
		����Զ����߹���
		˯��ģʽͨ���������(���CAN1->MCR��SLEEPλ)
		���Ĳ�����,�µĸ��Ǿɵ�
		���ȼ��ɱ��ı�ʶ������
	*/
	
	CAN1->MCR |= 1<<0;	//����CAN�����ʼ��ģʽ
	while ((CAN1->MSR & 1 << 0) == 0)
	{
		if (++cnt > 100)
		{
			return 2;	//�����ʼ��ģʽʧ��----��ʱ���
		}
	}
	
	CAN1->MCR |= 1<<4;	//��ֹ�����Զ��ط�
	
	CAN1->BTR = 0;	//���ԭ��������.
	CAN1->BTR |= mode<<30;	//ģʽ���� 0,��ͨģʽ;1,�ػ�ģʽ;
	CAN1->BTR |= (tsjw - 1) << 24; 	//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ
	CAN1->BTR |= (tbs2 - 1) << 20; 	//Tbs2=tbs2+1��ʱ�䵥λ
	CAN1->BTR |= (tbs1 - 1) << 16;	//Tbs1=tbs1+1��ʱ�䵥λ
	CAN1->BTR |= (brp - 1) ;  	//��Ƶϵ��(Fdiv)Ϊbrp+1
								//������:Fpclk1/((Tbs1+Tbs2+1)*Fdiv)
	
	CAN1->MCR &= ~(1<<0);		//����CAN�˳���ʼ��ģʽ
	while ((CAN1->MSR & 1<<0) == 1)
	{
		if (++cnt > 0XFFF0)
		{
			return 3;//�˳���ʼ��ģʽʧ��
		}
	}
	
	//��������ʼ��
	CAN1->FMR |= 1<<0;		//�������鹤���ڳ�ʼ��ģʽ
	CAN1->FA1R &= ~(1<<0);	//������0�ȹر�
	
	CAN1->FS1R |= 1<<0; 		//������λ��Ϊ32λ.
	CAN1->FM1R &= ~1;			//������0�����ڱ�ʶ������λģʽ
	
	CAN1->FFA1R &= ~1;		//������0������FIFO_0
	
	CAN1->sFilterRegister[0].FR1 = 0;//32λID
	CAN1->sFilterRegister[0].FR2 = 0;//32λMASK
	CAN1->FA1R |= 1<<0;		//���������0
	CAN1->FMR &= ~1;			//���������������ģʽ

	return 0;
}   


u8 can1_tx_msg(u32 id,u8 ide,u8 rtr,u8 len, u8 *p_dat)
{	   
	u8 mbox;
	
	if (CAN1->TSR & (1<<26))
	{
		mbox=0;			//����0Ϊ��
	}
	else if (CAN1->TSR & (1<<27))
	{
		mbox=1;			//����1Ϊ��
	}
	else if (CAN1->TSR & (1<<28))
	{
		mbox=2;			//����2Ϊ��
	}
	else 
	{
		return 0XFF;	//�޿�����,�޷����� 
	}
	
	CAN1->sTxMailBox[mbox].TIR = 0;		//���֮ǰ������
	if (ide == 0)	//��׼֡
	{
		id &= 0x7ff;//ȡ��11λstdid
		id <<= 21;		  
	}
	else		//��չ֡
	{
		id &= 0X1FFFFFFF;//ȡ��32λextid
		id <<= 3;									   
	}
	CAN1->sTxMailBox[mbox].TIR |= id;		 
	CAN1->sTxMailBox[mbox].TIR |= ide << 2;	  
	CAN1->sTxMailBox[mbox].TIR |= rtr << 1;
	
	len &= 0X0F;//�õ�����λ
	CAN1->sTxMailBox[mbox].TDTR &= ~(0XF);
	CAN1->sTxMailBox[mbox].TDTR |= len;	   //����DLC.
		
	CAN1->sTxMailBox[mbox].TDLR = 0;
	CAN1->sTxMailBox[mbox].TDHR = 0;
	//���������ݴ�������.
    switch(len)
    {
        case 8:CAN1->sTxMailBox[mbox].TDHR |= (p_dat[7] << 24);
        case 7:CAN1->sTxMailBox[mbox].TDHR |= (p_dat[6] << 16);
        case 6:CAN1->sTxMailBox[mbox].TDHR |= (p_dat[5] <<  8);
        case 5:CAN1->sTxMailBox[mbox].TDHR |= (p_dat[4] <<  0);
        case 4:CAN1->sTxMailBox[mbox].TDLR |= (p_dat[3] << 24);
        case 3:CAN1->sTxMailBox[mbox].TDLR |= (p_dat[2] << 16);
        case 2:CAN1->sTxMailBox[mbox].TDLR |= (p_dat[1] <<  8);
        case 1:CAN1->sTxMailBox[mbox].TDLR |= (p_dat[0] <<  0);break;
        default: break;
    }
    
	CAN1->sTxMailBox[mbox].TIR |= 1; //��������������  TXRQ
	
	return mbox;
}


//��÷���״̬.
//mbox:������;
//����ֵ:����״̬. 0,����;0X05,����ʧ��;0X07,���ͳɹ�.
u8 can1_tx_status(u8 mbox)
{	
	u8 sta = 0;
	
	switch (mbox)
	{
		case 0: 
			sta |= CAN1->TSR & (1<<0);			//RQCP0
			sta |= CAN1->TSR & (1<<1);			//TXOK0
			sta |= (CAN1->TSR & (1<<26)) >> 24;	//TME0
			break;
		
		case 1: 
			sta |= CAN1->TSR & (1<<8) >> 8;		//RQCP1
			sta |= CAN1->TSR & (1<<9) >> 8;	    //TXOK1
			sta |= (CAN1->TSR & (1<<27)) >> 25;	//TME1	   
			break;
		
		case 2: 
			sta |= CAN1->TSR & (1<<16) >> 16;		//RQCP2
			sta |= CAN1->TSR & (1<<17) >> 16;		//TXOK2
			sta |= (CAN1->TSR & (1<<28)) >> 26;	//TME2
			break;
		
		default:
			sta = 0X05;//����Ų���,�϶�ʧ��.
			break;
	}
	return sta;
} 


//�õ���FIFO0/FIFO1�н��յ��ı��ĸ���.
//fifox:0/1.FIFO���;
//����ֵ:FIFO0/FIFO1�еı��ĸ���.
u8 can1_fifo_status(u8 fifox)
{
	if (fifox == 0)
	{
		return CAN1->RF0R & 0x03;
	}		
	else if (fifox == 1)
	{
		return CAN1->RF1R & 0x03; 
	}
	else 
	{
		return 0;
	}
}


//��������
void can1_rx_msg(u32 *id,u8 *ide,u8 *rtr,u8 *len,u8 *p_dat,u8 fifox)
{
	*ide = CAN1->sFIFOMailBox[fifox].RIR & 0x04;//�õ���ʶ��ѡ��λ��ֵ  
 	if(*ide == 0)//��׼��ʶ��
	{
		*id = CAN1->sFIFOMailBox[fifox].RIR >> 21;
	}
	else	   //��չ��ʶ��
	{
		*id = CAN1->sFIFOMailBox[fifox].RIR >> 3;
	}
	
	*rtr = CAN1->sFIFOMailBox[fifox].RIR & 0x02;	//�õ�Զ�̷�������ֵ.
	*len = CAN1->sFIFOMailBox[fifox].RDTR & 0x0F;	//�õ�DLC

	//��ȡ����
    switch(*len)
    {
        case 8:p_dat[7] = ((CAN1->sFIFOMailBox[fifox].RDHR & 0xff000000) >> 24);
        case 7:p_dat[6] = ((CAN1->sFIFOMailBox[fifox].RDHR & 0x00ff0000) >> 16);
        case 6:p_dat[5] = ((CAN1->sFIFOMailBox[fifox].RDHR & 0x0000ff00) >>  8);
        case 5:p_dat[4] = ((CAN1->sFIFOMailBox[fifox].RDHR & 0x000000ff) >>  0);
        case 4:p_dat[3] = ((CAN1->sFIFOMailBox[fifox].RDLR & 0xff000000) >> 24);
        case 3:p_dat[2] = ((CAN1->sFIFOMailBox[fifox].RDLR & 0x00ff0000) >> 16);
        case 2:p_dat[1] = ((CAN1->sFIFOMailBox[fifox].RDLR & 0x0000ff00) >>  8);
        case 1:p_dat[0] = ((CAN1->sFIFOMailBox[fifox].RDLR & 0x000000ff) >>  0);break;
        default: break;    
    }

	fifox ? (CAN1->RF1R |= 0X20) : (CAN1->RF0R |= 0X20);//�ͷ�FIFOx����
}


//can����һ������(�̶���ʽ:IDΪ0Xaa,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 can1_send_msg( u32 id,u8* msg, u8 len)
{	
	u8 mbox;
	u16 i=0;	 
	
    mbox = can1_tx_msg(id,0,0,len,msg);
	
	while ((can1_tx_status(mbox) != 0X07))//�ȴ����ͽ���
	{
		if(++i >= 0XFFF)
		{
			return 1;//����ʧ��?
		}
	}
	return 0;	//���ͳɹ�;
}


//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 can1_receive_msg(u8 *buf,u32 *id)
{		   		   
	u8 ide,rtr = 0,len=0; 
	
	if(can1_fifo_status(0) == 0)
	{
		return 0;	//û�н��յ�����,ֱ���˳� 	 
	}
  	can1_rx_msg(id,&ide,&rtr,&len,buf, 0); 	//��ȡ����
  	
    if (rtr == 1)
    {
        len = 1;
    }
	return len;	
}





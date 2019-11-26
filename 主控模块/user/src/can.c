#include "Can.h"
	    
/*
函数功能：初始化CAN1
函数形参：brp：分频器；tbs1：时间段1；tbs2：时间段2；tsjw：同步跳转；mode：模式
*/
u8 init_can1(u8 brp, u8 tbs1, u8 tbs2, u8 tsjw, u8 mode)
{
	u16 cnt=0;

	RCC->AHB1ENR|=1<<0;  	//使能PORTA口时钟 
	
	GPIOA->MODER &= ~(3<<22);
	GPIOA->MODER |= (2<<22); //PA11 复用功能
	GPIOA->MODER &= ~(3<<24);
	GPIOA->MODER |= (2<<24); //PA12 复用功能 
	
	GPIOA->OTYPER &= ~(3 << 11);//类型  推挽
	
	GPIOA->OSPEEDR &= ~(0xf<< 22);//速度  50M
	GPIOA->OSPEEDR |= 0xa << 22;  
	
	GPIOA->AFR[1] &= ~(0XF << 12);
	GPIOA->AFR[1] |= 9 << 12;  //PA11  can1功能   AF9
	GPIOA->AFR[1] &= ~(0XF << 16);
	GPIOA->AFR[1] |= 9 << 16;  //PA12  can1功能   AF9
	
	
	RCC->APB1ENR |= 1<<25;//使能CAN1时钟 CAN1使用的是APB1的时钟(max:42M)
	
	CAN1->MCR = 0;		//退出睡眠模式(同时设置所有位为0)
	/* 	
		非时间触发通信模式
		软件自动离线管理
		睡眠模式通过软件唤醒(清除CAN1->MCR的SLEEP位)
		报文不锁定,新的覆盖旧的
		优先级由报文标识符决定
	*/
	
	CAN1->MCR |= 1<<0;	//请求CAN进入初始化模式
	while ((CAN1->MSR & 1 << 0) == 0)
	{
		if (++cnt > 100)
		{
			return 2;	//进入初始化模式失败----超时监测
		}
	}
	
	CAN1->MCR |= 1<<4;	//禁止报文自动重发
	
	CAN1->BTR = 0;	//清除原来的设置.
	CAN1->BTR |= mode<<30;	//模式设置 0,普通模式;1,回环模式;
	CAN1->BTR |= (tsjw - 1) << 24; 	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位
	CAN1->BTR |= (tbs2 - 1) << 20; 	//Tbs2=tbs2+1个时间单位
	CAN1->BTR |= (tbs1 - 1) << 16;	//Tbs1=tbs1+1个时间单位
	CAN1->BTR |= (brp - 1) ;  	//分频系数(Fdiv)为brp+1
								//波特率:Fpclk1/((Tbs1+Tbs2+1)*Fdiv)
	
	CAN1->MCR &= ~(1<<0);		//请求CAN退出初始化模式
	while ((CAN1->MSR & 1<<0) == 1)
	{
		if (++cnt > 0XFFF0)
		{
			return 3;//退出初始化模式失败
		}
	}
	
	//过滤器初始化
	CAN1->FMR |= 1<<0;		//过滤器组工作在初始化模式
	CAN1->FA1R &= ~(1<<0);	//过滤器0先关闭
	
	CAN1->FS1R |= 1<<0; 		//过滤器位宽为32位.
	CAN1->FM1R &= ~1;			//过滤器0工作在标识符屏蔽位模式
	
	CAN1->FFA1R &= ~1;		//过滤器0关联到FIFO_0
	
	CAN1->sFilterRegister[0].FR1 = 0;//32位ID
	CAN1->sFilterRegister[0].FR2 = 0;//32位MASK
	CAN1->FA1R |= 1<<0;		//激活过滤器0
	CAN1->FMR &= ~1;			//过滤器组进入正常模式

	return 0;
}   


u8 can1_tx_msg(u32 id,u8 ide,u8 rtr,u8 len, u8 *p_dat)
{	   
	u8 mbox;
	
	if (CAN1->TSR & (1<<26))
	{
		mbox=0;			//邮箱0为空
	}
	else if (CAN1->TSR & (1<<27))
	{
		mbox=1;			//邮箱1为空
	}
	else if (CAN1->TSR & (1<<28))
	{
		mbox=2;			//邮箱2为空
	}
	else 
	{
		return 0XFF;	//无空邮箱,无法发送 
	}
	
	CAN1->sTxMailBox[mbox].TIR = 0;		//清除之前的设置
	if (ide == 0)	//标准帧
	{
		id &= 0x7ff;//取低11位stdid
		id <<= 21;		  
	}
	else		//扩展帧
	{
		id &= 0X1FFFFFFF;//取低32位extid
		id <<= 3;									   
	}
	CAN1->sTxMailBox[mbox].TIR |= id;		 
	CAN1->sTxMailBox[mbox].TIR |= ide << 2;	  
	CAN1->sTxMailBox[mbox].TIR |= rtr << 1;
	
	len &= 0X0F;//得到低四位
	CAN1->sTxMailBox[mbox].TDTR &= ~(0XF);
	CAN1->sTxMailBox[mbox].TDTR |= len;	   //设置DLC.
		
	CAN1->sTxMailBox[mbox].TDLR = 0;
	CAN1->sTxMailBox[mbox].TDHR = 0;
	//待发送数据存入邮箱.
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
    
	CAN1->sTxMailBox[mbox].TIR |= 1; //请求发送邮箱数据  TXRQ
	
	return mbox;
}


//获得发送状态.
//mbox:邮箱编号;
//返回值:发送状态. 0,挂起;0X05,发送失败;0X07,发送成功.
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
			sta = 0X05;//邮箱号不对,肯定失败.
			break;
	}
	return sta;
} 


//得到在FIFO0/FIFO1中接收到的报文个数.
//fifox:0/1.FIFO编号;
//返回值:FIFO0/FIFO1中的报文个数.
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


//接收数据
void can1_rx_msg(u32 *id,u8 *ide,u8 *rtr,u8 *len,u8 *p_dat,u8 fifox)
{
	*ide = CAN1->sFIFOMailBox[fifox].RIR & 0x04;//得到标识符选择位的值  
 	if(*ide == 0)//标准标识符
	{
		*id = CAN1->sFIFOMailBox[fifox].RIR >> 21;
	}
	else	   //扩展标识符
	{
		*id = CAN1->sFIFOMailBox[fifox].RIR >> 3;
	}
	
	*rtr = CAN1->sFIFOMailBox[fifox].RIR & 0x02;	//得到远程发送请求值.
	*len = CAN1->sFIFOMailBox[fifox].RDTR & 0x0F;	//得到DLC

	//读取数据
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

	fifox ? (CAN1->RF1R |= 0X20) : (CAN1->RF0R |= 0X20);//释放FIFOx邮箱
}


//can发送一组数据(固定格式:ID为0Xaa,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 can1_send_msg( u32 id,u8* msg, u8 len)
{	
	u8 mbox;
	u16 i=0;	 
	
    mbox = can1_tx_msg(id,0,0,len,msg);
	
	while ((can1_tx_status(mbox) != 0X07))//等待发送结束
	{
		if(++i >= 0XFFF)
		{
			return 1;//发送失败?
		}
	}
	return 0;	//发送成功;
}


//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 can1_receive_msg(u8 *buf,u32 *id)
{		   		   
	u8 ide,rtr = 0,len=0; 
	
	if(can1_fifo_status(0) == 0)
	{
		return 0;	//没有接收到数据,直接退出 	 
	}
  	can1_rx_msg(id,&ide,&rtr,&len,buf, 0); 	//读取数据
  	
    if (rtr == 1)
    {
        len = 1;
    }
	return len;	
}





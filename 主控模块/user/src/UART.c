#include "uart.h"
#include "stdio.h"

static u32 rev_cnt = 0;
static u8 group = 0;
static u32 addr_cnt = 0;

typedef struct
{
    u16 uart_rev_len; //接收长度
    u8 data_buf[256]; //数据缓存区
}TYP_UART_FONT;
TYP_UART_FONT font_buffer[2]; //缓存区
/********************************************************************
* Function： void USART1_IRQHandler()
* Description: 串口接收中断服务函数
* Calls:
* Called By:
* Input：
* Output：
* Return :
* Author: 信盈达
* Others: PC 端发送完一个文件后， 还需要发送 0xfafaffff 作为传输结束标志
* date of completion: 2015-8-21
* date of last modify: 2015-8-21
*********************************************************************/
void USART1_IRQHandler()
{
   u8 ch;
/* 串口接收中断 */
    if (USART1->SR & (1 << 5))
    {
        USART1->SR &= ~(1 << 5);
        ch = USART1->DR;

        font_buffer[group].data_buf[rev_cnt] = ch;           
        /* 接收计数变量+1，为接收下一个字节做准备 */
        rev_cnt++;
        
        if (rev_cnt == 256)
        {
            font_buffer[group].uart_rev_len = rev_cnt;
            group = !group;
            rev_cnt = 0;
            addr_cnt++;
        }

    }
    else if(USART1->SR & 1<<4)  //IDLE中断
	{
		ch = USART1->DR;//读DR  清除IDLE标识
        /* 保存接收到的数据长度 */
        font_buffer[group].uart_rev_len = rev_cnt;
        /* 接收计数变量清零，为新一轮接收做准备 */
        rev_cnt = 0;   
	}
//	u8 data;
//	if(USART1->SR & (0X01<<5))  //接收中断
//	{
//		data=USART1->DR;
//		while(!(USART1->SR&(0x01<<7)));
//		USART1->DR=data;
//	}
}


/********************************************************************
*  Function   : uart1_int_init()
*  Description: 串口1中断初始化程序;
*  Calls      : none;
*  Called By  : none;
*  Input      : none;
*  Output     : ;
*  Return     : 无;
*  Author     : ChenZhiFa
*  Others     :
*  date of completion:   2012-09-07
*********************************************************************/
void uart1_int_init(void)
{
    uint32_t Priority;
    /*USART1  RXNEIE：接收缓冲区非空中断使能 */
    USART1->CR1 |= 1 << 5 ;

    /*USART1   IDLE中断使能 (IDLE interrupt enable) */
    USART1->CR1 |= 1 << 4 ;


    /* USART1 NVIC 配置 */
    NVIC_SetPriorityGrouping(7 - 2);                  //设置分组，分组2
    Priority = NVIC_EncodePriority (7 - 2, 0, 0);     //编码优先级数值,抢占是0，响应0，根据需要设置
    NVIC_SetPriority(USART1_IRQn, Priority);          //设置优先级到寄存器中。
    NVIC_EnableIRQ(USART1_IRQn);                      //使能

}


/********************************************************************
*  Function   : uart1_init()
*  Description: 查询方式从uart1读取一个数据;
*  Calls      : none;
*  Called By  : none;
*  Input      : pclk2:APB2时钟;bound：目标波特率;
*  Output     : ;
*  Return     : 无;
*  Author     : ChenZhiFa
*  Others     :
*  date of completion:   2014-09-07
*********************************************************************/
void uart1_init(uint32_t pclk2, uint32_t bound)
{
    float temp;
    uint16_t mantissa;
    uint16_t fraction;

    /* 任何模块配置其寄存器前都要先开启对应时钟*/
    RCC->AHB1ENR |= 1;                   //PA时钟使能
    RCC->APB2ENR |= 1 << 4;              //使能 usart1 时钟

    /* 开发板使用的是PA9.PA10，
    * 首先配置为复用功能，其次具体为哪一种复用功能
     */
    GPIOA->MODER &= ~(3 << 18);
    GPIOA->MODER |= (2 << 18);          //PA9 复用功能  TX

    //配置具体为哪一种复用功能
    GPIOA->AFR[1] &= ~(0XF << 4);       //参见 图18  寄存器描述
    GPIOA->AFR[1] |= 7 << 4;            //PA9  uart1功能    AF7

    GPIOA->MODER  &= ~(3 << 20);
    GPIOA->MODER  |=  (2 << 20);        //PA10 复用功能 RX

    GPIOA->AFR[1] &= ~(0XF << 8);
    GPIOA->AFR[1] |=  7 << 8;           //PA10  uart1功能   AF7


    /* 波特率设置   */
    /* 得到整数部分 */
    temp           = (float)((pclk2) / (bound * 16.0));
    mantissa       = temp;             //小数没有了

    /* 得到小数部分 */
    fraction       = (int)( ( temp - mantissa ) * 16 + 0.5f);

    /* 合成16位数 */
    mantissa     <<= 4;
    mantissa      |= fraction;

    /* 写入寄存器，波特率设置 */
    USART1->BRR  = mantissa;

    /* 常规参数配置，使能UART,8位字数据，1位停止,无校验位. */
    USART1->CR1 = 1 << 13 |  /* UE：USART使能*/
                  0 << 12 |  /* M：字长,0--8位，1--9位*/
                  0 << 10 |  /* PCE：检验控制使能,0--禁止校验，1--使能校验 */
                  0 << 8  |  /* PEIE：PE中断使能,0--禁止中断，1--使能中断 */
                  1 << 3  |  /* TE：发送使能,0--禁止发送，1--使能发送 */
                  1 << 2  ;  /* RE：接收使能,0--禁止接收，1--使能接收 */

    /* 常规参数配置，使能UART,8位字数据，1位停止,无校验位. */
    USART1->CR2 = 0 << 12 ;  /*STOP：停止位(STOP bits)  00：1个停止位；*/

    USART1->CR3 = 0;
			
    uart1_int_init();
}


//查接收一个字节函数
u8 rev_byte()
{
	while ( (USART1->SR & 1<<5) == 0 )
	{
		;//等待 接收缓冲区非空。
	}
	
	return USART1->DR; //接收数据。
}


//发送一个字节函数
void send_byte(u8 val)
{
	while ( (USART1->SR & 1<<6) == 0 )
	{
		;//等待发送完成
	}
	
	USART1->DR = val; //发送数据
}




/* printf  scanf 最底层会调用这两个函数 */
int sendchar (int c) 
{
	send_byte(c);	//串口发送一个字节
	
	return 0;
}

int getkey   (void)
{
	return rev_byte();//串口接收一个字节
}
#pragma import(__use_no_semihosting_swi)


extern int sendchar (int c);
extern int getkey   (void);


struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;


int fputc(int c, FILE *f) {//printf最底层会调用这个函数
  return (sendchar(c));
}


int fgetc(FILE *f) {//scanf();最底层会调用这个函数
  return (getkey());
}


int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int c) {
  sendchar(c);
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}













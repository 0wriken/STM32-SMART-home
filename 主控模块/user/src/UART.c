#include "uart.h"
#include "stdio.h"

static u32 rev_cnt = 0;
static u8 group = 0;
static u32 addr_cnt = 0;

typedef struct
{
    u16 uart_rev_len; //���ճ���
    u8 data_buf[256]; //���ݻ�����
}TYP_UART_FONT;
TYP_UART_FONT font_buffer[2]; //������
/********************************************************************
* Function�� void USART1_IRQHandler()
* Description: ���ڽ����жϷ�����
* Calls:
* Called By:
* Input��
* Output��
* Return :
* Author: ��ӯ��
* Others: PC �˷�����һ���ļ��� ����Ҫ���� 0xfafaffff ��Ϊ���������־
* date of completion: 2015-8-21
* date of last modify: 2015-8-21
*********************************************************************/
void USART1_IRQHandler()
{
   u8 ch;
/* ���ڽ����ж� */
    if (USART1->SR & (1 << 5))
    {
        USART1->SR &= ~(1 << 5);
        ch = USART1->DR;

        font_buffer[group].data_buf[rev_cnt] = ch;           
        /* ���ռ�������+1��Ϊ������һ���ֽ���׼�� */
        rev_cnt++;
        
        if (rev_cnt == 256)
        {
            font_buffer[group].uart_rev_len = rev_cnt;
            group = !group;
            rev_cnt = 0;
            addr_cnt++;
        }

    }
    else if(USART1->SR & 1<<4)  //IDLE�ж�
	{
		ch = USART1->DR;//��DR  ���IDLE��ʶ
        /* ������յ������ݳ��� */
        font_buffer[group].uart_rev_len = rev_cnt;
        /* ���ռ����������㣬Ϊ��һ�ֽ�����׼�� */
        rev_cnt = 0;   
	}
//	u8 data;
//	if(USART1->SR & (0X01<<5))  //�����ж�
//	{
//		data=USART1->DR;
//		while(!(USART1->SR&(0x01<<7)));
//		USART1->DR=data;
//	}
}


/********************************************************************
*  Function   : uart1_int_init()
*  Description: ����1�жϳ�ʼ������;
*  Calls      : none;
*  Called By  : none;
*  Input      : none;
*  Output     : ;
*  Return     : ��;
*  Author     : ChenZhiFa
*  Others     :
*  date of completion:   2012-09-07
*********************************************************************/
void uart1_int_init(void)
{
    uint32_t Priority;
    /*USART1  RXNEIE�����ջ������ǿ��ж�ʹ�� */
    USART1->CR1 |= 1 << 5 ;

    /*USART1   IDLE�ж�ʹ�� (IDLE interrupt enable) */
    USART1->CR1 |= 1 << 4 ;


    /* USART1 NVIC ���� */
    NVIC_SetPriorityGrouping(7 - 2);                  //���÷��飬����2
    Priority = NVIC_EncodePriority (7 - 2, 0, 0);     //�������ȼ���ֵ,��ռ��0����Ӧ0��������Ҫ����
    NVIC_SetPriority(USART1_IRQn, Priority);          //�������ȼ����Ĵ����С�
    NVIC_EnableIRQ(USART1_IRQn);                      //ʹ��

}


/********************************************************************
*  Function   : uart1_init()
*  Description: ��ѯ��ʽ��uart1��ȡһ������;
*  Calls      : none;
*  Called By  : none;
*  Input      : pclk2:APB2ʱ��;bound��Ŀ�겨����;
*  Output     : ;
*  Return     : ��;
*  Author     : ChenZhiFa
*  Others     :
*  date of completion:   2014-09-07
*********************************************************************/
void uart1_init(uint32_t pclk2, uint32_t bound)
{
    float temp;
    uint16_t mantissa;
    uint16_t fraction;

    /* �κ�ģ��������Ĵ���ǰ��Ҫ�ȿ�����Ӧʱ��*/
    RCC->AHB1ENR |= 1;                   //PAʱ��ʹ��
    RCC->APB2ENR |= 1 << 4;              //ʹ�� usart1 ʱ��

    /* ������ʹ�õ���PA9.PA10��
    * ��������Ϊ���ù��ܣ���ξ���Ϊ��һ�ָ��ù���
     */
    GPIOA->MODER &= ~(3 << 18);
    GPIOA->MODER |= (2 << 18);          //PA9 ���ù���  TX

    //���þ���Ϊ��һ�ָ��ù���
    GPIOA->AFR[1] &= ~(0XF << 4);       //�μ� ͼ18  �Ĵ�������
    GPIOA->AFR[1] |= 7 << 4;            //PA9  uart1����    AF7

    GPIOA->MODER  &= ~(3 << 20);
    GPIOA->MODER  |=  (2 << 20);        //PA10 ���ù��� RX

    GPIOA->AFR[1] &= ~(0XF << 8);
    GPIOA->AFR[1] |=  7 << 8;           //PA10  uart1����   AF7


    /* ����������   */
    /* �õ��������� */
    temp           = (float)((pclk2) / (bound * 16.0));
    mantissa       = temp;             //С��û����

    /* �õ�С������ */
    fraction       = (int)( ( temp - mantissa ) * 16 + 0.5f);

    /* �ϳ�16λ�� */
    mantissa     <<= 4;
    mantissa      |= fraction;

    /* д��Ĵ��������������� */
    USART1->BRR  = mantissa;

    /* ����������ã�ʹ��UART,8λ�����ݣ�1λֹͣ,��У��λ. */
    USART1->CR1 = 1 << 13 |  /* UE��USARTʹ��*/
                  0 << 12 |  /* M���ֳ�,0--8λ��1--9λ*/
                  0 << 10 |  /* PCE���������ʹ��,0--��ֹУ�飬1--ʹ��У�� */
                  0 << 8  |  /* PEIE��PE�ж�ʹ��,0--��ֹ�жϣ�1--ʹ���ж� */
                  1 << 3  |  /* TE������ʹ��,0--��ֹ���ͣ�1--ʹ�ܷ��� */
                  1 << 2  ;  /* RE������ʹ��,0--��ֹ���գ�1--ʹ�ܽ��� */

    /* ����������ã�ʹ��UART,8λ�����ݣ�1λֹͣ,��У��λ. */
    USART1->CR2 = 0 << 12 ;  /*STOP��ֹͣλ(STOP bits)  00��1��ֹͣλ��*/

    USART1->CR3 = 0;
			
    uart1_int_init();
}


//�����һ���ֽں���
u8 rev_byte()
{
	while ( (USART1->SR & 1<<5) == 0 )
	{
		;//�ȴ� ���ջ������ǿա�
	}
	
	return USART1->DR; //�������ݡ�
}


//����һ���ֽں���
void send_byte(u8 val)
{
	while ( (USART1->SR & 1<<6) == 0 )
	{
		;//�ȴ��������
	}
	
	USART1->DR = val; //��������
}




/* printf  scanf ��ײ��������������� */
int sendchar (int c) 
{
	send_byte(c);	//���ڷ���һ���ֽ�
	
	return 0;
}

int getkey   (void)
{
	return rev_byte();//���ڽ���һ���ֽ�
}
#pragma import(__use_no_semihosting_swi)


extern int sendchar (int c);
extern int getkey   (void);


struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;


int fputc(int c, FILE *f) {//printf��ײ������������
  return (sendchar(c));
}


int fgetc(FILE *f) {//scanf();��ײ������������
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













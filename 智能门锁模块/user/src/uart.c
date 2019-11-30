#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "uart.h"


//TX	PA9
//RX	PA10

/*���ò��裺																
	(1)��GPIO��USART��ʱ��												
	(2)����USART�����ܽ�GPIOģʽ											  
	(3)����USART���ݸ�ʽ�������ʵȲ���										 
	(4)���ʹ��USART����													  
*/

void Usart1Init(u32 apb2clk,u32 bound)
{
	//1.ʹ��AFIO,GPIO��USART������ʱ��
	//RCC->APB2ENR |=0x01<<0;
	RCC->APB2ENR |=0x01<<2;
	RCC->APB2ENR |=0x01<<14;
	
	//2.��USART Tx��GPIO����Ϊ���츴��ģʽ,����ٶ�50MHz
	GPIOA->CRH &=~(0xF<<4);
	GPIOA->CRH |=(0xB<<4);
	
	//3.��USART Rx��GPIO����Ϊ��������ģʽ
	GPIOA->CRH &=~(0xF<<8);
	GPIOA->CRH |=(0x01<<10);
	
	//4.����USART����
	USART1->BRR =apb2clk*1000000/bound;//16*div=fck/bound
	USART1->CR1 &=~(0x01<<12);         	//���ݳ���Ϊ8λ
	USART1->CR2 &=~(0x03<<12);    			//����ֹͣλΪ1λ
	
	//USART1->CR1 |=0X01<<4;  //�����ж�ʹ��
	USART1->CR1 |=0X01<<5; //���ջ������ǿ��ж�ʹ��
	
	//nvic������uart1�����ȼ���ʹ��uart1�ж�
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,3,3));
	NVIC_EnableIRQ(USART1_IRQn);
	
	//5.	ʹ�ܽ��ܡ�ʹ�ܷ��͡���ģ�顣
	USART1->CR1 |=0X01<<3;    //�����ڵķ��͹���
	USART1->CR1 |=0X01<<2;    //�����ڵĽ��ܹ���
	USART1->CR1 |=0X01<<13;   //������1
	

}



void USART1_IRQHandler(void)
{
	u8 data;
	if(USART1->SR & (0X01<<5))  //�����ж�
	{
		data=USART1->DR;
		while(!(USART1->SR&(0x01<<7)));
		USART1->DR=data;
	}

}


//PA2--TX
//PA3--RX
void Usart2Init(u32 apb2clk,u32 bound)
{
	//1.ʹ��AFIO,GPIO��USART������ʱ��
	//RCC->APB2ENR |=0x01<<0;
	RCC->APB2ENR |=0x01<<2;
	RCC->APB1ENR |=0x01<<17;
	
	//2.��USART Tx��GPIO����Ϊ���츴��ģʽ,����ٶ�50MHz
	GPIOA->CRL &=~(0xF<<8);
	GPIOA->CRL |=(0xB<<8);
	
	//3.��USART Rx��GPIO����Ϊ��������ģʽ
	GPIOA->CRL &=~(0xF<<12);
	GPIOA->CRL |=(0x01<<14);
	
	//4.����USART����
	USART2->BRR =apb2clk*1000000/bound;//16*div=fck/bound
	USART2->CR1 &=~(0x01<<12);         	//���ݳ���Ϊ8λ
	USART2->CR2 &=~(0x03<<12);    			//����ֹͣλΪ1λ
	
	//���ջ������ǿ��ж�ʹ��
	USART2->CR1 |=0X01<<5; 
	USART2->CR1 |=0X01<<4; 
	
	//nvic������uart1�����ȼ���ʹ��uart1�ж�
	NVIC_SetPriority(USART2_IRQn,NVIC_EncodePriority(7-2,3,4));
	NVIC_EnableIRQ(USART2_IRQn);
	
	
	//5.	ʹ�ܽ��ܡ�ʹ�ܷ��͡���ģ�顣
	USART2->CR1 |=0X01<<3;    //�����ڵķ��͹���
	USART2->CR1 |=0X01<<2;    //�����ڵĽ��ܹ���
	USART2->CR1 |=0X01<<13;   //������2
	

}




void USART2_IRQHandler(void)
{
	u8 data;
	if(USART2->SR & (0X01<<5))  //�����ж�
	{
		MG200rev.RevBuf[MG200rev.RevLen++]=USART2->DR;
	}
	else if(USART2->SR & (0X01<<4))//�����ж�
	{
		data=USART2->SR;
		data=USART2->DR;
		//printf("%s",MG200rev.RevBuf);
		MG200rev.RevLen=0;
		MG200rev.RevOver=1;
	}
	
}

//����1�����ַ���
void uart1_send_string(u8 *p)
{
	while(*p!='\0')
	{
		while((USART1->SR & (0X01<<7))==0);  //�ȴ����ͻ�����Ϊ��
		USART1->DR=*p++;
	}
}

//����2�����ַ���
void uart2_send_string(u8 *p)
{
	while(*p!='\0')
	{
		while((USART2->SR & (0X01<<7))==0);  //�ȴ����ͻ�����Ϊ��
		USART2->DR=*p++;
	}
}





#pragma import(__use_no_semihosting_swi) //ȡ��������״̬

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) 
{
	while((USART1->SR &(0X01<<7))==0);  //�ȴ�֮ǰ�����ݷ������
		USART1->DR=ch;
  return (ch);
}

int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
}


void _sys_exit(int return_code) {
label :  goto label;  /* endless loop */
}
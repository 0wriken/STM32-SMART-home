#include "uart.h"
#include "stdio.h"

u8 uart_rx_buff[128] = {0};

void uart1_Init(void)
{
    GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_IT);
    UART1_Init(115200,UART1_WORDLENGTH_8D,
        UART1_STOPBITS_1,UART1_PARITY_NO,
        UART1_SYNCMODE_LASTBIT_DISABLE,UART1_MODE_TXRX_ENABLE);

    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
	UART1_ITConfig(UART1_IT_IDLE, ENABLE);
	enableInterrupts();
    UART1_Cmd(ENABLE);
}

void send_string(u8 *str)
{
    while(*str)
    {
        UART1_SendData8(*str);
        while(!UART1_GetFlagStatus(UART1_FLAG_TXE));
        str++;
    }
}

/*printf函数支持*/
int putchar(int c)
{

//RESET or SET

    while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
    {
        ;
    }
    UART1_SendData8((char)c);
    return (c);
}

#pragma vector = 18
 __interrupt void uart_recv_IRQ(void)
{
    char u8_data;
	static u8 i = 0;
	if(UART1_GetITStatus(UART1_IT_RXNE) == SET)
	{
		UART1_ClearITPendingBit(UART1_IT_RXNE);
		u8_data = UART1_ReceiveData8();
		uart_rx_buff[i++] = u8_data;
		if(i == 128)
		{
			i = 0;
		}
	}
	else if(UART1_GetITStatus(UART1_IT_IDLE) == SET)
	{
		uart_rx_buff[i++] = '\0';
        printf("%s\r\n",uart_rx_buff);
		i = 0;
	}
}
#include "stm8s.h"
#include "rgb.h"
#include "key.h"
#include "uart.h"
#include "tim2.h"
#include "mcp2515.h"
#include <string.h>
#include <stdio.h> 
unsigned char buf[100]={0};
void delay_ms(int time)
{    
   u32 i=0;  
   while(time--)
   {
      i=12000;  //自己定义
      while(i--) ;    
   }

 }
int main()
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_SLOW);
    uart1_Init();
    MCP2515_Init();
    init_tim2();
    rgb_gpio_init();
    //Timer4Init();
    printf("ooo\n");
    while(1)
    {
      if(CAN_Receive_Buffer((unsigned char *)buf))
      {
        printf("rec:%s\n",(char *)buf);
        
        if(strcmp((const char*)buf,"RED")==0)
        {
          CAN_Send_Buffer((unsigned char *)"OK",3);
          rgb_blue_on();
        }
        if(strcmp((const char*)buf,"LED1_OF")==0)
        {
          CAN_Send_Buffer((unsigned char *)"OK",3);
          rgb_blue_off();
        }
        }
     //printf("rec_ok\n");
      delay_ms(2000);
    }
    
  
}

/*
下述代码必须加上，否则报错
*/
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

#include "stm8s.h"
#include "light.h"
#include "key.h"
#include "uart.h"
#include "tim2.h"
#include "mcp2515.h"
#include <string.h>
#include "stdio.h"

void delay11(u32 t)
{
    while(t--);
}

int main()
{
    u8 tx_buff[8];
    u32 flag = 0; 
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_SLOW);
    uart1_Init();
    MCP2515_Init();
    init_light();
    init_tim2();
    
    
    while(1)
    {
     
    CAN_Receive_Buffer((u8 *)tx_buff);
    delay11(50000);
    printf("%s",tx_buff);
    if(strcmp((const char *)tx_buff ,"l_on") == 0)
    {
      flag = 1; 
    }
    if(strcmp((const char *)tx_buff ,"l_off") == 0)
    {
      flag = 2;
    }
    if(flag == 1)
    {
	ADC1_Cmd(ENABLE);
        TIM2_Cmd(ENABLE);
    }
    else if(flag == 2)
        {
          ADC1_Cmd(DISABLE);
          TIM2_Cmd(DISABLE);
        }
    
    memset(tx_buff ,0, sizeof(tx_buff) );
    delay11(100000);     
        
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

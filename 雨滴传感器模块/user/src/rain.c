#include "rain.h"
#include "stdio.h"
#include "mcp2515.h"
#include "uart.h"

/*
函数功能：初始化ADC1
函数形参：无
函数返回值：无
备注：测量雨滴传感器值
*/
void init_RAIN(void)
{
    GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT);
	ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_3,\
        ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT,\
		ADC1_SCHMITTTRIG_CHANNEL3, DISABLE);
	ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);
	enableInterrupts();

    
}


#pragma vector = 24
 __interrupt void ADC1_IRQHandler(void)
{
    u8 rain = 0;
    u8 rx_buff_1[8] = {"rain   "};
    u8 rx_buff_2[8] = {"no_rain"};
      
    rain = ADC1_GetConversionValue();
   
    if(rain < 200)
    {GPIO_WriteLow(GPIOC, GPIO_PIN_3);
        
    printf("%d\r\n",rain);
        CAN_Send_Buffer(rx_buff_1,8);
       
    }
    else
    {GPIO_WriteLow(GPIOC, GPIO_PIN_3);

     CAN_Send_Buffer(rx_buff_2,8);
      printf("%d\r\n",rain);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
    }
    
    ADC1_ClearITPendingBit(ADC1_IT_EOC);
}

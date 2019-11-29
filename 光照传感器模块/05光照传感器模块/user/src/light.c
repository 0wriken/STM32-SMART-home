#include "light.h"
#include "stdio.h"
#include "mcp2515.h"
#include "uart.h"

/*
函数功能：初始化ADC1
函数形参：无
函数返回值：无
备注：测量振动传感器值
*/
void init_light(void)
{
  
    GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT);
	ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_3,\
        ADC1_PRESSEL_FCPU_D2, ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT,\
		ADC1_SCHMITTTRIG_CHANNEL3, DISABLE);
	ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);
	enableInterrupts();

    
}

/**/
#pragma vector = 24
 __interrupt void adc_recv_IRQ(void)
{
    u8 light = 0;
    u8 rx_buff_1[8] = {"light"};
    u8 rx_buff_2[8] = {"dark "};
      
    light = ADC1_GetConversionValue();
    if(light < 100)
    {GPIO_WriteLow(GPIOC, GPIO_PIN_3);
           printf("%d\r\n",light);
        CAN_Send_Buffer(rx_buff_1,8);
       
    }
    else
    {GPIO_WriteLow(GPIOC, GPIO_PIN_3);
      printf("%d\r\n",light);
     CAN_Send_Buffer(rx_buff_2,8);

    }
    
    ADC1_ClearITPendingBit(ADC1_IT_EOC);
}

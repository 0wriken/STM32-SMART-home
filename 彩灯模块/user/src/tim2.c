#include "tim2.h"
#include "uart.h"

void init_tim2(void)
{
    TIM2_TimeBaseInit(TIM2_PRESCALER_1024, 15625);
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    enableInterrupts();
    TIM2_Cmd(ENABLE);
    
}


#pragma vector=15
__interrupt void TIM2_RQHandler()
{
    TIM2_ClearITPendingBit(TIM2_IT_UPDATE);   
    ADC1_StartConversion();//启动转换
}

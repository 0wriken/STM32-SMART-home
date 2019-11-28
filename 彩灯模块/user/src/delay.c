#include "delay.h"
/*******************************************************************************
**功能描述：定时器4参数初始化
**入口参数：无
**输出：无
*******************************************************************************/
void Timer4Init()
{
  TIM4_TimeBaseInit(TIM4_PRESCALER_16, 1000);
  TIM4_ITConfig(TIM4_IT_UPDATE, DISABLE);        // 禁止中断
TIM4_UpdateDisableConfig(ENABLE);      // 允许产生更新事件
TIM4_SetCounter(0x00);              // 设定计数器的初值
}
/*******************************************************************************
**函数名称：void TIM4Delayms(unsigned int ms)
**功能描述：定时器4参进行精确延时，最小为1毫秒，最大65535
**入口参数：unsigned int ms     1=< ms <= 65535
**输出：无
*******************************************************************************/
void Delay_Nms(unsigned int ms)
{
TIM4_SetCounter(0x00);                // 设定计数器的初值
TIM4_Cmd(ENABLE);           //启动定时器4开始计数
printf("coming\r\n");
while(ms--)
  {
   // while(TIM4_GetFlagStatus(TIM4_FLAG_UPDATE)==RESET);//等待计数是否达到1毫秒
  TIM4_Cmd(ENABLE);
  while(TIM4_GetFlagStatus(TIM4_FLAG_UPDATE)==RESET); //等待计数是否达到1毫秒
  }
TIM4_ClearFlag(TIM4_FLAG_UPDATE);         //计数完成1毫秒，清除相应的标志
TIM4_Cmd(DISABLE); //延时全部结束，关闭定时器4
}

#include "delay.h"
/*******************************************************************************
**������������ʱ��4������ʼ��
**��ڲ�������
**�������
*******************************************************************************/
void Timer4Init()
{
  TIM4_TimeBaseInit(TIM4_PRESCALER_16, 1000);
  TIM4_ITConfig(TIM4_IT_UPDATE, DISABLE);        // ��ֹ�ж�
TIM4_UpdateDisableConfig(ENABLE);      // ������������¼�
TIM4_SetCounter(0x00);              // �趨�������ĳ�ֵ
}
/*******************************************************************************
**�������ƣ�void TIM4Delayms(unsigned int ms)
**������������ʱ��4�ν��о�ȷ��ʱ����СΪ1���룬���65535
**��ڲ�����unsigned int ms     1=< ms <= 65535
**�������
*******************************************************************************/
void Delay_Nms(unsigned int ms)
{
TIM4_SetCounter(0x00);                // �趨�������ĳ�ֵ
TIM4_Cmd(ENABLE);           //������ʱ��4��ʼ����
printf("coming\r\n");
while(ms--)
  {
   // while(TIM4_GetFlagStatus(TIM4_FLAG_UPDATE)==RESET);//�ȴ������Ƿ�ﵽ1����
  TIM4_Cmd(ENABLE);
  while(TIM4_GetFlagStatus(TIM4_FLAG_UPDATE)==RESET); //�ȴ������Ƿ�ﵽ1����
  }
TIM4_ClearFlag(TIM4_FLAG_UPDATE);         //�������1���룬�����Ӧ�ı�־
TIM4_Cmd(DISABLE); //��ʱȫ���������رն�ʱ��4
}

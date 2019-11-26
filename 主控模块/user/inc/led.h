#ifndef __LED_H__
#define __LED_H__

#include "stm32f4xx.h"

#define LED4_ON()    GPIOC->BSRRH |= 0X1 << 0
#define LED4_OFF()   GPIOC->BSRRL |= 0X1 << 0
#define LED4_TOR()   GPIOC->ODR ^= 0X1 << 0


#define LED3_ON()    GPIOF->BSRRH |= 0X1 << 6
#define LED3_OFF()   GPIOF->BSRRL |= 0X1 << 6
#define LED3_TOR()   GPIOF->ODR ^= 0X1 << 6


/********************************���?�����䨬??����??a��?***************************************/
#define KEY_ONE_RES    1		//���?��1���??����??�̡�?��o��
#define KEY_TWO_RES    2		//���?��2���??����??�̡�?��o��
#define KEY_THREE_RES    3		//���?��3���??����??�̡�?��o��
#define KEY_FOUR_RES    4		//���?��4���??����??�̡�?��o��
#define KEY_ERROR_RES    0		//?T���?�����??����??�̡�?��o��
/********************************���?�����䨬??����??����?***************************************/

/********************************���?����?��e?����??a��?***************************************/
#define KEY_ONE_SCAN  (!!(GPIOA->IDR & (0x1 << 0)))		//��?����?D??���?��1��?��?���??��?
#define KEY_TWO_SCAN  (!!(GPIOE->IDR & (0x1 << 2)))	
#define KEY_THREE_SCAN  (!!(GPIOE->IDR & (0x1 << 3)))
#define KEY_FOUR_SCAN  (!!(GPIOE->IDR & (0x1 << 4)))
/********************************���?����?��e?����??����?***************************************/

void led_init(void);
void key_init(void);
u8 key_status(void);
#endif

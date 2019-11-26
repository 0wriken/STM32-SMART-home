#ifndef __LED_H__
#define __LED_H__

#include "stm32f4xx.h"

#define LED4_ON()    GPIOC->BSRRH |= 0X1 << 0
#define LED4_OFF()   GPIOC->BSRRL |= 0X1 << 0
#define LED4_TOR()   GPIOC->ODR ^= 0X1 << 0


#define LED3_ON()    GPIOF->BSRRH |= 0X1 << 6
#define LED3_OFF()   GPIOF->BSRRL |= 0X1 << 6
#define LED3_TOR()   GPIOF->ODR ^= 0X1 << 6


/********************************¡ã¡ä?¨¹¡Á¡ä¨¬??¡§¨°??a¨º?***************************************/
#define KEY_ONE_RES    1		//¡ã¡ä?¨¹1¡ã¡ä??¡¤¦Ì??¦Ì¡À?¡ão¨º
#define KEY_TWO_RES    2		//¡ã¡ä?¨¹2¡ã¡ä??¡¤¦Ì??¦Ì¡À?¡ão¨º
#define KEY_THREE_RES    3		//¡ã¡ä?¨¹3¡ã¡ä??¡¤¦Ì??¦Ì¡À?¡ão¨º
#define KEY_FOUR_RES    4		//¡ã¡ä?¨¹4¡ã¡ä??¡¤¦Ì??¦Ì¡À?¡ão¨º
#define KEY_ERROR_RES    0		//?T¡ã¡ä?¨¹¡ã¡ä??¡¤¦Ì??¦Ì¡À?¡ão¨º
/********************************¡ã¡ä?¨¹¡Á¡ä¨¬??¡§¨°??¨¢¨º?***************************************/

/********************************¡ã¡ä?¨¹¨º?¡Àe?¡§¨°??a¨º?***************************************/
#define KEY_ONE_SCAN  (!!(GPIOA->IDR & (0x1 << 0)))		//¨®?¨¤¡ä?D??¡ã¡ä?¨¹1¨º?¡¤?¡ã¡ä??¨®?
#define KEY_TWO_SCAN  (!!(GPIOE->IDR & (0x1 << 2)))	
#define KEY_THREE_SCAN  (!!(GPIOE->IDR & (0x1 << 3)))
#define KEY_FOUR_SCAN  (!!(GPIOE->IDR & (0x1 << 4)))
/********************************¡ã¡ä?¨¹¨º?¡Àe?¡§¨°??¨¢¨º?***************************************/

void led_init(void);
void key_init(void);
u8 key_status(void);
#endif

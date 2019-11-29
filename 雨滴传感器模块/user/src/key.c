
#include "key.h"

void delay(u32 t)
{
    while (t)
    {
        t--;
    }
}


void init_key()
{
    GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);
}


u8 key_scan()
{
    static u8 key_sta = 1;
    
    if (key_sta && (KEY1==0 || KEY2==0))
    {
        key_sta = 0;
        delay(100);
        
        if (KEY1 == 0)
        {
            return 1;
        }
        else if (KEY2 == 0)
        {
            return 2;
        }
    }
    else if (KEY1 && KEY2)
    {
        key_sta = 1;
    }
    
    return 0;
}


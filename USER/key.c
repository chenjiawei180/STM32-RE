 /******************************************************************************
  * @file       USER/Key.c
  * @author  cjw
  * @date     2016.1.27
  * @brief     This file contains the source of the Key.
  ******************************************************************************/
#include "key.h"

#if defined KEY_GLOBAL

/**
  * @brief  This function is Init Key GPIO.
  * @param  None
  * @retval None
  */

void Key_IO_Init(void)
{
    GPIO_InitTypeDef  GPIO_Struct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_Struct.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5;
    GPIO_Struct.GPIO_Mode= GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_Struct);
}

/**
  * @brief  This function is Key scan.
  * @param  None
  * @retval Key_value
  */

unsigned char Key_Scan(void)
{
    u8 Key_val=0;
    if(!(KEY_MAIN&&KEY_UP&&KEY_DOWN&&KEY_ESC))
    {
        Key_Delay_us(10000);
        if(!(KEY_MAIN&&KEY_UP&&KEY_DOWN&&KEY_ESC))
        {
            if(!KEY_MAIN) Key_val=KEY_VALUE_MAIN;
            if(!KEY_UP) Key_val=KEY_VALUE_UP;
            if(!KEY_DOWN) Key_val=KEY_VALUE_DOWN;
            if(!KEY_ESC) Key_val=KEY_VALUE_ESC;         
        }  
    }
    return Key_val;
}

/**
  * @brief  This function is Key scan.
  * @param  Time
  * @retval None
  */

void Key_Delay_us(u16 Time)
{
    u8 i;
    while(Time--)
    {
        i=12;
        while(i--);
    }
}

#endif /* KEY_GLOBAL */


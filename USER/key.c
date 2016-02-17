 /******************************************************************************
  * @file       USER/Key.c
  * @author  cjw
  * @date     2016.1.27
  * @brief     This file contains the source of the Key.
  ******************************************************************************/
#include "key.h"
#include "menu.h"
#include "set_system_clock.h"
#include "usart2.h"

#if defined KEY_GLOBAL

Menu_index M_index = STANDBY_MENU;
void (*Function)(void);

u8 gKeyValue=0; /*the value of key press*/

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
    u8 Key_val=0xff;
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

/**
  * @brief  This function is Key Process.It's contain the MENU skip.
  * @param  Time
  * @retval None
  */
void Key_Process(void)
{
    LongPressSec=0;
    gKeyValue=Key_Scan();
    if(gKeyValue)
    {
        switch(gKeyValue)
        {
            case KEY_VALUE_MAIN:
                while(
                        (!KEY_MAIN)&&( ( M_index==STANDBY_MENU )  
                                        //      ||( M_index==ONE_MENU_FA)     
                                              || (M_index==ONE_MENU_FB)     
                                              || (M_index==ONE_MENU_FC)     
                                        //      || (M_index==ONE_MENU_FD)     
                                              || (M_index==ONE_MENU_FE)     )
                       )
                {
                    if( LongPressSec>2)      // ����
                    {
                        switch(M_index)
                        {
                            case STANDBY_MENU :M_index=ONE_MENU_F1; break;
                         //   case ONE_MENU_FA   :M_index=ONE_MENU_F1; break;
                            case ONE_MENU_FB   :M_index=TWO_MENU_FB_SET; break;
                            case ONE_MENU_FC   :M_index=TWO_MENU_FC_SET; break;
                         //   case ONE_MENU_FD   :M_index=ONE_MENU_F1; break;
                            case ONE_MENU_FE   :M_index=TWO_MENU_FE_E1; break;
                            default: break;
                        }
                        
                        Function = MenuProc[M_index].handle;
                        (*Function)();
                        while(!KEY_MAIN) ;
                        return;
                    }                   
                }
                M_index = MenuProc[M_index].Enter;
                Function = MenuProc[M_index].handle;
                (*Function)();
                while(!KEY_MAIN) ;
            break;

            case KEY_VALUE_UP:
                M_index = MenuProc[M_index].Up;
                Function = MenuProc[M_index].handle;  
                (*Function)();
                while(!KEY_UP) ;
            break;

           case KEY_VALUE_DOWN:  
                M_index = MenuProc[M_index].Down;
                Function = MenuProc[M_index].handle;
                (*Function)();  
                while(!KEY_DOWN) ;
            break;

            case KEY_VALUE_ESC:
                M_index = MenuProc[M_index].Return;
                Function = MenuProc[M_index].handle;
                (*Function)();
                while(!KEY_ESC) ;
            break;

            default:
                if(Flag_1_Sec == 1)
                {
                    Flag_1_Sec=0;
                    Function = MenuProc[M_index].handle;  
                    (*Function)();  
                }
            break;        
        }
    }
}

#endif /* KEY_GLOBAL */


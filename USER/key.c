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
#include "eeprom_24c.h"
#include "rf.h"
#include "rf_app.h"
#include "string.h"

#if defined KEY_GLOBAL

Menu_index M_index = STANDBY_MENU;
void (*Function)(void);

u8 gKeyValue=0; /*the value of key press*/
u8 Decoder_First_Long_Press = 0;
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
    if(gKeyValue == 0xff && Host_Enter_Table ==1)
    {
        if(Host_Enter_Times == 0 || Host_Enter_Times > 30)
        {
            Host_Enter_Table = 0;
            switch(Host_Enter_Fun_Id)
            {
                case 0x01:if(Host_Enter_Times  > 20)
                	          {
                	              LongPressSec = 3;
					Host_Enter_Times = 1;
					RF_Same_Count = 1;
					Decoder_First_Long_Press = 1;
					gKeyValue =KEY_VALUE_MAIN;break;
                	          }
                               else if(Decoder_First_Long_Press == 1)
                               {
                                   Decoder_First_Long_Press = 0; break;
                               }
                               else
                               {
                                   gKeyValue =KEY_VALUE_MAIN;break;
                               }
                case 0x02: gKeyValue =KEY_VALUE_UP ; break;
                case 0x04: gKeyValue =KEY_VALUE_DOWN; break;
                case 0x08: gKeyValue =KEY_VALUE_ESC; break;
                default:break;				
            }
        }
        else
        {
            Host_Enter_Times = RF_Same_Count ;
        }
    }
	
    if(gKeyValue)
    {
        switch(gKeyValue)
        {
            case KEY_VALUE_MAIN:
                while(
                        ( (!KEY_MAIN) || LongPressSec > 2)&&( ( M_index==STANDBY_MENU )  
                                        //      ||( M_index==ONE_MENU_FA)     
                                              || (M_index==ONE_MENU_FB)     
                                              || (M_index==ONE_MENU_FC)     
                                        //      || (M_index==ONE_MENU_FD)     
                                              || (M_index==THREE_MENU_F2_E1_D1)   
                                              || (M_index==THREE_MENU_F2_E1_D2)   
                                              || (M_index==THREE_MENU_F2_E1_D3)   
                                              || (M_index==THREE_MENU_F2_E1_D4)   
                                              || (M_index==THREE_MENU_F2_E2_D1)   
                                              || (M_index==THREE_MENU_F2_E2_D2)   
                                              || (M_index==THREE_MENU_F2_E2_D3)   
                                              || (M_index==THREE_MENU_F2_E2_D4)   
                                              || (M_index==THREE_MENU_F2_E3_D1)   
                                              || (M_index==THREE_MENU_F2_E3_D2)   
                                              || (M_index==THREE_MENU_F2_E3_D3)   
                                              || (M_index==THREE_MENU_F2_E3_D4)   
                                              || (M_index==THREE_MENU_F2_E4_D1)   
                                              || (M_index==THREE_MENU_F2_E4_D2)   
                                              || (M_index==THREE_MENU_F2_E4_D3)   
                                              || (M_index==THREE_MENU_F2_E4_D4)   
                                              || (M_index==DECODER_MENU )   
                                              || (M_index==ONE_MENU_FE)     )
                       )
                {
                    if( LongPressSec>2)      // ³¤°´
                    {
                        switch(M_index)
                        {
                            case STANDBY_MENU :M_index=ONE_MENU_F1; break;
                         //   case ONE_MENU_FA   :M_index=ONE_MENU_F1; break;
                            case ONE_MENU_FB   :M_index=TWO_MENU_FB_SET; break;
                            case ONE_MENU_FC   :M_index=TWO_MENU_FC_SET; break;
                         //   case ONE_MENU_FD   :M_index=ONE_MENU_F1; break;
                            case ONE_MENU_FE   :M_index=TWO_MENU_FE_E1; break;

				case THREE_MENU_F2_E1_D1:
				case THREE_MENU_F2_E1_D2:
				case THREE_MENU_F2_E1_D3:
				case THREE_MENU_F2_E1_D4:    Delete_Call_Function(Delete_Call_Buff);    break;

				case THREE_MENU_F2_E2_D1:
				case THREE_MENU_F2_E2_D2:
				case THREE_MENU_F2_E2_D3:
				case THREE_MENU_F2_E2_D4:    Delete_Host_Function(Delete_Host_Buff);    break;

				case THREE_MENU_F2_E3_D1:
				case THREE_MENU_F2_E3_D2:
				case THREE_MENU_F2_E3_D3:
				case THREE_MENU_F2_E3_D4:    Delete_Alarm_Function(Delete_Alarm_Buff);    break;

				case THREE_MENU_F2_E4_D1:
				case THREE_MENU_F2_E4_D2:
				case THREE_MENU_F2_E4_D3:
				case THREE_MENU_F2_E4_D4:    Delete_Cancel_Function(Delete_Cancel_Buff);    break;

                            case DECODER_MENU:    M_index=ONE_MENU_F1;
							                memset(Decoder_Call_Save_Queue,0,800);
									  break;
				
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
		  RF_Flag = 0;
                M_index = MenuProc[M_index].Return;
                Function = MenuProc[M_index].handle;
                (*Function)();
                while(!KEY_ESC) ;
            break;

            default:
                if(Flag_Half_Sec == 1)
                {
                    Flag_Half_Sec=0;
                    Function = MenuProc[M_index].handle;  
                    (*Function)();  
					
                    if(M_index == DECODER_MENU)
                    {
                        Decoder_Function_Of_Remove_Call_Time();
                        Decoder_Function_Of_Cycle_Call_Time();
                    }
                }

            break;        
        }
    }
}

#endif /* KEY_GLOBAL */


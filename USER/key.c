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
#include "transmit.h"
#include "tm1629.h"

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
            Return_Standby_Time_Count = 0;
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
#ifdef STM32_TRANSMIT
    if(gKeyValue == 0xff)
    {
        gKeyValue = ReadDataFrom1629_1();
    }
#endif
    if(gKeyValue == 0xff && Host_Enter_Table ==1)
    {
        if(Host_Enter_Times > 30 && Host_Enter_Fun_Id == 0x01)
        {
            Host_Enter_Table = 0;
            LongPressSec = 3;
            Host_Enter_Times = 1;
            RF_Same_Count = 1;
            Decoder_First_Long_Press = 1;
            gKeyValue =KEY_VALUE_MAIN;
        }
        else if(Host_Enter_Times == 0)
        {
            Host_Enter_Table = 0;
            switch(Host_Enter_Fun_Id)
            {
                case 0x01: if(Decoder_First_Long_Press == 1)
                	           {
                	               Decoder_First_Long_Press = 0;
                	           }
				    else
				    {
				         gKeyValue =KEY_VALUE_MAIN;
				    }
				    break;
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
                        ( (!KEY_MAIN  ||  ReadDataFrom1629_1() == KEY_03 ) || LongPressSec > 2)&&( ( M_index==STANDBY_MENU )  
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
                                              || (M_index==TWO_MENU_F9_E1) 
                                              || (M_index==TWO_MENU_F9_E2) 
                                              || (M_index==DECODER_MENU )   
                                              || (M_index==ONE_MENU_FE)     )
                       )
                {
                    if( LongPressSec>2)      // 长按
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
                            case TWO_MENU_F9_E2 :    Var_Init();
								            Env_Save();
								            __set_FAULTMASK(1);
                                                                    NVIC_SystemReset();
								            break;
                            case TWO_MENU_F9_E1 :    Var_Init();
							                   Key_Init();
								            Env_Save();
							                   EEP_Write_Buffer( SIN_KEY_START, single_key, 16);
							                   EEP_Write_Buffer( MUL_KEY_START, multiple_key, 16);
							                   Delete_All_Data();
								            __set_FAULTMASK(1);
								            NVIC_SystemReset();
								            break;
                            default: break;
                        }
                        
                        Function = MenuProc[M_index].handle;
                        (*Function)();
                        while(!KEY_MAIN  ||  ReadDataFrom1629_1() == KEY_03) ;
                        return;
                    }                   
                }
                M_index = MenuProc[M_index].Enter;
                Function = MenuProc[M_index].handle;
                (*Function)();
                while(!KEY_MAIN  ||  ReadDataFrom1629_1() == KEY_03 ) ;
            break;

            case KEY_VALUE_UP:
                M_index = MenuProc[M_index].Up;
                Function = MenuProc[M_index].handle;  
                (*Function)();
                while(!KEY_UP  ||  ReadDataFrom1629_1() == KEY_11 ) ;
            break;

           case KEY_VALUE_DOWN:  
                M_index = MenuProc[M_index].Down;
                Function = MenuProc[M_index].handle;
                (*Function)();  
                while(!KEY_DOWN  ||  ReadDataFrom1629_1() == KEY_19 ) ;
            break;

            case KEY_VALUE_ESC:
		  RF_Flag = 0;
                M_index = MenuProc[M_index].Return;
                Function = MenuProc[M_index].handle;
                (*Function)();
                while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_27 ) ;
            break;
#ifdef STM32_TRANSMIT
            case KEY_01:break;
            case KEY_02:break;
            //case KEY_03:break;
            case KEY_04:
                Transmit_Set_Data(0);
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_04 ) ;
                break;// 0
            case KEY_05:
                Transmit_Set_Data(7);
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_05) ;
                break; // 7
            case KEY_06:
                Transmit_Set_Data(4);
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_06 ) ;
                break;// 4
            case KEY_07:	
                Transmit_Set_Data(1);
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_07 ) ;
                break;//  1
            case KEY_08:
                if(Transmit_Data < 999)
                {
                    Transmit_Data++;
                }
                else
                {
                    Transmit_Data = 999;
                }
                Display_Ram_To_Tm1629();	
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_08 ) ;
                break;//功能1  号数+1
            case KEY_09:break;
            case KEY_10:break;
            //case KEY_11:break;
            case KEY_12:
                Transmit_Data = 0;
                Display_Ram_To_Tm1629();
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_12 ) ;
                break;//clear
            case KEY_13:
                Transmit_Set_Data(8);
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_13 ) ;
                break;//8
            case KEY_14:
                Transmit_Set_Data(5);
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_14 ) ;
                break;//5
            case KEY_15:
                Transmit_Set_Data(2);
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_15 ) ;
                break;// 2
            case KEY_16:			
                if(Transmit_Data>0)
                {
                    Transmit_Data--;
                }
                else
                {
                    Transmit_Data = 0;
                }
                Display_Ram_To_Tm1629();
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_16 ) ;
                break;//功能2 号数-1
            case KEY_17:break;
            case KEY_18:break;
            //case KEY_19:break;
            case KEY_20:break;
            case KEY_21:
                Transmit_Set_Data(9);
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_21 ) ;
                break;//9
            case KEY_22:
                Transmit_Set_Data(6);
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_22 ) ;
                break;//6
            case KEY_23:
                Transmit_Set_Data(3);
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_23 ) ;
                break;// 3
            case KEY_24:
                Transmit_Number_Of_Set();
                Transmit_Confirm_Key_Press= 1;
                Display_Ram_To_Tm1629();
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_24 ) ;
                break;//功能3 发送确认
            case KEY_25:break;
            case KEY_26:break;
            //case KEY_27:break;
            case KEY_28:break;//F4
            case KEY_29:
                Printer58mm_Print();
                Queue_Number++;
                Display_Ram_To_Tm1629();
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_29 ) ;
                break;//F3//打印
            case KEY_30:
		  Transmit_Number_Of_Reciver_Sub_One();
                Transmit_Confirm_Key_Press= 1;
                Display_Ram_To_Tm1629();
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_30 ) ;
                break;//F2//接收-1
            case KEY_31:
                Transmit_Number_Of_Reciver_Add_One();
                Transmit_Confirm_Key_Press= 1;
                Display_Ram_To_Tm1629();
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_31 ) ;
                break;//F1 //接收+1
            case KEY_32:
                Transmit_Data_Set_FANGQU = 1;
                Display_Ram_To_Tm1629();
		  while(!KEY_ESC  ||  ReadDataFrom1629_1() == KEY_32 ) ;
                break;//功能4		
#endif
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


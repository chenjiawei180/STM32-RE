 /******************************************************************************
  * @file       USER/rf_app.c
  * @author  cjw
  * @date     2016.2.25
  * @brief     This file contains the source of the RF.
  ******************************************************************************/
#include "rf_app.h"
#include "rf.h"
#include "menu.h"
#include "key.h"
#include "eeprom_24c.h"
#include "string.h"
#include "usart2.h"

#if defined RF_GLOBAL

u32 Call_Code_Bak = 0;			//上一次处理的呼叫编码
u32 Call_Off_Time = 0;			//呼叫空闲时间

/**
  * @brief  This function is RF decoder process.
  * @param  None
  * @retval None
  */
  
void Decoder_Process(void)
{
    u8 fun_id;
    u32 call_code;
    if( 
        RF_ID && RF_Flag &&( ( M_index == STANDBY_MENU) 
                            || ( M_index == THREE_MENU_F1_E1_D1) 
                            || ( M_index == THREE_MENU_F1_E1_D2) 
                            || ( M_index == THREE_MENU_F1_E1_D3) 
                            || ( M_index == THREE_MENU_F1_E1_D4) 
                            || ( M_index == THREE_MENU_F1_E2_D1) 
                            || ( M_index == THREE_MENU_F1_E2_D2) 
                            || ( M_index == THREE_MENU_F1_E2_D3) 
                            || ( M_index == THREE_MENU_F1_E2_D4) 
                            || ( M_index == THREE_MENU_F1_E3_D1) 
                            || ( M_index == THREE_MENU_F1_E3_D2) 
                            || ( M_index == THREE_MENU_F1_E3_D3) 
                            || ( M_index == THREE_MENU_F1_E3_D4) 
                            || ( M_index == THREE_MENU_F1_E4_D1) 
                            || ( M_index == THREE_MENU_F1_E4_D2) 
                            || ( M_index == THREE_MENU_F1_E4_D3) 
                            || ( M_index == THREE_MENU_F1_E4_D4) 
        )
    )
    {
        if(RF_Same_Count > 0)
        {
            call_code = RF_ID ;
            //call_code = (RF_ID << 4) + RF_Fun;		//还原24位编码
            //fun_id = Call_Fun_Tab[RF_Fun];	//取默认呼叫功能      
            if(Call_Code_Bak != call_code)
            {
                Call_Code_Bak = call_code;
                switch(M_index)
                {
                    case STANDBY_MENU:Decoder_Standby();    break;

                    case THREE_MENU_F1_E1_D1:
                    case THREE_MENU_F1_E1_D2:
                    case THREE_MENU_F1_E1_D3:
                    case THREE_MENU_F1_E1_D4:Decoder_F1_E1();    break;

                    case THREE_MENU_F1_E2_D1:
                    case THREE_MENU_F1_E2_D2:
                    case THREE_MENU_F1_E2_D3:
                    case THREE_MENU_F1_E2_D4:Decoder_F1_E2();     break;

                    case THREE_MENU_F1_E3_D1:
                    case THREE_MENU_F1_E3_D2:
                    case THREE_MENU_F1_E3_D3:
                    case THREE_MENU_F1_E3_D4:Decoder_F1_E3();     break;

                    case THREE_MENU_F1_E4_D1:
                    case THREE_MENU_F1_E4_D2:
                    case THREE_MENU_F1_E4_D3:
                    case THREE_MENU_F1_E4_D4:Decoder_F1_E4();     break;

                    default:break;
                }
            }
        }
        RF_Flag = 0 ;
        RF_ID = 0;
    }
}

/**
  * @brief  This function is RF decoder Standby.
  * @param  None
  * @retval None
  */

void Decoder_Standby(void)
{

}

/**
  * @brief  This function is RF decoder F1 E1.
  * @param  None
  * @retval None
  */

void Decoder_F1_E1(void)
{
    RF_def tmp;
    memcpy(tmp.region, Register_Call_Buff, 4);
    tmp.rf = RF_ID;
    if (!(Register_Call_Function(&tmp)))
    {
        Buff_Add_One(Register_Call_Buff);
        M_index = THREE_MENU_F1_E1_D1;
    }
}

/**
  * @brief  This function is RF decoder F1 E2.
  * @param  None
  * @retval None
  */

void Decoder_F1_E2(void)
{
    RF_def tmp;
    memcpy(tmp.region, Register_Host_Buff, 4);
    tmp.rf = RF_ID;
    if (!(Register_Host_Function(&tmp)))
    {
        Buff_Add_One(Register_Host_Buff);
        M_index = THREE_MENU_F1_E2_D1;
    }
}

/**
  * @brief  This function is RF decoder F1 E3.
  * @param  None
  * @retval None
  */

void Decoder_F1_E3(void)
{
    RF_def tmp;
    memcpy(tmp.region, Register_Alarm_Buff, 4);
    tmp.rf = RF_ID;
    if (!(Register_Alarm_Function(&tmp)))
    {
        Buff_Add_One(Register_Alarm_Buff);
        M_index = THREE_MENU_F1_E3_D1;
    }
}

/**
  * @brief  This function is RF decoder F1 E4.
  * @param  None
  * @retval None
  */

void Decoder_F1_E4(void)
{
    RF_def tmp;
    memcpy(tmp.region, Register_Cancel_Buff, 4);
    tmp.rf = RF_ID;
    if (!(Register_Cancel_Function(&tmp)))
    {
        Buff_Add_One(Register_Cancel_Buff);
        M_index = THREE_MENU_F1_E4_D1;
    }
}

#endif /* RF_GLOBAL */ 


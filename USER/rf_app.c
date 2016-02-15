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

#if defined RF_GLOBAL

/**
  * @brief  This function is RF decoder process.
  * @param  None
  * @retval None
  */
  
void Decoder_Process(void)
{
    if( 
        RF_Flag &&( ( index == STANDBY_MENU) 
                            || ( index == THREE_MENU_F1_E1_D1) 
                            || ( index == THREE_MENU_F1_E1_D2) 
                            || ( index == THREE_MENU_F1_E1_D3) 
                            || ( index == THREE_MENU_F1_E1_D4) 
                            || ( index == THREE_MENU_F1_E2_D1) 
                            || ( index == THREE_MENU_F1_E2_D2) 
                            || ( index == THREE_MENU_F1_E2_D3) 
                            || ( index == THREE_MENU_F1_E2_D4) 
                            || ( index == THREE_MENU_F1_E3_D1) 
                            || ( index == THREE_MENU_F1_E3_D2) 
                            || ( index == THREE_MENU_F1_E3_D3) 
                            || ( index == THREE_MENU_F1_E3_D4) 
                            || ( index == THREE_MENU_F1_E4_D1) 
                            || ( index == THREE_MENU_F1_E4_D2) 
                            || ( index == THREE_MENU_F1_E4_D3) 
                            || ( index == THREE_MENU_F1_E4_D4) 
        )
    )
    {
        switch(index)
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
        RF_Flag = 0 ;
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
        
    }
}

/**
  * @brief  This function is RF decoder F1 E2.
  * @param  None
  * @retval None
  */

void Decoder_F1_E2(void)
{

}

/**
  * @brief  This function is RF decoder F1 E3.
  * @param  None
  * @retval None
  */

void Decoder_F1_E3(void)
{

}

/**
  * @brief  This function is RF decoder F1 E4.
  * @param  None
  * @retval None
  */

void Decoder_F1_E4(void)
{

}

#endif /* RF_GLOBAL */ 


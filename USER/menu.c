 /******************************************************************************
  * @file       USER/menu.c
  * @author  cjw
  * @date     2016.1.28
  * @brief     This file contains the source of the menu.
  ******************************************************************************/
#include "menu.h"
#include "tm1629.h"
#include "usart2.h"
#include "key.h"

#if defined MENU_GLOBAL

u8 Remove_Call_Time = 0 ;
u8 Cycle_Call_Time = 0;

/**
  * @brief  This is Menu index.
  * @param  None
  * @retval None
  */

const struct Menu_def MenuProc[]=
{
    {STANDBY_MENU,  STANDBY_MENU, STANDBY_MENU, STANDBY_MENU,STANDBY_MENU,Menu_Standby},  
    {ONE_MENU_F0,  TWO_MENU_F0_MINUTE, ONE_MENU_FE, ONE_MENU_F1,STANDBY_MENU,Menu_F0},  
    {ONE_MENU_F1,  TWO_MENU_F1_E1, ONE_MENU_F0, ONE_MENU_F2,STANDBY_MENU,Menu_F1},  
    {ONE_MENU_F2,  TWO_MENU_F2_E1, ONE_MENU_F1, ONE_MENU_F3,STANDBY_MENU,Menu_F2},  
    {ONE_MENU_F3,  TWO_MENU_F3_E1, ONE_MENU_F2, ONE_MENU_F4,STANDBY_MENU,Menu_F3},  
    {ONE_MENU_F4,  TWO_MENU_F4_SET, ONE_MENU_F3, ONE_MENU_F5,STANDBY_MENU,Menu_F4}, 
    {ONE_MENU_F5,  TWO_MENU_F5_SET, ONE_MENU_F4, ONE_MENU_F6,STANDBY_MENU,Menu_F5},  
    {ONE_MENU_F6,  ONE_MENU_F6, ONE_MENU_F5, ONE_MENU_F7,STANDBY_MENU,Menu_F6},  
    {ONE_MENU_F7,  ONE_MENU_F7, ONE_MENU_F6, ONE_MENU_F8,STANDBY_MENU,Menu_F7},  
    {ONE_MENU_F8,  ONE_MENU_F8, ONE_MENU_F7, ONE_MENU_F9,STANDBY_MENU,Menu_F8},  
    {ONE_MENU_F9,  ONE_MENU_F9, ONE_MENU_F8, ONE_MENU_FA,STANDBY_MENU,Menu_F9},  
    {ONE_MENU_FA,  ONE_MENU_FA, ONE_MENU_F9, ONE_MENU_FB,STANDBY_MENU,Menu_FA},  
    {ONE_MENU_FB,  ONE_MENU_FB, ONE_MENU_FA,ONE_MENU_FC,STANDBY_MENU,Menu_FB},  
    {ONE_MENU_FC,  ONE_MENU_FC, ONE_MENU_FB, ONE_MENU_FD,STANDBY_MENU,Menu_FC},  
    {ONE_MENU_FD,  ONE_MENU_FD, ONE_MENU_FC, ONE_MENU_FE,STANDBY_MENU,Menu_FD},  
    {ONE_MENU_FE,  ONE_MENU_FE, ONE_MENU_FD, ONE_MENU_F0,STANDBY_MENU,Menu_FE},

    {TWO_MENU_F0_MINUTE,  TWO_MENU_F0_HOUR, TWO_MENU_F0_MINUTE, TWO_MENU_F0_MINUTE,ONE_MENU_F0,Menu_F0_Minute},
    {TWO_MENU_F0_HOUR,  TWO_MENU_F0_DAY, TWO_MENU_F0_HOUR, TWO_MENU_F0_HOUR,ONE_MENU_F0,Menu_F0_Hour},
    {TWO_MENU_F0_DAY,  TWO_MENU_F0_MONTH, TWO_MENU_F0_DAY, TWO_MENU_F0_DAY,ONE_MENU_F0,Menu_F0_Day},
    {TWO_MENU_F0_MONTH,  TWO_MENU_F0_YEAR, TWO_MENU_F0_MONTH, TWO_MENU_F0_MONTH,ONE_MENU_F0,Menu_F0_Month},
    {TWO_MENU_F0_YEAR,  TWO_MENU_F0_WEEK, TWO_MENU_F0_YEAR, TWO_MENU_F0_YEAR,ONE_MENU_F0,Menu_F0_Year},
    {TWO_MENU_F0_WEEK,  TWO_MENU_F0_MINUTE, TWO_MENU_F0_WEEK, TWO_MENU_F0_WEEK,ONE_MENU_F0,Menu_F0_Week},    

    {TWO_MENU_F1_E1,  TWO_MENU_F1_E1, TWO_MENU_F1_E4, TWO_MENU_F1_E2,ONE_MENU_F1,Menu_F1_E1},
    {TWO_MENU_F1_E2,  TWO_MENU_F1_E2, TWO_MENU_F1_E1, TWO_MENU_F1_E3,ONE_MENU_F1,Menu_F1_E2},
    {TWO_MENU_F1_E3,  TWO_MENU_F1_E3, TWO_MENU_F1_E2, TWO_MENU_F1_E4,ONE_MENU_F1,Menu_F1_E3},
    {TWO_MENU_F1_E4,  TWO_MENU_F1_E4, TWO_MENU_F1_E3, TWO_MENU_F1_E1,ONE_MENU_F1,Menu_F1_E4},
    
    {TWO_MENU_F2_E1,  TWO_MENU_F2_E1, TWO_MENU_F2_E4, TWO_MENU_F2_E2,ONE_MENU_F2,Menu_F2_E1},
    {TWO_MENU_F2_E2,  TWO_MENU_F2_E2, TWO_MENU_F2_E1, TWO_MENU_F2_E3,ONE_MENU_F2,Menu_F2_E2},
    {TWO_MENU_F2_E3,  TWO_MENU_F2_E3, TWO_MENU_F2_E2, TWO_MENU_F2_E4,ONE_MENU_F2,Menu_F2_E3},
    {TWO_MENU_F2_E4,  TWO_MENU_F2_E4, TWO_MENU_F2_E3, TWO_MENU_F2_E1,ONE_MENU_F2,Menu_F2_E4},

    {TWO_MENU_F3_E1,  TWO_MENU_F3_E1, TWO_MENU_F3_E2, TWO_MENU_F3_E2,ONE_MENU_F3,Menu_F3_E1},
    {TWO_MENU_F3_E2,  TWO_MENU_F3_E2, TWO_MENU_F3_E1, TWO_MENU_F3_E1,ONE_MENU_F3,Menu_F3_E2},

    {TWO_MENU_F4_SET,  TWO_MENU_F4_SET, TWO_MENU_F4_SET, TWO_MENU_F4_SET,ONE_MENU_F4,Menu_F4_Set},
    {TWO_MENU_F5_SET,  TWO_MENU_F5_SET, TWO_MENU_F5_SET, TWO_MENU_F5_SET,ONE_MENU_F5,Menu_F5_Set},

    {TWO_MENU_F6_E1,  TWO_MENU_F6_E1, TWO_MENU_F6_E1, TWO_MENU_F6_E1,ONE_MENU_F6,Menu_F0},
    {TWO_MENU_F6_E2,  TWO_MENU_F6_E2, TWO_MENU_F6_E2, TWO_MENU_F6_E2,ONE_MENU_F6,Menu_F0},
    {TWO_MENU_F6_E3,  TWO_MENU_F6_E3, TWO_MENU_F6_E3, TWO_MENU_F6_E3,ONE_MENU_F6,Menu_F0},
    {TWO_MENU_F6_E4,  TWO_MENU_F6_E4, TWO_MENU_F6_E4, TWO_MENU_F6_E4,ONE_MENU_F6,Menu_F0},
    {TWO_MENU_F6_E5,  TWO_MENU_F6_E5, TWO_MENU_F6_E5, TWO_MENU_F6_E5,ONE_MENU_F6,Menu_F0},
    {TWO_MENU_F6_E6,  TWO_MENU_F6_E6, TWO_MENU_F6_E6, TWO_MENU_F6_E6,ONE_MENU_F6,Menu_F0},
    {TWO_MENU_F6_E7,  TWO_MENU_F6_E7, TWO_MENU_F6_E7, TWO_MENU_F6_E7,ONE_MENU_F6,Menu_F0},
    {TWO_MENU_F6_E8,  TWO_MENU_F6_E8, TWO_MENU_F6_E8, TWO_MENU_F6_E8,ONE_MENU_F6,Menu_F0},

    {TWO_MENU_F7_E1,  TWO_MENU_F7_E1, TWO_MENU_F7_E1, TWO_MENU_F7_E1,ONE_MENU_F7,Menu_F0},
    {TWO_MENU_F7_E2,  TWO_MENU_F7_E2, TWO_MENU_F7_E2, TWO_MENU_F7_E2,ONE_MENU_F7,Menu_F0},
    {TWO_MENU_F7_E3,  TWO_MENU_F7_E3, TWO_MENU_F7_E3, TWO_MENU_F7_E3,ONE_MENU_F7,Menu_F0},
    {TWO_MENU_F7_E4,  TWO_MENU_F7_E4, TWO_MENU_F7_E4, TWO_MENU_F7_E4,ONE_MENU_F7,Menu_F0},

    {TWO_MENU_F8_E1,  TWO_MENU_F8_E1, TWO_MENU_F8_E1, TWO_MENU_F8_E1,ONE_MENU_F8,Menu_F0},
    {TWO_MENU_F8_E2,  TWO_MENU_F8_E2, TWO_MENU_F8_E2, TWO_MENU_F8_E2,ONE_MENU_F8,Menu_F0},

    {TWO_MENU_F9_E1,  TWO_MENU_F9_E1, TWO_MENU_F9_E1, TWO_MENU_F9_E1,ONE_MENU_F9,Menu_F0},
    {TWO_MENU_F9_E2,  TWO_MENU_F9_E2, TWO_MENU_F9_E2, TWO_MENU_F9_E2,ONE_MENU_F9,Menu_F0},    

    {TWO_MENU_FA_SET,  TWO_MENU_FA_SET, TWO_MENU_FA_SET, TWO_MENU_FA_SET,ONE_MENU_FA,Menu_F0},
    {TWO_MENU_FB_SET,  TWO_MENU_FB_SET, TWO_MENU_FB_SET, TWO_MENU_FB_SET,ONE_MENU_FB,Menu_F0},
    {TWO_MENU_FC_SET,  TWO_MENU_FC_SET, TWO_MENU_FC_SET, TWO_MENU_FC_SET,ONE_MENU_FC,Menu_F0},
    {TWO_MENU_FD_SET,  TWO_MENU_FD_SET, TWO_MENU_FD_SET, TWO_MENU_FD_SET,ONE_MENU_FD,Menu_F0},
    {TWO_MENU_FE_E1,  TWO_MENU_FE_E1, TWO_MENU_FE_E1, TWO_MENU_FE_E1,ONE_MENU_FE,Menu_F0},
    {TWO_MENU_FE_E2,  TWO_MENU_FE_E2, TWO_MENU_FE_E2, TWO_MENU_FE_E2,ONE_MENU_FE,Menu_F0},    

    {THREE_MENU_F1_E1_D1,  THREE_MENU_F1_E1_D1, THREE_MENU_F1_E1_D1, THREE_MENU_F1_E1_D1,TWO_MENU_F1_E1,Menu_F0},  
    {THREE_MENU_F1_E1_D2,  THREE_MENU_F1_E1_D2, THREE_MENU_F1_E1_D2, THREE_MENU_F1_E1_D2,TWO_MENU_F1_E1,Menu_F0},
    {THREE_MENU_F1_E1_D3,  THREE_MENU_F1_E1_D3, THREE_MENU_F1_E1_D3, THREE_MENU_F1_E1_D3,TWO_MENU_F1_E1,Menu_F0},
    {THREE_MENU_F1_E1_D4,  THREE_MENU_F1_E1_D4, THREE_MENU_F1_E1_D4, THREE_MENU_F1_E1_D4,TWO_MENU_F1_E1,Menu_F0},

    {THREE_MENU_F1_E2_D1,  THREE_MENU_F1_E2_D1, THREE_MENU_F1_E2_D1, THREE_MENU_F1_E2_D1,TWO_MENU_F1_E2,Menu_F0},  
    {THREE_MENU_F1_E2_D2,  THREE_MENU_F1_E2_D2, THREE_MENU_F1_E2_D2, THREE_MENU_F1_E2_D2,TWO_MENU_F1_E2,Menu_F0},
    {THREE_MENU_F1_E2_D3,  THREE_MENU_F1_E2_D3, THREE_MENU_F1_E2_D3, THREE_MENU_F1_E2_D3,TWO_MENU_F1_E2,Menu_F0},
    {THREE_MENU_F1_E2_D4,  THREE_MENU_F1_E2_D4, THREE_MENU_F1_E2_D4, THREE_MENU_F1_E2_D4,TWO_MENU_F1_E2,Menu_F0},

    {THREE_MENU_F1_E3_D1,  THREE_MENU_F1_E3_D1, THREE_MENU_F1_E3_D1, THREE_MENU_F1_E3_D1,TWO_MENU_F1_E3,Menu_F0},  
    {THREE_MENU_F1_E3_D2,  THREE_MENU_F1_E3_D2, THREE_MENU_F1_E3_D2, THREE_MENU_F1_E3_D2,TWO_MENU_F1_E3,Menu_F0},
    {THREE_MENU_F1_E3_D3,  THREE_MENU_F1_E3_D3, THREE_MENU_F1_E3_D3, THREE_MENU_F1_E3_D3,TWO_MENU_F1_E3,Menu_F0},
    {THREE_MENU_F1_E3_D4,  THREE_MENU_F1_E3_D4, THREE_MENU_F1_E3_D4, THREE_MENU_F1_E3_D4,TWO_MENU_F1_E3,Menu_F0},

    {THREE_MENU_F1_E4_D1,  THREE_MENU_F1_E4_D1, THREE_MENU_F1_E4_D1, THREE_MENU_F1_E4_D1,TWO_MENU_F1_E4,Menu_F0},  
    {THREE_MENU_F1_E4_D2,  THREE_MENU_F1_E4_D2, THREE_MENU_F1_E4_D2, THREE_MENU_F1_E4_D2,TWO_MENU_F1_E4,Menu_F0},
    {THREE_MENU_F1_E4_D3,  THREE_MENU_F1_E4_D3, THREE_MENU_F1_E4_D3, THREE_MENU_F1_E4_D3,TWO_MENU_F1_E4,Menu_F0},
    {THREE_MENU_F1_E4_D4,  THREE_MENU_F1_E4_D4, THREE_MENU_F1_E4_D4, THREE_MENU_F1_E4_D4,TWO_MENU_F1_E4,Menu_F0},

    {THREE_MENU_F2_E1_D1,  THREE_MENU_F2_E1_D1, THREE_MENU_F2_E1_D1, THREE_MENU_F2_E1_D1,TWO_MENU_F2_E1,Menu_F0},  
    {THREE_MENU_F2_E1_D2,  THREE_MENU_F2_E1_D2, THREE_MENU_F2_E1_D2, THREE_MENU_F2_E1_D2,TWO_MENU_F2_E1,Menu_F0},
    {THREE_MENU_F2_E1_D3,  THREE_MENU_F2_E1_D3, THREE_MENU_F2_E1_D3, THREE_MENU_F2_E1_D3,TWO_MENU_F2_E1,Menu_F0},
    {THREE_MENU_F2_E1_D4,  THREE_MENU_F2_E1_D4, THREE_MENU_F2_E1_D4, THREE_MENU_F2_E1_D4,TWO_MENU_F2_E1,Menu_F0},

    {THREE_MENU_F2_E2_D1,  THREE_MENU_F2_E2_D1, THREE_MENU_F2_E2_D1, THREE_MENU_F2_E2_D1,TWO_MENU_F2_E2,Menu_F0},  
    {THREE_MENU_F2_E2_D2,  THREE_MENU_F2_E2_D2, THREE_MENU_F2_E2_D2, THREE_MENU_F2_E2_D2,TWO_MENU_F2_E2,Menu_F0},
    {THREE_MENU_F2_E2_D3,  THREE_MENU_F2_E2_D3, THREE_MENU_F2_E2_D3, THREE_MENU_F2_E2_D3,TWO_MENU_F2_E2,Menu_F0},
    {THREE_MENU_F2_E2_D4,  THREE_MENU_F2_E2_D4, THREE_MENU_F2_E2_D4, THREE_MENU_F2_E2_D4,TWO_MENU_F2_E2,Menu_F0},

    {THREE_MENU_F2_E3_D1,  THREE_MENU_F2_E3_D1, THREE_MENU_F2_E3_D1, THREE_MENU_F2_E3_D1,TWO_MENU_F2_E3,Menu_F0},  
    {THREE_MENU_F2_E3_D2,  THREE_MENU_F2_E3_D2, THREE_MENU_F2_E3_D2, THREE_MENU_F2_E3_D2,TWO_MENU_F2_E3,Menu_F0},
    {THREE_MENU_F2_E3_D3,  THREE_MENU_F2_E3_D3, THREE_MENU_F2_E3_D3, THREE_MENU_F2_E3_D3,TWO_MENU_F2_E3,Menu_F0},
    {THREE_MENU_F2_E3_D4,  THREE_MENU_F2_E3_D4, THREE_MENU_F2_E3_D4, THREE_MENU_F2_E3_D4,TWO_MENU_F2_E3,Menu_F0},

    {THREE_MENU_F2_E4_D1,  THREE_MENU_F2_E4_D1, THREE_MENU_F2_E4_D1, THREE_MENU_F2_E4_D1,TWO_MENU_F2_E4,Menu_F0},  
    {THREE_MENU_F2_E4_D2,  THREE_MENU_F2_E4_D2, THREE_MENU_F2_E4_D2, THREE_MENU_F2_E4_D2,TWO_MENU_F2_E4,Menu_F0},
    {THREE_MENU_F2_E4_D3,  THREE_MENU_F2_E4_D3, THREE_MENU_F2_E4_D3, THREE_MENU_F2_E4_D3,TWO_MENU_F2_E4,Menu_F0},
    {THREE_MENU_F2_E4_D4,  THREE_MENU_F2_E4_D4, THREE_MENU_F2_E4_D4, THREE_MENU_F2_E4_D4,TWO_MENU_F2_E4,Menu_F0},

    {THREE_MENU_F3_E1_SET,  THREE_MENU_F3_E1_SET, THREE_MENU_F3_E1_SET, THREE_MENU_F3_E1_SET,TWO_MENU_F3_E1,Menu_F0},    
    {THREE_MENU_F3_E2_SET,  THREE_MENU_F3_E2_SET, THREE_MENU_F3_E2_SET, THREE_MENU_F3_E2_SET,TWO_MENU_F3_E2,Menu_F0},    

    {THREE_MENU_F6_E1_SET,  THREE_MENU_F6_E1_SET, THREE_MENU_F6_E1_SET, THREE_MENU_F6_E1_SET,TWO_MENU_F6_E1,Menu_F0},      
    {THREE_MENU_F6_E2_SET,  THREE_MENU_F6_E2_SET, THREE_MENU_F6_E2_SET, THREE_MENU_F6_E2_SET,TWO_MENU_F6_E2,Menu_F0},    
    {THREE_MENU_F6_E3_SET,  THREE_MENU_F6_E3_SET, THREE_MENU_F6_E3_SET, THREE_MENU_F6_E3_SET,TWO_MENU_F6_E3,Menu_F0},    
    {THREE_MENU_F6_E4_SET,  THREE_MENU_F6_E4_SET, THREE_MENU_F6_E4_SET, THREE_MENU_F6_E4_SET,TWO_MENU_F6_E4,Menu_F0},    
    {THREE_MENU_F6_E5_SET,  THREE_MENU_F6_E5_SET, THREE_MENU_F6_E5_SET, THREE_MENU_F6_E5_SET,TWO_MENU_F6_E5,Menu_F0},    
    {THREE_MENU_F6_E6_SET,  THREE_MENU_F6_E6_SET, THREE_MENU_F6_E6_SET, THREE_MENU_F6_E6_SET,TWO_MENU_F6_E6,Menu_F0},    
    {THREE_MENU_F6_E7_SET,  THREE_MENU_F6_E7_SET, THREE_MENU_F6_E7_SET, THREE_MENU_F6_E7_SET,TWO_MENU_F6_E7,Menu_F0},    
    {THREE_MENU_F6_E8_SET,  THREE_MENU_F6_E8_SET, THREE_MENU_F6_E8_SET, THREE_MENU_F6_E8_SET,TWO_MENU_F6_E8,Menu_F0},    

    {THREE_MENU_F7_E1_SET,  THREE_MENU_F7_E1_SET, THREE_MENU_F7_E1_SET, THREE_MENU_F7_E1_SET,TWO_MENU_F7_E1,Menu_F0},      
    {THREE_MENU_F7_E2_SET,  THREE_MENU_F7_E2_SET, THREE_MENU_F7_E2_SET, THREE_MENU_F7_E2_SET,TWO_MENU_F7_E2,Menu_F0},    
    {THREE_MENU_F7_E3_SET,  THREE_MENU_F7_E3_SET, THREE_MENU_F7_E3_SET, THREE_MENU_F7_E3_SET,TWO_MENU_F7_E3,Menu_F0},    
    {THREE_MENU_F7_E4_SET,  THREE_MENU_F7_E4_SET, THREE_MENU_F7_E4_SET, THREE_MENU_F7_E4_SET,TWO_MENU_F7_E4,Menu_F0},   

    {THREE_MENU_F8_E1_SET,  THREE_MENU_F8_E1_SET, THREE_MENU_F8_E1_SET, THREE_MENU_F8_E1_SET,TWO_MENU_F8_E1,Menu_F0},      
    {THREE_MENU_F8_E2_SET,  THREE_MENU_F8_E2_SET, THREE_MENU_F8_E2_SET, THREE_MENU_F8_E2_SET,TWO_MENU_F8_E2,Menu_F0},    

    {THREE_MENU_FE_E1_SET,  THREE_MENU_FE_E1_SET, THREE_MENU_FE_E1_SET, THREE_MENU_FE_E1_SET,TWO_MENU_FE_E1,Menu_F0},      
    {THREE_MENU_FE_E2_SET,  THREE_MENU_FE_E2_SET, THREE_MENU_FE_E2_SET, THREE_MENU_FE_E2_SET,TWO_MENU_FE_E2,Menu_F0},    

    {DECODER_MENU,  DECODER_MENU, DECODER_MENU, DECODER_MENU,DECODER_MENU,Menu_F0},    

};

/**
  * @brief  This function is Show TM1629 of Standby menu.
  * @param  None
  * @retval None
  */

void Menu_Standby(void)
{
    static u8 index=0;
    Tm1629_Clear();
    Tm1629_Display_Ram[0][index&0x03]=0x40; /* - */
    Tm1629_Display();
    index++;
}

/**
  * @brief  This function is Show TM1629 of F0 menu.
  * @param  None
  * @retval None
  */

void Menu_F0(void)
{
    Tm1629_Show_Fx(0x00);
}

/**
  * @brief  This function is Show TM1629 of F1 menu.
  * @param  None
  * @retval None
  */
  
void Menu_F1(void)
{
    Tm1629_Show_Fx(0x01);
}

/**
  * @brief  This function is Show TM1629 of F2 menu.
  * @param  None
  * @retval None
  */
  
void Menu_F2(void)
{
    Tm1629_Show_Fx(0x02);
}

/**
  * @brief  This function is Show TM1629 of F3 menu.
  * @param  None
  * @retval None
  */
  
void Menu_F3(void)
{
    Tm1629_Show_Fx(0x03);
}

/**
  * @brief  This function is Show TM1629 of F4 menu.
  * @param  None
  * @retval None
  */

void Menu_F4(void)
{
    Tm1629_Show_Fx(0x04);
}

/**
  * @brief  This function is Show TM1629 of F5 menu.
  * @param  None
  * @retval None
  */

void Menu_F5(void)
{
    Tm1629_Show_Fx(0x05);
}

/**
  * @brief  This function is Show TM1629 of F6 menu.
  * @param  None
  * @retval None
  */
  
void Menu_F6(void)
{
    Tm1629_Show_Fx(0x06);
}

/**
  * @brief  This function is Show TM1629 of F7 menu.
  * @param  None
  * @retval None
  */
  
void Menu_F7(void)
{
    Tm1629_Show_Fx(0x07);
}

/**
  * @brief  This function is Show TM1629 of F8 menu.
  * @param  None
  * @retval None
  */
  
void Menu_F8(void)
{
    Tm1629_Show_Fx(0x08);
}

/**
  * @brief  This function is Show TM1629 of F9 menu.
  * @param  None
  * @retval None
  */
  
void Menu_F9(void)
{
    Tm1629_Show_Fx(0x09);
}

/**
  * @brief  This function is Show TM1629 of FA menu.
  * @param  None
  * @retval None
  */
  
void Menu_FA(void)
{
    Tm1629_Show_Fx(0x0A);
}

/**
  * @brief  This function is Show TM1629 of FB menu.
  * @param  None
  * @retval None
  */
  
void Menu_FB(void)
{
    Tm1629_Show_Fx(0x0B);
}

/**
  * @brief  This function is Show TM1629 of FC menu.
  * @param  None
  * @retval None
  */
  
void Menu_FC(void)
{
    Tm1629_Show_Fx(0x0C);
}

/**
  * @brief  This function is Show TM1629 of FD menu.
  * @param  None
  * @retval None
  */
  
void Menu_FD(void)
{
    Tm1629_Show_Fx(0x0D);
}

/**
  * @brief  This function is Show TM1629 of FE menu.
  * @param  None
  * @retval None
  */
  
void Menu_FE(void)
{
    Tm1629_Show_Fx(0x0E);
}

/**
  * @brief  This function is Show TM1629 of F0 menu show Minute.
  * @param  None
  * @retval None
  */
  
void Menu_F0_Minute(void)
{
    Tm1629_Blink_Time_Minute();
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Tm1629_Test_Time[5] == 59)
            Tm1629_Test_Time[5]=0;
        else
            Tm1629_Test_Time[5]++;
        Tm1629_Show_Time(Tm1629_Test_Time);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Tm1629_Test_Time[5] == 0)
            Tm1629_Test_Time[5]=59;
        else
            Tm1629_Test_Time[5]--;
        Tm1629_Show_Time(Tm1629_Test_Time);
    }
}

/**
  * @brief  This function is Show TM1629 of F0 menu show Hour.
  * @param  None
  * @retval None
  */
  
void Menu_F0_Hour(void)
{
    Tm1629_Blink_Time_Hour();
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Tm1629_Test_Time[4] == 23)
            Tm1629_Test_Time[4]=0;
        else
            Tm1629_Test_Time[4]++;
        Tm1629_Show_Time(Tm1629_Test_Time);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Tm1629_Test_Time[4] == 0)
            Tm1629_Test_Time[4]=23;
        else
            Tm1629_Test_Time[4]--;
        Tm1629_Show_Time(Tm1629_Test_Time);
    }
}

/**
  * @brief  This function is Show TM1629 of F0 menu show Day.
  * @param  None
  * @retval None
  */
  
void Menu_F0_Day(void)
{
    Tm1629_Blink_Time_Day();
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Tm1629_Test_Time[3] == 31)
            Tm1629_Test_Time[3]=1;
        else
            Tm1629_Test_Time[3]++;
        Tm1629_Show_Time(Tm1629_Test_Time);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Tm1629_Test_Time[3] == 1)
            Tm1629_Test_Time[3]=31;
        else
            Tm1629_Test_Time[3]--;
        Tm1629_Show_Time(Tm1629_Test_Time);
    }
}

/**
  * @brief  This function is Show TM1629 of F0 menu show Month.
  * @param  None
  * @retval None
  */
  
void Menu_F0_Month(void)
{
    Tm1629_Blink_Time_Month();
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Tm1629_Test_Time[2] == 12)
            Tm1629_Test_Time[2]=1;
        else
            Tm1629_Test_Time[2]++;
        Tm1629_Show_Time(Tm1629_Test_Time);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Tm1629_Test_Time[2] == 1)
            Tm1629_Test_Time[2]=12;
        else
            Tm1629_Test_Time[2]--;
        Tm1629_Show_Time(Tm1629_Test_Time);
    }
}

/**
  * @brief  This function is Show TM1629 of F0 menu show Year.
  * @param  None
  * @retval None
  */
  
void Menu_F0_Year(void)
{
    Tm1629_Blink_Time_Year();
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Tm1629_Test_Time[1] == 99)
            Tm1629_Test_Time[1]=0;
        else
            Tm1629_Test_Time[1]++;
        Tm1629_Show_Time(Tm1629_Test_Time);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Tm1629_Test_Time[1] == 0)
            Tm1629_Test_Time[1]=99;
        else
            Tm1629_Test_Time[1]--;
        Tm1629_Show_Time(Tm1629_Test_Time);
    }
}

/**
  * @brief  This function is Show TM1629 of F0 menu show Week.
  * @param  None
  * @retval None
  */

void Menu_F0_Week(void)
{
    Tm1629_Blink_Time_Week();
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Tm1629_Test_Time[7] == 7)
            Tm1629_Test_Time[7]=0;
        else
            Tm1629_Test_Time[7]++;
        Tm1629_Show_Time(Tm1629_Test_Time);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Tm1629_Test_Time[7] == 1)
            Tm1629_Test_Time[7]=7;
        else
            Tm1629_Test_Time[7]--;
        Tm1629_Show_Time(Tm1629_Test_Time);
    }
}

/**
  * @brief  This function is Show sub menu of F1. E1
  * @param  None
  * @retval None
  */
  
void Menu_F1_E1(void)
{
    Tm1629_Show_Ex(0x01);
}

/**
  * @brief  This function is Show sub menu of F1. E2
  * @param  None
  * @retval None
  */
  
void Menu_F1_E2(void)
{
    Tm1629_Show_Ex(0x02);
}

/**
  * @brief  This function is Show sub menu of F1. E3
  * @param  None
  * @retval None
  */
  
void Menu_F1_E3(void)
{
    Tm1629_Show_Ex(0x03);
}

/**
  * @brief  This function is Show sub menu of F1. E4
  * @param  None
  * @retval None
  */
  
void Menu_F1_E4(void)
{
    Tm1629_Show_Ex(0x04);
}

/**
  * @brief  This function is Show sub menu of F2. E1
  * @param  None
  * @retval None
  */
  
void Menu_F2_E1(void)
{
    Tm1629_Show_Ex(0x01);
}

/**
  * @brief  This function is Show sub menu of F2. E2
  * @param  None
  * @retval None
  */
  
void Menu_F2_E2(void)
{
    Tm1629_Show_Ex(0x02);
}

/**
  * @brief  This function is Show sub menu of F2. E3
  * @param  None
  * @retval None
  */
  
void Menu_F2_E3(void)
{
    Tm1629_Show_Ex(0x03);
}

/**
  * @brief  This function is Show sub menu of F2. E4
  * @param  None
  * @retval None
  */
  
void Menu_F2_E4(void)
{
    Tm1629_Show_Ex(0x04);
}

/**
  * @brief  This function is Show sub menu of F3. E1
  * @param  None
  * @retval None
  */
  
void Menu_F3_E1(void)
{
    Tm1629_Show_Ex(0x01);
}

/**
  * @brief  This function is Show sub menu of F3. E2
  * @param  None
  * @retval None
  */
  
void Menu_F3_E2(void)
{
    Tm1629_Show_Ex(0x02);
}

/**
  * @brief  This function is Show menu of F4.
  * @param  None
  * @retval None
  */
  
void Menu_F4_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Remove_Call_Time == 99)
            Remove_Call_Time=0;
        else
            Remove_Call_Time++;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Remove_Call_Time == 0)
            Remove_Call_Time=99;
        else
            Remove_Call_Time--;
    }
    Tm1629_Show_Two_Number(Remove_Call_Time); 
}

/**
  * @brief  This function is Show menu of F5.
  * @param  None
  * @retval None
  */
  
void Menu_F5_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Cycle_Call_Time== 99)
            Cycle_Call_Time=0;
        else
            Cycle_Call_Time++;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Cycle_Call_Time == 0)
            Cycle_Call_Time=99;
        else
            Cycle_Call_Time--;
    }
    Tm1629_Show_Two_Number(Cycle_Call_Time); 
}



#endif /* MENU_GLOBAL */


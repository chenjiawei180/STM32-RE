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
#include "rf_app.h"

#if defined MENU_GLOBAL

u8 Remove_Call_Time = 0 ;
u8 Cycle_Call_Time = 0;

u8 Change_Standby_Display_Mode = 0;    /* FC */
u8 Set_Key_Remove_Call_Is_Or_Not = 0;    /* FB */

u8 Register_Call_Buff[5]={0,0,0,1,0};
u8 Delete_Call_Buff[5]={0,0,0,1,0};
u8 Register_Host_Buff[5]={0,0,0,1,0};
u8 Delete_Host_Buff[5]={0,0,0,1,0};
u8 Register_Alarm_Buff[5]={0,0,0,1,0};
u8 Delete_Alarm_Buff[5]={0,0,0,1,0};
u8 Register_Cancel_Buff[5]={0,0,0,1,0};
u8 Delete_Cancel_Buff[5]={0,0,0,1,0};

u8 Set_Call_Queue_Mode = 1;
u8 Set_Call_Display_Number= 10;
u8 Set_Voice_Play_Mode = 0;
u8 Set_Voice_Play_Time = 1;
u8 Set_Voice_Play_Is_Or_Not_In_Cycle_Mode = 0;
u8 Set_Voice_Volume = 6;
u8 Set_Display_Tube_Brightness = 3;
u8 Set_Voice_Navigation_Mode = 1;
u8 Set_Voice_Navigation_On_Or_OFF = 0;
u8 Set_Display_LED_Mode = 0;

u8 Set_Two_Menu_F7_E1 = 11;
u8 Set_Two_Menu_F7_E2 = 0;
u8 Set_Two_Menu_F7_E3 = 0;
u8 Set_Two_Menu_F7_E4 = 0;

u8 Set_Singal_Or_Multiple_Key_Mode = 2;
u8 Set_Key_Of_Call_Mode = 0 ;

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
    {ONE_MENU_F6,  TWO_MENU_F6_E1, ONE_MENU_F5, ONE_MENU_F7,STANDBY_MENU,Menu_F6},  
    {ONE_MENU_F7,  TWO_MENU_F7_E1, ONE_MENU_F6, ONE_MENU_F8,STANDBY_MENU,Menu_F7},  
    {ONE_MENU_F8,  TWO_MENU_F8_E1, ONE_MENU_F7, ONE_MENU_F9,STANDBY_MENU,Menu_F8},  
    {ONE_MENU_F9,  TWO_MENU_F9_E1, ONE_MENU_F8, ONE_MENU_FA,STANDBY_MENU,Menu_F9},  
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

    {TWO_MENU_F1_E1,  THREE_MENU_F1_E1_D1, TWO_MENU_F1_E4, TWO_MENU_F1_E2,ONE_MENU_F1,Menu_F1_E1},
    {TWO_MENU_F1_E2,  THREE_MENU_F1_E2_D1, TWO_MENU_F1_E1, TWO_MENU_F1_E3,ONE_MENU_F1,Menu_F1_E2},
    {TWO_MENU_F1_E3,  THREE_MENU_F1_E3_D1, TWO_MENU_F1_E2, TWO_MENU_F1_E4,ONE_MENU_F1,Menu_F1_E3},
    {TWO_MENU_F1_E4,  THREE_MENU_F1_E4_D1, TWO_MENU_F1_E3, TWO_MENU_F1_E1,ONE_MENU_F1,Menu_F1_E4},
    
    {TWO_MENU_F2_E1,  THREE_MENU_F2_E1_D1, TWO_MENU_F2_E4, TWO_MENU_F2_E2,ONE_MENU_F2,Menu_F2_E1},
    {TWO_MENU_F2_E2,  THREE_MENU_F2_E2_D1, TWO_MENU_F2_E1, TWO_MENU_F2_E3,ONE_MENU_F2,Menu_F2_E2},
    {TWO_MENU_F2_E3,  THREE_MENU_F2_E3_D1, TWO_MENU_F2_E2, TWO_MENU_F2_E4,ONE_MENU_F2,Menu_F2_E3},
    {TWO_MENU_F2_E4,  THREE_MENU_F2_E4_D1, TWO_MENU_F2_E3, TWO_MENU_F2_E1,ONE_MENU_F2,Menu_F2_E4},

    {TWO_MENU_F3_E1,  THREE_MENU_F3_E1_SET, TWO_MENU_F3_E2, TWO_MENU_F3_E2,ONE_MENU_F3,Menu_F3_E1},
    {TWO_MENU_F3_E2,  THREE_MENU_F3_E2_SET, TWO_MENU_F3_E1, TWO_MENU_F3_E1,ONE_MENU_F3,Menu_F3_E2},

    {TWO_MENU_F4_SET,  TWO_MENU_F4_SET, TWO_MENU_F4_SET, TWO_MENU_F4_SET,ONE_MENU_F4,Menu_F4_Set},
    {TWO_MENU_F5_SET,  TWO_MENU_F5_SET, TWO_MENU_F5_SET, TWO_MENU_F5_SET,ONE_MENU_F5,Menu_F5_Set},

    {TWO_MENU_F6_E1,  THREE_MENU_F6_E1_SET, TWO_MENU_F6_E8, TWO_MENU_F6_E2,ONE_MENU_F6,Menu_F6_E1},
    {TWO_MENU_F6_E2,  THREE_MENU_F6_E2_SET, TWO_MENU_F6_E1, TWO_MENU_F6_E3,ONE_MENU_F6,Menu_F6_E2},
    {TWO_MENU_F6_E3,  THREE_MENU_F6_E3_SET, TWO_MENU_F6_E2, TWO_MENU_F6_E4,ONE_MENU_F6,Menu_F6_E3},
    {TWO_MENU_F6_E4,  THREE_MENU_F6_E4_SET, TWO_MENU_F6_E3, TWO_MENU_F6_E5,ONE_MENU_F6,Menu_F6_E4},
    {TWO_MENU_F6_E5,  THREE_MENU_F6_E5_SET, TWO_MENU_F6_E4, TWO_MENU_F6_E6,ONE_MENU_F6,Menu_F6_E5},
    {TWO_MENU_F6_E6,  THREE_MENU_F6_E6_SET, TWO_MENU_F6_E5, TWO_MENU_F6_E7,ONE_MENU_F6,Menu_F6_E6},
    {TWO_MENU_F6_E7,  THREE_MENU_F6_E7_SET, TWO_MENU_F6_E6, TWO_MENU_F6_E8,ONE_MENU_F6,Menu_F6_E7},
    {TWO_MENU_F6_E8,  THREE_MENU_F6_E8_SET, TWO_MENU_F6_E7, TWO_MENU_F6_E1,ONE_MENU_F6,Menu_F6_E8},

    {TWO_MENU_F7_E1,  THREE_MENU_F7_E1_SET, TWO_MENU_F7_E4, TWO_MENU_F7_E2,ONE_MENU_F7,Menu_F7_E1},
    {TWO_MENU_F7_E2,  THREE_MENU_F7_E2_SET, TWO_MENU_F7_E1, TWO_MENU_F7_E3,ONE_MENU_F7,Menu_F7_E2},
    {TWO_MENU_F7_E3,  THREE_MENU_F7_E3_SET, TWO_MENU_F7_E2, TWO_MENU_F7_E4,ONE_MENU_F7,Menu_F7_E3},
    {TWO_MENU_F7_E4,  THREE_MENU_F7_E4_SET, TWO_MENU_F7_E3, TWO_MENU_F7_E1,ONE_MENU_F7,Menu_F7_E4},

    {TWO_MENU_F8_E1,  THREE_MENU_F8_E1_SET, TWO_MENU_F8_E2, TWO_MENU_F8_E2,ONE_MENU_F8,Menu_F8_E1},
    {TWO_MENU_F8_E2,  THREE_MENU_F8_E2_SET, TWO_MENU_F8_E1, TWO_MENU_F8_E1,ONE_MENU_F8,Menu_F8_E2},

    {TWO_MENU_F9_E1,  TWO_MENU_F9_E1, TWO_MENU_F9_E2, TWO_MENU_F9_E2,ONE_MENU_F9,Menu_F9_E1},
    {TWO_MENU_F9_E2,  TWO_MENU_F9_E2, TWO_MENU_F9_E1, TWO_MENU_F9_E1,ONE_MENU_F9,Menu_F9_E2},    

    {TWO_MENU_FA_SET,  TWO_MENU_FA_SET, TWO_MENU_FA_SET, TWO_MENU_FA_SET,ONE_MENU_FA,Menu_FA_Set},
    {TWO_MENU_FB_SET,  TWO_MENU_FB_SET, TWO_MENU_FB_SET, TWO_MENU_FB_SET,ONE_MENU_FB,Menu_FB_Set},
    {TWO_MENU_FC_SET,  TWO_MENU_FC_SET, TWO_MENU_FC_SET, TWO_MENU_FC_SET,ONE_MENU_FC,Menu_FC_Set},
    {TWO_MENU_FD_SET,  TWO_MENU_FD_SET, TWO_MENU_FD_SET, TWO_MENU_FD_SET,ONE_MENU_FD,Menu_FD_Set},
    {TWO_MENU_FE_E1,  TWO_MENU_FE_E1, TWO_MENU_FE_E2, TWO_MENU_FE_E2,ONE_MENU_FE,Menu_FE_E1},
    {TWO_MENU_FE_E2,  TWO_MENU_FE_E2, TWO_MENU_FE_E1, TWO_MENU_FE_E1,ONE_MENU_FE,Menu_FE_E2},    

    {THREE_MENU_F1_E1_D1,  THREE_MENU_F1_E1_D2, THREE_MENU_F1_E1_D1, THREE_MENU_F1_E1_D1,TWO_MENU_F1_E1,Menu_F1_E1_D1},  
    {THREE_MENU_F1_E1_D2,  THREE_MENU_F1_E1_D3, THREE_MENU_F1_E1_D2, THREE_MENU_F1_E1_D2,TWO_MENU_F1_E1,Menu_F1_E1_D2},
    {THREE_MENU_F1_E1_D3,  THREE_MENU_F1_E1_D4, THREE_MENU_F1_E1_D3, THREE_MENU_F1_E1_D3,TWO_MENU_F1_E1,Menu_F1_E1_D3},
    {THREE_MENU_F1_E1_D4,  THREE_MENU_F1_E1_D1, THREE_MENU_F1_E1_D4, THREE_MENU_F1_E1_D4,TWO_MENU_F1_E1,Menu_F1_E1_D4},

    {THREE_MENU_F1_E2_D1,  THREE_MENU_F1_E2_D2, THREE_MENU_F1_E2_D1, THREE_MENU_F1_E2_D1,TWO_MENU_F1_E2,Menu_F1_E2_D1},  
    {THREE_MENU_F1_E2_D2,  THREE_MENU_F1_E2_D3, THREE_MENU_F1_E2_D2, THREE_MENU_F1_E2_D2,TWO_MENU_F1_E2,Menu_F1_E2_D2},
    {THREE_MENU_F1_E2_D3,  THREE_MENU_F1_E2_D4, THREE_MENU_F1_E2_D3, THREE_MENU_F1_E2_D3,TWO_MENU_F1_E2,Menu_F1_E2_D3},
    {THREE_MENU_F1_E2_D4,  THREE_MENU_F1_E2_D1, THREE_MENU_F1_E2_D4, THREE_MENU_F1_E2_D4,TWO_MENU_F1_E2,Menu_F1_E2_D4},

    {THREE_MENU_F1_E3_D1,  THREE_MENU_F1_E3_D2, THREE_MENU_F1_E3_D1, THREE_MENU_F1_E3_D1,TWO_MENU_F1_E3,Menu_F1_E3_D1},  
    {THREE_MENU_F1_E3_D2,  THREE_MENU_F1_E3_D3, THREE_MENU_F1_E3_D2, THREE_MENU_F1_E3_D2,TWO_MENU_F1_E3,Menu_F1_E3_D2},
    {THREE_MENU_F1_E3_D3,  THREE_MENU_F1_E3_D4, THREE_MENU_F1_E3_D3, THREE_MENU_F1_E3_D3,TWO_MENU_F1_E3,Menu_F1_E3_D3},
    {THREE_MENU_F1_E3_D4,  THREE_MENU_F1_E3_D1, THREE_MENU_F1_E3_D4, THREE_MENU_F1_E3_D4,TWO_MENU_F1_E3,Menu_F1_E3_D4},

    {THREE_MENU_F1_E4_D1,  THREE_MENU_F1_E4_D2, THREE_MENU_F1_E4_D1, THREE_MENU_F1_E4_D1,TWO_MENU_F1_E4,Menu_F1_E4_D1},  
    {THREE_MENU_F1_E4_D2,  THREE_MENU_F1_E4_D3, THREE_MENU_F1_E4_D2, THREE_MENU_F1_E4_D2,TWO_MENU_F1_E4,Menu_F1_E4_D2},
    {THREE_MENU_F1_E4_D3,  THREE_MENU_F1_E4_D4, THREE_MENU_F1_E4_D3, THREE_MENU_F1_E4_D3,TWO_MENU_F1_E4,Menu_F1_E4_D3},
    {THREE_MENU_F1_E4_D4,  THREE_MENU_F1_E4_D1, THREE_MENU_F1_E4_D4, THREE_MENU_F1_E4_D4,TWO_MENU_F1_E4,Menu_F1_E4_D4},

    {THREE_MENU_F2_E1_D1,  THREE_MENU_F2_E1_D2, THREE_MENU_F2_E1_D1, THREE_MENU_F2_E1_D1,TWO_MENU_F2_E1,Menu_F2_E1_D1},  
    {THREE_MENU_F2_E1_D2,  THREE_MENU_F2_E1_D3, THREE_MENU_F2_E1_D2, THREE_MENU_F2_E1_D2,TWO_MENU_F2_E1,Menu_F2_E1_D2},
    {THREE_MENU_F2_E1_D3,  THREE_MENU_F2_E1_D4, THREE_MENU_F2_E1_D3, THREE_MENU_F2_E1_D3,TWO_MENU_F2_E1,Menu_F2_E1_D3},
    {THREE_MENU_F2_E1_D4,  THREE_MENU_F2_E1_D1, THREE_MENU_F2_E1_D4, THREE_MENU_F2_E1_D4,TWO_MENU_F2_E1,Menu_F2_E1_D4},

    {THREE_MENU_F2_E2_D1,  THREE_MENU_F2_E2_D2, THREE_MENU_F2_E2_D1, THREE_MENU_F2_E2_D1,TWO_MENU_F2_E2,Menu_F2_E2_D1},  
    {THREE_MENU_F2_E2_D2,  THREE_MENU_F2_E2_D3, THREE_MENU_F2_E2_D2, THREE_MENU_F2_E2_D2,TWO_MENU_F2_E2,Menu_F2_E2_D2},
    {THREE_MENU_F2_E2_D3,  THREE_MENU_F2_E2_D4, THREE_MENU_F2_E2_D3, THREE_MENU_F2_E2_D3,TWO_MENU_F2_E2,Menu_F2_E2_D3},
    {THREE_MENU_F2_E2_D4,  THREE_MENU_F2_E2_D1, THREE_MENU_F2_E2_D4, THREE_MENU_F2_E2_D4,TWO_MENU_F2_E2,Menu_F2_E2_D4},

    {THREE_MENU_F2_E3_D1,  THREE_MENU_F2_E3_D2, THREE_MENU_F2_E3_D1, THREE_MENU_F2_E3_D1,TWO_MENU_F2_E3,Menu_F2_E3_D1},  
    {THREE_MENU_F2_E3_D2,  THREE_MENU_F2_E3_D3, THREE_MENU_F2_E3_D2, THREE_MENU_F2_E3_D2,TWO_MENU_F2_E3,Menu_F2_E3_D2},
    {THREE_MENU_F2_E3_D3,  THREE_MENU_F2_E3_D4, THREE_MENU_F2_E3_D3, THREE_MENU_F2_E3_D3,TWO_MENU_F2_E3,Menu_F2_E3_D3},
    {THREE_MENU_F2_E3_D4,  THREE_MENU_F2_E3_D1, THREE_MENU_F2_E3_D4, THREE_MENU_F2_E3_D4,TWO_MENU_F2_E3,Menu_F2_E3_D4},

    {THREE_MENU_F2_E4_D1,  THREE_MENU_F2_E4_D2, THREE_MENU_F2_E4_D1, THREE_MENU_F2_E4_D1,TWO_MENU_F2_E4,Menu_F2_E4_D1},  
    {THREE_MENU_F2_E4_D2,  THREE_MENU_F2_E4_D3, THREE_MENU_F2_E4_D2, THREE_MENU_F2_E4_D2,TWO_MENU_F2_E4,Menu_F2_E4_D2},
    {THREE_MENU_F2_E4_D3,  THREE_MENU_F2_E4_D4, THREE_MENU_F2_E4_D3, THREE_MENU_F2_E4_D3,TWO_MENU_F2_E4,Menu_F2_E4_D3},
    {THREE_MENU_F2_E4_D4,  THREE_MENU_F2_E4_D1, THREE_MENU_F2_E4_D4, THREE_MENU_F2_E4_D4,TWO_MENU_F2_E4,Menu_F2_E4_D4},

    {THREE_MENU_F3_E1_SET,  THREE_MENU_F3_E1_SET, THREE_MENU_F3_E1_SET, THREE_MENU_F3_E1_SET,TWO_MENU_F3_E1,Menu_F3_E1_Set},    
    {THREE_MENU_F3_E2_SET,  THREE_MENU_F3_E2_SET, THREE_MENU_F3_E2_SET, THREE_MENU_F3_E2_SET,TWO_MENU_F3_E2,Menu_F3_E2_Set},    

    {THREE_MENU_F6_E1_SET,  THREE_MENU_F6_E1_SET, THREE_MENU_F6_E1_SET, THREE_MENU_F6_E1_SET,TWO_MENU_F6_E1,Menu_F6_E1_Set},      
    {THREE_MENU_F6_E2_SET,  THREE_MENU_F6_E2_SET, THREE_MENU_F6_E2_SET, THREE_MENU_F6_E2_SET,TWO_MENU_F6_E2,Menu_F6_E2_Set},    
    {THREE_MENU_F6_E3_SET,  THREE_MENU_F6_E3_SET, THREE_MENU_F6_E3_SET, THREE_MENU_F6_E3_SET,TWO_MENU_F6_E3,Menu_F6_E3_Set},    
    {THREE_MENU_F6_E4_SET,  THREE_MENU_F6_E4_SET, THREE_MENU_F6_E4_SET, THREE_MENU_F6_E4_SET,TWO_MENU_F6_E4,Menu_F6_E4_Set},    
    {THREE_MENU_F6_E5_SET,  THREE_MENU_F6_E5_SET, THREE_MENU_F6_E5_SET, THREE_MENU_F6_E5_SET,TWO_MENU_F6_E5,Menu_F6_E5_Set},    
    {THREE_MENU_F6_E6_SET,  THREE_MENU_F6_E6_SET, THREE_MENU_F6_E6_SET, THREE_MENU_F6_E6_SET,TWO_MENU_F6_E6,Menu_F6_E6_Set},    
    {THREE_MENU_F6_E7_SET,  THREE_MENU_F6_E7_SET, THREE_MENU_F6_E7_SET, THREE_MENU_F6_E7_SET,TWO_MENU_F6_E7,Menu_F6_E7_Set},    
    {THREE_MENU_F6_E8_SET,  THREE_MENU_F6_E8_SET, THREE_MENU_F6_E8_SET, THREE_MENU_F6_E8_SET,TWO_MENU_F6_E8,Menu_F6_E8_Set},    

    {THREE_MENU_F7_E1_SET,  THREE_MENU_F7_E1_SET, THREE_MENU_F7_E1_SET, THREE_MENU_F7_E1_SET,TWO_MENU_F7_E1,Menu_F7_E1_Set},      
    {THREE_MENU_F7_E2_SET,  THREE_MENU_F7_E2_SET, THREE_MENU_F7_E2_SET, THREE_MENU_F7_E2_SET,TWO_MENU_F7_E2,Menu_F7_E2_Set},    
    {THREE_MENU_F7_E3_SET,  THREE_MENU_F7_E3_SET, THREE_MENU_F7_E3_SET, THREE_MENU_F7_E3_SET,TWO_MENU_F7_E3,Menu_F7_E3_Set},    
    {THREE_MENU_F7_E4_SET,  THREE_MENU_F7_E4_SET, THREE_MENU_F7_E4_SET, THREE_MENU_F7_E4_SET,TWO_MENU_F7_E4,Menu_F7_E4_Set},   

    {THREE_MENU_F8_E1_SET,  THREE_MENU_F8_E1_SET, THREE_MENU_F8_E1_SET, THREE_MENU_F8_E1_SET,TWO_MENU_F8_E1,Menu_F8_E1_Set},      
    {THREE_MENU_F8_E2_SET,  THREE_MENU_F8_E2_SET, THREE_MENU_F8_E2_SET, THREE_MENU_F8_E2_SET,TWO_MENU_F8_E2,Menu_F8_E2_Set},    

    {THREE_MENU_FE_E1_SET,  THREE_MENU_FE_E1_SET, THREE_MENU_FE_E1_SET, THREE_MENU_FE_E1_SET,TWO_MENU_FE_E1,Menu_F0},      
    {THREE_MENU_FE_E2_SET,  THREE_MENU_FE_E2_SET, THREE_MENU_FE_E2_SET, THREE_MENU_FE_E2_SET,TWO_MENU_FE_E2,Menu_F0},    

    {DECODER_MENU,  DECODER_MENU, DECODER_MENU, DECODER_MENU,DECODER_MENU,Menu_Decoder},    

};

/**
  * @brief  This function is change Str to int.
  * @param  str
  * @retval int
  */

u16 Str_To_Int(u8 *str)
{
    u16 tmp=0; 
//     tmp=(str[0]-'0')*1000;
    tmp+=(str[1])*100;
    tmp+=(str[2])*10;
    tmp+=(str[3]);
    return tmp;
}

/**
  * @brief  This function is change Str to int.
  * @param  str
  * @retval int
  */

void Int_To_Str(u16 dat,u8 *str)
{
    u16 tmp;
    if(dat>15999) dat=0;
    str[0]=dat/1000;
    tmp=dat%1000;
    str[1]=tmp/100;
    tmp=tmp%100;
    str[2]=tmp/10;
    str[3]=tmp%10;
    str[4]=0;
}

/**
  * @brief  This function is add number of buff add one.
  * @param  str
  * @retval None
  */

void Buff_Add_One(u8 * str)
{
    if (str[3] == 9)
    {
        if (str[2] == 9)
        {
            if (str[1] == 9)
            {
                str[0]++;
                str[1] = 0;
                str[2] = 0;
                if (str[0] == 21)
                {
                    str[0] = 0;
                    str[3] = 1;
                }
                else
                {
                    str[3] = 1;
                }
            }
            else
            {
                str[1]++;
                str[2] = 0;
                str[3] = 0;
            }
        }
        else
        {
            str[2]++;
            str[3] = 0;
        }
    }
    else
    {
        str[3]++;
    }
}

/**
  * @brief  This function is Show TM1629 of Standby menu.
  * @param  None
  * @retval None
  */

void Menu_Standby(void)
{
    static u8 Standby_index=0;
    if(Decoder_Call_Save_Queue[0] == 0 )
    {
        Tm1629_Clear();
        Tm1629_Display_Ram[0][Standby_index&0x03]=0x40; /* - */
        Tm1629_Display();
        Standby_index++;
    }
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        Decoder_Function_Of_Down();
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        Decoder_Function_Of_Up();
    }
    else if(gKeyValue == KEY_VALUE_ESC)
    {
        Decoder_Function_Of_Esc();
    }
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

/**
  * @brief  This function is Show sub menu of F6. E1
  * @param  None
  * @retval None
  */
  
void Menu_F6_E1(void)
{
    Tm1629_Show_Ex(0x01);
}

/**
  * @brief  This function is Show sub menu of F6. E2
  * @param  None
  * @retval None
  */
  
void Menu_F6_E2(void)
{
    Tm1629_Show_Ex(0x02);
}

/**
  * @brief  This function is Show sub menu of F6. E3
  * @param  None
  * @retval None
  */
  
void Menu_F6_E3(void)
{
    Tm1629_Show_Ex(0x03);
}

/**
  * @brief  This function is Show sub menu of F6. E4
  * @param  None
  * @retval None
  */
  
void Menu_F6_E4(void)
{
    Tm1629_Show_Ex(0x04);
}


/**
  * @brief  This function is Show sub menu of F6. E5
  * @param  None
  * @retval None
  */
  
void Menu_F6_E5(void)
{
    Tm1629_Show_Ex(0x05);
}

/**
  * @brief  This function is Show sub menu of F6. E6
  * @param  None
  * @retval None
  */
  
void Menu_F6_E6(void)
{
    Tm1629_Show_Ex(0x06);
}

/**
  * @brief  This function is Show sub menu of F6. E7
  * @param  None
  * @retval None
  */
  
void Menu_F6_E7(void)
{
    Tm1629_Show_Ex(0x07);
}

/**
  * @brief  This function is Show sub menu of F6. E8
  * @param  None
  * @retval None
  */
  
void Menu_F6_E8(void)
{
    Tm1629_Show_Ex(0x08);
}

/**
  * @brief  This function is Show sub menu of F7. E1
  * @param  None
  * @retval None
  */
  
void Menu_F7_E1(void)
{
    Tm1629_Show_Ex(0x01);
}

/**
  * @brief  This function is Show sub menu of F7. E2
  * @param  None
  * @retval None
  */
  
void Menu_F7_E2(void)
{
    Tm1629_Show_Ex(0x02);
}

/**
  * @brief  This function is Show sub menu of F7. E3
  * @param  None
  * @retval None
  */
  
void Menu_F7_E3(void)
{
    Tm1629_Show_Ex(0x03);
}

/**
  * @brief  This function is Show sub menu of F7. E4
  * @param  None
  * @retval None
  */
  
void Menu_F7_E4(void)
{
    Tm1629_Show_Ex(0x04);
}

/**
  * @brief  This function is Show sub menu of F8. E1
  * @param  None
  * @retval None
  */
  
void Menu_F8_E1(void)
{
    Tm1629_Show_Ex(0x01);
}

/**
  * @brief  This function is Show sub menu of F8. E2
  * @param  None
  * @retval None
  */
  
void Menu_F8_E2(void)
{
    Tm1629_Show_Ex(0x02);
}

/**
  * @brief  This function is Show sub menu of F9. E1
  * @param  None
  * @retval None
  */
  
void Menu_F9_E1(void)
{
    Tm1629_Show_Ex(0x01);
}

/**
  * @brief  This function is Show sub menu of F9. E2
  * @param  None
  * @retval None
  */
  
void Menu_F9_E2(void)
{
    Tm1629_Show_Ex(0x02);
}

/**
  * @brief  This function is Show menu of FA.
  * @param  None
  * @retval None
  */
  
void Menu_FA_Set(void)
{
    ;
}

/**
  * @brief  This function is Show menu of FB.
  * @param  None
  * @retval None
  */
  
void Menu_FB_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Key_Remove_Call_Is_Or_Not == 1)
            Set_Key_Remove_Call_Is_Or_Not = 0;
        else
            Set_Key_Remove_Call_Is_Or_Not = 1;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Key_Remove_Call_Is_Or_Not == 1)
            Set_Key_Remove_Call_Is_Or_Not = 0;
        else
            Set_Key_Remove_Call_Is_Or_Not = 1;
    }
    Tm1629_Show_One_Number(Set_Key_Remove_Call_Is_Or_Not); 
}

/**
  * @brief  This function is Show menu of FC.
  * @param  None
  * @retval None
  */
  
void Menu_FC_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Change_Standby_Display_Mode== 1)
            Change_Standby_Display_Mode = 0;
        else
            Change_Standby_Display_Mode = 1;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Change_Standby_Display_Mode == 1)
            Change_Standby_Display_Mode = 0;
        else
            Change_Standby_Display_Mode = 1;
    }
    Tm1629_Show_One_Number(Change_Standby_Display_Mode); 
}

/**
  * @brief  This function is Show menu of FD.
  * @param  None
  * @retval None
  */
  
void Menu_FD_Set(void)
{
    ;
}

/**
  * @brief  This function is Show sub menu of FE. E1
  * @param  None
  * @retval None
  */
  
void Menu_FE_E1(void)
{
    Tm1629_Show_Ex(0x01);
}

/**
  * @brief  This function is Show sub menu of FE. E2
  * @param  None
  * @retval None
  */
  
void Menu_FE_E2(void)
{
    Tm1629_Show_Ex(0x02);
}

/**
  * @brief  This function is Show sub menu of F1. E1. D1
  * @param  None
  * @retval None
  */
  
void  Menu_F1_E1_D1(void)
{
    Tm1629_Blink_One_Position(Register_Call_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Register_Call_Buff[3] == 9)
            Register_Call_Buff[3]=0;
        else
            Register_Call_Buff[3]++;
        Tm1629_Show_Four_Number(Register_Call_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Register_Call_Buff[3] == 0)
            Register_Call_Buff[3]=9;
        else
            Register_Call_Buff[3]--;
        Tm1629_Show_Four_Number(Register_Call_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F1. E1. D2
  * @param  None
  * @retval None
  */
  
void  Menu_F1_E1_D2(void)
{
    Tm1629_Blink_Two_Position(Register_Call_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Register_Call_Buff[2] == 9)
            Register_Call_Buff[2]=0;
        else
            Register_Call_Buff[2]++;
        Tm1629_Show_Four_Number(Register_Call_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Register_Call_Buff[2] == 0)
            Register_Call_Buff[2]=9;
        else
            Register_Call_Buff[2]--;
        Tm1629_Show_Four_Number(Register_Call_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F1. E1. D3
  * @param  None
  * @retval None
  */
  
void  Menu_F1_E1_D3(void)
{
    Tm1629_Blink_Three_Position(Register_Call_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Register_Call_Buff[1] == 9)
            Register_Call_Buff[1]=0;
        else
            Register_Call_Buff[1]++;
        Tm1629_Show_Four_Number(Register_Call_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Register_Call_Buff[1] == 0)
            Register_Call_Buff[1]=9;
        else
            Register_Call_Buff[1]--;
        Tm1629_Show_Four_Number(Register_Call_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F1. E1. D4
  * @param  None
  * @retval None
  */
  
void  Menu_F1_E1_D4(void)
{
    Tm1629_Blink_Four_Position(Register_Call_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Register_Call_Buff[0] == 9)
            Register_Call_Buff[0]=0;
        else
            Register_Call_Buff[0]++;
        Tm1629_Show_Four_Number(Register_Call_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Register_Call_Buff[0] == 0)
            Register_Call_Buff[0]=9;
        else
            Register_Call_Buff[0]--;
        Tm1629_Show_Four_Number(Register_Call_Buff);
    }
}


/**
  * @brief  This function is Show sub menu of F1. E2. D1
  * @param  None
  * @retval None
  */
  
void  Menu_F1_E2_D1(void)
{
    Tm1629_Blink_One_Position(Register_Host_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Register_Host_Buff[3] == 9)
            Register_Host_Buff[3]=0;
        else
            Register_Host_Buff[3]++;
        Tm1629_Show_Four_Number(Register_Host_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Register_Host_Buff[3] == 0)
            Register_Host_Buff[3]=9;
        else
            Register_Host_Buff[3]--;
        Tm1629_Show_Four_Number(Register_Host_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F1. E2. D2
  * @param  None
  * @retval None
  */
  
void  Menu_F1_E2_D2(void)
{
    Tm1629_Blink_Two_Position(Register_Host_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Register_Host_Buff[2] == 9)
            Register_Host_Buff[2]=0;
        else
            Register_Host_Buff[2]++;
        Tm1629_Show_Four_Number(Register_Host_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Register_Host_Buff[2] == 0)
            Register_Host_Buff[2]=9;
        else
            Register_Host_Buff[2]--;
        Tm1629_Show_Four_Number(Register_Host_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F1. E2. D3
  * @param  None
  * @retval None
  */
  
void  Menu_F1_E2_D3(void)
{
    Tm1629_Blink_Three_Position(Register_Host_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Register_Host_Buff[1] == 9)
            Register_Host_Buff[1]=0;
        else
            Register_Host_Buff[1]++;
        Tm1629_Show_Four_Number(Register_Host_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Register_Host_Buff[1] == 0)
            Register_Host_Buff[1]=9;
        else
            Register_Host_Buff[1]--;
        Tm1629_Show_Four_Number(Register_Host_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F1. E2. D4
  * @param  None
  * @retval None
  */
  
void  Menu_F1_E2_D4(void)
{
    Tm1629_Blink_Four_Position(Register_Host_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Register_Host_Buff[0] == 9)
            Register_Host_Buff[0]=0;
        else
            Register_Host_Buff[0]++;
        Tm1629_Show_Four_Number(Register_Host_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Register_Host_Buff[0] == 0)
            Register_Host_Buff[0]=9;
        else
            Register_Host_Buff[0]--;
        Tm1629_Show_Four_Number(Register_Host_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F1. E3. D1
  * @param  None
  * @retval None
  */
  
void  Menu_F1_E3_D1(void)
{
    Tm1629_Blink_One_Position(Register_Alarm_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Register_Alarm_Buff[3] == 9)
            Register_Alarm_Buff[3]=0;
        else
            Register_Alarm_Buff[3]++;
        Tm1629_Show_Four_Number(Register_Alarm_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Register_Alarm_Buff[3] == 0)
            Register_Alarm_Buff[3]=9;
        else
            Register_Alarm_Buff[3]--;
        Tm1629_Show_Four_Number(Register_Alarm_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F1. E3. D2
  * @param  None
  * @retval None
  */
  
void  Menu_F1_E3_D2(void)
{
    Tm1629_Blink_Two_Position(Register_Alarm_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Register_Alarm_Buff[2] == 9)
            Register_Alarm_Buff[2]=0;
        else
            Register_Alarm_Buff[2]++;
        Tm1629_Show_Four_Number(Register_Alarm_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Register_Alarm_Buff[2] == 0)
            Register_Alarm_Buff[2]=9;
        else
            Register_Alarm_Buff[2]--;
        Tm1629_Show_Four_Number(Register_Alarm_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F1. E3. D3
  * @param  None
  * @retval None
  */
  
void  Menu_F1_E3_D3(void)
{
    Tm1629_Blink_Three_Position(Register_Alarm_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Register_Alarm_Buff[1] == 9)
            Register_Alarm_Buff[1]=0;
        else
            Register_Alarm_Buff[1]++;
        Tm1629_Show_Four_Number(Register_Alarm_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Register_Alarm_Buff[1] == 0)
            Register_Alarm_Buff[1]=9;
        else
            Register_Alarm_Buff[1]--;
        Tm1629_Show_Four_Number(Register_Alarm_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F1. E3. D4
  * @param  None
  * @retval None
  */
  
void  Menu_F1_E3_D4(void)
{
    Tm1629_Blink_Four_Position(Register_Alarm_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Register_Alarm_Buff[0] == 9)
            Register_Alarm_Buff[0]=0;
        else
            Register_Alarm_Buff[0]++;
        Tm1629_Show_Four_Number(Register_Alarm_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Register_Alarm_Buff[0] == 0)
            Register_Alarm_Buff[0]=9;
        else
            Register_Alarm_Buff[0]--;
        Tm1629_Show_Four_Number(Register_Alarm_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F1. E4. D1
  * @param  None
  * @retval None
  */
  
void  Menu_F1_E4_D1(void)
{
    Tm1629_Blink_One_Position(Register_Cancel_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Register_Cancel_Buff[3] == 9)
            Register_Cancel_Buff[3]=0;
        else
            Register_Cancel_Buff[3]++;
        Tm1629_Show_Four_Number(Register_Cancel_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Register_Cancel_Buff[3] == 0)
            Register_Cancel_Buff[3]=9;
        else
            Register_Cancel_Buff[3]--;
        Tm1629_Show_Four_Number(Register_Cancel_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F1. E4. D2
  * @param  None
  * @retval None
  */
  
void  Menu_F1_E4_D2(void)
{
    Tm1629_Blink_Two_Position(Register_Cancel_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Register_Cancel_Buff[2] == 9)
            Register_Cancel_Buff[2]=0;
        else
            Register_Cancel_Buff[2]++;
        Tm1629_Show_Four_Number(Register_Cancel_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Register_Cancel_Buff[2] == 0)
            Register_Cancel_Buff[2]=9;
        else
            Register_Cancel_Buff[2]--;
        Tm1629_Show_Four_Number(Register_Cancel_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F1. E4. D3
  * @param  None
  * @retval None
  */
  
void  Menu_F1_E4_D3(void)
{
    Tm1629_Blink_Three_Position(Register_Cancel_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Register_Cancel_Buff[1] == 9)
            Register_Cancel_Buff[1]=0;
        else
            Register_Cancel_Buff[1]++;
        Tm1629_Show_Four_Number(Register_Cancel_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Register_Cancel_Buff[1] == 0)
            Register_Cancel_Buff[1]=9;
        else
            Register_Cancel_Buff[1]--;
        Tm1629_Show_Four_Number(Register_Cancel_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F1. E4. D4
  * @param  None
  * @retval None
  */
  
void  Menu_F1_E4_D4(void)
{
    Tm1629_Blink_Four_Position(Register_Cancel_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Register_Cancel_Buff[0] == 9)
            Register_Cancel_Buff[0]=0;
        else
            Register_Cancel_Buff[0]++;
        Tm1629_Show_Four_Number(Register_Cancel_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Register_Cancel_Buff[0] == 0)
            Register_Cancel_Buff[0]=9;
        else
            Register_Cancel_Buff[0]--;
        Tm1629_Show_Four_Number(Register_Cancel_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F2. E1. D1
  * @param  None
  * @retval None
  */
  
void  Menu_F2_E1_D1(void)
{
    Tm1629_Blink_One_Position(Delete_Call_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Delete_Call_Buff[3] == 9)
            Delete_Call_Buff[3]=0;
        else
            Delete_Call_Buff[3]++;
        Tm1629_Show_Four_Number(Delete_Call_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Delete_Call_Buff[3] == 0)
            Delete_Call_Buff[3]=9;
        else
            Delete_Call_Buff[3]--;
        Tm1629_Show_Four_Number(Delete_Call_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F2. E1. D2
  * @param  None
  * @retval None
  */
  
void  Menu_F2_E1_D2(void)
{
    Tm1629_Blink_Two_Position(Delete_Call_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Delete_Call_Buff[2] == 9)
            Delete_Call_Buff[2]=0;
        else
            Delete_Call_Buff[2]++;
        Tm1629_Show_Four_Number(Delete_Call_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Delete_Call_Buff[2] == 0)
            Delete_Call_Buff[2]=9;
        else
            Delete_Call_Buff[2]--;
        Tm1629_Show_Four_Number(Delete_Call_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F2. E1. D3
  * @param  None
  * @retval None
  */
  
void  Menu_F2_E1_D3(void)
{
    Tm1629_Blink_Three_Position(Delete_Call_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Delete_Call_Buff[1] == 9)
            Delete_Call_Buff[1]=0;
        else
            Delete_Call_Buff[1]++;
        Tm1629_Show_Four_Number(Delete_Call_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Delete_Call_Buff[1] == 0)
            Delete_Call_Buff[1]=9;
        else
            Delete_Call_Buff[1]--;
        Tm1629_Show_Four_Number(Delete_Call_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F2. E1. D4
  * @param  None
  * @retval None
  */
  
void  Menu_F2_E1_D4(void)
{
    Tm1629_Blink_Four_Position(Delete_Call_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Delete_Call_Buff[0] == 9)
            Delete_Call_Buff[0]=0;
        else
            Delete_Call_Buff[0]++;
        Tm1629_Show_Four_Number(Delete_Call_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Delete_Call_Buff[0] == 0)
            Delete_Call_Buff[0]=9;
        else
            Delete_Call_Buff[0]--;
        Tm1629_Show_Four_Number(Delete_Call_Buff);
    }
}



/**
  * @brief  This function is Show sub menu of F2. E2. D1
  * @param  None
  * @retval None
  */
  
void  Menu_F2_E2_D1(void)
{
    Tm1629_Blink_One_Position(Delete_Host_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Delete_Host_Buff[3] == 9)
            Delete_Host_Buff[3]=0;
        else
            Delete_Host_Buff[3]++;
        Tm1629_Show_Four_Number(Delete_Host_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Delete_Host_Buff[3] == 0)
            Delete_Host_Buff[3]=9;
        else
            Delete_Host_Buff[3]--;
        Tm1629_Show_Four_Number(Delete_Host_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F2. E2. D2
  * @param  None
  * @retval None
  */
  
void  Menu_F2_E2_D2(void)
{
    Tm1629_Blink_Two_Position(Delete_Host_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Delete_Host_Buff[2] == 9)
            Delete_Host_Buff[2]=0;
        else
            Delete_Host_Buff[2]++;
        Tm1629_Show_Four_Number(Delete_Host_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Delete_Host_Buff[2] == 0)
            Delete_Host_Buff[2]=9;
        else
            Delete_Host_Buff[2]--;
        Tm1629_Show_Four_Number(Delete_Host_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F2. E2. D3
  * @param  None
  * @retval None
  */
  
void  Menu_F2_E2_D3(void)
{
    Tm1629_Blink_Three_Position(Delete_Host_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Delete_Host_Buff[1] == 9)
            Delete_Host_Buff[1]=0;
        else
            Delete_Host_Buff[1]++;
        Tm1629_Show_Four_Number(Delete_Host_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Delete_Host_Buff[1] == 0)
            Delete_Host_Buff[1]=9;
        else
            Delete_Host_Buff[1]--;
        Tm1629_Show_Four_Number(Delete_Host_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F2. E2. D4
  * @param  None
  * @retval None
  */
  
void  Menu_F2_E2_D4(void)
{
    Tm1629_Blink_Four_Position(Delete_Host_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Delete_Host_Buff[0] == 9)
            Delete_Host_Buff[0]=0;
        else
            Delete_Host_Buff[0]++;
        Tm1629_Show_Four_Number(Delete_Host_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Delete_Host_Buff[0] == 0)
            Delete_Host_Buff[0]=9;
        else
            Delete_Host_Buff[0]--;
        Tm1629_Show_Four_Number(Delete_Host_Buff);
    }
}


/**
  * @brief  This function is Show sub menu of F2. E3. D1
  * @param  None
  * @retval None
  */
  
void  Menu_F2_E3_D1(void)
{
    Tm1629_Blink_One_Position(Delete_Alarm_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Delete_Alarm_Buff[3] == 9)
            Delete_Alarm_Buff[3]=0;
        else
            Delete_Alarm_Buff[3]++;
        Tm1629_Show_Four_Number(Delete_Alarm_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Delete_Alarm_Buff[3] == 0)
            Delete_Alarm_Buff[3]=9;
        else
            Delete_Alarm_Buff[3]--;
        Tm1629_Show_Four_Number(Delete_Alarm_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F2. E3. D2
  * @param  None
  * @retval None
  */
  
void  Menu_F2_E3_D2(void)
{
    Tm1629_Blink_Two_Position(Delete_Alarm_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Delete_Alarm_Buff[2] == 9)
            Delete_Alarm_Buff[2]=0;
        else
            Delete_Alarm_Buff[2]++;
        Tm1629_Show_Four_Number(Delete_Alarm_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Delete_Alarm_Buff[2] == 0)
            Delete_Alarm_Buff[2]=9;
        else
            Delete_Alarm_Buff[2]--;
        Tm1629_Show_Four_Number(Delete_Alarm_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F2. E3. D3
  * @param  None
  * @retval None
  */
  
void  Menu_F2_E3_D3(void)
{
    Tm1629_Blink_Three_Position(Delete_Alarm_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Delete_Alarm_Buff[1] == 9)
            Delete_Alarm_Buff[1]=0;
        else
            Delete_Alarm_Buff[1]++;
        Tm1629_Show_Four_Number(Delete_Alarm_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Delete_Alarm_Buff[1] == 0)
            Delete_Alarm_Buff[1]=9;
        else
            Delete_Alarm_Buff[1]--;
        Tm1629_Show_Four_Number(Delete_Alarm_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F2. E3. D4
  * @param  None
  * @retval None
  */
  
void  Menu_F2_E3_D4(void)
{
    Tm1629_Blink_Four_Position(Delete_Alarm_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Delete_Alarm_Buff[0] == 9)
            Delete_Alarm_Buff[0]=0;
        else
            Delete_Alarm_Buff[0]++;
        Tm1629_Show_Four_Number(Delete_Alarm_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Delete_Alarm_Buff[0] == 0)
            Delete_Alarm_Buff[0]=9;
        else
            Delete_Alarm_Buff[0]--;
        Tm1629_Show_Four_Number(Delete_Alarm_Buff);
    }
}


/**
  * @brief  This function is Show sub menu of F2. E4. D1
  * @param  None
  * @retval None
  */
  
void  Menu_F2_E4_D1(void)
{
    Tm1629_Blink_One_Position(Delete_Cancel_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Delete_Cancel_Buff[3] == 9)
            Delete_Cancel_Buff[3]=0;
        else
            Delete_Cancel_Buff[3]++;
        Tm1629_Show_Four_Number(Delete_Cancel_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Delete_Cancel_Buff[3] == 0)
            Delete_Cancel_Buff[3]=9;
        else
            Delete_Cancel_Buff[3]--;
        Tm1629_Show_Four_Number(Delete_Cancel_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F2. E4. D2
  * @param  None
  * @retval None
  */
  
void  Menu_F2_E4_D2(void)
{
    Tm1629_Blink_Two_Position(Delete_Cancel_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Delete_Cancel_Buff[2] == 9)
            Delete_Cancel_Buff[2]=0;
        else
            Delete_Cancel_Buff[2]++;
        Tm1629_Show_Four_Number(Delete_Cancel_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Delete_Cancel_Buff[2] == 0)
            Delete_Cancel_Buff[2]=9;
        else
            Delete_Cancel_Buff[2]--;
        Tm1629_Show_Four_Number(Delete_Cancel_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F2. E4. D3
  * @param  None
  * @retval None
  */
  
void  Menu_F2_E4_D3(void)
{
    Tm1629_Blink_Three_Position(Delete_Cancel_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Delete_Cancel_Buff[1] == 9)
            Delete_Cancel_Buff[1]=0;
        else
            Delete_Cancel_Buff[1]++;
        Tm1629_Show_Four_Number(Delete_Cancel_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Delete_Cancel_Buff[1] == 0)
            Delete_Cancel_Buff[1]=9;
        else
            Delete_Cancel_Buff[1]--;
        Tm1629_Show_Four_Number(Delete_Cancel_Buff);
    }
}

/**
  * @brief  This function is Show sub menu of F2. E4. D4
  * @param  None
  * @retval None
  */
  
void  Menu_F2_E4_D4(void)
{
    Tm1629_Blink_Four_Position(Delete_Cancel_Buff);
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Delete_Cancel_Buff[0] == 9)
            Delete_Cancel_Buff[0]=0;
        else
            Delete_Cancel_Buff[0]++;
        Tm1629_Show_Four_Number(Delete_Cancel_Buff);
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Delete_Cancel_Buff[0] == 0)
            Delete_Cancel_Buff[0]=9;
        else
            Delete_Cancel_Buff[0]--;
        Tm1629_Show_Four_Number(Delete_Cancel_Buff);
    }
}

/**
  * @brief  This function is Show menu of F3 E1 SET.
  * @param  None
  * @retval None
  */
  
void Menu_F3_E1_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Call_Queue_Mode== 1)
            Set_Call_Queue_Mode = 0;
        else
            Set_Call_Queue_Mode = 1;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Call_Queue_Mode == 1)
            Set_Call_Queue_Mode = 0;
        else
            Set_Call_Queue_Mode = 1;
    }
    Tm1629_Show_One_Number(Set_Call_Queue_Mode); 
}

/**
  * @brief  This function is Show menu of F3 E2 SET
  * @param  None
  * @retval None
  */
  
void Menu_F3_E2_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Call_Display_Number== 99)
            Set_Call_Display_Number=0;
        else
            Set_Call_Display_Number++;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Call_Display_Number == 0)
            Set_Call_Display_Number=99;
        else
            Set_Call_Display_Number--;
    }
    Tm1629_Show_Two_Number(Set_Call_Display_Number); 
}

/**
  * @brief  This function is Show menu of F6 E1 SET
  * @param  None
  * @retval None
  */
  
void Menu_F6_E1_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Voice_Play_Mode== 7)
            Set_Voice_Play_Mode=0;
        else
            Set_Voice_Play_Mode++;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Voice_Play_Mode == 0)
            Set_Voice_Play_Mode=7;
        else
            Set_Voice_Play_Mode--;
    }
    Tm1629_Show_One_Number(Set_Voice_Play_Mode); 
}

/**
  * @brief  This function is Show menu of F6 E2 SET
  * @param  None
  * @retval None
  */
  
void Menu_F6_E2_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Voice_Play_Time== 9)
            Set_Voice_Play_Time=1;
        else
            Set_Voice_Play_Time++;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Voice_Play_Time == 1)
            Set_Voice_Play_Time=9;
        else
            Set_Voice_Play_Time--;
    }
    Tm1629_Show_One_Number(Set_Voice_Play_Time); 
}

/**
  * @brief  This function is Show menu of F6 E3 SET.
  * @param  None
  * @retval None
  */
  
void Menu_F6_E3_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Voice_Play_Is_Or_Not_In_Cycle_Mode== 1)
            Set_Voice_Play_Is_Or_Not_In_Cycle_Mode = 0;
        else
            Set_Voice_Play_Is_Or_Not_In_Cycle_Mode = 1;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Voice_Play_Is_Or_Not_In_Cycle_Mode == 1)
            Set_Voice_Play_Is_Or_Not_In_Cycle_Mode = 0;
        else
            Set_Voice_Play_Is_Or_Not_In_Cycle_Mode = 1;
    }
    Tm1629_Show_One_Number(Set_Voice_Play_Is_Or_Not_In_Cycle_Mode); 
}

/**
  * @brief  This function is Show menu of F6 E4 SET
  * @param  None
  * @retval None
  */
  
void Menu_F6_E4_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Voice_Volume== 9)
            Set_Voice_Volume=1;
        else
            Set_Voice_Volume++;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Voice_Volume == 1)
            Set_Voice_Volume=9;
        else
            Set_Voice_Volume--;
    }
    Tm1629_Show_One_Number(Set_Voice_Volume); 
}

/**
  * @brief  This function is Show menu of F6 E5 SET
  * @param  None
  * @retval None
  */
  
void Menu_F6_E5_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Display_Tube_Brightness== 7)
            Set_Display_Tube_Brightness=1;
        else
            Set_Display_Tube_Brightness++;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Display_Tube_Brightness == 1)
            Set_Display_Tube_Brightness=7;
        else
            Set_Display_Tube_Brightness--;
    }
    Tm1629_Show_One_Number(Set_Display_Tube_Brightness); 
}

/**
  * @brief  This function is Show menu of F6 E6 SET.
  * @param  None
  * @retval None
  */
  
void Menu_F6_E6_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Voice_Navigation_Mode== 1)
            Set_Voice_Navigation_Mode = 0;
        else
            Set_Voice_Navigation_Mode = 1;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Voice_Navigation_Mode == 1)
            Set_Voice_Navigation_Mode = 0;
        else
            Set_Voice_Navigation_Mode = 1;
    }
    Tm1629_Show_One_Number(Set_Voice_Navigation_Mode); 
}

/**
  * @brief  This function is Show menu of F6 E7 SET.
  * @param  None
  * @retval None
  */
  
void Menu_F6_E7_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Voice_Navigation_On_Or_OFF== 1)
            Set_Voice_Navigation_On_Or_OFF = 0;
        else
            Set_Voice_Navigation_On_Or_OFF = 1;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Voice_Navigation_On_Or_OFF == 1)
            Set_Voice_Navigation_On_Or_OFF = 0;
        else
            Set_Voice_Navigation_On_Or_OFF = 1;
    }
    Tm1629_Show_One_Number(Set_Voice_Navigation_On_Or_OFF); 
}

/**
  * @brief  This function is Show menu of F6 E8 SET.
  * @param  None
  * @retval None
  */
  
void Menu_F6_E8_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Display_LED_Mode== 1)
            Set_Display_LED_Mode = 0;
        else
            Set_Display_LED_Mode = 1;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Display_LED_Mode == 1)
            Set_Display_LED_Mode = 0;
        else
            Set_Display_LED_Mode = 1;
    }
    Tm1629_Show_One_Number(Set_Display_LED_Mode); 
}

/**
  * @brief  This function is Show menu of F7 E1 SET
  * @param  None
  * @retval None
  */
  
void Menu_F7_E1_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Two_Menu_F7_E1== 11)
            Set_Two_Menu_F7_E1=0;
        else
            Set_Two_Menu_F7_E1++;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Two_Menu_F7_E1 == 0)
            Set_Two_Menu_F7_E1=11;
        else
            Set_Two_Menu_F7_E1--;
    }
    Tm1629_Show_Two_Number(Set_Two_Menu_F7_E1); 
}

/**
  * @brief  This function is Show menu of F7 E2 SET
  * @param  None
  * @retval None
  */
  
void Menu_F7_E2_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Two_Menu_F7_E2== 11)
            Set_Two_Menu_F7_E2=0;
        else
            Set_Two_Menu_F7_E2++;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Two_Menu_F7_E2 == 0)
            Set_Two_Menu_F7_E2=11;
        else
            Set_Two_Menu_F7_E2--;
    }
    Tm1629_Show_Two_Number(Set_Two_Menu_F7_E2); 
}

/**
  * @brief  This function is Show menu of F7 E3 SET
  * @param  None
  * @retval None
  */
  
void Menu_F7_E3_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Two_Menu_F7_E3== 11)
            Set_Two_Menu_F7_E3=0;
        else
            Set_Two_Menu_F7_E3++;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Two_Menu_F7_E3 == 0)
            Set_Two_Menu_F7_E3=11;
        else
            Set_Two_Menu_F7_E3--;
    }
    Tm1629_Show_Two_Number(Set_Two_Menu_F7_E3); 
}

/**
  * @brief  This function is Show menu of F7 E4 SET
  * @param  None
  * @retval None
  */
  
void Menu_F7_E4_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Two_Menu_F7_E4== 11)
            Set_Two_Menu_F7_E4=0;
        else
            Set_Two_Menu_F7_E4++;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Two_Menu_F7_E4== 0)
            Set_Two_Menu_F7_E4=11;
        else
            Set_Two_Menu_F7_E4--;
    }
    Tm1629_Show_Two_Number(Set_Two_Menu_F7_E4); 
}

/**
  * @brief  This function is Show menu of F8 E1 SET.
  * @param  None
  * @retval None
  */
  
void Menu_F8_E1_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Singal_Or_Multiple_Key_Mode== 1)
            Set_Singal_Or_Multiple_Key_Mode = 2;
        else
            Set_Singal_Or_Multiple_Key_Mode = 1;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Singal_Or_Multiple_Key_Mode == 1)
            Set_Singal_Or_Multiple_Key_Mode = 2;
        else
            Set_Singal_Or_Multiple_Key_Mode = 1;
    }
    Tm1629_Show_One_Number(Set_Singal_Or_Multiple_Key_Mode); 
}

/**
  * @brief  This function is Show menu of F8 E2 SET
  * @param  None
  * @retval None
  */
  
void Menu_F8_E2_Set(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        if(Set_Key_Of_Call_Mode== 38)
            Set_Key_Of_Call_Mode=0;
        else
            Set_Key_Of_Call_Mode++;
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        if(Set_Key_Of_Call_Mode== 0)
            Set_Key_Of_Call_Mode=38;
        else
            Set_Key_Of_Call_Mode--;
    }
    Tm1629_Show_Two_Number(Set_Key_Of_Call_Mode); 
}

/**
  * @brief  This function is Show menu of Decoder
  * @param  None
  * @retval None
  */
  
void Menu_Decoder(void)
{
    if(gKeyValue == KEY_VALUE_DOWN)
    {
        Decoder_Function_Of_Down();
    }
    else if(gKeyValue == KEY_VALUE_UP)
    {
        Decoder_Function_Of_Up();
    }
    else if(gKeyValue == KEY_VALUE_ESC)
    {
        Decoder_Function_Of_Esc();
    }
}

#endif /* MENU_GLOBAL */


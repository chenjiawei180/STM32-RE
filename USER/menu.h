 /******************************************************************************
  * @file       USER/menu.h
  * @author  cjw
  * @date     2016.1.28
  * @brief     This file contains the headers of the menu.
  ******************************************************************************/
#ifndef __MENU_H
#define __MENU_H

#include "stm32f10x.h"
#include "global.h"
#include "stm32_rtc.h"

typedef enum
{
    STANDBY_MENU = 0,
    ONE_MENU_F0,
    ONE_MENU_F1,
    ONE_MENU_F2,
    ONE_MENU_F3,
    ONE_MENU_F4,
    ONE_MENU_F5,
    ONE_MENU_F6,
    ONE_MENU_F7,
    ONE_MENU_F8,
    ONE_MENU_F9,
    ONE_MENU_FA,
    ONE_MENU_FB,
    ONE_MENU_FC,
    ONE_MENU_FD,
    ONE_MENU_FE,
    
    TWO_MENU_F0_MINUTE,
    TWO_MENU_F0_HOUR,
    TWO_MENU_F0_DAY,
    TWO_MENU_F0_MONTH,
    TWO_MENU_F0_YEAR,
    TWO_MENU_F0_WEEK,    

    TWO_MENU_F1_E1,
    TWO_MENU_F1_E2,
    TWO_MENU_F1_E3,
    TWO_MENU_F1_E4,

    TWO_MENU_F2_E1,
    TWO_MENU_F2_E2,
    TWO_MENU_F2_E3,
    TWO_MENU_F2_E4,

    TWO_MENU_F3_E1,
    TWO_MENU_F3_E2,

    TWO_MENU_F4_SET,
    TWO_MENU_F5_SET,

    TWO_MENU_F6_E1,
    TWO_MENU_F6_E2,
    TWO_MENU_F6_E3,
    TWO_MENU_F6_E4,
    TWO_MENU_F6_E5,
    TWO_MENU_F6_E6,
    TWO_MENU_F6_E7,
    TWO_MENU_F6_E8,
    
    TWO_MENU_F7_E1,
    TWO_MENU_F7_E2,
    TWO_MENU_F7_E3,
    TWO_MENU_F7_E4,

    TWO_MENU_F8_E1,
    TWO_MENU_F8_E2,

    TWO_MENU_F9_E1,
    TWO_MENU_F9_E2,

    TWO_MENU_FA_SET,
    TWO_MENU_FB_SET,
    TWO_MENU_FC_SET,
    TWO_MENU_FD_SET,
    TWO_MENU_FE_E1,
    TWO_MENU_FE_E2,

    THREE_MENU_F1_E1_D1,
    THREE_MENU_F1_E1_D2,
    THREE_MENU_F1_E1_D3,
    THREE_MENU_F1_E1_D4,

    THREE_MENU_F1_E2_D1,
    THREE_MENU_F1_E2_D2,
    THREE_MENU_F1_E2_D3,
    THREE_MENU_F1_E2_D4,

    THREE_MENU_F1_E3_D1,
    THREE_MENU_F1_E3_D2,
    THREE_MENU_F1_E3_D3,
    THREE_MENU_F1_E3_D4,

    THREE_MENU_F1_E4_D1,
    THREE_MENU_F1_E4_D2,
    THREE_MENU_F1_E4_D3,
    THREE_MENU_F1_E4_D4,

    THREE_MENU_F2_E1_D1,
    THREE_MENU_F2_E1_D2,
    THREE_MENU_F2_E1_D3,
    THREE_MENU_F2_E1_D4,

    THREE_MENU_F2_E2_D1,
    THREE_MENU_F2_E2_D2,
    THREE_MENU_F2_E2_D3,
    THREE_MENU_F2_E2_D4,

    THREE_MENU_F2_E3_D1,
    THREE_MENU_F2_E3_D2,
    THREE_MENU_F2_E3_D3,
    THREE_MENU_F2_E3_D4,

    THREE_MENU_F2_E4_D1,
    THREE_MENU_F2_E4_D2,
    THREE_MENU_F2_E4_D3,
    THREE_MENU_F2_E4_D4,

    THREE_MENU_F3_E1_SET,
    THREE_MENU_F3_E2_SET,

    THREE_MENU_F6_E1_SET,
    THREE_MENU_F6_E2_SET,
    THREE_MENU_F6_E3_SET,
    THREE_MENU_F6_E4_SET,
    THREE_MENU_F6_E5_SET,
    THREE_MENU_F6_E6_SET,
    THREE_MENU_F6_E7_SET,
    THREE_MENU_F6_E8_SET,

    THREE_MENU_F7_E1_SET,
    THREE_MENU_F7_E2_SET,
    THREE_MENU_F7_E3_SET,
    THREE_MENU_F7_E4_SET,

    THREE_MENU_F8_E1_SET,
    THREE_MENU_F8_E2_SET,

    THREE_MENU_FE_E1_SET,
    THREE_MENU_FE_E2_SET,

    DECODER_MENU,

}Menu_index;

struct Menu_def 
{
    Menu_index Index;               //本菜单索引号
    Menu_index Enter;               //按确认确时的索引
    Menu_index Up;                  //按UP键时的索引
    Menu_index Down;                //按DOWN键时的索引
    Menu_index Return;              //要执行的函数
    void (*handle)(void);    //本单的标题
};

extern const struct Menu_def MenuProc[];
extern u8 Register_Call_Buff[5];
extern u8 Delete_Call_Buff[5];
extern u8 Register_Host_Buff[5];
extern u8 Delete_Host_Buff[5];
extern u8 Register_Alarm_Buff[5];
extern u8 Delete_Alarm_Buff[5];
extern u8 Register_Cancel_Buff[5];
extern u8 Delete_Cancel_Buff[5];

extern u8 Remove_Call_Time  ;
extern u8 Cycle_Call_Time ;

extern u8 Change_Standby_Display_Mode ;    /* FC */
extern u8 Set_Key_Remove_Call_Is_Or_Not ;    /* FB */

extern u8 Set_Call_Queue_Mode ;
extern u8 Set_Call_Display_Number ;
extern u8 Set_Voice_Play_Mode ;
extern u8 Set_Voice_Play_Time ;
extern u8 Set_Voice_Play_Is_Or_Not_In_Cycle_Mode ;
extern u8 Set_Voice_Volume ;
extern u8 Set_Display_Tube_Brightness ;
extern u8 Set_Voice_Navigation_Mode ;
extern u8 Set_Voice_Navigation_On_Or_OFF ;
extern u8 Set_Display_LED_Mode ;
extern u8 Set_Two_Menu_F7_E1 ;
extern u8 Set_Two_Menu_F7_E2 ;
extern u8 Set_Two_Menu_F7_E3 ;
extern u8 Set_Two_Menu_F7_E4 ;
extern u8 Set_Singal_Or_Multiple_Key_Mode ;
extern u8 Set_Key_Of_Call_Mode  ;

extern u32 Return_Standby_Time_Count ;

//extern u16 Str_To_Int(u8 *str);
//extern void Int_To_Str(u16 dat,u8 *str);
extern void Buff_Time_To_Struct_Time(u8 *buff,struct rtc_time *tm);
extern void Struct_Time_To_Buff_Time(struct rtc_time *tm,u8 *buff);
extern void Time_Adjust(struct rtc_time *tm);
extern void Buff_Add_One(u8 *str);
static void Play_Navigation_Voice(u8 data);

static void Menu_Standby(void);
static void Menu_F0(void);
static void Menu_F1(void);
static void Menu_F2(void);
static void Menu_F3(void);
static void Menu_F4(void);
static void Menu_F5(void);
static void Menu_F6(void);
static void Menu_F7(void);
static void Menu_F8(void);
static void Menu_F9(void);
static void Menu_FA(void);
static void Menu_FB(void);
static void Menu_FC(void);
static void Menu_FD(void);
static void Menu_FE(void);
static void Menu_F0_Minute(void);
static void Menu_F0_Hour(void);
static void Menu_F0_Day(void);
static void Menu_F0_Month(void);
static void Menu_F0_Year(void);
static void Menu_F0_Week(void);
static void Menu_F1_E1(void);
static void Menu_F1_E2(void);
static void Menu_F1_E3(void);
static void Menu_F1_E4(void);
static void Menu_F2_E1(void);
static void Menu_F2_E2(void);
static void Menu_F2_E3(void);
static void Menu_F2_E4(void);
static void Menu_F3_E1(void);
static void Menu_F3_E2(void);
static void Menu_F4_Set(void);
static void Menu_F5_Set(void);
static void Menu_F6_E1(void);
static void Menu_F6_E2(void);
static void Menu_F6_E3(void);
static void Menu_F6_E4(void);
static void Menu_F6_E5(void);
static void Menu_F6_E6(void);
static void Menu_F6_E7(void);
static void Menu_F6_E8(void);
static void Menu_F7_E1(void);
static void Menu_F7_E2(void);
static void Menu_F7_E3(void);
static void Menu_F7_E4(void);
static void Menu_F8_E1(void);
static void Menu_F8_E2(void);
static void Menu_F9_E1(void);
static void Menu_F9_E2(void);
static void Menu_FA_Set(void);
static void Menu_FB_Set(void);
static void Menu_FC_Set(void);
static void Menu_FD_Set(void);
static void Menu_FE_E1(void);
static void Menu_FE_E2(void);
static void Menu_F1_E1_D1(void);
static void Menu_F1_E1_D2(void);
static void Menu_F1_E1_D3(void);
static void Menu_F1_E1_D4(void);
static void Menu_F1_E2_D1(void);
static void Menu_F1_E2_D2(void);
static void Menu_F1_E2_D3(void);
static void Menu_F1_E2_D4(void);
static void Menu_F1_E3_D1(void);
static void Menu_F1_E3_D2(void);
static void Menu_F1_E3_D3(void);
static void Menu_F1_E3_D4(void);
static void Menu_F1_E4_D1(void);
static void Menu_F1_E4_D2(void);
static void Menu_F1_E4_D3(void);
static void Menu_F1_E4_D4(void);
static void Menu_F2_E1_D1(void);
static void Menu_F2_E1_D2(void);
static void Menu_F2_E1_D3(void);
static void Menu_F2_E1_D4(void);
static void Menu_F2_E2_D1(void);
static void Menu_F2_E2_D2(void);
static void Menu_F2_E2_D3(void);
static void Menu_F2_E2_D4(void);
static void Menu_F2_E3_D1(void);
static void Menu_F2_E3_D2(void);
static void Menu_F2_E3_D3(void);
static void Menu_F2_E3_D4(void);
static void Menu_F2_E4_D1(void);
static void Menu_F2_E4_D2(void);
static void Menu_F2_E4_D3(void);
static void Menu_F2_E4_D4(void);
static void Menu_F3_E1_Set(void);
static void Menu_F3_E2_Set(void);
static void Menu_F6_E1_Set(void);
static void Menu_F6_E2_Set(void);
static void Menu_F6_E3_Set(void);
static void Menu_F6_E4_Set(void);
static void Menu_F6_E5_Set(void);
static void Menu_F6_E6_Set(void);
static void Menu_F6_E7_Set(void);
static void Menu_F6_E8_Set(void);
static void Menu_F7_E1_Set(void);
static void Menu_F7_E2_Set(void);
static void Menu_F7_E3_Set(void);
static void Menu_F7_E4_Set(void);
static void Menu_F8_E1_Set(void);
static void Menu_F8_E2_Set(void);
static void Menu_Decoder(void);

#endif /* __MENU_H */


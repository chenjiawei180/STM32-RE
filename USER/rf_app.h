 /******************************************************************************
  * @file       USER/rf_app.h
  * @author  cjw
  * @date     2016.2.15
  * @brief     This file contains the headers of decoder of RF application.
  ******************************************************************************/
#ifndef __RF_APP_H
#define __RF_APP_H

#include "stm32f10x.h"
#include "global.h"
#include "bitband.h"
#include "eeprom_24c.h"

extern u8 Decoder_Call_Save_Queue[800];
extern unsigned char single_key[16] ;
extern unsigned char multiple_key[16] ;
extern u32 Call_Code_Bak;
extern u32 Call_Off_Time;

extern u8 Remove_Or_Cycle_Time_Count ;
extern u8 Remove_Or_Cycle_Time_Sec_Number ;

extern u8 Host_Enter_Table ;
extern u8 Host_Enter_Fun_Id ;
extern u8 Host_Enter_Times ;

extern void Decoder_Process(void);
static void Decoder_Standby(void);
static void Decoder_F1_E1(void);
static void Decoder_F1_E2(void);
static void Decoder_F1_E3(void);
static void Decoder_F1_E4(void);
static void Decoder_F8_E2(void);
static void Decoder_Decoder_Menu(void);

static void Buff_Move_Up_One_Position(unsigned char * buff);
static void Buff_Move_Down_One_Position(unsigned char * buff);
static void Buff_Move_Up_All_Position_And_Init(unsigned char * buff);
static void Buff_Move_Down_All_Position_And_Init(unsigned char * buff);
static u8 Return_End_Of_Buff(unsigned char * buff);
extern void Left_Buff_Add_To_Head_Of_Right_Buff(unsigned char * left_buff , unsigned char * right_buff);
static void Left_Buff_Add_To_End_Of_Right_Buff(unsigned char * left_buff , unsigned char * right_buff);
static void Decoder_Line_To_Display_Ram_For_Eight_Byte(unsigned char Display_Ram[48], unsigned char * Decoder_Line);
extern void Display_Ram_To_Tm1629(void);
static u8 Decoder_Search_Buff_Is_Or_Not_In_Queue(unsigned char * buff);
static void Decoder_Function_Of_Cancel(unsigned char * buff);
extern void Decoder_Function_Of_Esc(void);
extern void Decoder_Function_Of_Up(void);
extern void Decoder_Function_Of_Down(void);
static void Decoder_Function_Of_Return_Fun_Id(unsigned char * buff,u32 data);
static void Decoder_Function_Of_Assignment_For_KeyBoard(unsigned char * buff,u32 data);
static void Decoder_Function_Of_Assignment_For_Call_Id(unsigned char * buff,u32 data,RF_def RFtmp,u8 state);
static u8 Decoder_Funciton_Of_Is_Or_Not_KeyBoard(u32 data);
extern void Decoder_Function_Of_Remove_Call_Time(void);
extern void Decoder_Function_Of_Cycle_Call_Time(void);

#endif  /* __RF_APP_H */


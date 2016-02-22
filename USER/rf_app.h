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

extern u8 Decoder_Call_Save_Line[800];

extern u32 Call_Code_Bak;
extern u32 Call_Off_Time;

extern void Decoder_Process(void);
static void Decoder_Standby(void);
static void Decoder_F1_E1(void);
static void Decoder_F1_E2(void);
static void Decoder_F1_E3(void);
static void Decoder_F1_E4(void);

static void Buff_Move_Up_One_Position(unsigned char * buff);
static void Buff_Move_Down_One_Position(unsigned char * buff);
static void Buff_Move_Up_All_Position_And_Init(unsigned char * buff);
static void Buff_Move_Down_All_Position_And_Init(unsigned char * buff);
static u8 Return_End_Of_Buff(unsigned char * buff);
static void Left_Buff_Add_To_Head_Of_Right_Buff(unsigned char * left_buff , unsigned char * right_buff);
static void Left_Buff_Add_To_End_Of_Right_Buff(unsigned char * left_buff , unsigned char * right_buff);
static void Decoder_Line_To_Display_Ram_For_Eight_Byte(unsigned char Display_Ram[48], unsigned char * Decoder_Line);
static void Display_Ram_To_Tm1629(void);

#endif  /* __RF_APP_H */


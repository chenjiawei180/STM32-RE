 /******************************************************************************
  * @file       USER/transmit.h
  * @author  cjw
  * @date     2016.3.25
  * @brief     This file contains the headers of transmit.
  ******************************************************************************/
#ifndef __TRANSMIT_H
#define __TRANSMIT_H

#include "stm32f10x.h"
#include "global.h"
#include "bitband.h"

#define Transmit_Pin    PAout(8)

extern u8   Transmit_Data_FANGQU;
extern u8   Transmit_Data_Set_FANGQU;
extern u16 Transmit_Data;
extern u16 Queue_Number;
extern u8 Transmit_Confirm_Key_Press;

extern void Transmit_IO_Init(void);
extern void Transmit_Timer_Config(void);
extern void Transmit_NVIC_Configuration(void);
extern void TIM3_IRQHandler(void);
extern void Transmit_Function(u32 data);
extern void Transmit_Set_Data(unsigned char data);
extern void Transmit_Number_Of_Set(void);
extern void Transmit_Number_Of_Reciver_Add_One(void);
extern void Transmit_Number_Of_Reciver_Sub_One(void);

#endif


 /******************************************************************************
  * @file       USER/tm1629.h
  * @author  cjw
  * @date     2016.1.22
  * @brief     This file contains the headers of tm1629 drive.
  ******************************************************************************/
#ifndef __TM1629_H
#define __TM1629_H

#include "stm32f10x.h"
#include "global.h"
#include "bitband.h"

#define KEY_01 1
#define KEY_02 2
#define KEY_03 3
#define KEY_04 4
#define KEY_05 5
#define KEY_06 6
#define KEY_07 7
#define KEY_08 8
#define KEY_09 9
#define KEY_10 10
#define KEY_11 11
#define KEY_12 12
#define KEY_13 13
#define KEY_14 14
#define KEY_15 15
#define KEY_16 16
#define KEY_17 17
#define KEY_18 18
#define KEY_19 19
#define KEY_20 20
#define KEY_21 21
#define KEY_22 22
#define KEY_23 23
#define KEY_24 24
#define KEY_25 25
#define KEY_26 26
#define KEY_27 27
#define KEY_28 28
#define KEY_29 29
#define KEY_30 30
#define KEY_31 31
#define KEY_32 32

#define  TM1629_CLK PBout(9) 
#define  TM1629_DIN PBout(8) 
#define  TM1629_DOUT PBin(4)
#define  TM1629_STB1 PBout(5)
#define  TM1629_STB2 PBout(6) 
#define  TM1629_STB3 PBout(7) 


extern u8 Tm1629_Test_Time[8];
extern u8 Tm1629_Display_Ram[6][8]; 
extern void Delayus(u16 time);
extern void Tm1629_GPIO_Config(void);

static void WriteDataTo1629_1(unsigned char Data);
static void WriteDataTo1629_2(unsigned char Data);
static void WriteDataTo1629_3(unsigned char Data);
extern u8 ReadDataFrom1629_1(void);
static void SendCommandTo1629_1(unsigned char Data);
static void SendCommandTo1629_2(unsigned char Data);
static void SendCommandTo1629_3(unsigned char Data);
extern void Tm1629_Display(void);

extern void Tm1629_Clear(void);
extern void Tm1629_Show_Fx(u8 Data);
extern void Tm1629_Show_Ex(u8 Data);
extern void Tm1629_Show_Time(unsigned char* Time);
extern void Tm1629_Blink_Time_Minute(void);
extern void Tm1629_Blink_Time_Hour(void);
extern void Tm1629_Blink_Time_Day(void);
extern void Tm1629_Blink_Time_Month(void);
extern void Tm1629_Blink_Time_Year(void);
extern void Tm1629_Blink_Time_Week(void);

#endif /* __TM1629_H */


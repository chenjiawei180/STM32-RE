 /******************************************************************************
  * @file       USER/gd5800.h
  * @author  cjw
  * @date     2016.3.5
  * @brief     This file contains the headers of gd5800.
  ******************************************************************************/
#ifndef __GD5800_H
#define __GD5800_H

#include "stm32f10x.h"
#include "global.h"
#include "bitband.h"

#define	SIMPLE_REPORT			0
#define	NOT_REPORT				1
#define	COMPLICATED_REPORT		2
#define	NOT_REPORT_C			3
#define	DINGDONG_type			4
#define	YINYUEYI_type			5
#define	YINYUEER_type			6
#define	EMERGENCY_type			7

typedef struct  Repeat_Mode_Saved_Parameters{
unsigned char repeat_times ;
unsigned char report_mode;
unsigned char report_number[8];
unsigned int call_type;
unsigned char key_value;
}Repeat_Save_Paras;

#define GD5800_Busy_Hard_Table    PAin(11)
extern Repeat_Save_Paras Sound_Data;
extern u8 GD5800_Busy_Soft_Table_Count;
extern u8 GD5800_Busy_Soft_Table ;


extern void GD5800_Initialization(void);
extern void Specify_Music_Play(u8 num);
extern void Specify_Volume(u8 num);

static void GD5800_Play_Music_Of_Defense_Area_For_Head(u8 report_mode,u8 * number);
static void GD5800_Play_Music_Of_Defense_Area_For_End(u8 report_mode,u8 * number);
static void GD5800_Play_Music_Of_Hundred(u8 report_mode,u8 * number);
static void GD5800_Play_Music_Of_Ten(u8 report_mode,u8 * number);
static void GD5800_Play_Music_Of_One(u8 report_mode,u8 * number);
static void GD5800_Play_Music_Of_Other(u8 report_mode);
static void GD5800_Play_Music_Of_Last_Three_Bit(u8 report_mode,u8 * number);
static void GD5800_Play_Music_Of_Play_Music_Of_Mode_Within_NOT_REPORT_C(u8 key_type);
static void GD5800_Play_Music_Of_Play_Music_Of_Mode_More_Than_NOT_REPORT_C(u8 report_mode,u8 * number ,u8 key_type);
extern void GD5800_Play_Music_Of_Play_Music(u8 report_mode,u8 * number , u8 call_type, u8 key_type);
extern void GD5800_Play_Mucic_Of_Decoder_Process(u8 report_mode,u8 * number , u8 call_type, u8 key_type,u8 repeat_time);
extern void GD5800_Play_Mucic_Of_Main_Process(void);

#endif /*  __GD5800_H  */

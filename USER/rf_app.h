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

extern u32 Call_Code_Bak;
extern u32 Call_Off_Time;

extern void Decoder_Process(void);
static void Decoder_Standby(void);
static void Decoder_F1_E1(void);
static void Decoder_F1_E2(void);
static void Decoder_F1_E3(void);
static void Decoder_F1_E4(void);

#endif  /* __RF_APP_H */


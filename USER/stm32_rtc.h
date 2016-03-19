 /******************************************************************************
  * @file       USER/stm32_rtc.h
  * @author  cjw
  * @date     2016.3.18
  * @brief     This file contains the headers of the RTC.
  ******************************************************************************/
#ifndef __STM32_RTC_H
#define __STM32_RTC_H

#include "stm32f10x.h"
#include "global.h"
#include "bsp_date.h"

extern struct rtc_time systmtime;

extern void RTC_NVIC_Config(void);
extern void RTC_CheckAndConfig(struct rtc_time *tm);
static void RTC_Configuration(void);
#endif


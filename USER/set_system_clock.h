 /******************************************************************************
  * @file       USER/set_system_clock.h
  * @author  cjw
  * @date     2016.1.18
  * @brief     This file contains the headers of the set system clock funtion and set RTC clock funtion.
  ******************************************************************************/
#ifndef __SET_SYSTEM_CLOCK_H
#define __SET_SYSTEM_CLOCK_H

#include "stm32f10x.h"
#include "global.h"

extern u8 Flag_1_Sec;    //1��ʱ�䵽���־
extern u8 Flag_Half_Sec;    //����ʱ�䵽���־
extern u32 System_Time_Count;    //ϵͳ��ʱ��ֻ���ڲ������־

extern void Set_System_Clock(void);
extern void SysTick_Configuration(void);

#endif  /*  __SET_SYSTEM_CLOCK_H  */


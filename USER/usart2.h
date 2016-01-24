 /******************************************************************************
  * @file       USER/usart2.h
  * @author  cjw
  * @date     2016.1.19
  * @brief     This file contains the headers of the usart2.
  ******************************************************************************/
#ifndef __USART2_H
#define __USART2_H

#include "stm32f10x.h"
#include "global.h"
#include "stdio.h"

extern void CH340_USART2_Config(void);
extern void CH340_NVIC_Configuration(void);
extern void USART2_SendByte(u8 Date);
extern void USART2_IRQHandler(void);

#endif  /* __USART2_H  */


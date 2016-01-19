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

void CH340_USART2_Config(void);
void CH340_NVIC_Configuration(void);
void USART2_SendByte(u8 Date);
void USART2_IRQHandler(void);

#endif  /* __USART2_H  */


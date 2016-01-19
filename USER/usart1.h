 /******************************************************************************
  * @file       USER/usart1.h
  * @author  cjw
  * @date     2016.1.19
  * @brief     This file contains the headers of the usart1.
  ******************************************************************************/
#ifndef __USART1_H
#define __USART1_H

#include "stm32f10x.h"
#include "global.h"

void GD5800_USART1_Config(void);
void GD5800_NVIC_Configuration(void);
void USART1_SendByte(u8 Date);
void USART1_IRQHandler(void);

#endif  /* __USART1_H  */


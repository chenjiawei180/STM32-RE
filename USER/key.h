 /******************************************************************************
  * @file       USER/key.h
  * @author  cjw
  * @date     2016.1.27
  * @brief     This file contains the headers of the Key.
  ******************************************************************************/
#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include "global.h"
#include "bitband.h"
#include "menu.h"

#define KEY_VALUE_MAIN    0x01
#define KEY_VALUE_UP    0x02
#define KEY_VALUE_DOWN    0x03
#define KEY_VALUE_ESC    0x04

#define KEY_MAIN    PAin(0)
#define KEY_UP    PAin(1)
#define KEY_DOWN    PAin(4)
#define KEY_ESC    PAin(5)

extern u8 gKeyValue; /*the value of key press*/
extern Menu_index M_index;  /* the index of menu */

extern void Key_IO_Init(void);
extern unsigned char Key_Scan(void);
static void Key_Delay_us(u16 Time);
extern void Key_Process(void);

#endif /* __KEY_H */


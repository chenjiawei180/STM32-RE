 /******************************************************************************
  * @file       USER/rf.h
  * @author  cjw
  * @date     2016.1.21
  * @brief     This file contains the headers of the PT2242 or EV1527.
  ******************************************************************************/
#ifndef __RF_H
#define __RF_H

#include "stm32f10x.h"
#include "global.h"

#define RF_PIN_LEVEL (GPIOA->IDR & GPIO_Pin_15)    //Read the pin data from GPIOA_PIN15


#define MIN_WIDTH 21 //最小极限电平宽度;//21    //10
#define MAX_WIDTH 210 //最大极限电平宽度;//210   //100
#define DATA_H_WIDTH 85 //0、1判断界线;  //85    //40
#define SYN_WIDTH 300 //同步码低电平宽度限值


extern u8 RF_Fun;    //呼叫键值
extern u32 RF_ID;    //呼叫ID号
extern u8 RF_Fun_Bak;    //历史呼叫键值
extern u32 RF_ID_Bak;    //历史呼叫ID号
extern u32 RF_Temp;    //接收过程移位变量
extern u32 RF_Temp_Bak;    //上一次接收结果
extern u8  RF_Count;    //接收位数计数
extern u8 RF_Flag;    //新信息接收标志
extern u8 RF_Same_Count;    //重复相同接收次数


extern void EXTI15_10_IRQHandler(void);
static void RF_IO_Init(void); 
extern void RF_Config(void);

#endif  /* __RF_H  */


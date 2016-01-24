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


#define MIN_WIDTH 21 //��С���޵�ƽ���;//21    //10
#define MAX_WIDTH 210 //����޵�ƽ���;//210   //100
#define DATA_H_WIDTH 85 //0��1�жϽ���;  //85    //40
#define SYN_WIDTH 300 //ͬ����͵�ƽ�����ֵ


extern u8 RF_Fun;    //���м�ֵ
extern u32 RF_ID;    //����ID��
extern u8 RF_Fun_Bak;    //��ʷ���м�ֵ
extern u32 RF_ID_Bak;    //��ʷ����ID��
extern u32 RF_Temp;    //���չ�����λ����
extern u32 RF_Temp_Bak;    //��һ�ν��ս��
extern u8  RF_Count;    //����λ������
extern u8 RF_Flag;    //����Ϣ���ձ�־
extern u8 RF_Same_Count;    //�ظ���ͬ���մ���


extern void EXTI15_10_IRQHandler(void);
static void RF_IO_Init(void); 
extern void RF_Config(void);

#endif  /* __RF_H  */


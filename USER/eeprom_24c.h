 /******************************************************************************
  * @file       USER/eeprom_24c.h
  * @author  cjw
  * @date     2016.1.24
  * @brief     This file contains the headers of the eeprom.such as 24c32.
  ******************************************************************************/
#ifndef __EEPROM_24C_H
#define __EEPROM_24C_H

#include "stm32f10x.h"
#include "global.h"
#include "bitband.h"
#include "menu.h"

#define I2C_SCL_H    PBout(10)=1    //SCL引脚输出高电平
#define I2C_SCL_L    PBout(10)=0    //SCL引脚输出低电平
#define I2C_SDA_H    PBout(11)=1    //SDA引脚输出高电平
#define I2C_SDA_L    PBout(11)=0    //SDA引脚输出低电平
#define Read_Bit()    PBin(11)    //读SDA引脚电平
#define EEP_WP_ON()    PBout(12)=1    //保护状态
#define EEP_WP_OFF()    PBout(12)=0    //读写状态

#define I2C_OK    0x00    //I2C总线正常标志
#define I2C_ERR    0x01    //I2C总线错误标志
#define EEPADD    0xA0    //EEPROM器件地址
#define EEP_PAGE_SIZE    32    //EEPROM写页大小

#define  CALL_NUMBER   512
#define  ALARM_NUMBER  64
#define  CANCEL_NUMBER 64
#define  HOST_NUMBER   32
#define  CALL_DATA_START   0X0020
#define  ALARM_DATA_START  0X1020
#define  CANCEL_DATA_START 0X1220
#define  HOST_DATA_START   0X1420
#define  SIN_KEY_START    0X1520
#define  MUL_KEY_START    0X1530
#define  BACK_START    0X1540

typedef struct Env_struct{
	unsigned char Set_Call_Queue_Mode;     //及时模式或者排队模式
	unsigned char Set_Call_Display_Number;     //呼叫存储数量
	unsigned char Remove_Call_Time;     //销号时间
	unsigned char Cycle_Call_Time;     //循环时间
	unsigned char Set_Voice_Play_Mode;    //语音模式
	unsigned char Set_Voice_Play_Time;      //语音报读次数
	unsigned char Set_Voice_Play_Is_Or_Not_In_Cycle_Mode;   //循环时候是否报读
	unsigned char Set_Voice_Volume;  //音量大小调整
	unsigned char Set_Display_Tube_Brightness;  //显示屏LED亮度调整
	unsigned char Set_Voice_Navigation_On_Or_OFF;    //语音导航
	unsigned char Set_Voice_Navigation_Mode;     //中英文
	unsigned char Set_Display_LED_Mode;     //闪烁
	unsigned char Set_Two_Menu_F7_E1;     //防区设置
	unsigned char Set_Singal_Or_Multiple_Key_Mode;     //单键位还是多键位
	unsigned char Set_Key_Remove_Call_Is_Or_Not;     //键盘销号
	unsigned char Change_Standby_Display_Mode;     //时间切换
}Env_t;

typedef struct {
    u8 region[4];      //区号0~9~A~Z; //存放的ASCII码
    u32 rf;            //RF地址码，高8位为0表示此块已经被使用。其它为未使用 
}RF_def;

extern void I2C_IO_Init(void);
static void I2C_Delay_us(u16 time);
static void I2C_Start(void);
static u8 I2C_Send_Byte(u8 Data);
static u8 I2C_Receive_Byte(void);
static void I2C_Stop(void);
extern u8 EEP_Read_Buffer(u16 ADD,u8 *buff,u16 Length);
extern u8 EEP_Write_Buffer(u16 ADD,u8 *DatStr,u16 Length);

extern u8 Register_Call_Function(RF_def *pRF);
extern u8 Delete_Call_Function(unsigned char *buf);
extern u8 Register_Host_Function(RF_def *pRF);
extern u8 Delete_Host_Function(unsigned char *buf);
extern u8 Register_Alarm_Function(RF_def *pRF);
extern u8 Delete_Alarm_Function(unsigned char *buf);
extern u8 Register_Cancel_Function(RF_def *pRF);
extern u8 Delete_Cancel_Function(unsigned char *buf);
extern void Delete_All_Data(void);
extern u8 Find_RF_EEPROM(RF_def *p, u32 dat);
extern u8 Find_RF_EEPROM_Call(RF_def *p, u32 dat);
extern u8 Find_RF_EEPROM_Host(RF_def *p, u32 dat);
extern u8 Find_RF_EEPROM_Alarm(RF_def *p, u32 dat);
extern u8 Find_RF_EEPROM_Cancel(RF_def *p, u32 dat);

extern void Env_Save(void);
extern void Env_Init(void);
extern void Var_Init(void);
extern void Key_Init(void);




#endif /* __EEPROM_24C_H */


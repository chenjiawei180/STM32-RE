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

#define I2C_SCL_H    PBout(10)=1    //SCL��������ߵ�ƽ
#define I2C_SCL_L    PBout(10)=0    //SCL��������͵�ƽ
#define I2C_SDA_H    PBout(11)=1    //SDA��������ߵ�ƽ
#define I2C_SDA_L    PBout(11)=0    //SDA��������͵�ƽ
#define Read_Bit()    PBin(11)    //��SDA���ŵ�ƽ
#define EEP_WP_ON()    PBout(12)=1    //����״̬
#define EEP_WP_OFF()    PBout(12)=0    //��д״̬

#define I2C_OK    0x00    //I2C����������־
#define I2C_ERR    0x01    //I2C���ߴ����־
#define EEPADD    0xA0    //EEPROM������ַ
#define EEP_PAGE_SIZE    32    //EEPROMдҳ��С

#define  CALL_NUMBER   512
#define  ALARM_NUMBER  64
#define  CANCEL_NUMBER 64
#define  HOST_NUMBER   32
#define  CALL_DATA_START   0X0020
#define  ALARM_DATA_START  0X0820
#define  CANCEL_DATA_START 0X0A20
#define  HOST_DATA_START   0X0C20

typedef struct {
    u8 region[4];      //����0~9~A~Z; //��ŵ�ASCII��
    u32 rf;            //RF��ַ�룬��8λΪ0��ʾ�˿��Ѿ���ʹ�á�����Ϊδʹ�� 
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


#endif /* __EEPROM_24C_H */


 /******************************************************************************
  * @file       USER/eeprom_24c.c
  * @author  cjw
  * @date     2016.1.24
  * @brief     This file contains the source of the eeprom.such as 24c32.
  ******************************************************************************/
#include "eeprom_24c.h"
#include "string.h"
#include "menu.h"
#include "gd5800.h"
#include "rf_app.h"
#include "usart2.h"

#if defined EEPROM_GLOBAL

Env_t    EEPROM;


/**
  * @brief  This function is Init the eeprom GPIO.
  * @param  None
  * @retval None
  */

void I2C_IO_Init(void)
{
    GPIO_InitTypeDef  GPIO_Struct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    //SDA,SCL
    GPIO_Struct.GPIO_Mode= GPIO_Mode_Out_OD;
    GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Struct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_Struct); 
    //EEPROM写保护
    GPIO_Struct.GPIO_Mode= GPIO_Mode_Out_PP;
    GPIO_Struct.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOB, &GPIO_Struct);

    EEP_WP_ON();    //将EEPROM置为写保护状态
    I2C_SDA_H;    //数据高电平
    I2C_SCL_L;    //时钟低电平
}

/**
  * @brief  This function is I2C Delay.
  * @param  None
  * @retval None
  */

void I2C_Delay_us(u16 time)
{
    u8 i;
    while(time--)
    {
        i=7;
        while(i--);
    }
}

/**
  * @brief  This function is I2C send start signal.
  * @param  None
  * @retval None
  */

void I2C_Start(void)
{
    I2C_SDA_H;
    I2C_Delay_us(5);
    I2C_SCL_H;
    I2C_Delay_us(5);
    I2C_SDA_L;
    I2C_Delay_us(5);
    I2C_SCL_L;
    I2C_Delay_us(5);
}

/**
  * @brief  This function is I2C send a Byte.
  * @param  Data
  * @retval Success or error
  */

u8 I2C_Send_Byte(u8 Data)
{
    u8 i;
    for(i=0;i<8;i++)
    {
        if(Data&0x80)
            I2C_SDA_H;
        else
            I2C_SDA_L;
        I2C_Delay_us(2);
        I2C_SCL_H;
        I2C_Delay_us(2);
        Data<<=1;
        I2C_SCL_L;
        I2C_Delay_us(2);
    }
    I2C_SDA_H;
    I2C_Delay_us(2);
    I2C_SCL_H;
    I2C_Delay_us(2);
    if(Read_Bit())
    return I2C_ERR;

    I2C_SCL_L;
    I2C_Delay_us(2);
    I2C_SDA_L;
    I2C_Delay_us(2);
    return I2C_OK;
}

/**
  * @brief  This function is I2C Receive a Byte.
  * @param  None
  * @retval Data
  */

u8 I2C_Receive_Byte(void)
{
    u8 i,Data=0;
    I2C_SDA_H;    //释放总线准备接收
    for(i=0;i<8;i++)
    {
        Data <<= 1;
        I2C_SCL_H;
        I2C_Delay_us(2);
        if(Read_Bit())
            Data |= 0x01;
        I2C_SCL_L;
        I2C_Delay_us(2);
    }
    return Data;
}

/**
  * @brief  This function is send I2C stop signal.
  * @param  None
  * @retval None
  */

void I2C_Stop(void)
{
    I2C_SCL_L;
    I2C_Delay_us(5);
    I2C_SDA_L;
    I2C_Delay_us(5);
    I2C_SCL_H;
    I2C_Delay_us(5);
    I2C_SDA_H;
    I2C_Delay_us(5);
}

/**
  * @brief  This function is send a buffer.
  * @param  ADD of internal eeprom,index of data buffer,length of data buffer.
  * @retval success or error
  */
  
u8 EEP_Read_Buffer(u16 ADD,u8 *buff,u16 Length)
{
    u8 Flag=1;

    I2C_Start();
    Flag=I2C_Send_Byte(EEPADD);    //发送器件地址
    if(Flag)
        return I2C_ERR;

    Flag=I2C_Send_Byte((ADD >> 8) & 0x1F);    //发送高字节地址
    if(Flag)
        return I2C_ERR;

    Flag=I2C_Send_Byte(ADD & 0xFF);    //发送低字节地址
    if(Flag)
        return I2C_ERR;

    I2C_Start();    //重启总线
    Flag=I2C_Send_Byte(EEPADD+1);    //发送器件地址读
    if(Flag)
        return I2C_ERR;

    while(Length>1)
    {
        *buff = I2C_Receive_Byte();    //读取数据

        I2C_SDA_L;    //发送ACK
        I2C_Delay_us(5);
        I2C_SCL_H;
        I2C_Delay_us(5);
        I2C_SCL_L;
        I2C_Delay_us(5);

        buff++;
        Length--;
    }
    *buff = I2C_Receive_Byte();

    I2C_SDA_H;    //发送NACK
    I2C_Delay_us(5);
    I2C_SCL_H;
    I2C_Delay_us(5);
    I2C_SCL_L;
    I2C_Delay_us(5);

    I2C_Stop();    //总线停止

    return I2C_OK;
}

/**
  * @brief  This function is send a buffer.
  * @param  ADD of internal eeprom,index of data buffer,length of data buffer.
  * @retval success or error
  */

u8 EEP_Write_Buffer(u16 ADD,u8 *DatStr,u16 Length)
{
    u16 temp;
    u16 addr;
    u8 Flag=1;
    u8 i;

    EEP_WP_OFF();    //取消EEPROM写保护状态
    temp = 0;
    while(temp < Length)
    {
        addr = ADD + temp;    //计算地址偏移
        I2C_Start();    //启动总线
        Flag=I2C_Send_Byte(EEPADD);    //发送器件地址
        if(Flag)
            return I2C_ERR;

        Flag=I2C_Send_Byte((addr >> 8) & 0x1F);    //发送高字节地址
        if(Flag)
            return I2C_ERR;

        Flag=I2C_Send_Byte(addr & 0xFF);    //发送低字节地址
        if(Flag)
            return I2C_ERR;

        for (i = 0; i<Length; i++)
        {
            Flag=I2C_Send_Byte(*(DatStr+temp));    //发送数据
            if(Flag)
                return I2C_ERR;
            temp++;
        }

        I2C_Stop();
        //IWDG_ReloadCounter();    //清狗
        I2C_Delay_us(2000);
    }
  
    EEP_WP_ON();//将EEPROM置为写保护状态
    return I2C_OK;
}

/**
  * @brief  This function is register call .
  * @param  RF_def *pRF.
  * @retval success or error
  */

u8 Register_Call_Function(RF_def *pRF)
{
    u16 addr;
    u16 i;
    RF_def RFtmp;
    u16 tmp = 0;
    addr = CALL_DATA_START;

    for (i = 0; i < CALL_NUMBER; i++)
    {
        EEP_Read_Buffer(addr,(u8 *)&RFtmp, sizeof(RF_def));

        if (!(RFtmp.rf & 0xff000000))
        {
            if (Set_Singal_Or_Multiple_Key_Mode == 2)      //多键模式低位为0
            {
                RFtmp.rf &= 0x00fffff0;
                pRF->rf &= 0x00fffff0;
            }
			
            if (RFtmp.rf == pRF->rf)    //有相等RF在里面，重新覆盖
            {
                memcpy(Register_Call_Buff, RFtmp.region, 4);
                Register_Call_Buff[4] = 0;
                Specify_Music_Play(SHIBAI);
                return 1;
            }
        }
        else
        {
            if (!tmp) tmp = addr;
        }
        addr += sizeof(RF_def);
    }

    EEP_Write_Buffer(tmp, (u8 *)pRF, sizeof(RF_def));
    if (tmp != 0)
    {
        Specify_Music_Play(CHENGGONG);
    }
    else
    {
        Specify_Music_Play(SHIBAI);
        return 1;
    }
    //EEP_WriteBytes(tmp, (uint8_t *)pRF, sizeof(RF_def));
    return 0;
}

/**
  * @brief  This function is register Host .
  * @param  RF_def *pRF.
  * @retval success or error
  */

u8 Register_Host_Function(RF_def *pRF)
{
    u16 addr;
    u16 i;
    RF_def RFtmp;
    u16 tmp = 0;
    addr = HOST_DATA_START;

    for (i = 0; i < HOST_NUMBER; i++)
    {
        EEP_Read_Buffer(addr,(u8 *)&RFtmp, sizeof(RF_def));

        if (!(RFtmp.rf & 0xff000000))
        {
            if (Set_Singal_Or_Multiple_Key_Mode == 2)      //多键模式低位为0
            {
                RFtmp.rf &= 0x00fffff0;
                pRF->rf &= 0x00fffff0;
            }

            if (RFtmp.rf == pRF->rf)    //有相等RF在里面，重新覆盖
            {
                memcpy(Register_Host_Buff, RFtmp.region, 4);
                Register_Host_Buff[4] = 0;
                Specify_Music_Play(SHIBAI);
                return 1;
            }
        }
        else
        {
            if (!tmp) tmp = addr;
        }
        addr += sizeof(RF_def);
    }

    EEP_Write_Buffer(tmp, (u8 *)pRF, sizeof(RF_def));
    if (tmp != 0)
    {
        Specify_Music_Play(CHENGGONG);
    }
    else
    {
        Specify_Music_Play(SHIBAI);
        return 1;
    }
    //EEP_WriteBytes(tmp, (uint8_t *)pRF, sizeof(RF_def));
    return 0;
}

/**
  * @brief  This function is register Alarm .
  * @param  RF_def *pRF.
  * @retval success or error
  */

u8 Register_Alarm_Function(RF_def *pRF)
{
    u16 addr;
    u16 i;
    RF_def RFtmp;
    u16 tmp = 0;
    addr = ALARM_DATA_START;

    for (i = 0; i < ALARM_NUMBER; i++)
    {
        EEP_Read_Buffer(addr,(u8 *)&RFtmp, sizeof(RF_def));

        if (!(RFtmp.rf & 0xff000000))
        {
            if (Set_Singal_Or_Multiple_Key_Mode == 2)      //多键模式低位为0
            {
                RFtmp.rf &= 0x00fffff0;
                pRF->rf &= 0x00fffff0;
            }

            if (RFtmp.rf == pRF->rf)    //有相等RF在里面，重新覆盖
            {
                memcpy(Register_Alarm_Buff, RFtmp.region, 4);
                Register_Alarm_Buff[4] = 0;
                Specify_Music_Play(SHIBAI);
                return 1;
            }
        }
        else
        {
            if (!tmp) tmp = addr;
        }
        addr += sizeof(RF_def);
    }

    EEP_Write_Buffer(tmp, (u8 *)pRF, sizeof(RF_def));
    if (tmp != 0)
    {
        Specify_Music_Play(CHENGGONG);
    }
    else
    {
        Specify_Music_Play(SHIBAI);
        return 1;
    }
    //EEP_WriteBytes(tmp, (uint8_t *)pRF, sizeof(RF_def));
    return 0;
}

/**
  * @brief  This function is register Cancel .
  * @param  RF_def *pRF.
  * @retval success or error
  */

u8 Register_Cancel_Function(RF_def *pRF)
{
    u16 addr;
    u16 i;
    RF_def RFtmp;
    u16 tmp = 0;
    addr = CANCEL_DATA_START;

    for (i = 0; i < CANCEL_NUMBER; i++)
    {
        EEP_Read_Buffer(addr,(u8 *)&RFtmp, sizeof(RF_def));

        if (!(RFtmp.rf & 0xff000000))
        {

            if (Set_Singal_Or_Multiple_Key_Mode == 2)      //多键模式低位为0
            {
                RFtmp.rf &= 0x00fffff0;
                pRF->rf &= 0x00fffff0;
            }

            if (RFtmp.rf == pRF->rf)    //有相等RF在里面，重新覆盖
            {
                memcpy(Register_Cancel_Buff, RFtmp.region, 4);
                Register_Cancel_Buff[4] = 0;
                Specify_Music_Play(SHIBAI);
                return 1;
            }
        }
        else
        {
            if (!tmp) tmp = addr;
        }
        addr += sizeof(RF_def);
    }

    EEP_Write_Buffer(tmp, (u8 *)pRF, sizeof(RF_def));
    if (tmp != 0)
    {
        Specify_Music_Play(CHENGGONG);
    }
    else
    {
        Specify_Music_Play(SHIBAI);
        return 1;
    }
    //EEP_WriteBytes(tmp, (uint8_t *)pRF, sizeof(RF_def));
    return 0;
}

/**
  * @brief  This function is delete Call .
  * @param  unsigned char *buf
  * @retval success or error
  */

u8 Delete_Call_Function(unsigned char *buf)//buf为组码数组的指针
{
    RF_def RFtmp;
    u16 addr = CALL_DATA_START;
    u16 i;
    unsigned char dofly[32] = {0};
    memset(dofly,0xff,32);
    if (*(volatile u32*)buf == 0x00000000ul)
    {
        for (i = 0; i<128; i++)
        {
            EEP_Write_Buffer( addr + (i << 5), dofly, 32);
            I2C_Delay_us(100);
        }
    }
    else
    {
        for (i = 0; i < CALL_NUMBER; i++)
        {
            EEP_Read_Buffer(addr, (u8 *)&RFtmp, sizeof(RF_def));
            if (!(RFtmp.rf & 0xff000000ul))
            {
                if (*(volatile u32*)buf == *(volatile u32*)(RFtmp.region))
                {
                    memset(&RFtmp, 0xff, sizeof(RF_def));
                    EEP_Write_Buffer(addr, (u8 *)&RFtmp, sizeof(RF_def));
                    break;
                }
            }
            addr += sizeof(RF_def);
        }
    }
    return 0;
}

/**
  * @brief  This function is delete Host .
  * @param  unsigned char *buf
  * @retval success or error
  */

u8 Delete_Host_Function(unsigned char *buf)//buf为组码数组的指针
{
    RF_def RFtmp;
    u16 addr = HOST_DATA_START;
    u16 i;
    unsigned char dofly[32] = {0};
    memset(dofly,0xff,32);
    if (*(volatile u32*)buf == 0x00000000ul)
    {
        for (i = 0; i<8; i++)
        {
            EEP_Write_Buffer( addr + (i << 5), dofly, 32);
            I2C_Delay_us(100);
        }
    }
    else
    {
        for (i = 0; i < HOST_NUMBER; i++)
        {
            EEP_Read_Buffer(addr, (u8 *)&RFtmp, sizeof(RF_def));
            if (!(RFtmp.rf & 0xff000000ul))
            {
                if (*(volatile u32*)buf == *(volatile u32*)(RFtmp.region))
                {
                    memset(&RFtmp, 0xff, sizeof(RF_def));
                    EEP_Write_Buffer(addr, (u8 *)&RFtmp, sizeof(RF_def));
                    break;
                }
            }
            addr += sizeof(RF_def);
        }
    }
    return 0;
}

/**
  * @brief  This function is delete Alarm .
  * @param  unsigned char *buf
  * @retval success or error
  */

u8 Delete_Alarm_Function(unsigned char *buf)//buf为组码数组的指针
{
    RF_def RFtmp;
    u16 addr = ALARM_DATA_START;
    u16 i;
    unsigned char dofly[32] = {0};
    memset(dofly,0xff,32);
    if (*(volatile u32*)buf == 0x00000000ul)
    {
        for (i = 0; i<16; i++)
        {
            EEP_Write_Buffer( addr + (i << 5), dofly, 32);
            I2C_Delay_us(100);
        }
    }
    else
    {
        for (i = 0; i < ALARM_NUMBER; i++)
        {
            EEP_Read_Buffer(addr, (u8 *)&RFtmp, sizeof(RF_def));
            if (!(RFtmp.rf & 0xff000000ul))
            {
                if (*(volatile u32*)buf == *(volatile u32*)(RFtmp.region))
                {
                    memset(&RFtmp, 0xff, sizeof(RF_def));
                    EEP_Write_Buffer(addr, (u8 *)&RFtmp, sizeof(RF_def));
                    break;
                }
            }
            addr += sizeof(RF_def);
        }
    }
    return 0;
}

/**
  * @brief  This function is delete Cancel
  * @param  unsigned char *buf.
  * @retval success or error
  */

u8 Delete_Cancel_Function(unsigned char *buf)//buf为组码数组的指针
{
    RF_def RFtmp;
    u16 addr = CANCEL_DATA_START;
    u16 i;
    unsigned char dofly[32] = {0};
    memset(dofly,0xff,32);
    if (*(volatile u32*)buf == 0x00000000ul)
    {
        for (i = 0; i<16; i++)
        {
            EEP_Write_Buffer( addr + (i << 5), dofly, 32);
            I2C_Delay_us(100);
        }
    }
    else
    {
        for (i = 0; i < CANCEL_NUMBER; i++)
        {
            EEP_Read_Buffer(addr, (u8 *)&RFtmp, sizeof(RF_def));
            if (!(RFtmp.rf & 0xff000000ul))
            {
                if (*(volatile u32*)buf == *(volatile u32*)(RFtmp.region))
                {
                    memset(&RFtmp, 0xff, sizeof(RF_def));
                    EEP_Write_Buffer(addr, (u8 *)&RFtmp, sizeof(RF_def));
                    break;
                }
            }
            addr += sizeof(RF_def);
        }
    }
    return 0;
}

/**
  * @brief  This function is delete all data.
  * @param  None
  * @retval None
  */

void Delete_All_Data(void)
{
    unsigned char dofly[32] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    unsigned int k;
    for (k = 0; k<168; k++)
    {
        EEP_Write_Buffer( 0 + (k << 5), dofly, 32);                   //写入24c02
    }
}

/**
  * @brief  This function is find the u32 dat is EEPROM save ID and it's type or not.
  * @param  RF_def *p, u32 dat
  * @retval 0 1 2 3   0 is not . 1 is cancel . 2 is alarm . 3 is call.
  */

u8 Find_RF_EEPROM(RF_def *p, u32 dat)
{
    if (Find_RF_EEPROM_Cancel(p, dat))
    {
        return 1;
    }
    else if (Find_RF_EEPROM_Alarm(p, dat))
    {
        return 2;
    }
    else if (Find_RF_EEPROM_Call(p, dat))
    {
        return 3;
    }
    else
    {
        return 0;
    }
}

/**
  * @brief  This function is find the u32 dat is EEPROM save CALL ID or not.
  * @param  RF_def *p, u32 dat
  * @retval 0 1 . 1 is success  0 is failure
  */
  
u8 Find_RF_EEPROM_Call(RF_def *p, u32 dat)
{
    RF_def RFtmp;
    u16 addr;
    u16 i;
    addr = CALL_DATA_START;

    for (i = 0; i<CALL_NUMBER; i++)
    {
        EEP_Read_Buffer(addr, (u8 *)&RFtmp, sizeof(RF_def));
        //EEP_ReadBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));

        if (Set_Singal_Or_Multiple_Key_Mode == 2)      //多键模式低位为0
        {
            RFtmp.rf &= 0x00fffff0;
            dat &= 0x00fffff0;
        }

        if (RFtmp.rf == dat)
        {
            memcpy(p, &RFtmp, sizeof(RF_def));
            return 1;
        }
        addr += sizeof(RF_def);
    }
    return 0;
}

/**
  * @brief  This function is find the u32 dat is EEPROM save Host ID or not.
  * @param  RF_def *p, u32 dat
  * @retval 0 1 . 1 is success  0 is failure
  */
  
u8 Find_RF_EEPROM_Host(RF_def *p, u32 dat)
{
    RF_def RFtmp;
    u16 addr;
    u16 i;
    addr = HOST_DATA_START;

    for (i = 0; i<HOST_NUMBER; i++)
    {
        EEP_Read_Buffer(addr, (u8 *)&RFtmp, sizeof(RF_def));
        //EEP_ReadBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));

        if (Set_Singal_Or_Multiple_Key_Mode == 2)      //多键模式低位为0
        {
            RFtmp.rf &= 0x00fffff0;
            dat &= 0x00fffff0;
        }

        if (RFtmp.rf == dat)
        {
            memcpy(p, &RFtmp, sizeof(RF_def));
            return 1;
        }
        addr += sizeof(RF_def);
    }
    return 0;
}

/**
  * @brief  This function is find the u32 dat is EEPROM save Alarm ID or not.
  * @param  RF_def *p, u32 dat
  * @retval 0 1 . 1 is success  0 is failure
  */

u8 Find_RF_EEPROM_Alarm(RF_def *p, u32 dat)
{
    RF_def RFtmp;
    u16 addr;
    u16 i;
    addr = ALARM_DATA_START;

    for (i = 0; i<ALARM_NUMBER; i++)
    {
        EEP_Read_Buffer(addr, (u8 *)&RFtmp, sizeof(RF_def));
        //EEP_ReadBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));

        if (Set_Singal_Or_Multiple_Key_Mode == 2)      //多键模式低位为0
        {
            RFtmp.rf &= 0x00fffff0;
            dat &= 0x00fffff0;
        }

        if (RFtmp.rf == dat)
        {
            memcpy(p, &RFtmp, sizeof(RF_def));
            return 1;
        }
        addr += sizeof(RF_def);
    }
    return 0;
}

/**
  * @brief  This function is find the u32 dat is EEPROM save Cancel ID or not.
  * @param  RF_def *p, u32 dat
  * @retval 0 1 . 1 is success  0 is failure
  */
  
u8 Find_RF_EEPROM_Cancel(RF_def *p, u32 dat)
{
    RF_def RFtmp;
    u16 addr;
    u16 i;
    addr = CANCEL_DATA_START;

    for (i = 0; i<CANCEL_NUMBER; i++)
    {
        EEP_Read_Buffer(addr, (u8 *)&RFtmp, sizeof(RF_def));
        //EEP_ReadBytes(addr, (uint8_t *)&RFtmp, sizeof(RF_def));

        if (Set_Singal_Or_Multiple_Key_Mode == 2)      //多键模式低位为0
        {
            RFtmp.rf &= 0x00fffff0;
            dat &= 0x00fffff0;
        }

        if (RFtmp.rf == dat)
        {
            memcpy(p, &RFtmp, sizeof(RF_def));
            return 1;
        }
        addr += sizeof(RF_def);
    }
    return 0;
}

/**
  * @brief  This function is Env_Save.
  * @param  None
  * @retval None
  */
  
void Env_Save(void)
{
    EEPROM.Set_Call_Queue_Mode = Set_Call_Queue_Mode;
    EEPROM.Set_Call_Display_Number = Set_Call_Display_Number;
    EEPROM.Remove_Call_Time = Remove_Call_Time;
    EEPROM.Cycle_Call_Time = Cycle_Call_Time;
    EEPROM.Set_Voice_Play_Mode = Set_Voice_Play_Mode;
    EEPROM.Set_Voice_Play_Time = Set_Voice_Play_Time;
    EEPROM.Set_Voice_Play_Is_Or_Not_In_Cycle_Mode = Set_Voice_Play_Is_Or_Not_In_Cycle_Mode;
    EEPROM.Set_Voice_Volume = Set_Voice_Volume;
    EEPROM.Set_Display_Tube_Brightness = Set_Display_Tube_Brightness;
    EEPROM.Set_Voice_Navigation_On_Or_OFF = Set_Voice_Navigation_On_Or_OFF;
    EEPROM.Set_Voice_Navigation_Mode = Set_Voice_Navigation_Mode;
    EEPROM.Set_Display_LED_Mode = Set_Display_LED_Mode;
    EEPROM.Set_Two_Menu_F7_E1 = Set_Two_Menu_F7_E1;
    EEPROM.Set_Singal_Or_Multiple_Key_Mode = Set_Singal_Or_Multiple_Key_Mode;
    EEPROM.Set_Key_Remove_Call_Is_Or_Not = Set_Key_Remove_Call_Is_Or_Not;
    EEPROM.Change_Standby_Display_Mode = Change_Standby_Display_Mode;

    EEP_Write_Buffer( BACK_START, (u8 *)&EEPROM, 16);
}

/**
  * @brief  This function is Env_Save.
  * @param  None
  * @retval None
  */
  
void Env_Init(void)
{
    EEP_Read_Buffer(SIN_KEY_START, single_key, 16);
    EEP_Read_Buffer(MUL_KEY_START, multiple_key, 16);
    EEP_Read_Buffer(BACK_START, (u8 *)&EEPROM, 16);
    Set_Call_Queue_Mode = EEPROM.Set_Call_Queue_Mode;
    Set_Call_Display_Number = EEPROM.Set_Call_Display_Number ;
    Remove_Call_Time = EEPROM.Remove_Call_Time ;
    Cycle_Call_Time = EEPROM.Cycle_Call_Time ;
    Set_Voice_Play_Mode = EEPROM.Set_Voice_Play_Mode ;
    Set_Voice_Play_Time = EEPROM.Set_Voice_Play_Time ;
    Set_Voice_Play_Is_Or_Not_In_Cycle_Mode = EEPROM.Set_Voice_Play_Is_Or_Not_In_Cycle_Mode ;
    Set_Voice_Volume = EEPROM.Set_Voice_Volume ;
    Set_Display_Tube_Brightness = EEPROM.Set_Display_Tube_Brightness ;
    Set_Voice_Navigation_On_Or_OFF = EEPROM.Set_Voice_Navigation_On_Or_OFF ;
    Set_Voice_Navigation_Mode = EEPROM.Set_Voice_Navigation_Mode ;
    Set_Display_LED_Mode = EEPROM.Set_Display_LED_Mode ;
    Set_Two_Menu_F7_E1 = EEPROM.Set_Two_Menu_F7_E1 ;
    Set_Singal_Or_Multiple_Key_Mode = EEPROM.Set_Singal_Or_Multiple_Key_Mode ;
    Set_Key_Remove_Call_Is_Or_Not = EEPROM.Set_Key_Remove_Call_Is_Or_Not ;
    Change_Standby_Display_Mode = EEPROM.Change_Standby_Display_Mode ;

    if (EEPROM.Set_Call_Queue_Mode > 2)
    {
        Set_Call_Queue_Mode = 1;
    }
    if (EEPROM.Set_Call_Display_Number > 99)
    {
        Set_Call_Display_Number = 1;
    }
    if (EEPROM.Remove_Call_Time > 99)
    {
        Remove_Call_Time = 1;
    }
    if (EEPROM.Cycle_Call_Time > 99)
    {
        Cycle_Call_Time = 1;
    }
    if (EEPROM.Set_Voice_Play_Mode > 7)
    {
        Set_Voice_Play_Mode = 0;
    }
    if (EEPROM.Set_Voice_Play_Time > 9)
    {
        Set_Voice_Play_Time = 1;
    }
    if (EEPROM.Set_Voice_Play_Is_Or_Not_In_Cycle_Mode > 1)
    {
        Set_Voice_Play_Is_Or_Not_In_Cycle_Mode = 0;
    }
    if (EEPROM.Set_Voice_Volume > 9)
    {
        Set_Voice_Volume = 6;
    }
    if (EEPROM.Set_Display_Tube_Brightness > 7)
    {
        Set_Display_Tube_Brightness = 3;
    }
    if (EEPROM.Set_Voice_Navigation_On_Or_OFF > 1)
    {
        Set_Voice_Navigation_On_Or_OFF = 0;
    }
    if (EEPROM.Set_Voice_Navigation_Mode > 1)
    {
        Set_Voice_Navigation_Mode = 0;
    }
    if (EEPROM.Set_Display_LED_Mode > 1)
    {
        Set_Display_LED_Mode = 0;
    }
    if (EEPROM.Set_Two_Menu_F7_E1 > 11)
    {
        Set_Two_Menu_F7_E1 = 11;
    }
    if (EEPROM.Set_Singal_Or_Multiple_Key_Mode > 2)
    {
        Set_Singal_Or_Multiple_Key_Mode = 2;
    }
    if (EEPROM.Set_Key_Remove_Call_Is_Or_Not > 1)
    {
        Set_Key_Remove_Call_Is_Or_Not = 1;
    }
    if (EEPROM.Change_Standby_Display_Mode > 2)
    {
        Change_Standby_Display_Mode = 1;
    }
}

/**
  * @brief  This function is Var_Init.
  * @param  None
  * @retval None
  */
  
void Var_Init(void)
{
    Set_Call_Queue_Mode = 1;     //及时模式或者排队模式
    Set_Call_Display_Number = 1;     //呼叫存储数量
    Remove_Call_Time = 0;     //销号时间
    Cycle_Call_Time = 0;     //循环时间
    Set_Voice_Play_Mode = 0;    //语音模式
    Set_Voice_Play_Time = 1;      //语音报读次数
    Set_Voice_Play_Is_Or_Not_In_Cycle_Mode = 0;   //循环时候是否报读
    Set_Voice_Volume = 6;  //音量大小调整
    Set_Display_Tube_Brightness = 3;  //显示屏LED亮度调整
    Set_Voice_Navigation_On_Or_OFF = 1;    //语音导航
    Set_Voice_Navigation_Mode = 0;     //中英文
    Set_Display_LED_Mode = 0;     //闪烁
    Set_Two_Menu_F7_E1 = 11;     //防区设置
    Set_Singal_Or_Multiple_Key_Mode = 2;     //单键位还是多键位
}

/**
  * @brief  This function is Key_init.
  * @param  None
  * @retval None
  */
  
void Key_Init(void)
{
    memset(single_key,0x01,16);

    multiple_key[0] = 0X01;
    multiple_key[1] = QUXIAO - QUXIAO;
    multiple_key[2] = JIEZHANG - QUXIAO;
    multiple_key[3] = DIANDANG - QUXIAO;
    multiple_key[4] = JIUSHUI - QUXIAO;
    multiple_key[5] = 0X01;
    multiple_key[6] = 0x01;
    multiple_key[7] = JIASHUI - QUXIAO;
    multiple_key[8] = HUJIAO - QUXIAO;
    multiple_key[9] =  0x01;
    multiple_key[10] = 0x01;
    multiple_key[11] = 0x01;
    multiple_key[12] = 0x01;
    multiple_key[13] = 0x01;
    multiple_key[14] = 0x01;
    multiple_key[15] = 0x01;
}




#endif /* EEPROM_GLOBAL */

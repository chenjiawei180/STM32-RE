 /******************************************************************************
  * @file       USER/eeprom_24c.c
  * @author  cjw
  * @date     2016.1.24
  * @brief     This file contains the source of the eeprom.such as 24c32.
  ******************************************************************************/
#include "eeprom_24c.h"
#include "string.h"
#include "menu.h"

#if defined EEPROM_GLOBAL

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
/*
            if (return_Two_Menu_F8_E1() == 2)      //多键模式低位为0
            {
                RFtmp.rf &= 0x00fffff0;
                pRF->rf &= 0x00fffff0;
            }
*/
            if (RFtmp.rf == pRF->rf)    //有相等RF在里面，重新覆盖
            {
                memcpy(Register_Call_Buff, RFtmp.region, 4);
                Register_Call_Buff[4] = 0;
                //GD5800_select_chapter_new(SHIBAI);
                return 1;
            }
        }
        else
        {
            if (!tmp) tmp = addr;
        }
        addr += sizeof(RF_def);
    }

    EEP_Write_Buffer(tmp, (uint8_t *)pRF, sizeof(RF_def));
    if (tmp != 0)
    {
        //GD5800_select_chapter_new(CHENGGONG);
    }
    else
    {
        //GD5800_select_chapter_new(SHIBAI);
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
/*
            if (return_Two_Menu_F8_E1() == 2)      //多键模式低位为0
            {
                RFtmp.rf &= 0x00fffff0;
                pRF->rf &= 0x00fffff0;
            }
*/
            if (RFtmp.rf == pRF->rf)    //有相等RF在里面，重新覆盖
            {
                memcpy(Register_Host_Buff, RFtmp.region, 4);
                Register_Host_Buff[4] = 0;
                //GD5800_select_chapter_new(SHIBAI);
                return 1;
            }
        }
        else
        {
            if (!tmp) tmp = addr;
        }
        addr += sizeof(RF_def);
    }

    EEP_Write_Buffer(tmp, (uint8_t *)pRF, sizeof(RF_def));
    if (tmp != 0)
    {
        //GD5800_select_chapter_new(CHENGGONG);
    }
    else
    {
        //GD5800_select_chapter_new(SHIBAI);
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
/*
            if (return_Two_Menu_F8_E1() == 2)      //多键模式低位为0
            {
                RFtmp.rf &= 0x00fffff0;
                pRF->rf &= 0x00fffff0;
            }
*/
            if (RFtmp.rf == pRF->rf)    //有相等RF在里面，重新覆盖
            {
                memcpy(Register_Alarm_Buff, RFtmp.region, 4);
                Register_Alarm_Buff[4] = 0;
                //GD5800_select_chapter_new(SHIBAI);
                return 1;
            }
        }
        else
        {
            if (!tmp) tmp = addr;
        }
        addr += sizeof(RF_def);
    }

    EEP_Write_Buffer(tmp, (uint8_t *)pRF, sizeof(RF_def));
    if (tmp != 0)
    {
        //GD5800_select_chapter_new(CHENGGONG);
    }
    else
    {
        //GD5800_select_chapter_new(SHIBAI);
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
/*
            if (return_Two_Menu_F8_E1() == 2)      //多键模式低位为0
            {
                RFtmp.rf &= 0x00fffff0;
                pRF->rf &= 0x00fffff0;
            }
*/
            if (RFtmp.rf == pRF->rf)    //有相等RF在里面，重新覆盖
            {
                memcpy(Register_Cancel_Buff, RFtmp.region, 4);
                Register_Cancel_Buff[4] = 0;
                //GD5800_select_chapter_new(SHIBAI);
                return 1;
            }
        }
        else
        {
            if (!tmp) tmp = addr;
        }
        addr += sizeof(RF_def);
    }

    EEP_Write_Buffer(tmp, (uint8_t *)pRF, sizeof(RF_def));
    if (tmp != 0)
    {
        //GD5800_select_chapter_new(CHENGGONG);
    }
    else
    {
        //GD5800_select_chapter_new(SHIBAI);
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

#endif /* EEPROM_GLOBAL */

 /******************************************************************************
  * @file       USER/gd5800.c
  * @author  cjw
  * @date     2016.3.5
  * @brief     This file contains the source of the gd5800.
  ******************************************************************************/
#include "gd5800.h"
#include "usart1.h"
#include "sound_list.h"
#include "menu.h"
#include "string.h"
#include "usart2.h"
#include "rf_app.h"

#ifdef USART2_GLOBAL

Repeat_Save_Paras Sound_Data;
u8 GD5800_Busy_Soft_Table_Count = 0;
u8 GD5800_Busy_Soft_Table = 1;
u8 GD5800_Voice_Save_Queue[800] = {0};

/**
  * @brief  This function is Initialization GD5800. set the cycle mode.
  * @param  None
  * @retval None
  */

void GD5800_Initialization(void)
{
    GPIO_InitTypeDef  GPIO_Struct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_Struct.GPIO_Pin =GPIO_Pin_11;
    GPIO_Struct.GPIO_Mode= GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_Struct);

    USART1_SendByte(0x7E);       /***frame command ****/
    USART1_SendByte(0X03);
    USART1_SendByte(0X33);
    USART1_SendByte(0x04);
    USART1_SendByte(0XEF);
}

/**
  * @brief  This function is Specify_Music_Play .
  * @param  number
  * @retval None
  */

void Specify_Music_Play(u8 num)
{
    USART1_SendByte(0x7E);       /***frame command ****/
    USART1_SendByte(0X04);
    USART1_SendByte(0X41);
    USART1_SendByte(0x00);
    USART1_SendByte(num);
    USART1_SendByte(0XEF);
}

/**
  * @brief  This function is Specify_Volume .
  * @param  number
  * @retval None
  */

void Specify_Volume(u8 num)
{
    USART1_SendByte(0x7E);       /***frame command ****/
    USART1_SendByte(0X03);
    USART1_SendByte(0X31);
    USART1_SendByte(num);
    USART1_SendByte(0XEF);
}

/**
  * @brief  This function is GD5800_Play_Music_Of_Defense_Area_For_Head .
  * @param  report_mode number
  * @retval None
  */

void GD5800_Play_Music_Of_Defense_Area_For_Head(u8 report_mode,u8 * number)
{
    if (*(number + 1) <= 9)
    {
        if (report_mode <= NOT_REPORT)
        {
            Specify_Music_Play(*(number + 1) + ZERO);
        }
        else
        {
            if (*(number + 1) != 0)
            {
                Specify_Music_Play(*(number + 1) + ZERO);
                Specify_Music_Play(THOUSAND);
            }
        }
    }
    else if (*(number + 1) <= 15 && *(number + 1) >= 10)
    {
        Specify_Music_Play(*(number + 1) + ZIMU_A - 10);
    }
    else
    {
        switch (*(number + 1))
        {
            case 16:Specify_Music_Play(ZIMU_H); break;//H
            case 17:Specify_Music_Play(ZIMU_L); break;//L
            case 18:Specify_Music_Play(ZIMU_O); break;//O
            case 19:Specify_Music_Play(ZIMU_P); break;//P
            case 20:Specify_Music_Play(ZIMU_U); break;//U
            default:break;
        }
    }
}

/**
  * @brief  This function is GD5800_Play_Music_Of_Defense_Area_For_End .
  * @param  report_mode number
  * @retval None
  */

void GD5800_Play_Music_Of_Defense_Area_For_End(u8 report_mode,u8 * number)
{
    Specify_Music_Play(DAO);
    if (*(number + 1) <= 9)
    {
        Specify_Music_Play(*(number + 1) + ZERO);
    }
    else if (*(number + 1) <= 15 && *(number + 1) >= 10)
    {
        Specify_Music_Play(*(number + 1) + ZIMU_A - 10);
    }
    else
    {
        switch (*(number + 1))
        {
            case 16:Specify_Music_Play(ZIMU_H); break;//H
            case 17:Specify_Music_Play(ZIMU_L); break;//L
            case 18:Specify_Music_Play(ZIMU_O); break;//O
            case 19:Specify_Music_Play(ZIMU_P); break;//P
            case 20:Specify_Music_Play(ZIMU_U); break;//U
            default:break;
        }
    }
    Specify_Music_Play(GUITAI);
}

/**
  * @brief  This function is GD5800_Play_Music_Of_Hundred .
  * @param  report_mode number
  * @retval None
  */

void GD5800_Play_Music_Of_Hundred(u8 report_mode,u8 * number)
{
    if (report_mode <= NOT_REPORT)
    {
        Specify_Music_Play(*(number + 2) + ZERO);/*呼叫号数百位*/
    }
    else
    {
        if (*(number + 2) != 0)
        {
            Specify_Music_Play(*(number + 2) + ZERO);
            Specify_Music_Play(HUNDRED);/*hundred 的声音*/
            Specify_Music_Play(AND);/*and的声音*/
        }
    }
}

/**
  * @brief  This function is GD5800_Play_Music_Of_Ten .
  * @param  report_mode number
  * @retval None
  */

void GD5800_Play_Music_Of_Ten(u8 report_mode,u8 * number)
{
    if (report_mode <= NOT_REPORT)
    {
        Specify_Music_Play(*(number + 3) + ZERO);
    }
    else
    {
        if (*(number + 3) != 0)
        {
            if (*(number + 3) == 1)
            {
                if (*(number + 4) == 0) //10
                {
                    Specify_Music_Play(TEN);
                }
                else//11-19
                {
                    Specify_Music_Play(*(number + 4) + ELEVEN - 1);
                }
            }
            else//20-90
            {
                Specify_Music_Play(*(number + 3) + TWENTY - 2);
            }
        }
    }
}

/**
  * @brief  This function is GD5800_Play_Music_Of_One .
  * @param  report_mode number
  * @retval None
  */

void GD5800_Play_Music_Of_One(u8 report_mode,u8 * number)
{
    if (report_mode <= NOT_REPORT)
    {
        Specify_Music_Play(*(number + 4) + ZERO);
    }
    else
    {
        if (*(number + 4) != 0 && *(number + 3) != 1)
        {
            Specify_Music_Play(*(number + 4) + ZERO);
        }
    }
}

/**
  * @brief  This function is GD5800_Play_Music_Of_Other .
  * @param  report_mode 
  * @retval None
  */

void GD5800_Play_Music_Of_Other(u8 report_mode)
{
    switch (report_mode)
    {
        case DINGDONG_type:
            Specify_Music_Play(DINGDONG);
            break;
        case YINYUEYI_type:
            Specify_Music_Play(YINGYUEYI);
            break;
        case YINYUEER_type:
            Specify_Music_Play(YINGYUEER);
            break;
        case EMERGENCY_type:
            Specify_Music_Play(BAOJING);
            break;
        default:break;
    }
}

/**
  * @brief  This function is GD5800_Play_Music_Of_Last_Three_Bit .
  * @param  report_mode number
  * @retval None
  */

void GD5800_Play_Music_Of_Last_Three_Bit(u8 report_mode,u8 * number)
{
    if (report_mode <= NOT_REPORT_C)
    {
        GD5800_Play_Music_Of_Hundred(report_mode,number);
        GD5800_Play_Music_Of_Ten(report_mode,number);
        GD5800_Play_Music_Of_One(report_mode,number);
    }
    else
    {
        GD5800_Play_Music_Of_Other(report_mode);
    }
}

/**
  * @brief  This function is GD5800_Play_Music_Of_Play_Music_Of_Mode_Within_NOT_REPORT_C .
  * @param  call_type
  * @retval None
  */

void GD5800_Play_Music_Of_Play_Music_Of_Mode_Within_NOT_REPORT_C(u8 call_type)
{
    if (call_type < 50)
    {
        Specify_Music_Play(call_type + QUXIAO);
    }
    else if (call_type == BAOJING_1)
    {
        Specify_Music_Play(BAOJING);
    }		
}

/**
  * @brief  This function is GD5800_Play_Music_Of_Play_Music_Of_Mode_More_Than_NOT_REPORT_C .
  * @param  report_mode number
  * @retval None
  */

void GD5800_Play_Music_Of_Play_Music_Of_Mode_More_Than_NOT_REPORT_C(u8 report_mode,u8 * number ,u8 call_type)
{
    if (call_type == BAOJING_1 || call_type == BAOJING-QUXIAO)
    {
        Specify_Music_Play(BAOJING);
    }
    else
    {
        GD5800_Play_Music_Of_Last_Three_Bit(report_mode, number);
    }
}

/**
  * @brief  This function is GD5800_Play_Music_Of_Play_Music .
  * @param  report_mode number
  * @retval None
  */

void GD5800_Play_Music_Of_Play_Music(u8 report_mode,u8 * number , u8 call_type, u8 key_type)
{
    if(report_mode <= NOT_REPORT_C)
    {
        Specify_Music_Play(DINGDONG);
		
        if(key_type == 15)
        {
            Specify_Music_Play(QING);
        }
		
        if (Set_Voice_Navigation_Mode == 0 || ( Set_Voice_Navigation_Mode == 1 && call_type >= COMPLICATED_REPORT))
        {
            Specify_Music_Play(HAO);
        }
		
        if(key_type !=15)
        {
            GD5800_Play_Music_Of_Defense_Area_For_Head(report_mode, number);
        }
        GD5800_Play_Music_Of_Last_Three_Bit(report_mode, number);
		
        if (Set_Voice_Navigation_Mode == 1 && report_mode <= NOT_REPORT)
        {
            Specify_Music_Play(HAO);
        }

        if(key_type == 15)
        {
            GD5800_Play_Music_Of_Defense_Area_For_End(report_mode, number);
        }
		
        else if (report_mode != NOT_REPORT && report_mode != NOT_REPORT_C)
        {
            GD5800_Play_Music_Of_Play_Music_Of_Mode_Within_NOT_REPORT_C(call_type);
        }
    }
    else
    {
        GD5800_Play_Music_Of_Play_Music_Of_Mode_More_Than_NOT_REPORT_C(report_mode,number,call_type);
    }
}

/**
  * @brief  This function is GD5800_Play_Mucic_Of_Decoder_Process .
  * @param  report_mode number call_type key_type repeat_time
  * @retval None
  */

void GD5800_Play_Mucic_Of_Decoder_Process(u8 report_mode,u8 * number , u8 call_type, u8 key_type,u8 repeat_time)
{
    if(repeat_time > 1)
    {
        Sound_Data.repeat_times = repeat_time;
        Sound_Data.call_type = call_type;
        Sound_Data.key_value = key_type;
        Sound_Data.report_mode= report_mode;
        memcpy(Sound_Data.report_number,number,8);
    }
    else
    {
        GD5800_Play_Music_Of_Play_Music(report_mode,number ,call_type, key_type);
    }
}

/**
  * @brief  This function is GD5800_Play_Mucic_Of_Main_Process .
  * @param  None
  * @retval None
  */

void GD5800_Play_Mucic_Of_Main_Process(void)
{
    u8 voice_buff[8]={0};
    if(Set_Call_Queue_Mode <3)
    {
        if(Sound_Data.repeat_times>0)
        {
            if(GD5800_Busy_Soft_Table && GD5800_Busy_Hard_Table)
            {
                GD5800_Busy_Soft_Table = 0 ;
                GD5800_Play_Music_Of_Play_Music(Sound_Data.report_mode,Sound_Data.report_number,Sound_Data.call_type,Sound_Data.key_value);
                Sound_Data.repeat_times--;
            }
        }
    }
    else
    {
        if(GD5800_Busy_Soft_Table && GD5800_Busy_Hard_Table)
        {
            if(Sound_Data.repeat_times>0)
            {
                GD5800_Busy_Soft_Table = 0 ;
                GD5800_Play_Music_Of_Play_Music(Sound_Data.report_mode,Sound_Data.report_number,Sound_Data.call_type,Sound_Data.key_value);
                Sound_Data.repeat_times--;
            }
            else if(Sound_Data.repeat_times == 0 && GD5800_Voice_Save_Queue[0] !=0)
            {
                memcpy(voice_buff,GD5800_Voice_Save_Queue,8);
                Buff_Move_Up_All_Position_And_Init(GD5800_Voice_Save_Queue);
                Left_Buff_Add_To_Head_Of_Right_Buff(voice_buff,Decoder_Call_Save_Queue);
                Display_Ram_To_Tm1629();
                GD5800_Play_Mucic_Of_Decoder_Process(Set_Voice_Play_Mode, voice_buff, voice_buff[0], voice_buff[7] & 0x0f, Set_Voice_Play_Time);
            }
        }
    }
}

#endif


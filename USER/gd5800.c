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

#ifdef USART2_GLOBAL

/**
  * @brief  This function is Initialization GD5800. set the cycle mode.
  * @param  None
  * @retval None
  */

void GD5800_Initialization(void)
{
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
  * @param  report_mode number
  * @retval None
  */

void GD5800_Play_Music_Of_Play_Music_Of_Mode_Within_NOT_REPORT_C(u8 key_type)
{
    if (key_type < 50)
    {
        Specify_Music_Play(key_type + QUXIAO);
    }
    else if (key_type == BAOJING_1)
    {
        Specify_Music_Play(BAOJING);
    }		
}

/**
  * @brief  This function is GD5800_Play_Music_Of_Play_Music_Of_Mode_More_Than_NOT_REPORT_C .
  * @param  report_mode number
  * @retval None
  */

void GD5800_Play_Music_Of_Play_Music_Of_Mode_More_Than_NOT_REPORT_C(u8 report_mode,u8 * number ,u8 key_type)
{
    if (key_type == BAOJING_1 || key_type == BAOJING-QUXIAO)
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
    if(call_type <= NOT_REPORT_C)
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
            GD5800_Play_Music_Of_Play_Music_Of_Mode_Within_NOT_REPORT_C(key_type);
        }
    }
    else
    {
        GD5800_Play_Music_Of_Play_Music_Of_Mode_More_Than_NOT_REPORT_C(report_mode,number,key_type);
    }
}

#endif


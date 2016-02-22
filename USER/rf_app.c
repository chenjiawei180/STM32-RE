 /******************************************************************************
  * @file       USER/rf_app.c
  * @author  cjw
  * @date     2016.2.25
  * @brief     This file contains the source of the RF.
  ******************************************************************************/
#include "rf_app.h"
#include "rf.h"
#include "menu.h"
#include "key.h"
#include "eeprom_24c.h"
#include "string.h"
#include "usart2.h"
#include "string.h"
#include "tm1629.h"

#if defined RF_GLOBAL

u8 Decoder_Call_Save_Line[800] = {0};

u32 Call_Code_Bak = 0;			//上一次处理的呼叫编码
u32 Call_Off_Time = 0;			//呼叫空闲时间

/**
  * @brief  This function is RF decoder process.
  * @param  None
  * @retval None
  */
  
void Decoder_Process(void)
{
    u8 fun_id;
    u32 call_code;
    if( 
        RF_ID && RF_Flag &&( ( M_index == STANDBY_MENU) 
                            || ( M_index == THREE_MENU_F1_E1_D1) 
                            || ( M_index == THREE_MENU_F1_E1_D2) 
                            || ( M_index == THREE_MENU_F1_E1_D3) 
                            || ( M_index == THREE_MENU_F1_E1_D4) 
                            || ( M_index == THREE_MENU_F1_E2_D1) 
                            || ( M_index == THREE_MENU_F1_E2_D2) 
                            || ( M_index == THREE_MENU_F1_E2_D3) 
                            || ( M_index == THREE_MENU_F1_E2_D4) 
                            || ( M_index == THREE_MENU_F1_E3_D1) 
                            || ( M_index == THREE_MENU_F1_E3_D2) 
                            || ( M_index == THREE_MENU_F1_E3_D3) 
                            || ( M_index == THREE_MENU_F1_E3_D4) 
                            || ( M_index == THREE_MENU_F1_E4_D1) 
                            || ( M_index == THREE_MENU_F1_E4_D2) 
                            || ( M_index == THREE_MENU_F1_E4_D3) 
                            || ( M_index == THREE_MENU_F1_E4_D4) 
        )
    )
    {
        if(RF_Same_Count > 0)
        {
            call_code = RF_ID ;
            //call_code = (RF_ID << 4) + RF_Fun;		//还原24位编码
            //fun_id = Call_Fun_Tab[RF_Fun];	//取默认呼叫功能      
            if(Call_Code_Bak != call_code)
            {
                Call_Code_Bak = call_code;
                switch(M_index)
                {
                    case STANDBY_MENU:Decoder_Standby();    break;

                    case THREE_MENU_F1_E1_D1:
                    case THREE_MENU_F1_E1_D2:
                    case THREE_MENU_F1_E1_D3:
                    case THREE_MENU_F1_E1_D4:Decoder_F1_E1();    break;

                    case THREE_MENU_F1_E2_D1:
                    case THREE_MENU_F1_E2_D2:
                    case THREE_MENU_F1_E2_D3:
                    case THREE_MENU_F1_E2_D4:Decoder_F1_E2();     break;

                    case THREE_MENU_F1_E3_D1:
                    case THREE_MENU_F1_E3_D2:
                    case THREE_MENU_F1_E3_D3:
                    case THREE_MENU_F1_E3_D4:Decoder_F1_E3();     break;

                    case THREE_MENU_F1_E4_D1:
                    case THREE_MENU_F1_E4_D2:
                    case THREE_MENU_F1_E4_D3:
                    case THREE_MENU_F1_E4_D4:Decoder_F1_E4();     break;

                    default:break;
                }
            }
        }
        RF_Flag = 0 ;
        RF_ID = 0;
    }
}

/**
  * @brief  This function is RF decoder Standby.
  * @param  None
  * @retval None
  */

void Decoder_Standby(void)
{
    u32 dat;
    u8 state;
    RF_def RFtmp;
    unsigned char decoder_temp_buff[8];
    dat = RF_ID;
    if(0)
    {
        
    }
    else
    {
        state = Find_RF_EEPROM(&RFtmp, dat);
        if (state)
        {
            decoder_temp_buff[0] = (u8)(dat & 0x0f);
            memcpy(decoder_temp_buff+1, RFtmp.region, 4);
            decoder_temp_buff[5] = (u8) ((dat>>16)&0xff);
            decoder_temp_buff[6] = (u8) ((dat>>8)&0xff);
            decoder_temp_buff[7] = (u8) ((dat)&0xff);
            Left_Buff_Add_To_Head_Of_Right_Buff(decoder_temp_buff,Decoder_Call_Save_Line);
            Display_Ram_To_Tm1629();
	 }
    }
}

/**
  * @brief  This function is RF decoder F1 E1.
  * @param  None
  * @retval None
  */

void Decoder_F1_E1(void)
{
    RF_def tmp;
    memcpy(tmp.region, Register_Call_Buff, 4);
    tmp.rf = RF_ID;
    if (!(Register_Call_Function(&tmp)))
    {
        Buff_Add_One(Register_Call_Buff);
        M_index = THREE_MENU_F1_E1_D1;
    }
}

/**
  * @brief  This function is RF decoder F1 E2.
  * @param  None
  * @retval None
  */

void Decoder_F1_E2(void)
{
    RF_def tmp;
    memcpy(tmp.region, Register_Host_Buff, 4);
    tmp.rf = RF_ID;
    if (!(Register_Host_Function(&tmp)))
    {
        Buff_Add_One(Register_Host_Buff);
        M_index = THREE_MENU_F1_E2_D1;
    }
}

/**
  * @brief  This function is RF decoder F1 E3.
  * @param  None
  * @retval None
  */

void Decoder_F1_E3(void)
{
    RF_def tmp;
    memcpy(tmp.region, Register_Alarm_Buff, 4);
    tmp.rf = RF_ID;
    if (!(Register_Alarm_Function(&tmp)))
    {
        Buff_Add_One(Register_Alarm_Buff);
        M_index = THREE_MENU_F1_E3_D1;
    }
}

/**
  * @brief  This function is RF decoder F1 E4.
  * @param  None
  * @retval None
  */

void Decoder_F1_E4(void)
{
    RF_def tmp;
    memcpy(tmp.region, Register_Cancel_Buff, 4);
    tmp.rf = RF_ID;
    if (!(Register_Cancel_Function(&tmp)))
    {
        Buff_Add_One(Register_Cancel_Buff);
        M_index = THREE_MENU_F1_E4_D1;
    }
}

/**
  * @brief  This function is Move eight byte up for buff.
  * @param  buff
  * @retval None
  */

void Buff_Move_Up_One_Position(unsigned char * buff)
{
    memcpy(buff,buff+8,8);
}

/**
  * @brief  This function is Move eight byte up for buff.
  * @param  buff
  * @retval None
  */
  
void Buff_Move_Down_One_Position(unsigned char * buff)
{
    memcpy(buff+8,buff,8);
}

/**
  * @brief  This function is find the last potision of Buff,the first byte is 0x00 per every 8 Byte.
  * @param  buff
  * @retval None
  */
  
u8 Return_End_Of_Buff(unsigned char * buff)
{
    u8 buff_index;
    u8 i;
    buff_index = Set_Call_Display_Number;
    for(i=0;i<Set_Call_Display_Number;i++)
    {
        if(*(buff+(i<<3)) == 0)    //cjw    ==0
        {
            return i;
        }
    }
    return buff_index;
}

/**
  * @brief  This function is move up from 2 to end.
  * @param  buff
  * @retval None
  */

void Buff_Move_Up_All_Position_And_Init(unsigned char * buff)
{
    u8 buff_index;
    u8 i;
    buff_index = Return_End_Of_Buff(buff); // find the last one on end of buff
    for(i=0;i<buff_index;i++)    //move up from 2 to end
    {
        Buff_Move_Up_One_Position(buff+(i<<3));
    }
    memset(buff+((i-1)<<3),0,8);//Init the last one 8Byte.
}

/**
  * @brief  This function is move down from 1 to end-1.
  * @param  buff
  * @retval None
  */

void Buff_Move_Down_All_Position_And_Init(unsigned char * buff)   
{
    u8 buff_index;
    signed char i;     //cjw 2016.2.22 u8 change signed char
    buff_index = Return_End_Of_Buff(buff); // find the last one on end of buff
    if(buff_index == Set_Call_Display_Number)    //the line is full.the last one is abort.  cjw 2016.2.22
    {
        buff_index--;
    }
    for(i=(buff_index-1);i>=0;i--)    //move up from 2 to end  // cjw 2016.2.22 i>=0
    {
        Buff_Move_Down_One_Position(buff+(i<<3));
    }
    memset(buff,0,8);//Init the last one 8Byte.
}

/**
  * @brief  This function is add a 8Byte to the head of right_buff.
  * @param  left_buff  right_buff
  * @retval None
  */

void Left_Buff_Add_To_Head_Of_Right_Buff(unsigned char * left_buff , unsigned char * right_buff)
{
    Buff_Move_Down_All_Position_And_Init(right_buff);
    memcpy(right_buff,left_buff,8);
}

/**
  * @brief  This function is add a 8Byte to the end of right_buff.
  * @param  left_buff  right_buff
  * @retval None
  */
  
void Left_Buff_Add_To_End_Of_Right_Buff(unsigned char * left_buff , unsigned char * right_buff)
{
    unsigned char index;
    index = Return_End_Of_Buff(right_buff);
    if(index == Set_Call_Display_Number)    //the line is full
    {
        Buff_Move_Up_All_Position_And_Init(right_buff);
        memcpy(right_buff+((Set_Call_Display_Number-1)<<3),left_buff,8);
    }
    else  
    {
        memcpy(right_buff+(index<<3),left_buff,8);
    }
}

/**
  * @brief  This function is move 48 byte from Decoder_Line to Display_Ram.
  * @param  left_buff  right_buff
  * @retval None
  */
  
void Decoder_Line_To_Display_Ram_For_Eight_Byte(unsigned char Display_Ram[48], unsigned char * Decoder_Line)
{
/*
*  @ the first byte is call mode;
*  @ the second to fifth bytes is  number. such as A000
*  @ the sixth to eighth bytes is  Decoder ID . such as 0xAA 0xBB 0XCC.
*/
    *(Display_Ram) = Dis_TAB[*(Decoder_Line + 1)];
    *(Display_Ram - 1) = Dis_TAB[*(Decoder_Line + 2)];
    *(Display_Ram - 2) = Dis_TAB[*(Decoder_Line + 3)];
    *(Display_Ram - 3) = Dis_TAB[*(Decoder_Line + 4)];

    switch (*(Decoder_Line + 7) & 0x0f)//显示呼叫类型
    {
        case 2:
            *(Display_Ram - 3) |= 0x80;
            *(Display_Ram - 2) |= 0x80;

        break;//7键
        case 3:
            *(Display_Ram - 3) |= 0x80;
            *(Display_Ram - 2) |= 0x80;
            *(Display_Ram - 1) |= 0x80;
            *(Display_Ram) |= 0x80;
        break;//78键
        case 4:
            *(Display_Ram - 3) |= 0x80;
            *(Display_Ram - 2) |= 0x80;
            *(Display_Ram - 1) |= 0x80;
        break;//6键
        case 7:
            *(Display_Ram - 2) |= 0x80;
        break;//678键
        case 8:
            *(Display_Ram - 3) |= 0x80;
        break;//5键
        default:break;
}
}

/**
  * @brief  This function is move the all display buff to the tm1629.large 12 call.
  * @param  None
  * @retval None
  */
  
void Display_Ram_To_Tm1629(void)
{
    Tm1629_Clear();
    if (*(Decoder_Call_Save_Line + 88) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[5] + 7, Decoder_Call_Save_Line + 88);
    if (*(Decoder_Call_Save_Line + 80) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[5] + 3, Decoder_Call_Save_Line + 80);
    if (*(Decoder_Call_Save_Line + 72) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[4] + 7, Decoder_Call_Save_Line + 72);
    if (*(Decoder_Call_Save_Line + 64) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[4] + 3, Decoder_Call_Save_Line + 64);
    if (*(Decoder_Call_Save_Line + 56) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[3] + 7, Decoder_Call_Save_Line + 56);
    if (*(Decoder_Call_Save_Line + 48) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[3] + 3, Decoder_Call_Save_Line + 48);
    if (*(Decoder_Call_Save_Line + 40) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[2] + 7, Decoder_Call_Save_Line + 40);
    if (*(Decoder_Call_Save_Line + 32) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[2] + 3, Decoder_Call_Save_Line + 32);
    if (*(Decoder_Call_Save_Line + 24) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[1] + 7, Decoder_Call_Save_Line + 24);
    if (*(Decoder_Call_Save_Line + 16) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[1] + 3, Decoder_Call_Save_Line + 16);
    if (*(Decoder_Call_Save_Line + 8) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[0] + 7, Decoder_Call_Save_Line + 8);
    if (*(Decoder_Call_Save_Line + 0) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[0] + 3, Decoder_Call_Save_Line);
    Tm1629_Display();
}

#endif /* RF_GLOBAL */ 


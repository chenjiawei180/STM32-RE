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
#include "sound_list.h"

#if defined RF_GLOBAL

u8 Decoder_Call_Save_Queue[800] = {0};

u32 Call_Code_Bak = 0;			//上一次处理的呼叫编码
u32 Call_Off_Time = 0;			//呼叫空闲时间

unsigned char single_key[16]   = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };//单键位设置存储数组
unsigned char multiple_key[16] = { 0x01, QUXIAO-QUXIAO, JIEZHANG-QUXIAO, DIANDANG-QUXIAO, JIUSHUI-QUXIAO, 0X01, 0x01, JIASHUI-QUXIAO, HUJIAO - QUXIAO, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };//多键位设置存储数组

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
                            || ( M_index == THREE_MENU_F8_E2_SET) 
                            || ( M_index == DECODER_MENU) 
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

                    case THREE_MENU_F8_E2_SET:Decoder_F8_E2();    break;

                    case DECODER_MENU:Decoder_Decoder_Menu();    break;

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
    u8 state=0;
    RF_def RFtmp;
    unsigned char decoder_temp_buff[8];
    dat = RF_ID;
    if(Decoder_Funciton_Of_Is_Or_Not_KeyBoard(dat))
    {
        Decoder_Function_Of_Return_Fun_Id(decoder_temp_buff,dat);
        Decoder_Function_Of_Assignment_For_KeyBoard(decoder_temp_buff,dat);
        if(decoder_temp_buff[0] != QUXIAO_1 && decoder_temp_buff[0])
        {
            if(!Decoder_Search_Buff_Is_Or_Not_In_Queue(decoder_temp_buff))
            {
                Left_Buff_Add_To_Head_Of_Right_Buff(decoder_temp_buff,Decoder_Call_Save_Queue);
                Display_Ram_To_Tm1629();
                M_index = DECODER_MENU;
            }
        }
        else
        {
             Decoder_Function_Of_Cancel(decoder_temp_buff);
        }
    }
    else
    {
        state = Find_RF_EEPROM(&RFtmp, dat);
        if (state)
        {
            Decoder_Function_Of_Assignment_For_Call_Id(decoder_temp_buff,dat,RFtmp,state);
            if(decoder_temp_buff[0] != QUXIAO_1 && decoder_temp_buff[0])
            {
                if(!Decoder_Search_Buff_Is_Or_Not_In_Queue(decoder_temp_buff))
                {
                    Left_Buff_Add_To_Head_Of_Right_Buff(decoder_temp_buff,Decoder_Call_Save_Queue);
                    Display_Ram_To_Tm1629();
                    M_index = DECODER_MENU;
                }
            }
            else
            {
                Decoder_Function_Of_Cancel(decoder_temp_buff);
            }
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
  * @brief  This function is RF decoder F8 E2.
  * @param  None
  * @retval None
  */

void Decoder_F8_E2(void)
{
    if (Set_Singal_Or_Multiple_Key_Mode == 1)
    {
        single_key[RF_ID & 0x0f] = Set_Key_Of_Call_Mode;
        //ISendStr(I2C_ADDRESS, SIN_KEY, single_key, 16);
    }
    else
    {
        multiple_key[RF_ID & 0x0f] = Set_Key_Of_Call_Mode;
        //ISendStr(I2C_ADDRESS, MUL_KEY, multiple_key, 16);
    }
}

/**
  * @brief  This function is RF decoder menu.
  * @param  None
  * @retval None
  */

void Decoder_Decoder_Menu(void)
{
    u32 dat;
    u8 state=0;
    RF_def RFtmp;
    unsigned char decoder_temp_buff[8];
    dat = RF_ID;
    if(Decoder_Funciton_Of_Is_Or_Not_KeyBoard(dat))
    {
        Decoder_Function_Of_Return_Fun_Id(decoder_temp_buff,dat);
        Decoder_Function_Of_Assignment_For_KeyBoard(decoder_temp_buff,dat);
        if(decoder_temp_buff[0] != QUXIAO_1 && decoder_temp_buff[0])
        {
            if(!Decoder_Search_Buff_Is_Or_Not_In_Queue(decoder_temp_buff))
            {
                if(Set_Call_Queue_Mode == 1)
                {
                    Left_Buff_Add_To_Head_Of_Right_Buff(decoder_temp_buff,Decoder_Call_Save_Queue);                    
                }
                else
                {
                     Left_Buff_Add_To_End_Of_Right_Buff(decoder_temp_buff,Decoder_Call_Save_Queue);                       
                }
                Display_Ram_To_Tm1629();
            }
        }
        else
        {
             Decoder_Function_Of_Cancel(decoder_temp_buff);
        }
    }
    else
    {
        state = Find_RF_EEPROM(&RFtmp, dat);
        if (state)
        {
            Decoder_Function_Of_Assignment_For_Call_Id(decoder_temp_buff,dat,RFtmp,state);
            if(decoder_temp_buff[0] != QUXIAO_1 && decoder_temp_buff[0])
            {
                if(!Decoder_Search_Buff_Is_Or_Not_In_Queue(decoder_temp_buff))
                {
                    if(Set_Call_Queue_Mode == 1)
                    {
                        Left_Buff_Add_To_Head_Of_Right_Buff(decoder_temp_buff,Decoder_Call_Save_Queue);                    
                    }
                    else
                    {
                         Left_Buff_Add_To_End_Of_Right_Buff(decoder_temp_buff,Decoder_Call_Save_Queue);                       
                   }
                    Display_Ram_To_Tm1629();
                }
            }
            else
            {
                Decoder_Function_Of_Cancel(decoder_temp_buff);
            }
        }
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
    for(i=0;i<buff_index-1;i++)    //move up from 2 to end
    {
        Buff_Move_Up_One_Position(buff+(i<<3));
    }
    memset(buff+(i<<3),0,8);//Init the last one 8Byte.
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
    if(index == Set_Call_Display_Number)    //the queuefull
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
    if (*(Decoder_Call_Save_Queue + 88) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[5] + 7, Decoder_Call_Save_Queue + 88);
    if (*(Decoder_Call_Save_Queue + 80) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[5] + 3, Decoder_Call_Save_Queue + 80);
    if (*(Decoder_Call_Save_Queue + 72) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[4] + 7, Decoder_Call_Save_Queue + 72);
    if (*(Decoder_Call_Save_Queue + 64) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[4] + 3, Decoder_Call_Save_Queue + 64);
    if (*(Decoder_Call_Save_Queue + 56) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[3] + 7, Decoder_Call_Save_Queue + 56);
    if (*(Decoder_Call_Save_Queue + 48) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[3] + 3, Decoder_Call_Save_Queue + 48);
    if (*(Decoder_Call_Save_Queue + 40) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[2] + 7, Decoder_Call_Save_Queue + 40);
    if (*(Decoder_Call_Save_Queue + 32) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[2] + 3, Decoder_Call_Save_Queue + 32);
    if (*(Decoder_Call_Save_Queue + 24) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[1] + 7, Decoder_Call_Save_Queue + 24);
    if (*(Decoder_Call_Save_Queue + 16) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[1] + 3, Decoder_Call_Save_Queue + 16);
    if (*(Decoder_Call_Save_Queue + 8) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[0] + 7, Decoder_Call_Save_Queue + 8);
    if (*(Decoder_Call_Save_Queue + 0) != 0)
        Decoder_Line_To_Display_Ram_For_Eight_Byte(Tm1629_Display_Ram[0] + 3, Decoder_Call_Save_Queue);
    Tm1629_Display();
}

/**
  * @brief  This function is Search call is or not in decoder queue.If the call in the queue.acconding to the Set_Call_Display_Number decide to how to do.
  * @param  buff
  * @retval 1 is exist . 0 is not exist
  */

u8 Decoder_Search_Buff_Is_Or_Not_In_Queue(unsigned char * buff)
{
    unsigned char index,index_end;
    signed char i;
    index = Return_End_Of_Buff(Decoder_Call_Save_Queue);
    index_end = index ;
    for(i=0;i<index;i++)
    {
       // if (*(buff + 1) == *(Decoder_Call_Save_Queue + (i << 3) + 1) && *(buff + 2) == *(Decoder_Call_Save_Queue + (i << 3) + 2) && *(buff + 2) == *(Decoder_Call_Save_Queue + (i << 3) + 2)&& *(buff + 2) == *(Decoder_Call_Save_Queue + (i << 3) + 2))
        if (*(volatile u32*)(buff+1) == *(volatile u32*)(Decoder_Call_Save_Queue + (i << 3) + 1) )
        {
            index = i;
            break;
        }
    }

    if(index !=index_end )
    {
        if(Set_Call_Queue_Mode== 1)
        {
            index--;
            for(i=index;i>=0;i--)
            {
                Buff_Move_Down_One_Position(Decoder_Call_Save_Queue + (i << 3) );
            }
            memcpy(Decoder_Call_Save_Queue,buff,8);
            Display_Ram_To_Tm1629();
            return 1;
        }
        else
        {
            return 1;
        }
    }
    return 0;        
}

/**
  * @brief  This function is function of cancel. delete the call.
  * @param  buff
  * @retval None
  */

void Decoder_Function_Of_Cancel(unsigned char * buff)
{
    unsigned char index,index_end;
    signed char i;
    index = Return_End_Of_Buff(Decoder_Call_Save_Queue);
    index_end = index ;
    for(i=0;i<index;i++)
    {
       //if (*(buff + 1) == *(Decoder_Call_Save_Queue + (i << 3) + 1) && *(buff + 2) == *(Decoder_Call_Save_Queue + (i << 3) + 2) && *(buff + 3) == *(Decoder_Call_Save_Queue + (i << 3) + 3)&& *(buff + 4) == *(Decoder_Call_Save_Queue + (i << 3) + 4))
        if (*(volatile u32*)(buff+1) == *(volatile u32*)(Decoder_Call_Save_Queue + (i << 3) + 1) )
        {
            index = i;
            break;
        }
    }

    if(index !=index_end )
    {
        for(i=index;i<index_end-1;i++)    //move up from 2 to end
        {
            Buff_Move_Up_One_Position(Decoder_Call_Save_Queue+(i<<3));
        }
        memset(Decoder_Call_Save_Queue+(i<<3),0,8);//Init the last one 8Byte.
    }      
    Display_Ram_To_Tm1629();
}

/**
  * @brief  This function is function of Esc. delete the first call.
  * @param  buff
  * @retval None
  */
	
void Decoder_Function_Of_Esc(void)
{
    Buff_Move_Up_All_Position_And_Init(Decoder_Call_Save_Queue);
    Display_Ram_To_Tm1629();
}

/**
  * @brief  This function is function of up. up cycle of queue.
  * @param  buff
  * @retval None
  */
	
void Decoder_Function_Of_Up(void)
{
    unsigned char buff_temp[8]={0};
    memcpy(buff_temp,Decoder_Call_Save_Queue,8);
    Buff_Move_Up_All_Position_And_Init(Decoder_Call_Save_Queue);
    Left_Buff_Add_To_End_Of_Right_Buff(buff_temp,Decoder_Call_Save_Queue);
    Display_Ram_To_Tm1629();
}

/**
  * @brief  This function is function of down. down cycle of queue.
  * @param  buff
  * @retval None
  */
	
void Decoder_Function_Of_Down(void)
{
    unsigned char buff_temp[8]={0};
    unsigned char index;
    index = Return_End_Of_Buff(Decoder_Call_Save_Queue);
    memcpy(buff_temp,Decoder_Call_Save_Queue+((index-1)<<3),8);
    memset(Decoder_Call_Save_Queue+((index-1)<<3),0,8);
    Left_Buff_Add_To_Head_Of_Right_Buff(buff_temp,Decoder_Call_Save_Queue);
    Display_Ram_To_Tm1629();
}

/**
  * @brief  This function is function of return fun id. fun id is in the multiple_key[] of singal_key[]
  * @param  buff  data
  * @retval None
  */
	
void Decoder_Function_Of_Return_Fun_Id(unsigned char * buff,u32 data)
{
    if (Set_Singal_Or_Multiple_Key_Mode == 1)  //为按键值
    {
        buff[0] = single_key[data & 0x0f];
    }
    else
    {
        buff[0] = multiple_key[data & 0x0f];
    }
}

/**
  * @brief  This function is judgment the call RFID is or not KeyBoard send.
  * @param  data
  * @retval 1 is keyboard . 0 is not.
  */
	
u8 Decoder_Funciton_Of_Is_Or_Not_KeyBoard(u32 data)
{
    //if  (   ((old2_RF_RECE_REG[2] & 0xf0) == 0x00 && ((old2_RF_RECE_REG[0] >> 4) == Two_Menu_F7_E1_temp) && (old2_RF_RECE_REG[0] >> 4) < 10)  ||  (Two_Menu_F7_E1_temp == 11 &&(old2_RF_RECE_REG[2] & 0xf0) == 0x00 )  )
    if( (  ( data & 0xf0)==0x00 &&  (data >>20)==Set_Two_Menu_F7_E1)  ||  ( Set_Two_Menu_F7_E1 == 11 &&  ( data & 0xf0)==0x00   )  )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
  * @brief  This function is data Assignment for decoder_temp at keyboard.
  * @param  buff  data
  * @retval None
  */
	
void Decoder_Function_Of_Assignment_For_KeyBoard(unsigned char * buff,u32 data)
{
    buff[1] = (u8) ((data>>16)&0x0f);	//为防区号
    buff[2] = (u8) ((data>>16)&0x0f);	//为3位组码第一位
    buff[3] = (u8) ((data>>8)&0xf0);	//为3位组码第二位
    buff[4] = (u8) ((data>>8)&0x0f);	//为3位组码第三位
    buff[5] = (u8) ((data>>16)&0xff);
    buff[6] = (u8) ((data>>8)&0xff);
    buff[7] = (u8) ((data)&0xff);
}

/**
  * @brief  This function is data Assignment for decoder_temp at Call Id.
  * @param  buff  data RFtmp
  * @retval None
  */

void Decoder_Function_Of_Assignment_For_Call_Id(unsigned char * buff,u32 data,RF_def RFtmp,u8 state)
{
    if(state == 1)
    {
        buff[0] = QUXIAO_1;
    }
    else if(state == 2)
    {
        buff[0] = BAOJING_1;
    }
    else
    {
        Decoder_Function_Of_Return_Fun_Id(buff,data);
    }
    memcpy(buff+1, RFtmp.region, 4);
    buff[5] = (u8) ((data>>16)&0xff);
    buff[6] = (u8) ((data>>8)&0xff);
    buff[7] = (u8) ((data)&0xff);
}

/*	
static void Decoder_Function_Of_Remove_Call_Time(unsigned char * buff);
static void Decoder_Function_Of_Cycle_Call_Time(unsigned char * buff);
*/

#endif /* RF_GLOBAL */ 


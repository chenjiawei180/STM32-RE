 /******************************************************************************
  * @file       USER/tm1629.c
  * @author  cjw
  * @date     2016.1.22
  * @brief     This file contains the source of the tm1629 drive.
  ******************************************************************************/
#include "tm1629.h"
#include "string.h"
#include "key.h"
#include "stm32_rtc.h"
#include "menu.h"

#if defined TM1629_GLOBAL

u8 const Dis_TAB[]={
                                0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,    //0~7  		
                                0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,    //8~F
    0x38,    //'L' 0x10
    0x00,    //' ' 0x11
    0x40,    //'-' 0x12
    0x76,    //'H' 0x13
    0x73,    //'P' 0x14
    0x50,    //'r' 0x15
    0x3E,    //'U' 0x16
    0x54,    //'n' 0x17
    0x5C,    //'o' 0x18
    0x80,    //'.' 0x19

    0x3F+0x80,0x06+0x80,0x5B+0x80,0x4F+0x80,0x66+0x80,0x6D+0x80,0x7D+0x80,0x07+0x80,
    0x7F+0x80,0x6F+0x80,0x77+0x80,0x7C+0x80,0x39+0x80,0x5E+0x80,0x79+0x80,0x71+0x80,//带小数点0-F,+0x1A

    0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,//段 abcdefgh，0-8，+0x2A
};

u8 Tm1629_Display_Ram[6][8]={0}; 

u8 Tm1629_Test_Time[8]={20,16,1,31,14,12,30,7};

/**
  * @brief  This function is Tm1629 delay.
  * @param  time
  * @retval None
  */

void Delayus(u16 time)
{
    u16 time_temp;
    while(time --)
    {
        time_temp = 2;
        while(--time_temp);
    }
}

/**
  * @brief  This function is Init Tm1629 GPIO .
  * @param  None
  * @retval None
  */

void Tm1629_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  This function is Tm1629_1 send data .
  * @param  Data
  * @retval None
  */

void WriteDataTo1629_1(unsigned char Data)
{
    unsigned int i;
    TM1629_STB1 = 0;
    for (i = 0; i<8; i++)
    {
        TM1629_CLK = 0;
        if (Data & 0x01)
        TM1629_DIN = 1;
        else
        TM1629_DIN = 0;
        Delayus(5);
        TM1629_CLK = 1;
        Data = Data >> 1;
    }
    TM1629_CLK = 0;
    TM1629_DIN = 0;
}

/**
  * @brief  This function is Tm1629_2 send data .
  * @param  Data
  * @retval None
  */

void WriteDataTo1629_2(unsigned char Data)
{
    unsigned int i;
    TM1629_STB2 = 0;
    for (i = 0; i<8; i++)
    {
        TM1629_CLK = 0;
        if (Data & 0x01)
        TM1629_DIN = 1;
        else
        TM1629_DIN = 0;
        Delayus(5);
        TM1629_CLK = 1;
        Data = Data >> 1;
    }
    TM1629_CLK = 0;
    TM1629_DIN = 0;
}

/**
  * @brief  This function is Tm1629_3 send data .
  * @param  Data
  * @retval None
  */

void WriteDataTo1629_3(unsigned char Data)
{
    unsigned int i;
    TM1629_STB3 = 0;
    for (i = 0; i<8; i++)
    {
        TM1629_CLK = 0;
        if (Data & 0x01)
        TM1629_DIN = 1;
        else
        TM1629_DIN = 0;
        Delayus(5);
        TM1629_CLK = 1;
        Data = Data >> 1;
    }
    TM1629_CLK = 0;
    TM1629_DIN = 0;
}

/**
  * @brief  This function is read data from 1629_1 .
  * @param  None
  * @retval Key_value
  */
u8 ReadDataFrom1629_1(void)
{
    unsigned char i,j;
    unsigned char Key_temp=0xff;
    unsigned char Key[4]={0};
    SendCommandTo1629_1(0x42);
    TM1629_DOUT=1;    //释放DIO
    for(j=0;j<4;j++)
    {
        for(i=0;i<8;i++)
        {
            TM1629_CLK=0;
            Key[j]=Key[j]>>1;
            TM1629_CLK=1;
            if(TM1629_DOUT)
            Key[j]=Key[j]|0x80;
            Delayus(5);
        }
        Delayus(5);

        if(Key[0]!=0 || Key[1]!=0 ||Key[2]!=0 ||Key[3]!=0)
        {
            switch (Key[0])
            {
                case (0x01): Key_temp = KEY_25;break;
                case (0x02): Key_temp = KEY_17;break;
                case (0x04): Key_temp = KEY_09;break;
                case (0x08): Key_temp = KEY_01;break;
                case (0x10): Key_temp = KEY_26;break;
                case (0x20): Key_temp = KEY_18;break;
                case (0x40): Key_temp = KEY_10;break;
                case (0x80): Key_temp = KEY_02;break;
                default:break;
            }
            switch (Key[1])
            {
                case (0x01): Key_temp = KEY_27;break;
                case (0x02): Key_temp = KEY_19;break;
                case (0x04): Key_temp = KEY_11;break;
                case (0x08): Key_temp = KEY_03;break;
                case (0x10): Key_temp = KEY_28;break;
                case (0x20): Key_temp = KEY_20;break;
                case (0x40): Key_temp = KEY_12;break;
                case (0x80): Key_temp = KEY_04;break;	
                default:break;
            }
            switch (Key[2])
            {
                case (0x01): Key_temp = KEY_29;break;
                case (0x02): Key_temp = KEY_21;break;
                case (0x04): Key_temp = KEY_13;break;
                case (0x08): Key_temp = KEY_05;break;
                case (0x10): Key_temp = KEY_30;break;
                case (0x20): Key_temp = KEY_22;break;
                case (0x40): Key_temp = KEY_14;break;
                case (0x80): Key_temp = KEY_06;break;	
                default:break;
            }
            switch (Key[3])
            {
                case (0x01): Key_temp = KEY_31;break;
                case (0x02): Key_temp = KEY_23;break;
                case (0x04): Key_temp = KEY_15;break;
                case (0x08): Key_temp = KEY_07;break;
                case (0x10): Key_temp = KEY_32;break;
                case (0x20): Key_temp = KEY_24;break;
                case (0x40): Key_temp = KEY_16;break;
                case (0x80): Key_temp = KEY_08;break;
                default:break;
            }
        }
    }
    TM1629_CLK=0;
    TM1629_DOUT=0;
    TM1629_STB1=1;
    return Key_temp;
}

/**
  * @brief  This function is Tm1629_1 send command .
  * @param  Data
  * @retval None
  */

void SendCommandTo1629_1(unsigned char Data)
{
    TM1629_STB1 = 1;
    Delayus(5);
    TM1629_STB1 = 0;
    WriteDataTo1629_1(Data);
}

/**
  * @brief  This function is Tm1629_2 send command .
  * @param  Data
  * @retval None
  */

void SendCommandTo1629_2(unsigned char Data)
{
    TM1629_STB2 = 1;
    Delayus(5);
    TM1629_STB2 = 0;
    WriteDataTo1629_2(Data);
}

/**
  * @brief  This function is Tm1629_3 send command .
  * @param  Data
  * @retval None
  */

void SendCommandTo1629_3(unsigned char Data)
{
    TM1629_STB3 = 1;
    Delayus(5);
    TM1629_STB3 = 0;
    WriteDataTo1629_3(Data);
}

/**
  * @brief  This function is Tm1629 display .
  * @param  Data
  * @retval None
  */

 void Tm1629_Display(void)
{
    unsigned char i;

    SendCommandTo1629_1(0x40);    //设置数据命令:普通模式、地址自增1，写数据到显存
    SendCommandTo1629_1(0xc0);    //设置显示地址命令：从00H开始
    for (i = 0; i<8; i++)    //发送16字节的显存数据
    {
    WriteDataTo1629_1(Tm1629_Display_Ram[0][i]);
    WriteDataTo1629_1(Tm1629_Display_Ram[1][i]);
    }
    SendCommandTo1629_1(0x88+Set_Display_Tube_Brightness);    //设置显示控制命令：打开显示，并设置为11/16.
    TM1629_STB1 = 1;

    SendCommandTo1629_2(0x40);    //设置数据命令:普通模式、地址自增1，写数据到显存
    SendCommandTo1629_2(0xc0);    //设置显示地址命令：从00H开始
    for (i = 0; i<8; i++)	//发送16字节的显存数据
    {
    WriteDataTo1629_2(Tm1629_Display_Ram[2][i]);
    WriteDataTo1629_2(Tm1629_Display_Ram[3][i]);
    }
    SendCommandTo1629_2(0x88+Set_Display_Tube_Brightness);    //设置显示控制命令：打开显示，并设置为11/16.
    TM1629_STB2 = 1;

    SendCommandTo1629_3(0x40);    //设置数据命令:普通模式、地址自增1，写数据到显存
    SendCommandTo1629_3(0xc0);    //设置显示地址命令：从00H开始
    for (i = 0; i<8; i++)  //发送16字节的显存数据
    {
    WriteDataTo1629_3(Tm1629_Display_Ram[4][i]);
    WriteDataTo1629_3(Tm1629_Display_Ram[5][i]);
    }
    SendCommandTo1629_3(0x88+Set_Display_Tube_Brightness);    //设置显示控制命令：打开显示，并设置为11/16.
    TM1629_STB3 = 1;

}

/**
  * @brief  This function is clear the tm1629 display buffer .
  * @param  Data
  * @retval None
  */

void Tm1629_Clear(void)
{
    memset(Tm1629_Display_Ram,0,48);
}

/**
  * @brief  This function is show menu number . such F1 F2 F3 ,Data is F number.
  * @param  Data
  * @retval None
  */

void Tm1629_Show_Fx(u8 Data)
{
    Tm1629_Clear();
    Tm1629_Display_Ram[0][1]=Dis_TAB[0x0F];
    Tm1629_Display_Ram[0][0]=Dis_TAB[Data];
    Tm1629_Display();
}

/**
  * @brief  This function is show menu number . such E1 E2 E3 ,Data is E number.
  * @param  Data
  * @retval None
  */

void Tm1629_Show_Ex(u8 Data)
{
    Tm1629_Clear();
    Tm1629_Display_Ram[0][1]=Dis_TAB[0x0E];
    Tm1629_Display_Ram[0][0]=Dis_TAB[Data];
    Tm1629_Display();
}

/**
  * @brief  This function is show RTC.
  * @param  Time
  * @retval None
  */
  
void Tm1629_Show_Time(unsigned char* Time)
{
    Tm1629_Clear();
    /* Show the Year on Left the digital tube.the rule is in china*/
    Tm1629_Display_Ram[1][3] = Dis_TAB[(*(Time+0))/10];
    Tm1629_Display_Ram[1][2] = Dis_TAB[(*(Time+0))%10];
    Tm1629_Display_Ram[1][1] = Dis_TAB[(*(Time+1))/10];
    Tm1629_Display_Ram[1][0] = Dis_TAB[(*(Time+1))%10];

    /*Show the Month on the digital tube*/
    Tm1629_Display_Ram[0][7] = Dis_TAB[(*(Time+2))/10];
    Tm1629_Display_Ram[0][6] = Dis_TAB[(*(Time+2))%10];

    /*Show the Day on the digital tube*/
    Tm1629_Display_Ram[0][5] = Dis_TAB[(*(Time+3))/10];
    Tm1629_Display_Ram[0][4] = Dis_TAB[(*(Time+3))%10];

    /*Show the Hour on the digital tube*/
    Tm1629_Display_Ram[0][3] = Dis_TAB[(*(Time+4))/10];
    Tm1629_Display_Ram[0][2] = Dis_TAB[(*(Time+4))%10];

    /*Show the Minute on the digital tube*/
    Tm1629_Display_Ram[0][1] = Dis_TAB[(*(Time+5))/10];
    Tm1629_Display_Ram[0][0] = Dis_TAB[(*(Time+5))%10];

    /*Show the Week on the digital tube*/
    Tm1629_Display_Ram[1][4] = Dis_TAB[*(Time+7)];

    Tm1629_Display();
}

/**
  * @brief  This function is blink the digital tube of Minute.
  * @param  None
  * @retval None
  */
  
void Tm1629_Blink_Time_Minute(void)
{
    static u8 Flag_Tm1629_Blink_Time_Minute = 0;
    if(Flag_Tm1629_Blink_Time_Minute == 0)
    {
        Tm1629_Display_Ram[0][1] =0;
        Tm1629_Display_Ram[0][0] =0;
        Tm1629_Display();
        Flag_Tm1629_Blink_Time_Minute = 1;
    }
    else
    {
        Tm1629_Show_Time(Tm1629_Test_Time);
        Flag_Tm1629_Blink_Time_Minute = 0;
    }
}

/**
  * @brief  This function is blink the digital tube of Hour.
  * @param  None
  * @retval None
  */
  
void Tm1629_Blink_Time_Hour(void)
{
    static u8 Flag_Tm1629_Blink_Time_Hour = 0;
    if(Flag_Tm1629_Blink_Time_Hour == 0)
    {
        Tm1629_Display_Ram[0][3] =0;
        Tm1629_Display_Ram[0][2] =0;
        Tm1629_Display();
        Flag_Tm1629_Blink_Time_Hour = 1;
    }
    else
    {
        Tm1629_Show_Time(Tm1629_Test_Time);
        Flag_Tm1629_Blink_Time_Hour = 0;
    }
}

/**
  * @brief  This function is blink the digital tube of Day.
  * @param  None
  * @retval None
  */
  
void Tm1629_Blink_Time_Day(void)
{
    static u8 Flag_Tm1629_Blink_Time_Day = 0;
    if(Flag_Tm1629_Blink_Time_Day == 0)
    {
        Tm1629_Display_Ram[0][5] =0;
        Tm1629_Display_Ram[0][4] =0;
        Tm1629_Display();
        Flag_Tm1629_Blink_Time_Day = 1;
    }
    else
    {
        Tm1629_Show_Time(Tm1629_Test_Time);
        Flag_Tm1629_Blink_Time_Day = 0;
    }
}

/**
  * @brief  This function is blink the digital tube of Month.
  * @param  None
  * @retval None
  */
  
void Tm1629_Blink_Time_Month(void)
{
    static u8 Flag_Tm1629_Blink_Time_Month = 0;
    if(Flag_Tm1629_Blink_Time_Month == 0)
    {
        Tm1629_Display_Ram[0][7] =0;
        Tm1629_Display_Ram[0][6] =0;
        Tm1629_Display();
        Flag_Tm1629_Blink_Time_Month = 1;
    }
    else
    {
        Tm1629_Show_Time(Tm1629_Test_Time);
        Flag_Tm1629_Blink_Time_Month = 0;
    }
}

/**
  * @brief  This function is blink the digital tube of Year.
  * @param  None
  * @retval None
  */
  
void Tm1629_Blink_Time_Year(void)
{
    static u8 Flag_Tm1629_Blink_Time_Year = 0;
    if(Flag_Tm1629_Blink_Time_Year == 0)
    {
        Tm1629_Display_Ram[1][1] =0;
        Tm1629_Display_Ram[1][0] =0;
        Tm1629_Display();
        Flag_Tm1629_Blink_Time_Year = 1;
    }
    else
    {
        Tm1629_Show_Time(Tm1629_Test_Time);
        Flag_Tm1629_Blink_Time_Year = 0;
    }
}

/**
  * @brief  This function is blink the digital tube of Year.
  * @param  None
  * @retval None
  */
  
void Tm1629_Blink_Time_Week(void)
{
    static u8 Flag_Tm1629_Blink_Time_Week = 0;
    if(Flag_Tm1629_Blink_Time_Week == 0)
    {
        Tm1629_Display_Ram[1][4] =0;
        Tm1629_Display();
        Flag_Tm1629_Blink_Time_Week = 1;
    }
    else
    {
        Tm1629_Show_Time(Tm1629_Test_Time);
        Flag_Tm1629_Blink_Time_Week = 0;
    }
}

/**
  * @brief  This function is show four bit number for digital tube on the right. 
  * @param  u8* Str
  * @retval None
  */
  
void Tm1629_Show_Four_Number(u8* Str)
{
    Tm1629_Clear();
    Tm1629_Display_Ram[0][3]=Dis_TAB[*(Str+0)];
    Tm1629_Display_Ram[0][2]=Dis_TAB[*(Str+1)];
    Tm1629_Display_Ram[0][1]=Dis_TAB[*(Str+2)];
    Tm1629_Display_Ram[0][0]=Dis_TAB[*(Str+3)];
    Tm1629_Display();
}

/**
  * @brief  This function is show two bit number for digital tube on the right. 
  * @param  Number
  * @retval None
  */
  
void Tm1629_Show_Two_Number(u8 Number)
{
    Tm1629_Clear();
    Tm1629_Display_Ram[0][1]=Dis_TAB[Number/10];
    Tm1629_Display_Ram[0][0]=Dis_TAB[Number%10];
    Tm1629_Display();
}

/**
  * @brief  This function is show one bit number for digital tube on the right. 
  * @param  Number
  * @retval None
  */
  
void Tm1629_Show_One_Number(u8 Number)
{
    Tm1629_Clear();
    Tm1629_Display_Ram[0][0]=Dis_TAB[Number%10];
    Tm1629_Display();
}

/**
  * @brief  This function is blink the first digital tube. 
  * @param  u8* Str
  * @retval None
  */
  
void Tm1629_Blink_One_Position(u8* Str)
{
    static u8 Flag_Tm1629_Blink_One_Position = 0;
    if(Flag_Tm1629_Blink_One_Position == 0)
    {
        Tm1629_Display_Ram[0][0] =0;
        Tm1629_Display();
        Flag_Tm1629_Blink_One_Position = 1;
    }
    else
    {
        Tm1629_Show_Four_Number(Str);
        Flag_Tm1629_Blink_One_Position = 0;
    }
}

/**
  * @brief  This function is blink the second digital tube. 
  * @param  u8* Str
  * @retval None
  */
  
void Tm1629_Blink_Two_Position(u8* Str)
{
    static u8 Flag_Tm1629_Blink_Two_Position = 0;
    if(Flag_Tm1629_Blink_Two_Position == 0)
    {
        Tm1629_Display_Ram[0][1] =0;
        Tm1629_Display();
        Flag_Tm1629_Blink_Two_Position = 1;
    }
    else
    {
        Tm1629_Show_Four_Number(Str);
        Flag_Tm1629_Blink_Two_Position = 0;
    }
}

/**
  * @brief  This function is blink the third digital tube. 
  * @param  u8* Str
  * @retval None
  */
  
void Tm1629_Blink_Three_Position(u8* Str)
{
    static u8 Flag_Tm1629_Blink_Three_Position = 0;
    if(Flag_Tm1629_Blink_Three_Position == 0)
    {
        Tm1629_Display_Ram[0][2] =0;
        Tm1629_Display();
        Flag_Tm1629_Blink_Three_Position = 1;
    }
    else
    {
        Tm1629_Show_Four_Number(Str);
        Flag_Tm1629_Blink_Three_Position = 0;
    }
}

/**
  * @brief  This function is blink the fouth digital tube. 
  * @param  u8* Str
  * @retval None
  */
  
void Tm1629_Blink_Four_Position(u8* Str)
{
    static u8 Flag_Tm1629_Blink_Four_Position = 0;
    if(Flag_Tm1629_Blink_Four_Position == 0)
    {
        Tm1629_Display_Ram[0][3] =0;
        Tm1629_Display();
        Flag_Tm1629_Blink_Four_Position = 1;
    }
    else
    {
        Tm1629_Show_Four_Number(Str);
        Flag_Tm1629_Blink_Four_Position = 0;
    }
}


#endif /* TM1629_GLOBAL */

 /******************************************************************************
  * @file       USER/transmit.c
  * @author  cjw
  * @date     2016.3.25
  * @brief     This file contains the source of the transmit drive.
  ******************************************************************************/
#include "transmit.h"
#include "rf_app.h"

#ifdef STM32_TRANSMIT

u8 Transmit_Count = 0;
u8 Transmit_Count_End = 0;
u8 Transmit_Flag = 0;

u8   Transmit_Data_FANGQU=0;
u8   Transmit_Data_Set_FANGQU=0;
u16 Transmit_Data=0;
u16 Queue_Number=1;
u8 Transmit_Confirm_Key_Press=0;

/**
  * @brief  This function is Transmit_IO_Init.
  * @param  None
  * @retval None
  */
  
void Transmit_IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);	   
}

/**
  * @brief  This function is Transmit_Timer_Config.
  * @param  None
  * @retval None
  */
  
void Transmit_Timer_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
    //TIM_DeInit(TIM2);
	
    TIM_TimeBaseStructure.TIM_Period=520;
	
    TIM_TimeBaseStructure.TIM_Prescaler= 23;
	
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM3, ENABLE);																		
    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE);		
}

/**
  * @brief  This function is Transmit_NVIC_Configuration.
  * @param  None
  * @retval None
  */
  
void Transmit_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
												
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  This function is TIM3_IRQHandler.
  * @param  None
  * @retval None
  */
  
void TIM3_IRQHandler(void)
{
    if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
    {	
        TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update); 
	 Transmit_Count++;
	 if(Transmit_Flag == 1)
	     Transmit_Pin = 1;
	 else
	     Transmit_Pin = 0;
    }		
}

/**
  * @brief  This function is Transmit_Data.
  * @param  data
  * @retval None
  */
  
void Transmit_Function(u32 data)
{
    u8 i;

    EXTI->PR=1<<15;    
    EXTI->IMR&=~(1<<15); // close the exti 15
	
    Transmit_Count_End = 1;
    Transmit_Flag = 1;
    Transmit_Count = 0;
    do{}while(Transmit_Count < Transmit_Count_End);

    Transmit_Count_End = 31;
    Transmit_Flag = 0;
    Transmit_Count = 0;
    do{}while(Transmit_Count < Transmit_Count_End);

    for(i=0;i<24;i++)
    {
        if(data & 0x800000ul)  //判断二进制数高位为1还是0
        {
            Transmit_Count_End = 3;  //1为宽的高电平
            Transmit_Flag = 1;
        }
        else
        {
            Transmit_Count_End = 1;   //0为窄的高电平
            Transmit_Flag = 1;
        }	
        Transmit_Count = 0;
        do{}while(Transmit_Count < Transmit_Count_End);
		
        Transmit_Count_End = 4 - Transmit_Count_End;
        Transmit_Flag = 0;
        Transmit_Count = 0;
        do{}while( Transmit_Count < Transmit_Count_End);
        data = data<<1;
    }
	
    EXTI->IMR|=1<<15; //open the exti 15
}

/**
  * @brief  This function is Transmit_Data.
  * @param  data
  * @retval None
  */
  
void Transmit_Set_Data(unsigned char data)
{
    if(Transmit_Data_Set_FANGQU == 1)
    {
        Transmit_Data_FANGQU =data;
        Transmit_Data_Set_FANGQU = 0;
    }
    else
    {
        if(Transmit_Confirm_Key_Press == 1)
        {
            Transmit_Data = 0;
            Transmit_Confirm_Key_Press = 0;
        }

        if(Transmit_Data < 100)
        {
            Transmit_Data =Transmit_Data*10 + data;
        }
    }
    Display_Ram_To_Tm1629();
}

/**
  * @brief  This function is Transmit_Number_Of_Set.
  * @param  None
  * @retval None
  */

void Transmit_Number_Of_Set(void)
{
    u8 Transmit_buf[3] = {0};
    u32 Transmit_Data_Temp=0;
    Transmit_buf[0]= ((Transmit_Data_FANGQU)<<4)|((Transmit_Data%1000)/100);
    Transmit_buf[1]= (((Transmit_Data%100)/10)<<4)|(Transmit_Data%10);	
    Transmit_buf[2]= 0x08;
    Transmit_Data_Temp = Transmit_buf[0]<<16 | Transmit_buf[1] << 8 | Transmit_buf[2];
    Transmit_Function(Transmit_Data_Temp);
    Transmit_Function(Transmit_Data_Temp);
    Transmit_Function(Transmit_Data_Temp);
    Transmit_Function(Transmit_Data_Temp);
    Transmit_Function(Transmit_Data_Temp);
    Transmit_Function(Transmit_Data_Temp);
}

/**
  * @brief  This function is Transmit_Number_Of_Reciver_Add_One.
  * @param  None
  * @retval None
  */

void Transmit_Number_Of_Reciver_Add_One(void)
{
    u8 Transmit_buf[3] = {0};
    u32 Transmit_Data_Temp=0;
    Transmit_Data = *(Decoder_Call_Save_Queue+2)*100+*(Decoder_Call_Save_Queue+3)*10+*(Decoder_Call_Save_Queue+4)+1;
    Transmit_buf[0]= ((Transmit_Data_FANGQU)<<4)|((Transmit_Data%1000)/100);
    Transmit_buf[1]= (((Transmit_Data%100)/10)<<4)|(Transmit_Data%10);	
    Transmit_buf[2]= 0x08;
    Transmit_Data_Temp = Transmit_buf[0]<<16 | Transmit_buf[1] << 8 | Transmit_buf[2];
    Transmit_Function(Transmit_Data_Temp);
    Transmit_Function(Transmit_Data_Temp);
    Transmit_Function(Transmit_Data_Temp);
    Transmit_Function(Transmit_Data_Temp);
    Transmit_Function(Transmit_Data_Temp);
    Transmit_Function(Transmit_Data_Temp);
}

/**
  * @brief  This function is Transmit_Number_Of_Reciver_Sub_One.
  * @param  None
  * @retval None
  */
  
void Transmit_Number_Of_Reciver_Sub_One(void)
{
    u8 Transmit_buf[3] = {0};
    u32 Transmit_Data_Temp=0;
    if(*(Decoder_Call_Save_Queue + 0) != 0 )
    {
        Transmit_Data = *(Decoder_Call_Save_Queue+2)*100+*(Decoder_Call_Save_Queue+3)*10+*(Decoder_Call_Save_Queue+4)-1;
        Transmit_buf[0]= ((Transmit_Data_FANGQU)<<4)|((Transmit_Data%1000)/100);
        Transmit_buf[1]= (((Transmit_Data%100)/10)<<4)|(Transmit_Data%10);	
        Transmit_buf[2]= 0x08;
        Transmit_Data_Temp = Transmit_buf[0]<<16 | Transmit_buf[1] << 8 | Transmit_buf[2];
        Transmit_Function(Transmit_Data_Temp);
        Transmit_Function(Transmit_Data_Temp);
        Transmit_Function(Transmit_Data_Temp);
        Transmit_Function(Transmit_Data_Temp);
        Transmit_Function(Transmit_Data_Temp);
        Transmit_Function(Transmit_Data_Temp);
    }
}

#endif


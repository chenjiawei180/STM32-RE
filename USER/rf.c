 /******************************************************************************
  * @file       USER/rf.c
  * @author  cjw
  * @date     2016.1.21
  * @brief     This file contains the source of the RF.
  ******************************************************************************/
#include "rf.h"
#include "rf_app.h"
#include "usart2.h"
#include "menu.h"

#if defined RF_GLOBAL

u8 RF_Fun = 0;    //呼叫键值
u32 RF_ID = 0;    //呼叫ID号
u8 RF_Fun_Bak = 0;    //历史呼叫键值
u32 RF_ID_Bak = 0;    //历史呼叫ID号
u32 RF_Temp = 0;    //接收过程移位变量
u32 RF_Temp_Bak = 0;    //上一次接收结果
u8 RF_Count = 0;    //接收位数计数
u8 RF_Flag = 0;    //新信息接收标志
u8 RF_Same_Count = 0;    //重复相同接收次数


/**
  * @brief  This function is GPIOA_PIN15 interrupt process.
  * @param  None
  * @retval None
  */
  
void EXTI15_10_IRQHandler(void)//
{
    u16 tim_value;
    u8 flag;
    flag = 0;
    if(RF_PIN_LEVEL != 0)    //引脚低电平，说明模块输出信号为高电平，信号上升沿中断
    {
        flag = 1;
    }
    tim_value =  TIM2 ->CNT;
    TIM2 ->CNT = 0;

    if(flag)
    {
        if((tim_value > SYN_WIDTH))    //大于3mS判断为结束码
        {
            if(RF_Count > 24)
            {
                RF_Temp >>= 1;    //去除同步码的高电平
                RF_Temp &= 0xFFFFFF;    //取最后24位

                if((RF_Temp & 0xFFFFF0) != 0x000000)    //编码不为0
                {
                    if(RF_Temp == RF_Temp_Bak)
                    {
                        RF_Same_Count ++;    //相同呼叫计数
                        if( RF_Same_Count > 250 )
                        {
                            RF_Same_Count = 250 ;
                        }
/* key mode mask                        
                        if(Key_MODE==1)    //单按键模式
                        {
                            RF_ID=RF_Temp;
                            RF_Fun=8;    //呼叫
                        }
                        else
                        {
                            RF_ID = (RF_Temp >> 4) ;    //置呼叫编码ID
                            RF_Fun = (RF_Temp ) & 0xF;	//置呼叫按键编码
                        }
*/
                        RF_ID=RF_Temp; //cjw 2016.1.21 for main while(1) test
                        RF_Flag = 1;//置接收标志
                        Return_Standby_Time_Count = 0;
                    }
                    else
                    {
                        RF_Same_Count = 0;
                        RF_Temp_Bak = RF_Temp;
/* key mode mask 
                        if(Key_MODE==1)//单按键模式
                        {
                            RF_ID=RF_Temp;
                            RF_Fun=2;//呼叫
                        }
                        else
                        {
                            RF_ID = (RF_Temp >> 4) ;    //置呼叫编码ID
                            RF_Fun = (RF_Temp ) & 0xF;    //置呼叫按键编码
                        }
*/
                        RF_ID=RF_Temp; //cjw 2016.1.21 for main while(1) test
                        RF_Flag = 1;    //置接收标志
                        Return_Standby_Time_Count = 0;
                    }
                    Call_Off_Time = 0;
                }
                RF_Temp = 0;
                RF_Count = 0;
            }
        }  
    }
    else
    {
        if((tim_value < MIN_WIDTH)||(tim_value > MAX_WIDTH))    //过小或过大的作丢弃处理
        {
            RF_Count = 0;
            RF_Temp = 0;
            RF_Same_Count = 0;
        }
        else
        {  
            RF_Temp <<= 1;
            if(tim_value > DATA_H_WIDTH) RF_Temp |=0x01;    //高电平大于分界值，判断为1
            RF_Count ++;
        }
    }

    EXTI_ClearITPendingBit(EXTI_Line15);	//清中断标志
}



/**
  * @brief  This function is init RF GPIOA_PIN15.
  * @param  None
  * @retval None
  */

void RF_IO_Init(void)
{
    GPIO_InitTypeDef  GPIO_Struct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_Struct.GPIO_Pin = GPIO_Pin_15;
    GPIO_Struct.GPIO_Mode= GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_Struct);
}

/**
  * @brief  This function is init anything of RF module.
  *   It contain the timer,EXTI of GPIOA_PIN15,interrupt.
  * @param  None
  * @retval None
  */

void RF_Config(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    u16 PrescalerValue = 0;

    RF_IO_Init();  //RF输入引脚端口初始化
    //*******************************************************中断配置
    /* Enable AFIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* Connect EXTI1 Line to PA.1 pin */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);

    /* Configure EXTI1 line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line15;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//双跳变沿
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set EXTI1 Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //***************************************************定时器配置
    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    /* Compute the prescaler value */
    PrescalerValue = (u16) (SystemCoreClock / 100000) - 1;//10US
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 65535;   //计数范围最大值
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;   //时钟预分频计数
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //向上计数

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    /* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);
} 

#endif

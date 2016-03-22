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

u8 RF_Fun = 0;    //���м�ֵ
u32 RF_ID = 0;    //����ID��
u8 RF_Fun_Bak = 0;    //��ʷ���м�ֵ
u32 RF_ID_Bak = 0;    //��ʷ����ID��
u32 RF_Temp = 0;    //���չ�����λ����
u32 RF_Temp_Bak = 0;    //��һ�ν��ս��
u8 RF_Count = 0;    //����λ������
u8 RF_Flag = 0;    //����Ϣ���ձ�־
u8 RF_Same_Count = 0;    //�ظ���ͬ���մ���


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
    if(RF_PIN_LEVEL != 0)    //���ŵ͵�ƽ��˵��ģ������ź�Ϊ�ߵ�ƽ���ź��������ж�
    {
        flag = 1;
    }
    tim_value =  TIM2 ->CNT;
    TIM2 ->CNT = 0;

    if(flag)
    {
        if((tim_value > SYN_WIDTH))    //����3mS�ж�Ϊ������
        {
            if(RF_Count > 24)
            {
                RF_Temp >>= 1;    //ȥ��ͬ����ĸߵ�ƽ
                RF_Temp &= 0xFFFFFF;    //ȡ���24λ

                if((RF_Temp & 0xFFFFF0) != 0x000000)    //���벻Ϊ0
                {
                    if(RF_Temp == RF_Temp_Bak)
                    {
                        RF_Same_Count ++;    //��ͬ���м���
                        if( RF_Same_Count > 250 )
                        {
                            RF_Same_Count = 250 ;
                        }
/* key mode mask                        
                        if(Key_MODE==1)    //������ģʽ
                        {
                            RF_ID=RF_Temp;
                            RF_Fun=8;    //����
                        }
                        else
                        {
                            RF_ID = (RF_Temp >> 4) ;    //�ú��б���ID
                            RF_Fun = (RF_Temp ) & 0xF;	//�ú��а�������
                        }
*/
                        RF_ID=RF_Temp; //cjw 2016.1.21 for main while(1) test
                        RF_Flag = 1;//�ý��ձ�־
                        Return_Standby_Time_Count = 0;
                    }
                    else
                    {
                        RF_Same_Count = 0;
                        RF_Temp_Bak = RF_Temp;
/* key mode mask 
                        if(Key_MODE==1)//������ģʽ
                        {
                            RF_ID=RF_Temp;
                            RF_Fun=2;//����
                        }
                        else
                        {
                            RF_ID = (RF_Temp >> 4) ;    //�ú��б���ID
                            RF_Fun = (RF_Temp ) & 0xF;    //�ú��а�������
                        }
*/
                        RF_ID=RF_Temp; //cjw 2016.1.21 for main while(1) test
                        RF_Flag = 1;    //�ý��ձ�־
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
        if((tim_value < MIN_WIDTH)||(tim_value > MAX_WIDTH))    //��С����������������
        {
            RF_Count = 0;
            RF_Temp = 0;
            RF_Same_Count = 0;
        }
        else
        {  
            RF_Temp <<= 1;
            if(tim_value > DATA_H_WIDTH) RF_Temp |=0x01;    //�ߵ�ƽ���ڷֽ�ֵ���ж�Ϊ1
            RF_Count ++;
        }
    }

    EXTI_ClearITPendingBit(EXTI_Line15);	//���жϱ�־
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

    RF_IO_Init();  //RF�������Ŷ˿ڳ�ʼ��
    //*******************************************************�ж�����
    /* Enable AFIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* Connect EXTI1 Line to PA.1 pin */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);

    /* Configure EXTI1 line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line15;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//˫������
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set EXTI1 Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //***************************************************��ʱ������
    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    /* Compute the prescaler value */
    PrescalerValue = (u16) (SystemCoreClock / 100000) - 1;//10US
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 65535;   //������Χ���ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;   //ʱ��Ԥ��Ƶ����
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;   //ʱ�ӷ�Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //���ϼ���

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    /* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);
} 

#endif

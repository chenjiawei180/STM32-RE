 /******************************************************************************
  * @file       USER/usart1.c
  * @author  cjw
  * @date     2016.1.19
  * @brief     This file contains the source of the usart1 drive.
  ******************************************************************************/
#include "usart1.h"

#if defined USART1_GOLBAL

/**
  * @brief  This function is init usart1.
  * @param  None
  * @retval None
  */
  
void GD5800_USART1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* config USART1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    /* USART1 GPIO config */
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART1 mode config */
    USART_InitStructure.USART_BaudRate = 9600;//115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  This function is set the interrupt class of usart1.
  * @param  None
  * @retval None
  */
  
void GD5800_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  This function is usart1 send data.
  * @param  u8 Data
  * @retval None
  */

void USART1_SendByte(u8 Date)
{
    USART_SendData(USART1, Date);
    while (!(USART1->SR & USART_FLAG_TXE));
}

/**
  * @brief  This function is usart1 interrupt process.
  * @param  None
  * @retval None
  */

void USART1_IRQHandler(void)
{
    //u8 c;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        //c=USART1->DR;
        USART_ClearFlag(USART1,USART_FLAG_RXNE);
    } 
}

#endif  /* USART1_GOLBAL */

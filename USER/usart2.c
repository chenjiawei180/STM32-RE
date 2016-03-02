 /******************************************************************************
  * @file       USER/usart2.c
  * @author  cjw
  * @date     2016.1.19
  * @brief     This file contains the source of the usart2 drive.
  ******************************************************************************/
#include "usart2.h"

#if defined USART2_GLOBAL

/**
  * @brief  This function is init usart2.
  * @param  None
  * @retval None
  */
void CH340_USART2_Config(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* config USART2 clock and GPIOA clock*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* USART2 GPIO config */
    /* Configure USART2 Tx (PA.02) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
    /* Configure USART2 Rx (PA.03) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART2 mode config */
    USART_InitStructure.USART_BaudRate = 9600;//115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART2, ENABLE);
}


/**
  * @brief  This function is set the interrupt class of usart2.
  * @param  None
  * @retval None
  */
void CH340_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  This function is send the char ch to the index of FILE *f.
  * @param  char of ch,index of FILE *f.
  * @retval char of ch.
  */

int fputc(int ch, FILE *f)
{
    /* 将Printf内容发往串口 */
    USART_SendData(USART2, (unsigned char) ch);
    while (!(USART2->SR & USART_FLAG_TXE));

    return (ch);
}

/**
  * @brief  This function is usart2 send data.
  * @param  None
  * @retval None
  */
  
void USART2_SendByte(u8 Date)
{
    printf("%c",Date); 
}

/**
  * @brief  This function is usart2 interrupt process.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
    //u8 c=0;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        //c=USART1->DR;
        USART_ClearFlag(USART2,USART_FLAG_RXNE);
    }
} 


#endif  /* USART2_GLOBAL  */


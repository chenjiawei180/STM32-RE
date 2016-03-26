 /******************************************************************************
  * @file       USER/usart2.c
  * @author  cjw
  * @date     2016.1.19
  * @brief     This file contains the source of the usart2 drive.
  ******************************************************************************/
#include "usart2.h"
#include "menu.h"
#include "tm1629.h"
#include "transmit.h"

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

#ifdef STM32_RECIVER

/**
  * @brief  This function is Mcu_Send_Call_To_Computer
  * @param  call_type number key_type
  * @retval None
  */
  
void Mcu_Send_Call_To_Computer(unsigned char call_type, unsigned char* number,unsigned char key_type)
{
	unsigned char temp1=0,temp2=0,key=0;

	unsigned char sec = 0, hour = 0, min = 0;
	to_tm(RTC_GetCounter()+28800, &systmtime);
       Struct_Time_To_Buff_Time(&systmtime, Tm1629_Test_Time);

	USART2_SendByte(0xaa);
	USART2_SendByte(0x42);
	USART2_SendByte(0x01);
	USART2_SendByte(0x01);
	USART2_SendByte(0x01);
	USART2_SendByte(call_type);//呼叫或者取消  91 92
	USART2_SendByte(0xff);

	temp1 = *(number+1) << 4 | *(number + 2);
	temp2 = *(number+3) << 4 | *(number + 4);
	USART2_SendByte(temp1);//号码
	USART2_SendByte(temp2);//号码

	switch (key_type)
	{
	case 1:
		key = 0x01;
		USART2_SendByte(0x01);
		break;	
	case 2:
		key = 0x0d; 
		USART2_SendByte(0x0d);
		break;
	case 3:
		key = 0x03;
		USART2_SendByte(0x03);
		break;
	case 4:
		key = 0x0b; 
		USART2_SendByte(0x0b); 
		break;
	case 5:
		key = 0x05;
		USART2_SendByte(0x05);
		break;
	case 6:
		key = 0x06;
		USART2_SendByte(0x06);
		break;
	case 7:
		key = 0x07;
		USART2_SendByte(0x07);
		break;
	case 8:
		key = 0x0c;
		USART2_SendByte(0x0c); 
		break;
	case 9:
		key = 0x09;
		USART2_SendByte(0x09);
		break;
	case 10:
		key = 0x0A;
		USART2_SendByte(0x0A);
		break;
	case 11:
		key = 0x04;
		USART2_SendByte(0x04);
		break;
	case 12:
		key = 0x08;
		USART2_SendByte(0x08);
		break;
	case 13:
		key = 0x02;
		USART2_SendByte(0x02);
		break;
	case 14:
		key = 0x0e;
		USART2_SendByte(0x0e);
		break;
	case 15:
		key = 0x0f;
		USART2_SendByte(0x0f);
		break;
	default:
		key = 0x00;
		USART2_SendByte(0x00);
		break;
	}
	//Usart1_SendData(key_type);
	//键值
	

	USART2_SendByte(0);
	USART2_SendByte(0);

	hour  =  ((Tm1629_Test_Time[4] / 10)*6) + Tm1629_Test_Time[4];
	min =    ((Tm1629_Test_Time[5] / 10)*6) + Tm1629_Test_Time[5];
	sec  =   ((Tm1629_Test_Time[6] / 10)*6) + Tm1629_Test_Time[6];

	USART2_SendByte(hour);
	USART2_SendByte(min);
	USART2_SendByte(sec);

	USART2_SendByte(0x1EE + call_type + temp1 + temp2 + key + hour + min + sec);//校验


	USART2_SendByte(0x55);
}
#endif

#ifdef STM32_TRANSMIT

/**
  * @brief  This function is Printer58mm_Print
  * @param  call_type number key_type
  * @retval None
  */
  
void Printer58mm_Print(void)
{
    unsigned char temp=0;

    USART2_SendByte(0x1B);      /*send command to printer for initialization*/
    USART2_SendByte(0x40);     //初始化

    USART2_SendByte(0x1B);   //设置居中
    USART2_SendByte(0x61);
    USART2_SendByte(0x01);

    USART2_SendByte(0x57);   //welcome
    USART2_SendByte(0x65);
    USART2_SendByte(0x6C);
    USART2_SendByte(0x63);
    USART2_SendByte(0x6F);
    USART2_SendByte(0x6D);
    USART2_SendByte(0x65);

    USART2_SendByte(0x0A); //换行
    USART2_SendByte(0x0A); //换行

    USART2_SendByte(0x1B); //倍高
    USART2_SendByte(0x21);
    USART2_SendByte(0x10);

    USART2_SendByte(0x4E); //No. 
    USART2_SendByte(0x6F);
    USART2_SendByte(0x2E);
    USART2_SendByte(0x20);

    USART2_SendByte(Queue_Number/100+0x30);
    USART2_SendByte(0x20);
    USART2_SendByte(Queue_Number%100/10+0x30);
    USART2_SendByte(0x20);	
    USART2_SendByte(Queue_Number%10+0x30);
    USART2_SendByte(0x20);

    USART2_SendByte(0x1B);//取消倍高
    USART2_SendByte(0x21);
    USART2_SendByte(0x00);

    USART2_SendByte(0x0A);//换行
    USART2_SendByte(0x0A);//换行

    USART2_SendByte(0x77);//wait number 
    USART2_SendByte(0x61);
    USART2_SendByte(0x69);
    USART2_SendByte(0x74);
    USART2_SendByte(0x20);
    USART2_SendByte(0x6E);
    USART2_SendByte(0x75);
    USART2_SendByte(0x6D);
    USART2_SendByte(0x62);
    USART2_SendByte(0x65);
    USART2_SendByte(0x72);
    USART2_SendByte(0x3A);

    if(Transmit_Data > (Queue_Number-1))
    {
        temp = 0;
    }
    else
    {
        temp=Queue_Number - Transmit_Data-1;
    }
    USART2_SendByte(temp/100+0x30);
    USART2_SendByte(temp%100/10+0x30);
    USART2_SendByte(temp%10+0x30);


    USART2_SendByte(0x0A);//换行

    USART2_SendByte(0x1B); //进纸
    USART2_SendByte(0x4A);
    USART2_SendByte(0x21);

    USART2_SendByte(0x1B);//进纸
    USART2_SendByte(0x4A);
    USART2_SendByte(0x21);

    USART2_SendByte(0x1B);//进纸
    USART2_SendByte(0x4A);
    USART2_SendByte(0x21);

    USART2_SendByte(0x1B);//进纸
    USART2_SendByte(0x4A);
    USART2_SendByte(0x21);

    USART2_SendByte(0x1B);//进纸
    USART2_SendByte(0x4A);
    USART2_SendByte(0x21);	
}
#endif

#endif  /* USART2_GLOBAL  */


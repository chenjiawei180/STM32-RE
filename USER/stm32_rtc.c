 /******************************************************************************
  * @file       USER/stm32_rtc.c
  * @author  cjw
  * @date     2016.3.18
  * @brief     This file contains the source of the RTC.
  ******************************************************************************/
  
#include "stm32_rtc.h"
#include "usart2.h"
#include "menu.h"

#ifdef RTC_GLOBAL

struct rtc_time systmtime;

/**
  * @brief  This function is RTC_NVIC_Config.
  * @param  None
  * @retval None
  */

void RTC_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  This function is RTC_CheckAndConfig.
  * @param  struct rtc_time *tm
  * @retval None
  */


void RTC_CheckAndConfig(struct rtc_time *tm)
{
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
    {
        //printf("\r\n\r\n RTC not yet configured....");

        /* RTC Configuration */
        RTC_Configuration();

        //printf("\r\n\r\n RTC configured....");

        /* Adjust time by users typed on the hyperterminal */
        Time_Adjust(tm);

        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
    }
    else
    {
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
            //printf("\r\n\r\n Power On Reset occurred....");
        }
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
            //printf("\r\n\r\n External Reset occurred....");
        }

        //printf("\r\n No need to configure RTC....");

	 /* Enable PWR and BKP clocks */
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	 /* Allow access to BKP Domain */
	 PWR_BackupAccessCmd(ENABLE);

        RTC_WaitForSynchro();

        RTC_ITConfig(RTC_IT_SEC, ENABLE);

        RTC_WaitForLastTask();
    }
    
#ifdef RTCClockOutput_Enable
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Disable the Tamper Pin */
    BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
    functionality must be disabled */

    /* Enable RTC Clock Output on Tamper Pin */
    BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
#endif

    /* Clear reset flags */
    RCC_ClearFlag();
}

/**
  * @brief  This function is RTC_Configuration.
  * @param  None
  * @retval None
  */
  
void RTC_Configuration(void)
{
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	
	/* Reset Backup Domain */
	BKP_DeInit();
	
	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{}
	
	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	
	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Enable the RTC Second */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

#endif 

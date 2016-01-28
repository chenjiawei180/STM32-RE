 /******************************************************************************
  * @file       USER/set_system_clock.c
  * @author  cjw
  * @date     2016.1.18
  * @brief     This file contains the source of the set system clock funtion and set RTC clock funtion.
  ******************************************************************************/
#include "set_system_clock.h"

#if defined SYSTEM_CLOCK_GLOBAL

u8 Flag_1_Sec = 0;    //1秒时间到达标志
u8 Flag_Half_Sec = 0;    //半秒时间到达标志
u32 System_Time_Count = 0;    //系统计时，只用于产生秒标志
u8 LongPressSec=0; // calculate the sec number of main key press

/**
  * @brief  This function set the system clock and release GPIO of JTAG.
  * @param  None
  * @retval None
  */
void Set_System_Clock(void)
{
    RCC_DeInit(); //初始化RCC各寄存器
    RCC_HSICmd(ENABLE); //使能内部HSI时
    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET); //等待设置成功
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_6); //HSI/2时钟输入,倍频16,4*12=48M
    RCC_HCLKConfig(RCC_SYSCLK_Div1); //设置AHB时钟
    RCC_PCLK1Config(RCC_HCLK_Div1); //设置低速AHB时钟
    RCC_PCLK2Config(RCC_HCLK_Div2); //设置高速AHB时钟
    RCC_PLLCmd(ENABLE); //使能PLL
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //等待使能成功
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //选择PLL输出做为系统时钟
    while(RCC_GetSYSCLKSource() != 0x08); //等待设置成功


    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE); 
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //释放出三个JTAG的口PB3，PB4，PA15,做普通IO口线,保留SWD端口

    GPIO_PinRemapConfig( GPIO_Remap_PD01 , ENABLE );	//释放PD0，PD1做普通IO口
}

/**
  * @brief  This function set the Systick timer.
  * @param  None
  * @retval None
  */

void SysTick_Configuration(void)
{
    /* Setup SysTick Timer for 10 msec interrupts  */
    if (SysTick_Config(SystemCoreClock / 200)) //SysTick配置函数
    { 
        /* Capture error */ 
        while (1);
    }  
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);//SysTick中断优先级设置
}

#endif /* SYSTEM_CLOCK_GLOBAL */


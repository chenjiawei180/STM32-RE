 /******************************************************************************
  * @file       USER/set_system_clock.c
  * @author  cjw
  * @date     2016.1.18
  * @brief     This file contains the source of the set system clock funtion and set RTC clock funtion.
  ******************************************************************************/
#include "set_system_clock.h"

#if defined SYSTEM_CLOCK_GLOBAL
/**
  * @brief  This function set the system clock and release GPIO of JTAG.
  * @param  None
  * @retval None
  */
void Set_System_Clock(void)
{
    RCC_DeInit(); //��ʼ��RCC���Ĵ���
    RCC_HSICmd(ENABLE); //ʹ���ڲ�HSIʱ
    while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET); //�ȴ����óɹ�
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_6); //HSI/2ʱ������,��Ƶ16,4*12=48M
    RCC_HCLKConfig(RCC_SYSCLK_Div1); //����AHBʱ��
    RCC_PCLK1Config(RCC_HCLK_Div1); //���õ���AHBʱ��
    RCC_PCLK2Config(RCC_HCLK_Div2); //���ø���AHBʱ��
    RCC_PLLCmd(ENABLE); //ʹ��PLL
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //�ȴ�ʹ�ܳɹ�
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //ѡ��PLL�����Ϊϵͳʱ��
    while(RCC_GetSYSCLKSource() != 0x08); //�ȴ����óɹ�


    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE); 
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //�ͷų�����JTAG�Ŀ�PB3��PB4��PA15,����ͨIO����,����SWD�˿�

    GPIO_PinRemapConfig( GPIO_Remap_PD01 , ENABLE );	//�ͷ�PD0��PD1����ͨIO��
}

#endif /* SYSTEM_CLOCK_GLOBAL */


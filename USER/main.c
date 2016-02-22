 /******************************************************************************
  * @file       USER/main.c
  * @author  cjw
  * @date     2016.1.19
  * @brief     This file contains the source of main function.
  ******************************************************************************/

#include "stm32f10x.h"
#include "global.h"
#include "set_system_clock.h"
#include "usart2.h"
#include "usart1.h"
#include "rf.h"
#include "tm1629.h"
#include "eeprom_24c.h"
#include "key.h"
#include "rf_app.h"

int main(void)
{

/* Configure the NVIC Preemption Priority Bits .
    if need use interrupt,the funtion is must.And the whole project only have one this funtion.*/  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

/* Set the system clock and release the GPIO of JTAG */
#if defined SYSTEM_CLOCK_GLOBAL
    Set_System_Clock();
    SysTick_Configuration();
#endif /* SYSTEM_CLOCK_GLOBAL */

/* Init the usart2 and set it's class of interrupt*/
#if defined USART2_GLOBAL
    CH340_USART2_Config();
    CH340_NVIC_Configuration();
#endif /* USART2_GLOBAL */

/* Init the usart1 and set it's class of interrupt*/
#if defined USART1_GOLBAL
    GD5800_USART1_Config();
    GD5800_NVIC_Configuration();
#endif /* USART1_GOLBAL */

/* Init the RF config */
#if defined RF_GLOBAL
    RF_Config();
#endif

#if defined TM1629_GLOBAL
    Tm1629_GPIO_Config();
    Tm1629_Display();
#endif

#if defined EEPROM_GLOBAL
    I2C_IO_Init();
#endif /* EEPROM_GLOBAL */

#if defined KEY_GLOBAL
    Key_IO_Init();
#endif /* KEY_GLOBAL */

/*Print the test information for DUBUG*/
#if defined (USART2_GLOBAL) && defined (DEBUG_GLOBAL)
    printf("Hello stm32. ");
    //Delete_All_Data();
    printf("Delete_All_Data \r\n ");
#endif /* USART2_GLOBAL && DEBUG_GLOBAL */




    while(1)
    {
#if 0//defined (RF_GLOBAL) && defined (DEBUG_GLOBAL)
        if(RF_Flag == 1)
        {
            printf("RF_ID is %x  \r\n ",RF_ID);
            RF_Flag = 0;
        }
#endif /* RF_GLOBAL && DEBUG_GLOBAL */

#if defined RF_GLOBAL
        Decoder_Process();
#endif

#if defined KEY_GLOBAL
        Key_Process();
#endif /* KEY_GLOBAL */

        ;
    }
}


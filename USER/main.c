#include "stm32f10x.h"
#include "global.h"
#include "set_system_clock.h"
#include "usart2.h"
#include "usart1.h"

int main(void)
{

/* Configure the NVIC Preemption Priority Bits .
    if need use interrupt,the funtion is must.And the whole project only have one this funtion.*/  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

/* Set the system clock and release the GPIO of JTAG */
#if defined SYSTEM_CLOCK_GLOBAL
    set_system_clock();
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

    while(1)
    {
        ;
    }
}


#include "stm32f10x.h"
#include "global.h"
#include "set_system_clock.h"
#include "usart2.h"

int main(void)
{

/* Configure the NVIC Preemption Priority Bits */  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//if need use interrupt,the funtion is must.

/* Set the system clock and release the GPIO of JTAG */
#ifdef SYSTEM_CLOCK_GLOBAL
    set_system_clock();
#endif

/* Init the usart2 and it's class of interrupt*/
#ifdef USART2_GLOBAL
    CH340_USART2_Config();
    CH340_NVIC_Configuration();
#endif

    while(1)
    {
        ;
    }
}


#include "stm32f10x.h"
#include "global.h"
#include "set_system_clock.h"
int main(void)
{
//set the system clock and release the GPIO of JTAG
#ifdef SYSTEM_CLOCK_GLOBAL
    set_system_clock();
#endif
    while(1)
    {
        ;
    }
}


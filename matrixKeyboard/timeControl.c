#include <stm32f0xx.h>

#include "timeControl.h"

void timerInit(void)
{
	SystemCoreClockUpdate();
	SysTick->LOAD = SystemCoreClock / 1000 - 1; //1 ms
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
}

void timeWait(int lenght)
{	
	SysTick->VAL = 0;
	
	int count = 0;
	
	while(count < lenght) 
	{
		if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk ) count++;
	}
}

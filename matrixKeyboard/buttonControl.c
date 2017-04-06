#include <stm32f0xx.h>

#include "buttonControl.h"

enum buttons matrixInquiry(void)
{
	{
		GPIOA->BSRR = GPIO_BSRR_BS_15;
    uint32_t button = GPIOA->IDR & GPIO_IDR_4;
    GPIOA->BSRR = GPIO_BSRR_BR_15;
    if (button){
			return leftButton;
		}
  }
  {
		GPIOA->BSRR = GPIO_BSRR_BS_15;
		uint32_t button = GPIOA->IDR & GPIO_IDR_5;
		GPIOA->BSRR = GPIO_BSRR_BR_15;
		if (button)
			return rightButton;
  }
  {
		GPIOC->BSRR = GPIO_BSRR_BS_12;
		uint32_t button = GPIOA->IDR & GPIO_IDR_5;
		GPIOC->BSRR = GPIO_BSRR_BR_12;
    if (button)
			return bottomButton;
  }
  {
		GPIOC->BSRR = GPIO_BSRR_BS_12;
    uint32_t button = GPIOA->IDR & GPIO_IDR_4;
    GPIOC->BSRR = GPIO_BSRR_BR_12;
    if (button)
			return topButton;
  }
	
	return noneButton;
}

enum buttons noiseFilteredMatrixInquiry(uint8_t filterTime)
{  
	static enum buttons currButton = noneButton;
	static enum buttons filteredButton = noneButton;
	enum buttons nextButton = matrixInquiry();
	
  static uint8_t timeCounter = 0;
        
  if (filteredButton != nextButton)
	{
		timeCounter = 0;
		filteredButton = nextButton;
	}
	else if (timeCounter < filterTime)
	{
		timeCounter++;
	}
	else
	{
		currButton = filteredButton;
	}
	
	return currButton;
}

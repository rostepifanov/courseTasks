#include <stm32f0xx.h>

#include "buttonControl.h"
#include "timeControl.h"
#include "initModule.h"

//Red PC6
//Orange PC7
//Blue PC8
//Green PC9

enum lamp 
{
	redDiod = GPIO_ODR_6,
	orangeDiod = GPIO_ODR_8,
	blueDiod = GPIO_ODR_7,
	greenDiod = GPIO_ODR_9
};

void turnOnLight(enum lamp diod)
{
	GPIOC->ODR &= ~diod;
	GPIOC->ODR |= diod;
}

void turnOffLight(enum lamp diod)
{
	GPIOC->ODR &= ~diod;
}

void winkLight(enum lamp diod, int times)
{
	for(int i = 0; i != times; i++)
	{
		turnOnLight(diod);
		timeWait(100);
		turnOffLight(diod);
		timeWait(100);
	}
}

void SysTick_Handler () 
{

}


void matrixTurnOnLED(uint16_t row, uint16_t col)
{
	GPIOA->BSRR = GPIO_BSRR_BR_8;
	row = 0x01 << row;
	col = 0x01 << col;
	SPI2->DR = row << 2 * 4 | col;
	while (SPI2->SR & SPI_SR_BSY);
	GPIOA->BSRR = GPIO_BSRR_BS_8;
}

void matrixTurnOnCross(uint16_t row, uint16_t col)
{
	uint16_t center = ((0x01 << row) << 2 * 4) | (0x01 << col);
	uint16_t right = ((0x01 << row) << 2 * 4) | (0x01 << (col + 1));
	uint16_t left = ((0x01 << row) << 2 * 4) | (0x01 << (col - 1));
	uint16_t top = ((0x01 << (row + 1)) << 2 * 4) | (0x01 << col);
	uint16_t bottom = ((0x01 << (row - 1)) << 2 * 4) | (0x01 << col);
	
	GPIOA->BSRR = GPIO_BSRR_BR_8;
	if(col == 7) right = 0x0000; 
	if(col == 0) left = 0x0000; 
	SPI2->DR = center | right | left;
	while (SPI2->SR & SPI_SR_BSY);
	GPIOA->BSRR = GPIO_BSRR_BS_8;
	
	if(row < 7)
	{
	GPIOA->BSRR = GPIO_BSRR_BR_8;
	SPI2->DR = top; 
	while (SPI2->SR & SPI_SR_BSY);
	GPIOA->BSRR = GPIO_BSRR_BS_8;
	}
	
	if(row > 0)
	{
	GPIOA->BSRR = GPIO_BSRR_BR_8;
	SPI2->DR = bottom; 
	while (SPI2->SR & SPI_SR_BSY);
	GPIOA->BSRR = GPIO_BSRR_BS_8;
	}
}
 
int main(void) 
{
	init();

	matrixTurnOnLED(1 , 6);
	
	uint16_t x = 0;
	uint16_t y = 0;
	
	enum buttons currButton = noneButton;
	
  while (1)
  {	
		if (currButton == noneButton)
		{
			switch (currButton = noiseFilteredMatrixInquiry(5))
			{
				case topButton:
					y = (y < 7) ? y+1 : y;
					break;
				case bottomButton:
					y = (y > 0) ? y-1 : y;
					break;
				case leftButton:
					x = (x > 0) ? x-1 : x;
					break;
				case rightButton:
					x = (x < 7) ? x+1 : x;
					break;
			}
		}
		else
		{
			currButton = noiseFilteredMatrixInquiry(5);
		}
		matrixTurnOnCross(y , x);
	}
}

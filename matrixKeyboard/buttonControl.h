#ifndef buttonControl
#define buttonControl

enum buttons 
{
	topButton,
	rightButton,
	bottomButton,
	leftButton,
	noneButton
};

enum buttons matrixInquiry(void);

enum buttons noiseFilteredMatrixInquiry(uint8_t filterTime);

#endif

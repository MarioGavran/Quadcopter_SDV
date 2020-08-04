#include "ADC_STM32L031K6.h"



//************************************|  INITIALIZE ADC  |************************************
void initADC(void)
{
	
	RCC->APB2ENR	|=	RCC_APB2ENR_ADC1EN;				// Enable clock on ADC1.
	RCC->IOPENR		|=	RCC_IOPENR_GPIOAEN;				// Enable clock on GPIOA.
	
	
	GPIOA->MODER	|=	GPIO_MODER_MODE0_Msk;			// Enable analog mode on PA4.
	GPIOA->MODER	|=	GPIO_MODER_MODE1_Msk;			// Enable analog mode on PA7.
	GPIOA->MODER	|=	GPIO_MODER_MODE3_Msk;			// Enable analog mode on PB0.
	GPIOA->MODER	|=	GPIO_MODER_MODE4_Msk;			// Enable analog mode on PB1.
	
	if ((ADC1->CR & ADC_CR_ADEN) != 0)					// Make sure that ADC is diabled before calibration
	{
		ADC1->CR |=	ADC_CR_ADDIS;
	}
	while(ADC1->CR & ADC_CR_ADEN);							// Wait until the ADC is fully disabled.
	
	
	ADC1_COMMON->CCR	|=	ADC_CCR_LFMEN;				// LFMEN Low frequency mode enable
	ADC1->CFGR2				|=	ADC_CFGR2_CKMODE_Msk;	// PCLK not divided
	
	// Calibration
	ADC1->CR			|=	ADC_CR_ADCAL;							// Start the calibration, automatically sets ADVREGEN.
	while(!(ADC1->ISR & ADC_ISR_EOCAL));				// EOCAL is set when calibration completes.
	while(ADC1->CR & ADC_CR_ADCAL);							// ADCAL is cleared when calibration completes.
	ADC1->ISR			|=	ADC_ISR_EOCAL;						// Write EOCAL=1 to clear it.
	
	
	// ADC enable
	ADC1->ISR			|=	ADC_ISR_ADRDY;						// Write ADRDY=1 to clear it,
	ADC1->CR 			|=	ADC_CR_ADEN;							// enable the ADC,
	while(!(ADC1->ISR & ADC_ISR_ADRDY));				// wait until ADC_CR ADRDY = 1, 
	
	
	ADC1->CHSELR	|=	ADC_CHSELR_CHSEL0;				// Select channel ADC_CH0
	ADC1->CHSELR	|=	ADC_CHSELR_CHSEL1;				// Select channel ADC_CH1
	ADC1->CHSELR	|=	ADC_CHSELR_CHSEL3;				// Select channel ADC_CH3
	ADC1->CHSELR	|=	ADC_CHSELR_CHSEL4;				// Select channel ADC_CH4
	
	ADC1->SMPR		|=	ADC_SMPR_SMP_Msk;
}


//************************************| SINGLE CONVERSION |***********************************
uint16_t getSingleADC(uint8_t INx)
{
	uint16_t val=0;															// Data variable.
	
	ADC1->CHSELR	&=	~(ADC_CHSELR_CHSEL_Msk);	// Clear all channels.
	ADC1->CHSELR	|=	(1<<INx);							// Select channel.
	
	while(ADC1->CR	&	ADC_CR_ADSTART);					// Wait until ADSTART=0,
	ADC1->CR	|=	ADC_CR_ADSTART;								// Start the conversion.
	while(!(ADC1->ISR & ADC_ISR_EOC));					// Wait for EOC=1 
	val=ADC1->DR;																// EOC is cleared by reading the ADC_DR register.
	while(!(ADC1->ISR & ADC_ISR_EOS));					// Wait for EOS=1
	ADC1->ISR |= ADC_ISR_EOS;										// Write EOS=1 to clear it.
	return val;																	// Return measured value.
}


//************************************| BURST CONVERSION |***********************************
void getMultiADC(JOY_DATA_Type* Data_pointer)
{
	ADC1->CHSELR	&=	~(ADC_CHSELR_CHSEL_Msk);	// Clear all channels.
	ADC1->CHSELR	|=	ADC_CHSELR_CHSEL0;				// Select channel ADC_CH0.
	ADC1->CHSELR	|=	ADC_CHSELR_CHSEL1;				// Select channel ADC_CH1.
	ADC1->CHSELR	|=	ADC_CHSELR_CHSEL3;				// Select channel ADC_CH3.
	ADC1->CHSELR	|=	ADC_CHSELR_CHSEL4;				// Select channel ADC_CH4.
	
	uint8_t i=0;																// Counter variable.
	
	ADC1->CFGR1	|=	ADC_CFGR1_DISCEN;						// Enable discontinous mode.
	
	for(i=0;i<4;i++)
	{
		while(ADC1->CR	&	ADC_CR_ADSTART);				// Wait until ADSTART=0,
		ADC1->CR		|=	ADC_CR_ADSTART;						// Start the conversion.
		while(!(ADC1->ISR & ADC_ISR_EOC));				// Wait for EOC=1
		
		switch(i)
		{
			case 0: (Data_pointer->Throttle)=ADC1->DR;		// EOC is cleared by reading the ADC_DR register.
			break;
			case 1:	(Data_pointer->Roll)=ADC1->DR;
			break;
			case 2:	(Data_pointer->Pitch)=ADC1->DR;
			break;
			case 3:	(Data_pointer->Yaw)=ADC1->DR;
			break;
			default: ;break;
		}
	}
	while(!(ADC1->ISR & ADC_ISR_EOS));					// Wait for EOS=1
	ADC1->ISR |= ADC_ISR_EOS;										// Write EOS=1 to clear it.
	
	ADC1->CFGR1	&=	~(ADC_CFGR1_DISCEN);				// Disable discontinous mode.
}

/************************************************************************************************************************
LIBRARY: 		PWM_TIM2.c

AUTHOR: 		Mario Gavran, E5029604, University of Maribor - FERI
*************************************************************************************************************************/
#include "PWM_TIM2.h"													// delay_TIM2 header file.

void PWM_TIM2_init(void)
{
	RCC->IOPENR 	|=	RCC_IOPENR_GPIOAEN;				// Enable PORTA.
	RCC->IOPENR 	|=	RCC_IOPENR_GPIOBEN;				// Enable PORTB.
	
	
	//TIM2_CH1
	GPIOA->MODER	&=	~(GPIO_MODER_MODE8_Msk);	// Clear MODER for PA8.
	GPIOA->MODER	|=	GPIO_MODER_MODE8_1;				// Set AF for PA8.
	GPIOA->MODER	&=	~(GPIO_MODER_MODE8_0);				
	GPIOA->AFR[1]	|=	GPIO_AFRH_AFSEL8_0	 			// Set AF5 for PA8.
									|	GPIO_AFRH_AFSEL8_2;
	//TIM2_CH2
	GPIOB->MODER	&=	~(GPIO_MODER_MODE3_Msk);	// Clear MODER for PB3.
	GPIOB->MODER	|=	GPIO_MODER_MODE3_1;				// Set AF for PB3.
	GPIOB->MODER	&=	~(GPIO_MODER_MODE3_0);		// 
	GPIOB->AFR[0]	|=	GPIO_AFRL_AFSEL3_1; 			// Set AF2 for PB3.
	
	//TIM2_CH3
	GPIOB->MODER	&=	~(GPIO_MODER_MODE0_Msk);	// Clear MODER for PB0.
	GPIOB->MODER	|=	GPIO_MODER_MODE0_1;				// Set AF for PB0.
	GPIOB->MODER	&=	~(GPIO_MODER_MODE0_0);		// Set AF for PB0.
	GPIOB->AFR[0]	|=	GPIO_AFRL_AFSEL0_0 				// Set AF5 for PB0.
								|		GPIO_AFRL_AFSEL0_2;
	//TIM2_CH4
	GPIOB->MODER	&=	~(GPIO_MODER_MODE1_Msk);	// Clear MODER for PB1.
	GPIOB->MODER	|=	GPIO_MODER_MODE1_1;				// Set AF for PB1.
	GPIOB->MODER	&=	~(GPIO_MODER_MODE1_0);		// 
	GPIOB->AFR[0]	|=	GPIO_AFRL_AFSEL1_0 				// Set AF5 for PB1.
									|	GPIO_AFRL_AFSEL1_2;
	
	
	
	RCC->APB1ENR	|= 	RCC_APB1ENR_TIM2EN;				// Enable TIM2.
	TIM2->PSC = 0;															// Set prescaler for TIM2.
	TIM2->ARR =21008;														// Set frequency of the counter.(2.1MHz/21008=100Hz)
	TIM2->CCR1 =4200;														// Set duty cycle of PWM signal.
	TIM2->CCR2 =4200;
	TIM2->CCR3 =4200;
	TIM2->CCR4 =4200;
	
	// Select MODE1:
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_2	|	TIM_CCMR1_OC1M_1;
	TIM2->CCMR1 |= TIM_CCMR1_OC2M_2	| TIM_CCMR1_OC2M_1;
	TIM2->CCMR2 |= TIM_CCMR2_OC3M_2	| TIM_CCMR2_OC3M_1;
	TIM2->CCMR2 |= TIM_CCMR2_OC4M_2	| TIM_CCMR2_OC4M_1;
	
	// set OC polarity to active high(standard PWM signal):
	TIM2->CCER &= ~(TIM_CCER_CC1P);
	TIM2->CCER &= ~(TIM_CCER_CC2P);
	TIM2->CCER &= ~(TIM_CCER_CC3P);
	TIM2->CCER &= ~(TIM_CCER_CC4P);
	
	// enable output:
	TIM2->CCER |= TIM_CCER_CC1E;
	TIM2->CCER |= TIM_CCER_CC2E;
	TIM2->CCER |= TIM_CCER_CC3E;
	TIM2->CCER |= TIM_CCER_CC4E;
	
	TIM2->CR1 |= TIM_CR1_ARPE;									// The PRELOAD REGISTER is transferred into the shadow register at each UEV.
	TIM2->EGR	|= TIM_EGR_UG;										// Initialize all registers.
	TIM2->CR1 |= TIM_CR1_CEN;									// Enable counter.
}

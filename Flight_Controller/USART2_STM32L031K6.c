/************************************************************************************************************************
LIBRARY: 		USART2_STM32L031K6.c
					- transmitter and reciever through UART2 with virtual COM port.
					- only for STM32L031K6 devices.
					- declares functions for initializing the interface, sending chars, sending strings and recieving chars.

AUTHOR: 		Mario Gavran, E5029604, University of Maribor - FERI
*************************************************************************************************************************/
#include "USART2_STM32L031K6.h"				// UART2 header file.




/*----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------|INITIALIZATION|--------------------------------------------------*/
void initUSART2(void)
{
	// Clock initialization for USART2 peripheral.
	RCC->APB1ENR 		|=	 RCC_APB1ENR_USART2EN;			// Enable USART2.
	RCC->IOPENR 		|=	 RCC_IOPENR_GPIOAEN;				// Enable PORTA.
	
	// PA2 - TX Virtual COM Port: 
	GPIOA->MODER		&= ~(GPIO_MODER_MODE2_Msk);			// Clear MODER on PA2.
	GPIOA->MODER		|=	 GPIO_MODER_MODE2_1;				// Set AF for PA2.
	GPIOA->AFR[0]		|=	 GPIO_AFRL_AFSEL2_2; 				// turn on gpio AF4  on PA2.
	
	/*
	// PA15 - RX Virtual COM Port:
	GPIOA->MODER 		&= ~(GPIO_MODER_MODE15_Msk);		// Clear MODER for PA15.
	GPIOA->MODER		|=	 GPIO_MODER_MODE15_1;				// Set AF for PA15.
	GPIOA ->AFR[1] 	|=	 GPIO_AFRH_AFSEL15_2;				// turn on gpio AF4  on PA15.
	*/
	// PA9 - TX
	GPIOA->MODER 		&= ~(GPIO_MODER_MODE9_Msk);			// Clear MODER for PA9.
	GPIOA->MODER		|=	 GPIO_MODER_MODE9_1;				// Set AF for PA9.
	GPIOA ->AFR[1] 	|=	 GPIO_AFRH_AFSEL9_2;				// turn on gpio AF4  on PA9.
	
	// PA10 - RX
	GPIOA->MODER 		&= ~(GPIO_MODER_MODE10_Msk);		// Clear MODER for PA10.
	GPIOA->MODER		|=	 GPIO_MODER_MODE10_1;				// Set AF for PA10.
	GPIOA ->AFR[1] 	|=	 GPIO_AFRH_AFSEL10_2;				// turn on gpio AF4  on PA10.
	
	USART2->BRR |= 875;															// BRR=2.1MHz/9600bps.
	USART2->CR1 |= USART_CR1_UE;										// USART enable.
	USART2->CR1 |= USART_CR1_TE;										// Transmitter enable.
	USART2->CR1 |= USART_CR1_RE;										// Reciver enable.
	
	USART2->CR1 |= USART_CR1_RXNEIE;								// Enable interupts on RXNE bit.OUTPUT INTERUPT ENABLED
	NVIC_EnableIRQ(USART2_IRQn);										// Enable interupts to the CPU. INPUT INTERUPT ENABLED
}


/*----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------|SEND CHAR|--------------------------------------------------*/
void USART2_sendData(uint8_t data)
{
	while (!(USART2->ISR & USART_ISR_TXE)){}
  USART2->TDR = data;
}


/*----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------|SEND STRING|--------------------------------------------------*/
void USART2_sendString(uint8_t *str)
{
	while (*str)					// until NULL character
	{
		USART2_sendData(*str++);
	}
}


/*----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------|READ CHAR|--------------------------------------------------*/
uint8_t  USART2_readData(void)
{
  while (!(USART2->ISR & USART_ISR_RXNE));
	return (uint8_t)USART2->RDR;
}

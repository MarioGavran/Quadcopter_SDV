/************************************************************************************************************************
LIBRARY: 		I2C1_STM32L031K6.c

AUTHOR: 		Mario Gavran, E5029604, University of Maribor - FERI
*************************************************************************************************************************/

#include "I2C1_STM32L031K6.h"						// I2C1 header


/*----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------|INITIALIZATION|--------------------------------------------------*/
void initI2C1(uint8_t DeviceAddress)
{
	// Clocks setup:
	RCC->IOPENR	|= RCC_IOPENR_GPIOBEN;		// Enable clock on GPIOB
	RCC->CR		|= RCC_CR_HSION;		// Enable HSI16 clock
	while(!((RCC->CR) & (RCC_CR_HSIRDY)));		// Wait for HSI16 RDY flag
	RCC->CCIPR	|= RCC_CCIPR_I2C1SEL_1;		// Set HSI16 as I2C1 clock
	RCC->APB1ENR	|= RCC_APB1ENR_I2C1EN;		// Enable clock on I2C1
	
	// Set MODER register to AF mode:
	GPIOB->MODER	|= GPIO_MODER_MODE6_1;		// PB6 - SCL
	GPIOB->MODER	&= ~GPIO_MODER_MODE6_0;
	GPIOB->MODER	|= GPIO_MODER_MODE7_1;		// PB7 - SDA
	GPIOB->MODER	&= ~GPIO_MODER_MODE7_0;
	
	// Set AFRH register to AF1:
	GPIOB->AFR[0]	|= GPIO_AFRL_AFSEL6_0;
	GPIOB->AFR[0]	|= GPIO_AFRL_AFSEL7_0;
	
	// Set OTYPER register to open-drain:
	GPIOB->OTYPER	|= GPIO_OTYPER_OT_6;
	GPIOB->OTYPER	|= GPIO_OTYPER_OT_7;
	
	// Set OSPEEDR register to Low speed:
	GPIOB->OSPEEDR	&= ~(GPIO_OSPEEDER_OSPEED6_1 | GPIO_OSPEEDER_OSPEED6_0);
	GPIOB->OSPEEDR	&= ~(GPIO_OSPEEDER_OSPEED7_1 | GPIO_OSPEEDER_OSPEED7_0);
	
	// Set PUPDR register to no pull-up or pull-down resstors
	GPIOB->PUPDR	&= ~(GPIO_PUPDR_PUPD6_1	| GPIO_PUPDR_PUPD6_0);
	GPIOB->PUPDR	&= ~(GPIO_PUPDR_PUPD7_1	| GPIO_PUPDR_PUPD7_0);
	
	// Enable Fast-mode-plus FMP
	RCC->APB2ENR	|= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->CFGR2	|= SYSCFG_CFGR2_I2C1_FMP | SYSCFG_CFGR2_I2C_PB6_FMP | SYSCFG_CFGR2_I2C_PB7_FMP;
	
	// Set I2C_TIMING register:
	I2C1->TIMINGR |= (1<<(I2C_TIMINGR_PRESC_Pos));		// Set I2C1CLK prescaler to 2 (PRESC=1)
	I2C1->TIMINGR |= (0x9U<<(I2C_TIMINGR_SCLL_Pos));	// SCLL=0x9 : 
	I2C1->TIMINGR |= (0x3U<<(I2C_TIMINGR_SCLH_Pos));	// SCLH=0x3 : 
	I2C1->TIMINGR |= (0x2U<<(I2C_TIMINGR_SDADEL_Pos));	// SCLDEL=0x2 :  2 prescaled cycles
	I2C1->TIMINGR |= (0x3U<<(I2C_TIMINGR_SCLDEL_Pos));	// SCLDEL=0x2 :  2 prescaled cycles
	
	// Master-communication setup:
	I2C1->CR2 &= ~(I2C_CR2_ADD10);				// 7-bit addressing mode
	I2C1->CR2 |= (0x000000FEU & DeviceAddress);		// Slave address, (FE)last bit must be 0 if 7bit addressing
	I2C1->CR2 &= ~(I2C_CR2_RD_WRN);				// Transfer direction: Write (WRN:0, RD:0)
	I2C1->CR2 &= ~(I2C_CR2_NBYTES_Msk);			// Clear NBYTES so you can set it properly in nex statement.
	I2C1->CR2 |= (2<<I2C_CR2_NBYTES_Pos);			// NBYTES=2 - Number of bytes
	
	I2C1->CR2 &= ~(I2C_CR2_RELOAD);				// RELOAD=0 - Number of bytes is lower than 255
	I2C1->CR2 |= (I2C_CR2_AUTOEND);				// AUTOEND=0 - TC flag is set once the NBYTES number of bytes has been transferred
	
	// Enable I2C1 peripheral
	I2C1->CR1 |= I2C_CR1_PE;
}



/*----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------|  SEND DATA   |--------------------------------------------------*/
void I2C1_writeData(uint8_t REG, uint8_t DATA)
{
	while (!((I2C1->ISR) & I2C_ISR_TXE));			// Wait for the TXDR to be empty.
	I2C1->TXDR = I2C_TXDR_TXDATA_Msk & REG;			// Write REG in empty TXDR.
	
	I2C1->CR2 |= I2C_CR2_START;				// Send START bit and than the slave address.
	while (!((I2C1->ISR) & I2C_ISR_TXE))			// Wait for the REG to be shifted.
	{
		if((I2C1->ISR) & I2C_ISR_NACKF) 		// If NACK is detected:
		{
			I2C1->ICR |= I2C_ICR_NACKCF;		// clear NACKF flag
			I2C1->ISR |= I2C_ISR_TXE;		// set TXE bit(flush the TXDR).
		}
	}
	I2C1->TXDR = I2C_TXDR_TXDATA_Msk & DATA;		// Write DATA to the TXDR.
	while (!((I2C1->ISR) & I2C_ISR_STOPF));			// Wait for the DATA to be transmitted, by waiting for STOP flag.
	I2C1->ICR |= I2C_ICR_STOPCF;				// Clear STOPF flag.
	I2C1->ISR |= I2C_ISR_TXE;				// Set TXE bit(flush the TXDR so if the NACK was detected, the com. can start over)
}



/*----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------|  READ WORD   |--------------------------------------------------*/
uint8_t I2C1_readData(uint8_t REG)
{
	uint8_t a=0;
	
	I2C1->CR2 &= ~(I2C_CR2_NBYTES_Msk);			// Clear NBYTES
	I2C1->CR2 |= (1<<I2C_CR2_NBYTES_Pos);			// Change NBYTES to 1(only write reg addres, the data will be read).
	
	while (!((I2C1->ISR) & I2C_ISR_TXE))			// Wait for the TXDR to be empty.
	{
		if((I2C1->ISR) & I2C_ISR_NACKF) 		// If NACK is detected:
		{
			I2C1->ICR |= I2C_ICR_NACKCF;		// Clear NACKF flag
			I2C1->ISR |= I2C_ISR_TXE;		// Set TXE bit to flush the TXDR.
		}
	}
	I2C1->TXDR = I2C_TXDR_TXDATA_Msk & REG;			// Write REG in the TXDR.
	I2C1->CR2 |= I2C_CR2_START;				// Send START bit and than the slave address.
	while (!((I2C1->ISR) & I2C_ISR_STOPF));			// Wait for the REG to be recived, by waiting for STOP flag.
	I2C1->ICR |= I2C_ICR_STOPCF;				// Clear STOP flag.
	
	/*Set up the reciver*/
	I2C1->CR2 |= I2C_CR2_RD_WRN;				// Change transfer direction to Read (WRN:1, RD:1).
	
	I2C1->CR2 |= I2C_CR2_START;				// Send START bit and the slave address.
	while (!((I2C1->ISR) & I2C_ISR_STOPF));			// Wait for STOP flag.
	
	a=I2C1->RXDR;						// Reading the RXDR will clear the RXNE bit.
	I2C1->ICR |= I2C_ICR_STOPCF;				// Clear STOP flag.

	I2C1->CR2 &= ~(I2C_CR2_RD_WRN);				// Change transfer direction back to Write (WRN:0, RD:0).
	I2C1->CR2 &= ~(I2C_CR2_NBYTES_Msk);			// Clear NBYTES so you can set it properly in nex statement.
	I2C1->CR2 |= (2<<I2C_CR2_NBYTES_Pos);			// Change NBYTES to 2.
	
	return(a);		
}



/*----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------| BURST READ   |--------------------------------------------------*/
void I2C1_readMulData(uint8_t REG, uint8_t N, GYRO_DATA_Type* dataPointer)
{
	uint16_t i=0;
	
	I2C1->CR2 &= ~(I2C_CR2_NBYTES_Msk);			// Clear NBYTES so you can set it properly in the next statement.
	I2C1->CR2 |= (1<<I2C_CR2_NBYTES_Pos);			// Change NBYTES to 1(only write reg addres, the data will be read).
	
	while (!((I2C1->ISR) & I2C_ISR_TXE))			// Wait for the TXDR to be empty.
	{
		if((I2C1->ISR) & I2C_ISR_NACKF) 		// If NACK is detected:
		{
			I2C1->ICR |= I2C_ICR_NACKCF;		// Clear NACKF flag.
			I2C1->ISR |= I2C_ISR_TXE;		// Set TXE bit to flush the TXDR.
		}
	}
	I2C1->TXDR = I2C_TXDR_TXDATA_Msk & REG;			// Write REG in the TXDR.
	I2C1->CR2 |= I2C_CR2_START;				// Send START bit, than the slave address and than the .
	while (!((I2C1->ISR) & I2C_ISR_STOPF));			// Wait for the REG to be transmitted, by waiting for STOP flag.
	I2C1->ICR |= I2C_ICR_STOPCF;				// Clear STOP flag.
	
	
	/*Set up the reciver for multiple bytes read*/
	I2C1->CR2 |= I2C_CR2_RD_WRN;				// Change transfer direction to Read (WRN:1, RD:1).
	I2C1->CR2 &= ~(I2C_CR2_NBYTES_Msk);			// Clear NBYTES.
	I2C1->CR2 |= (N<<I2C_CR2_NBYTES_Pos);			// Set NBYTES to N.
	
	/*burst reception*/
	I2C1->CR2 |= I2C_CR2_START;				// Send START bit and the slave address.
	
	for(i=0;i<N;i++)					// Read N times.
	{
		while (!((I2C1->ISR) & I2C_ISR_RXNE));		// Wait for RXNE flag.
		
		switch(i)
		{
			case 0:	(dataPointer->XAxisH) = (I2C1->RXDR);	// Reading the RXDR will clear the RXNE bit.
			break;
			case 1:	(dataPointer->XAxisL) = (I2C1->RXDR);
			break;
			case 2:	(dataPointer->YAxisH) = (I2C1->RXDR);
			break;
			case 3:	(dataPointer->YAxisL) = (I2C1->RXDR);
			break;
			case 4:	(dataPointer->ZAxisH) = (I2C1->RXDR);
			break;
			case 5:	(dataPointer->ZAxisL) = (I2C1->RXDR);
			break;
			default:	;break;
		}
	}
	
	while (!((I2C1->ISR) & I2C_ISR_STOPF));				// Wait for STOP flag.
	I2C1->ICR |= I2C_ICR_STOPCF;					// Clear STOP flag.
	
	I2C1->CR2 &= ~(I2C_CR2_RD_WRN);					// Change transfer direction back to Write (WRN:0, RD:0).
	I2C1->CR2 &= ~(I2C_CR2_NBYTES_Msk);				// Clear NBYTES.
	I2C1->CR2 |= (2<<I2C_CR2_NBYTES_Pos);				// Change NBYTES to 2.
}


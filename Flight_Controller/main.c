#include "stm32l0xx.h"                  // Device header.
#include "USART2_STM32L031K6.h"					// USART driver.
#include "PWM_TIM2.h"										// PWM driver.
#include "I2C1_STM32L031K6.h"						// I2C driver.
#include <stdint.h>											// Standard integer data types.
#include "My_Data_Types.h"							// Data structures.


//********* INTERUPT HANDLER DECLARATION **********************************************************************
void USART2_IRQHandler(void);


//********* GLOBAL DATA STRUCTURES INSTANCES: *****************************************************************
JOY_DATA_Type 	JoystickBuffer;												// Reciver buffer.
JOY_DATA_Type*	JoystickBufferP = &JoystickBuffer;

JOY_DATA_Type 	Joystick;															// Reciver data.
JOY_DATA_Type*	JoystickP = &Joystick;

GYRO_DATA_Type 	Gyroscope;														// Gyroscope data.
GYRO_DATA_Type* GyroscopeP = &Gyroscope;


//********* PID CONTROLLER GAIN SETTINGS: *********************************************************************
#define		P_X_GAIN	1
#define		I_X_GAIN	0.01
#define		D_X_GAIN	1.5

#define		P_Y_GAIN	P_X_GAIN
#define		I_Y_GAIN	I_X_GAIN
#define		D_Y_GAIN	D_X_GAIN

#define		P_Z_GAIN	0
#define		I_Z_GAIN	0
#define		D_Z_GAIN	0


//******************************** main() FUNCTION: ***********************************************************
int main(void)
{
	
	PWM_TIM2_init();													// Initialize PWM
	initI2C1(MPU6050Address);									// Initialize I2C
	initUSART2();															// Initialize USART2
	
	
//******************************** MAIN-SCOPE VARIABLES: ******************************************************
	// Throttle variables:
	int16_t		Throttle_Data=0;								// Used for conditioning reciver throttle data.
	uint32_t	Throttle_Offset=0;							// Initial throttle offset.
	
	// Roll (X) axis gyroscope & reciver variables:
	int16_t		Roll_Data=0;										// For conditioning reciver Roll data.
	int32_t		Roll_Offset=0;									// Recivers initial Roll offset.
	int16_t		GYRO_X_Data=0;									// For conditioning gyroscope X data.
	int32_t		GYRO_X_Offset=0;								// Initial gyroscope X offset.
	// Pitch (Y) axis gyroscope & reciver variables:
	int16_t		Pitch_Data=0;
	int32_t		Pitch_Offset=0;
	int16_t		GYRO_Y_Data=0;
	int32_t		GYRO_Y_Offset=0;
	// Yaw (Z) axis gyroscope & reciver variables:
	int16_t		Yaw_Data=0;
	int32_t		Yaw_Offset=0;
	int16_t		GYRO_Z_Data=0;
	int32_t		GYRO_Z_Offset=0;
	
	// Roll (X) PID Controller variables:
	int16_t		PID_X_OUT=0;										// PID regulator output.
	int16_t		I_X_OUT=0;											// Intergral regulator output.
	int16_t		ERR_X=0;												// Current error.
	int16_t		PERR_X=0;												// Previous error.
	// Pitch (Y) PID Controller variables:
	int16_t		PID_Y_OUT=0;
	int16_t		I_Y_OUT=0;
	int16_t		ERR_Y=0;
	int16_t		PERR_Y=0;
	// Yaw (Z) PID Controller variables:
	int16_t		PID_Z_OUT=0;
	int16_t		I_Z_OUT=0;
	int16_t		ERR_Z=0;
	int16_t		PERR_Z=0;
	
	// ESC PWM CCR temporary variables:
	int16_t		ESC_CCR1=0;
	int16_t		ESC_CCR2=0;
	int16_t		ESC_CCR3=0;
	int16_t		ESC_CCR4=0;
	
	uint16_t	i=0;
	
	
//**************************************** MPU6050 setup    ***************************************************
	do																													// Make sure that MPU6050 registers are set correctly.
	{
		I2C1_writeData(0x1B,0x18);																// MPU6050 REG:Gyroscope Config, Full scale FS_SEL[1:0]=2
		I2C1_writeData(0x6B,0x00);																// MPU6050 REG:Power Management1, Disable sleep mode
	}while( !(I2C1_readData(0x1B)==0x18) ||
					!(I2C1_readData(0x6B)==0x00) );											// Repeat until you read what you wrote.
	
	
//**************************************** GYROSCOPE offset ***************************************************
	while(i<3000)																													// Acquire 3000 samples and sum them all.
	{
		I2C1_readMulData(0x43,6U,GyroscopeP);																// Read all axies.
		GYRO_X_Offset	+= (int16_t)(0xFFFF & (((GyroscopeP->XAxisH) << 8) |
																(GyroscopeP->XAxisL)));
		GYRO_Y_Offset	+= (int16_t)(0xFFFF & (((GyroscopeP->YAxisH) << 8) |
																(GyroscopeP->YAxisL)));
		GYRO_Z_Offset	+= (int16_t)(0xFFFF & (((GyroscopeP->ZAxisH) << 8) |
																(GyroscopeP->ZAxisL)));
		i++;																																// Increment counter.
	}
	GYRO_X_Offset /= 3000;																								// Calculate offsets.
	GYRO_Y_Offset /= 3000;
	GYRO_Z_Offset /= 3000;
	i=0;																																	// Reset counter to be used for JOYSTICK offset
	
	
//**************************************** JOYSTICK offset ****************************************************
	while( ((JoystickP->Throttle)<<1) > 200)																	// Wait until stick is in low position.
	{
		TIM2->CCR1 = 2200 + ((JoystickP->Throttle)<<1);													// While waitnig, update the PWM CCR registers.
		TIM2->CCR2 = TIM2->CCR1;
		TIM2->CCR3 = TIM2->CCR1;
		TIM2->CCR4 = TIM2->CCR1;
	}
	
	while(i<2000)																																// Acquire 2000 samples.
	{
		Throttle_Offset += (int16_t)(0x07FFU & ((JoystickP->Throttle)<<1) );			// Sum all reads.
		Roll_Offset			+= (int16_t)(0x07FFU & (JoystickP->Roll));
		Pitch_Offset		+= (int16_t)(0x07FFU & (JoystickP->Pitch));
		Yaw_Offset			+= (int16_t)(0x07FFU & (JoystickP->Yaw));
		i++;																																			// Increment counter.
	}
	Throttle_Offset	/= 2000;																										// Calculate offsets.
	Roll_Offset			/= 2000;
	Pitch_Offset		/= 2000;
	Yaw_Offset			/= 2000;
	
	
//**************************************** main() FUNCTION LOOP: **********************************************
	while(1)
	{
		// Read all 3 axis from the gyroscope.
		I2C1_readMulData(0x43,6U,GyroscopeP);
		
		// Gyroscope data conditioning:
		GYRO_X_Data	= (int16_t)(((int16_t)(0xFFFF & ( ((GyroscopeP->XAxisH)<<8) | (GyroscopeP->XAxisL))) - GYRO_X_Offset)/5);
		if(((int16_t)(GYRO_X_Data)<(2)) && ((int16_t)(GYRO_X_Data)>=(-5))) GYRO_X_Data=0;
		
		GYRO_Y_Data	= (int16_t)(((int16_t)(0xFFFF & ( ((GyroscopeP->YAxisH)<<8) | (GyroscopeP->YAxisL))) - GYRO_Y_Offset)/5);
		if(((int16_t)(GYRO_Y_Data)<(2)) && ((int16_t)(GYRO_Y_Data)>(-2))) GYRO_Y_Data=0;
		
		GYRO_Z_Data	= (int16_t)(((int16_t)(0xFFFF & ( ((GyroscopeP->ZAxisH)<<8) | (GyroscopeP->ZAxisL))) - GYRO_Z_Offset)/5);
		if(((int16_t)(GYRO_Z_Data)<(2)) && ((int16_t)(GYRO_Z_Data)>(-2))) GYRO_Z_Data=0;
		
		
		// Reciver data conditioning:
		NVIC_DisableIRQ(USART2_IRQn);																											// Disable interupt while conditioning
		Throttle_Data = (0x07FFU & ((JoystickP->Throttle)<<1))- Throttle_Offset;					//    (0) - (+2046)
		
		Roll_Data 	= (int16_t) ((0x03FFU & ((JoystickP->Roll)))	- Roll_Offset);					// (-512) - (+511)
		if(((int16_t)(Roll_Data)<(11)) && ((int16_t)(Roll_Data)>(-11))) Roll_Data=0;			// Deadband for Roll data.
		
		Pitch_Data	= (int16_t) ((0x03FFU & ((JoystickP->Pitch)))	- Pitch_Offset);				// (-512) - (+511)
		if(((int16_t)(Pitch_Data)<(11)) && ((int16_t)(Pitch_Data)>(-11))) Pitch_Data=0;		// Deadband for Pitch data.
		
		Yaw_Data		= (int16_t) ((0x03FFU & ((JoystickP->Yaw)))		- Yaw_Offset);					// (-512) - (+511)
		if(((int16_t)(Yaw_Data)<(11)) && ((int16_t)(Yaw_Data)>(-11))) Yaw_Data=0;					// Deadband for Pitch data.
		NVIC_EnableIRQ(USART2_IRQn);																											// Enable interupt
		
		
		// PID Roll (X) controller calculation:
		ERR_X			=	GYRO_X_Data - Roll_Data;															// Current error.
		I_X_OUT		=	I_X_OUT + ERR_X*I_X_GAIN;															// Integral controller output.
		PID_X_OUT	= ERR_X*P_X_GAIN + I_X_OUT + (PERR_X-ERR_X)*D_X_GAIN;		// PID controller output.
		PERR_X		=	ERR_X;																								// Previous error for next controller loop(initially is 0).
		
		// PID Pitch (Y) controller calculation:
		ERR_Y			=	GYRO_Y_Data - Pitch_Data;
		I_Y_OUT		=	I_Y_OUT + ERR_Y*I_Y_GAIN;
		PID_Y_OUT	= ERR_Y*P_Y_GAIN + I_Y_OUT + (PERR_Y-ERR_Y)*D_Y_GAIN;
		PERR_Y		=	ERR_Y;
		
		// PID Pitch (Y) controller calculation:
		ERR_Z			=	GYRO_Z_Data - Yaw_Data;
		I_Z_OUT		=	I_Z_OUT + ERR_Z*I_Z_GAIN;
		PID_Z_OUT	= ERR_Z*P_Z_GAIN + I_Z_OUT + (PERR_Z-ERR_Z)*D_Z_GAIN;
		PERR_Z		=	ERR_Z;
		
		
		// Limit the throttle.
		if(Throttle_Data > 2048) 		Throttle_Data = 2048;
		else if(Throttle_Data < 0)	Throttle_Data = 0;
		
		// Limit the X, Y & Z CONTROLLER outputs.
		if(PID_X_OUT<(-500)) PID_X_OUT = -500;
		else if(PID_X_OUT>500) PID_X_OUT = 500;
		
		if(PID_Y_OUT<(-500)) PID_Y_OUT = -500;
		else if(PID_Y_OUT>500) PID_Y_OUT = 500;
		
		if(PID_Z_OUT<(-500)) PID_Z_OUT = -500;
		else if(PID_Z_OUT>500) PID_Z_OUT = 500;
		
		
		// Conditioning TIMERs PWM signal high-time duration:
		ESC_CCR1 = 2100 + (Throttle_Data + PID_X_OUT + PID_Y_OUT + PID_Z_OUT);				// Front right
		ESC_CCR2 = 2100 + (Throttle_Data - PID_X_OUT + PID_Y_OUT - PID_Z_OUT);				// Front left
		ESC_CCR3 = 2100 + (Throttle_Data + PID_X_OUT - PID_Y_OUT - PID_Z_OUT);				// Rear right
		ESC_CCR4 = 2100 + (Throttle_Data - PID_X_OUT - PID_Y_OUT + PID_Z_OUT);				// Rear left
		
		
		
		// Limiting the PWM signals
		if(ESC_CCR1<2100) ESC_CCR1 = 2100;
		if(ESC_CCR2<2100) ESC_CCR2 = 2100;
		if(ESC_CCR3<2100) ESC_CCR3 = 2100;
		if(ESC_CCR4<2100) ESC_CCR4 = 2100;
		if(ESC_CCR1>4200) ESC_CCR1 = 4200;
		if(ESC_CCR2>4200) ESC_CCR2 = 4200;
		if(ESC_CCR3>4200) ESC_CCR3 = 4200;
		if(ESC_CCR4>4200) ESC_CCR4 = 4200;
		
		
		
		// Update PWM CCR registers
		TIM2->CCR1 = ESC_CCR1;
		TIM2->CCR2 = ESC_CCR2;
		TIM2->CCR3 = ESC_CCR3;
		TIM2->CCR4 = ESC_CCR4;
		
	}
	
}



//**************************************** INTERUPT HANDLER DEFINITION ****************************************
// -Each recived byte has an ordinal number encoded in the 3 least significant bits. The most significant 5 
//  bits are data bits. Each two bytes recived represent high and low parts of the whole 10-bit data, for every
//  2 bytes recived. Static local-scope variable Tag is used for checking the byte's ordinal number. The 3 least
//  significant bits in the Tag variable represent the ordinal nuber of the byte that is expected to be recived.
//  Only if all 8 bytes are recived in correct order the data structure is used in calculations.(see last case)
//*************************************************************************************************************

void USART2_IRQHandler(void)
{
	
	uint8_t RecivedByte = USART2_readData();
	
	static uint8_t Tag = 0x00U;																	
	if((RecivedByte & 0x07U) == 0x00)	Tag = 0x00;			// 0x07 = 0b00000111
		
	if((RecivedByte & 0x07U) == Tag)
	{
		switch((RecivedByte & 0x07U))
		{
		case 0x00:		// XXXXX000
			Tag=0x01U;
			JoystickBufferP->Throttle	= ((0xF8U & RecivedByte)<<2);
			break;
		case 0x01:		// XXXXX001
			Tag=0x02U;
			JoystickBufferP->Throttle	|= ((0xF8U & RecivedByte)>>3);
			break;
		case 0x02:		// XXXXX010
			Tag=0x03U;
			JoystickBufferP->Roll 		= ((0xF8U & RecivedByte)<<2);
			break;
		case 0x03:		// XXXXX011
			Tag=0x04U;
			JoystickBufferP->Roll 		|= ((0xF8U & RecivedByte)>>3);
			break;
		case 0x04:		// XXXXX100
			Tag=0x05U;
			JoystickBufferP->Pitch 		= ((0xF8U & RecivedByte)<<2);
			break;
		case 0x05:		// XXXXX101
			Tag=0x06U;
			JoystickBufferP->Pitch 		|= ((0xF8U & RecivedByte)>>3);
			break;
		case 0x06:		// XXXXX110
			Tag=0x07U;
			JoystickBufferP->Yaw 			= 0; //((0xF8U & RecivedByte)<<2);
			break;
		case 0x07:		// XXXXX111
			Tag=0x00U;
			JoystickBufferP->Yaw 			|= 0; //((0xF8U & RecivedByte)>>3);
			*JoystickP = *(JoystickBufferP);
			break;
		default:
			break;
		}
	}
}

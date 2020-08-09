/*------------------------------------------------------------------------------------------------
Joystick driver software.
------------------------------------------------------------------------------------------------*/

#include "stm32l0xx.h"                  // Device header
#include "ADC_STM32L031K6.h"
#include "My_Data_Types.h"
#include "USART2_STM32L031K6.h"

JOY_DATA_Type Joystick;
JOY_DATA_Type* JoystickP = &Joystick;

int main(void)
{
	initUSART2();
	initADC();
	
	int i=0;
	
	while(1)
	{
		getMultiADC(JoystickP);
		
		u2_sendCh((0xF8U & (((JoystickP->Throttle))>>4)) | (0x00U));
		u2_sendCh((0xF8U & (((JoystickP->Throttle))<<1)) | (0x01U));
		
		u2_sendCh((0xF8U & (((JoystickP->Roll))>>4)) | (0x02U));
		u2_sendCh((0xF8U & (((JoystickP->Roll))<<1)) | (0x03U));
		
		u2_sendCh((0xF8U & (((JoystickP->Pitch))>>4)) | (0x04U));
		u2_sendCh((0xF8U & (((JoystickP->Pitch))<<1)) | (0x05U));
		
		u2_sendCh((0xF8U & (((JoystickP->Yaw))>>4)) | (0x06U));
		u2_sendCh((0xF8U & (((JoystickP->Yaw))<<1)) | (0x07U));
		
		/*
		u2_sendCh(0x00U | ( (uint8_t)0xF8U & (((int16_t) ((JoystickP->Throttle)>>2)) <<3)));
		u2_sendCh(0x01U | ( (uint8_t)0xF8U & ((int16_t)  ((JoystickP->Throttle)>>2)<<3 )));
		
		u2_sendCh(0x02U | ( (uint8_t)0xF8U & (((JoystickP->Roll)>>2)<<3)) );
		u2_sendCh(0x03U | ( (uint8_t)0xF8U & (JoystickP->Roll)>>2)<<3 );
		
		u2_sendCh(0x04U | ( (uint8_t)0xF8U & (((JoystickP->Pitch)>>2)<<3)) );
		u2_sendCh(0x05U | ( (uint8_t)0xF8U & (JoystickP->Pitch)>>2)<<3 );
		
		u2_sendCh(0x06U | ( (uint8_t)0xF8U & (((JoystickP->Yaw)>>2)<<3)) );
		u2_sendCh(0x07U | ( (uint8_t)0xF8U & (JoystickP->Yaw)>>2) <<3);
		*/
		while (i<2000)i++;
		//u2_sendCh(0);
	}
}


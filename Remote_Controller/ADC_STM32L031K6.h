#include "stm32l0xx.h"			// Device header
#include <stdint.h>			// Standard data types
#include "My_Data_Types.h"

void initADC(void);
uint16_t getSingleADC(uint8_t INx);
void getMultiADC(JOY_DATA_Type* Data_pointer);


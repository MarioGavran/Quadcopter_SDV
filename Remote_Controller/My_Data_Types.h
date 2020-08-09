#ifndef __MY_DATA_TYPES_H__
#define __MY_DATA_TYPES_H__

#include <stdint.h>		// Standard integers header file.

typedef struct
{
	uint16_t Throttle;
	uint16_t Roll;
	uint16_t Pitch;
	uint16_t Yaw;
}JOY_DATA_Type;

typedef struct
{
	uint8_t XAxisH;
	uint8_t XAxisL;
	uint8_t YAxisH;
	uint8_t YAxisL;
	uint8_t ZAxisH;
	uint8_t ZAxisL;
}GYRO_DATA_Type;

#endif


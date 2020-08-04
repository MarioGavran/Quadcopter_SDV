#include <stdint.h>								// Standard data types
#ifndef __MY_DATA_TYPES_H__
#define __MY_DATA_TYPES_H__

typedef struct
{
	uint16_t Throttle;
	uint16_t Roll;
	uint16_t Pitch;
	uint16_t Yaw;
}JOY_DATA_Type;


typedef struct
{
	uint8_t XAxis1;
	uint8_t XAxis2;
	uint8_t YAxis1;
	uint8_t YAxis2;
	uint8_t ZAxis1;
	uint8_t ZAxis2;
}GYRO_DATA_Type;

#endif

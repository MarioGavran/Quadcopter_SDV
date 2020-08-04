#ifndef __I2C1_STM32L031K6_H__
#define	__I2C1_STM32L031K6_H__
#include "stm32l0xx.h"                  // Device header
#include "My_Data_Types.h"							// Data structures.
#include "stm32l0xx.h"									// Device header


extern void			initI2C1(uint8_t DeviceAddress);
extern void			I2C1_writeData(uint8_t REG, uint8_t DATA);
extern uint8_t	I2C1_readData(uint8_t REG);
void 						I2C1_readMulData(uint8_t REG, uint8_t N, GYRO_DATA_Type* Data_pointer);


#define MPU6050Address					(0xD0U)


#define GPIO_AFRL_AFSEL6_0			(0x1U << GPIO_AFRL_AFSEL6_Pos)			/*!< 0x01000000 */
#define GPIO_AFRL_AFSEL6_1			(0x2U << GPIO_AFRL_AFSEL6_Pos)			/*!< 0x02000000 */
#define GPIO_AFRL_AFSEL6_2			(0x4U << GPIO_AFRL_AFSEL6_Pos)			/*!< 0x04000000 */
#define GPIO_AFRL_AFSEL6_3			(0x8U << GPIO_AFRL_AFSEL6_Pos)			/*!< 0x08000000 */


#define GPIO_AFRL_AFSEL7_0			(0x1U << GPIO_AFRL_AFSEL7_Pos)			/*!< 0x10000000 */
#define GPIO_AFRL_AFSEL7_1			(0x2U << GPIO_AFRL_AFSEL7_Pos)			/*!< 0x20000000 */
#define GPIO_AFRL_AFSEL7_2			(0x4U << GPIO_AFRL_AFSEL7_Pos)			/*!< 0x40000000 */
#define GPIO_AFRL_AFSEL7_3			(0x8U << GPIO_AFRL_AFSEL7_Pos)			/*!< 0x80000000 */

#endif /*__I2C1_STM32L031K6_H__*/


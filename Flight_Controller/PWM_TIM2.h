#ifndef __PWM_TIM2_H__
#define	__PWM_TIM2_H__

#include "stm32l0xx.h"                  // Device header

extern void PWM_TIM2_init(void);

//ch1
#define		GPIO_AFRH_AFSEL8_0	(0x1U << GPIO_AFRH_AFSEL8_Pos)		/*!< 0x00000001 */
#define		GPIO_AFRH_AFSEL8_1	(0x2U << GPIO_AFRH_AFSEL8_Pos)		/*!< 0x00000002 */
#define		GPIO_AFRH_AFSEL8_2	(0x4U << GPIO_AFRH_AFSEL8_Pos)		/*!< 0x00000004 */
#define		GPIO_AFRH_AFSEL8_3	(0x8U << GPIO_AFRH_AFSEL8_Pos)		/*!< 0x00000008 */

//ch2
#define		GPIO_AFRL_AFSEL3_0	(0x1U << GPIO_AFRL_AFSEL3_Pos)		/*!< 0x00001000 */
#define		GPIO_AFRL_AFSEL3_1	(0x2U << GPIO_AFRL_AFSEL3_Pos)		/*!< 0x00002000 */
#define		GPIO_AFRL_AFSEL3_2	(0x4U << GPIO_AFRL_AFSEL3_Pos)		/*!< 0x00004000 */
#define		GPIO_AFRL_AFSEL3_3	(0x8U << GPIO_AFRL_AFSEL3_Pos)		/*!< 0x00008000 */

//ch3
#define		GPIO_AFRL_AFSEL0_0	(0x1U << GPIO_AFRL_AFSEL0_Pos)		/*!< 0x00000001 */
#define		GPIO_AFRL_AFSEL0_1	(0x2U << GPIO_AFRL_AFSEL0_Pos)		/*!< 0x00000002 */
#define		GPIO_AFRL_AFSEL0_2	(0x4U << GPIO_AFRL_AFSEL0_Pos)		/*!< 0x00000004 */
#define		GPIO_AFRL_AFSEL0_3	(0x8U << GPIO_AFRL_AFSEL0_Pos)		/*!< 0x00000008 */

//ch4
#define		GPIO_AFRL_AFSEL1_0	(0x1U << GPIO_AFRL_AFSEL1_Pos)		/*!< 0x00000010 */
#define		GPIO_AFRL_AFSEL1_1	(0x2U << GPIO_AFRL_AFSEL1_Pos)		/*!< 0x00000020 */
#define		GPIO_AFRL_AFSEL1_2	(0x4U << GPIO_AFRL_AFSEL1_Pos)		/*!< 0x00000040 */
#define		GPIO_AFRL_AFSEL1_3	(0x8U << GPIO_AFRL_AFSEL1_Pos)		/*!< 0x00000080 */

#endif /*__PWM_TIM2_H__*/


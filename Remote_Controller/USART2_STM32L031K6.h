#include "stm32l0xx.h"								// Device header
#include <stdint.h>
#include <stdlib.h>

extern void initUSART2(void);
extern void u2_sendCh(uint8_t ch);
extern uint8_t u2_getCh(void);
extern void u2_sendStr(uint8_t *s);


/*---------------------------------------------------------------------------------------------*/
#define GPIO_AFRL_AFSEL2_0							(0x1U << GPIO_AFRL_AFSEL2_Pos)				/*!< 0x00000100 */
#define GPIO_AFRL_AFSEL2_1							(0x2U << GPIO_AFRL_AFSEL2_Pos)				/*!< 0x00000200 */
#define GPIO_AFRL_AFSEL2_2							(0x4U << GPIO_AFRL_AFSEL2_Pos)				/*!< 0x00000400 */
#define GPIO_AFRL_AFSEL2_3							(0x8U << GPIO_AFRL_AFSEL2_Pos)				/*!< 0x00000800 */


#define GPIO_AFRH_AFSEL15_0							(0x1U << GPIO_AFRH_AFSEL15_Pos)				/*!< 0x10000000 */
#define GPIO_AFRH_AFSEL15_1							(0x2U << GPIO_AFRH_AFSEL15_Pos)				/*!< 0x20000000 */
#define GPIO_AFRH_AFSEL15_2							(0x4U << GPIO_AFRH_AFSEL15_Pos)				/*!< 0x40000000 */
#define GPIO_AFRH_AFSEL15_3							(0x8U << GPIO_AFRH_AFSEL15_Pos)				/*!< 0x80000000 */


#define GPIO_AFRH_AFSEL9_0							(0x1U << GPIO_AFRH_AFSEL9_Pos)				/*!< 0x00000010 */
#define GPIO_AFRH_AFSEL9_1              (0x2U << GPIO_AFRH_AFSEL9_Pos)				/*!< 0x00000020 */
#define GPIO_AFRH_AFSEL9_2              (0x4U << GPIO_AFRH_AFSEL9_Pos)				/*!< 0x00000040 */
#define GPIO_AFRH_AFSEL9_3              (0x8U << GPIO_AFRH_AFSEL9_Pos)				/*!< 0x00000080 */


#define GPIO_AFRH_AFSEL10_0							(0x1U << GPIO_AFRH_AFSEL10_Pos)				/*!< 0x00000100 */
#define GPIO_AFRH_AFSEL10_1							(0x2U << GPIO_AFRH_AFSEL10_Pos)				/*!< 0x00000200 */
#define GPIO_AFRH_AFSEL10_2							(0x4U << GPIO_AFRH_AFSEL10_Pos)				/*!< 0x00000400 */
#define GPIO_AFRH_AFSEL10_3							(0x8U << GPIO_AFRH_AFSEL10_Pos)				/*!< 0x00000800 */


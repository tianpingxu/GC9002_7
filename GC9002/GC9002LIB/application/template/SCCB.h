#ifndef _SCCB_H
#define _SCCB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx.h"
#include "core_rv32.h"
#define SCCB_SIC_BIT     GPIO_PIN_15	//¶Ë¿ÚµÚ7Î»
#define SCCB_SID_BIT     GPIO_PIN_14	 //¶Ë¿ÚµÚ8Î»

#define SCCB_SIC_H()     HAL_GPIO_WritePin(GPIOB, SCCB_SIC_BIT, GPIO_PIN_SET)
#define SCCB_SIC_L()     HAL_GPIO_WritePin(GPIOB, SCCB_SIC_BIT, GPIO_PIN_RESET)

#define SCCB_SID_H()     HAL_GPIO_WritePin(GPIOB, SCCB_SID_BIT, GPIO_PIN_SET)
#define SCCB_SID_L()     HAL_GPIO_WritePin(GPIOB, SCCB_SID_BIT, GPIO_PIN_RESET)
/**/
#define SCCB_SID_IN      HAL_GPIO_SetDir(GPIOB, SCCB_SID_BIT, GPIO_DIR_INPUT)
#define SCCB_SID_OUT     HAL_GPIO_SetDir(GPIOB, SCCB_SID_BIT, GPIO_DIR_OUTPUT)


#define SCCB_SID_STATE	 (HAL_GPIO_ReadPin(GPIOB, SCCB_SID_BIT) == GPIO_PIN_SET)

///////////////////////////////////////////	 
void SCCB_GPIO_Config(void);
void SCCB_SID_GPIO_OUTPUT(void);
void SCCB_SID_GPIO_INPUT(void);
void startSCCB(void);
void stopSCCB(void);
void noAck(void);
unsigned char SCCBwriteByte(unsigned char m_data);
unsigned char SCCBreadByte(void);
void delay_us(uint32_t us);

#endif /* _SCCB_H */



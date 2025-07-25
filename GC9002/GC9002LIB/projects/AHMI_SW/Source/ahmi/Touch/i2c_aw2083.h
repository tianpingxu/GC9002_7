////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI
//
// Create Date:   2015/11/17
// File Name:     Trace.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by Yu Chunying
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef __AW2083_H
#define __AW2083_H
#include "stdint.h"
#include "platform.h"
#include "AHMITouchDefine.h"

#ifdef __cplusplus 
extern "C" {
#endif

#ifdef EMBEDDED

typedef enum
{
    AW2083Touch_GetTouchState = 0,
    AW2083Touch_GetXValue,  
    AW2083Touch_GetYValue,
    AW2083Touch_GetZ1Value,
	AW2083Touch_GetZ2Value,
	AW2083Touch_GetPosition
}AW2083Touch_teRtpMainSM;
	
typedef struct
{
	uint16_t                   uint16XValue;
	uint16_t                   uint16YValue;
	uint16_t                   uint16Z1Value;
	uint16_t                   uint16Z2Value;
	AW2083Touch_teRtpMainSM    teRtpManSM;
}AW2083Touch_tsWorkbuf;
	
static void AW2083_Delay(void);//I2C delay
void AW2083_Start(void);// i2c start
void AW2083_Stop(void);//i2c stop
void AW2083_SendByte(uint8_t ucByte);//send one byte by i2c
uint8_t AW2083_ReadByte(void);//read one byte by i2c

uint8_t AW2083_WaitAck(void);//i2c wait acknowledge

void AW2083_Ack(void);//send i2c acknowledge
void AW2083_NoAck(void);//no i2c acknowledge
void AW2083_ConfGPIO(void);//config aw2083 gpio
uint8_t AW2083_InitDevice(void);//initial
uint16_t AW2083_ReadSinglePosition(uint16_t AW2083_cw);//read one position

uint16_t AW2083_ReadX(void);//read x channel adc value
uint16_t AW2083_ReadY(void);//read y channel adc value
uint16_t AW2083_ReadZ1(void);//read z1 channel adc value
uint16_t AW2083_ReadZ2(void);//read z2 channel adc value

void AW2083touch_vMain(void);
void AW2083_ReadPosition(Position* CurPosition);//read position information

uint32_t AW2083_GetTouchState(void);
#endif

#ifdef __cplusplus 
}
#endif
#endif

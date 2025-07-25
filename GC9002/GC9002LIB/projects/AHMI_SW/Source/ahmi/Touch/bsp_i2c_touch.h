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
#ifndef __BSP_I2C_TOUCH_H
#define __BSP_I2C_TOUCH_H

#include "stdint.h"
#include <stdio.h>
//#include <string.h>

#ifdef __cplusplus 
extern "C" {
#endif

#include "cfg_c1.h"    

//#define USE_I2C_Hardware
#define USE_I2C_GPIO

//#ifdef EMBEDDED 

#define TOUCH_SCL_GPIO_PIN     					g_touchI2c_SclPin
#define TOUCH_SDA_GPIO_PIN     					g_touchI2c_SdaPin
#define TOUCH_IRQ_GPIO_PIN     				    g_touchI2c_IrqPin
#define TOUCH_RST_GPIO_PIN     				    g_touchI2c_RstPin

#define TOUCH_SCL_GPIO_PORT                     g_touchI2c_SclPort
#define TOUCH_SDA_GPIO_PORT                     g_touchI2c_SdaPort
#define TOUCH_IRQ_GPIO_PORT                     g_touchI2c_IrqPort
#define TOUCH_RST_GPIO_PORT                     g_touchI2c_RstPort



#define Touch_I2C_WR	0		/* Write data. */
#define Touch_I2C_RD	1		/* Read data. */    
    
static void Touch_I2C_Delay(void);//I2C delay
void Touch_I2C_Start(void);// i2c start
void Touch_I2C_Stop(void);//i2c stop
void Touch_I2C_SendByte(uint8_t ucByte);//send one byte by i2c
uint8_t Touch_I2C_ReadByte(void);//read one byte by i2c

uint8_t Touch_I2C_WaitAck(void);//i2c wait acknowledge

void Touch_I2C_Ack(void);//send i2c acknowledge
void Touch_I2C_NoAck(void);//no i2c acknowledge
void Touch_I2C_ConfGPIO(void);//config aw2083 gpio
void Touch_I2C_ConfI2C(void);

uint32_t Touch_I2C_ReadBytes(uint8_t deviceAddr,uint8_t* pBuffer, uint16_t NumByteToRead);
uint32_t Touch_I2C_WriteBytes(uint8_t deviceAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite);

//#endif

#ifdef __cplusplus 
}
#endif
#endif

/**
  ******************************************************************************
  * @file    gc90x_bt656.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file contains all the functions prototypes for the BT565 
  *          firmware library.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 Grephichina.com</center></h2>
  ******************************************************************************
  */

#ifndef GC90X_BT656_H
#define GC90X_BT656_H
  
#include <stdio.h> 
//#include "CPU1_CMSDK_CM0.h"
#include <assert.h>
//#include "CMSDK_CM0.h"
#include "core_rv32.h"    
#include "gc90xx.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup Vedio Capture Interrupt definition 
  * @{
  */
	
#define BT656_IT_C0DD										((uint32_t)0x00000001)
#define BT656_IT_WFFIFO										((uint32_t)0x00000100)
#define BT656_IT_FOSE										((uint32_t)0x00000200)
#define IS_BT656_IT(IT)										(((IT) == BT656_IT_C0DD) || \
																((IT) == BT656_IT_WFFIFO) || \
																((IT) == BT656_IT_FOSE))
//
//#define BT656_REGS_BASE             (0x4010B000)
//
//#define NumberOfChunnel             (1)
//
////#define ENABLE                        (0x1)
////#define DISABLE                       (0x0)
//#define COLOR_16BIT                   (0x0)
//#define COLOR_32BIT                   (0X1)
//#define ACTIVE_HIGH                   (0x1)
//#define ACTIVE_LOW                    (0x0)
#define SAMPLE_ODD_FIELD              (0x0)  //only sample odd field
#define SAMPLE_EVEN_FIELD             (0x1)  //only sample even field
#define SAMPLE_BOTH_FIELD             (0x2)  //sample both field
#define SAMPLE_FIELD_MASK             (0x300) 
//																				
//
//typedef struct
//{
//    __IO uint32_t Control;
//    __IO uint32_t InterruptControl;
//    __IO uint32_t InterruptStatus;
//    __IO uint32_t MaxSize;
//    __IO uint32_t CH0FieldParameter;
//    __IO uint32_t CH0BaseAddr;
//}BT656_REG_Type;
//
//#define BT656_REG    ((BT656_REG_Type*) BT656_REGS_BASE)
//
//typedef struct
//{
//    uint32_t Enable;
//    uint32_t ColorBit;
//    uint32_t CHnum;
//    uint32_t FieldSampleMode;
//    uint32_t FrameDrawDoneInterrupt;
//    uint32_t WriteFifoFullInterrrupt;
//    uint32_t MaxAddrSize;
//    uint32_t CH0FieldWidth;
//    uint32_t CH0FieldHeight;
//    uint32_t CH0FieldBaseAddr; 
//}BT656_InitTypeDef;


//void BT656_Init(BT656_InitTypeDef *BT656_InitStruct);
void BT656_Configurate(uint8_t videotype);
void BT656_SetBaseAddress(uint32_t address);
void BT656_Disable(void);
void BT656_Start(void);
uint8_t BT656_GetStartStatus(void);
void BT656_ResetTIStatus(uint32_t BT656_IT);
void BT656_SetSampleMode(uint8_t flag);
ITStatus BT656_GetITStatus(uint32_t BT656_IT);
void BT656_GetParam(uint16_t* Height, uint16_t* Width);

void Video_Cap_HandlerISR(void);

#ifdef __cplusplus
}
#endif

#endif

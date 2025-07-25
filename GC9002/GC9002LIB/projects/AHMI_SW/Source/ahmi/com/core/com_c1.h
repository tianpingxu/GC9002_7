/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : com_c1.h
*
* Description  : This file contains the interfaces of COM
*
* Environment  : MDK / PC Simulation
*
* Responsible  : 
*
* Guidelines   : 
*
* Template name: C++
*
* Data         : 2024-02-27
****************************************************************************/
#ifndef COM_C1_H
#define COM_C1_H

#ifdef __cplusplus 
extern "C" {
#endif

#include "com_ct.h"
#include "com_ci.h"
#include "com_ce.h"
#include "com_fifo.h"



#if COM_COMPILE_SWITCH


void COM_vInit(void);
int32_t COM_iUartAsynSend(uint8_t* buf, uint16_t len);
void COM_UART_DMA_RxCpltCallback(DMA_HandleTypeDef *hdma);
int32_t RxDataParser(uint8_t* buf, uint16_t len);



#endif


#ifdef __cplusplus 
}
#endif

#endif


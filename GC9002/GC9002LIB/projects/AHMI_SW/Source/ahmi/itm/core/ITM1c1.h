/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : ITM1c1.h
*
* Description  : This file contains the interfaces of ITM
*
* Environment  : MDK / PC Simulation
*
* Responsible  : Zhu Jiahai
*
* Guidelines   : 
*
* Template name: C++
*
* Data         : 2022-05-19
****************************************************************************/
#ifndef ITM_C1_H
#define ITM_C1_H

#ifdef __cplusplus 
extern "C" {
#endif

#include "ITM1ce.h"

void ITM_vIsrProc(void);
void ITM_vInit(void);
void ITM_vDeinit(void);
void ITM_vMain(void*);


#ifdef __cplusplus 
}
#endif

#endif


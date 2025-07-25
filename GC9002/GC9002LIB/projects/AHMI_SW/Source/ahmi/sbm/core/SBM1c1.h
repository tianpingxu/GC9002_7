/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : SBM1c1.h
*
* Description  : This file contains the interfaces of SBM
*
* Environment  : MDK / PC Simulation
*
* Responsible  : Zhu Jiahai
*
* Guidelines   : 
*
* Template name: C++
*
* Data         : 2022-07-21
****************************************************************************/
#ifndef SBM_C1_H
#define SBM_C1_H

#ifdef __cplusplus 
extern "C" {
#endif

#include "SBM1ce.h"


void SBM_vEnterCpu1HardwareOp(void);
void SBM_vExitCpu1HardwareOp(void);
void SBM_vEnterCpu0HardwareOp(void);
void SBM_vExitCpu0HardwareOp(void);
void SBM_vSetCpu1HardwareOpState(uint8_t state);
void SBM_vSetCpu0HardwareOpState(uint8_t state);




#ifdef __cplusplus 
}
#endif

#endif


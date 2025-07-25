/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : player1c1.h
*
* Description  : This file contains the interfaces of PLAR
*
* Environment  : MDK / PC Simulation
*
* Responsible  : Zhu Jiahai
*
* Guidelines   : 
*
* Template name: C++
*
* Data         : 2022-09-23
****************************************************************************/
#ifndef PLAR_C1_H
#define PLAR_C1_H

#ifdef __cplusplus 
extern "C" {
#endif

#include "player1ce.h"
#include "player1ct.h"
#include "player1ci.h"




void PLAR_vInit(void);
void PLAR_vMainTask(void* pvParameters);
	
int32_t PLAR__vCmdMouseTouch(PIDPoint* pPressPoint, PIDPoint* pReleasePoint, uint32_t event, u8 screenType);
int32_t PLAR_vStartPlay(void);




/*
GetCurrentPage(pPage)	
GetCurrentCanvas(pCanvas)
GetCurrentSubCanvas(pSubCanvas)
GetCurrentPlayStatus(pStatus)
*/


#ifdef __cplusplus 
}
#endif

#endif


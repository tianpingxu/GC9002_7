/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : adcshare_c1.h
*
* Description  : This file contains the interfaces of ADCSHARE
*
* Environment  : MDK / PC Simulation
*
* Responsible  : 
*
* Guidelines   : 
*
* Template name: C++
*
* Data         : 2023-12-18
****************************************************************************/
#ifndef ADCSHARE_C1_H
#define ADCSHARE_C1_H

#ifdef __cplusplus 
extern "C" {
#endif

#include "adcshare_ct.h"
#include "adcshare_ci.h"
#include "adcshare_ce.h"


void ADCSHR_vTrigCycleSample(void);
uint32_t ADCSHR_u32RegisterChannel(uint8_t channel, uint8_t continousSampleTimes);
uint32_t ADCSHR_u32UnRegisterChannel(uint8_t channel);
uint32_t ADCSHR_u32GetChannelDataImmediately(uint32_t channel, uint16_t* buffer, uint32_t len);
uint32_t ADCSHR_u32GetChannelDataWait(uint32_t channel, uint16_t* buffer, uint32_t len, uint16_t timeoutMs);
uint32_t ADCSHR_u32PrioGetChannelData(uint32_t channel, uint16_t* buffer, uint32_t len);
void ADCSHR_vInit(void);







#ifdef __cplusplus 
}
#endif

#endif


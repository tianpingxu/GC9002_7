/**
  ******************************************************************************
  * @file    srv_timer_c1.h
  * @author  Zhu Jiahai
  * @version V1.0.0
  * @date    08-07-2021
  * @brief   The head file of Timer serve module.
  *          
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SRV_TIMER_C1_H
#define __SRV_TIMER_C1_H

//#ifdef __cplusplus
// extern "C" {
//#endif
	
/* Includes ------------------------------------------------------------------*/
#include "srv_timer_ct.h"
#include "srv_timer_ci.h"



bool SRV_TIMER_bIfTimeout(uint8_t u8Index);

int32_t SRV_TIMER_i32SetTimer(uint8_t u8Index, uint32_t u32Ms, CallbackHandler cbHandler);

int32_t SRV_TIMER_i32StopTimer(uint8_t u8Index);

void SRV_TIMER_vInit(void);

void SRV_TIMER_vDeInit(void);

void SRV_TIMER_vMainProcess(void);

uint32_t SRV_TIMER_u32GetTimerTick(void);

	 

//#ifdef __cplusplus
//}
//#endif

#endif /* __SRV_TIMER_C1_H */





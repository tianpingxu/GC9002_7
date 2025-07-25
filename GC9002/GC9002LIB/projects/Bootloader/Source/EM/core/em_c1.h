/**
  ******************************************************************************
  * @file    em_c1.h
  * @author  Zhu Jiahai
  * @version V1.0.0
  * @date    12-07-2021
  * @brief   The head file of Error Mangement module.
  *          
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EM_C1_H
#define __EM_C1_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"
#include "ff.h"

#include "em_ct.h"
#include "em_ci.h"


void EM_vInit(void);
void EM_vLog(uint16_t u16ErrType, uint8_t u8ErrLevel, uint8_t u8ModuleId, uint32_t u32ExtroInfo);
uint8_t* EV_pu8GetErrDescription(uint16_t u16ErrType);
bool EM_bIfErrorExisted(void);
void EM_vPrintErrorInfoToFile(FIL* fp);

extern uint32_t SCREEN_WIDTH;
extern uint32_t SCREEN_HEIGHT;
	 

#ifdef __cplusplus
}
#endif

#endif /* __EM_C1_H */





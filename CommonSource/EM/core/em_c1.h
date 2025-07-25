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
#include "em_ct.h"
#include "em_ci.h"


void EM_vInit(void);
void EM_vLog(uint16_t u16ErrType, uint16_t u16ErrLevel, uint16_t u16PageID, uint16_t u16CanvasID, uint16_t u16SubcanvasID, uint16_t u16WidgetID, uint16_t u16TagID, uint32_t u32TagValue);
uint8_t* EV_pu8GetErrDescription(uint16_t u16ErrType);
EM_tenErrBool EM_bIfErrorExisted(void);
void EM_vPrintErrorInfoToFile(void);

void ErrorManagementTask(void);

#ifdef __cplusplus
}
#endif

#endif /* __EM_C1_H */





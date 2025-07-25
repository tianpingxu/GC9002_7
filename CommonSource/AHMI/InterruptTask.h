////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMIÐ¡×é³ÉÔ±
//
// Create Date:   2016/03/21
// File Name:     DynamicPageClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/08/11 by Zhou Yuzhi
// Additional Comments:
//    handle the interruption
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef INTERRUPT_TASK__H
#define INTERRUPT_TASK__H
#include "FreeRTOS.h"
#include "semphr.h"
#include "publicDefine.h"

#ifdef AHMI_CORE
#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  CVBS refresh interrupt service function
  * @note
  * @retval None
  */
void ExtiTrigCvbsHandler( void );

/**
* the task to change the write address of cvbs data
* changes every time cvbs interrupt comes
*/
void interuptTask(void* pvParameters);

	
/**
* the task to calibration the resolution
*/
void calibrationTask(void* pvParameters);

#if defined(CHIP_GC9005) || defined(PC_SIM)
/**
* the task to decode the h264 data
*/
void h264PlayTask(void* pvParameters);
#endif

#ifdef __cplusplus
}
#endif	
	
	
#endif

#endif

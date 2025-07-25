/**
  ******************************************************************************
  * @file    gc9002_hal_pmu.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2023-1-6
  * @brief   This file contains all the functions prototypes for the PMU
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC9002_HAL_PMU_H__
#define __GC9002_HAL_PMU_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal_def.h"

#ifdef HAL_PMU_MODULE_ENABLED

#define TEST_ADC_ARRAY_COUNT    20

HAL_StatusTypeDef HAL_PMU_VTR12_Calibrate(uint16_t threshold);
HAL_StatusTypeDef HAL_PMU_VTR3318_Calibrate(uint16_t threshold);

#endif/* HAL_PMU_MODULE_ENABLED */
#ifdef __cplusplus
 }
#endif
#endif // __GC9002_HAL_PMU_H__

/**
  ******************************************************************************
  * @file    gc9002_hal_crc32.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2023-1-6
  * @brief   This file contains all the functions prototypes for the CRC32
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC9002_HAL_CRC32_H__
#define __GC9002_HAL_CRC32_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal_def.h"

#ifdef HAL_CRC32_MODULE_ENABLED


HAL_StatusTypeDef HAL_CRC32_Begin(uint32_t *addr, uint32_t len);
HAL_StatusTypeDef HAL_CRC32_GetResult(uint32_t *result, uint32_t Timeout);

#endif/* HAL_CRC32_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC9002_HAL_CRC32_H__ */

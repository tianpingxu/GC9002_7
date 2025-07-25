/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-08-22     huyt         the first version
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC90XX_H__

#define __GC90XX_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#if(defined CHIP_GC9002)
#include "gc9002.h"
#elif(defined CHIP_GC9005)
#include "gc9005.h"
#elif(defined CHIP_GC9005U)
#include <gc9005u.h>
#endif

/** @addtogroup Exported_types
  * @{
  */
typedef enum
{
    RESET = 0U,
    SET = !RESET
} FlagStatus, ITStatus;

typedef enum
{
    DISABLE = 0U,
    ENABLE = !DISABLE
} FunctionalState;

#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum
{
    SUCCESS = 0U,
    ERROR = !SUCCESS
} ErrorStatus;

enum
{
    TRUE = 1,
    FALSE = 0
};


#if defined (USE_HAL_DRIVER)
 #include <gc90xx_hal.h>
#endif /* USE_HAL_DRIVER */

#ifdef __cplusplus
}
#endif

#endif /* __GC90XX_H__ */

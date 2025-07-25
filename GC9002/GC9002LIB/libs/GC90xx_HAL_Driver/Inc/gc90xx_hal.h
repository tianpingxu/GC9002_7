/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-25     huyt         the first version
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC90XX_HAL_H__
#define __GC90XX_HAL_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "gc90xx_hal_conf.h"

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t* file, uint32_t line);
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed. 
  *         If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
#else
  #define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */


typedef enum
{
  HAL_TICK_FREQ_10HZ         = 100U,
  HAL_TICK_FREQ_100HZ        = 10U,
  HAL_TICK_FREQ_1KHZ         = 1U,
  HAL_TICK_FREQ_DEFAULT      = HAL_TICK_FREQ_1KHZ
} HAL_TickFreqTypeDef;

/**
  * @brief Provides a tick value in millisecond.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @retval tick value
  */
uint32_t HAL_GetTick(void);
void HAL_Delay(uint32_t Delay);

#define WORD_ALIGN_2B_MASK              0x1U /* Mask the first 1 bits */
#define WORD_ALIGN_4B_MASK              0x3U /* Mask the first 2 bits */
#define WORD_ALIGN_64B_MASK             0x3FU /* Mask the first 6 bits */
#define IS_ADDR_ALIGNED(ADDR, MASK)     (((uint32_t)(ADDR) & (MASK)) == 0U)
#define IS_ADDR_ALIGNED_2B(ADDR)        (((uint32_t)(ADDR) & (WORD_ALIGN_2B_MASK)) == 0U)
#define IS_ADDR_ALIGNED_4B(ADDR)        (((uint32_t)(ADDR) & (WORD_ALIGN_4B_MASK)) == 0U)
#define IS_ADDR_ALIGNED_64B(ADDR)       (((uint32_t)(ADDR) & (WORD_ALIGN_64B_MASK)) == 0U)


__attribute__((always_inline)) static inline void WriteUint32_LittleEndian(uint8_t *pbuff, uint32_t data)
{
    pbuff[0] =  data & 0xFF;
    pbuff[1] = (data >>  8) & 0xFF;
    pbuff[2] = (data >> 16) & 0xFF;
    pbuff[3] = (data >> 24) & 0xFF;
}
__attribute__((always_inline)) static inline uint32_t ReadUint32_LittleEndian(uint8_t *pbuff)
{
    return ((pbuff[3] << 24) | \
            (pbuff[2] << 16) | \
            (pbuff[1] <<  8) | \
             pbuff[0]);
}
__attribute__((always_inline)) static inline void WriteUint32_BigEndian(uint8_t *pbuff, uint32_t data)
{
    pbuff[0] = (data >> 24) & 0xFF;
    pbuff[1] = (data >> 16) & 0xFF;
    pbuff[2] = (data >>  8) & 0xFF;
    pbuff[3] =  data & 0xFF;
}
__attribute__((always_inline)) static inline uint32_t ReadUint32_BigEndian(uint8_t *pbuff)
{
    return ((pbuff[0] << 24) | \
            (pbuff[1] << 16) | \
            (pbuff[2] <<  8) | \
             pbuff[3]);
}

__attribute__((always_inline)) static inline void WriteUint16_LittleEndian(uint8_t *pbuff, uint16_t data)
{
    pbuff[0] =  data & 0xFF;
    pbuff[1] = (data >> 8) & 0xFF;
}
__attribute__((always_inline)) static inline uint16_t ReadUint16_LittleEndian(uint8_t *pbuff)
{
    return ((pbuff[1] << 8) | pbuff[0]);
}
__attribute__((always_inline)) static inline void WriteUint16_BigEndian(uint8_t *pbuff, uint16_t data)
{
    pbuff[0] = (data >> 8) & 0xFF;
    pbuff[1] =  data & 0xFF;
}
__attribute__((always_inline)) static inline uint16_t ReadUint16_BigEndian(uint8_t *pbuff)
{
    return ((pbuff[0] << 8) | pbuff[1]);
}

#ifdef __cplusplus
}
#endif

#endif /* __GC90XX_HAL_H__ */

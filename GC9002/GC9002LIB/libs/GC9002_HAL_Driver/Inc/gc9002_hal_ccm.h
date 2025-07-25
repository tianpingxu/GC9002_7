/**
  ******************************************************************************
  * @file    gc9002_hal_ccm.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2023-1-6
  * @brief   This file contains all the functions prototypes for the Dcache
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC9002_HAL_CCM_H__
#define __GC9002_HAL_CCM_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal_def.h"

#ifdef HAL_CCM_MODULE_ENABLED

#define __HAL_CCM_DRIVER_ATTRIBUTE    __attribute__((optimize(0)))

/* CCM(Cache Control and Mantainence) 
 */
#define CACHE_LINE_SIZE         (64UL)


void ccm_EnableDCache(void);

__ALWAYS_STATIC_INLINE void ccm_DisableDCache(void)
{
#if (defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U))
    RCC->DCACHE_SRAM_SHARE.bit.dcache_share_en = 0;
#endif/* (defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)) */
}

__ALWAYS_STATIC_INLINE bool ccm_IsDCacheEnabled(void)
{
	return (RCC->DCACHE_SRAM_SHARE.bit.dcache_share_en == 1U && (DCACHE->CTRL & 0x1) == 0);
}

void ccm_FlushDCache(void);

/**
    \brief   Invalidate D-Cache
    \details Invalidates D-Cache
*/
__ALWAYS_STATIC_INLINE void ccm_InvalidateDCache(void)
{
#if (defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U))
    DCACHE->CTRL |= 0x4;
#endif/* (defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)) */
}

void ccm_FlushInvalidateDCache(void);
void ccm_FlushDCache_by_Addr(void *addr, int32_t dsize);
void ccm_InvalidateDCache_by_Addr(void *addr, int32_t dsize);
void ccm_FlushInvalidateDCache_by_Addr(void *addr, int32_t dsize);

#endif/* HAL_CCM_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC9002_HAL_CCM_H__ */

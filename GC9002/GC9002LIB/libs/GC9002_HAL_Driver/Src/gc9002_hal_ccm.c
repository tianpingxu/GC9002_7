/*
 * Copyright (c) 2022-2050, GraphichinaSX Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-26     huyt         V1.0.1
 */
#include "gc90xx_hal.h"

#ifdef HAL_CCM_MODULE_ENABLED

/* ##########################  Cache functions  #################################### */
__HAL_CCM_DRIVER_ATTRIBUTE void ccm_EnableDCache(void)
{
#if (defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U))
	RCC->DCACHE_SRAM_SHARE.bit.dcache_share_en = 1;
	DCACHE->CTRL = 0;
	while((DCACHE->CTRL & 0x1) != 0);
    //The DCache cannot be flushed empty, so perform a read/write operation
    unsigned int tempreg = *((unsigned int *)HYPER_DATA_BASE);
	*((unsigned int *)HYPER_DATA_BASE) = tempreg;
#endif/* (defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)) */
}

/**
    \brief   Flush D-Cache
    \details Flushs D-Cache
*/
__HAL_CCM_DRIVER_ATTRIBUTE void ccm_FlushDCache(void)
{
#if (defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U))
    DCACHE->ADDR = 0;
    DCACHE->RANGE = 0xFFFFFFFF;
    DCACHE->CTRL |= 0x2;
    while((DCACHE->CTRL & 0x2) != RESET);
#endif/* (defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)) */
}


/**
    \brief   Flush & Invalidate D-Cache
    \details Flushs and Invalidates D-Cache
*/
__HAL_CCM_DRIVER_ATTRIBUTE void ccm_FlushInvalidateDCache(void)
{
#if (defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U))
    DCACHE->ADDR = 0;
    DCACHE->RANGE = 0xFFFFFFFF;
    DCACHE->CTRL |= 0x2;
    while((DCACHE->CTRL & 0x2) != RESET);
    DCACHE->CTRL |= 0x4;
#endif/* (defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)) */
}

/**
    \brief   D-Cache Flush by address
    \details Flushs D-Cache for the given address
    \param[in]   addr    address (aligned to 32-byte boundary)
    \param[in]   dsize   size of memory block (in number of bytes)
*/
__HAL_CCM_DRIVER_ATTRIBUTE void ccm_FlushDCache_by_Addr(void *addr, int32_t dsize)
{
#if (defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U))
    DCACHE->ADDR = (uint32_t)addr;
    DCACHE->RANGE = (((uint32_t)addr)%CACHE_LINE_SIZE)  + dsize;//Consider address no CACHE_LINE_SIZE byte alignment
    DCACHE->CTRL |= 0x2;
    while((DCACHE->CTRL & 0x2) != RESET);
#endif/* (defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)) */
}

/**
    \brief   D-Cache Invalidate by address
    \details Invalidates D-Cache for the given address
    \param[in]   addr    address (aligned to 32-byte boundary)
    \param[in]   dsize   size of memory block (in number of bytes)
*/
__HAL_CCM_DRIVER_ATTRIBUTE void ccm_InvalidateDCache_by_Addr(void *addr, int32_t dsize)
{
    ccm_FlushInvalidateDCache_by_Addr(addr, dsize);
}

/**
    \brief   D-Cache Flush and Invalidate by address
    \details Flush and Invalidates D-Cache for the given address
    \param[in]   addr    address (aligned to 32-byte boundary)
    \param[in]   dsize   size of memory block (in number of bytes)
*/
__HAL_CCM_DRIVER_ATTRIBUTE void ccm_FlushInvalidateDCache_by_Addr(void *addr, int32_t dsize)
{
#if (defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U))
    DCACHE->ADDR = (uint32_t)addr;
    DCACHE->RANGE = (((uint32_t)addr)%CACHE_LINE_SIZE)  + dsize;//Consider address no CACHE_LINE_SIZE byte alignment
    DCACHE->CTRL |= 0x2;
    while((DCACHE->CTRL & 0x2) != RESET);
    DCACHE->CTRL |= 0x4;
#endif/* (defined (__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)) */
}


#endif /* HAL_CCM_MODULE_ENABLED */
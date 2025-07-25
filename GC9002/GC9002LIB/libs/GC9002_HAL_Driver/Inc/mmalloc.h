/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-08-19     huyt        the first version
 */
#ifndef __MMALLOC_H__
#define __MMALLOC_H__

#ifdef __cplusplus
 extern "C" {
#endif
#include "gc90xx_hal_conf.h"
#include "stdint.h"

// #define HAL_MMALLOC_ENABLED

#ifndef CACHE_LINE_SIZE
#define CACHE_LINE_SIZE                         (64UL)
#endif

#define _STATIC_MEMORY_BLOCK_COUNT              (64UL)
#define _STATIC_MEMORY_ALIGNED_SIZE             CACHE_LINE_SIZE
#define _STATIC_MEMORY_BUFF_SIZE                (_STATIC_MEMORY_ALIGNED_SIZE*_STATIC_MEMORY_BLOCK_COUNT)/* µ¥Î»£º×Ö½Ú£¨byte£© */



void* mmalloc(unsigned int size);
void mfree(void* addr);

#ifdef __cplusplus
}
#endif

#endif /* __MMALLOC_H__ */

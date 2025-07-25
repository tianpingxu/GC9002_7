/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-08-19     huyt        the first version
 */

#include "stdlib.h"
#include "string.h"
#include "mmalloc.h"
#ifdef HAL_MMALLOC_ENABLED

#ifndef CACHE_LINE_SIZE
#define CACHE_LINE_SIZE                         (64UL)
#endif

#define LOCK_COUNT      0x1FFF

__attribute__((aligned(CACHE_LINE_SIZE))) static char static_memory[_STATIC_MEMORY_BLOCK_COUNT][_STATIC_MEMORY_ALIGNED_SIZE];

struct memory_ctl_type
{
    unsigned int mFreeSize; /*剩余内存空间大小，单位字节（byte）*/
    unsigned int mlock;
    uint64_t mUseFlag;
    unsigned char mCount[_STATIC_MEMORY_BLOCK_COUNT];
};

static struct memory_ctl_type _mm_ctl = { _STATIC_MEMORY_BUFF_SIZE, 0, 0, };

static inline void memory_lock(void)
{
    _mm_ctl.mlock = LOCK_COUNT;
}
static inline void memory_unlock(void)
{
    _mm_ctl.mlock = 0;
}
static int memory_lock_wait(void)
{
    unsigned int i = 0;
    while(_mm_ctl.mlock != 0)
    {
        if(++i > LOCK_COUNT)
        {
            return 1;
        }
    }
    return 0;
}

static int get_index(void* addr)
{
    for (int i = 0; i < _STATIC_MEMORY_BLOCK_COUNT; i++)
    {
        if ((unsigned int)addr == (unsigned int)(&(static_memory[i][0])))
        {
            return i;
        }
    }
    return -1;
}

void* mmalloc(unsigned int size)
{
#ifdef USE_RTTHREAD
    rt_base_t level;
#endif
    char* ret_ptr = (char*)0;
    unsigned int count = 0;
    unsigned int i = 0;
    uint64_t temp_flag = 0;
    unsigned int temp_size = 0;

    count = (size / CACHE_LINE_SIZE);
    count += ((size % CACHE_LINE_SIZE) > 0 ? 1 : 0);
    temp_size = count * CACHE_LINE_SIZE;
    if(memory_lock_wait() != 0)
    {
        return (void*)0;
    }
    memory_lock();
    if (count > 0 && size <= _mm_ctl.mFreeSize)
    {
        for (i = 0; i < count; ++i)
        {
            temp_flag |= (0x1UL << i);
        }
        for (i = 0; i < _STATIC_MEMORY_BLOCK_COUNT; ++i)
        {
            if ((_mm_ctl.mUseFlag & (temp_flag << i)) == 0)
            {
#ifdef USE_RTTHREAD
#include "rthw.h"
                level = rt_hw_interrupt_disable();
#endif
                _mm_ctl.mUseFlag |= (temp_flag << i);
                _mm_ctl.mFreeSize -= temp_size;
                _mm_ctl.mCount[i] = count;
#ifdef USE_RTTHREAD
                rt_hw_interrupt_enable(level);
#endif
                ret_ptr = &static_memory[i][0];
                memset((void *)ret_ptr, 0, temp_size);
                memory_unlock();
                return (void*)ret_ptr;
            }
        }
    }
    memory_unlock();
    return (void*)0;
}
void mfree(void* addr)
{
#ifdef USE_RTTHREAD
    rt_base_t level;
#endif
    uint64_t temp_flag = 0;
    unsigned int temp_size = 0;

    if(!addr)
    {
        return;
    }
    if(memory_lock_wait() != 0)
    {
        return;
    }
    memory_lock();
    int index = get_index(addr);
    if(index >= 0)
    {
        temp_size = _mm_ctl.mCount[index] * CACHE_LINE_SIZE;
        for (int i = 0; i < _mm_ctl.mCount[index]; ++i)
        {
            temp_flag |= (0x1UL << (index + i));
        }
#ifdef USE_RTTHREAD
        level = rt_hw_interrupt_disable();
#endif
        _mm_ctl.mUseFlag &= ~temp_flag;
        _mm_ctl.mFreeSize += temp_size;
        _mm_ctl.mCount[index] = 0;
#ifdef USE_RTTHREAD
        rt_hw_interrupt_enable(level);
#endif
        memset((void *)&(static_memory[index][0]), 0, temp_size);
    }
    memory_unlock();
}


#endif /* HAL_MMALLOC_ENABLED */

/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */

#ifndef __RT_DEF_H__
#define __RT_DEF_H__

/* include rtconfig header to import configuration */
#include <rtconfig.h>
#include "gc90xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RT_SYSTEM_WORKQUEUE_STACKSIZE 2048
#define DFS_FD_MAX 300


/* RT-Thread error code definitions */
#define RT_EOK                          0               /**< There is no error */
#define RT_ERROR                        1               /**< A generic error happens */
#define RT_ETIMEOUT                     2               /**< Timed out */
#define RT_EFULL                        3               /**< The resource is full */
#define RT_EEMPTY                       4               /**< The resource is empty */
#define RT_ENOMEM                       5               /**< No memory */
#define RT_ENOSYS                       6               /**< No system */
#define RT_EBUSY                        7               /**< Busy */
#define RT_EIO                          8               /**< IO error */
#define RT_EINTR                        9               /**< Interrupted system call */
#define RT_EINVAL                       10              /**< Invalid argument */


#define RT_NULL                         NULL
#define RT_ASSERT                       assert_param
#define RT_FALSE                        (0)
#define RT_TRUE                         (1)

/* RT-Thread basic data type definitions */
#ifndef RT_USING_ARCH_DATA_TYPE
typedef signed   char                   rt_int8_t;      /**<  8bit integer type */
typedef signed   short                  rt_int16_t;     /**< 16bit integer type */
typedef signed   int                    rt_int32_t;     /**< 32bit integer type */
typedef unsigned char                   rt_uint8_t;     /**<  8bit unsigned integer type */
typedef unsigned short                  rt_uint16_t;    /**< 16bit unsigned integer type */
typedef unsigned int                    rt_uint32_t;    /**< 32bit unsigned integer type */

#ifdef ARCH_CPU_64BIT
typedef signed long                     rt_int64_t;     /**< 64bit integer type */
typedef unsigned long                   rt_uint64_t;    /**< 64bit unsigned integer type */
#else
typedef signed long long                rt_int64_t;     /**< 64bit integer type */
typedef unsigned long long              rt_uint64_t;    /**< 64bit unsigned integer type */
#endif
#endif

typedef int                             rt_bool_t;      /**< boolean type */
typedef int                             rt_base_t;      /**< Nbit CPU related date type */
typedef unsigned int                    rt_ubase_t;     /**< Nbit unsigned CPU related data type */

typedef rt_base_t                       rt_err_t;       /**< Type for error number */
typedef rt_uint32_t                     rt_time_t;      /**< Type for time stamp */
typedef rt_uint64_t                     rt_tick_t;      /**< Type for tick count */
typedef rt_base_t                       rt_flag_t;      /**< Type for flags */
typedef rt_ubase_t                      rt_size_t;      /**< Type for size number */
typedef rt_ubase_t                      rt_dev_t;       /**< Type for device */
typedef rt_base_t                       rt_off_t;       /**< Type for offset */


#define rt_inline                   static __inline

#define RT_USING_EVENT
#ifdef RT_USING_EVENT
/**
 * flag defintions in event
 */
#define RT_EVENT_FLAG_AND               0x01            /**< logic and */
#define RT_EVENT_FLAG_OR                0x02            /**< logic or */
#define RT_EVENT_FLAG_CLEAR             0x04            /**< clear flag */
#endif

/**
 * IPC flags and control command definitions
 */
#define RT_IPC_FLAG_FIFO                0x00            /**< FIFOed IPC. @ref IPC. */
#define RT_IPC_FLAG_PRIO                0x01            /**< PRIOed IPC. @ref IPC. */

#define RT_IPC_CMD_UNKNOWN              0x00            /**< unknown IPC command */
#define RT_IPC_CMD_RESET                0x01            /**< reset IPC object */


/**
 * Single List structure
 */
struct rt_slist_node
{
    struct rt_slist_node *next;                         /**< point to next node. */
};
typedef struct rt_slist_node rt_slist_t;                /**< Type for single list. */

/**
 * Double List structure
 */
struct rt_list_node
{
    struct rt_list_node *next;                          /**< point to next node. */
    struct rt_list_node *prev;                          /**< point to prev node. */
};
typedef struct rt_list_node rt_list_t;                  /**< Type for lists. */



#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif

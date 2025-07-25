
#ifndef __OS_DEF_H__
#define __OS_DEF_H__

#include "stdio.h"
#include "FreeRTOS.h"
#include "csi_kernel.h"
#include "semphr.h"
#include "stddef.h"
#include "malloc.h"
#include "string.h"
#include "ringbuffer.h"
#include "at_config.h"
#include "rtdef.h"
#include "csi_uart.h"
#include "event_groups.h"

#ifdef __cplusplus
extern "C" {
#endif
#define RT_DEVICE_NAME_MAX      8
#define RT_TICK_PER_SECOND      CONFIG_SYSTICK_HZ

typedef SemaphoreHandle_t       rt_sem_t;
typedef SemaphoreHandle_t       rt_mutex_t;
typedef TaskHandle_t            rt_thread_t;
typedef EventGroupHandle_t      rt_event_t;

typedef struct ringbuffer       rt_ringbuffer;
typedef rt_ringbuffer*          rt_ringbuffer_t;

#define RT_WAITING_FOREVER      portMAX_DELAY             /**< Block forever until get resource. */
#define RT_WAITING_NO           0               /**< Non-block. */

#define rt_tick_from_millisecond(ms)    (ms)
#define rt_tick_get             HAL_GetTick

#define rt_malloc               malloc
#define rt_calloc               calloc
#define rt_free                 free
#define rt_realloc              realloc

#define rt_memcpy               memcpy
#define rt_memset               memset
#define rt_memcmp               memcmp
#define rt_strcmp               strcmp
#define rt_strncmp              strncmp
#define rt_strncpy              strncpy
#define rt_strlen               strlen
#define rt_snprintf             snprintf
#define rt_sprintf              sprintf

#define LOG_W                   printf
#define LOG_D                   printf
#define LOG_I                   printf
#define LOG_E                   printf
#define LOG_RAW                 printf

#define rt_kprintf              printf

#define rt_thread_delay         csi_kernel_delay
#define rt_thread_mdelay        csi_kernel_delay

#if defined (__GNUC__)                /* GNU GCC Compiler */
    /* the version of GNU GCC must be greater than 4.x */
    typedef __builtin_va_list       __gnuc_va_list;
    typedef __gnuc_va_list          va_list;
    #define va_start(v,l)           __builtin_va_start(v,l)
    #define va_end(v)               __builtin_va_end(v)
    #define va_arg(v,l)             __builtin_va_arg(v,l)
#endif

#if(defined CONFIG_AT_CLIENT_ENABLE && CONFIG_AT_CLIENT_ENABLE == 1U)

struct device
{
    char name[RT_DEVICE_NAME_MAX];                       /**< name of kernel object */
    char rx_buff[AT_DEVICE_RX_BUFFER_SIZE];
    struct ringbuffer *rb_p;
    rt_uint16_t put_index;
    rt_uint16_t get_index;
    rt_bool_t is_full;
    rt_uint32_t oflag;
    csi_uart_t uart;
    
#if((defined AT_DEVICE_RX_USING_DMA && AT_DEVICE_RX_USING_DMA == 1) || \
    (defined AT_DEVICE_TX_USING_DMA && AT_DEVICE_TX_USING_DMA == 1))
    csi_dma_ctrl_t dma_ctrl;
#endif
#if(defined AT_DEVICE_RX_USING_DMA && AT_DEVICE_RX_USING_DMA == 1)
    csi_dma_ch_t rx_dma_ch;
    // char *rx_dma_buff;
    char rx_dma_buff[AT_DEVICE_RX_DMA_BUFFER_SIZE];
#endif
#if(defined AT_DEVICE_TX_USING_DMA && AT_DEVICE_TX_USING_DMA == 1)
    csi_dma_ch_t tx_dma_ch;
#endif
    rt_err_t (*rx_indicate)(struct device* dev, rt_size_t size);
};
typedef struct device*      device_t;
    
/**
 * device flags defitions
 */
#define AT_DEVICE_FLAG_DEACTIVATE       0x000           /**< device is not not initialized */

#define AT_DEVICE_FLAG_RDONLY           0x001           /**< read only */
#define AT_DEVICE_FLAG_WRONLY           0x002           /**< write only */
#define AT_DEVICE_FLAG_RDWR             0x003           /**< read and write */

#define AT_DEVICE_FLAG_REMOVABLE        0x004           /**< removable device */
#define AT_DEVICE_FLAG_STANDALONE       0x008           /**< standalone device */
#define AT_DEVICE_FLAG_ACTIVATED        0x010           /**< device is activated */
#define AT_DEVICE_FLAG_SUSPENDED        0x020           /**< device is suspended */
#define AT_DEVICE_FLAG_STREAM           0x040           /**< stream mode */

#define AT_DEVICE_FLAG_INT_RX           0x100           /**< INT mode on Rx */
#define AT_DEVICE_FLAG_DMA_RX           0x200           /**< DMA mode on Rx */
#define AT_DEVICE_FLAG_INT_TX           0x400           /**< INT mode on Tx */
#define AT_DEVICE_FLAG_DMA_TX           0x800           /**< DMA mode on Tx */

#define AT_DEVICE_OFLAG_CLOSE           0x000           /**< device is closed */
#define AT_DEVICE_OFLAG_RDONLY          0x001           /**< read only access */
#define AT_DEVICE_OFLAG_WRONLY          0x002           /**< write only access */
#define AT_DEVICE_OFLAG_RDWR            0x003           /**< read and write */
#define AT_DEVICE_OFLAG_OPEN            0x008           /**< device is opened */
#define AT_DEVICE_OFLAG_MASK            0xf0f           /**< mask of open flag */

#endif /* CONFIG_AT_CLIENT_ENABLE */

#ifdef __cplusplus
}
#endif

#endif /* __OS_DEF_H__ */
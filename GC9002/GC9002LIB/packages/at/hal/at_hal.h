
#ifndef __AT_HAL_H__
#define __AT_HAL_H__

#include "osdef.h"
#include "csi_irq.h"
#include "csi_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

#if(defined CONFIG_AT_CLIENT_ENABLE && CONFIG_AT_CLIENT_ENABLE == 1U)


/**
 * IPC flags and control command definitions
 */
#define AT_IPC_FLAG_FIFO                0x00            /**< FIFOed IPC. @ref IPC. */
#define AT_IPC_FLAG_PRIO                0x01            /**< PRIOed IPC. @ref IPC. */

#define AT_TIMEOUT_MAX                  portMAX_DELAY


device_t    at_device_find(const char *name);
rt_err_t    at_device_open(device_t dev, rt_uint16_t oflag);
rt_size_t   at_device_write(device_t dev, rt_off_t pos, const void *buffer, rt_size_t size);
rt_size_t   at_device_read(device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
rt_err_t    at_device_close(device_t dev);
rt_err_t    at_device_set_rx_indicate(device_t dev, rt_err_t (*rx_ind)(device_t dev, rt_size_t size));
rt_err_t    at_device_restart_receive_async(device_t dev);



rt_ringbuffer_t rt_ringbuffer_create(rt_uint16_t size);
rt_size_t       rt_ringbuffer_getchar(rt_ringbuffer_t rb, rt_uint8_t *ch);
void            rt_ringbuffer_destroy(struct ringbuffer *rb);
rt_size_t       rt_ringbuffer_put_force(rt_ringbuffer_t rb, const rt_uint8_t *ptr, rt_uint16_t length);

device_t at_console_get_device(void);

#endif /* CONFIG_AT_CLIENT_ENABLE */

#ifdef __cplusplus
}
#endif

#endif /* __AT_HAL_H__ */
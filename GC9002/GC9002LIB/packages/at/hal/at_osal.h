
#ifndef __AT_OSAL_H__
#define __AT_OSAL_H__

#include "osdef.h"

#ifdef __cplusplus
extern "C" {
#endif

#if(defined CONFIG_AT_CLIENT_ENABLE && CONFIG_AT_CLIENT_ENABLE == 1U)


#define AT_THREAD_PRIORITY_MAX      configMAX_PRIORITIES
rt_thread_t at_osal_thread_create(const char *name,
                                 void (*entry)(void *parameter),
                                 void       *parameter,
                                 rt_uint32_t stack_size,
                                 rt_uint8_t  priority,
                                 rt_uint32_t tick);
rt_err_t at_osal_thread_startup(rt_thread_t thread);
rt_err_t at_osal_thread_delete(rt_thread_t thread);
void at_osal_task_wait(void);

rt_mutex_t at_osal_mutex_create(const char *name, rt_uint8_t flag);
rt_err_t   at_osal_mutex_take(rt_mutex_t mutex, rt_int32_t time);
rt_err_t   at_osal_mutex_release(rt_mutex_t mutex);
rt_err_t   at_osal_mutex_init(rt_mutex_t mutex, const char *name, rt_uint8_t flag);
rt_err_t   at_osal_mutex_delete(rt_mutex_t mutex);

#define SEM_Max_Count      0x10U
rt_err_t at_osal_sem_init(rt_sem_t  *sem_p,
                         const char *name,
                         rt_uint32_t value,
                         rt_uint8_t  flag);
rt_sem_t at_osal_sem_create(const char *name, rt_uint32_t value, rt_uint8_t flag);
rt_err_t at_osal_sem_take(rt_sem_t sem, rt_int32_t time);
rt_err_t at_osal_sem_trytake(rt_sem_t sem);
rt_err_t at_osal_sem_release(rt_sem_t sem);
rt_err_t at_osal_sem_release_isr(rt_sem_t sem);
rt_err_t at_osal_sem_delete(rt_sem_t sem);
rt_err_t at_osal_sem_detach(rt_sem_t sem);
rt_err_t at_osal_event_send(rt_event_t event, rt_uint32_t set);
rt_err_t at_osal_event_recv(rt_event_t   event,
                            rt_uint32_t  set,
                            rt_uint8_t   option,
                            rt_int32_t   timeout,
                            rt_uint32_t *recved);
rt_event_t at_osal_event_create(const char *name, rt_uint8_t flag);

void rt_enter_critical(void);
void rt_exit_critical(void);

void      rt_hw_interrupt_enable(rt_base_t level);
rt_base_t rt_hw_interrupt_disable(void);
void      at_osal_msleep(rt_uint32_t delay);

char *rt_strstr(const char *s1, const char *s2);

void os_thread_suspend(rt_thread_t thread);
void os_thread_resume(rt_thread_t thread);
void os_schedule(void);

#endif /* CONFIG_AT_CLIENT_ENABLE */

#ifdef __cplusplus
}
#endif

#endif /* __AT_OSAL_H__ */
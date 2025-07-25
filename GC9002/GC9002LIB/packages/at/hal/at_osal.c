#include "at_osal.h"
#if(defined CONFIG_AT_CLIENT_ENABLE && CONFIG_AT_CLIENT_ENABLE == 1U)

rt_thread_t at_osal_thread_create(const char *name,
                             void (*entry)(void *parameter),
                             void       *parameter,
                             rt_uint32_t stack_size,
                             rt_uint8_t  priority,
                             rt_uint32_t tick)
{
    rt_thread_t task = RT_NULL;
    BaseType_t xReturn;
    stack_size /= sizeof(StackType_t);
    xReturn = xTaskCreate(entry, name, stack_size, parameter, priority, &task);
    if(pdPASS != xReturn)
    {
        LOG_E("AT: xTaskCreate error.\n");
    }
    return task;
}
rt_err_t at_osal_thread_startup(rt_thread_t thread)
{
    xTaskNotifyGive(thread);
    return RT_EOK;
}
rt_err_t at_osal_thread_delete(rt_thread_t thread)
{
    vTaskDelete(thread);
    return RT_EOK;
}
void at_osal_task_wait(void)
{
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

rt_err_t at_osal_mutex_init(rt_mutex_t mutex, const char *name, rt_uint8_t flag)
{
    return RT_EOK;
}
rt_mutex_t at_osal_mutex_create(const char *name, rt_uint8_t flag)
{
    return xSemaphoreCreateMutex();
}
rt_err_t at_osal_mutex_take(rt_mutex_t mutex, rt_int32_t time)
{
    return (xSemaphoreTake(mutex, portMAX_DELAY) == pdPASS) ? RT_EOK : -RT_ETIMEOUT;
}
rt_err_t at_osal_mutex_release(rt_mutex_t mutex)
{
    return (xSemaphoreGive(mutex) == pdPASS) ? RT_EOK : RT_ETIMEOUT;
}
rt_err_t at_osal_mutex_delete(rt_mutex_t mutex)
{
    vSemaphoreDelete(mutex);
    return RT_EOK;
}

rt_err_t at_osal_sem_init(rt_sem_t  *sem_p,
                         const char *name,
                         rt_uint32_t value,
                         rt_uint8_t  flag)
{
    (void)name;
    (void)flag;
    *sem_p = xSemaphoreCreateCounting(SEM_Max_Count, value);
    return RT_EOK;
}
rt_sem_t at_osal_sem_create(const char *name, rt_uint32_t value, rt_uint8_t flag)
{
    (void)name;
    (void)flag;
    return xSemaphoreCreateCounting(SEM_Max_Count, value);
}
rt_err_t at_osal_sem_take(rt_sem_t sem, rt_int32_t time)
{
    return (xSemaphoreTake(sem, (time < 0 ? portMAX_DELAY : pdMS_TO_TICKS(time))) == pdPASS) ? RT_EOK : RT_ETIMEOUT;
}
rt_err_t at_osal_sem_trytake(rt_sem_t sem)
{
    return (xSemaphoreTake(sem, 0) == pdPASS) ? RT_EOK : RT_ETIMEOUT;
}
rt_err_t at_osal_sem_release(rt_sem_t sem)
{
    return (xSemaphoreGive(sem) == pdPASS) ? RT_EOK : RT_ERROR;
}
rt_err_t at_osal_sem_release_isr(rt_sem_t sem)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult = pdFAIL;
    xResult = xSemaphoreGiveFromISR(sem, &xHigherPriorityTaskWoken);
    if (xResult == pdPASS) {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    return (xResult == pdPASS) ? RT_EOK : RT_ERROR;
}
rt_err_t at_osal_sem_delete(rt_sem_t sem)
{
    vSemaphoreDelete(sem);
    return RT_EOK;
}
rt_err_t at_osal_sem_detach(rt_sem_t sem)
{
    return RT_EOK;
}
rt_err_t at_osal_event_send(rt_event_t event, rt_uint32_t set)
{
    rt_uint32_t event_result = xEventGroupSetBits(event, set);
    return RT_EOK;
}
rt_err_t at_osal_event_send_isr(rt_event_t event, rt_uint32_t set)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult = pdFAIL;
    xResult = xEventGroupSetBitsFromISR(event, set, &xHigherPriorityTaskWoken);
    // Was the message posted successfully?
    if( xResult == pdPASS )
    {
        // If xHigherPriorityTaskWoken is now set to pdTRUE then a context
        // switch should be requested.  The macro used is port specific and
        // will be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() -
        // refer to the documentation page for the port being used.
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    return (xResult == pdPASS) ? RT_EOK : RT_ERROR;
}
rt_err_t at_osal_event_recv(rt_event_t   event,
                            rt_uint32_t  set,
                            rt_uint8_t   option,
                            rt_int32_t   timeout,
                            rt_uint32_t *recved)
{
    *recved = xEventGroupWaitBits(event, set, //等待的event标志位
                                ((option & RT_EVENT_FLAG_CLEAR) ? pdTRUE : pdFALSE), //退出xEventGroupWaitBits之前，event标志位是否清除，若不清除，那么当3个标志位一起到来后，xEventGroupWaitBits持续激活态
                                ((option & RT_EVENT_FLAG_AND) ? pdTRUE : pdFALSE),   //event事件之间的关系,pdTRUE: &, pdFALSE：|
                                timeout);
    if(option & RT_EVENT_FLAG_AND)
    {
        if((*recved & set) != set)
        {
            return RT_ETIMEOUT;
        }
    }
    else
    {
        if((*recved & set) == 0)
        {
            return RT_ETIMEOUT;
        }
    }
    return RT_EOK;
}
rt_event_t at_osal_event_create(const char *name, rt_uint8_t flag)
{
    return xEventGroupCreate();
}


void rt_enter_critical(void)
{
    vTaskSuspendAll();
}
void rt_exit_critical(void)
{
    xTaskResumeAll();
}

void rt_hw_interrupt_enable(rt_base_t level)
{
    (void)level;
    taskENABLE_INTERRUPTS();
}
rt_base_t rt_hw_interrupt_disable(void)
{
    taskDISABLE_INTERRUPTS();
    return 1;
}
void at_osal_msleep(rt_uint32_t delay)
{
    vTaskDelay(pdMS_TO_TICKS(delay));
}

/**
 * This function will return the first occurrence of a string.
 *
 * @param s1 the source string
 * @param s2 the find string
 *
 * @return the first occurrence of a s2 in s1, or RT_NULL if no found.
 */
char *rt_strstr(const char *s1, const char *s2)
{
    int l1, l2;

    l2 = rt_strlen(s2);
    if (!l2)
        return (char *)s1;
    l1 = rt_strlen(s1);
    while (l1 >= l2)
    {
        l1 --;
        if (!rt_memcmp(s1, s2, l2))
            return (char *)s1;
        s1 ++;
    }

    return RT_NULL;
}


void os_thread_suspend(rt_thread_t thread)
{
    vTaskSuspend(thread);
}
void os_thread_resume(rt_thread_t thread)
{
    vTaskResume(thread);
}
void os_schedule(void)
{
    vTaskStartScheduler();
}
#endif /* CONFIG_AT_CLIENT_ENABLE */
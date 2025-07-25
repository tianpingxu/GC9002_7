/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_timer.h
 * @brief    header File for TIMER Driver
 * @version  V1.0
 * @date     2023/02/22
 * @model    timer
 ******************************************************************************/
#ifndef __CSI_TIMER_H__
#define __CSI_TIMER_H__

#include <csi_common.h>
#include <csi_irq.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_TIMER_ENABLED

#define CSI_TIMER_CNT       2   // Peripheral count

typedef struct _csi_timer
{
    csi_dev_t dev;      //csi Device unity handle
    void(*callback)(struct _csi_timer *timer, void *arg);//User callback function
    void *arg;          //Callback function argument
    void *priv;         //Device private variable
}csi_timer_t;

csi_error_t csi_timer_init(csi_timer_t *timer, uint32_t idx);
void        csi_timer_uninit(csi_timer_t *timer);

csi_error_t csi_timer_start(csi_timer_t *timer, uint32_t timeout_us);
csi_error_t csi_timer_start_autoreload(csi_timer_t *timer, uint32_t timeout_us);
void        csi_timer_stop(csi_timer_t *timer);

uint32_t    csi_timer_get_remaining_value(csi_timer_t *timer);
uint32_t    csi_timer_get_load_value(csi_timer_t *timer);

bool        csi_timer_is_running(csi_timer_t *timer);
csi_error_t csi_timer_attach_callback(csi_timer_t *timer, void(*callback)(struct _csi_timer *timer, void *args), void *arg);
void        csi_timer_detach_callback(csi_timer_t *timer);

#endif /* CONFIG_CSI_DRV_TIMER_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_TIMER_H__ */
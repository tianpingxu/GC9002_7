/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_wdt.h
 * @brief    header File for WDT Driver
 * @version  V1.0
 * @date     2023/02/24
 * @model    wdt
 ******************************************************************************/
#ifndef __CSI_WDT_H__
#define __CSI_WDT_H__

#include <csi_common.h>
#include <csi_irq.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_WDT_ENABLED

#define CSI_WDT_CNT    1   // Peripheral count


typedef struct _csi_wdt
{
    csi_dev_t dev;      //csi Device unity handle
    void(*callback)(struct _csi_wdt *wdt, void *arg);//User callback function
    void *arg;          //Callback function argument
    void *priv;         //Device private variable
}csi_wdt_t;

csi_error_t csi_wdt_init(csi_wdt_t *wdt, uint32_t idx);
csi_error_t csi_wdt_set_timeout(csi_wdt_t *wdt, uint32_t ms);
csi_error_t csi_wdt_start(csi_wdt_t *wdt);
void        csi_wdt_stop(csi_wdt_t *wdt);
csi_error_t csi_wdt_feed(csi_wdt_t *wdt);
uint32_t    csi_wdt_get_remaining_time(csi_wdt_t *wdt);
bool        csi_wdt_is_running(csi_wdt_t *wdt);
csi_error_t csi_wdt_attach_callback(csi_wdt_t *wdt, void(*callback)(struct _csi_wdt *wdt, void *arg), void *arg);
void        csi_wdt_detach_callback(csi_wdt_t *wdt);




#endif /* CONFIG_CSI_DRV_WDT_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_WDT_H__ */
/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_rtc.h
 * @brief    header File for RTC Driver
 * @version  V1.0
 * @date     2023/02/24
 * @model    rtc
 ******************************************************************************/
#ifndef __CSI_RTC_H__
#define __CSI_RTC_H__

#include <csi_common.h>
#include <csi_irq.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_RTC_ENABLED
#define CSI_RTC_CNT            1   // Peripheral count

typedef struct _csi_rtc
{
    csi_dev_t dev;      //csi Device unity handle
    void(*callback)(struct _csi_rtc *rtc, void *arg);//User callback function
    void *arg;          //Callback function argument
    RTC_AlarmTypeDef s_alarm;
    void *priv;         //Device private variable
}csi_rtc_t;

#define CSI_YEAR_BASE           1900
#define CSI_MONTH_BASE          1
/* 例：设置2020年1月13日23点59分59秒则如下配置：
 * rtctime.tm_year = 120 , 
 * rtctime.tm_mon = 0 , 
 * rtctime.tm_mday = 13 ,
 * rtctime.tm_hour = 23 , 
 * rtctime.tm_min = 59 , 
 * rtctime.tm_sec = 59
 * */
typedef struct
{
    int tm_sec;     //秒 Value range[0-59]
    int tm_min;     //分 Value range[0-59]
    int tm_hour;    //小时 Value range[0-23]
    int tm_mday;    //天 Value range[1-31]
    int tm_mon;     //月 Value range[0-11]
    int tm_year;    //年 Value range[70-199]
    int tm_wday;    //周 Value range[0-6]
    int tm_yday;    //一年中的第几天 Value range[0-365]
}csi_rtc_time_t;

csi_error_t csi_rtc_init(csi_rtc_t *rtc, uint32_t idx);
void        csi_rtc_uninit(csi_rtc_t *rtc);
csi_error_t csi_rtc_set_time(csi_rtc_t *rtc, const csi_rtc_time_t *rtctime);
csi_error_t csi_rtc_set_time_no_wait(csi_rtc_t *rtc, const csi_rtc_time_t *rtctime);
csi_error_t csi_rtc_get_time(csi_rtc_t *rtc, csi_rtc_time_t *rtctime);
uint32_t    csi_rtc_get_alarm_remaining_time(csi_rtc_t *rtc);
csi_error_t csi_rtc_set_alarm(csi_rtc_t *rtc, const csi_rtc_time_t *rtctime, void(*callback)(struct _csi_rtc *rtc, void *arg), void *arg);
csi_error_t csi_rtc_cancel_alarm(csi_rtc_t *rtc);
bool        csi_rtc_is_running(csi_rtc_t *rtc);

#endif /* CONFIG_CSI_DRV_RTC_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_RTC_H__ */
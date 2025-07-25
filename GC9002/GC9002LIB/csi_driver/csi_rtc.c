#include "csi_rtc.h"
#ifdef CONFIG_CSI_DRV_RTC_ENABLED


static RTC_HandleTypeDef handle_rtc = {.Instance = RTC};

static void CSI_HAL_RTC_AlarmEventCallback(RTC_HandleTypeDef *hrtc)
{
    RTC_TimeTypeDef time;
    if(hrtc != NULL)
    {
        HAL_RTC_GetTime(hrtc, &time, RTC_FORMAT_BIN);
        if(hrtc->priv_obj != NULL)
        {
            if(time.Minute == ((csi_rtc_t *)hrtc->priv_obj)->s_alarm.time.Minute && 
            time.Hour == ((csi_rtc_t *)hrtc->priv_obj)->s_alarm.time.Hour)
            {
                if(((csi_rtc_t *)hrtc->priv_obj)->callback != NULL)
                {
                    ((csi_rtc_t *)hrtc->priv_obj)->callback((csi_rtc_t *)hrtc->priv_obj, ((csi_rtc_t *)hrtc->priv_obj)->arg);
                }
            }
        }
    }
}

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_RTC_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_RTC_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

	HAL_RTC_AlarmIRQHandler(&handle_rtc);
    
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

csi_error_t csi_rtc_init(csi_rtc_t *rtc, uint32_t idx)
{
    if(rtc == NULL || idx >= CSI_RTC_CNT)
    {
        return CSI_ERROR;
    }
    if(idx == 0)
    {
        HANDLE_REG_BASE(rtc) = RTC_BASE;
        HANDLE_IRQ_NUM(rtc) = RTC_IRQn;
        HANDLE_DEV_IDX(rtc) = 0;
        HANDLE_IRQ_HANDLER(rtc) = (void *)CSI_RTC_IRQHandler;
    }    
    handle_rtc.priv_obj = rtc;
    rtc->priv = (void *)&handle_rtc;
    HAL_RTC_Struct_Init((RTC_HandleTypeDef *)(rtc->priv));
    HAL_RTC_Init((RTC_HandleTypeDef *)(rtc->priv));
    HAL_RTC_RegisterCallback((RTC_HandleTypeDef *)(rtc->priv), HAL_RTC_ALARM_EVENT_CB_ID, CSI_HAL_RTC_AlarmEventCallback);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(HANDLE_IRQ_NUM(rtc), CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, HANDLE_IRQ_HANDLER(rtc));
#else
    csi_clic_register_irq(HANDLE_IRQ_NUM(rtc), CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, HANDLE_IRQ_HANDLER(rtc));
#endif
    return CSI_OK;
}
void csi_rtc_uninit(csi_rtc_t *rtc)
{
    if(rtc == NULL)
    {
        return;
    }
    HANDLE_REG_BASE(rtc) = 0;
    HANDLE_IRQ_NUM(rtc) = 0;
    HANDLE_DEV_IDX(rtc) = 0;
    HANDLE_IRQ_HANDLER(rtc) = NULL;
    rtc->priv = NULL;
}
csi_error_t csi_rtc_set_time(csi_rtc_t *rtc, const csi_rtc_time_t *rtctime)
{
    if(rtc == NULL || rtctime == NULL)
    {
        return CSI_ERROR;
    }
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
    time.Second = rtctime->tm_sec;
    time.Minute = rtctime->tm_min;
    time.Hour = rtctime->tm_hour;
    
    date.Day = rtctime->tm_mday;
    date.Month = rtctime->tm_mon + CSI_MONTH_BASE;
    date.Year = rtctime->tm_year + CSI_YEAR_BASE;
    
    HAL_RTC_SetTime((RTC_HandleTypeDef *)(rtc->priv), &time, RTC_FORMAT_BIN);
    HAL_RTC_SetDate((RTC_HandleTypeDef *)(rtc->priv), &date, RTC_FORMAT_BIN);
    return CSI_OK;
}
csi_error_t csi_rtc_set_time_no_wait(csi_rtc_t *rtc, const csi_rtc_time_t *rtctime)
{
    return csi_rtc_set_time(rtc, rtctime);
}
csi_error_t csi_rtc_get_time(csi_rtc_t *rtc, csi_rtc_time_t *rtctime)
{
    if(rtc == NULL || rtctime == NULL)
    {
        return CSI_ERROR;
    }
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
    HAL_RTC_GetTime((RTC_HandleTypeDef *)(rtc->priv), &time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate((RTC_HandleTypeDef *)(rtc->priv), &date, RTC_FORMAT_BIN);
    rtctime->tm_sec = time.Second;
    rtctime->tm_min = time.Minute;
    rtctime->tm_hour = time.Hour;
    
    rtctime->tm_mday = date.Day;
    rtctime->tm_mon = date.Month - CSI_MONTH_BASE;
    rtctime->tm_year = date.Year - CSI_YEAR_BASE;
    return CSI_OK;
}
uint32_t csi_rtc_get_alarm_remaining_time(csi_rtc_t *rtc)
{
    return 0;
}
csi_error_t csi_rtc_set_alarm(csi_rtc_t *rtc, const csi_rtc_time_t *rtctime, void(*callback)(struct _csi_rtc *rtc, void *arg), void *arg)
{
    if(rtc == NULL || rtctime == NULL || callback == NULL)
    {
        return CSI_ERROR;
    }
    rtc->s_alarm.time.Second = rtctime->tm_sec;
    rtc->s_alarm.time.Minute = rtctime->tm_min;
    rtc->s_alarm.time.Hour = rtctime->tm_hour;
    rtc->s_alarm.AlarmMask = RTC_ALARMMASK_HOURS | RTC_ALARMMASK_MINUTES | RTC_ALARMMASK_SECONDS;
    
    HAL_RTC_SetAlarm((RTC_HandleTypeDef *)(rtc->priv), &rtc->s_alarm, RTC_FORMAT_BIN);
    rtc->callback = callback;
    rtc->arg = arg;
    return CSI_OK;
}
csi_error_t csi_rtc_cancel_alarm(csi_rtc_t *rtc)
{
    if(rtc == NULL)
    {
        return CSI_ERROR;
    }
    HAL_RTC_DeactivateAlarm((RTC_HandleTypeDef *)(rtc->priv));
    return CSI_OK;
}
bool csi_rtc_is_running(csi_rtc_t *rtc)
{
    (void)rtc;
    return true;
}

#endif /* CONFIG_CSI_DRV_RTC_ENABLED */
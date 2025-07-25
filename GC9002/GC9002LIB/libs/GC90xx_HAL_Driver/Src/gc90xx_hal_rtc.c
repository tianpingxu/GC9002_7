/*
 * Copyright (c) 2022-2050, GraphichinaSX Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-02-09     mazy         V1.0.1
 */
#include <gc90xx_hal.h>

#ifdef HAL_RTC_MODULE_ENABLED

static uint8_t RTC_ByteToBcd2(uint8_t Value);
static uint8_t RTC_Bcd2ToByte(uint8_t Value);
static unsigned int decimal_to_bcd_code(unsigned int decimal);
static unsigned int bcd_to_decimal_code(unsigned int bcd);



void HAL_RTC_Struct_Init(RTC_HandleTypeDef *hrtc)
{
    if(hrtc == NULL)
    {
       return;
    }
    hrtc->Init.date.value = 0x20001230;
    hrtc->Init.time.value = 0x00125958;
    hrtc->Init.format = RTC_FORMAT_BCD;
}

/**
  * @brief  Initializes the RTC peripheral
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_Init(RTC_HandleTypeDef *hrtc)
{
    /* Check the RTC peripheral state */
    if(hrtc == NULL)
    {
       return HAL_ERROR;
    }

#if(defined USE_PERIPHERAL_LOCK && USE_PERIPHERAL_LOCK == 1U)
    __HAL_CHECK_PERI_LOCK((uint32_t)(hrtc->Instance));
#endif

#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)
    if(hrtc->State == HAL_RTC_STATE_RESET)
    {
        __HAL_LOCK(hrtc);
        hrtc->AlarmEventCallback           =  HAL_RTC_AlarmEventCallback;        /* Legacy weak Alarm EventCallback      */
        hrtc->WakeUpTimerEventCallback     =  HAL_RTCEx_WakeUpTimerEventCallback; /* Legacy weak WakeUpTimerEventCallback */
        if(hrtc->MspInitCallback == NULL)
        {
            hrtc->MspInitCallback = HAL_RTC_MspInit;
        }
        /* Init the low level hardware */
        hrtc->MspInitCallback(hrtc);
        if(hrtc->MspDeInitCallback == NULL)
        {
            hrtc->MspDeInitCallback = HAL_RTC_MspDeInit;
        }
        __HAL_UNLOCK(hrtc);
    }
#else
    if(hrtc->State == HAL_RTC_STATE_RESET)
    {
        __HAL_LOCK(hrtc);
        /* Initialize RTC MSP */
        HAL_RTC_MspInit(hrtc);
        __HAL_UNLOCK(hrtc);
    }
#endif
    HAL_RTC_SetDate(hrtc, &hrtc->Init.date, hrtc->Init.format);
    HAL_RTC_SetTime(hrtc, &hrtc->Init.time, hrtc->Init.format);
    __HAL_RTC_DISABLE_IT(hrtc, RTC_IT_ALL);
    __HAL_RTC_CLEAR_FLAG(hrtc, RTC_IT_ALL);
    __HAL_RTC_ENABLE(hrtc);
    /* Set RTC state */
    hrtc->State = HAL_RTC_STATE_READY;
    __HAL_UNLOCK(hrtc);

#if(defined USE_PERIPHERAL_LOCK && USE_PERIPHERAL_LOCK == 1U)
    __HAL_SET_PERI_LOCK((uint32_t)(hrtc->Instance), PERIPHERAL_INITED);
#endif
    return HAL_OK;
}

/**
  * @brief  DeInitializes the RTC peripheral
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @note   This function doesn't reset the RTC Backup Data registers.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_DeInit(RTC_HandleTypeDef *hrtc)
{
    (void)hrtc;
    return HAL_OK;
}

/**
  * @brief  Initializes the RTC MSP.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
__WEAK void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
    (void)hrtc;
}
/**
  * @brief  DeInitializes the RTC MSP.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
__WEAK void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
    (void)hrtc;
}

/**
  * @brief  Sets RTC current time.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sTime Pointer to Time structure
  * @param  Format Specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_FORMAT_BIN: Binary data format
  *            @arg RTC_FORMAT_BCD: BCD data format
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format)
{
    RTC_TimeTypeDef time;
    /* Check the RTC peripheral state */
    if(hrtc == NULL)
    {
       return HAL_ERROR;
    }
    /* Process Locked */
    __HAL_LOCK(hrtc);
    /* Change RTC state */
    hrtc->State = HAL_RTC_STATE_BUSY;
    time.value = sTime->value;
    if(Format == RTC_FORMAT_BIN)
    {
        time.Second = decimal_to_bcd_code(time.Second);
        time.Minute = decimal_to_bcd_code(time.Minute);
        time.Hour = decimal_to_bcd_code(time.Hour);
    }
    __HAL_RTC_DISABLE(hrtc);
    hrtc->Instance->TIME = time.value;
    __HAL_RTC_ENABLE(hrtc);
    /* Change RTC state */
    hrtc->State = HAL_RTC_STATE_READY;
    /* Process Unlocked */
    __HAL_UNLOCK(hrtc);
    return HAL_OK;
}

/**
  * @brief  Gets RTC current time.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sTime Pointer to Time structure
  * @param  Format Specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_FORMAT_BIN: Binary data format
  *            @arg RTC_FORMAT_BCD: BCD data format
  * @note  You can use SubSeconds and SecondFraction (sTime structure fields returned) to convert SubSeconds
  *        value in second fraction ratio with time unit following generic formula:
  *        Second fraction ratio * time_unit= [(SecondFraction-SubSeconds)/(SecondFraction+1)] * time_unit
  *        This conversion can be performed only if no shift operation is pending (ie. SHFP=0) when PREDIV_S >= SS
  * @note You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the values
  *        in the higher-order calendar shadow registers to ensure consistency between the time and date values.
  *        Reading RTC current time locks the values in calendar shadow registers until current date is read.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_GetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format)
{
    RTC_TimeTypeDef time;
    /* Check the RTC peripheral state */
    if(hrtc == NULL)
    {
       return HAL_ERROR;
    }
    time.value = hrtc->Instance->TIME;
    if(Format == RTC_FORMAT_BIN)
    {
        time.Second = bcd_to_decimal_code(time.Second);
        time.Minute = bcd_to_decimal_code(time.Minute);
        time.Hour = bcd_to_decimal_code(time.Hour);
    }
    sTime->value = time.value;
    return HAL_OK;
}

/**
  * @brief  Sets RTC current date.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sDate Pointer to date structure
  * @param  Format specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_FORMAT_BIN: Binary data format
  *            @arg RTC_FORMAT_BCD: BCD data format
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format)
{
    RTC_DateTypeDef date;
    /* Check the RTC peripheral state */
    if(hrtc == NULL)
    {
       return HAL_ERROR;
    }
    /* Process Locked */
    __HAL_LOCK(hrtc);
    /* Change RTC state */
    hrtc->State = HAL_RTC_STATE_BUSY;
    date.value = sDate->value;
    if(Format == RTC_FORMAT_BIN)
    {
        date.Day = decimal_to_bcd_code(date.Day);
        date.Month = decimal_to_bcd_code(date.Month);
        date.Year = decimal_to_bcd_code(date.Year);
    }
    __HAL_RTC_DISABLE(hrtc);
    hrtc->Instance->DATE = date.value;
    __HAL_RTC_ENABLE(hrtc);
    /* Change RTC state */
    hrtc->State = HAL_RTC_STATE_READY;
    /* Process Unlocked */
    __HAL_UNLOCK(hrtc);
    return HAL_OK;
}

/**
  * @brief  Gets RTC current date.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sDate Pointer to Date structure
  * @param  Format Specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_FORMAT_BIN:  Binary data format
  *            @arg RTC_FORMAT_BCD:  BCD data format
  * @note You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the values
  * in the higher-order calendar shadow registers to ensure consistency between the time and date values.
  * Reading RTC current time locks the values in calendar shadow registers until Current date is read.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_GetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format)
{
    RTC_DateTypeDef date;
    /* Check the RTC peripheral state */
    if(hrtc == NULL)
    {
       return HAL_ERROR;
    }
    date.value = hrtc->Instance->DATE;
    if(Format == RTC_FORMAT_BIN)
    {
        date.Day = bcd_to_decimal_code(date.Day);
        date.Month = bcd_to_decimal_code(date.Month);
        date.Year = bcd_to_decimal_code(date.Year);
    }
    sDate->value = date.value;
    return HAL_OK;
}

/**
  * @brief  Sets the specified RTC Alarm.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sAlarm Pointer to Alarm structure
  * @param  Format Specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *             @arg RTC_FORMAT_BIN: Binary data format
  *             @arg RTC_FORMAT_BCD: BCD data format
  * @note   if set 8:20:00 RTC_ALARMMASK_SECONDS(second interrupt)
  *         trigger interrupt when second carry the same number, 8:21:00,8:22:00...
  *         if set 8:20:00 RTC_ALARMMASK_MINUTES(minute interrupt)
  *         trigger interrupt when minuter carry the same number, 9:20:00,10:20:00...
  *         if set 8:20:00 RTC_ALARMMASK_HOURS(hour interrupt)
  *         trigger interrupt when hour carry the same number, 8:00:00
  *         if set 8:20:00 RTC_ALARMMASK_SECONDS_EVERY(per second interrupt)
  *         trigger interrupt per second, 8:20:00,8:20:01,8:20:02...
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_SetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Format)
{
    RTC_TimeTypeDef time;
    /* Check the RTC peripheral state */
    if(hrtc == NULL)
    {
       return HAL_ERROR;
    }
    assert_param(IS_RTC_ALARMMASK(sAlarm->AlarmMask));
    /* Process Locked */
    __HAL_LOCK(hrtc);
    /* Change RTC state */
    hrtc->State = HAL_RTC_STATE_BUSY;
    time.value = sAlarm->time.value;
    if(Format == RTC_FORMAT_BIN)
    {
        time.Second = decimal_to_bcd_code(time.Second);
        time.Minute = decimal_to_bcd_code(time.Minute);
        time.Hour = decimal_to_bcd_code(time.Hour);
    }
    hrtc->Instance->INTR_TIME = time.value;
    __HAL_RTC_SET_IT(hrtc, sAlarm->AlarmMask);
    /* Change RTC state */
    hrtc->State = HAL_RTC_STATE_READY;
    /* Process Unlocked */
    __HAL_UNLOCK(hrtc);
    return HAL_OK;
}

/**
  * @brief  Deactivate the specified RTC Alarm
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_DeactivateAlarm(RTC_HandleTypeDef *hrtc)
{
    /* Check the RTC peripheral state */
    if(hrtc == NULL)
    {
       return HAL_ERROR;
    }
    __HAL_RTC_DISABLE_IT(hrtc, RTC_IT_ALL);
    return HAL_OK;
}

/**
  * @brief  Gets the RTC Alarm value and masks.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sAlarm Pointer to Date structure
  * @param  Alarm Specifies the Alarm.
  *          This parameter can be one of the following values:
  *             @arg RTC_ALARM_A: AlarmA
  *             @arg RTC_ALARM_B: AlarmB
  * @param  Format Specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *             @arg RTC_FORMAT_BIN: Binary data format
  *             @arg RTC_FORMAT_BCD: BCD data format
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_GetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Format)
{
    /* Check the RTC peripheral state */
    if(hrtc == NULL)
    {
       return HAL_ERROR;
    }
    sAlarm->time.value = hrtc->Instance->INTR_TIME;
    if(Format == RTC_FORMAT_BIN)
    {
        sAlarm->time.Second = bcd_to_decimal_code(sAlarm->time.Second);
        sAlarm->time.Minute = bcd_to_decimal_code(sAlarm->time.Minute);
        sAlarm->time.Hour = bcd_to_decimal_code(sAlarm->time.Hour);
    }
    return HAL_OK;
}

/**
  * @brief  This function handles Alarm interrupt request.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
void HAL_RTC_AlarmIRQHandler(RTC_HandleTypeDef *hrtc)
{
    if(__HAL_RTC_GET_FLAG(hrtc, RTC_IT_ALL))
    {
        __HAL_RTC_CLEAR_FLAG(hrtc, RTC_IT_ALL);
#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)
        if(hrtc->AlarmEventCallback)
        {
            hrtc->AlarmEventCallback(hrtc);
        }
#else
        HAL_RTC_AlarmEventCallback(hrtc);
#endif
    }
}

/**
  * @brief  Alarm callback.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
__WEAK void HAL_RTC_AlarmEventCallback(RTC_HandleTypeDef *hrtc)
{
    (void)hrtc;
}

/**
  * @brief  Weak up callback.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval None
  */
__WEAK void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    (void)hrtc;
}

/**
  * @brief  Returns the RTC state.
  * @param  hrtc pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @retval HAL state
  */
HAL_RTCStateTypeDef HAL_RTC_GetState(RTC_HandleTypeDef *hrtc)
{
    /* Check the RTC peripheral state */
    if(hrtc == NULL)
    {
       return HAL_ERROR;
    }
    return hrtc->State;
}

/**
  * @brief  Converts a 2 digit decimal to BCD format.
  * @param  Value Byte to be converted <0~99>
  * @retval Converted byte
  */
static uint8_t RTC_ByteToBcd2(uint8_t Value)
{
    uint32_t bcdhigh = 0U;

    while(Value >= 10U)
    {
        bcdhigh++;
        Value -= 10U;
    }
    return ((uint8_t)(bcdhigh << 4U) | Value);
}

/**
  * @brief  Converts from 2 digit BCD to Binary.
  * @param  Value BCD value to be converted <0x00~0x99>
  * @retval Converted word
  */
static uint8_t RTC_Bcd2ToByte(uint8_t Value)
{
    uint32_t tmp = 0U;
    tmp = ((uint8_t)(Value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
    return (tmp + (Value & (uint8_t)0x0F));
}

static unsigned int decimal_to_bcd_code(unsigned int decimal)
{
    unsigned int bcd = 0;  //sum return BCD code
 
    for (int i = 0; decimal > 0; i++)
    {
        bcd |= ((decimal % 10 ) << ( 4 * i));
        decimal /= 10;
    }
    return bcd;
}

static unsigned int bcd_to_decimal_code(unsigned int bcd)
{
    unsigned int decimal = 0, c = 1;  // sum return decimal
 
    for(int i = 1; bcd > 0; i++)
    {
        if( i >= 2)
        {
            c*=10;
        }
        decimal += (bcd%16) * c;
        bcd /= 16;
    }
    return decimal;
}

#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register a User RTC Callback
  *         To be used instead of the weak predefined callback
  * @param  hrtc RTC handle
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_RTC_ALARM_EVENT_CB_ID            Alarm Event Callback ID
  *          @arg @ref HAL_RTC_WAKEUPTIMER_EVENT_CB_ID      Wake-Up Timer Event Callback ID
  *          @arg @ref HAL_RTC_MSPINIT_CB_ID                Msp Init callback ID
  *          @arg @ref HAL_RTC_MSPDEINIT_CB_ID              Msp DeInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_RegisterCallback(RTC_HandleTypeDef *hrtc, HAL_RTC_CallbackIDTypeDef CallbackID, pRTC_CallbackTypeDef pCallback)
{
    HAL_StatusTypeDef status = HAL_OK;
    if(pCallback == NULL)
    {
        return HAL_ERROR;
    }
    /* Process locked */
    __HAL_LOCK(hrtc);
    if(HAL_RTC_STATE_READY == hrtc->State || HAL_RTC_STATE_RESET == hrtc->State)
    {
        switch (CallbackID)
        {
            case HAL_RTC_ALARM_EVENT_CB_ID :
                hrtc->AlarmEventCallback = pCallback;
            break;
            case HAL_RTC_WAKEUPTIMER_EVENT_CB_ID :
                hrtc->WakeUpTimerEventCallback = pCallback;
            break;
            case HAL_RTC_MSPINIT_CB_ID :
                hrtc->MspInitCallback = pCallback;
            break;

            case HAL_RTC_MSPDEINIT_CB_ID :
                hrtc->MspDeInitCallback = pCallback;
            break;
            default :
                /* Return error status */
                status =  HAL_ERROR;
            break;
        }
    }
    else
    {
        /* Return error status */
        status =  HAL_ERROR;
    }
    /* Release Lock */
    __HAL_UNLOCK(hrtc);
    return status;
}

/**
  * @brief  Unregister an RTC Callback
  *         RTC callabck is redirected to the weak predefined callback
  * @param  hrtc RTC handle
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_RTC_ALARM_EVENT_CB_ID            Alarm Event Callback ID
  *          @arg @ref HAL_RTC_WAKEUPTIMER_EVENT_CB_ID      Wake-Up Timer Event Callback ID
  *          @arg @ref HAL_RTC_MSPINIT_CB_ID                Msp Init callback ID
  *          @arg @ref HAL_RTC_MSPDEINIT_CB_ID              Msp DeInit callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_UnRegisterCallback(RTC_HandleTypeDef *hrtc, HAL_RTC_CallbackIDTypeDef CallbackID)
{
    HAL_StatusTypeDef status = HAL_OK;
    /* Process locked */
    __HAL_LOCK(hrtc);
    if(HAL_RTC_STATE_READY == hrtc->State || HAL_RTC_STATE_RESET == hrtc->State)
    {
        switch (CallbackID)
        {
            case HAL_RTC_ALARM_EVENT_CB_ID :
                hrtc->AlarmEventCallback = HAL_RTC_AlarmEventCallback;         /* Legacy weak AlarmAEventCallback    */
            break;
            case HAL_RTC_WAKEUPTIMER_EVENT_CB_ID :
                hrtc->WakeUpTimerEventCallback = HAL_RTCEx_WakeUpTimerEventCallback; /* Legacy weak WakeUpTimerEventCallback */
            break;
            case HAL_RTC_MSPINIT_CB_ID :
                hrtc->MspInitCallback = HAL_RTC_MspInit;
            break;
            case HAL_RTC_MSPDEINIT_CB_ID :
                hrtc->MspDeInitCallback = HAL_RTC_MspDeInit;
            break;
            default :
            /* Return error status */
            status =  HAL_ERROR;
            break;
        }
    }
    else
    {
        /* Return error status */
        status =  HAL_ERROR;
    }
    /* Release Lock */
    __HAL_UNLOCK(hrtc);
    return status;
}
#endif /* USE_HAL_RTC_REGISTER_CALLBACKS */
#endif /* HAL_RTC_MODULE_ENABLED */

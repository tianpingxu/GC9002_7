/**
  ******************************************************************************
  * @file    gc90xx_hal_rtc.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2022-10-18
  * @brief   This file contains all the functions prototypes for the RTC
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC90XX_HAL_RTC_H__
#define __GC90XX_HAL_RTC_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <gc90xx_hal_def.h>

#ifdef HAL_RTC_MODULE_ENABLED

 /**
   * @brief  RTC Date structure definition
   *         0x20221113<2022/11/13>
   */
typedef union
{
    uint32_t value;
    struct
    {
        uint8_t  Day;           /*!< Specifies the RTC Date.
                                 This parameter must be a number between Min_Data = 1 and Max_Data = 31 */
        uint8_t  Month;         /*!< Specifies the RTC Date Month (in BCD format).
                                 This parameter can be a value of @ref RTC_Month_Date_Definitions */
        uint16_t Year;          /*!< Specifies the RTC Date Year.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 9999 */
    };
}RTC_DateTypeDef;
/**
  * @brief  RTC Time structure definition
  *         0x00133030<13:30:30>
  */
typedef union
{
    uint32_t value;
    struct
    {
        uint8_t Second;         /*!< Specifies the RTC Time Seconds.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 59 */
        uint8_t Minute;         /*!< Specifies the RTC Time Minutes.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 59 */
        uint8_t Hour;           /*!< Specifies the RTC Time Hour.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 12 if the RTC_HourFormat_12 is selected.
                                 This parameter must be a number between Min_Data = 0 and Max_Data = 23 if the RTC_HourFormat_24 is selected  */
        uint8_t Reserved;
    };
}RTC_TimeTypeDef;          

/**
  * @brief  HAL State structures definition
  */
typedef enum
{
    HAL_RTC_STATE_RESET             = 0x00U,  /*!< RTC not yet initialized or disabled */
    HAL_RTC_STATE_READY             = 0x01U,  /*!< RTC initialized and ready for use   */
    HAL_RTC_STATE_BUSY              = 0x02U,  /*!< RTC process is ongoing              */
    HAL_RTC_STATE_TIMEOUT           = 0x03U,  /*!< RTC timeout state                   */
    HAL_RTC_STATE_ERROR             = 0x04U   /*!< RTC error state                     */
}HAL_RTCStateTypeDef;

typedef enum
{
    RTC_FORMAT_BCD = 0,
    RTC_FORMAT_BIN
}RTC_FormatTypeDef;
/**
  * @brief  RTC Configuration Structure definition
  */
typedef struct
{
    RTC_DateTypeDef date;
    RTC_TimeTypeDef time;
    RTC_FormatTypeDef format;
}RTC_InitTypeDef;

/**
  * @brief  RTC Alarm structure definition
  */
typedef struct
{
    RTC_TimeTypeDef time;           /*!< Specifies the RTC Alarm Time members */
    uint32_t AlarmMask;             /*!< Specifies the RTC Alarm Masks.
                                      This parameter can be a value of @ref RTC_AlarmMask_Definitions */
}RTC_AlarmTypeDef;
/**
  * @brief  RTC Handle Structure definition
  */
#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)
typedef struct __RTC_HandleTypeDef
#else
typedef struct
#endif /* (USE_HAL_RTC_REGISTER_CALLBACKS) */
{
    RTC_TypeDef                 *Instance;  /*!< Register base address    */
    RTC_InitTypeDef             Init;       /*!< RTC required parameters  */
    HAL_LockTypeDef             Lock;       /*!< RTC locking object       */
    __IO HAL_RTCStateTypeDef    State;      /*!< Time communication state */
    void                        *priv_obj;

#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)
    void  (* AlarmEventCallback)       ( struct __RTC_HandleTypeDef * hrtc);  /*!< RTC Alarm Event callback           */
    void  (* WakeUpTimerEventCallback) ( struct __RTC_HandleTypeDef * hrtc);  /*!< RTC WakeUpTimer Event callback     */
    void  (* MspInitCallback)          ( struct __RTC_HandleTypeDef * hrtc);  /*!< RTC Msp Init callback              */
    void  (* MspDeInitCallback)        ( struct __RTC_HandleTypeDef * hrtc);  /*!< RTC Msp DeInit callback            */
#endif /* (USE_HAL_RTC_REGISTER_CALLBACKS) */

}RTC_HandleTypeDef;

#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)
/**
  * @brief  HAL RTC Callback ID enumeration definition
  */
typedef enum
{
  HAL_RTC_ALARM_EVENT_CB_ID             = 0x00u,    /*!< RTC Alarm Event Callback ID         */
  HAL_RTC_WAKEUPTIMER_EVENT_CB_ID       = 0x03u,    /*!< RTC Wake-Up Timer Event Callback ID */
  HAL_RTC_MSPINIT_CB_ID                 = 0x0Eu,    /*!< RTC Msp Init callback ID            */
  HAL_RTC_MSPDEINIT_CB_ID               = 0x0Fu     /*!< RTC Msp DeInit callback ID          */
}HAL_RTC_CallbackIDTypeDef;

/**
  * @brief  HAL RTC Callback pointer definition
  */
typedef  void (*pRTC_CallbackTypeDef)(RTC_HandleTypeDef * hrtc); /*!< pointer to an RTC callback function */
#endif /* USE_HAL_RTC_REGISTER_CALLBACKS */

#define RTC_TIMEOUT_VALUE       1000

#define RTC_CTRL_SETTIME        (0x01UL)

#define RTC_IT_ALL              (0xF)
#define RTC_IT_HOUR             (0x1 << 3)
#define RTC_IT_MINUTE           (0x1 << 2)
#define RTC_IT_SECOND           (0x1 << 1)
#define RTC_IT_SECOND_EVERY     (0x1)

/** @defgroup RTC_AlarmMask_Definitions RTC Alarm Mask Definitions
  * @{
  */
#define RTC_ALARMMASK_NONE                  0x00000000U
#define RTC_ALARMMASK_HOURS                 RTC_IT_HOUR
#define RTC_ALARMMASK_MINUTES               RTC_IT_MINUTE
#define RTC_ALARMMASK_SECONDS               RTC_IT_SECOND
#define RTC_ALARMMASK_SECONDS_EVERY         RTC_IT_SECOND_EVERY
#define RTC_ALARMMASK_ALL                   RTC_IT_ALL
#define IS_RTC_ALARMMASK(MASK)              (((MASK) & (~(RTC_ALARMMASK_ALL))) == 0)
/** @defgroup RTC_Month_Date_Definitions RTC Month Date Definitions
  * @{
  */
/* Coded in BCD format */
#define RTC_MONTH_JANUARY              ((uint8_t)0x01)
#define RTC_MONTH_FEBRUARY             ((uint8_t)0x02)
#define RTC_MONTH_MARCH                ((uint8_t)0x03)
#define RTC_MONTH_APRIL                ((uint8_t)0x04)
#define RTC_MONTH_MAY                  ((uint8_t)0x05)
#define RTC_MONTH_JUNE                 ((uint8_t)0x06)
#define RTC_MONTH_JULY                 ((uint8_t)0x07)
#define RTC_MONTH_AUGUST               ((uint8_t)0x08)
#define RTC_MONTH_SEPTEMBER            ((uint8_t)0x09)
#define RTC_MONTH_OCTOBER              ((uint8_t)0x10)
#define RTC_MONTH_NOVEMBER             ((uint8_t)0x11)
#define RTC_MONTH_DECEMBER             ((uint8_t)0x12)

#define __HAL_RTC_ENABLE(__HANDLE__)        ((__HANDLE__)->Instance->CR = RTC_CTRL_SETTIME)
#define __HAL_RTC_DISABLE(__HANDLE__)       ((__HANDLE__)->Instance->CR = 0)
/** @brief  Enable or disable the specified RTC interrupts.
   * @param  __HANDLE__ specifies the I2C Handle.
   * @param  __INTERRUPT__ specifies the interrupt source to enable or disable.
   *         This parameter can be one of the following values:
   *            @arg RTC_IT_HOUR            Interrupt for hour.
   *            @arg RTC_IT_MINUTE          Interrupt for minute.
   *            @arg RTC_IT_SECOND          Interrupt for second.
   *            @arg RTC_IT_SECOND_EVERY    Interrupt for every second.
   * @retval None
   */
#define __HAL_RTC_ENABLE_IT(__HANDLE__, __INTERRUPT__)      SET_BIT((__HANDLE__)->Instance->IMR, (__INTERRUPT__))
#define __HAL_RTC_DISABLE_IT(__HANDLE__, __INTERRUPT__)     CLEAR_BIT((__HANDLE__)->Instance->IMR, (__INTERRUPT__))
#define __HAL_RTC_SET_IT(__HANDLE__, __INTERRUPT__)         MODIFY_REG((__HANDLE__)->Instance->IMR, RTC_IT_ALL, (__INTERRUPT__))
/** @brief  Get Flag of the specified I2C interrupts.
  * @param  __HANDLE__ specifies the I2C Handle.
 * @param  __FLAG__    specifies the interrupt source.
  *         This parameter can be one of the following values:
   *            @arg RTC_IT_HOUR            Interrupt for hour.
   *            @arg RTC_IT_MINUTE          Interrupt for minute.
   *            @arg RTC_IT_SECOND          Interrupt for second.
   *            @arg RTC_IT_SECOND_EVERY    Interrupt for every second.
  * @retval None
  */
#define __HAL_RTC_GET_FLAG(__HANDLE__, __FLAG__)            (((__HANDLE__)->Instance->ISR & (__FLAG__)) != RESET)
/** @brief  Get Flag of the specified I2C interrupts.
  * @param  __HANDLE__ specifies the I2C Handle.
 * @param  __FLAG__    specifies the interrupt source.
  *         This parameter can be one of the following values:
   *            @arg RTC_IT_HOUR            Interrupt for hour.
   *            @arg RTC_IT_MINUTE          Interrupt for minute.
   *            @arg RTC_IT_SECOND          Interrupt for second.
   *            @arg RTC_IT_SECOND_EVERY    Interrupt for every second.
  * @retval None
  */
#define __HAL_RTC_CLEAR_FLAG(__HANDLE__, __FLAG__)            ((__HANDLE__)->Instance->ISR &= ~(__FLAG__))
/** @addtogroup RTC_Exported_Functions_Group1
  * @{
  */
/* Initialization and de-initialization functions  ****************************/
void HAL_RTC_Struct_Init(RTC_HandleTypeDef *hrtc);
HAL_StatusTypeDef HAL_RTC_Init(RTC_HandleTypeDef *hrtc);
HAL_StatusTypeDef HAL_RTC_DeInit(RTC_HandleTypeDef *hrtc);
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc);
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc);

/* Callbacks Register/UnRegister functions  ***********************************/
#if (USE_HAL_RTC_REGISTER_CALLBACKS == 1)
HAL_StatusTypeDef HAL_RTC_RegisterCallback(RTC_HandleTypeDef *hrtc, HAL_RTC_CallbackIDTypeDef CallbackID, pRTC_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_RTC_UnRegisterCallback(RTC_HandleTypeDef *hrtc, HAL_RTC_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_RTC_REGISTER_CALLBACKS */

/** @addtogroup RTC_Exported_Functions_Group2
  * @{
  */
/* RTC Time and Date functions ************************************************/
HAL_StatusTypeDef HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format);
HAL_StatusTypeDef HAL_RTC_GetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format);
HAL_StatusTypeDef HAL_RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format);
HAL_StatusTypeDef HAL_RTC_GetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format);

/** @addtogroup RTC_Exported_Functions_Group3
  * @{
  */
/* RTC Alarm functions ********************************************************/
HAL_StatusTypeDef HAL_RTC_SetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Format);
HAL_StatusTypeDef HAL_RTC_DeactivateAlarm(RTC_HandleTypeDef *hrtc);
HAL_StatusTypeDef HAL_RTC_GetAlarm(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Format);
void              HAL_RTC_AlarmIRQHandler(RTC_HandleTypeDef *hrtc);
void              HAL_RTC_AlarmEventCallback(RTC_HandleTypeDef *hrtc);
void              HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc);
/** @addtogroup RTC_Exported_Functions_Group4
  * @{
  */
/* Peripheral State functions *************************************************/
HAL_RTCStateTypeDef HAL_RTC_GetState(RTC_HandleTypeDef *hrtc);

/* Private functions ---------------------------------------------------------*/
/** @defgroup RTC_Private_Functions RTC Private Functions
  * @{
  */


#endif/* HAL_RTC_MODULE_ENABLED */
#ifdef __cplusplus
 }
#endif
#endif /* __GC90XX_HAL_RTC_H__ */

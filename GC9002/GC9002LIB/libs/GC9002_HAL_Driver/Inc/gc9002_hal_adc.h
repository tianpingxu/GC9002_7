/**
  ******************************************************************************
  * @file    gc9002_hal_adc.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2023-1-6
  * @brief   This file contains all the functions prototypes for the ADC
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC9002_HAL_ADC_H__
#define __GC9002_HAL_ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal_def.h"

#ifdef HAL_ADC_MODULE_ENABLED

#define __HAL_ADC_DRIVER_ATTRIBUTE              __attribute__((optimize(0)))
/** @addtogroup GC9002_HAL_Driver
  * @{
  */

/** @addtogroup ADC
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup ADC_Exported_Types ADC Exported Types
  * @{
  */

/**
  * @brief  ADC Configuration Structure definition
  */
typedef struct
{
    uint32_t ClockDiv   : 4;          /*!< Specifies the ADC clock divide value.
                                       This parameter can be a value of @ref 2~0xF */
    uint32_t Mode       : 1;          /*!< Specifies the ADC convert mode, single or continus. 
                                       This parameter can be a value of @ref  ADC_Mode */
    uint32_t Channel    : 5;          /*!< Specifies the ADC convert channel.
                                       This parameter can be a value of @ref ADC_channels  */
    uint32_t FullSize   : 4;          /*!< Specifies the ADC full size.
                                       This parameter can be a value of @ref 0~ADC_FIFO_DEPTH-1 */
    uint32_t SampleTime : 4;          /*!< ADC sample time selection control signal.@ref 0~0xF */
    uint32_t ALG_MEAN   : 2;          /*!< Temperature sampling points selection for mean algorithm. @ref 0~0x3 */
    uint32_t TTRIM      : 4;          /*!< Built-In reference voltage temperature ratio control.@ref 0~0xF */
    uint32_t ADJ_TD_OS  : 4;          /*!< Temperature sensor offset adjustment signal. @ref 0~0xF */
    uint32_t ADJ_TD_GA  : 4;          /*!< Temperature sensor gain adjustment signal. @ref 0~0xF */
} ADC_InitTypeDef;

/** 
  * @brief  HAL ADC state machine: ADC states definition (bitfields)
  */ 
/* States of ADC global scope */
enum HAL_ADC_STATUS
{
    HAL_ADC_STATE_RESET,  /*!< ADC not yet initialized or disabled */
    HAL_ADC_STATE_READY,  /*!< ADC peripheral ready for use */

    HAL_ADC_STATE_BUSY_INTERNAL,  /*!< ADC is busy to internal process (initialization, calibration) */
    HAL_ADC_STATE_TIMEOUT,  /*!< TimeOut occurrence */

    /* States of ADC errors */
    HAL_ADC_STATE_ERROR_INTERNAL,  /*!< Internal error occurrence */
    HAL_ADC_STATE_ERROR_CONFIG,  /*!< Configuration error occurrence */
    HAL_ADC_STATE_ERROR_DMA,  /*!< DMA error occurrence */

    /* States of ADC FIFO */
    HAL_ADC_STATE_FIFO_EMPTY,
    HAL_ADC_STATE_FIFO_FULL  /*!< Fifo full */
};

/**
  * @brief  ADC handle Structure definition
  */
typedef struct __ADC_HandleTypeDef
{
    ADC_TypeDef                *Instance;      /*!< ADC registers base address               */
    ADC_InitTypeDef            Init;           /*!< ADC communication parameters             */
    DMA_HandleTypeDef          *hdma;          /*!< Pointer DMA Handler                      */
    uint16_t                   *pBuffPtr;      /*!< Pointer to ADC Tx transfer Buffer        */
    uint16_t                   XferSize;       /*!< ADC Tx Transfer size                     */
    __IO uint16_t              XferCount;      /*!< ADC Tx Transfer Counter                  */
    HAL_LockTypeDef            Lock;           /*!< Locking object                           */
    __IO uint32_t              State;          /*!< ADC communication state                  */
    __IO uint32_t              ErrorCode;      /*!< ADC Error code                           */
    void                       *priv_obj;
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1U)
  void (* ConvCpltCallback)(struct __ADC_HandleTypeDef *hadc);           /*!< ADC conversion complete callback */
  void (* AlmostFullCpltCallback)(struct __ADC_HandleTypeDef *hadc);     /*!< ADC conversion DMA half-transfer callback */
  void (* ErrorCallback)(struct __ADC_HandleTypeDef *hadc);              /*!< ADC error callback */
  void (* MspInitCallback)(struct __ADC_HandleTypeDef *hadc);            /*!< ADC Msp Init callback */
  void (* MspDeInitCallback)(struct __ADC_HandleTypeDef *hadc);          /*!< ADC Msp DeInit callback */
#endif  /* USE_HAL_ADC_REGISTER_CALLBACKS */
} ADC_HandleTypeDef;

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1U)
/**
  * @brief  HAL ADC Callback ID enumeration definition
  */
typedef enum
{
    HAL_ADC_CONVERSION_COMPLETE_CB_ID     = 0x00U,  /*!< ADC conversion complete callback ID */
    HAL_ADC_ALMOST_FULL_CB_ID             = 0x01U,  /*!< ADC conversion DMA half-transfer callback ID */
    HAL_ADC_ERROR_CB_ID                   = 0x02U,  /*!< ADC error callback ID */
    HAL_ADC_MSPINIT_CB_ID                 = 0x03U,  /*!< ADC Msp Init callback ID          */
    HAL_ADC_MSPDEINIT_CB_ID               = 0x04U   /*!< ADC Msp DeInit callback ID        */
} HAL_ADC_CallbackIDTypeDef;

/**
  * @brief  HAL ADC Callback pointer definition
  */
typedef  void (*pADC_CallbackTypeDef)(ADC_HandleTypeDef *hadc); /*!< pointer to an ADC callback function */

#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup ADC_Error_Code ADC Error Code
  * @{
  */
#define HAL_ADC_ERROR_NONE        0x00U   /*!< No error                                              */
#define HAL_ADC_ERROR_INTERNAL    0x01U   /*!< ADC IP internal error: if problem of clocking, 
                                               enable/disable, erroneous state                       */
#define HAL_ADC_ERROR_FULL        0x02U   /*!< Fifo full error                                         */
#define HAL_ADC_ERROR_DMA         0x04U   /*!< DMA transfer error                                    */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
#define HAL_ADC_ERROR_INVALID_CALLBACK  (0x10U)   /*!< Invalid Callback error */
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @defgroup ADC_Mode ADC Convert Mode Selection
  * @{
  */
#define ADC_CONVERT_SINGLE_MODE           0x0U
#define ADC_CONVERT_CONTINUE_MODE         0x1U
/**
  * @}
  */ 

/** @defgroup ADC_channels  ADC Common Channels
  * @{
  */ 
#define ADC_CHANNEL_0           (0U)
#define ADC_CHANNEL_1           (1U)
#define ADC_CHANNEL_2           (2U)
#define ADC_CHANNEL_3           (3U)
#define ADC_CHANNEL_4           (4U)
#define ADC_CHANNEL_5           (5U)
#define ADC_CHANNEL_6           (6U)
#define ADC_CHANNEL_7           (7U)
#define ADC_CHANNEL_8           (8U)
#define ADC_CHANNEL_9           (9U)   //reserved
#define ADC_CHANNEL_10          (10U)  //reserved
#define ADC_CHANNEL_11          (11U)  //reserved
#define ADC_CHANNEL_12          (12U)  //reserved
#define ADC_CHANNEL_13          (13U)  //reserved
#define ADC_CHANNEL_14          (14U)  //core 1.2V
#define ADC_CHANNEL_15          (15U)  //1.8V
#define ADC_CHANNEL_TS          (16U)  //Temperature Sensor channel
#define ADC_CHANNEL_MAX         ADC_CHANNEL_TS
/**
  * @}
  */ 
  
/** @defgroup ADC_interrupts_definition ADC Interrupts Definition
  * @{
  */ 
#define ADC_IT_EOC      ((uint32_t)ADC_INTR_EN)
/**
  * @}
  */ 
    
/** @defgroup ADC_flags_definition ADC Flags Definition
  * @{
  */ 
#define ADC_FLAG_EOC    ((uint32_t)ADC_INTR)
/**
  * @}
  */ 
  
#define ADC_FIFO_DEPTH  16


#define IS_ADC_CLOCKDIV(DIV)           ((DIV) >= 0x2 && ((DIV) <= 0xF))
#define IS_ADC_MODE(MODE)              (((MODE) == ADC_CONVERT_SINGLE_MODE) || \
                                        ((MODE) == ADC_CONVERT_CONTINUE_MODE))
#define IS_ADC_CHANNEL(CH)             ((CH) <= ADC_CHANNEL_MAX)
#define IS_ADC_FULLSIZE(SIZE)          ((SIZE) < ADC_FIFO_DEPTH)
#define IS_ADC_SAMPLETIME(TIME)        ((TIME) >= 0 && ((TIME) <= 0xF))

/**
  * @brief  Enable the ADC peripheral.
  * @param  __HANDLE__ ADC handle
  * @retval None
  */
#define __HAL_ADC_ENABLE(__HANDLE__) ((__HANDLE__)->Instance->CLKDIV |=  ADC_CLKDIV_ADCON)

/**
  * @brief  Disable the ADC peripheral.
  * @param  __HANDLE__ ADC handle
  * @retval None
  */
#define __HAL_ADC_DISABLE(__HANDLE__) ((__HANDLE__)->Instance->CLKDIV &=  ~ADC_CLKDIV_ADCON)

/**
  * @brief  Enable the ADC end of conversion interrupt.
  * @param  __HANDLE__ specifies the ADC Handle.
  * @param  __INTERRUPT__ ADC Interrupt.
  * @retval None
  */
#define __HAL_ADC_ENABLE_IT(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->Instance->INTR_EN) |= (__INTERRUPT__))

/**
  * @brief  Disable the ADC end of conversion interrupt.
  * @param  __HANDLE__ specifies the ADC Handle.
  * @param  __INTERRUPT__ ADC interrupt.
  * @retval None
  */
#define __HAL_ADC_DISABLE_IT(__HANDLE__, __INTERRUPT__) (((__HANDLE__)->Instance->INTR_EN) &= ~(__INTERRUPT__))

/** @brief  Check if the specified ADC interrupt source is enabled or disabled.
  * @param  __HANDLE__ specifies the ADC Handle.
  * @param  __INTERRUPT__ specifies the ADC interrupt source to check.
  * @retval The new state of __IT__ (TRUE or FALSE).
  */
#define __HAL_ADC_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__)  (((__HANDLE__)->Instance->INTR_EN & (__INTERRUPT__)) == (__INTERRUPT__))

/**
  * @brief  Clear the ADC's pending flags.
  * @param  __HANDLE__ specifies the ADC Handle.
  * @param  __FLAG__ ADC flag.
  * @retval None
  */
#define __HAL_ADC_CLEAR_FLAG(__HANDLE__, __FLAG__) (((__HANDLE__)->Instance->INTR) = ~(__FLAG__))

/**
  * @brief  Get the selected ADC's flag status.
  * @param  __HANDLE__ specifies the ADC Handle.
  * @param  __FLAG__ ADC flag.
  * @retval None
  */
#define __HAL_ADC_GET_FLAG(__HANDLE__, __FLAG__) ((((__HANDLE__)->Instance->INTR) & (__FLAG__)) == (__FLAG__))

/**
  * @brief  Wait adc convert done.
  * @param  __HANDLE__ specifies the ADC Handle.
  * @retval None
  */
#define __HAL_ADC_WAIT_DONE(__HANDLE__) while((((__HANDLE__)->Instance->DONE) & ADC_DONE) != ADC_DONE)

/**
  * @brief Simultaneously clears and sets specific bits of the handle State
  * @note: ADC_STATE_CLR_SET() macro is merely aliased to generic macro MODIFY_REG(),
  *        the first parameter is the ADC handle State, the second parameter is the
  *        bit field to clear, the third and last parameter is the bit field to set.
  * @retval None
  */
#define ADC_STATE_CLR_SET MODIFY_REG

/**
  * @brief Clear ADC error code (set it to error code: "no error")
  * @param __HANDLE__ ADC handle
  * @retval None
  */
#define ADC_CLEAR_ERRORCODE(__HANDLE__) ((__HANDLE__)->ErrorCode = HAL_ADC_ERROR_NONE)



/* Exported functions --------------------------------------------------------*/
/** @addtogroup ADC_Exported_Functions
  * @{
  */

/** @addtogroup ADC_Exported_Functions_Group1
  * @{
  */
/* Initialization/de-initialization functions ***********************************/
void HAL_ADC_StructInit(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_DeInit(ADC_HandleTypeDef *hadc);
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc);

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
/* Callbacks Register/UnRegister functions  ***********************************/
HAL_StatusTypeDef HAL_ADC_RegisterCallback(ADC_HandleTypeDef *hadc, HAL_ADC_CallbackIDTypeDef CallbackID, pADC_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_ADC_UnRegisterCallback(ADC_HandleTypeDef *hadc, HAL_ADC_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @addtogroup ADC_Exported_Functions_Group2
  * @{
  */
/* I/O operation functions ******************************************************/
HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_Start_IT(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_Stop_IT(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint16_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_ADC_Stop_DMA(ADC_HandleTypeDef* hadc);

void HAL_ADC_IRQHandler(ADC_HandleTypeDef* hadc);

uint8_t HAL_ADC_GetValue(ADC_HandleTypeDef* hadc, uint16_t *buf, uint8_t size);

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_AlmostFullCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc);

HAL_StatusTypeDef HAL_ADC_SetChannel(ADC_HandleTypeDef* hadc, uint8_t channel);
HAL_StatusTypeDef HAL_ADC_ContinueModeCtrl(ADC_HandleTypeDef* hadc, bool is_enable);
HAL_StatusTypeDef HAL_ADC_SetFreqDiv(ADC_HandleTypeDef* hadc, uint32_t div);
HAL_StatusTypeDef HAL_ADC_SetSampleTime(ADC_HandleTypeDef* hadc, uint32_t clock_num);

#define ADC_0Celsius_ADC_VALUE        (1365.0)
static inline int HAL_ADC_ValueToCelsius(uint16_t value)
{
    float value_float = (float)value;
    float diff = value_float - ADC_0Celsius_ADC_VALUE;
    float mv = diff/4096.0*3.3*1000.0;
    return (int)(mv/6);
}


#endif /* HAL_ADC_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC9002_HAL_ADC_H__ */

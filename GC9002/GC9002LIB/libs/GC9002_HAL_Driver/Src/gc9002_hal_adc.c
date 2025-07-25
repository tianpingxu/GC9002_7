/**
  ******************************************************************************
  * @file    gc9002_hal_adc.c
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2023-1-6
  * @brief   ADC HAL module driver.
  *******************************************************************************/
#include "gc90xx_hal.h"
#ifdef HAL_ADC_MODULE_ENABLED

/** @addtogroup GC9002_HAL_Driver
  * @{
  */

/** @defgroup ADC ADC
  * @brief ADC driver modules
  * @{
  */
    
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** @addtogroup ADC_Private_Functions
  * @{
  */
/* Private function prototypes -----------------------------------------------*/
static void ADC_Init(ADC_HandleTypeDef* hadc);
static void ADC_ClearFIFO(ADC_HandleTypeDef* hadc);
/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
/** @defgroup ADC_Exported_Functions ADC Exported Functions
  * @{
  */

/** @defgroup ADC_Exported_Functions_Group1 Initialization and de-initialization functions 
 *  @brief    Initialization and Configuration functions 
 *
@verbatim    
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Initialize and configure the ADC. 
      (+) De-initialize the ADC. 
         
@endverbatim
  * @{
  */

/**
  * @brief  Fills each ADC_InitStruct member with its default value.
  * @param  ADC_InitStruct: pointer to a ADC_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void HAL_ADC_StructInit(ADC_HandleTypeDef* hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  /* ADC_InitStruct members default value */
  hadc->Init.ClockDiv = 0xF;
  hadc->Init.Mode = ADC_CONVERT_SINGLE_MODE;
  hadc->Init.Channel = ADC_CHANNEL_TS;
  hadc->Init.FullSize = 14;
  
  hadc->Init.SampleTime = 0xA;
  hadc->Init.ALG_MEAN = 0x2;
  hadc->Init.TTRIM = 0x7;

  hadc->Init.ADJ_TD_OS = 0x8;
  hadc->Init.ADJ_TD_GA = 0x8;
}

/**
  * @brief  Initializes the ADCx peripheral according to the specified parameters 
  *         in the ADC_InitStruct and initializes the ADC MSP.
  *           
  * @note   This function is used to configure the global features of the ADC ( 
  *         ClockPrescaler, Resolution, Data Alignment and number of conversion), however,
  *         the rest of the configuration parameters are specific to the regular
  *         channels group (scan mode activation, continuous mode activation,
  *         External trigger source and edge, DMA continuous request after the  
  *         last transfer and End of conversion selection).
  *             
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef* hadc)
{
  /* Check ADC handle */
  if(hadc == NULL)
  {
    return HAL_ERROR;
  }
  
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_CLOCKDIV(hadc->Init.ClockDiv));
  assert_param(IS_ADC_MODE(hadc->Init.Mode));
  assert_param(IS_ADC_CHANNEL(hadc->Init.Channel));
  assert_param(IS_ADC_FULLSIZE(hadc->Init.FullSize));
  assert_param(IS_ADC_SAMPLETIME(hadc->Init.SampleTime));
  
  if(hadc->State == HAL_ADC_STATE_RESET)
  {
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    /* Init the ADC Callback settings */
    hadc->ConvCpltCallback              = HAL_ADC_ConvCpltCallback;                 /* Legacy weak callback */
    hadc->AlmostFullCpltCallback        = HAL_ADC_AlmostFullCpltCallback;           /* Legacy weak callback */
    hadc->ErrorCallback                 = HAL_ADC_ErrorCallback;                    /* Legacy weak callback */
    if (hadc->MspInitCallback == NULL)
    {
      hadc->MspInitCallback = HAL_ADC_MspInit; /* Legacy weak MspInit  */
    }

    /* Init the low level hardware */
    hadc->MspInitCallback(hadc);
#else
    /* Init the low level hardware */
    HAL_ADC_MspInit(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

    /* Initialize ADC error code */
    ADC_CLEAR_ERRORCODE(hadc);
    
    /* Allocate lock resource and initialize it */
    hadc->Lock = HAL_UNLOCKED;
  }
  
  __HAL_ADC_DISABLE(hadc);

  /* Set ADC parameters */
  ADC_Init(hadc);
  
  __HAL_ADC_ENABLE(hadc);

  /* Set ADC error code to none */
  ADC_CLEAR_ERRORCODE(hadc);

  /* Set the ADC state */
  hadc->State = HAL_ADC_STATE_READY;

  /* Release Lock */
  __HAL_UNLOCK(hadc);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Deinitializes the ADCx peripheral registers to their default reset values. 
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_DeInit(ADC_HandleTypeDef* hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Stop potential conversion on going, on regular and injected groups */
  /* Disable ADC peripheral */
  __HAL_ADC_DISABLE(hadc);
  
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
  if (hadc->MspDeInitCallback == NULL)
  {
    hadc->MspDeInitCallback = HAL_ADC_MspDeInit; /* Legacy weak MspDeInit  */
  }

  /* DeInit the low level hardware: RCC clock, NVIC */
  hadc->MspDeInitCallback(hadc);
#else
  /* DeInit the low level hardware: RCC clock, NVIC */
  HAL_ADC_MspDeInit(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
    
  /* Set ADC error code to none */
  ADC_CLEAR_ERRORCODE(hadc);
    
  /* Set ADC state */
  hadc->State = HAL_ADC_STATE_RESET;
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return HAL_OK;
}

#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register a User ADC Callback
  *         To be used instead of the weak predefined callback
  * @param  hadc Pointer to a ADC_HandleTypeDef structure that contains
  *                the configuration information for the specified ADC.
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_ADC_CONVERSION_COMPLETE_CB_ID      ADC conversion complete callback ID
  *          @arg @ref HAL_ADC_CONVERSION_HALF_CB_ID          ADC conversion DMA half-transfer callback ID
  *          @arg @ref HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID    ADC analog watchdog 1 callback ID
  *          @arg @ref HAL_ADC_ERROR_CB_ID                    ADC error callback ID
  *          @arg @ref HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID  ADC group injected conversion complete callback ID
  *          @arg @ref HAL_ADC_INJ_QUEUE_OVEFLOW_CB_ID        ADC group injected context queue overflow callback ID
  *          @arg @ref HAL_ADC_MSPINIT_CB_ID                  ADC Msp Init callback ID
  *          @arg @ref HAL_ADC_MSPDEINIT_CB_ID                ADC Msp DeInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_RegisterCallback(ADC_HandleTypeDef *hadc, HAL_ADC_CallbackIDTypeDef CallbackID, pADC_CallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  if (pCallback == NULL)
  {
    /* Update the error code */
    hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;

    return HAL_ERROR;
  }

  if ((hadc->State & HAL_ADC_STATE_READY) != 0UL)
  {
    switch (CallbackID)
    {
      case HAL_ADC_CONVERSION_COMPLETE_CB_ID :
        hadc->ConvCpltCallback = pCallback;
        break;

      case HAL_ADC_ALMOST_FULL_CB_ID :
        hadc->AlmostFullCpltCallback = pCallback;
        break;

      case HAL_ADC_ERROR_CB_ID :
        hadc->ErrorCallback = pCallback;
        break;

      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = pCallback;
        break;

      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = pCallback;
        break;

      default :
        /* Update the error code */
        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;

        /* Return error status */
        status = HAL_ERROR;
        break;
    }
  }
  else if (HAL_ADC_STATE_RESET == hadc->State)
  {
    switch (CallbackID)
    {
      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = pCallback;
        break;

      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = pCallback;
        break;

      default :
        /* Update the error code */
        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;

        /* Return error status */
        status = HAL_ERROR;
        break;
    }
  }
  else
  {
    /* Update the error code */
    hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;

    /* Return error status */
    status =  HAL_ERROR;
  }

  return status;
}

/**
  * @brief  Unregister a ADC Callback
  *         ADC callback is redirected to the weak predefined callback
  * @param  hadc Pointer to a ADC_HandleTypeDef structure that contains
  *                the configuration information for the specified ADC.
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_ADC_CONVERSION_COMPLETE_CB_ID      ADC conversion complete callback ID
  *          @arg @ref HAL_ADC_CONVERSION_HALF_CB_ID          ADC conversion DMA half-transfer callback ID
  *          @arg @ref HAL_ADC_LEVEL_OUT_OF_WINDOW_1_CB_ID    ADC analog watchdog 1 callback ID
  *          @arg @ref HAL_ADC_ERROR_CB_ID                    ADC error callback ID
  *          @arg @ref HAL_ADC_INJ_CONVERSION_COMPLETE_CB_ID  ADC group injected conversion complete callback ID
  *          @arg @ref HAL_ADC_INJ_QUEUE_OVEFLOW_CB_ID        ADC group injected context queue overflow callback ID
  *          @arg @ref HAL_ADC_MSPINIT_CB_ID                  ADC Msp Init callback ID
  *          @arg @ref HAL_ADC_MSPDEINIT_CB_ID                ADC Msp DeInit callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_UnRegisterCallback(ADC_HandleTypeDef *hadc, HAL_ADC_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

  if ((hadc->State & HAL_ADC_STATE_READY) != 0UL)
  {
    switch (CallbackID)
    {
      case HAL_ADC_CONVERSION_COMPLETE_CB_ID :
        hadc->ConvCpltCallback = HAL_ADC_ConvCpltCallback;
        break;

      case HAL_ADC_ALMOST_FULL_CB_ID :
        hadc->AlmostFullCpltCallback = HAL_ADC_AlmostFullCpltCallback;
        break;

      case HAL_ADC_ERROR_CB_ID :
        hadc->ErrorCallback = HAL_ADC_ErrorCallback;
        break;

      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = HAL_ADC_MspInit; /* Legacy weak MspInit              */
        break;

      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = HAL_ADC_MspDeInit; /* Legacy weak MspDeInit            */
        break;

      default :
        /* Update the error code */
        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;

        /* Return error status */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (HAL_ADC_STATE_RESET == hadc->State)
  {
    switch (CallbackID)
    {
      case HAL_ADC_MSPINIT_CB_ID :
        hadc->MspInitCallback = HAL_ADC_MspInit;                   /* Legacy weak MspInit              */
        break;

      case HAL_ADC_MSPDEINIT_CB_ID :
        hadc->MspDeInitCallback = HAL_ADC_MspDeInit;               /* Legacy weak MspDeInit            */
        break;

      default :
        /* Update the error code */
        hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;

        /* Return error status */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* Update the error code */
    hadc->ErrorCode |= HAL_ADC_ERROR_INVALID_CALLBACK;

    /* Return error status */
    status =  HAL_ERROR;
  }

  return status;
}
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */

/**
  * @brief  Initializes the ADC MSP.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.  
  * @retval None
  */
__WEAK void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_ADC_MspInit could be implemented in the user file
   */ 
}

/**
  * @brief  DeInitializes the ADC MSP.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.  
  * @retval None
  */
__WEAK void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_ADC_MspDeInit could be implemented in the user file
   */ 
}

/**
  * @}
  */

/** @defgroup ADC_Exported_Functions_Group2 IO operation functions
 *  @brief    IO operation functions 
 *
@verbatim   
 ===============================================================================
             ##### IO operation functions #####
 ===============================================================================  
    [..]  This section provides functions allowing to:
      (+) Start conversion of regular channel.
      (+) Stop conversion of regular channel.
      (+) Start conversion of regular channel and enable interrupt.
      (+) Stop conversion of regular channel and disable interrupt.
      (+) Start conversion of regular channel and enable DMA transfer.
      (+) Stop conversion of regular channel and disable DMA transfer.
      (+) Handle ADC interrupt request. 
               
@endverbatim
  * @{
  */

/**
  * @brief  Enables ADC and starts conversion of the regular channels.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Process locked */
  __HAL_LOCK(hadc);
  ADC_ClearFIFO(hadc);
  /* Start conversion if ADC is effectively enabled */
  if(HAL_IS_BIT_SET(hadc->Instance->CLKDIV, ADC_CLKDIV_ADCON))
  {
    /* Set ADC state                                                          */
    /* - Clear state bitfield related to regular group conversion results     */
    /* - Set state bitfield related to regular group operation                */
    hadc->State = HAL_ADC_STATE_BUSY_INTERNAL;

    /* Process unlocked */
    /* Unlock before starting ADC conversions: in case of potential           */
    /* interruption, to let the process to ADC IRQ Handler.                   */
    __HAL_UNLOCK(hadc);
    CLEAR_BIT(hadc->Instance->CONTSTOP, ADC_STOP);
    CLEAR_BIT(hadc->Instance->DONE, ADC_DONE);
    SET_BIT(hadc->Instance->START, ADC_START);
  }
  else
  {
    __HAL_UNLOCK(hadc);
    return HAL_BUSY;
  }
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Disables ADC and stop conversion of regular channels.
  * 
  * @note   Caution: This function will stop also injected channels.  
  *
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  *
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef* hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Process locked */
  __HAL_LOCK(hadc);
  
  SET_BIT(hadc->Instance->CONTSTOP, ADC_STOP);
  
  /* Set ADC state */
  hadc->State = HAL_ADC_STATE_READY;
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Enables the interrupt and starts ADC conversion of regular channels.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_ADC_Start_IT(ADC_HandleTypeDef* hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Process locked */
  __HAL_LOCK(hadc);
  ADC_ClearFIFO(hadc);
  
  /* Start conversion if ADC is effectively enabled */
  if(HAL_IS_BIT_SET(hadc->Instance->CLKDIV, ADC_CLKDIV_ADCON))
  {
    /* Set ADC state                                                          */
    /* - Clear state bitfield related to regular group conversion results     */
    /* - Set state bitfield related to regular group operation                */
    hadc->State = HAL_ADC_STATE_BUSY_INTERNAL;

    /* Process unlocked */
    /* Unlock before starting ADC conversions: in case of potential           */
    /* interruption, to let the process to ADC IRQ Handler.                   */
    __HAL_UNLOCK(hadc);
    hadc->XferCount = 0;
    /* Clear regular group conversion flag and overrun flag */
    /* (To ensure of no unknown state from potential previous ADC operations) */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOC);
    
    /* Enable end of conversion interrupt for regular group */
    __HAL_ADC_ENABLE_IT(hadc, ADC_IT_EOC);

    CLEAR_BIT(hadc->Instance->DONE, ADC_DONE);
    SET_BIT(hadc->Instance->START, ADC_START);
  }
  else
  {
    __HAL_UNLOCK(hadc);
    return HAL_BUSY;
  }
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Disables the interrupt and stop ADC conversion of regular channels.
  * 
  * @note   Caution: This function will stop also injected channels.  
  *
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_ADC_Stop_IT(ADC_HandleTypeDef* hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* Disable ADC end of conversion interrupt for regular group */
  __HAL_ADC_DISABLE_IT(hadc, ADC_IT_EOC);
  
  /* Stop potential conversion on going */
  SET_BIT(hadc->Instance->CONTSTOP, ADC_STOP);
  
  /* Set ADC state */
  hadc->State = HAL_ADC_STATE_READY;
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return HAL_OK;
}
/**
  * @brief  DMA ADC receive process complete callback.
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void ADC_DMAReceiveCplt(DMA_HandleTypeDef *hdma)
{
    ADC_HandleTypeDef *hadc = (ADC_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;
    /* DMA Normal mode*/
    if(!__HAL_DMA_GET_RELOADEN(hdma))
    {
        hadc->XferCount = 0x00U;
    }
    HAL_ADC_Stop_DMA(hadc);
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    /*Call registered complete callback*/
    hadc->ConvCpltCallback(hadc);
#else
    /*Call legacy weak complete callback*/
    HAL_ADC_ConvCpltCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
}
/**
  * @brief  DMA ADC communication error callback.
  * @param  hdma  Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA module.
  * @retval None
  */
static void ADC_DMAError(DMA_HandleTypeDef *hdma)
{
  ADC_HandleTypeDef *hadc = (ADC_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;

  hadc->ErrorCode |= HAL_ADC_ERROR_DMA;
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    /*Call registered error callback*/
    hadc->ErrorCallback(hadc);
#else
    /*Call legacy weak error callback*/
    HAL_ADC_ErrorCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
}
HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint16_t *pData, uint16_t Size)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  /* Process locked */
  __HAL_LOCK(hadc);
  ADC_ClearFIFO(hadc);
  
  /* Start conversion if ADC is effectively enabled */
  if(HAL_IS_BIT_SET(hadc->Instance->CLKDIV, ADC_CLKDIV_ADCON))
  {
    /* Set ADC state                                                          */
    /* - Clear state bitfield related to regular group conversion results     */
    /* - Set state bitfield related to regular group operation                */
    hadc->State = HAL_ADC_STATE_BUSY_INTERNAL;
    /* Process unlocked */
    /* Unlock before starting ADC conversions: in case of potential           */
    /* interruption, to let the process to ADC IRQ Handler.                   */
    __HAL_UNLOCK(hadc);

    hadc->pBuffPtr = pData;
    hadc->XferSize = Size;
    hadc->XferCount = 0;
    /* Set the ADC DMA transfer complete callback */
    hadc->hdma->XferCpltCallback = ADC_DMAReceiveCplt;
    /* Set the DMA error callback */
    hadc->hdma->XferErrorCallback = ADC_DMAError;
    /* Set the DMA abort callback */
    hadc->hdma->XferAbortCallback = NULL;
    /* Set ADC Scan Mode */
    hadc->Instance->ADCCON &= ~ADC_CON_CONT_Msk;
    hadc->Instance->ADCCON |= (ADC_CONVERT_CONTINUE_MODE << ADC_CON_CONT_Pos);

    /* Enable the ADC transmit DMA stream */
    HAL_DMA_Start_IT(hadc->hdma, (uint32_t)&hadc->Instance->DOUT, (uint32_t)pData, Size);
    SET_BIT(hadc->Instance->ADCCON, ADC_CON_DMA_EN);

    /* Clear regular group conversion flag and overrun flag */
    /* (To ensure of no unknown state from potential previous ADC operations) */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOC);
    
    /* Enable end of conversion interrupt for regular group */
    // __HAL_ADC_ENABLE_IT(hadc, ADC_IT_EOC);

    CLEAR_BIT(hadc->Instance->DONE, ADC_DONE);
    SET_BIT(hadc->Instance->START, ADC_START);
  }
  else
  {
    __HAL_UNLOCK(hadc);
    /* Return function status */
    return HAL_ERROR;
  }
  /* Return function status */
  return HAL_OK;
}
HAL_StatusTypeDef HAL_ADC_Stop_DMA(ADC_HandleTypeDef* hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  
  /* Process locked */
  __HAL_LOCK(hadc);
  
  /* Disable ADC end of conversion interrupt for regular group */
  __HAL_ADC_DISABLE_IT(hadc, ADC_IT_EOC);
  
  /* Stop potential conversion on going */
  SET_BIT(hadc->Instance->CONTSTOP, ADC_STOP);
  CLEAR_BIT(hadc->Instance->ADCCON, ADC_CON_DMA_EN);
  
  /* Set ADC state */
  hadc->State = HAL_ADC_STATE_READY;
  
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return HAL_OK;
}
/**
  * @brief  Handles ADC interrupt request  
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @retval None
  */
void HAL_ADC_IRQHandler(ADC_HandleTypeDef* hadc)
{
  uint32_t tmp1 = 0U, tmp2 = 0U;

  tmp1 = __HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOC);
  tmp2 = __HAL_ADC_GET_IT_SOURCE(hadc, ADC_IT_EOC);
  /* Check End of conversion flag for regular channels */
  if(tmp1 && tmp2)
  {    
    /* Conversion complete callback */
#if (USE_HAL_ADC_REGISTER_CALLBACKS == 1)
    hadc->ConvCpltCallback(hadc);
#else
    HAL_ADC_ConvCpltCallback(hadc);
#endif /* USE_HAL_ADC_REGISTER_CALLBACKS */
    
    /* Clear regular group conversion flag */
    __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_EOC);
  }
}

/**
  * @brief  Gets the converted value from data register of regular channel.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @retval the count of adc value reading from fifo
  */
uint8_t HAL_ADC_GetValue(ADC_HandleTypeDef* hadc, uint16_t *buf, uint8_t size)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  // assert_param(IS_ADC_FULLSIZE(size));

  if(HAL_IS_BIT_SET(hadc->Instance->FIFOSTATUS, ADC_FIFOSTA_EMPTY) || buf == NULL)
  {
    return 0;
  }
  
  uint32_t dataCount = hadc->Instance->FIFOSTATUS & ADC_FIFOSTA_DATA_COUNT;
  dataCount = (size <= dataCount ? size : dataCount);
  for(uint32_t i = 0U; i < dataCount; i++)
  {
    buf[i] = (uint16_t)(hadc->Instance->DOUT & 0x00000FFF);
  }
  return dataCount;
}

/**
  * @brief  Regular conversion complete callback in non blocking mode 
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @retval None
  */
__WEAK void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_ADC_ConvCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Regular conversion half DMA transfer callback in non blocking mode 
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @retval None
  */
__WEAK void HAL_ADC_AlmostFullCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_ADC_ConvHalfCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Error ADC callback.
  * @note   In case of error due to overrun when using ADC with DMA transfer 
  *         (HAL ADC handle paramater "ErrorCode" to state "HAL_ADC_ERROR_OVR"):
  *         - Reinitialize the DMA using function "HAL_ADC_Stop_DMA()".
  *         - If needed, restart a new ADC conversion using function
  *           "HAL_ADC_Start_DMA()"
  *           (this function is also clearing overrun flag)
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @retval None
  */
__WEAK void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_ADC_ErrorCallback could be implemented in the user file
   */
}

/** @defgroup ADC_Exported_Functions_Group4 ADC Peripheral State functions
 *  @brief   ADC Peripheral State functions 
 *
@verbatim   
 ===============================================================================
            ##### Peripheral State and errors functions #####
 ===============================================================================  
    [..]
    This subsection provides functions allowing to
      (+) Check the ADC state
      (+) Check the ADC Error
         
@endverbatim
  * @{
  */
  
/**
  * @brief  return the ADC state
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @retval HAL state
  */
uint32_t HAL_ADC_GetState(ADC_HandleTypeDef* hadc)
{
  /* Return ADC state */
  return hadc->State;
}

/**
  * @brief  Return the ADC error code
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @retval ADC Error Code
  */
uint32_t HAL_ADC_GetError(ADC_HandleTypeDef *hadc)
{
  return hadc->ErrorCode;
}

/** @addtogroup ADC_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the ADCx peripheral according to the specified parameters 
  *         in the ADC_InitStruct without initializing the ADC MSP.       
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.  
  * @retval None
  */
static void ADC_Init(ADC_HandleTypeDef* hadc)
{
  /* Set ADC Clk Divide */
  hadc->Instance->CLKDIV &= ~ADC_CLKDIV_Msk;
  hadc->Instance->CLKDIV |= (hadc->Init.ClockDiv | (hadc->Init.ClockDiv << 4));

  /* Clear Dma Enable Bit */
  hadc->Instance->ADCCON &= ~ADC_CON_DMA_EN_Msk;
  /* Set ADC Scan Mode */
  hadc->Instance->ADCCON &= ~ADC_CON_CONT_Msk;
  hadc->Instance->ADCCON |= (hadc->Init.Mode << ADC_CON_CONT_Pos);
  /* Set ADC Convert Channel */
  hadc->Instance->ADCCON &= ~ADC_CON_CH_Msk;
  hadc->Instance->TEMPCON = 0;
  hadc->Instance->TEMPCON |= (hadc->Init.SampleTime << ADC_TEMPCON_ST_SEL_Pos);
  if(hadc->Init.Channel != ADC_CHANNEL_TS)
  {
    hadc->Instance->ADCCON &= ~ADC_CON_VTEMP_Msk;
    hadc->Instance->ADCCON |= hadc->Init.Channel;
    hadc->Instance->TEMPCON &= ~ADC_TEMPCON_EN_TS_Msk;
  }
  else
  {
    hadc->Instance->ADCCON |= ADC_CON_VTEMP_Msk;
    hadc->Instance->TEMPCON |= (hadc->Init.ALG_MEAN << ADC_TEMPCON_ALG_MEAN_Pos);
    hadc->Instance->TEMPCON |= (hadc->Init.TTRIM << ADC_TEMPCON_TIRIM_Pos);
    hadc->Instance->TEMPCON |= (hadc->Init.ADJ_TD_OS << ADC_TEMPCON_TDOS_Pos);
    hadc->Instance->TEMPCON |= (hadc->Init.ADJ_TD_GA << ADC_TEMPCON_TDGA_Pos);
    hadc->Instance->TEMPCON |= ADC_TEMPCON_EN_TS;
    HAL_Delay(1);
  }

  /* Set ADC Full Size */
  hadc->Instance->FULLSIZE &= ~ADC_FULL_SIZE_Msk;
  hadc->Instance->FULLSIZE |= hadc->Init.FullSize;
}
__HAL_ADC_DRIVER_ATTRIBUTE static void ADC_ClearFIFO(ADC_HandleTypeDef* hadc)
{
  uint16_t tempreg = 0;
  uint8_t count = ((hadc->Instance->FIFOSTATUS & ADC_FIFOSTA_DATA_COUNT_Msk) >> ADC_FIFOSTA_DATA_COUNT_Pos);
  for (int i = 0; i < count; i++)
  {
    tempreg = hadc->Instance->DOUT;
  }
}
/**
  * @brief  Set ADC channel.       
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.  
  * @param  channel id of channel
  * @retval None
  */
HAL_StatusTypeDef HAL_ADC_SetChannel(ADC_HandleTypeDef* hadc, uint8_t channel)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  if(!IS_ADC_CHANNEL(channel))
  {
    return HAL_ERROR;
  }
  /* Lock */
  __HAL_LOCK(hadc);
  hadc->Init.Channel = channel;
  __HAL_ADC_DISABLE(hadc);
  /* Set ADC Convert Channel */
  hadc->Instance->ADCCON &= ~ADC_CON_CH_Msk;
  if(hadc->Init.Channel != ADC_CHANNEL_TS)
  {
    hadc->Instance->ADCCON &= ~ADC_CON_VTEMP_Msk;
    hadc->Instance->ADCCON |= hadc->Init.Channel;
    hadc->Instance->TEMPCON &= ~ADC_TEMPCON_EN_TS_Msk;
  }
  else
  {
    hadc->Instance->ADCCON |= ADC_CON_VTEMP_Msk;
    hadc->Instance->TEMPCON |= ADC_TEMPCON_EN_TS;
    HAL_Delay(1);
  }
  __HAL_ADC_ENABLE(hadc);
  /* Release Lock */
  __HAL_UNLOCK(hadc);
  return HAL_OK;
}
/**
  * @brief  ADC Continue Mode controler.       
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.  
  * @param  is_enable  true->enable false->disable
  * @retval None
  */
HAL_StatusTypeDef HAL_ADC_ContinueModeCtrl(ADC_HandleTypeDef* hadc, bool is_enable)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  /* Lock */
  __HAL_LOCK(hadc);
  if(is_enable)
  {
    hadc->Init.Mode = ADC_CONVERT_CONTINUE_MODE;
  }
  else
  {
    hadc->Init.Mode = ADC_CONVERT_SINGLE_MODE;
  }
  __HAL_ADC_DISABLE(hadc);
  /* Set ADC Scan Mode */
  hadc->Instance->ADCCON &= ~ADC_CON_CONT_Msk;
  hadc->Instance->ADCCON |= (hadc->Init.Mode << ADC_CON_CONT_Pos);
  __HAL_ADC_ENABLE(hadc);
  /* Release Lock */
  __HAL_UNLOCK(hadc);
  return HAL_OK;
}
/**
  * @brief  Set ADC frequency division coefficient.       
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.  
  * @param  div  frequency division coefficient
  * @retval None
  */
HAL_StatusTypeDef HAL_ADC_SetFreqDiv(ADC_HandleTypeDef* hadc, uint32_t div)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  if(!IS_ADC_CLOCKDIV(div))
  {
    return HAL_ERROR;
  }
  /* Lock */
  __HAL_LOCK(hadc);
  hadc->Init.ClockDiv = div;
  __HAL_ADC_DISABLE(hadc);
  /* Set ADC Clk Divide */
  hadc->Instance->CLKDIV &= ~ADC_CLKDIV_Msk;
  hadc->Instance->CLKDIV |= (hadc->Init.ClockDiv | (hadc->Init.ClockDiv << 4));
  __HAL_ADC_ENABLE(hadc);
  /* Release Lock */
  __HAL_UNLOCK(hadc);
  return HAL_OK;
}

/**
  * @brief  Set ADC sample time.       
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.  
  * @param  clock_num  Number of clock cycles
  *         0x0: 3
  *         0x1: 5
  *         0x2: 7
  *         0x3: 10
  *         0x4: 13
  *         0x5: 16
  *         0x6: 20
  *         0x7: 30
  *         0x8: 60
  *         0x9: 80
  *         0xA: 100
  *         0xB: 120
  *         0xC: 160
  *         0xD: 320
  *         0xE: 480
  *         0xF: 640
  * @retval None
  */
HAL_StatusTypeDef HAL_ADC_SetSampleTime(ADC_HandleTypeDef* hadc, uint32_t clock_num)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  if(!IS_ADC_SAMPLETIME(clock_num))
  {
    return HAL_ERROR;
  }
  /* Lock */
  __HAL_LOCK(hadc);
  hadc->Init.SampleTime = clock_num;
  __HAL_ADC_DISABLE(hadc);
  /* Set ADC Clk Divide */
  hadc->Instance->TEMPCON &= ~ADC_TEMPCON_ST_SEL_Msk;
  hadc->Instance->TEMPCON |= (hadc->Init.SampleTime << ADC_TEMPCON_ST_SEL_Pos);
  __HAL_ADC_ENABLE(hadc);
  /* Release Lock */
  __HAL_UNLOCK(hadc);
  return HAL_OK;
}
#endif /* HAL_ADC_MODULE_ENABLED */
/*
 * Copyright (c) 2022-2050, GraphichinaSX Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-18     huyt         V1.0.1
 */
#include <gc90xx_hal.h>

#ifdef HAL_I2S_MODULE_ENABLED

/** @defgroup I2S I2S
  * @brief I2S HAL module driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup I2S_Private_Functions I2S Private Functions
  * @{
  */
#if(defined BSP_I2S_TXRX_DMA_ENABLED && BSP_I2S_TXRX_DMA_ENABLED == 1U)
static void               I2S_DMATxCplt(DMA_HandleTypeDef *hdma);
static void               I2S_DMATxHalfCplt(DMA_HandleTypeDef *hdma);
static void               I2S_DMARxCplt(DMA_HandleTypeDef *hdma);
static void               I2S_DMARxHalfCplt(DMA_HandleTypeDef *hdma);
static void               I2S_DMAError(DMA_HandleTypeDef *hdma);
#endif
static void               I2S_Transmit_IT(I2S_HandleTypeDef *hi2s, I2S_ChannelType chx);
static void               I2S_Receive_IT(I2S_HandleTypeDef *hi2s, I2S_ChannelType chx);
static HAL_StatusTypeDef  I2S_WaitFlagStateUntilTimeout(I2S_HandleTypeDef *hi2s, I2S_ChannelType chx, uint32_t Flag, FlagStatus State, uint32_t Timeout);
static HAL_StatusTypeDef  HAL_I2S_ChannelConfig(I2S_HandleTypeDef *hi2s, I2S_ChannelType chx);

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/

/** @defgroup I2S_Exported_Functions I2S Exported Functions
  * @{
  */

/** @defgroup  I2S_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This subsection provides a set of functions allowing to initialize and
          de-initialize the I2Sx peripheral in simplex mode:

      (+) User must Implement HAL_I2S_MspInit() function in which he configures
          all related peripherals resources (CLOCK, GPIO, DMA, IT and NVIC ).

      (+) Call the function HAL_I2S_Init() to configure the selected device with
          the selected configuration:
        (++) Mode
        (++) Standard
        (++) Data Format
        (++) MCLK Output
        (++) Audio frequency
        (++) Polarity
        (++) Full duplex mode

     (+) Call the function HAL_I2S_DeInit() to restore the default configuration
          of the selected I2Sx peripheral.
  @endverbatim
  * @{
  */

/**
  * @brief  Initializes the I2S according to the specified parameters
  *         in the I2S_InitTypeDef and create the associated handle.
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Init(I2S_HandleTypeDef *hi2s)
{
    /* Check the I2S handle allocation */
    if (hi2s == NULL)
    {
        return HAL_ERROR;
    }
    /* Check the I2S parameters */
    assert_param(IS_I2S_INSTANCE(hi2s->Instance));
    assert_param(IS_I2S_MODE(hi2s->Init.Mode));
    assert_param(IS_I2S_STANDARD(hi2s->Init.Standard));
    assert_param(IS_I2S_DATA_FORMAT(hi2s->Init.DataFormat));
    assert_param(IS_I2S_AUDIO_FREQ(hi2s->Init.AudioFreq));

//    hi2s->State = HAL_I2S_STATE_RESET;
    /* Disable I2S */
    __HAL_I2S_DISABLE(hi2s);
    __HAL_I2S_CLK_DISABLE(hi2s);
    if (hi2s->State == HAL_I2S_STATE_RESET)
    {
        /* Allocate lock resource and initialize it */
        hi2s->Lock = HAL_UNLOCKED;
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
        /* Init the I2S Callback settings */
        hi2s->TxCpltCallback       = HAL_I2S_TxCpltCallback;          /* Legacy weak TxCpltCallback       */
        hi2s->RxCpltCallback       = HAL_I2S_RxCpltCallback;          /* Legacy weak RxCpltCallback       */
#if defined (I2S_FULLDUPLEX_SUPPORT)
        hi2s->TxRxCpltCallback     = HAL_I2SEx_TxRxCpltCallback;      /* Legacy weak TxRxCpltCallback     */
#endif
        hi2s->TxHalfCpltCallback   = HAL_I2S_TxHalfCpltCallback;      /* Legacy weak TxHalfCpltCallback   */
        hi2s->RxHalfCpltCallback   = HAL_I2S_RxHalfCpltCallback;      /* Legacy weak RxHalfCpltCallback   */
#if defined (I2S_FULLDUPLEX_SUPPORT)
        hi2s->TxRxHalfCpltCallback = HAL_I2SEx_TxRxHalfCpltCallback;  /* Legacy weak TxRxHalfCpltCallback */
#endif
        hi2s->ErrorCallback        = HAL_I2S_ErrorCallback;           /* Legacy weak ErrorCallback        */
        if (hi2s->MspInitCallback == NULL)
        {
            hi2s->MspInitCallback = HAL_I2S_MspInit; /* Legacy weak MspInit  */
        }
        /* Init the low level hardware : GPIO, CLOCK, NVIC... */
        hi2s->MspInitCallback(hi2s);
#else
        /* Init the low level hardware : GPIO, CLOCK, CORTEX...etc */
        HAL_I2S_MspInit(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
    }
    /* Disabel all channel*/
    __HAL_I2S_RX_BLOCK_DISABLE(hi2s);
    __HAL_I2S_TX_BLOCK_DISABLE(hi2s);
    /* Configuring CCR */
    hi2s->Instance->CCR = (hi2s->Init.DataFormat << I2S_CCR_WSS_Pos) | (NO_CLOCK_GATING << I2S_CCR_SCLKG_Pos);
    /* enable rx and tx block */
    __HAL_I2S_RX_BLOCK_ENABLE(hi2s);
    __HAL_I2S_TX_BLOCK_ENABLE(hi2s);
    __HAL_I2S_RX_FLUSH(hi2s);
    __HAL_I2S_TX_FLUSH(hi2s);
    __HAL_I2S_RX_BLOCK_DMA_ENABLE(hi2s);
    /* Enable I2S peripheral */
    __HAL_I2S_ENABLE(hi2s);
    return HAL_I2S_ChannelConfig(hi2s, hi2s->Init.Channel);
}

static HAL_StatusTypeDef HAL_I2S_ChannelConfig(I2S_HandleTypeDef *hi2s, I2S_ChannelType chx)
{
    /* Check the I2S handle allocation */
    if (hi2s == NULL)
    {
        return HAL_ERROR;
    }
    /* Check the I2S parameters */
    assert_param(IS_I2S_INSTANCE(hi2s->Instance));
    assert_param(IS_I2S_RESOLUTION(hi2s->Init.RxResolution));
    assert_param(IS_I2S_RESOLUTION(hi2s->Init.TxResolution));
    assert_param(IS_I2S_TRIGGER_LEVEL(hi2s->Init.RxTriggerLevel));
    assert_param(IS_I2S_TRIGGER_LEVEL(hi2s->Init.TxTriggerLevel));
    assert_param(IS_I2S_CHANNEL(chx));

    __HAL_I2S_CHX_RX_DISABLE(hi2s, chx);
    __HAL_I2S_CHX_TX_DISABLE(hi2s, chx);
    __HAL_I2S_CHX_RX_FIFO_RESET(hi2s, chx);
    __HAL_I2S_CHX_TX_FIFO_RESET(hi2s, chx);
    /* Configuring Transmit FIFO Trigger Level */
    hi2s->Instance->Chx[chx].RFCR = hi2s->Init.RxTriggerLevel;
    hi2s->Instance->Chx[chx].TFCR = hi2s->Init.TxTriggerLevel;
    /* Configuring RCRx & TCRx */
    hi2s->Instance->Chx[chx].RCR = hi2s->Init.RxResolution;
    hi2s->Instance->Chx[chx].TCR = hi2s->Init.TxResolution;

    __HAL_I2S_CHX_DISABLE_IT(hi2s, chx, I2S_IT_TXFOM);
    __HAL_I2S_CHX_DISABLE_IT(hi2s, chx, I2S_IT_TXFEM);
    __HAL_I2S_CHX_DISABLE_IT(hi2s, chx, I2S_IT_RXFOM);
    __HAL_I2S_CHX_DISABLE_IT(hi2s, chx, I2S_IT_RXDAM);
    __HAL_I2S_CHX_RX_DISABLE(hi2s, chx);
    __HAL_I2S_CHX_TX_DISABLE(hi2s, chx);
    hi2s->Lock = HAL_UNLOCKED;
    hi2s->State = HAL_I2S_STATE_READY;
    hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
    if(hi2s->Init.Mode == I2S_MODE_MASTER)
    {
        __HAL_I2S_CLK_ENABLE(hi2s);
    }
    return HAL_OK;
}
void HAL_I2S_StructInit(I2S_InitTypeDef* I2S_InitStruct)
{
    assert_param(I2S_InitStruct != NULL);
    I2S_InitStruct->Channel = I2S_CHANNEL_0;
    /* Initialize the I2S_Mode member */
    I2S_InitStruct->Mode = I2S_MODE_MASTER;
    /* Initialize the Standard member */
    I2S_InitStruct->Standard = I2S_STANDARD_PHILIPS;
    /* Initialize the DataFormat member */
    I2S_InitStruct->DataFormat = I2S_WSS_CLOCK_CYCLES_32;
    /* Initialize the AudioFreq member */
    I2S_InitStruct->AudioFreq = I2S_AUDIOFREQ_48K;
    I2S_InitStruct->RxResolution = I2S_WLEN_RESOLUTION_16_BIT;
    I2S_InitStruct->TxResolution = I2S_WLEN_RESOLUTION_16_BIT;
    I2S_InitStruct->RxTriggerLevel = I2S_TRIGGER_LEVEL_1;
    I2S_InitStruct->TxTriggerLevel = I2S_TRIGGER_LEVEL_1;
}
/**
  * @brief DeInitializes the I2S peripheral
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_DeInit(I2S_HandleTypeDef *hi2s)
{
    /* Check the I2S handle allocation */
    if (hi2s == NULL)
    {
        return HAL_ERROR;
    }
    /* Check the parameters */
    assert_param(IS_I2S_INSTANCE(hi2s->Instance));
    hi2s->State = HAL_I2S_STATE_RESET;
    /* Disable the I2S Peripheral Clock */
    __HAL_I2S_DISABLE(hi2s);
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
    if (hi2s->MspDeInitCallback == NULL)
    {
        hi2s->MspDeInitCallback = HAL_I2S_MspDeInit; /* Legacy weak MspDeInit  */
    }
    /* DeInit the low level hardware: GPIO, CLOCK, NVIC... */
    hi2s->MspDeInitCallback(hi2s);
#else
    /* DeInit the low level hardware: GPIO, CLOCK, NVIC... */
    HAL_I2S_MspDeInit(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
    hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
    hi2s->State     = HAL_I2S_STATE_RESET;
    /* Release Lock */
    hi2s->Lock = HAL_UNLOCKED;
    return HAL_OK;
}

/**
  * @brief I2S MSP Init
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__WEAK void HAL_I2S_MspInit(I2S_HandleTypeDef *hi2s)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2s);

    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_MspInit could be implemented in the user file
    */
}

/**
  * @brief I2S MSP DeInit
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__WEAK void HAL_I2S_MspDeInit(I2S_HandleTypeDef *hi2s)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2s);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_MspDeInit could be implemented in the user file
   */
}

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
/**
  * @brief  Register a User I2S Callback
  *         To be used instead of the weak predefined callback
  * @param  hi2s Pointer to a I2S_HandleTypeDef structure that contains
  *                the configuration information for the specified I2S.
  * @param  CallbackID ID of the callback to be registered
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_RegisterCallback(I2S_HandleTypeDef *hi2s, HAL_I2S_CallbackIDTypeDef CallbackID, pI2S_CallbackTypeDef pCallback)
{
    HAL_StatusTypeDef status = HAL_OK;
    if (pCallback == NULL)
    {
        /* Update the error code */
        hi2s->ErrorCode |= HAL_I2S_ERROR_INVALID_CALLBACK;
        return HAL_ERROR;
    }
    /* Process locked */
    __HAL_LOCK(hi2s);
    if (HAL_I2S_STATE_READY == hi2s->State)
    {
        switch (CallbackID)
        {
        case HAL_I2S_TX_COMPLETE_CB_ID :
            hi2s->TxCpltCallback = pCallback;
        break;

        case HAL_I2S_RX_COMPLETE_CB_ID :
            hi2s->RxCpltCallback = pCallback;
        break;

#if defined (I2S_FULLDUPLEX_SUPPORT)
        case HAL_I2S_TX_RX_COMPLETE_CB_ID :
            hi2s->TxRxCpltCallback = pCallback;
        break;
#endif

        case HAL_I2S_TX_HALF_COMPLETE_CB_ID :
            hi2s->TxHalfCpltCallback = pCallback;
        break;

        case HAL_I2S_RX_HALF_COMPLETE_CB_ID :
            hi2s->RxHalfCpltCallback = pCallback;
        break;

#if defined (I2S_FULLDUPLEX_SUPPORT)
        case HAL_I2S_TX_RX_HALF_COMPLETE_CB_ID :
            hi2s->TxRxHalfCpltCallback = pCallback;
        break;
#endif

        case HAL_I2S_ERROR_CB_ID :
            hi2s->ErrorCallback = pCallback;
        break;

        case HAL_I2S_MSPINIT_CB_ID :
            hi2s->MspInitCallback = pCallback;
        break;

        case HAL_I2S_MSPDEINIT_CB_ID :
            hi2s->MspDeInitCallback = pCallback;
        break;

        default :
            /* Update the error code */
            SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);
            /* Return error status */
            status =  HAL_ERROR;
        break;
        }
    }
    else if (HAL_I2S_STATE_RESET == hi2s->State)
    {
        switch (CallbackID)
        {
        case HAL_I2S_MSPINIT_CB_ID :
            hi2s->MspInitCallback = pCallback;
        break;

        case HAL_I2S_MSPDEINIT_CB_ID :
            hi2s->MspDeInitCallback = pCallback;
        break;

        default :
            /* Update the error code */
            SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);
            /* Return error status */
            status =  HAL_ERROR;
        break;
        }
    }
    else
    {
        /* Update the error code */
        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);
        /* Return error status */
        status =  HAL_ERROR;
    }
    /* Release Lock */
    __HAL_UNLOCK(hi2s);
    return status;
}

/**
  * @brief  Unregister an I2S Callback
  *         I2S callback is redirected to the weak predefined callback
  * @param  hi2s Pointer to a I2S_HandleTypeDef structure that contains
  *                the configuration information for the specified I2S.
  * @param  CallbackID ID of the callback to be unregistered
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_UnRegisterCallback(I2S_HandleTypeDef *hi2s, HAL_I2S_CallbackIDTypeDef CallbackID)
{
    HAL_StatusTypeDef status = HAL_OK;
    /* Process locked */
    __HAL_LOCK(hi2s);
    if (HAL_I2S_STATE_READY == hi2s->State)
    {
        switch (CallbackID)
        {
        case HAL_I2S_TX_COMPLETE_CB_ID :
            hi2s->TxCpltCallback = HAL_I2S_TxCpltCallback;                /* Legacy weak TxCpltCallback       */
        break;

        case HAL_I2S_RX_COMPLETE_CB_ID :
            hi2s->RxCpltCallback = HAL_I2S_RxCpltCallback;                /* Legacy weak RxCpltCallback       */
        break;

#if defined (I2S_FULLDUPLEX_SUPPORT)
        case HAL_I2S_TX_RX_COMPLETE_CB_ID :
            hi2s->TxRxCpltCallback = HAL_I2SEx_TxRxCpltCallback;          /* Legacy weak TxRxCpltCallback     */
        break;
#endif

        case HAL_I2S_TX_HALF_COMPLETE_CB_ID :
            hi2s->TxHalfCpltCallback = HAL_I2S_TxHalfCpltCallback;        /* Legacy weak TxHalfCpltCallback   */
        break;

        case HAL_I2S_RX_HALF_COMPLETE_CB_ID :
            hi2s->RxHalfCpltCallback = HAL_I2S_RxHalfCpltCallback;        /* Legacy weak RxHalfCpltCallback   */
        break;

#if defined (I2S_FULLDUPLEX_SUPPORT)
        case HAL_I2S_TX_RX_HALF_COMPLETE_CB_ID :
            hi2s->TxRxHalfCpltCallback = HAL_I2SEx_TxRxHalfCpltCallback;  /* Legacy weak TxRxHalfCpltCallback */
        break;
#endif

        case HAL_I2S_ERROR_CB_ID :
            hi2s->ErrorCallback = HAL_I2S_ErrorCallback;                  /* Legacy weak ErrorCallback        */
        break;

        case HAL_I2S_MSPINIT_CB_ID :
            hi2s->MspInitCallback = HAL_I2S_MspInit;                      /* Legacy weak MspInit              */
        break;

        case HAL_I2S_MSPDEINIT_CB_ID :
            hi2s->MspDeInitCallback = HAL_I2S_MspDeInit;                  /* Legacy weak MspDeInit            */
        break;

        default :
            /* Update the error code */
            SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);
            /* Return error status */
            status =  HAL_ERROR;
        break;
        }
    }
    else if (HAL_I2S_STATE_RESET == hi2s->State)
    {
        switch (CallbackID)
        {
        case HAL_I2S_MSPINIT_CB_ID :
            hi2s->MspInitCallback = HAL_I2S_MspInit;                      /* Legacy weak MspInit              */
        break;

        case HAL_I2S_MSPDEINIT_CB_ID :
            hi2s->MspDeInitCallback = HAL_I2S_MspDeInit;                  /* Legacy weak MspDeInit            */
        break;

        default :
            /* Update the error code */
            SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);
            /* Return error status */
            status =  HAL_ERROR;
        break;
        }
    }
    else
    {
        /* Update the error code */
        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_INVALID_CALLBACK);
        /* Return error status */
        status =  HAL_ERROR;
    }
    /* Release Lock */
    __HAL_UNLOCK(hi2s);
    return status;
}
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @defgroup I2S_Exported_Functions_Group2 IO operation functions
  *  @brief Data transfers functions
  *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the I2S data
    transfers.

    (#) There are two modes of transfer:
       (++) Blocking mode : The communication is performed in the polling mode.
            The status of all data processing is returned by the same function
            after finishing transfer.
       (++) No-Blocking mode : The communication is performed using Interrupts
            or DMA. These functions return the status of the transfer startup.
            The end of the data processing will be indicated through the
            dedicated I2S IRQ when using Interrupt mode or the DMA IRQ when
            using DMA mode.

    (#) Blocking mode functions are :
        (++) HAL_I2S_Transmit()
        (++) HAL_I2S_Receive()

    (#) No-Blocking mode functions with Interrupt are :
        (++) HAL_I2S_Transmit_IT()
        (++) HAL_I2S_Receive_IT()

    (#) No-Blocking mode functions with DMA are :
        (++) HAL_I2S_Transmit_DMA()
        (++) HAL_I2S_Receive_DMA()

    (#) A set of Transfer Complete Callbacks are provided in non Blocking mode:
        (++) HAL_I2S_TxCpltCallback()
        (++) HAL_I2S_RxCpltCallback()
        (++) HAL_I2S_ErrorCallback()

@endverbatim
  * @{
  */
/**
  * @brief  Transmit an amount of data in blocking mode
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  chx I2S channel.
  * @param  pData a 8-bit pointer to data buffer.
  * @param  Size number of data sample to be sent(bits):
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Transmit(I2S_HandleTypeDef *hi2s, uint8_t *pData, uint32_t Size, uint32_t Timeout)
{
    if ((hi2s == NULL) || (pData == NULL) || (Size == 0U))
    {
        return  HAL_ERROR;
    }
    I2S_ChannelType chx = hi2s->Init.Channel;
    if(chx >= I2S_TX_CHANNELS)
    {
        return  HAL_ERROR;
    }
    /* Process Locked */
    __HAL_LOCK(hi2s);
    if (hi2s->State != HAL_I2S_STATE_RESET &&
       (hi2s->State & (HAL_I2S_STATE_BUSY_TX | HAL_I2S_STATE_BUSY_DMA_TX | HAL_I2S_STATE_ERROR)) == RESET)
    {
        /* Set state and reset error code */
        hi2s->State |= HAL_I2S_STATE_BUSY_TX;
        hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
        hi2s->pTxBuffPtr = pData;
        uint32_t resolution = hi2s->Instance->Chx[chx].TCR & I2S_TCR_WLEN_Msk;
        if(resolution == I2S_WLEN_RESOLUTION_12_BIT || resolution == I2S_WLEN_RESOLUTION_16_BIT)
        {
            hi2s->TX_Word_size = 2;
        }
        else
        {
            hi2s->TX_Word_size = 4;
        }
        hi2s->TxXferCount = Size/(hi2s->TX_Word_size*2);
        /* Enable I2S peripheral */
        __HAL_I2S_CHX_DISABLE_IT(hi2s, chx, I2S_IT_TXFOM | I2S_IT_TXFEM);
        __HAL_I2S_CHX_TX_ENABLE(hi2s, chx);
        while (hi2s->TxXferCount > 0U)
        {
            // FIFO not overflow
            if(!__HAL_I2S_CHX_GET_FLAG(hi2s, chx, I2S_FLAG_TXFO))
            {
                hi2s->Instance->Chx[chx].LTHR = ReadUint32_LittleEndian(hi2s->pTxBuffPtr);
                hi2s->pTxBuffPtr += hi2s->TX_Word_size;
                hi2s->Instance->Chx[chx].RTHR = ReadUint32_LittleEndian(hi2s->pTxBuffPtr);
                hi2s->pTxBuffPtr += hi2s->TX_Word_size;
                hi2s->TxXferCount--;
            }
            else
            {
                /* Wait until TXFE flag is set */
                if (I2S_WaitFlagStateUntilTimeout(hi2s, chx, I2S_FLAG_TXFE, SET, Timeout) == HAL_OK)
                {
                    __HAL_I2S_CHX_TX_CLEAR_OVRFLAG(hi2s, chx);
                }
                else
                {
                    /* Set the error code */
                    SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_TIMEOUT);
                    hi2s->State &= ~HAL_I2S_STATE_BUSY_TX;
                    __HAL_UNLOCK(hi2s);
                    return HAL_ERROR;
                }
            }
        }
        while(!__HAL_I2S_CHX_GET_FLAG(hi2s, chx, I2S_FLAG_TXFE));
        __HAL_I2S_CHX_TX_DISABLE(hi2s, chx);
        hi2s->State &= ~HAL_I2S_STATE_BUSY_TX;
        __HAL_UNLOCK(hi2s);
        return HAL_OK;
    }
    else
    {
        __HAL_UNLOCK(hi2s);
        return HAL_BUSY;
    }
}

/**
  * @brief  Receive an amount of data in blocking mode
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  chx I2S channel.
  * @param  pData a 8-bit pointer to data buffer.
  * @param  Size number of data sample to be sent(bits)
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Receive(I2S_HandleTypeDef *hi2s, uint8_t *pData, uint32_t Size, uint32_t Timeout)
{
    uint32_t word_Read = 0;
    uint32_t i = 0;

    if ((hi2s == NULL) || (pData == NULL) || (Size == 0U))
    {
        return  HAL_ERROR;
    }
    I2S_ChannelType chx = hi2s->Init.Channel;
    if(chx >= I2S_RX_CHANNELS)
    {
        return  HAL_ERROR;
    }
    /* Process Locked */
    __HAL_LOCK(hi2s);
    if (hi2s->State != HAL_I2S_STATE_RESET &&
       (hi2s->State & (HAL_I2S_STATE_BUSY_RX | HAL_I2S_STATE_BUSY_DMA_RX | HAL_I2S_STATE_ERROR)) == RESET)
    {
        /* Set state and reset error code */
        hi2s->State |= HAL_I2S_STATE_BUSY_RX;
        hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
        hi2s->pRxBuffPtr = pData;
        uint32_t resolution = hi2s->Instance->Chx[chx].RCR & I2S_RCR_WLEN_Msk;
        if(resolution == I2S_WLEN_RESOLUTION_12_BIT || resolution == I2S_WLEN_RESOLUTION_16_BIT)
        {
            hi2s->RX_Word_size = 2;
        }
        else
        {
            hi2s->RX_Word_size = 4;
        }
        hi2s->RxXferCount = Size/(hi2s->RX_Word_size*2);
        __HAL_I2S_CHX_DISABLE_IT(hi2s, chx, I2S_IT_RXFOM | I2S_IT_RXDAM);
        __HAL_I2S_CHX_RX_CLEAR_OVRFLAG(hi2s, chx);
        __HAL_I2S_CHX_RX_ENABLE(hi2s, chx);
        while (hi2s->RxXferCount > 0U)
        {
            // FIFO not overflow
            if(__HAL_I2S_CHX_GET_FLAG(hi2s, chx, I2S_FLAG_RXDA))
            {
                if(hi2s->RX_Word_size == 2)
                {
                    WriteUint16_LittleEndian(hi2s->pRxBuffPtr, hi2s->Instance->Chx[chx].LRBR & 0x0000FFFF);
                    hi2s->pRxBuffPtr += hi2s->RX_Word_size;
                    WriteUint16_LittleEndian(hi2s->pRxBuffPtr, hi2s->Instance->Chx[chx].RRBR & 0x0000FFFF);
                    hi2s->pRxBuffPtr += hi2s->RX_Word_size;
                }
                else
                {
                    WriteUint32_LittleEndian(hi2s->pRxBuffPtr, hi2s->Instance->Chx[chx].LRBR);
                    hi2s->pRxBuffPtr += hi2s->RX_Word_size;
                    WriteUint32_LittleEndian(hi2s->pRxBuffPtr, hi2s->Instance->Chx[chx].RRBR);
                    hi2s->pRxBuffPtr += hi2s->RX_Word_size;
                }
                hi2s->TxXferCount--;
            }
            else
            {
                /* Wait until TXE flag is set */
                if (I2S_WaitFlagStateUntilTimeout(hi2s, chx, I2S_FLAG_RXDA, SET, Timeout) != HAL_OK)
                {
                    /* Set the error code */
                    SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_TIMEOUT);
                    hi2s->State &= ~HAL_I2S_STATE_BUSY_RX;
                    __HAL_UNLOCK(hi2s);
                    return HAL_ERROR;
                }
            }
        }
        __HAL_I2S_CHX_RX_DISABLE(hi2s, chx);
        hi2s->State &= ~HAL_I2S_STATE_BUSY_RX;
        __HAL_UNLOCK(hi2s);
        return HAL_OK;
    }
    else
    {
        __HAL_UNLOCK(hi2s);
        return HAL_BUSY;
    }
}

/**
  * @brief  Transmit an amount of data in non-blocking mode with Interrupt
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  chx I2S channel.
  * @param  pData a 8-bit pointer to data buffer.
  * @param  Size number of data sample to be sent:
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Transmit_IT(I2S_HandleTypeDef *hi2s, uint8_t *pData, uint32_t Size)
{
    if ((hi2s == NULL) || (pData == NULL) || (Size == 0U))
    {
        return  HAL_ERROR;
    }
    I2S_ChannelType chx = hi2s->Init.Channel;
    if(chx >= I2S_TX_CHANNELS)
    {
        return  HAL_ERROR;
    }
    /* Process Locked */
    __HAL_LOCK(hi2s);
    if (hi2s->State != HAL_I2S_STATE_RESET &&
       (hi2s->State & (HAL_I2S_STATE_BUSY_TX | HAL_I2S_STATE_BUSY_DMA_TX | HAL_I2S_STATE_ERROR)) == RESET)
    {
//        while(!__HAL_I2S_CHX_GET_FLAG(hi2s, chx, I2S_FLAG_TXFE));
        /* Set state and reset error code */
        hi2s->State |= HAL_I2S_STATE_BUSY_TX;
        hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
        hi2s->pTxBuffPtr = pData;
        uint32_t resolution = hi2s->Instance->Chx[chx].TCR & I2S_TCR_WLEN_Msk;
        if(resolution == I2S_WLEN_RESOLUTION_12_BIT || resolution == I2S_WLEN_RESOLUTION_16_BIT)
        {
            hi2s->TX_Word_size = 2;
        }
        else
        {
            hi2s->TX_Word_size = 4;
        }
        hi2s->TxXferCount = Size/(hi2s->TX_Word_size*2);
        /* Enable I2S peripheral */
        __HAL_I2S_CHX_TX_ENABLE(hi2s, chx);
        __HAL_I2S_CHX_ENABLE_IT(hi2s, chx, I2S_IT_TXFOM | I2S_IT_TXFEM);
        __HAL_UNLOCK(hi2s);
        return HAL_OK;
    }
    else
    {
        __HAL_UNLOCK(hi2s);
        return HAL_BUSY;
    }
}

/**
  * @brief  Receive an amount of data in non-blocking mode with Interrupt
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  chx I2S channel.
  * @param  pData a 8-bit pointer to the Receive data buffer.
  * @param  Size number of data sample to be sent:
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Receive_IT(I2S_HandleTypeDef *hi2s, uint8_t *pData, uint32_t Size)
{
    if ((hi2s == NULL) || (pData == NULL) || (Size == 0U))
    {
        return  HAL_ERROR;
    }
    I2S_ChannelType chx = hi2s->Init.Channel;
    if(chx >= I2S_RX_CHANNELS)
    {
        return  HAL_ERROR;
    }
    /* Process Locked */
    __HAL_LOCK(hi2s);
    if (hi2s->State != HAL_I2S_STATE_RESET &&
       (hi2s->State & (HAL_I2S_STATE_BUSY_RX | HAL_I2S_STATE_BUSY_DMA_RX | HAL_I2S_STATE_ERROR)) == RESET)
    {
        /* Set state and reset error code */
        hi2s->State |= HAL_I2S_STATE_BUSY_RX;
        hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
        hi2s->pRxBuffPtr = pData;
        uint32_t resolution = hi2s->Instance->Chx[chx].RCR & I2S_RCR_WLEN_Msk;
        if(resolution == I2S_WLEN_RESOLUTION_12_BIT || resolution == I2S_WLEN_RESOLUTION_16_BIT)
        {
            hi2s->RX_Word_size = 2;
        }
        else
        {
            hi2s->RX_Word_size = 4;
        }
        hi2s->RxXferCount = Size/(hi2s->RX_Word_size*2);
        /* Enable I2S peripheral */
        __HAL_I2S_CHX_RX_ENABLE(hi2s, chx);
        __HAL_I2S_CHX_ENABLE_IT(hi2s, chx, I2S_IT_RXFOM | I2S_IT_RXDAM);
        __HAL_UNLOCK(hi2s);
        return HAL_OK;
    }
    else
    {
        __HAL_UNLOCK(hi2s);
        return HAL_BUSY;
    }
}

/**
  * @brief  Transmit an amount of data in non-blocking mode with DMA
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  chx I2S channel.
  * @param  pData a 8-bit pointer to the Transmit data buffer.
  * @param  Size number of data sample to be sent:
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Transmit_DMA(I2S_HandleTypeDef *hi2s, uint8_t *pData, uint32_t Size)
{
#if(defined BSP_I2S_TXRX_DMA_ENABLED && BSP_I2S_TXRX_DMA_ENABLED == 1U)
    if ((hi2s == NULL) || (pData == NULL) || (Size == 0U))
    {
        return  HAL_ERROR;
    }
    I2S_ChannelType chx = hi2s->Init.Channel;
    if(chx >= I2S_TX_CHANNELS)
    {
        return  HAL_ERROR;
    }
    /* Process Locked */
    __HAL_LOCK(hi2s);
    if (hi2s->State != HAL_I2S_STATE_RESET &&
       (hi2s->State & (HAL_I2S_STATE_BUSY_TX | HAL_I2S_STATE_BUSY_DMA_TX |  HAL_I2S_STATE_ERROR)) == RESET)
    {
//        while(!__HAL_I2S_CHX_GET_FLAG(hi2s, chx, I2S_FLAG_TXFE));
        /* Set state and reset error code */
        hi2s->State |= HAL_I2S_STATE_BUSY_DMA_TX;
        hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
        hi2s->pTxBuffPtr = pData;
        uint32_t resolution = hi2s->Instance->Chx[chx].TCR & I2S_TCR_WLEN_Msk;
        if(resolution == I2S_WLEN_RESOLUTION_12_BIT ||resolution == I2S_WLEN_RESOLUTION_16_BIT)
        {
            hi2s->TX_Word_size = 2;
        } else {
            hi2s->TX_Word_size = 4;
        }
        hi2s->TxXferSize = Size/hi2s->TX_Word_size;
        hi2s->hdmatx->Parent = hi2s;
        HAL_DMA_RegisterCallback(hi2s->hdmatx, HAL_DMA_XFER_CPLT_CB_ID, I2S_DMATxCplt);
        HAL_DMA_RegisterCallback(hi2s->hdmatx, HAL_DMA_XFER_ERROR_CB_ID, I2S_DMAError);

        /* Enable I2S peripheral */
        __HAL_I2S_CHX_DISABLE_IT(hi2s, chx, I2S_IT_TXFEM);
        __HAL_I2S_CHX_TX_DMA_ENABLE(hi2s, chx);
        __HAL_I2S_CHX_TX_ENABLE(hi2s, chx);
        /* Enable the Tx DMA Stream/Channel */
        if (HAL_OK != HAL_DMA_Start_IT(hi2s->hdmatx, (uint32_t)hi2s->pTxBuffPtr, (uint32_t)&hi2s->Instance->TXDMA_CHx[chx], hi2s->TxXferSize))
        {
            /* Update SPI error code */
            SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_DMA);
            hi2s->State &= ~HAL_I2S_STATE_BUSY_DMA_TX;
            __HAL_UNLOCK(hi2s);
            return HAL_ERROR;
        }
        __HAL_UNLOCK(hi2s);
        return HAL_OK;
    }
    else
    {
        __HAL_UNLOCK(hi2s);
        return HAL_BUSY;
    }
#else
    UNUSED(hi2s);
    UNUSED(pData);
    UNUSED(Size);
    return HAL_ERROR;
#endif
}

/**
  * @brief  Receive an amount of data in non-blocking mode with DMA
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  pData a 16-bit pointer to the Receive data buffer.
  * @param  Size number of data sample to be sent:
  * @note   When a 16-bit data frame or a 16-bit data frame extended is selected during the I2S
  *         configuration phase, the Size parameter means the number of 16-bit data length
  *         in the transaction and when a 24-bit data frame or a 32-bit data frame is selected
  *         the Size parameter means the number of 16-bit data length.
  * @note   The I2S is kept enabled at the end of transaction to avoid the clock de-synchronization
  *         between Master and Slave(example: audio streaming).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_Receive_DMA(I2S_HandleTypeDef *hi2s, uint8_t *pData, uint32_t Size)
{
#if(defined BSP_I2S_TXRX_DMA_ENABLED && BSP_I2S_TXRX_DMA_ENABLED == 1U)
    if ((hi2s == NULL) || (pData == NULL) || (Size == 0U))
    {
        return  HAL_ERROR;
    }
    I2S_ChannelType chx = hi2s->Init.Channel;
    if(chx >= I2S_RX_CHANNELS)
    {
        return  HAL_ERROR;
    }
    /* Process Locked */
    __HAL_LOCK(hi2s);
    if (hi2s->State != HAL_I2S_STATE_RESET &&
       (hi2s->State & (HAL_I2S_STATE_BUSY_RX | HAL_I2S_STATE_BUSY_DMA_RX | HAL_I2S_STATE_ERROR)) == RESET)
    {
        /* Set state and reset error code */
        hi2s->State |= HAL_I2S_STATE_BUSY_DMA_RX;
        hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
        hi2s->pRxBuffPtr = pData;
        hi2s->RxXferSize = Size;
        uint32_t resolution = hi2s->Instance->Chx[chx].RCR & I2S_RCR_WLEN_Msk;
        if(resolution == I2S_WLEN_RESOLUTION_12_BIT || resolution == I2S_WLEN_RESOLUTION_16_BIT)
        {
            hi2s->RX_Word_size = 2;
        }
        else
        {
            hi2s->RX_Word_size = 4;
        }
        hi2s->hdmarx->Parent = hi2s;
        HAL_DMA_RegisterCallback(hi2s->hdmarx, HAL_DMA_XFER_CPLT_CB_ID, I2S_DMARxCplt);
        HAL_DMA_RegisterCallback(hi2s->hdmarx, HAL_DMA_XFER_ERROR_CB_ID, I2S_DMAError);

        __HAL_I2S_CHX_RX_DMA_ENABLE(hi2s, chx);
        __HAL_I2S_CHX_RX_ENABLE(hi2s, chx);
        /* Enable the Rx DMA Stream/Channel */
        if (HAL_OK != HAL_DMA_Start_IT(hi2s->hdmarx, (uint32_t)&hi2s->Instance->RXDMA_CHx[chx], (uint32_t)hi2s->pRxBuffPtr, hi2s->RxXferSize))
        {
            /* Update SPI error code */
            SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_DMA);
            hi2s->State &= ~HAL_I2S_STATE_BUSY_DMA_RX;
            __HAL_UNLOCK(hi2s);
            return HAL_ERROR;
        }
        __HAL_UNLOCK(hi2s);
        return HAL_OK;
    }
    else
    {
        __HAL_UNLOCK(hi2s);
        return HAL_BUSY;
    }
#else
    UNUSED(hi2s);
    UNUSED(pData);
    UNUSED(Size);
    return HAL_ERROR;
#endif
}

/**
  * @brief  Pauses the audio DMA Stream/Channel playing from the Media.
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_DMAPause(I2S_HandleTypeDef *hi2s)
{
#if(defined BSP_I2S_TXRX_DMA_ENABLED && BSP_I2S_TXRX_DMA_ENABLED == 1U)
    if(hi2s == NULL)
    {
        return HAL_ERROR;
    }
    I2S_ChannelType chx = hi2s->Init.Channel;
    if(chx >= I2S_CHANNELS)
    {
        return  HAL_ERROR;
    }
    /* Process Locked */
    __HAL_LOCK(hi2s);
    if (hi2s->State == HAL_I2S_STATE_BUSY_TX)
    {
        /* Disable the I2S DMA Tx request */
        __HAL_I2S_CHX_TX_DMA_DISABLE(hi2s, chx);
    }
    else if (hi2s->State == HAL_I2S_STATE_BUSY_RX)
    {
        /* Disable the I2S DMA Rx request */
        __HAL_I2S_CHX_RX_DMA_DISABLE(hi2s, chx);
    }
    else
    {
        /* nothing to do */
    }
    /* Process Unlocked */
    __HAL_UNLOCK(hi2s);
    return HAL_OK;
#else
    UNUSED(hi2s);
    return HAL_ERROR;
#endif
}

/**
  * @brief  Resumes the audio DMA Stream/Channel playing from the Media.
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_DMAResume(I2S_HandleTypeDef *hi2s)
{
#if(defined BSP_I2S_TXRX_DMA_ENABLED && BSP_I2S_TXRX_DMA_ENABLED == 1U)
    if((hi2s == NULL))
    {
        return HAL_ERROR;
    }
    I2S_ChannelType chx = hi2s->Init.Channel;
    if(chx >= I2S_CHANNELS)
    {
        return  HAL_ERROR;
    }
    /* Process Locked */
    __HAL_LOCK(hi2s);
    if (hi2s->State == HAL_I2S_STATE_BUSY_TX)
    {
        /* Enable the I2S DMA Tx request */
        __HAL_I2S_CHX_TX_DMA_ENABLE(hi2s, chx);
    }
    else if (hi2s->State == HAL_I2S_STATE_BUSY_RX)
    {
        /* Enable the I2S DMA Rx request */
        __HAL_I2S_CHX_RX_DMA_ENABLE(hi2s, chx);
    }
    else
    {
        /* nothing to do */
    }
    /* Enable I2S peripheral */
    __HAL_I2S_ENABLE(hi2s);
    /* Process Unlocked */
    __HAL_UNLOCK(hi2s);
    return HAL_OK;
#else
    UNUSED(hi2s);
    return HAL_ERROR;
#endif
}

/**
  * @brief  Stops the audio DMA Stream/Channel playing from the Media.
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2S_DMAStop(I2S_HandleTypeDef *hi2s)
{
#if(defined BSP_I2S_TXRX_DMA_ENABLED && BSP_I2S_TXRX_DMA_ENABLED == 1U)
    if(hi2s == NULL)
    {
        return HAL_ERROR;
    }
    I2S_ChannelType chx = hi2s->Init.Channel;
    if(chx >= I2S_CHANNELS)
    {
        return  HAL_ERROR;
    }
    HAL_StatusTypeDef errorcode = HAL_OK;
    /* The Lock is not implemented on this API to allow the user application
         to call the HAL SPI API under callbacks HAL_I2S_TxCpltCallback() or HAL_I2S_RxCpltCallback()
         when calling HAL_DMA_Abort() API the DMA TX or RX Transfer complete interrupt is generated
         and the correspond call back is executed HAL_I2S_TxCpltCallback() or HAL_I2S_RxCpltCallback()
         */
    /* Disable the I2S Tx/Rx DMA requests */
    __HAL_I2S_CHX_RX_DMA_DISABLE(hi2s, chx);
    __HAL_I2S_CHX_TX_DMA_DISABLE(hi2s, chx);
    /* Abort the I2S DMA tx Stream/Channel */
    if (hi2s->hdmatx != NULL)
    {
        /* Disable the I2S DMA tx Stream/Channel */
        if (HAL_OK != HAL_DMA_Abort(hi2s->hdmatx))
        {
            SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_DMA);
            errorcode = HAL_ERROR;
        }
    }
    /* Abort the I2S DMA rx Stream/Channel */
    if (hi2s->hdmarx != NULL)
    {
        /* Disable the I2S DMA rx Stream/Channel */
        if (HAL_OK != HAL_DMA_Abort(hi2s->hdmarx))
        {
            SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_DMA);
            errorcode = HAL_ERROR;
        }
    }
    __HAL_I2S_CHX_TX_ENABLE(hi2s, chx);
    hi2s->State = HAL_I2S_STATE_READY;
    return errorcode;
#else
    UNUSED(hi2s);
    return HAL_ERROR;
#endif
}

/**
  * @brief  This function handles I2S interrupt request.
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
void HAL_I2S_IRQHandler(I2S_HandleTypeDef *hi2s)
{
    I2S_ChannelType chx = hi2s->Init.Channel;
    if(chx >= I2S_CHANNELS)
    {
        return;
    }
    __IO uint32_t i2ssr;
    i2ssr = hi2s->Instance->Chx[chx].ISR;
    /* Clear Overrun flag */
    __HAL_I2S_CHX_RX_CLEAR_OVRFLAG(hi2s, chx);
    __HAL_I2S_CHX_TX_CLEAR_OVRFLAG(hi2s, chx);
    /* I2S in mode Receiver ------------------------------------------------*/
    if (((i2ssr & I2S_FLAG_RXDA) == I2S_FLAG_RXDA) && (__HAL_I2S_CHX_GET_IT_SOURCE(hi2s, chx, I2S_IT_RXDAM) != RESET))
    {
        I2S_Receive_IT(hi2s, chx);
    }
    /* I2S Overrun error interrupt occurred -------------------------------------*/
    if (((i2ssr & I2S_FLAG_RXFO) == I2S_FLAG_RXFO) && (__HAL_I2S_CHX_GET_IT_SOURCE(hi2s, chx, I2S_FLAG_RXFO) != RESET))
    {
        /* Disable RXNE and ERR interrupt */
//        __HAL_I2S_CHX_DISABLE_IT(hi2s, chx, (I2S_IT_RXFOM | I2S_IT_RXDAM));
        /* Set the I2S State ready */
//        hi2s->State = HAL_I2S_STATE_READY;
        /* Set the error code and execute error callback*/
//        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_OVR);
        /* Call user error callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
        hi2s->ErrorCallback(hi2s);
#else
        HAL_I2S_ErrorCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
    }
    /* I2S in mode Transmitter -----------------------------------------------*/
    if (((i2ssr & I2S_FLAG_TXFE) == I2S_FLAG_TXFE) && (__HAL_I2S_CHX_GET_IT_SOURCE(hi2s, chx, I2S_IT_TXFEM) != RESET))
    {
        I2S_Transmit_IT(hi2s, chx);
    }
    /* I2S Underrun error interrupt occurred --------------------------------*/
    if (((i2ssr & I2S_FLAG_TXFO) == I2S_FLAG_TXFO) && (__HAL_I2S_CHX_GET_IT_SOURCE(hi2s, chx, I2S_IT_TXFOM) != RESET))
    {
        /* Disable TXE and ERR interrupt */
        __HAL_I2S_CHX_DISABLE_IT(hi2s, chx, (I2S_IT_TXFOM | I2S_IT_TXFEM));
        /* Set the I2S State ready */
        hi2s->State = HAL_I2S_STATE_READY;
        /* Set the error code and execute error callback*/
        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_UDR);
        /* Call user error callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
        hi2s->ErrorCallback(hi2s);
#else
        HAL_I2S_ErrorCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
    }
}

/**
  * @brief  Tx Transfer Half completed callbacks
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__WEAK void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2s);

    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_TxHalfCpltCallback could be implemented in the user file
    */
}

/**
  * @brief  Tx Transfer completed callbacks
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__WEAK void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2s);

    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_TxCpltCallback could be implemented in the user file
    */
}

/**
  * @brief  Rx Transfer half completed callbacks
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__WEAK void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2s);

    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_RxHalfCpltCallback could be implemented in the user file
    */
}

/**
  * @brief  Rx Transfer completed callbacks
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__WEAK void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2s);

    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_RxCpltCallback could be implemented in the user file
    */
}

/**
  * @brief  I2S error callbacks
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
__WEAK void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2s);

    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_ErrorCallback could be implemented in the user file
    */
}
/** @defgroup I2S_Exported_Functions_Group3 Peripheral State and Errors functions
  *  @brief   Peripheral State functions
  *
@verbatim
 ===============================================================================
                      ##### Peripheral State and Errors functions #####
 ===============================================================================
    [..]
    This subsection permits to get in run-time the status of the peripheral
    and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  Return the I2S state
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval HAL state
  */
HAL_I2S_StateTypeDef HAL_I2S_GetState(I2S_HandleTypeDef *hi2s)
{
    if(hi2s == NULL)
    {
        return HAL_I2S_STATE_ERROR;
    }
    return hi2s->State;
}

/**
  * @brief  Return the I2S error code
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval I2S Error Code
  */
uint32_t HAL_I2S_GetError(I2S_HandleTypeDef *hi2s)
{
    if(hi2s == NULL)
    {
        return 0xFFFFFFFFU;
    }
    return hi2s->ErrorCode;
}

/** @addtogroup I2S_Private_Functions I2S Private Functions
  * @{
  */
#if(defined BSP_I2S_TXRX_DMA_ENABLED && BSP_I2S_TXRX_DMA_ENABLED == 1U)
/**
  * @brief  DMA I2S transmit process complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void I2S_DMATxCplt(DMA_HandleTypeDef *hdma)
{
    I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */

    __HAL_I2S_CHX_TX_DMA_DISABLE(hi2s, hi2s->Init.Channel);
    hi2s->TxXferCount = 0U;
    hi2s->State &= ~HAL_I2S_STATE_BUSY_DMA_TX;
    /* Call user Tx complete callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
    hi2s->TxCpltCallback(hi2s);
#else
    HAL_I2S_TxCpltCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}
/**
  * @brief  DMA I2S transmit process half complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void I2S_DMATxHalfCplt(DMA_HandleTypeDef *hdma)
{
    I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */

    /* Call user Tx half complete callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
    hi2s->TxHalfCpltCallback(hi2s);
#else
    HAL_I2S_TxHalfCpltCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA I2S receive process complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void I2S_DMARxCplt(DMA_HandleTypeDef *hdma)
{
    I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */
    /* Disable Rx DMA Request */
    __HAL_I2S_CHX_RX_DMA_DISABLE(hi2s, hi2s->Init.Channel);
    hi2s->RxXferCount = 0U;
    hi2s->State &= ~HAL_I2S_STATE_BUSY_DMA_RX;
    /* Call user Rx complete callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
    hi2s->RxCpltCallback(hi2s);
#else
    HAL_I2S_RxCpltCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA I2S receive process half complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void I2S_DMARxHalfCplt(DMA_HandleTypeDef *hdma)
{
    I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */
    /* Call user Rx half complete callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
    hi2s->RxHalfCpltCallback(hi2s);
#else
    HAL_I2S_RxHalfCpltCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA I2S communication error callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void I2S_DMAError(DMA_HandleTypeDef *hdma)
{
    I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */
    /* Disable Rx and Tx DMA Request */
    if(hi2s->State | HAL_I2S_STATE_BUSY_TX)
    {
        hi2s->TxXferCount = 0U;
        hi2s->State &= ~HAL_I2S_STATE_BUSY_TX;
        __HAL_I2S_CHX_TX_DMA_DISABLE(hi2s, hi2s->Init.Channel);
    }
    if(hi2s->State | HAL_I2S_STATE_BUSY_RX)
    {
        hi2s->RxXferCount = 0U;
        hi2s->State &= ~HAL_I2S_STATE_BUSY_RX;
        __HAL_I2S_CHX_RX_DMA_DISABLE(hi2s, hi2s->Init.Channel);
    }
    /* Set the error code and execute error callback*/
    SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_DMA);
    /* Call user error callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
    hi2s->ErrorCallback(hi2s);
#else
    HAL_I2S_ErrorCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
}
#endif
/**
  * @brief  Transmit an amount of data in non-blocking mode with Interrupt
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
static void I2S_Transmit_IT(I2S_HandleTypeDef *hi2s, I2S_ChannelType chx)
{
    /* Transmit data */
    for(int i = 0; i < (16 - hi2s->Init.TxTriggerLevel); i++)
    {
        hi2s->Instance->Chx[chx].LTHR = ReadUint32_LittleEndian(hi2s->pTxBuffPtr);
        hi2s->pTxBuffPtr += hi2s->TX_Word_size;
        hi2s->Instance->Chx[chx].RTHR = ReadUint32_LittleEndian(hi2s->pTxBuffPtr);
        hi2s->pTxBuffPtr += hi2s->TX_Word_size;
        hi2s->TxXferCount--;
        if (hi2s->TxXferCount == 0U)
        {
//            while(!__HAL_I2S_CHX_GET_FLAG(hi2s, chx, I2S_FLAG_TXFE));
//            if(hi2s->Init.Mode == I2S_MODE_MASTER)
//            {
//                __HAL_I2S_CLK_DISABLE(hi2s);
//            }
//            __HAL_I2S_CHX_TX_DISABLE(hi2s, chx);
            /* Disable TXE and ERR interrupt */
            __HAL_I2S_CHX_DISABLE_IT(hi2s, chx, I2S_IT_TXFEM);
            hi2s->State &= ~HAL_I2S_STATE_BUSY_TX;
            /* Call user Tx complete callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
            hi2s->TxCpltCallback(hi2s);
#else
            HAL_I2S_TxCpltCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
            break;
        }
    }
    __HAL_I2S_CHX_TX_CLEAR_OVRFLAG(hi2s, chx);
}

/**
  * @brief  Receive an amount of data in non-blocking mode with Interrupt
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @retval None
  */
static void I2S_Receive_IT(I2S_HandleTypeDef *hi2s, I2S_ChannelType chx)
{
    /* Receive data */
    while(__HAL_I2S_CHX_GET_FLAG(hi2s, chx, I2S_FLAG_RXDA))
    {
        if(hi2s->RX_Word_size == 2)
        {
            WriteUint16_LittleEndian(hi2s->pRxBuffPtr, hi2s->Instance->Chx[chx].LRBR & 0x0000FFFF);
            hi2s->pRxBuffPtr += hi2s->RX_Word_size;
            WriteUint16_LittleEndian(hi2s->pRxBuffPtr, hi2s->Instance->Chx[chx].RRBR & 0x0000FFFF);
            hi2s->pRxBuffPtr += hi2s->RX_Word_size;
        }
        else
        {
            WriteUint32_LittleEndian(hi2s->pRxBuffPtr, hi2s->Instance->Chx[chx].LRBR);
            hi2s->pRxBuffPtr += hi2s->RX_Word_size;
            WriteUint32_LittleEndian(hi2s->pRxBuffPtr, hi2s->Instance->Chx[chx].RRBR);
            hi2s->pRxBuffPtr += hi2s->RX_Word_size;
        }
        hi2s->RxXferCount--;
        if (hi2s->RxXferCount == 0U)
        {
            /* Disable RXNE and ERR interrupt */
            __HAL_I2S_CHX_DISABLE_IT(hi2s, chx, (I2S_IT_RXFOM | I2S_IT_RXDAM));
//            if(hi2s->Init.Mode == I2S_MODE_MASTER)
//            {
//                __HAL_I2S_CLK_DISABLE(hi2s);
//            }
            hi2s->State &= ~HAL_I2S_STATE_BUSY_RX;
            /* Call user Rx complete callback */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
            hi2s->RxCpltCallback(hi2s);
#else
            HAL_I2S_RxCpltCallback(hi2s);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
            break;
        }
    }
}

/**
  * @brief  This function handles I2S Communication Timeout.
  * @param  hi2s pointer to a I2S_HandleTypeDef structure that contains
  *         the configuration information for I2S module
  * @param  Flag Flag checked
  * @param  State Value of the flag expected
  * @param  Timeout Duration of the timeout
  * @retval HAL status
  */
static HAL_StatusTypeDef I2S_WaitFlagStateUntilTimeout(I2S_HandleTypeDef *hi2s, I2S_ChannelType chx, uint32_t Flag, FlagStatus State, uint32_t Timeout)
{
    uint64_t tickstart;
    /* Get tick */
    tickstart = HAL_GetTick();
    /* Wait until flag is set to status*/
    while (((__HAL_I2S_CHX_GET_FLAG(hi2s, chx, Flag)) ? SET : RESET) != State)
    {
        if (Timeout != HAL_MAX_DELAY)
        {
            if (((HAL_GetTick() - tickstart) >= Timeout) || (Timeout == 0U))
            {
                /* Set the I2S State ready */
                hi2s->State = HAL_I2S_STATE_READY;
                /* Process Unlocked */
                __HAL_UNLOCK(hi2s);
                return HAL_TIMEOUT;
            }
        }
    }
    return HAL_OK;
}

#if(0)
static void I2S_Get_Div(void)
{
   float div0[4] = { 7.0, 13.0, 16.0, 17.0 };
   float div1[8] = { 1.0, 2.0, 3.0, 5.0, 7.0, 9.0, 11.0, 13.0 };
   float div2[8] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0 };
   float div = 8.0;
   double PLL[4] = {720000000.0, 960000000.0, 1200000000.0, 1440000000.0};
   double CLK = 0.0;
   double temp = 0.0;
   double CLK_Temp[9] = {192000.0, 96000.0, 48000.0, 44100.0, 32000.0, 22050.0, 16000.0, 11025.0, 8000.0};
   double limit = 8000.0;
   for (int n = 0; n < 9; n++)
   {
       for (int m = 0; m < 4; m++)
       {
           for (int i = 0; i < 4; i++)
           {
               for (int j = 0; j < 8; j++)
               {
                   for (int k = 0; k < 8; k++)
                   {
                       CLK = PLL[m] / div / div0[i] / div1[j] / div2[k];
                       if (CLK > CLK_Temp[n])
                       {
                           temp = CLK - CLK_Temp[n];
                       }
                       else
                       {
                           temp = CLK_Temp[n] - CLK;
                       }
                       if (temp <= limit)
                       {
                           //printf("PLL:%d\t,div0:%2d\t,div1:%2d\t,div2:%2d\t,CLK:%f\t,I2S±ê×¼CLK:%f\t,Îó²îÉÏÏÞ:%d\n", (int)PLL[m], (int)div0[i], (int)div1[j], (int)div2[k], CLK, CLK_Temp[n], (int)limit);
                       }
                   }
               }
           }
       }
   }
}
#endif

#endif /* HAL_I2S_MODULE_ENABLED */

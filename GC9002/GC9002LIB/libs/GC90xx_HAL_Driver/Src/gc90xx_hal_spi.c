/*
 * Copyright (c) 2022-2050, GraphichinaSX Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-06     mazy         V1.0.1
 */
#include <gc90xx_hal.h>

/** @defgroup SPI SPI
  * @brief SPI HAL module driver
  * @{
  */
#ifdef HAL_SPI_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/** @defgroup SPI_Private_Constants SPI Private Constants
  * @{
  */
#define SPI_DEFAULT_TIMEOUT 100U
#define SPI_BSY_FLAG_WORKAROUND_TIMEOUT 1000U /*!< Timeout 1000 us            */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup SPI_Private_Functions SPI Private Functions
  * @{
  */
static HAL_StatusTypeDef SPI_WaitFlagStateUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, FlagStatus State,
                                                       uint32_t Timeout, uint32_t Tickstart);
static void SPI_TxISR_MasterClock(struct __SPI_HandleTypeDef *hspi);
static void SPI_RxISR_ClearFIFO(struct __SPI_HandleTypeDef *hspi);
static void SPI_TxISR_8BIT(struct __SPI_HandleTypeDef *hspi);
static void SPI_TxISR_16BIT(struct __SPI_HandleTypeDef *hspi);
static void SPI_RxISR_8BIT(struct __SPI_HandleTypeDef *hspi);
static void SPI_RxISR_16BIT(struct __SPI_HandleTypeDef *hspi);
static void SPI_AbortRx_ISR(SPI_HandleTypeDef *hspi);
static void SPI_AbortTx_ISR(SPI_HandleTypeDef *hspi);
static void SPI_CloseRxTx_ISR(SPI_HandleTypeDef *hspi);
static void SPI_CloseRx_ISR(SPI_HandleTypeDef *hspi);
static void SPI_CloseTx_ISR(SPI_HandleTypeDef *hspi);
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup SPI_Exported_Functions SPI Exported Functions
  * @{
  */

/** @defgroup SPI_Exported_Functions_Group1 Initialization and de-initialization functions
 *  @brief    Initialization and Configuration functions
  * @{
  */

/**
  * @brief  Fills each SPI_InitTypeDef member with its default value.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module.
  * @retval None
  */
HAL_StatusTypeDef HAL_SPI_StructInit(SPI_HandleTypeDef *hspi)
{
    /* SPI_InitTypeDef members default value */
    hspi->Init.Mode = SPI_MODE_MASTER | SPI_MODE_NORMAL;
    hspi->Init.DataSize = SPI_DATASIZE_8BIT;
    hspi->Init.FrameFormat = SPI_FRF_MOTOROLA;
    hspi->Init.SPO = SPI_SPO_HIGH;
    hspi->Init.SPH = SPI_SPH_2EDGE;
    //baudrate = Fspiclk / (CPSDVR * (1 + SCR))
    hspi->Init.CPSDVR = 0x10; // Clock predivision coefficient Value range 2-254, only Even number
    hspi->Init.SCR = 7;       // Value range 0-255
    return HAL_OK;
}

/**
  * @brief  Initialize the SPI according to the specified parameters
  *         in the SPI_InitTypeDef and initialize the associated handle.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *         the configuration information for SPI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));
    assert_param(IS_SPI_MODE(hspi->Init.Mode));
    assert_param(IS_SPI_DATASIZE(hspi->Init.DataSize));

#if(defined USE_PERIPHERAL_LOCK && USE_PERIPHERAL_LOCK == 1U)
    __HAL_CHECK_PERI_LOCK((uint32_t)(hspi->Instance));
#endif
    if (hspi->State == HAL_SPI_STATE_RESET)
    {
        __HAL_LOCK(hspi);

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
        /* Init the SPI Callback settings */
        hspi->TxCpltCallback       = HAL_SPI_TxCpltCallback;       /* Legacy weak TxCpltCallback       */
        hspi->RxCpltCallback       = HAL_SPI_RxCpltCallback;       /* Legacy weak RxCpltCallback       */
        hspi->TxRxCpltCallback     = HAL_SPI_TxRxCpltCallback;     /* Legacy weak TxRxCpltCallback     */
        hspi->ErrorCallback        = HAL_SPI_ErrorCallback;        /* Legacy weak ErrorCallback        */
        hspi->AbortCpltCallback    = HAL_SPI_AbortCpltCallback;    /* Legacy weak AbortCpltCallback    */

        if (hspi->MspInitCallback == NULL)
        {
            hspi->MspInitCallback = HAL_SPI_MspInit; /* Legacy weak MspInit  */
        }

        /* Init the low level hardware : GPIO, CLOCK, ECLIC... */
        hspi->MspInitCallback(hspi);
#else
        /* Init the low level hardware : GPIO, CLOCK, ECLIC... */
        HAL_SPI_MspInit(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
    }

    hspi->State = HAL_SPI_STATE_BUSY;
    /* Disable the selected SPI peripheral */
    __HAL_SPI_DISABLE(hspi);
    /* Write to SPI CR0 */
    hspi->Init.SCR &= 0xFF;  //SCR must be 0x00~~0xFF
    hspi->Init.SCR  = (hspi->Init.SCR << SPI_CR0_SCR_Pos) & SPI_CR0_SCR_Msk;
    WRITE_REG(hspi->Instance->CR0, (hspi->Init.SCR | hspi->Init.SPH | hspi->Init.SPO |
                                    hspi->Init.FrameFormat | hspi->Init.DataSize));
    /* Write to SPI CR1 */
    WRITE_REG(hspi->Instance->CR1, hspi->Init.Mode);
    /* Write to SPI CPSR */
    WRITE_REG(hspi->Instance->CPSR, hspi->Init.CPSDVR);
    /* Enable the selected SPI peripheral */
    __HAL_SPI_ENABLE(hspi);

    hspi->ErrorCode = HAL_SPI_ERROR_NONE;
    hspi->State     = HAL_SPI_STATE_READY;
    hspi->RxState   = HAL_SPI_STATE_READY;
    __HAL_UNLOCK(hspi);

#if(defined USE_PERIPHERAL_LOCK && USE_PERIPHERAL_LOCK == 1U)
    __HAL_SET_PERI_LOCK((uint32_t)(hspi->Instance), PERIPHERAL_INITED);
#endif
    return HAL_OK;
}

/**
  * @brief  De-Initialize the SPI peripheral.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_DeInit(SPI_HandleTypeDef *hspi)
{
    /* Check the SPI handle allocation */
    if (hspi == NULL)
    {
        return HAL_ERROR;
    }

    /* Check SPI Instance parameter */
    assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));

    hspi->State = HAL_SPI_STATE_BUSY;

    /* Disable the SPI Peripheral Clock */
    __HAL_SPI_DISABLE(hspi);

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
    if (hspi->MspDeInitCallback == NULL)
    {
        hspi->MspDeInitCallback = HAL_SPI_MspDeInit; /* Legacy weak MspDeInit  */
    }

    /* DeInit the low level hardware: GPIO, CLOCK, NVIC... */
    hspi->MspDeInitCallback(hspi);
#else
    /* DeInit the low level hardware: GPIO, CLOCK, NVIC... */
    HAL_SPI_MspDeInit(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

    hspi->ErrorCode = HAL_SPI_ERROR_NONE;
    hspi->State = HAL_SPI_STATE_RESET;

    /* Release Lock */
    __HAL_UNLOCK(hspi);

    return HAL_OK;
}

/**
  * @brief  Initialize the SPI MSP.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__WEAK void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hspi);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_MspInit should be implemented in the user file
    */
}

/**
  * @brief  De-Initialize the SPI MSP.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__WEAK void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hspi);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_MspDeInit should be implemented in the user file
    */
}

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
/**
  * @brief  DMA SPI transmit process complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void SPI_DMATxCplt(DMA_HandleTypeDef *hdma)
{
    SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */

    hspi->TxXferCount = 0U;
    hspi->State &= ~HAL_SPI_STATE_BUSY_DMA_TX;
    /* Disable Tx DMA Request */
    CLEAR_BIT(hspi->Instance->DMACR, SPI_DMACR_TXEDMACR);
    /* Call user Tx complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
    hspi->TxCpltCallback(hspi);
#else
    HAL_SPI_TxCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA SPI receive process complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void SPI_DMARxCplt(DMA_HandleTypeDef *hdma)
{
    SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */
    hspi->RxXferCount = 0U;
    hspi->RxState &= ~HAL_SPI_STATE_BUSY_DMA_RX;
    /* Disable Rx DMA Request */
    CLEAR_BIT(hspi->Instance->DMACR, SPI_DMACR_RXEDMACR);
    /* Call user Rx complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
    hspi->RxCpltCallback(hspi);
#else
    HAL_SPI_RxCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}

/**
  * @brief  DMA SPI communication error callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
static void SPI_DMAError(DMA_HandleTypeDef *hdma)
{
    SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */
    /* Disable Rx and Tx DMA Request */
    if(hspi->State | HAL_SPI_STATE_BUSY_TX)
    {
        hspi->TxXferCount = 0U;
        hspi->State &= ~HAL_SPI_STATE_BUSY_TX;
        CLEAR_BIT(hspi->Instance->DMACR, SPI_DMACR_TXEDMACR);
    }
    if(hspi->RxState | HAL_SPI_STATE_BUSY_RX)
    {
        hspi->RxXferCount = 0U;
        hspi->RxState &= ~HAL_SPI_STATE_BUSY_RX;
        CLEAR_BIT(hspi->Instance->DMACR, SPI_DMACR_RXEDMACR);
    }
    /* Set the error code and execute error callback*/
    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);
    /* Call user error callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
    hspi->ErrorCallback(hspi);
#else
    HAL_SPI_ErrorCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}
/**
  * @brief  Register a User SPI Callback
  *         To be used instead of the weak predefined callback
  * @param  hspi Pointer to a SPI_HandleTypeDef structure that contains
  *                the configuration information for the specified SPI.
  * @param  CallbackID ID of the callback to be registered
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_RegisterCallback(SPI_HandleTypeDef *hspi, HAL_SPI_CallbackIDTypeDef CallbackID, pSPI_CallbackTypeDef pCallback)
{
    HAL_StatusTypeDef status = HAL_OK;

    if (pCallback == NULL)
    {
        /* Update the error code */
        hspi->ErrorCode |= HAL_SPI_ERROR_INVALID_CALLBACK;

        return HAL_ERROR;
    }
    /* Process locked */
    __HAL_LOCK(hspi);

    if (HAL_SPI_STATE_READY == hspi->State)
    {
        switch (CallbackID)
        {
        case HAL_SPI_TX_COMPLETE_CB_ID :
            hspi->TxCpltCallback = pCallback;
            break;

        case HAL_SPI_RX_COMPLETE_CB_ID :
            hspi->RxCpltCallback = pCallback;
            break;

        case HAL_SPI_TX_RX_COMPLETE_CB_ID :
            hspi->TxRxCpltCallback = pCallback;
            break;

        case HAL_SPI_ERROR_CB_ID :
            hspi->ErrorCallback = pCallback;
            break;

        case HAL_SPI_ABORT_CB_ID :
            hspi->AbortCpltCallback = pCallback;
            break;

        case HAL_SPI_MSPINIT_CB_ID :
            hspi->MspInitCallback = pCallback;
            break;

        case HAL_SPI_MSPDEINIT_CB_ID :
            hspi->MspDeInitCallback = pCallback;
            break;

        default :
            /* Update the error code */
            SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);

            /* Return error status */
            status =  HAL_ERROR;
            break;
        }
    }
    else if (HAL_SPI_STATE_RESET == hspi->State)
    {
        switch (CallbackID)
        {
        case HAL_SPI_MSPINIT_CB_ID :
            hspi->MspInitCallback = pCallback;
            break;

        case HAL_SPI_MSPDEINIT_CB_ID :
            hspi->MspDeInitCallback = pCallback;
            break;

        default :
            /* Update the error code */
            SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);

            /* Return error status */
            status =  HAL_ERROR;
            break;
        }
    }
    else
    {
        /* Update the error code */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);

        /* Return error status */
        status =  HAL_ERROR;
    }

    /* Release Lock */
    __HAL_UNLOCK(hspi);
    return status;
}

/**
  * @brief  Unregister an SPI Callback
  *         SPI callback is redirected to the weak predefined callback
  * @param  hspi Pointer to a SPI_HandleTypeDef structure that contains
  *                the configuration information for the specified SPI.
  * @param  CallbackID ID of the callback to be unregistered
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_UnRegisterCallback(SPI_HandleTypeDef *hspi, HAL_SPI_CallbackIDTypeDef CallbackID)
{
    HAL_StatusTypeDef status = HAL_OK;

    /* Process locked */
    __HAL_LOCK(hspi);

    if (HAL_SPI_STATE_READY == hspi->State)
    {
        switch (CallbackID)
        {
        case HAL_SPI_TX_COMPLETE_CB_ID :
            hspi->TxCpltCallback = HAL_SPI_TxCpltCallback;             /* Legacy weak TxCpltCallback       */
            break;

        case HAL_SPI_RX_COMPLETE_CB_ID :
            hspi->RxCpltCallback = HAL_SPI_RxCpltCallback;             /* Legacy weak RxCpltCallback       */
            break;

        case HAL_SPI_TX_RX_COMPLETE_CB_ID :
            hspi->TxRxCpltCallback = HAL_SPI_TxRxCpltCallback;         /* Legacy weak TxRxCpltCallback     */
            break;

        case HAL_SPI_ERROR_CB_ID :
            hspi->ErrorCallback = HAL_SPI_ErrorCallback;               /* Legacy weak ErrorCallback        */
            break;

        case HAL_SPI_ABORT_CB_ID :
            hspi->AbortCpltCallback = HAL_SPI_AbortCpltCallback;       /* Legacy weak AbortCpltCallback    */
            break;

        case HAL_SPI_MSPINIT_CB_ID :
            hspi->MspInitCallback = HAL_SPI_MspInit;                   /* Legacy weak MspInit              */
            break;

        case HAL_SPI_MSPDEINIT_CB_ID :
            hspi->MspDeInitCallback = HAL_SPI_MspDeInit;               /* Legacy weak MspDeInit            */
            break;

        default :
            /* Update the error code */
            SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);

            /* Return error status */
            status =  HAL_ERROR;
            break;
        }
    }
    else if (HAL_SPI_STATE_RESET == hspi->State)
    {
        switch (CallbackID)
        {
        case HAL_SPI_MSPINIT_CB_ID :
            hspi->MspInitCallback = HAL_SPI_MspInit;                   /* Legacy weak MspInit              */
            break;

        case HAL_SPI_MSPDEINIT_CB_ID :
            hspi->MspDeInitCallback = HAL_SPI_MspDeInit;               /* Legacy weak MspDeInit            */
            break;

        default :
            /* Update the error code */
            SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);

            /* Return error status */
            status =  HAL_ERROR;
            break;
        }
    }
    else
    {
        /* Update the error code */
        SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_INVALID_CALLBACK);

        /* Return error status */
        status =  HAL_ERROR;
    }

    /* Release Lock */
    __HAL_UNLOCK(hspi);
    return status;
}
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup SPI_Exported_Functions_Group2 IO operation functions
 *  @brief   Data transfers functions
  * @{
  */

__HAL_SPI_DRIVER_ATTRIBUTE static void SPI_ClearFIFO(SPI_HandleTypeDef *hspi)
{
    uint16_t tempreg = 0;
    for (uint8_t i = 0U; i < HAL_SPI_FIFO_DEEP; i++)
    {
        if((hspi->Instance->SR & SPI_FLAG_RXNE) == SPI_FLAG_RXNE)
        {
            tempreg = (hspi->Instance->DR & 0xFFFF);
        } else {
            break;
        }
    }
}
/**
  * @brief  Transmit an amount of data in blocking mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, const uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    return HAL_SPI_TransmitReceive(hspi, pData, NULL, Size, Timeout);
}

/**
  * @brief  Receive an amount of data in blocking mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be received
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    return HAL_SPI_TransmitReceive(hspi, NULL, pData, Size, Timeout);
}

/**
  * @brief  Transmit and Receive an amount of data in blocking mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pTxData pointer to transmission data buffer
  * @param  pRxData pointer to reception data buffer
  * @param  Size amount of data to be sent and received
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, const uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
{
    uint16_t             initial_TxXferCount;
    uint16_t             tempreg;
    uint32_t             tmp_mode;
    uint32_t             tickstart;
    HAL_StatusTypeDef    errorcode = HAL_OK;

    int32_t timeOutCnt = 10000;
    /* Check the parameters */
    assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));

    /* Process Locked */
    __HAL_LOCK(hspi);

    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();

    /* Init temporary variables */
    tmp_mode            = hspi->Init.Mode;
    initial_TxXferCount = Size;

    if (HAL_SPI_STATE_READY != hspi->State || \
        HAL_SPI_STATE_READY != hspi->RxState)
    {
        errorcode = HAL_BUSY;
        goto error;
    }

    if (((pTxData == NULL) && (pRxData == NULL)) || (Size == 0U))
    {
        errorcode = HAL_ERROR;
        goto error;
    }

    /*must wait send and recv end before restart next data transmit*/
    while((hspi->Instance->SR & SPI_FLAG_TXFE) == RESET)
    {
        if(timeOutCnt-- <= 0)
        {
            errorcode = HAL_TIMEOUT;
            goto __timeout;
        }
    }

    SPI_ClearFIFO(hspi);
    /* Set the transaction information */
    hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
    /*Init field not used in handle to zero */
    hspi->RxISR       = NULL;
    hspi->TxISR       = NULL;
    hspi->pTxBuffPtr  = (uint8_t *)pTxData;
    if(hspi->pTxBuffPtr)
    {
        hspi->State |= HAL_SPI_STATE_BUSY_TX;
        hspi->TxXferSize  = Size;
        hspi->TxXferCount = (hspi->Init.DataSize <= SPI_DATASIZE_8BIT ? Size : Size/2);
    } else if((hspi->Init.Mode & SPI_CR1_SLAVE_Msk) == SPI_MODE_MASTER) {
        hspi->State |= HAL_SPI_STATE_BUSY_TX;
        hspi->TxXferSize  = 0;
        hspi->TxXferCount = (hspi->Init.DataSize <= SPI_DATASIZE_8BIT ? Size : Size/2);
    } else {
        hspi->TxXferSize  = 0;
        hspi->TxXferCount = 0;
    }
    hspi->pRxBuffPtr  = (uint8_t *)pRxData;
    if(hspi->pRxBuffPtr)
    {
        hspi->RxState |= HAL_SPI_STATE_BUSY_RX;
        hspi->RxXferSize  = Size;
        hspi->RxXferCount = (hspi->Init.DataSize <= SPI_DATASIZE_8BIT ? Size : Size/2);
    } else {
        hspi->RxXferSize  = 0;
        hspi->RxXferCount = (hspi->Init.DataSize <= SPI_DATASIZE_8BIT ? Size : Size/2);
    }

    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);
    while ((hspi->TxXferCount > 0U) || (hspi->RxXferCount > 0U))
    {
        tickstart = HAL_GetTick();
        /* Check TXFE flag */
        if (hspi->TxXferCount > 0U && __HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXNF))
        {
            if(hspi->pTxBuffPtr != NULL)
            {
                hspi->Instance->DR = ReadUint16_LittleEndian(hspi->pTxBuffPtr);
                hspi->pTxBuffPtr += ((hspi->Init.DataSize <= SPI_DATASIZE_8BIT) ? 1 : 2);
            } else {
                hspi->Instance->DR = SPI_DUMMY_BYTE;
            }
            hspi->TxXferCount--;
        }
        /* Check RXNE flag */
        if ((hspi->RxXferCount > 0U) && (__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE)))
        {
            tempreg = hspi->Instance->DR & 0xFFFF;
            hspi->RxXferCount--;
            if(hspi->pRxBuffPtr)
            {
                if(hspi->Init.DataSize <= SPI_DATASIZE_8BIT)
                {
                    *hspi->pRxBuffPtr = tempreg & 0xFF;
                    hspi->pRxBuffPtr ++;
                } else {
                    WriteUint16_LittleEndian(hspi->pRxBuffPtr, tempreg);
                    hspi->pRxBuffPtr += 2;
                }
            }
        }
        if ((Timeout != HAL_MAX_DELAY && (HAL_GetTick() - tickstart) >=  Timeout) || (Timeout == 0U))
        {
            errorcode = HAL_TIMEOUT;
            goto __timeout;
        }
    }
    SPI_ClearFIFO(hspi);
__timeout:
    hspi->State   &= ~HAL_SPI_STATE_BUSY_TX;
    hspi->RxState &= ~HAL_SPI_STATE_BUSY_RX;
error :
    __HAL_UNLOCK(hspi);
    return errorcode;
}

/**
  * @brief  Transmit an amount of data in non-blocking mode with Interrupt.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, const uint8_t *pData, uint16_t Size)
{
    return HAL_SPI_TransmitReceive_IT(hspi, pData, NULL, Size);
}

/**
  * @brief  Receive an amount of data in non-blocking mode with Interrupt.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Receive_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
{
    return HAL_SPI_TransmitReceive_IT(hspi, NULL, pData, Size);
}

/**
  * @brief  Transmit and Receive an amount of data in non-blocking mode with Interrupt.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @param  pTxData pointer to transmission data buffer
  * @param  pRxData pointer to reception data buffer
  * @param  Size amount of data to be sent and received
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_TransmitReceive_IT(SPI_HandleTypeDef *hspi, const uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
    uint32_t             tmp_mode;
    HAL_SPI_StateTypeDef tmp_state;
    HAL_SPI_StateTypeDef tmp_RxState;
    HAL_StatusTypeDef    errorcode = HAL_OK;
    int32_t timeOutCnt = 10000;
    /* Check the parameters */
    assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));

    /* Process locked */
    __HAL_LOCK(hspi);

    /* Init temporary variables */
    tmp_state           = hspi->State;
    tmp_RxState         = hspi->RxState;
    tmp_mode            = hspi->Init.Mode;

    if (HAL_SPI_STATE_READY != tmp_state && \
        HAL_SPI_STATE_READY != tmp_RxState)
    {
        errorcode = HAL_BUSY;
        goto error;
    }

    if ((pTxData == NULL && pRxData == NULL) || (Size == 0U))
    {
        errorcode = HAL_ERROR;
        goto error;
    }

    /*must wait send and recv end before restart next data transmit*/
    while((hspi->Instance->SR & SPI_FLAG_TXFE) == RESET)
    {
        if(timeOutCnt-- <= 0)
        {
            errorcode = HAL_TIMEOUT;
            goto __timeout;
        }
    }

    SPI_ClearFIFO(hspi);
    /* Disable SPI peripheral */
    __HAL_SPI_DISABLE(hspi);

    /* Set the transaction information */
    hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
    /*----TX-----------------------------------------------------------------------------------------*/
    hspi->pTxBuffPtr  = (uint8_t *)pTxData;
    hspi->TxXferSize  = (pTxData == NULL ? 0 : Size);
    hspi->TxXferCount = (hspi->Init.DataSize <= SPI_DATASIZE_8BIT ? Size : Size/2);
    if(hspi->pTxBuffPtr)
    {
        hspi->State |= HAL_SPI_STATE_BUSY_TX;
        hspi->TxISR = (hspi->Init.DataSize > SPI_DATASIZE_8BIT ? SPI_TxISR_16BIT : SPI_TxISR_8BIT);
        __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_TXE));
    } else if((hspi->Init.Mode & SPI_CR1_SLAVE_Msk) == SPI_MODE_MASTER) {
        hspi->State |= HAL_SPI_STATE_BUSY_TX;
        hspi->TxISR = SPI_TxISR_MasterClock;
        __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_TXE));
    } else {
        hspi->TxXferCount = 0;
        hspi->TxISR = NULL;
    }
    /*----RX-----------------------------------------------------------------------------------------*/
    hspi->pRxBuffPtr  = (uint8_t *)pRxData;
    hspi->RxState    |= HAL_SPI_STATE_BUSY_RX;
    hspi->RxXferCount = (hspi->Init.DataSize <= SPI_DATASIZE_8BIT ? Size : Size/2);
    if(hspi->pRxBuffPtr)
    {
        hspi->RxXferSize  = (pRxData == NULL ? 0 : Size);
        hspi->RxISR = (hspi->Init.DataSize > SPI_DATASIZE_8BIT ? SPI_RxISR_16BIT : SPI_RxISR_8BIT);
    } else {
        hspi->RxXferSize = 0;
        hspi->RxISR = SPI_RxISR_ClearFIFO;
    }
    __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_OVERRUN));

    /* Clear interrupt */
    __HAL_SPI_SET_ICR(hspi);
    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);
__timeout:
error :
    /* Process Unlocked */
    __HAL_UNLOCK(hspi);
    return errorcode;
}

HAL_StatusTypeDef HAL_SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, const uint8_t *pData, uint16_t Size)
{
    return HAL_SPI_TransmitReceive_DMA(hspi, pData, NULL, Size);
}
HAL_StatusTypeDef HAL_SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
{
    return HAL_SPI_TransmitReceive_DMA(hspi, NULL, pData, Size);
}
HAL_StatusTypeDef HAL_SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi, const uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
    HAL_StatusTypeDef errorcode = HAL_OK;
    int32_t timeOutCnt = 10000;
    /* Check the parameters */
    assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));

    if ((pTxData == NULL && pRxData == NULL) || (Size == 0U))
    {
        errorcode = HAL_ERROR;
        goto error;
    }

    /* Process Locked */
    __HAL_LOCK(hspi);

    if (hspi->State != HAL_SPI_STATE_READY || hspi->RxState != HAL_SPI_STATE_READY)
    {
        errorcode = HAL_BUSY;
        goto error;
    }
    /*must wait send and recv end before restart next data transmit*/
    while((hspi->Instance->SR & SPI_FLAG_TXFE) == RESET)
    {
        if(timeOutCnt-- <= 0)
        {
            errorcode = HAL_TIMEOUT;
            goto __timeout;
        }
    }

    SPI_ClearFIFO(hspi);
    /* Disable SPI peripheral */
    __HAL_SPI_DISABLE(hspi);
    /* Set the transaction information */
    hspi->ErrorCode   = HAL_SPI_ERROR_NONE;
    /*----RX-----------------------------------------------------------------------------------------*/
    hspi->pRxBuffPtr  = (uint8_t *)pRxData;
    hspi->RxXferCount = (hspi->Init.DataSize <= SPI_DATASIZE_8BIT ? Size : Size/2);
    if(hspi->pRxBuffPtr)
    {
        hspi->RxState |= HAL_SPI_STATE_BUSY_DMA_RX;
        hspi->RxXferSize = Size;
        if(hspi->hdmarx != NULL)
        {
            hspi->RxISR = NULL;
            hspi->hdmarx->Parent = hspi;
            HAL_DMA_RegisterCallback(hspi->hdmarx, HAL_DMA_XFER_CPLT_CB_ID, SPI_DMARxCplt);
            HAL_DMA_RegisterCallback(hspi->hdmarx, HAL_DMA_XFER_ERROR_CB_ID, SPI_DMAError);
            /* Enable the Rx DMA Stream/Channel */
            if (HAL_OK != HAL_DMA_Start_IT(hspi->hdmarx, (uint32_t)&(hspi->Instance->DR), (uint32_t)hspi->pRxBuffPtr, hspi->RxXferCount))
            {
                /* Update SPI error code */
                SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);
                errorcode = HAL_ERROR;
                goto __exit;
            }
            /* Enable Rx DMA Request */
            SET_BIT(hspi->Instance->DMACR, SPI_DMACR_RXEDMACR);
        } else {
            hspi->RxISR = (hspi->Init.DataSize > SPI_DATASIZE_8BIT ? SPI_RxISR_16BIT : SPI_RxISR_8BIT);
            __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_OVERRUN));
        }
    }
    else {
        hspi->RxXferSize = 0;
        hspi->RxISR = SPI_RxISR_ClearFIFO;
        __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_OVERRUN));
    }
    /*----TX-----------------------------------------------------------------------------------------*/
    hspi->pTxBuffPtr  = (uint8_t *)pTxData;
    hspi->TxXferSize = Size;
    hspi->TxXferCount = (hspi->Init.DataSize <= SPI_DATASIZE_8BIT ? Size : Size/2);
    if(hspi->pTxBuffPtr)
    {
        hspi->State |= HAL_SPI_STATE_BUSY_DMA_TX;
        if(hspi->hdmatx)
        {
            hspi->TxISR = NULL;
            hspi->hdmatx->Parent = hspi;
            HAL_DMA_RegisterCallback(hspi->hdmatx, HAL_DMA_XFER_CPLT_CB_ID, SPI_DMATxCplt);
            HAL_DMA_RegisterCallback(hspi->hdmatx, HAL_DMA_XFER_ERROR_CB_ID, SPI_DMAError);
            /* Enable the Tx DMA Stream/Channel */
            if (HAL_OK != HAL_DMA_Start_IT(hspi->hdmatx, (uint32_t)hspi->pTxBuffPtr, (uint32_t)&(hspi->Instance->DR), hspi->TxXferCount))
            {
                /* Update SPI error code */
                SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_DMA);
                errorcode = HAL_ERROR;
                goto __exit;
            }
            /* Enable Tx DMA Request */
            SET_BIT(hspi->Instance->DMACR, SPI_DMACR_TXEDMACR);
        } else {
            hspi->TxISR = (hspi->Init.DataSize > SPI_DATASIZE_8BIT ? SPI_TxISR_16BIT : SPI_TxISR_8BIT);
            __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_TXE));
        }
    } else if((hspi->Init.Mode & SPI_CR1_SLAVE_Msk) == SPI_MODE_MASTER) {
        hspi->State |= HAL_SPI_STATE_BUSY_TX;
        hspi->TxISR = SPI_TxISR_MasterClock;
        __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_TXE));
    } else {
        hspi->TxXferSize = 0;
        hspi->TxXferCount = 0;
        hspi->TxISR = NULL;
    }
    __HAL_UNLOCK(hspi);
    /* Clear interrupt */
    __HAL_SPI_SET_ICR(hspi);
    /* Enable SPI peripheral */
    __HAL_SPI_ENABLE(hspi);
    return errorcode;
__exit:
    hspi->State   &= ~HAL_SPI_STATE_BUSY_DMA_TX;
    hspi->RxState &= ~HAL_SPI_STATE_BUSY_DMA_RX;
__timeout:
error :
    __HAL_UNLOCK(hspi);
    return errorcode;
}
/**
  * @brief  Abort ongoing transfer (blocking mode).
  * @param  hspi SPI handle.
  * @note   This procedure could be used for aborting any ongoing transfer (Tx and Rx),
  *         started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable SPI Interrupts (depending of transfer direction)
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort (in case of transfer in DMA mode)
  *           - Set handle State to READY
  * @note   This procedure is executed in blocking mode : when exiting function, Abort is considered as completed.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Abort(SPI_HandleTypeDef *hspi)
{
    HAL_StatusTypeDef errorcode;
    __IO uint32_t count;
    __IO uint32_t resetcount;
    /* Check the parameters */
    assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));

    /* Initialized local variable  */
    errorcode = HAL_OK;
    resetcount = SPI_DEFAULT_TIMEOUT * (GetSystemCoreClock() / 24U / 1000U);
    count = resetcount;

    /* Clear ERRIE interrupt to avoid error interrupts generation during Abort procedure */
    CLEAR_BIT(hspi->Instance->IMSC, SPI_IT_ERR);

    /* Disable TXE, RXNE and ERR(mode fault event, overrun error, TI frame error) interrupts */
    if (HAL_IS_BIT_SET(hspi->Instance->IMSC, SPI_IT_TXE))
    {
        hspi->TxISR = SPI_AbortTx_ISR;
        /* Wait HAL_SPI_STATE_ABORT state */
        do
        {
            if (count == 0U)
            {
                SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
                break;
            }
            count--;
        } while (hspi->State != HAL_SPI_STATE_ABORT);
        /* Reset Timeout Counter */
        count = resetcount;
    }

    if (HAL_IS_BIT_SET(hspi->Instance->IMSC, SPI_IT_RXNE))
    {
        hspi->RxISR = SPI_AbortRx_ISR;
        /* Wait HAL_SPI_STATE_ABORT state */
        do
        {
            if (count == 0U)
            {
                SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
                break;
            }
            count--;
        } while (hspi->State != HAL_SPI_STATE_ABORT);
        /* Reset Timeout Counter */
        count = resetcount;
    }

    /* Disable the SPI DMA Tx request if enabled */
    if (HAL_IS_BIT_SET(hspi->Instance->DMACR, SPI_DMACR_TXEDMACR))
    {
        /* Abort the SPI DMA Tx Stream/Channel : use blocking DMA Abort API (no callback) */
        if (hspi->hdmatx != NULL)
        {
            /* Set the SPI DMA Abort callback :
            will lead to call HAL_SPI_AbortCpltCallback() at end of DMA abort procedure */
            hspi->hdmatx->XferAbortCallback = NULL;

            /* Abort DMA Tx Handle linked to SPI Peripheral */
            if (HAL_DMA_Abort(hspi->hdmatx) != HAL_OK)
            {
                hspi->ErrorCode = HAL_SPI_ERROR_ABORT;
            }

            /* Disable Tx DMA Request */
            CLEAR_BIT(hspi->Instance->DMACR, SPI_DMACR_TXEDMACR);

            /* Wait until TXE flag is set */
            do
            {
                if (count == 0U)
                {
                    SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
                    break;
                }
                count--;
            } while ((hspi->Instance->SR & SPI_FLAG_TXFE) == RESET);
        }
    }

    /* Disable the SPI DMA Rx request if enabled */
    if (HAL_IS_BIT_SET(hspi->Instance->DMACR, SPI_DMACR_RXEDMACR))
    {
        /* Abort the SPI DMA Rx Stream/Channel : use blocking DMA Abort API (no callback) */
        if (hspi->hdmarx != NULL)
        {
            /* Set the SPI DMA Abort callback :
            will lead to call HAL_SPI_AbortCpltCallback() at end of DMA abort procedure */
            hspi->hdmarx->XferAbortCallback = NULL;

            /* Abort DMA Rx Handle linked to SPI Peripheral */
            if (HAL_DMA_Abort(hspi->hdmarx) != HAL_OK)
            {
                hspi->ErrorCode = HAL_SPI_ERROR_ABORT;
            }

            /* Disable Rx DMA Request */
            CLEAR_BIT(hspi->Instance->DMACR, SPI_DMACR_RXEDMACR);
        }
    }
    /* Reset Tx and Rx transfer counters */
    hspi->RxXferCount = 0U;
    hspi->TxXferCount = 0U;

    /* Check error during Abort procedure */
    if (hspi->ErrorCode == HAL_SPI_ERROR_ABORT)
    {
        /* return HAL_Error in case of error during Abort procedure */
        errorcode = HAL_ERROR;
    }
    else
    {
        /* Reset errorCode */
        hspi->ErrorCode = HAL_SPI_ERROR_NONE;
    }

    /* Restore hspi->state to ready */
    hspi->State = HAL_SPI_STATE_READY;

    return errorcode;
}

/**
  * @brief  Abort ongoing transfer (Interrupt mode).
  * @param  hspi SPI handle.
  * @note   This procedure could be used for aborting any ongoing transfer (Tx and Rx),
  *         started in Interrupt or DMA mode.
  *         This procedure performs following operations :
  *           - Disable SPI Interrupts (depending of transfer direction)
  *           - Disable the DMA transfer in the peripheral register (if enabled)
  *           - Abort DMA transfer by calling HAL_DMA_Abort_IT (in case of transfer in DMA mode)
  *           - Set handle State to READY
  *           - At abort completion, call user abort complete callback
  * @note   This procedure is executed in Interrupt mode, meaning that abort procedure could be
  *         considered as completed only when user abort complete callback is executed (not when exiting function).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Abort_IT(SPI_HandleTypeDef *hspi)
{
    HAL_StatusTypeDef errorcode;
    uint32_t abortcplt ;
    __IO uint32_t count;
    __IO uint32_t resetcount;
    /* Check the parameters */
    assert_param(IS_SPI_ALL_INSTANCE(hspi->Instance));

    /* Initialized local variable  */
    errorcode = HAL_OK;
    abortcplt = 1U;
    resetcount = SPI_DEFAULT_TIMEOUT * (GetSystemCoreClock() / 24U / 1000U);
    count = resetcount;

    /* Clear ERRIE interrupt to avoid error interrupts generation during Abort procedure */
    CLEAR_BIT(hspi->Instance->IMSC, SPI_IT_ERR);

    /* Change Rx and Tx Irq Handler to Disable TXEIE, RXNEIE and ERRIE interrupts */
    if (HAL_IS_BIT_SET(hspi->Instance->IMSC, SPI_IT_TXE))
    {
        hspi->TxISR = SPI_AbortTx_ISR;
        /* Wait HAL_SPI_STATE_ABORT state */
        do
        {
            if (count == 0U)
            {
                SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
                break;
            }
            count--;
        } while (hspi->State != HAL_SPI_STATE_ABORT);
        /* Reset Timeout Counter */
        count = resetcount;
    }

    if (HAL_IS_BIT_SET(hspi->Instance->IMSC, SPI_IT_RXNE))
    {
        hspi->RxISR = SPI_AbortRx_ISR;
        /* Wait HAL_SPI_STATE_ABORT state */
        do
        {
            if (count == 0U)
            {
                SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
                break;
            }
            count--;
        } while (hspi->State != HAL_SPI_STATE_ABORT);
        /* Reset Timeout Counter */
        count = resetcount;
    }

    /* If DMA Tx and/or DMA Rx Handles are associated to SPI Handle, DMA Abort complete callbacks should be initialised
    before any call to DMA Abort functions */
    /* DMA Tx Handle is valid */
    if (hspi->hdmatx != NULL)
    {
        /* Set DMA Abort Complete callback if SPI DMA Tx request if enabled.
        Otherwise, set it to NULL */
        if (HAL_IS_BIT_SET(hspi->Instance->DMACR, SPI_DMACR_TXEDMACR))
        {
            //hspi->hdmatx->XferAbortCallback = SPI_DMATxAbortCallback;
        }
        else
        {
            hspi->hdmatx->XferAbortCallback = NULL;
        }
    }
    /* DMA Rx Handle is valid */
    if (hspi->hdmarx != NULL)
    {
        /* Set DMA Abort Complete callback if SPI DMA Rx request if enabled.
        Otherwise, set it to NULL */
        if (HAL_IS_BIT_SET(hspi->Instance->DMACR, SPI_DMACR_RXEDMACR))
        {
            //hspi->hdmarx->XferAbortCallback = SPI_DMARxAbortCallback;
        }
        else
        {
            hspi->hdmarx->XferAbortCallback = NULL;
        }
    }

    /* Disable the SPI DMA Tx request if enabled */
    if (HAL_IS_BIT_SET(hspi->Instance->DMACR, SPI_DMACR_TXEDMACR))
    {
        /* Abort the SPI DMA Tx Stream/Channel */
        if (hspi->hdmatx != NULL)
        {
            /* Abort DMA Tx Handle linked to SPI Peripheral */
            if (HAL_DMA_Abort_IT(hspi->hdmatx) != HAL_OK)
            {
                hspi->hdmatx->XferAbortCallback = NULL;
                hspi->ErrorCode = HAL_SPI_ERROR_ABORT;
            }
            else
            {
                abortcplt = 0U;
            }
        }
    }
    /* Disable the SPI DMA Rx request if enabled */
    if (HAL_IS_BIT_SET(hspi->Instance->DMACR, SPI_DMACR_RXEDMACR))
    {
        /* Abort the SPI DMA Rx Stream/Channel */
        if (hspi->hdmarx != NULL)
        {
            /* Abort DMA Rx Handle linked to SPI Peripheral */
            if (HAL_DMA_Abort_IT(hspi->hdmarx) !=  HAL_OK)
            {
                hspi->hdmarx->XferAbortCallback = NULL;
                hspi->ErrorCode = HAL_SPI_ERROR_ABORT;
            }
            else
            {
                abortcplt = 0U;
            }
        }
    }

    if (abortcplt == 1U)
    {
        /* Reset Tx and Rx transfer counters */
        hspi->RxXferCount = 0U;
        hspi->TxXferCount = 0U;

        /* Check error during Abort procedure */
        if (hspi->ErrorCode == HAL_SPI_ERROR_ABORT)
        {
            /* return HAL_Error in case of error during Abort procedure */
            errorcode = HAL_ERROR;
        }
        else
        {
            /* Reset errorCode */
            hspi->ErrorCode = HAL_SPI_ERROR_NONE;
        }

        /* Restore hspi->State to Ready */
        hspi->State = HAL_SPI_STATE_READY;

        /* As no DMA to be aborted, call directly user Abort complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
        hspi->AbortCpltCallback(hspi);
#else
        HAL_SPI_AbortCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
    }

    return errorcode;
}

/**
  * @brief  Handle SPI interrupt request.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for the specified SPI module.
  * @retval None
  */
void HAL_SPI_IRQHandler(SPI_HandleTypeDef *hspi)
{
    uint32_t itsource = hspi->Instance->RIS;
    uint32_t itmis = hspi->Instance->MIS;
    uint32_t imsc = hspi->Instance->IMSC;

    /* SPI in mode Receiver ----------------------------------------------------*/
    if ((SPI_CHECK_MIS(itmis, SPI_MIS_RX) != RESET) && (SPI_CHECK_IT_SOURCE(itsource, SPI_IT_RXNE_FLAG) != RESET))
    {
        if(NULL != hspi->RxISR)
        {
            hspi->RxISR(hspi);
        } else {
            SPI_ClearFIFO(hspi);
        }
        return;
    }

    /* SPI in mode Transmitter -------------------------------------------------*/
    if ((SPI_CHECK_MIS(itmis, SPI_MIS_TX) != RESET) && (SPI_CHECK_IT_SOURCE(itsource, SPI_IT_TXE_FLAG) != RESET))
    {
        if(NULL != hspi->TxISR)
        {
            hspi->TxISR(hspi);
        }
        return;
    }

    if ( ((itmis & (SPI_MIS_RT|SPI_MIS_ROR)) != 0) && ((itsource & (SPI_IT_RXT_FLAG|SPI_IT_RXO_FLAG)) != 0) )
    {
        hspi->State = HAL_SPI_STATE_READY;
        /* Disable all interrupts */
        __HAL_SPI_DISABLE_IT(hspi, SPI_IT_RXNE | SPI_IT_TXE | SPI_IT_ERR);

        /* Call user error callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
        hspi->ErrorCallback(hspi);
#else
        HAL_SPI_ErrorCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
    }
}

/**
  * @brief  Tx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__WEAK void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_TxCpltCallback should be implemented in the user file
   */
}

/**
  * @brief  Rx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__WEAK void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_RxCpltCallback should be implemented in the user file
   */
}

/**
  * @brief  Tx and Rx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__WEAK void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_TxRxCpltCallback should be implemented in the user file
   */
}

/**
  * @brief  SPI error callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__WEAK void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_ErrorCallback should be implemented in the user file
   */
  /* NOTE : The ErrorCode parameter in the hspi handle is updated by the SPI processes
            and user can use HAL_SPI_GetError() API to check the latest error occurred
   */
}

/**
  * @brief  SPI Abort Complete callback.
  * @param  hspi SPI handle.
  * @retval None
  */
__WEAK void HAL_SPI_AbortCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_AbortCpltCallback can be implemented in the user file.
   */
}

/**
  * @}
  */

/** @defgroup SPI_Exported_Functions_Group3 Peripheral State and Errors functions
  * @brief   SPI control functions
  * @{
  */

/**
  * @brief  Return the SPI handle state.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval SPI state
  */
HAL_SPI_StateTypeDef HAL_SPI_GetState(SPI_HandleTypeDef *hspi)
{
    /* Return SPI handle state */
    return hspi->State;
}

/**
  * @brief  Return the SPI error code.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval SPI error code in bitmap format
  */
uint32_t HAL_SPI_GetError(SPI_HandleTypeDef *hspi)
{
    /* Return SPI ErrorCode */
    return hspi->ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */


/** @addtogroup SPI_Private_Functions
  * @brief   Private functions
  * @{
  */


__HAL_SPI_DRIVER_ATTRIBUTE static void SPI_TxISR_MasterClock(struct __SPI_HandleTypeDef *hspi)
{
    for (uint8_t i = 0U; i < HAL_SPI_HALF_FIFO_DEEP; i++)
    {
        hspi->Instance->DR = SPI_DUMMY_WORD;
        if(hspi->TxXferCount > 0)
        {
            hspi->TxXferCount--;
        }
        if (hspi->TxXferCount == 0U)
        {
            /* Disable TXE and ERR interrupt */
            __HAL_SPI_DISABLE_IT(hspi, SPI_IT_TXE);
            hspi->State = HAL_SPI_STATE_READY;
            //break;
        }
    }
}
__HAL_SPI_DRIVER_ATTRIBUTE static void SPI_RxISR_ClearFIFO(struct __SPI_HandleTypeDef *hspi)
{
    uint16_t data;

    for (uint8_t i = 0U; i < HAL_SPI_HALF_FIFO_DEEP; i++)
    {
        data = (hspi->Instance->DR & 0xFFFF);
        if(hspi->RxXferCount > 0)
        {
            hspi->RxXferCount--;
        }
        if (hspi->RxXferCount == 0U)
        {
            /* Disable RXNE and ERR interrupt */
            __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR));
            hspi->RxState = HAL_SPI_STATE_READY;
            //break;
        }
    }
}
/**
  * @brief  Manage the receive 8-bit in Interrupt context.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__HAL_SPI_DRIVER_ATTRIBUTE static void SPI_RxISR_8BIT(struct __SPI_HandleTypeDef *hspi)
{
    uint16_t data;
    uint8_t invalidRecvLen = (HAL_SPI_HALF_FIFO_DEEP > hspi->RxXferCount) ? (HAL_SPI_HALF_FIFO_DEEP - hspi->RxXferCount) : 0;

    for (uint8_t i=0U; i<HAL_SPI_HALF_FIFO_DEEP; i++)
    {
        *(hspi->pRxBuffPtr) = (hspi->Instance->DR & 0xFF);
        hspi->pRxBuffPtr++;
        hspi->RxXferCount--;

        if (hspi->RxXferCount == 0U)
        {
            SPI_CloseRx_ISR(hspi);
            break;
        }
    }

    if(invalidRecvLen > 0)
    {
        for (uint8_t i=0U; i<invalidRecvLen; i++)
        {
            data = (hspi->Instance->DR & 0xFFFF);
        }
    }
}

/**
  * @brief  Manage the 16-bit receive in Interrupt context.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__HAL_SPI_DRIVER_ATTRIBUTE void SPI_RxISR_16BIT(struct __SPI_HandleTypeDef *hspi)
{
    uint16_t data;
    uint8_t invalidRecvLen = (HAL_SPI_HALF_FIFO_DEEP > hspi->RxXferCount) ? (HAL_SPI_HALF_FIFO_DEEP - hspi->RxXferCount) : 0;

    for (uint8_t i=0U; i<HAL_SPI_HALF_FIFO_DEEP; i++)
    {
        WriteUint16_BigEndian(hspi->pTxBuffPtr, hspi->Instance->DR);
        hspi->pRxBuffPtr += sizeof(uint16_t);
        hspi->RxXferCount--;

        if (hspi->RxXferCount == 0U)
        {
            SPI_CloseRx_ISR(hspi);
            break;
        }
    }

    if(invalidRecvLen > 0)
    {
        for (uint8_t i=0U; i<invalidRecvLen; i++)
        {
            data = (hspi->Instance->DR & 0xFFFF);
        }
    }
}

/**
  * @brief  Handle the data 8-bit transmit in Interrupt mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_TxISR_8BIT(struct __SPI_HandleTypeDef *hspi)
{
    uint8_t data[HAL_SPI_HALF_FIFO_DEEP] = {SPI_DUMMY_BYTE, SPI_DUMMY_BYTE, SPI_DUMMY_BYTE, SPI_DUMMY_BYTE};

    if(hspi->TxXferCount < HAL_SPI_HALF_FIFO_DEEP)
    {
        memcpy(data, hspi->pTxBuffPtr, hspi->TxXferCount);
        hspi->pTxBuffPtr = data;
        hspi->TxXferCount = HAL_SPI_HALF_FIFO_DEEP;
    }

    for (uint8_t i=0U; i<HAL_SPI_HALF_FIFO_DEEP; i++)
    {
        hspi->Instance->DR = *(hspi->pTxBuffPtr);
        hspi->pTxBuffPtr++;
        hspi->TxXferCount--;

        if (hspi->TxXferCount == 0U)
        {
            SPI_CloseTx_ISR(hspi);
            break;
        }
    }
}

/**
  * @brief  Handle the data 16-bit transmit in Interrupt mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_TxISR_16BIT(struct __SPI_HandleTypeDef *hspi)
{
    uint16_t data[HAL_SPI_HALF_FIFO_DEEP] = {SPI_DUMMY_WORD, SPI_DUMMY_WORD, SPI_DUMMY_WORD, SPI_DUMMY_WORD};

    if(hspi->TxXferCount < HAL_SPI_HALF_FIFO_DEEP)
    {
        memcpy(data, hspi->pTxBuffPtr, hspi->TxXferCount*sizeof(uint16_t));
        hspi->pTxBuffPtr = (uint8_t *)data;
        hspi->TxXferCount = HAL_SPI_HALF_FIFO_DEEP;
    }

    for (uint8_t i = 0U; i < HAL_SPI_HALF_FIFO_DEEP; i++)
    {
        /* Transmit data in 16 Bit mode */
        hspi->Instance->DR = ReadUint16_LittleEndian(hspi->pTxBuffPtr);
        hspi->pTxBuffPtr += sizeof(uint16_t);
        hspi->TxXferCount--;
        if (hspi->TxXferCount == 0U)
        {
            SPI_CloseTx_ISR(hspi);
            break;
        }
    }
}

/**
  * @brief  Handle SPI Communication Timeout.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *              the configuration information for SPI module.
  * @param  Flag SPI flag to check
  * @param  State flag state to check
  * @param  Timeout Timeout duration
  * @param  Tickstart tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef SPI_WaitFlagStateUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, FlagStatus State,
                                                       uint32_t Timeout, uint32_t Tickstart)
{
    __IO uint32_t count;
    uint32_t tmp_timeout;
    uint32_t tmp_tickstart;

    /* Adjust Timeout value  in case of end of transfer */
    tmp_timeout   = Timeout - (HAL_GetTick() - Tickstart);
    tmp_tickstart = HAL_GetTick();

    /* Calculate Timeout based on a software loop to avoid blocking issue if Systick is disabled */
    count = tmp_timeout * ((GetSystemCoreClock() * 32U) >> 20U);

    while ((__HAL_SPI_GET_FLAG(hspi, Flag) ? SET : RESET) != State)
    {
        if (Timeout != HAL_MAX_DELAY)
        {
            if (((HAL_GetTick() - tmp_tickstart) >= tmp_timeout) || (tmp_timeout == 0U))
            {
                /* Disable the SPI and reset the CRC: the CRC value should be cleared
                on both master and slave sides in order to resynchronize the master
                and slave for their respective CRC calculation */

                /* Disable TXE, RXNE and ERR interrupts for the interrupt process */
                __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));

                hspi->State = HAL_SPI_STATE_READY;

                /* Process Unlocked */
                __HAL_UNLOCK(hspi);

                return HAL_TIMEOUT;
            }
            /* If Systick is disabled or not incremented, deactivate timeout to go in disable loop procedure */
            if (count == 0U)
            {
                tmp_timeout = 0U;
            }
            count--;
        }
    }

    return HAL_OK;
}

/**
  * @brief  Handle the end of the RX transaction.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_CloseRx_ISR(SPI_HandleTypeDef *hspi)
{
    /* Disable RXNE and ERR interrupt */
    __HAL_SPI_DISABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR));

    hspi->RxState = HAL_SPI_STATE_READY;

    /* Call user error callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
    hspi->RxCpltCallback(hspi);
#else
    HAL_SPI_RxCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}

/**
  * @brief  Handle the end of the TX transaction.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_CloseTx_ISR(SPI_HandleTypeDef *hspi)
{
    /* Disable TXE and ERR interrupt */
    __HAL_SPI_DISABLE_IT(hspi, SPI_IT_TXE);

    hspi->State = HAL_SPI_STATE_READY;

        /* Call user Rx complete callback */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
    hspi->TxCpltCallback(hspi);
#else
    HAL_SPI_TxCpltCallback(hspi);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
}

/**
  * @brief  Handle abort a Rx transaction.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_AbortRx_ISR(SPI_HandleTypeDef *hspi)
{
    __IO uint32_t tmpreg = 0U;
    __IO uint32_t count = SPI_DEFAULT_TIMEOUT * (GetSystemCoreClock() / 24U / 1000U);

    /* Wait until TXFE flag is set */
    do
    {
        if (count == 0U)
        {
            SET_BIT(hspi->ErrorCode, HAL_SPI_ERROR_ABORT);
            break;
        }
        count--;
    } while ((hspi->Instance->SR & SPI_FLAG_TXFE) == RESET);

    /* Disable SPI Peripheral */
    __HAL_SPI_DISABLE(hspi);

    /* Disable TXEIE, RXNEIE and ERRIE(mode fault event, overrun error, TI frame error) interrupts */
    CLEAR_BIT(hspi->Instance->IMSC, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));

    /* Flush Data Register by a blank read */
    tmpreg = READ_REG(hspi->Instance->DR);
    /* To avoid GCC warning */
    UNUSED(tmpreg);

    hspi->State = HAL_SPI_STATE_ABORT;
}

/**
  * @brief  Handle abort a Tx or Rx/Tx transaction.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
static void SPI_AbortTx_ISR(SPI_HandleTypeDef *hspi)
{
    /* Disable TXEIE interrupt */
    CLEAR_BIT(hspi->Instance->IMSC, SPI_IT_TXE);

    /* Disable SPI Peripheral */
    __HAL_SPI_DISABLE(hspi);

    hspi->State = HAL_SPI_STATE_ABORT;
}

/**
  * @}
  */

#endif /* HAL_SPI_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */


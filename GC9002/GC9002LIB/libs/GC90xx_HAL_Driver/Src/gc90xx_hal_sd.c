/*
 * Copyright (c) 2022-2050, GraphichinaSX Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-22     huyt         V1.0.1
 */

#include <gc90xx_hal.h>

#ifdef HAL_SD_MODULE_ENABLED

/** @addtogroup GC90xx_HAL_Driver
  * @{
  */
/** @addtogroup SD 
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup SD_Private_Defines
  * @{
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup SD_Private_Functions SD Private Functions
  * @{
  */
static uint32_t SD_InitCard(SD_HandleTypeDef *hsd);
static uint32_t SD_PowerON(SD_HandleTypeDef *hsd);                      
static uint32_t SD_SendSDStatus(SD_HandleTypeDef *hsd, uint32_t *pSDstatus);
static uint32_t SD_SendStatus(SD_HandleTypeDef *hsd, uint32_t *pCardStatus);
static uint32_t SD_WideBus_Enable(SD_HandleTypeDef *hsd);
static uint32_t SD_WideBus_Disable(SD_HandleTypeDef *hsd);
static uint32_t SD_FindSCR(SD_HandleTypeDef *hsd, uint32_t *pSCR);
static HAL_StatusTypeDef SD_PowerOFF(SD_HandleTypeDef *hsd);
static HAL_StatusTypeDef SD_Write_IT(SD_HandleTypeDef *hsd);
static HAL_StatusTypeDef SD_Read_IT(SD_HandleTypeDef *hsd);
static void SD_DMATransmitCplt(DMA_HandleTypeDef *hdma);
static void SD_DMAReceiveCplt(DMA_HandleTypeDef *hdma);
static void SD_DMAError(DMA_HandleTypeDef *hdma);
static void SD_DMATxAbort(DMA_HandleTypeDef *hdma);
static void SD_DMARxAbort(DMA_HandleTypeDef *hdma);

/* Exported functions --------------------------------------------------------*/
/** @addtogroup SD_Exported_Functions
  * @{
  */

/** @addtogroup SD_Exported_Functions_Group1
 *  @brief   Initialization and de-initialization functions 
 *
  ==============================================================================
          ##### Initialization and de-initialization functions #####
  ==============================================================================
  [..]  
    This section provides functions allowing to initialize/de-initialize the SD
    card device to be ready for use.
/**
  * @brief  Initializes the SD according to the specified parameters in the 
            SD_HandleTypeDef and create the associated handle.
  * @param  hsd Pointer to the SD handle  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_Init(SD_HandleTypeDef *hsd)
{
    HAL_StatusTypeDef state = HAL_OK;
    /* Check the SD handle allocation */
    if(hsd == NULL)
    {
        return HAL_ERROR;
    }
    /* Check the parameters */
    assert_param(IS_SDIO_HOST_INSTANCE(hsd->Instance->Host));
    assert_param(IS_SDIO_SLOT_INSTANCE(hsd->Instance->Slot));
    assert_param(IS_SDIO_SPEED_MODE(hsd->Init.SpeedMode));
    assert_param(IS_SDIO_CLOCK_MODE(hsd->Init.ClockMode));
    assert_param(IS_SDIO_BUS_WIDE(hsd->Init.BusWide));
    assert_param(IS_SDIO_CLKDIV(hsd->Init.ClockDiv));

    if(hsd->State == HAL_SD_STATE_RESET)
    {
        /* Allocate lock resource and initialize it */
        hsd->Lock = HAL_UNLOCKED;
#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)
        /* Reset Callback pointers in HAL_SD_STATE_RESET only */
        hsd->TxCpltCallback    = HAL_SD_TxCpltCallback;
        hsd->RxCpltCallback    = HAL_SD_RxCpltCallback;
        hsd->ErrorCallback     = HAL_SD_ErrorCallback;
        hsd->AbortCpltCallback = HAL_SD_AbortCallback;
        if(hsd->MspInitCallback == NULL)
        {
            hsd->MspInitCallback = HAL_SD_MspInit;
        }
        /* Init the low level hardware */
        hsd->MspInitCallback(hsd);
#else
        /* Init the low level hardware : GPIO, CLOCK, CORTEX...etc */
        HAL_SD_MspInit(hsd);
#endif
    }
    hsd->State = HAL_SD_STATE_BUSY;
    /* Initialize the Card parameters */
    state = HAL_SD_InitCard(hsd);
    /* Initialize the error code */
    hsd->ErrorCode = HAL_DMA_ERROR_NONE;
    /* Initialize the SD operation */
    hsd->Context = SD_CONTEXT_NONE;
    /* Initialize the SD state */
    hsd->State = HAL_SD_STATE_READY;
    return state;
}

/**
  * @brief  Initializes the SD Card.
  * @param  hsd Pointer to SD handle
  * @note   This function initializes the SD card. It could be used when a card 
            re-initialization is needed.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_InitCard(SD_HandleTypeDef *hsd)
{
    uint32_t errorstate = HAL_SD_ERROR_NONE;
    SDIO_InitTypeDef Init;

    /* Default SDIO peripheral configuration for SD card initialization */
    Init.SpeedMode           = SDIO_Speed_Mode_Full;
    Init.ClockMode           = SDIO_Clock_Mode_10Bit_Divider;
    Init.BusWide             = SDIO_BUS_WIDE_1B;
    Init.ClockDiv            = SDIO_INIT_CLK_DIV;
    /* Initialize SDIO peripheral interface with default configuration */
    SDIO_Init(hsd->Instance, Init);
    /* Disable SDIO Clock */
    __HAL_SD_DISABLE(hsd);
    /* Set Power State to ON */
    SDIO_PowerState_ON(hsd->Instance);
    /* Enable SDIO Clock */
    __HAL_SD_ENABLE(hsd);
    /* Required power up waiting time before starting the SD initialization
    sequence */
    HAL_Delay(2U);
    /* Identify card operating voltage */
    errorstate = SD_PowerON(hsd);
    if(errorstate != HAL_SD_ERROR_NONE)
    {
        hsd->State = HAL_SD_STATE_READY;
        hsd->ErrorCode |= errorstate;
        return HAL_ERROR;
    }
    /* Card initialization */
    errorstate = SD_InitCard(hsd);
    if(errorstate != HAL_SD_ERROR_NONE)
    {
        hsd->State = HAL_SD_STATE_READY;
        hsd->ErrorCode |= errorstate;
        return HAL_ERROR;
    }
    return HAL_OK;
}

/**
  * @brief  De-Initializes the SD card.
  * @param  hsd Pointer to SD handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_DeInit(SD_HandleTypeDef *hsd)
{
    /* Check the SD handle allocation */
    if(hsd == NULL)
    {
        return HAL_ERROR;
    }
    /* Check the parameters */
    assert_param(IS_SDIO_HOST_INSTANCE(hsd->Instance->Host));
    assert_param(IS_SDIO_SLOT_INSTANCE(hsd->Instance->Slot));
    hsd->State = HAL_SD_STATE_BUSY;
    /* Set SD power state to off */
    SD_PowerOFF(hsd);
  
#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)
    if(hsd->MspDeInitCallback == NULL)
    {
        hsd->MspDeInitCallback = HAL_SD_MspDeInit;
    }
    /* DeInit the low level hardware */
    hsd->MspDeInitCallback(hsd);
#else
    /* De-Initialize the MSP layer */
    HAL_SD_MspDeInit(hsd);
#endif
    hsd->ErrorCode |= HAL_SD_ERROR_NONE;
    hsd->State = HAL_SD_STATE_RESET;
    return HAL_OK;
}

/**
  * @brief  Initializes the SD MSP.
  * @param  hsd Pointer to SD handle
  * @retval None
  */
__WEAK void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsd);

    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SD_MspInit could be implemented in the user file
    */
}

/**
  * @brief  De-Initialize SD MSP.
  * @param  hsd Pointer to SD handle
  * @retval None
  */
__WEAK void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsd);

    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SD_MspDeInit could be implemented in the user file
    */
}
/** @addtogroup SD_Exported_Functions_Group2
 *  @brief   Data transfer functions 
 *
@verbatim   
  ==============================================================================
                        ##### IO operation functions #####
  ==============================================================================  
  [..]
    This subsection provides a set of functions allowing to manage the data 
    transfer from/to SD card.
  */
/**
  * @brief  Check hardware error
  * @param  hsd: Pointer to SD handle
  * @retval HAL status
  */
static HAL_StatusTypeDef SD_Check_Error(SD_HandleTypeDef *hsd)
{
    /* Get error state */

    if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_EINT))
    {
        if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_ECCRC))
        {
            hsd->HardwareErrCode |= HAL_SD_ERROR_HW_CMD_CRC;
        }
        else if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_ECEB))
        {
            hsd->HardwareErrCode |= HAL_SD_ERROR_HW_CMD_END_BIT;
        }
        else if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_ECI))
        {
            hsd->HardwareErrCode |= HAL_SD_ERROR_HW_CMD_Index;
        }
        else if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_EDT))
        {
            hsd->HardwareErrCode |= HAL_SD_ERROR_HW_DATA_TIMEOUT;
        }
        else if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_EDCRC))
        {
            hsd->HardwareErrCode |= HAL_SD_ERROR_HW_DATA_CRC;
        }
        else if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_EDEB))
        {
            hsd->HardwareErrCode |= HAL_SD_ERROR_HW_DATA_END_BIT;
        }
        else if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_ECL))
        {
            hsd->HardwareErrCode |= HAL_SD_ERROR_HW_Current_Limit;
        }
        else if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_EAC))
        {
            /* Clear all the static flags */
            hsd->HardwareErrCode |= HAL_SD_ERROR_HW_AUTO_CMD;
        }
        else if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_EADMA))
        {
            hsd->HardwareErrCode |= HAL_SD_ERROR_HW_ADMA;
        }
        else if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_ETNG))
        {
            hsd->HardwareErrCode |= HAL_SD_ERROR_HW_TUNING;
        }
        /* Clear all the error flags */
        __HAL_SD_CLEAR_FLAG(hsd, SD_IT_FLAGS_OTHER_ERRORS | SD_IT_CC | SD_IT_TC);
        return HAL_ERROR;
    }
    return HAL_OK;
}

/**
  * @brief  Reads block(s) from a specified address in a card. The Data transfer 
  *         is managed by polling mode.
  * @note   This API should be followed by a check on the card state through
  *         HAL_SD_GetCardState().
  * @param  hsd: Pointer to SD handle
  * @param  pData: pointer to the buffer that will contain the received data
  * @param  BlockAddr: Block Address from where data is to be read
  * @param  NumberOfBlocks: Number of SD blocks to read
  * @param  Timeout: Specify timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_ReadBlocks(SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAddr, uint32_t NumberOfBlocks, uint32_t Timeout)
{
    uint32_t errorstate = HAL_SD_ERROR_NONE;
    uint32_t tickstart = HAL_GetTick();
    uint32_t count = 0U;
    hsd->RxXferCount = 0;
    hsd->pRxPtr = (uint32_t *)pData;
    Timeout *= (GetSystemCoreClock() / 8U /1000U);

    if(hsd == NULL || NULL == pData)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }
    if((BlockAddr + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))
    {
        hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;
        return HAL_ERROR;
    }
    if(hsd->State == HAL_SD_STATE_READY)
    {
        hsd->ErrorCode = HAL_DMA_ERROR_NONE;
        while(__HAL_SD_GET_STATE(hsd, SD_STATE_CICMD) || __HAL_SD_GET_STATE(hsd, SD_STATE_CIDAT))
        {
            if((Timeout == 0U) || ((HAL_GetTick() - tickstart) >=  Timeout))
            {
                /* Clear all the static flags */
                __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;
                hsd->State= HAL_SD_STATE_READY;
                return HAL_TIMEOUT;
            }
        }
        hsd->State = HAL_SD_STATE_BUSY;
        /* Initialize data control register */
        hsd->Instance->Slot->BLOCK_CSR = 0U;
        if(hsd->SdCard.CardType != CARD_SDHC_SDXC)
        {
            BlockAddr *= 512U;
        }
        /* Set Block Size for Card */
        errorstate = SDMMC_CmdBlockLength(hsd->Instance, hsd->SdCard.BlockSize);
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
            hsd->ErrorCode |= errorstate;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        /* Configure the SD DPSM (Data Path State Machine) */
        hsd->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
        hsd->Config.DataLength    = NumberOfBlocks * hsd->SdCard.BlockSize;
        hsd->Config.DataBlockSize = hsd->SdCard.BlockSize;
        hsd->RxXferSize = hsd->Config.DataLength;
        SDIO_ConfigData(hsd->Instance, &hsd->Config);
        /* Read block(s) in polling mode */
        if(NumberOfBlocks > 1U)
        {
            hsd->Context = SD_CONTEXT_READ_MULTIPLE_BLOCK;
            /* Read Multi Block command */
            errorstate = SDMMC_CmdReadMultiBlock(hsd->Instance, BlockAddr);
        }
        else
        {
            hsd->Context = SD_CONTEXT_READ_SINGLE_BLOCK;
            /* Read Single Block command */
            errorstate = SDMMC_CmdReadSingleBlock(hsd->Instance, BlockAddr);
        }
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
            hsd->ErrorCode |= errorstate;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        /* Poll on SD flags */
        if(__HAL_SD_GET_IT_STATE_EN(hsd, SD_IT_STATE_BRR))
        {
            while(hsd->RxXferCount < hsd->Config.DataLength)
            {
                /* Wait for SRS12.BRR */
                while(!__HAL_SD_GET_FLAG(hsd, SD_FLAG_BRR))
                {
                    /* Get error state */
                    if(SD_Check_Error(hsd) == HAL_ERROR)
                    {
                        /* Clear all the static flags */
                        __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                        hsd->ErrorCode |= HAL_SD_ERROR_HARDWARE;
                        hsd->State = HAL_SD_STATE_READY;
                        return HAL_ERROR;
                    }
                    if((Timeout == 0U)||((HAL_GetTick() - tickstart) >=  Timeout))
                    {
                        /* Clear all the static flags */
                        __HAL_SD_CLEAR_FLAG(hsd, SD_FLAG_BRR | SD_STATIC_FLAGS);
                        hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;
                        hsd->State= HAL_SD_STATE_READY;
                        return HAL_TIMEOUT;
                    }
                }
                /* Check BRR */
                if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_BRR))
                {
                    __HAL_SD_READ_WAIT_ENABLE(hsd);
                    __HAL_SD_CLEAR_FLAG(hsd, SD_FLAG_BRR);
                    /* Read data from SD Rx FIFO */
                    for(count = 0U; count < hsd->Config.DataBlockSize/4; count++)
                    {
                        SDIO_ReadFIFO(hsd->Instance, hsd->pRxPtr++);
                    }
                    __HAL_SD_READ_WAIT_DISABLE(hsd);
                    hsd->RxXferCount += hsd->Config.DataBlockSize;
                    tickstart = HAL_GetTick();
                }
            }
        }
        else
        {
            while(hsd->RxXferCount < hsd->Config.DataLength)
            {
                //Buffer Read Enable
                while(!__HAL_SD_GET_STATE(hsd, SD_STATE_BRE))
                {
                    /* Get error state */
                    if(SD_Check_Error(hsd) == HAL_ERROR)
                    {
                        /* Clear all the static flags */
                        __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                        hsd->ErrorCode |= HAL_SD_ERROR_HARDWARE;
                        hsd->State = HAL_SD_STATE_READY;
                        return HAL_ERROR;
                    }
                    if((Timeout == 0U) || ((HAL_GetTick() - tickstart) >=  Timeout))
                    {
                        /* Clear all the static flags */
                        __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                        hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;
                        hsd->State= HAL_SD_STATE_READY;
                        return HAL_TIMEOUT;
                    }
                }
                __HAL_SD_READ_WAIT_ENABLE(hsd);
                __HAL_SD_CLEAR_FLAG(hsd, SD_FLAG_BRR);
                /* Read data from SD Rx FIFO */
                for(count = 0U; count < hsd->Config.DataBlockSize/4; count++)
                {
                    SDIO_ReadFIFO(hsd->Instance, hsd->pRxPtr++);
                }
                __HAL_SD_READ_WAIT_DISABLE(hsd);
                hsd->RxXferCount += hsd->Config.DataBlockSize;
                tickstart = HAL_GetTick();
            }
        }
        if(__HAL_SD_GET_IT_STATE_EN(hsd, SD_FLAG_TC))
        {
            while(!__HAL_SD_GET_FLAG(hsd, SD_FLAG_TC));
        }
        else
        {
            while(__HAL_SD_GET_STATE(hsd, SD_STATE_CIDAT));
        }
#if(!(defined USE_AUTO_CMD12 && USE_AUTO_CMD12))
        /* Send stop transmission command in case of multiblock read */
        if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_TC) && (NumberOfBlocks > 1U))
        {
            if(hsd->SdCard.CardType != CARD_SECURED)
            {
                /* Send stop transmission command */
                errorstate = SDMMC_CmdStopTransfer(hsd->Instance);
                if(errorstate != HAL_SD_ERROR_NONE)
                {
                    /* Clear all the static flags */
                    __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                    hsd->ErrorCode |= errorstate;
                    hsd->State = HAL_SD_STATE_READY;
                    return HAL_ERROR;
                }
            }
        }
#endif
        /* Get error state */
        if(SD_Check_Error(hsd) == HAL_ERROR)
        {
            /* Clear all the static flags */
            __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
            hsd->ErrorCode |= HAL_SD_ERROR_HARDWARE;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        /* Clear all the static flags */
        __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
        __HAL_SD_IT_STATE_DISABLE(hsd, SD_STATIC_FLAGS);
        hsd->State = HAL_SD_STATE_READY;
        return HAL_OK;
    }
    else
    {
        hsd->ErrorCode |= HAL_SD_ERROR_BUSY;
        return HAL_ERROR;
    }
}

/**
  * @brief  Allows to write block(s) to a specified address in a card. The Data
  *         transfer is managed by polling mode.
  * @note   This API should be followed by a check on the card state through
  *         HAL_SD_GetCardState().
  * @param  hsd Pointer to SD handle
  * @param  pData pointer to the buffer that will contain the data to transmit
  * @param  BlockAddr Block Address where data will be written
  * @param  NumberOfBlocks Number of SD blocks to write
  * @param  Timeout Specify timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_WriteBlocks(SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAddr, uint32_t NumberOfBlocks, uint32_t Timeout)
{
    uint32_t errorstate = HAL_SD_ERROR_NONE;
    uint32_t tickstart = HAL_GetTick();
    uint32_t count = 0U;
    hsd->TxXferCount = 0;
    hsd->pTxPtr = (uint32_t *)pData;
    Timeout *= (GetSystemCoreClock() / 8U /1000U);
    if(hsd == NULL || NULL == pData)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }
    if((BlockAddr + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))
    {
        hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;
        return HAL_ERROR;
    }
    if(hsd->State == HAL_SD_STATE_READY)
    {
        hsd->ErrorCode = HAL_DMA_ERROR_NONE;
        while(__HAL_SD_GET_STATE(hsd, SD_STATE_CICMD) || __HAL_SD_GET_STATE(hsd, SD_STATE_CIDAT))
        {
            if((Timeout == 0U) || ((HAL_GetTick() - tickstart) >=  Timeout))
            {
                /* Clear all the static flags */
                __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;
                hsd->State= HAL_SD_STATE_READY;
                return HAL_TIMEOUT;
            }
        }
        hsd->State = HAL_SD_STATE_BUSY;
        /* Initialize data control register */
        hsd->Instance->Slot->BLOCK_CSR = 0U;
        if(hsd->SdCard.CardType != CARD_SDHC_SDXC)
        {
            BlockAddr *= 512U;
        }
        /* Set Block Size for Card */
        errorstate = SDMMC_CmdBlockLength(hsd->Instance, hsd->SdCard.BlockSize);
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
            hsd->ErrorCode |= errorstate;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        /* Configure the SD DPSM (Data Path State Machine) */
        hsd->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
        hsd->Config.DataLength    = NumberOfBlocks * hsd->SdCard.BlockSize;
        hsd->Config.DataBlockSize = hsd->SdCard.BlockSize;
        hsd->TxXferSize = hsd->Config.DataLength;
        SDIO_ConfigData(hsd->Instance, &hsd->Config);
        __HAL_SD_IT_STATE_ENABLE(hsd, SD_IT_STATE_OTHER_ERRORS);
        /* Write Blocks in Polling mode */
        if(NumberOfBlocks > 1U)
        {
            hsd->Context = SD_CONTEXT_WRITE_MULTIPLE_BLOCK;
            /* Write Multi Block command */
            errorstate = SDMMC_CmdWriteMultiBlock(hsd->Instance, BlockAddr);
        }
        else
        {
            hsd->Context = SD_CONTEXT_WRITE_SINGLE_BLOCK;
            /* Write Single Block command */
            errorstate = SDMMC_CmdWriteSingleBlock(hsd->Instance, BlockAddr);
        }
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
            hsd->ErrorCode |= errorstate;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        /* Poll on SD flags */
        if(__HAL_SD_GET_IT_STATE_EN(hsd, SD_IT_STATE_BWR))
        {
            while(hsd->TxXferCount < hsd->Config.DataLength)
            {
                /* Wait for SRS12.BWR */
                while(!__HAL_SD_GET_FLAG(hsd, SD_FLAG_BWR))
                {
                    /* Get error state */
                    if(SD_Check_Error(hsd) == HAL_ERROR)
                    {
                        /* Clear all the static flags */
                        __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                        hsd->ErrorCode |= HAL_SD_ERROR_HARDWARE;
                        hsd->State = HAL_SD_STATE_READY;
                        return HAL_ERROR;
                    }
                    if((Timeout == 0U) || ((HAL_GetTick() - tickstart) >=  Timeout))
                    {
                        /* Clear all the static flags */
                        __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                        hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;
                        hsd->State= HAL_SD_STATE_READY;
                        return HAL_TIMEOUT;
                    }
                }
                /* Check BWR */
                if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_BWR))
                {
                    __HAL_SD_CLEAR_FLAG(hsd, SD_FLAG_BWR);
                    /* Write data to SD Tx FIFO */
                    for(count = 0U; count < hsd->Config.DataBlockSize/4; count++)
                    {
                        SDIO_WriteFIFO(hsd->Instance, hsd->pTxPtr++);
                    }
                    hsd->TxXferCount += hsd->Config.DataBlockSize;
                    tickstart = HAL_GetTick();
                }
            }
        }
        else
        {
            while(hsd->TxXferCount < hsd->Config.DataLength)
            {
                //Buffer Write Enable
                while(!__HAL_SD_GET_STATE(hsd, SD_STATE_BWE))
                {
                    /* Get error state */
                    if(SD_Check_Error(hsd) == HAL_ERROR)
                    {
                        /* Clear all the static flags */
                        __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                        hsd->ErrorCode |= HAL_SD_ERROR_HARDWARE;
                        hsd->State = HAL_SD_STATE_READY;
                        return HAL_ERROR;
                    }
                    if((Timeout == 0U)||((HAL_GetTick() - tickstart) >=  Timeout))
                    {
                        /* Clear all the static flags */
                        __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                        hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;
                        hsd->State= HAL_SD_STATE_READY;
                        return HAL_TIMEOUT;
                    }
                }
                __HAL_SD_CLEAR_FLAG(hsd, SD_FLAG_BWR);
                /* Write data to SD Tx FIFO */
                for(count = 0U; count < hsd->Config.DataBlockSize/4; count++)
                {
                    SDIO_WriteFIFO(hsd->Instance, hsd->pTxPtr++);
                }
                hsd->TxXferCount += hsd->Config.DataBlockSize;
                tickstart = HAL_GetTick();
            }
        }
        if(__HAL_SD_GET_IT_STATE_EN(hsd, SD_FLAG_TC))
        {
            while(!__HAL_SD_GET_FLAG(hsd, SD_FLAG_TC));
        }
        else
        {
            while(__HAL_SD_GET_STATE(hsd, SD_STATE_CIDAT));
        }
        /* enable AutoCMD12 */
#if(!(defined USE_AUTO_CMD12 && USE_AUTO_CMD12))
        /* Send stop transmission command in case of multiblock write */
        if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_TC) && (NumberOfBlocks > 1U))
        {
            if(hsd->SdCard.CardType != CARD_SECURED)
            {
                /* Send stop transmission command */
                errorstate = SDMMC_CmdStopTransfer(hsd->Instance);
                if(errorstate != HAL_SD_ERROR_NONE)
                {
                    /* Clear all the static flags */
                    __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                    hsd->ErrorCode |= errorstate;
                    hsd->State = HAL_SD_STATE_READY;
                    return HAL_ERROR;
                }
            }
        }
#endif
        /* Get error state */
        if(SD_Check_Error(hsd) == HAL_ERROR)
        {
            hsd->ErrorCode |= HAL_SD_ERROR_HARDWARE;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        /* Clear all the static flags */
        __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
        hsd->State = HAL_SD_STATE_READY;
        return HAL_OK;
    }
    else
    {
        hsd->ErrorCode |= HAL_SD_ERROR_BUSY;
        return HAL_ERROR;
    }
}

/**
  * @brief  Reads block(s) from a specified address in a card. The Data transfer 
  *         is managed in interrupt mode. 
  * @note   This API should be followed by a check on the card state through
  *         HAL_SD_GetCardState().
  * @note   You could also check the IT transfer process through the SD Rx 
  *         interrupt event.
  * @param  hsd Pointer to SD handle                 
  * @param  pData Pointer to the buffer that will contain the received data
  * @param  BlockAddr Block Address from where data is to be read
  * @param  NumberOfBlocks Number of blocks to read.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_ReadBlocks_IT(SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAddr, uint32_t NumberOfBlocks)
{
    uint32_t errorstate = HAL_SD_ERROR_NONE;
    uint32_t tickstart = HAL_GetTick();
    hsd->RxXferCount = 0;
    hsd->pRxPtr = (uint32_t *)pData;

    if(hsd == NULL || NULL == pData)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }
    if((BlockAddr + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))
    {
        hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;
        return HAL_ERROR;
    }
    if(hsd->State == HAL_SD_STATE_READY)
    {
        hsd->ErrorCode = HAL_DMA_ERROR_NONE;
        while(__HAL_SD_GET_STATE(hsd, SD_STATE_CICMD) || __HAL_SD_GET_STATE(hsd, SD_STATE_CIDAT))
        {
            if((HAL_GetTick() - tickstart) >=  SDIO_CMDTIMEOUT)
            {
                /* Clear all the static flags */
                __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;
                hsd->State= HAL_SD_STATE_READY;
                return HAL_TIMEOUT;
            }
        }
        hsd->State = HAL_SD_STATE_BUSY;
        /* Initialize data control register */
        hsd->Instance->Slot->BLOCK_CSR = 0U;
        hsd->pRxBuffPtr = (uint32_t *)pData;
        hsd->RxXferSize = hsd->SdCard.BlockSize * NumberOfBlocks;
        if(hsd->SdCard.CardType != CARD_SDHC_SDXC)
        {
            BlockAddr *= 512U;
        }
        /* Set Block Size for Card */
        errorstate = SDMMC_CmdBlockLength(hsd->Instance, hsd->SdCard.BlockSize);
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
            hsd->ErrorCode |= errorstate;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        /* Configure the SD DPSM (Data Path State Machine) */
        hsd->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
        hsd->Config.DataLength    = hsd->SdCard.BlockSize * NumberOfBlocks;
        hsd->Config.DataBlockSize = hsd->SdCard.BlockSize;
        SDIO_ConfigData(hsd->Instance, &hsd->Config);
        /* Enable transfer interrupts */
        __HAL_SD_CLEAR_FLAG(hsd, (SD_IT_BRR | SD_IT_TC | SD_IT_OTHER_ERRORS));
        __HAL_SD_ENABLE_IT(hsd, (SD_IT_BRR | SD_IT_TC | SD_IT_OTHER_ERRORS));
        /* Read Blocks in IT mode */
        if(NumberOfBlocks > 1U)
        {
            hsd->Context = (SD_CONTEXT_READ_MULTIPLE_BLOCK | SD_CONTEXT_IT);
            /* Read Multi Block command */
            errorstate = SDMMC_CmdReadMultiBlock(hsd->Instance, BlockAddr);
        }
        else
        {
            hsd->Context = (SD_CONTEXT_READ_SINGLE_BLOCK | SD_CONTEXT_IT);
            /* Read Single Block command */
            errorstate = SDMMC_CmdReadSingleBlock(hsd->Instance, BlockAddr);
        }
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            __HAL_SD_DISABLE_IT(hsd, (SD_IT_BRR | SD_IT_TC | SD_IT_OTHER_ERRORS));
            /* Clear all the static flags */
            __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
            hsd->ErrorCode |= errorstate;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Writes block(s) to a specified address in a card. The Data transfer 
  *         is managed in interrupt mode. 
  * @note   This API should be followed by a check on the card state through
  *         HAL_SD_GetCardState().
  * @note   You could also check the IT transfer process through the SD Tx 
  *         interrupt event. 
  * @param  hsd Pointer to SD handle
  * @param  pData Pointer to the buffer that will contain the data to transmit
  * @param  BlockAddr Block Address where data will be written
  * @param  NumberOfBlocks Number of blocks to write
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_WriteBlocks_IT(SD_HandleTypeDef *hsd, uint8_t *pData, uint32_t BlockAddr, uint32_t NumberOfBlocks)
{
    uint32_t errorstate = HAL_SD_ERROR_NONE;
    uint32_t tickstart = HAL_GetTick();
    hsd->TxXferCount = 0;
    hsd->pTxPtr = (uint32_t *)pData;
  
    if(hsd == NULL || NULL == pData)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }
    if((BlockAddr + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))
    {
        hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;
        return HAL_ERROR;
    }
    if(hsd->State == HAL_SD_STATE_READY)
    {
        hsd->ErrorCode = HAL_DMA_ERROR_NONE;
        while(__HAL_SD_GET_STATE(hsd, SD_STATE_CICMD) || __HAL_SD_GET_STATE(hsd, SD_STATE_CIDAT))
        {
            if((HAL_GetTick() - tickstart) >=  SDIO_CMDTIMEOUT)
            {
                /* Clear all the static flags */
                __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;
                hsd->State= HAL_SD_STATE_READY;
                return HAL_TIMEOUT;
            }
        }
        hsd->State = HAL_SD_STATE_BUSY;
        /* Initialize data control register */
        hsd->Instance->Slot->BLOCK_CSR = 0U;
        hsd->pTxBuffPtr = (uint32_t *)pData;
        hsd->TxXferSize = hsd->SdCard.BlockSize * NumberOfBlocks;
        if(hsd->SdCard.CardType != CARD_SDHC_SDXC)
        {
            BlockAddr *= 512U;
        }
        /* Set Block Size for Card */
        errorstate = SDMMC_CmdBlockLength(hsd->Instance, hsd->SdCard.BlockSize);
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
            hsd->ErrorCode |= errorstate;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        /* Configure the SD DPSM (Data Path State Machine) */
        hsd->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
        hsd->Config.DataLength    = hsd->SdCard.BlockSize * NumberOfBlocks;
        hsd->Config.DataBlockSize = hsd->SdCard.BlockSize;
        SDIO_ConfigData(hsd->Instance, &hsd->Config);
        /* Enable transfer interrupts */
        __HAL_SD_CLEAR_FLAG(hsd, (SD_IT_BWR | SD_IT_TC | SD_IT_OTHER_ERRORS));
        __HAL_SD_ENABLE_IT(hsd, (SD_IT_BWR | SD_IT_TC | SD_IT_OTHER_ERRORS));
        /* Write Blocks in Polling mode */
        if(NumberOfBlocks > 1U)
        {
            hsd->Context = (SD_CONTEXT_WRITE_MULTIPLE_BLOCK | SD_CONTEXT_IT);
            /* Write Multi Block command */
            errorstate = SDMMC_CmdWriteMultiBlock(hsd->Instance, BlockAddr);
        }
        else
        {
            hsd->Context = (SD_CONTEXT_WRITE_SINGLE_BLOCK | SD_CONTEXT_IT);
            /* Write Single Block command */
            errorstate = SDMMC_CmdWriteSingleBlock(hsd->Instance, BlockAddr);
        }
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_SD_DISABLE_IT(hsd, (SD_IT_BWR | SD_IT_TC | SD_IT_OTHER_ERRORS));
            __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
            hsd->ErrorCode |= errorstate;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Reads block(s) from a specified address in a card. The Data transfer 
  *         is managed by DMA mode. 
  * @note   This API should be followed by a check on the card state through
  *         HAL_SD_GetCardState().
  * @note   You could also check the DMA transfer process through the SD Rx 
  *         interrupt event.
  * @param  hsd Pointer SD handle                 
  * @param  DMAInit ADMAx Descriptor
  * @param  BlockAddr Block Address from where data is to be read
  * @param  NumberOfBlocks Number of blocks to read.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_ReadBlocks_DMA(SD_HandleTypeDef *hsd, SDIO_DMAInitTypedef *DMAInit, uint32_t BlockAddr, uint32_t NumberOfBlocks)
{
    uint32_t errorstate = HAL_SD_ERROR_NONE;
    uint32_t tickstart = HAL_GetTick();
    hsd->dmaInit = DMAInit;
    hsd->RxXferCount = 0;
    uint32_t block_size = 0;

    if(hsd == NULL ||
      (hsd->dmaInit->DMAx == SDIO_SDMA && hsd->dmaInit->SDMA_Descriptor == NULL) ||
      (hsd->dmaInit->DMAx == SDIO_ADMA1 && hsd->dmaInit->ADMA1_DescriptorTable == NULL) ||
      (hsd->dmaInit->DMAx == SDIO_ADMA2 && hsd->dmaInit->ADMA2_DescriptorTable == NULL))
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }
    if((BlockAddr + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))
    {
        hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;
        return HAL_ERROR;
    }
    if(hsd->State == HAL_SD_STATE_READY)
    {
        hsd->ErrorCode = HAL_DMA_ERROR_NONE;
        while(__HAL_SD_GET_STATE(hsd, SD_STATE_CICMD) || __HAL_SD_GET_STATE(hsd, SD_STATE_CIDAT))
        {
            if((HAL_GetTick() - tickstart) >=  SDIO_CMDTIMEOUT)
            {
                /* Clear all the static flags */
                __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;
                hsd->State= HAL_SD_STATE_READY;
                return HAL_TIMEOUT;
            }
        }
        hsd->State = HAL_SD_STATE_BUSY;
        /* Initialize data control register */
        hsd->Instance->Slot->BLOCK_CSR = 0U;
        if(hsd->dmaInit->DMAx == SDIO_SDMA)
        {
            block_size = hsd->dmaInit->SDMA_Descriptor->BlockSize;
        }
        else
        {
            block_size = hsd->SdCard.BlockSize;
        }
        if(hsd->SdCard.CardType != CARD_SDHC_SDXC)
        {
            BlockAddr *= 512U;
        }
        /* Set Block Size for Card */
        errorstate = SDMMC_CmdBlockLength(hsd->Instance, block_size);
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
            hsd->ErrorCode |= errorstate;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        if(hsd->dmaInit->DMAx == SDIO_SDMA)
        {
            hsd->pRxPtr = (uint32_t *)hsd->dmaInit->SDMA_Descriptor->Address;
            __HAL_SD_DMA_SELECT(hsd, hsd->dmaInit->DMAx);
            /* Set SDMA System Address */
            __HAL_SD_SDMA_SET_ADDR(hsd, hsd->dmaInit->SDMA_Descriptor->Address);
            __HAL_SD_SDMA_SET_BOUNDARY(hsd, hsd->dmaInit->SDMA_Descriptor->BufferBoundary);
            /* Flush and invalidate Cache by addr */
            ccm_FlushInvalidateDCache_by_Addr((void *)hsd->dmaInit->SDMA_Descriptor->Address, hsd->dmaInit->SDMA_Descriptor->BlockCount * hsd->dmaInit->SDMA_Descriptor->BlockSize);
            /* Configure the SD DPSM (Data Path State Machine) */
            hsd->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
            hsd->Config.DataLength    = hsd->dmaInit->SDMA_Descriptor->BlockCount * hsd->dmaInit->SDMA_Descriptor->BlockSize;
            hsd->Config.DataBlockSize = hsd->dmaInit->SDMA_Descriptor->BlockSize;
        }
        else if (hsd->dmaInit->DMAx == SDIO_ADMA1)
        {
            hsd->pRxPtr = (uint32_t *)(hsd->dmaInit->ADMA1_DescriptorTable[1].PageAddress);
            __HAL_SD_DMA_SELECT(hsd, hsd->dmaInit->DMAx);
            /* Set ADMA System Address */
            __HAL_SD_ADMA_SET_ADDR(hsd, (uint32_t)hsd->dmaInit->ADMA1_DescriptorTable);
            /* Flush Cache by addr */
            ccm_FlushDCache_by_Addr((void *)hsd->dmaInit->ADMA1_DescriptorTable, hsd->dmaInit->table_size*sizeof(SDIO_ADMA1_Descriptor));
            /* Flush Cache by addr */
            ccm_FlushInvalidateDCache_by_Addr((void *)hsd->dmaInit->ADMA1_DescriptorTable[1].PageAddress, hsd->SdCard.BlockSize * NumberOfBlocks);
            /* Configure the SD DPSM (Data Path State Machine) */
            hsd->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
            hsd->Config.DataLength    = hsd->SdCard.BlockSize * NumberOfBlocks;
            hsd->Config.DataBlockSize = hsd->SdCard.BlockSize;
        }
        else if (hsd->dmaInit->DMAx == SDIO_ADMA2)
        {
            hsd->pRxPtr = (uint32_t *)hsd->dmaInit->ADMA2_DescriptorTable->PageAddress;
            __HAL_SD_DMA_SELECT(hsd, hsd->dmaInit->DMAx);
            /* Set ADMA System Address */
            __HAL_SD_ADMA_SET_ADDR(hsd, (uint32_t)hsd->dmaInit->ADMA2_DescriptorTable);
            /* Flush Cache by addr */
            ccm_FlushDCache_by_Addr((void *)hsd->dmaInit->ADMA2_DescriptorTable, hsd->dmaInit->table_size*sizeof(SDIO_ADMA2_Descriptor));
            /* Flush Cache by addr */
            ccm_FlushInvalidateDCache_by_Addr((void *)hsd->dmaInit->ADMA2_DescriptorTable->PageAddress, hsd->SdCard.BlockSize * NumberOfBlocks);
            /* Configure the SD DPSM (Data Path State Machine) */
            hsd->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
            hsd->Config.DataLength    = hsd->SdCard.BlockSize * NumberOfBlocks;
            hsd->Config.DataBlockSize = hsd->SdCard.BlockSize;
        }
        else
        {
            hsd->ErrorCode |= HAL_SD_ERROR_HW_OTHER_DMA_NOT_SUP;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        /* Clear all the static flags */
        __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
        __HAL_SD_ENABLE_IT(hsd, (SD_IT_TC | SD_IT_DMAINT | SD_IT_OTHER_ERRORS));
        SDIO_ConfigData(hsd->Instance, &hsd->Config);
        /* Read Blocks in DMA mode */
        if(NumberOfBlocks > 1U)
        {
            hsd->Context = (SD_CONTEXT_READ_MULTIPLE_BLOCK | SD_CONTEXT_DMA);
            /* Read Multi Block command */
            errorstate = SDMMC_CmdReadMultiBlockDMA(hsd->Instance, BlockAddr);
        }
        else
        {
            hsd->Context = (SD_CONTEXT_READ_SINGLE_BLOCK | SD_CONTEXT_DMA);
            /* Read Single Block command */
            errorstate = SDMMC_CmdReadSingleBlockDMA(hsd->Instance, BlockAddr);
        }
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
            hsd->ErrorCode |= errorstate;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Writes block(s) to a specified address in a card. The Data transfer 
  *         is managed by DMA mode. 
  * @note   This API should be followed by a check on the card state through
  *         HAL_SD_GetCardState().
  * @note   You could also check the DMA transfer process through the SD Tx 
  *         interrupt event.
  * @param  hsd Pointer to SD handle
  * @param  DMAInit ADMAx Descriptor
  * @param  BlockAddr Block Address where data will be written
  * @param  NumberOfBlocks Number of blocks to write
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_WriteBlocks_DMA(SD_HandleTypeDef *hsd, SDIO_DMAInitTypedef *DMAInit, uint32_t BlockAddr, uint32_t NumberOfBlocks)
{
    uint32_t errorstate = HAL_SD_ERROR_NONE;
    uint32_t tickstart = HAL_GetTick();
    hsd->TxXferCount = 0;
    hsd->dmaInit = DMAInit;
    uint32_t block_size = 0;

    if(hsd == NULL ||
       (hsd->dmaInit->DMAx == SDIO_SDMA && hsd->dmaInit->SDMA_Descriptor == NULL) ||
       (hsd->dmaInit->DMAx == SDIO_ADMA1 && hsd->dmaInit->ADMA1_DescriptorTable == NULL) ||
       (hsd->dmaInit->DMAx == SDIO_ADMA2 && hsd->dmaInit->ADMA2_DescriptorTable == NULL))
    {
        hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        return HAL_ERROR;
    }
    if((BlockAddr + NumberOfBlocks) > (hsd->SdCard.LogBlockNbr))
    {
        hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;
        return HAL_ERROR;
    }
    if(hsd->State == HAL_SD_STATE_READY)
    {
        hsd->ErrorCode = HAL_DMA_ERROR_NONE;
        while(__HAL_SD_GET_STATE(hsd, SD_STATE_CICMD) || __HAL_SD_GET_STATE(hsd, SD_STATE_CIDAT))
        {
            if((HAL_GetTick() - tickstart) >=  SDIO_CMDTIMEOUT)
            {
                /* Clear all the static flags */
                __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;
                hsd->State= HAL_SD_STATE_READY;
                return HAL_TIMEOUT;
            }
        }
        hsd->State = HAL_SD_STATE_BUSY;
        /* Initialize data control register */
        hsd->Instance->Slot->BLOCK_CSR= 0U;
        if(hsd->dmaInit->DMAx == SDIO_SDMA)
        {
            block_size = hsd->dmaInit->SDMA_Descriptor->BlockSize;
        }
        else
        {
            block_size = hsd->SdCard.BlockSize;
        }
        /* Set Block Size for Card */
        errorstate = SDMMC_CmdBlockLength(hsd->Instance, block_size);
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
            hsd->ErrorCode |= errorstate;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        if(hsd->SdCard.CardType != CARD_SDHC_SDXC)
        {
            BlockAddr *= 512U;
        }
        if(hsd->dmaInit->DMAx == SDIO_SDMA)
        {
            hsd->pTxPtr = (uint32_t *)hsd->dmaInit->SDMA_Descriptor->Address;
            __HAL_SD_DMA_SELECT(hsd, hsd->dmaInit->DMAx);
            /* Set SDMA System Address */
            __HAL_SD_SDMA_SET_ADDR(hsd, hsd->dmaInit->SDMA_Descriptor->Address);
            __HAL_SD_SDMA_SET_BOUNDARY(hsd, hsd->dmaInit->SDMA_Descriptor->BufferBoundary);
            /* Flush Cache by addr */
            ccm_FlushDCache_by_Addr((void *)hsd->dmaInit->SDMA_Descriptor->Address, hsd->dmaInit->SDMA_Descriptor->BlockCount * hsd->dmaInit->SDMA_Descriptor->BlockSize);
            /* Configure the SD DPSM (Data Path State Machine) */
            hsd->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
            hsd->Config.DataLength    = hsd->dmaInit->SDMA_Descriptor->BlockCount * hsd->dmaInit->SDMA_Descriptor->BlockSize;
            hsd->Config.DataBlockSize = hsd->dmaInit->SDMA_Descriptor->BlockSize;
        }
        else if (hsd->dmaInit->DMAx == SDIO_ADMA1)
        {
            hsd->pTxPtr = (uint32_t *)(hsd->dmaInit->ADMA1_DescriptorTable->PageAddress);
            __HAL_SD_DMA_SELECT(hsd, hsd->dmaInit->DMAx);
            /* Set ADMA System Address */
            __HAL_SD_ADMA_SET_ADDR(hsd, (uint32_t)hsd->dmaInit->ADMA1_DescriptorTable);
            /* Flush Cache by addr */
            ccm_FlushDCache_by_Addr((void *)hsd->dmaInit->ADMA1_DescriptorTable, hsd->dmaInit->table_size*sizeof(SDIO_ADMA1_Descriptor));
            /* Flush Cache by addr */
            ccm_FlushDCache_by_Addr((void *)(hsd->dmaInit->ADMA1_DescriptorTable->PageAddress & 0xFFFFF000), hsd->SdCard.BlockSize * NumberOfBlocks);
            /* Configure the SD DPSM (Data Path State Machine) */
            hsd->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
            hsd->Config.DataLength    = hsd->SdCard.BlockSize * NumberOfBlocks;
            hsd->Config.DataBlockSize = hsd->SdCard.BlockSize;
        }
        else if (hsd->dmaInit->DMAx == SDIO_ADMA2)
        {
            hsd->pTxPtr = (uint32_t *)hsd->dmaInit->ADMA2_DescriptorTable->PageAddress;
            __HAL_SD_DMA_SELECT(hsd, hsd->dmaInit->DMAx);
            /* Set ADMA System Address */
            __HAL_SD_ADMA_SET_ADDR(hsd, (uint32_t)hsd->dmaInit->ADMA2_DescriptorTable);
            /* Flush Cache by addr */
            ccm_FlushDCache_by_Addr((void *)hsd->dmaInit->ADMA2_DescriptorTable, hsd->dmaInit->table_size*sizeof(SDIO_ADMA2_Descriptor));
            /* Flush Cache by addr */
            ccm_FlushDCache_by_Addr((void *)hsd->dmaInit->ADMA2_DescriptorTable->PageAddress, hsd->SdCard.BlockSize * NumberOfBlocks);
            /* Configure the SD DPSM (Data Path State Machine) */
            hsd->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
            hsd->Config.DataLength    = hsd->SdCard.BlockSize * NumberOfBlocks;
            hsd->Config.DataBlockSize = hsd->SdCard.BlockSize;
        }
        else
        {
            hsd->ErrorCode |= HAL_SD_ERROR_HW_OTHER_DMA_NOT_SUP;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        /* Clear all the static flags */
        __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
        /* Enable SD Error interrupts */
        __HAL_SD_ENABLE_IT(hsd, (SD_IT_TC | SD_IT_DMAINT | SD_IT_EADMA));
        SDIO_ConfigData(hsd->Instance, &hsd->Config);
        /* Write Blocks in Polling mode */
        if(NumberOfBlocks > 1U)
        {
            hsd->Context = (SD_CONTEXT_WRITE_MULTIPLE_BLOCK | SD_CONTEXT_DMA);
            /* Write Multi Block command */
            errorstate = SDMMC_CmdWriteMultiBlockDMA(hsd->Instance, BlockAddr);
        }
        else
        {
            hsd->Context = (SD_CONTEXT_WRITE_SINGLE_BLOCK | SD_CONTEXT_DMA);
            /* Write Single Block command */
            errorstate = SDMMC_CmdWriteSingleBlockDMA(hsd->Instance, BlockAddr);
        }
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_SD_DISABLE_IT(hsd, (SD_IT_TC | SD_IT_DMAINT | SD_IT_EADMA));
            __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
            hsd->ErrorCode |= errorstate;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Erases the specified memory area of the given SD card.
  * @note   This API should be followed by a check on the card state through
  *         HAL_SD_GetCardState().
  * @param  hsd Pointer to SD handle 
  * @param  BlockStartAdd Start Block address
  * @param  BlockEndAdd End Block address
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_Erase(SD_HandleTypeDef *hsd, uint32_t BlockStartAdd, uint32_t BlockEndAdd)
{
    uint32_t errorstate = HAL_SD_ERROR_NONE;
  
    if(hsd->State == HAL_SD_STATE_READY)
    {
        hsd->ErrorCode = HAL_DMA_ERROR_NONE;
        if(BlockEndAdd < BlockStartAdd)
        {
            hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
            return HAL_ERROR;
        }
        if(BlockEndAdd > (hsd->SdCard.LogBlockNbr))
        {
            hsd->ErrorCode |= HAL_SD_ERROR_ADDR_OUT_OF_RANGE;
            return HAL_ERROR;
        }
        hsd->State = HAL_SD_STATE_BUSY;
        /* Check if the card command class supports erase command */
        if(((hsd->SdCard.Class) & SDIO_CCCC_ERASE) == 0U)
        {
            /* Clear all the static flags */
            __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
            hsd->ErrorCode |= HAL_SD_ERROR_REQUEST_NOT_APPLICABLE;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        if((SDIO_GetResponse(hsd->Instance, SDIO_RESP0) & SDMMC_CARD_LOCKED) == SDMMC_CARD_LOCKED)
        {
            /* Clear all the static flags */
            __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
            hsd->ErrorCode |= HAL_SD_ERROR_LOCK_UNLOCK_FAILED;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        /* Get start and end block for high capacity cards */
        if(hsd->SdCard.CardType != CARD_SDHC_SDXC)
        {
            BlockStartAdd *= 512U;
            BlockEndAdd   *= 512U;
        }
        /* According to sd-card spec 1.0 ERASE_GROUP_START (CMD32) and erase_group_end(CMD33) */
        if(hsd->SdCard.CardType != CARD_SECURED)
        {
            /* Send CMD32 SD_ERASE_GRP_START with argument as addr  */
            errorstate = SDMMC_CmdSDEraseStartAdd(hsd->Instance, BlockStartAdd);
            if(errorstate != HAL_SD_ERROR_NONE)
            {
                /* Clear all the static flags */
                __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                hsd->ErrorCode |= errorstate;
                hsd->State = HAL_SD_STATE_READY;
                return HAL_ERROR;
            }
            /* Send CMD33 SD_ERASE_GRP_END with argument as addr  */
            errorstate = SDMMC_CmdSDEraseEndAdd(hsd->Instance, BlockEndAdd);
            if(errorstate != HAL_SD_ERROR_NONE)
            {
                /* Clear all the static flags */
                __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                hsd->ErrorCode |= errorstate;
                hsd->State = HAL_SD_STATE_READY;
                return HAL_ERROR;
            }
        }
        /* Send CMD38 ERASE */
        errorstate = SDMMC_CmdErase(hsd->Instance);
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
            hsd->ErrorCode |= errorstate;
            hsd->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        hsd->State = HAL_SD_STATE_READY;
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  This function handles SD card interrupt request.
  * @param  hsd Pointer to SD handle
  * @retval None
  */
void HAL_SD_IRQHandler(SD_HandleTypeDef *hsd)
{
    uint32_t errorstate = HAL_SD_ERROR_NONE;

    /* Check for SDIO interrupt flags */
    if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_DMAINT))
    {
#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)
        hsd->DMAINTCallback(hsd);
#else
        HAL_SD_DMAINTCallback(hsd);
#endif
        if(hsd->dmaInit->DMAx == SDIO_SDMA)
        {
            //update SDMA System Address, to resume the SDMA transaction.
            __HAL_SD_DMA_CONTINUE_REQUEST(hsd);
        }
        __HAL_SD_CLEAR_FLAG(hsd, SD_FLAG_DMAINT);
    }
    if(__HAL_SD_GET_FLAG(hsd, SD_IT_TC))
    {
        __HAL_SD_DISABLE_IT(hsd, SD_FLAG_TC | SD_FLAG_EDCRC | SD_FLAG_EDT | SD_FLAG_EDEB);
        __HAL_SD_CLEAR_FLAG(hsd, SD_IT_TC);
        if((hsd->Context & SD_CONTEXT_IT) != RESET)
        {
#if(defined USE_AUTO_CMD12 && !USE_AUTO_CMD12)
            if(((hsd->Context & SD_CONTEXT_READ_MULTIPLE_BLOCK) != RESET) || ((hsd->Context & SD_CONTEXT_WRITE_MULTIPLE_BLOCK) != RESET))
            {
                errorstate = SDMMC_CmdStopTransfer(hsd->Instance);
                if(errorstate != HAL_SD_ERROR_NONE)
                {
                    hsd->ErrorCode |= errorstate;
#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)
                    hsd->ErrorCallback(hsd);
#else
                    HAL_SD_ErrorCallback(hsd);
#endif
                }
            }
#endif
            hsd->State = HAL_SD_STATE_READY;
            if(((hsd->Context & SD_CONTEXT_READ_SINGLE_BLOCK) != RESET) || ((hsd->Context & SD_CONTEXT_READ_MULTIPLE_BLOCK) != RESET))
            {
#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)
                hsd->RxCpltCallback(hsd);
#else
                HAL_SD_RxCpltCallback(hsd);
#endif
            }
            else
            {
#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)
                hsd->TxCpltCallback(hsd);
#else      
                HAL_SD_TxCpltCallback(hsd);
#endif
            }
        }
        else if((hsd->Context & SD_CONTEXT_DMA) != RESET)
        {
#if(defined USE_AUTO_CMD12 && !USE_AUTO_CMD12)
            if((hsd->Context & SD_CONTEXT_WRITE_MULTIPLE_BLOCK) != RESET)
            {
                errorstate = SDMMC_CmdStopTransfer(hsd->Instance);
                if(errorstate != HAL_SD_ERROR_NONE)
                {
                    hsd->ErrorCode |= errorstate;
#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)
                    hsd->ErrorCallback(hsd);
#else
                    HAL_SD_ErrorCallback(hsd);
#endif
                }
            }
#endif
            if(((hsd->Context & SD_CONTEXT_WRITE_SINGLE_BLOCK) == RESET) && ((hsd->Context & SD_CONTEXT_WRITE_MULTIPLE_BLOCK) == RESET))
            {
                hsd->State = HAL_SD_STATE_READY;
#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)
                hsd->TxCpltCallback(hsd);
#else      
                HAL_SD_TxCpltCallback(hsd);
#endif
            }
            else if(((hsd->Context & SD_CONTEXT_READ_SINGLE_BLOCK) == RESET) && ((hsd->Context & SD_CONTEXT_READ_MULTIPLE_BLOCK) == RESET))
            {
                hsd->State = HAL_SD_STATE_READY;
#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)
                hsd->RxCpltCallback(hsd);
#else
                HAL_SD_RxCpltCallback(hsd);
#endif
            }
        }
    }
    else if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_BWR))
    {
        __HAL_SD_CLEAR_FLAG(hsd, SD_FLAG_BWR);
        SD_Write_IT(hsd);
    }
    else if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_BRR))
    {
        __HAL_SD_CLEAR_FLAG(hsd, SD_FLAG_BRR);
        SD_Read_IT(hsd);
    }
    else if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_EINT))
    {
        hsd->ErrorCode |= HAL_SD_ERROR_HARDWARE;
        /* Set Error code */
        SD_Check_Error(hsd);
        /* Disable all interrupts */
        __HAL_SD_DISABLE_IT(hsd, SD_IT_TC | SD_IT_OTHER_ERRORS);
        if((hsd->Context & SD_CONTEXT_IT) != RESET)
        {
            /* Set the SD state to ready to be able to start again the process */
            hsd->State = HAL_SD_STATE_READY;
            HAL_SD_ErrorCallback(hsd);
        }
    }
}

/**
  * @brief return the SD state
  * @param hsd Pointer to sd handle
  * @retval HAL state
  */
HAL_SD_StateTypeDef HAL_SD_GetState(SD_HandleTypeDef *hsd)
{
    return hsd->State;
}

/**
* @brief  Return the SD error code
* @param  hsd  Pointer to a SD_HandleTypeDef structure that contains
  *              the configuration information.
* @retval SD Error Code
*/
uint32_t HAL_SD_GetError(SD_HandleTypeDef *hsd)
{
    return hsd->ErrorCode;
}

/**
  * @brief DMA INT callbacks
  * @param hsd Pointer to SD handle
  * @retval None
  */
 __WEAK void HAL_SD_DMAINTCallback(SD_HandleTypeDef *hsd)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsd);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SD_DMAINTCallback can be implemented in the user file
    */
}
/**
  * @brief Tx Transfer completed callbacks
  * @param hsd Pointer to SD handle
  * @retval None
  */
 __WEAK void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsd);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SD_TxCpltCallback can be implemented in the user file
    */
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hsd Pointer SD handle
  * @retval None
  */
__WEAK void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsd);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SD_RxCpltCallback can be implemented in the user file
    */
}

/**
  * @brief SD error callbacks
  * @param hsd Pointer SD handle
  * @retval None
  */
__WEAK void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsd);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SD_ErrorCallback can be implemented in the user file
    */
}

/**
  * @brief SD Abort callbacks
  * @param hsd Pointer SD handle
  * @retval None
  */
__WEAK void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hsd);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SD_ErrorCallback can be implemented in the user file
    */
}

#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register a User SD Callback
  *         To be used instead of the weak (surcharged) predefined callback 
  * @param hsd : SD handle
  * @param CallbackId : Id of the callback to be registered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_SD_TX_CPLT_CB_ID    SD Tx Complete Callback ID
  *          @arg @ref HAL_SD_RX_CPLT_CB_ID    SD Rx Complete Callback ID
  *          @arg @ref HAL_SD_ERROR_CB_ID      SD Error Callback ID
  *          @arg @ref HAL_SD_ABORT_CB_ID      SD Abort Callback ID
  *          @arg @ref HAL_SD_MSP_INIT_CB_ID   SD MspInit Callback ID 
  *          @arg @ref HAL_SD_MSP_DEINIT_CB_ID SD MspDeInit Callback ID  
  * @param pCallback : pointer to the Callback function
  * @retval status
  */
HAL_StatusTypeDef HAL_SD_RegisterCallback(SD_HandleTypeDef *hsd, HAL_SD_CallbackIDTypeDef CallbackId, pSD_CallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;

  if(pCallback == NULL)
  {
    /* Update the error code */
    hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK;
    return HAL_ERROR;
  }

  /* Process locked */
  __HAL_LOCK(hsd);
  
  if(hsd->State == HAL_SD_STATE_READY)
  {
    switch (CallbackId)
    {
    case HAL_SD_DMAINT_CB_ID:
      hsd->DMAINTCallback = pCallback;
      break;
    case HAL_SD_TX_CPLT_CB_ID :
      hsd->TxCpltCallback = pCallback;
      break;
    case HAL_SD_RX_CPLT_CB_ID :
      hsd->RxCpltCallback = pCallback;
      break;
    case HAL_SD_ERROR_CB_ID :
      hsd->ErrorCallback = pCallback;
      break;
    case HAL_SD_ABORT_CB_ID :
      hsd->AbortCpltCallback = pCallback;
      break;
    case HAL_SD_MSP_INIT_CB_ID :
      hsd->MspInitCallback = pCallback;
      break;
    case HAL_SD_MSP_DEINIT_CB_ID :
      hsd->MspDeInitCallback = pCallback;
      break;
    default :
      /* Update the error code */
      hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK; 
      /* update return status */
      status =  HAL_ERROR;
      break;
    }
  }
  else if (hsd->State == HAL_SD_STATE_RESET)
  {
    switch (CallbackId)
    {
    case HAL_SD_MSP_INIT_CB_ID :
      hsd->MspInitCallback = pCallback;
      break;
    case HAL_SD_MSP_DEINIT_CB_ID :
      hsd->MspDeInitCallback = pCallback;
      break;
    default :
      /* Update the error code */
      hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK; 
      /* update return status */
      status =  HAL_ERROR;
      break;
    }
  }
  else
  {
    /* Update the error code */
    hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK; 
    /* update return status */
    status =  HAL_ERROR;
  }

  /* Release Lock */
  __HAL_UNLOCK(hsd);
  return status;
}

/**
  * @brief  Unregister a User SD Callback
  *         SD Callback is redirected to the weak (surcharged) predefined callback 
  * @param hsd : SD handle
  * @param CallbackId : Id of the callback to be unregistered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_SD_TX_CPLT_CB_ID    SD Tx Complete Callback ID
  *          @arg @ref HAL_SD_RX_CPLT_CB_ID    SD Rx Complete Callback ID
  *          @arg @ref HAL_SD_ERROR_CB_ID      SD Error Callback ID
  *          @arg @ref HAL_SD_ABORT_CB_ID      SD Abort Callback ID
  *          @arg @ref HAL_SD_MSP_INIT_CB_ID   SD MspInit Callback ID 
  *          @arg @ref HAL_SD_MSP_DEINIT_CB_ID SD MspDeInit Callback ID  
  * @retval status
  */
HAL_StatusTypeDef HAL_SD_UnRegisterCallback(SD_HandleTypeDef *hsd, HAL_SD_CallbackIDTypeDef CallbackId)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Process locked */
  __HAL_LOCK(hsd);
  
  if(hsd->State == HAL_SD_STATE_READY)
  {
    switch (CallbackId)
    {
    case HAL_SD_DMAINT_CB_ID :
      hsd->DMAINTCallback = HAL_SD_DMAINTCallback;
      break;
    case HAL_SD_TX_CPLT_CB_ID :
      hsd->TxCpltCallback = HAL_SD_TxCpltCallback;
      break;
    case HAL_SD_RX_CPLT_CB_ID :
      hsd->RxCpltCallback = HAL_SD_RxCpltCallback;
      break;
    case HAL_SD_ERROR_CB_ID :
      hsd->ErrorCallback = HAL_SD_ErrorCallback;
      break;
    case HAL_SD_ABORT_CB_ID :
      hsd->AbortCpltCallback = HAL_SD_AbortCallback;
      break;
    case HAL_SD_MSP_INIT_CB_ID :
      hsd->MspInitCallback = HAL_SD_MspInit;
      break;
    case HAL_SD_MSP_DEINIT_CB_ID :
      hsd->MspDeInitCallback = HAL_SD_MspDeInit;
      break;
    default :
      /* Update the error code */
      hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK; 
      /* update return status */
      status =  HAL_ERROR;
      break;
    }
  }
  else if (hsd->State == HAL_SD_STATE_RESET)
  {
    switch (CallbackId)
    {
    case HAL_SD_MSP_INIT_CB_ID :
      hsd->MspInitCallback = HAL_SD_MspInit;
      break;
    case HAL_SD_MSP_DEINIT_CB_ID :
      hsd->MspDeInitCallback = HAL_SD_MspDeInit;
      break;
    default :
      /* Update the error code */
      hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK; 
      /* update return status */
      status =  HAL_ERROR;
      break;
    }
  }
  else
  {
    /* Update the error code */
    hsd->ErrorCode |= HAL_SD_ERROR_INVALID_CALLBACK; 
    /* update return status */
    status =  HAL_ERROR;
  }

  /* Release Lock */
  __HAL_UNLOCK(hsd);
  return status;
}
#endif

/** @addtogroup SD_Exported_Functions_Group3
 *  @brief   management functions 
 *
@verbatim   
  ==============================================================================
                      ##### Peripheral Control functions #####
  ==============================================================================  
  [..]
    This subsection provides a set of functions allowing to control the SD card 
    operations and get the related information

@endverbatim
  * @{
  */

/**
  * @brief  Returns information the information of the card which are stored on
  *         the CID register.
  * @param  hsd Pointer to SD handle
  * @param  pCID Pointer to a HAL_SD_CIDTypeDef structure that  
  *         contains all CID register parameters 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_GetCardCID(SD_HandleTypeDef *hsd, HAL_SD_CardCIDTypeDef *pCID)
{
    uint32_t tmp = 0U;

    /* Byte 0 */
    tmp = (uint8_t)((hsd->CID[0U] & 0xFF000000U) >> 24U);
    pCID->ManufacturerID = tmp;

    /* Byte 1 */
    tmp = (uint8_t)((hsd->CID[0U] & 0x00FF0000U) >> 16U);
    pCID->OEM_AppliID = tmp << 8U;

    /* Byte 2 */
    tmp = (uint8_t)((hsd->CID[0U] & 0x000000FF00U) >> 8U);
    pCID->OEM_AppliID |= tmp;

    /* Byte 3 */
    tmp = (uint8_t)(hsd->CID[0U] & 0x000000FFU);
    pCID->ProdName1 = tmp << 24U;

    /* Byte 4 */
    tmp = (uint8_t)((hsd->CID[1U] & 0xFF000000U) >> 24U);
    pCID->ProdName1 |= tmp << 16;

    /* Byte 5 */
    tmp = (uint8_t)((hsd->CID[1U] & 0x00FF0000U) >> 16U);
    pCID->ProdName1 |= tmp << 8U;

    /* Byte 6 */
    tmp = (uint8_t)((hsd->CID[1U] & 0x0000FF00U) >> 8U);
    pCID->ProdName1 |= tmp;

    /* Byte 7 */
    tmp = (uint8_t)(hsd->CID[1U] & 0x000000FFU);
    pCID->ProdName2 = tmp;

    /* Byte 8 */
    tmp = (uint8_t)((hsd->CID[2U] & 0xFF000000U) >> 24U);
    pCID->ProdRev = tmp;

    /* Byte 9 */
    tmp = (uint8_t)((hsd->CID[2U] & 0x00FF0000U) >> 16U);
    pCID->ProdSN = tmp << 24U;

    /* Byte 10 */
    tmp = (uint8_t)((hsd->CID[2U] & 0x0000FF00U) >> 8U);
    pCID->ProdSN |= tmp << 16U;

    /* Byte 11 */
    tmp = (uint8_t)(hsd->CID[2U] & 0x000000FFU);
    pCID->ProdSN |= tmp << 8U;

    /* Byte 12 */
    tmp = (uint8_t)((hsd->CID[3U] & 0xFF000000U) >> 24U);
    pCID->ProdSN |= tmp;

    /* Byte 13 */
    tmp = (uint8_t)((hsd->CID[3U] & 0x00FF0000U) >> 16U);
    pCID->Reserved1   |= (tmp & 0xF0U) >> 4U;
    pCID->ManufactDate = (tmp & 0x0FU) << 8U;

    /* Byte 14 */
    tmp = (uint8_t)((hsd->CID[3U] & 0x0000FF00U) >> 8U);
    pCID->ManufactDate |= tmp;

    /* Byte 15 */
    tmp = (uint8_t)(hsd->CID[3U] & 0x000000FFU);
    pCID->CID_CRC   = (tmp & 0xFEU) >> 1U;
    pCID->Reserved2 = 1U;

    return HAL_OK;
}

/**
  * @brief  Returns information the information of the card which are stored on
  *         the CSD register.
  * @param  hsd Pointer to SD handle
  * @param  pCSD Pointer to a HAL_SD_CardCSDTypeDef structure that  
  *         contains all CSD register parameters  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_GetCardCSD(SD_HandleTypeDef *hsd, HAL_SD_CardCSDTypeDef *pCSD)
{
    uint32_t tmp = 0U;

    /* Byte 0 */
    tmp = (hsd->CSD[0U] & 0xFF000000U) >> 24U;
    pCSD->CSDStruct      = (uint8_t)((tmp & 0xC0U) >> 6U);
    pCSD->SysSpecVersion = (uint8_t)((tmp & 0x3CU) >> 2U);
    pCSD->Reserved1      = tmp & 0x03U;

    /* Byte 1 */
    tmp = (hsd->CSD[0U] & 0x00FF0000U) >> 16U;
    pCSD->TAAC = (uint8_t)tmp;

    /* Byte 2 */
    tmp = (hsd->CSD[0U] & 0x0000FF00U) >> 8U;
    pCSD->NSAC = (uint8_t)tmp;
    
    /* Byte 3 */
    tmp = hsd->CSD[0U] & 0x000000FFU;
    pCSD->MaxBusClkFrec = (uint8_t)tmp;
    
    /* Byte 4 */
    tmp = (hsd->CSD[1U] & 0xFF000000U) >> 24U;
    pCSD->CardComdClasses = (uint16_t)(tmp << 4U);
    
    /* Byte 5 */
    tmp = (hsd->CSD[1U] & 0x00FF0000U) >> 16U;
    pCSD->CardComdClasses |= (uint16_t)((tmp & 0xF0U) >> 4U);
    pCSD->RdBlockLen       = (uint8_t)(tmp & 0x0FU);
    
    /* Byte 6 */
    tmp = (hsd->CSD[1U] & 0x0000FF00U) >> 8U;
    pCSD->PartBlockRead   = (uint8_t)((tmp & 0x80U) >> 7U);
    pCSD->WrBlockMisalign = (uint8_t)((tmp & 0x40U) >> 6U);
    pCSD->RdBlockMisalign = (uint8_t)((tmp & 0x20U) >> 5U);
    pCSD->DSRImpl         = (uint8_t)((tmp & 0x10U) >> 4U);
    pCSD->Reserved2       = 0U; /*!< Reserved */

    if(hsd->SdCard.CardType == CARD_SDSC)
    {
        pCSD->DeviceSize = (tmp & 0x03U) << 10U;

        /* Byte 7 */
        tmp = (uint8_t)(hsd->CSD[1U] & 0x000000FFU);
        pCSD->DeviceSize |= (tmp) << 2U;

        /* Byte 8 */
        tmp = (uint8_t)((hsd->CSD[2U] & 0xFF000000U) >> 24U);
        pCSD->DeviceSize |= (tmp & 0xC0U) >> 6U;

        pCSD->MaxRdCurrentVDDMin = (tmp & 0x38U) >> 3U;
        pCSD->MaxRdCurrentVDDMax = (tmp & 0x07U);

        /* Byte 9 */
        tmp = (uint8_t)((hsd->CSD[2U] & 0x00FF0000U) >> 16U);
        pCSD->MaxWrCurrentVDDMin = (tmp & 0xE0U) >> 5U;
        pCSD->MaxWrCurrentVDDMax = (tmp & 0x1CU) >> 2U;
        pCSD->DeviceSizeMul      = (tmp & 0x03U) << 1U;
        /* Byte 10 */
        tmp = (uint8_t)((hsd->CSD[2U] & 0x0000FF00U) >> 8U);
        pCSD->DeviceSizeMul |= (tmp & 0x80U) >> 7U;

        hsd->SdCard.BlockNbr  = (pCSD->DeviceSize + 1U) ;
        hsd->SdCard.BlockNbr *= (1U << (pCSD->DeviceSizeMul + 2U));
        hsd->SdCard.BlockSize = 1U << (pCSD->RdBlockLen);

        hsd->SdCard.LogBlockNbr =  (hsd->SdCard.BlockNbr) * ((hsd->SdCard.BlockSize) / 512U);
        hsd->SdCard.LogBlockSize = 512U;
    }
    else if(hsd->SdCard.CardType == CARD_SDHC_SDXC)
    {
        /* Byte 7 */
        tmp = (uint8_t)(hsd->CSD[1U] & 0x000000FFU);
        pCSD->DeviceSize = (tmp & 0x3FU) << 16U;

        /* Byte 8 */
        tmp = (uint8_t)((hsd->CSD[2U] & 0xFF000000U) >> 24U);

        pCSD->DeviceSize |= (tmp << 8U);

        /* Byte 9 */
        tmp = (uint8_t)((hsd->CSD[2U] & 0x00FF0000U) >> 16U);

        pCSD->DeviceSize |= (tmp);

        /* Byte 10 */
        tmp = (uint8_t)((hsd->CSD[2U] & 0x0000FF00U) >> 8U);

        hsd->SdCard.LogBlockNbr = hsd->SdCard.BlockNbr = (((uint64_t)pCSD->DeviceSize + 1U) * 1024U);
        hsd->SdCard.LogBlockSize = hsd->SdCard.BlockSize = 512U;
    }
    else
    {
        /* Clear all the static flags */
        __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
        hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
        hsd->State = HAL_SD_STATE_READY;
        return HAL_ERROR;
    }
    
    pCSD->EraseGrSize = (tmp & 0x40U) >> 6U;
    pCSD->EraseGrMul  = (tmp & 0x3FU) << 1U;
    
    /* Byte 11 */
    tmp = (uint8_t)(hsd->CSD[2U] & 0x000000FFU);
    pCSD->EraseGrMul     |= (tmp & 0x80U) >> 7U;
    pCSD->WrProtectGrSize = (tmp & 0x7FU);
    
    /* Byte 12 */
    tmp = (uint8_t)((hsd->CSD[3U] & 0xFF000000U) >> 24U);
    pCSD->WrProtectGrEnable = (tmp & 0x80U) >> 7U;
    pCSD->ManDeflECC        = (tmp & 0x60U) >> 5U;
    pCSD->WrSpeedFact       = (tmp & 0x1CU) >> 2U;
    pCSD->MaxWrBlockLen     = (tmp & 0x03U) << 2U;
    
    /* Byte 13 */
    tmp = (uint8_t)((hsd->CSD[3U] & 0x00FF0000U) >> 16U);
    pCSD->MaxWrBlockLen      |= (tmp & 0xC0U) >> 6U;
    pCSD->WriteBlockPaPartial = (tmp & 0x20U) >> 5U;
    pCSD->Reserved3           = 0U;
    pCSD->ContentProtectAppli = (tmp & 0x01U);
    
    /* Byte 14 */
    tmp = (uint8_t)((hsd->CSD[3U] & 0x0000FF00U) >> 8U);
    pCSD->FileFormatGrouop = (tmp & 0x80U) >> 7U;
    pCSD->CopyFlag         = (tmp & 0x40U) >> 6U;
    pCSD->PermWrProtect    = (tmp & 0x20U) >> 5U;
    pCSD->TempWrProtect    = (tmp & 0x10U) >> 4U;
    pCSD->FileFormat       = (tmp & 0x0CU) >> 2U;
    pCSD->ECC              = (tmp & 0x03U);
    
    /* Byte 15 */
    tmp = (uint8_t)(hsd->CSD[3U] & 0x000000FFU);
    pCSD->CSD_CRC   = (tmp & 0xFEU) >> 1U;
    pCSD->Reserved4 = 1U;
    
    return HAL_OK;
}

/**
  * @brief  Gets the SD status info.
  * @param  hsd Pointer to SD handle      
  * @param  pStatus Pointer to the HAL_SD_CardStatusTypeDef structure that 
  *         will contain the SD card status information 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_GetCardStatus(SD_HandleTypeDef *hsd, HAL_SD_CardStatusTypeDef *pStatus)
{
    uint32_t tmp = 0U;
    uint32_t sd_status[16U];
    uint32_t errorstate = HAL_SD_ERROR_NONE;
    
    errorstate = SD_SendSDStatus(hsd, sd_status);
    if(errorstate != HAL_OK)
    {
        /* Clear all the static flags */
        __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
        hsd->ErrorCode |= errorstate;
        hsd->State = HAL_SD_STATE_READY;
        return HAL_ERROR;
    }
    else
    {
        /* Byte 0 */
        tmp = (sd_status[0U] & 0xC0U) >> 6U;
        pStatus->DataBusWidth = (uint8_t)tmp;

        /* Byte 0 */
        tmp = (sd_status[0U] & 0x20U) >> 5U;
        pStatus->SecuredMode = (uint8_t)tmp;

        /* Byte 2 */
        tmp = (sd_status[0U] & 0x00FF0000U) >> 16U;
        pStatus->CardType = (uint16_t)(tmp << 8U);

        /* Byte 3 */
        tmp = (sd_status[0U] & 0xFF000000U) >> 24U;
        pStatus->CardType |= (uint16_t)tmp;

        /* Byte 4 */
        tmp = (sd_status[1U] & 0xFFU);
        pStatus->ProtectedAreaSize = (uint32_t)(tmp << 24U);

        /* Byte 5 */
        tmp = (sd_status[1U] & 0xFF00U) >> 8U;
        pStatus->ProtectedAreaSize |= (uint32_t)(tmp << 16U);

        /* Byte 6 */
        tmp = (sd_status[1U] & 0xFF0000U) >> 16U;
        pStatus->ProtectedAreaSize |= (uint32_t)(tmp << 8U);

        /* Byte 7 */
        tmp = (sd_status[1U] & 0xFF000000U) >> 24U;
        pStatus->ProtectedAreaSize |= (uint32_t)tmp;

        /* Byte 8 */
        tmp = (sd_status[2U] & 0xFFU);
        pStatus->SpeedClass = (uint8_t)tmp;

        /* Byte 9 */
        tmp = (sd_status[2U] & 0xFF00U) >> 8U;
        pStatus->PerformanceMove = (uint8_t)tmp;

        /* Byte 10 */
        tmp = (sd_status[2U] & 0xF00000U) >> 20U;
        pStatus->AllocationUnitSize = (uint8_t)tmp;

        /* Byte 11 */
        tmp = (sd_status[2U] & 0xFF000000U) >> 24U;
        pStatus->EraseSize = (uint16_t)(tmp << 8U);

        /* Byte 12 */
        tmp = (sd_status[3U] & 0xFFU);
        pStatus->EraseSize |= (uint16_t)tmp;

        /* Byte 13 */
        tmp = (sd_status[3U] & 0xFC00U) >> 10U;
        pStatus->EraseTimeout = (uint8_t)tmp;

        /* Byte 13 */
        tmp = (sd_status[3U] & 0x0300U) >> 8U;
        pStatus->EraseOffset = (uint8_t)tmp;
    }
    
    return HAL_OK;
}

/**
  * @brief  Gets the SD card info.
  * @param  hsd Pointer to SD handle      
  * @param  pCardInfo Pointer to the HAL_SD_CardInfoTypeDef structure that 
  *         will contain the SD card status information 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_GetCardInfo(SD_HandleTypeDef *hsd, HAL_SD_CardInfoTypeDef *pCardInfo)
{
    pCardInfo->CardType     = (uint32_t)(hsd->SdCard.CardType);
    pCardInfo->CardVersion  = (uint32_t)(hsd->SdCard.CardVersion);
    pCardInfo->Class        = (uint32_t)(hsd->SdCard.Class);
    pCardInfo->RelCardAdd   = (uint32_t)(hsd->SdCard.RelCardAdd);
    pCardInfo->BlockNbr     = (uint32_t)(hsd->SdCard.BlockNbr);
    pCardInfo->BlockSize    = (uint32_t)(hsd->SdCard.BlockSize);
    pCardInfo->LogBlockNbr  = (uint32_t)(hsd->SdCard.LogBlockNbr);
    pCardInfo->LogBlockSize = (uint32_t)(hsd->SdCard.LogBlockSize);

    return HAL_OK;
}

/**
  * @brief  Enables wide bus operation for the requested card if supported by 
  *         card.
  * @param  hsd Pointer to SD handle       
  * @param  WideMode Specifies the SD card wide bus mode 
  *          This parameter can be one of the following values:
  *            @arg SDIO_BUS_WIDE_8B: 8-bit data transfer
  *            @arg SDIO_BUS_WIDE_4B: 4-bit data transfer
  *            @arg SDIO_BUS_WIDE_1B: 1-bit data transfer
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_ConfigWideBusOperation(SD_HandleTypeDef *hsd, uint32_t WideMode)
{
    SDIO_InitTypeDef Init;
    uint32_t errorstate = HAL_SD_ERROR_NONE;

    /* Check the parameters */
    assert_param(IS_SDIO_BUS_WIDE(WideMode));
  
    /* Chnage Satte */
    hsd->State = HAL_SD_STATE_BUSY;

    if(hsd->SdCard.CardType != CARD_SECURED)
    {
        if(WideMode == SDIO_BUS_WIDE_8B)
        {
            hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
        }
        else if(WideMode == SDIO_BUS_WIDE_4B)
        {
            errorstate = SD_WideBus_Enable(hsd);
            hsd->ErrorCode |= errorstate;
        }
        else if(WideMode == SDIO_BUS_WIDE_1B)
        {
            errorstate = SD_WideBus_Disable(hsd);
            hsd->ErrorCode |= errorstate;
        }
        else
        {
            /* WideMode is not a valid argument*/
            hsd->ErrorCode |= HAL_SD_ERROR_PARAM;
        }
    }
    else
    {
        /* MMC Card does not support this feature */
        hsd->ErrorCode |= HAL_SD_ERROR_UNSUPPORTED_FEATURE;
    }
  
    if(hsd->ErrorCode != HAL_SD_ERROR_NONE)
    {
        /* Clear all the static flags */
        __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
        hsd->State = HAL_SD_STATE_READY;
        return HAL_ERROR;
    }
    else
    {
        /* Configure the SDIO peripheral */
        Init.SpeedMode  = hsd->Init.SpeedMode;
        Init.ClockMode  = hsd->Init.ClockMode;
        Init.BusWide    = WideMode;
        Init.ClockDiv   = hsd->Init.ClockDiv;
        SDIO_Init(hsd->Instance, Init);
    }

    /* Change State */
    hsd->State = HAL_SD_STATE_READY;

    return HAL_OK;
}


/**
  * @brief  Gets the current sd card data state.
  * @param  hsd pointer to SD handle
  * @retval Card state
  */
HAL_SD_CardStateTypeDef HAL_SD_GetCardState(SD_HandleTypeDef *hsd)
{
    HAL_SD_CardStateTypeDef cardstate =  HAL_SD_CARD_TRANSFER;
    uint32_t errorstate = HAL_SD_ERROR_NONE;
    uint32_t resp0 = 0;

    errorstate = SD_SendStatus(hsd, &resp0);
    if(errorstate != HAL_OK)
    {
        hsd->ErrorCode |= errorstate;
    }
    cardstate = (HAL_SD_CardStateTypeDef)((resp0 >> 9U) & 0x0FU);
    return cardstate;
}

/**
  * @brief  Abort the current transfer and disable the SD.
  * @param  hsd pointer to a SD_HandleTypeDef structure that contains
  *                the configuration information for SD module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_Abort(SD_HandleTypeDef *hsd)
{
    UNUSED(hsd);
    return HAL_OK;
}

/**
  * @brief  Abort the current transfer and disable the SD (IT mode).
  * @param  hsd pointer to a SD_HandleTypeDef structure that contains
  *                the configuration information for SD module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SD_Abort_IT(SD_HandleTypeDef *hsd)
{
    UNUSED(hsd);
    return HAL_OK;
}
  
/* Private function ----------------------------------------------------------*/  
/** @addtogroup SD_Private_Functions
  * @{
  */

/**
  * @brief  DMA SD transmit process complete callback 
  * @param  hdma DMA handle
  * @retval None
  */
static void SD_DMATransmitCplt(DMA_HandleTypeDef *hdma)     
{
    SD_HandleTypeDef* hsd = (SD_HandleTypeDef* )(hdma->Parent);
    /* Enable DATAEND Interrupt */
    __HAL_SD_ENABLE_IT(hsd, (SD_IT_TC));
}

/**
  * @brief  DMA SD receive process complete callback 
  * @param  hdma DMA handle
  * @retval None
  */
static void SD_DMAReceiveCplt(DMA_HandleTypeDef *hdma)  
{
    SD_HandleTypeDef* hsd = (SD_HandleTypeDef* )(hdma->Parent);
    uint32_t errorstate = HAL_SD_ERROR_NONE;

    /* Send stop command in multiblock write */
    if(hsd->Context == (SD_CONTEXT_READ_MULTIPLE_BLOCK | SD_CONTEXT_DMA))
    {
        errorstate = SDMMC_CmdStopTransfer(hsd->Instance);
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            hsd->ErrorCode |= errorstate;
#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)
            hsd->ErrorCallback(hsd);
#else
            HAL_SD_ErrorCallback(hsd);
#endif
        }
    }
    /* Disable the DMA transfer for transmit request by setting the DMAEN bit
      in the SD DCTRL register */
    __HAL_SD_DMA_DISABLE(hsd);
    /* Clear all the static flags */
    __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
    hsd->State = HAL_SD_STATE_READY;
#if (USE_HAL_SD_REGISTER_CALLBACKS == 1)
    hsd->RxCpltCallback(hsd);
#else
    HAL_SD_RxCpltCallback(hsd);
#endif
}

/**
  * @brief  DMA SD communication error callback 
  * @param  hdma DMA handle
  * @retval None
  */
static void SD_DMAError(DMA_HandleTypeDef *hdma)   
{
    UNUSED(hdma);
}

/**
  * @brief  DMA SD Tx Abort callback 
  * @param  hdma DMA handle
  * @retval None
  */
static void SD_DMATxAbort(DMA_HandleTypeDef *hdma)   
{
    UNUSED(hdma);
}

/**
  * @brief  DMA SD Rx Abort callback 
  * @param  hdma DMA handle
  * @retval None
  */
static void SD_DMARxAbort(DMA_HandleTypeDef *hdma)   
{
    UNUSED(hdma);
}


/**
  * @brief  Initializes the sd card.
  * @param  hsd Pointer to SD handle
  * @retval SD Card error state
  */
static uint32_t SD_InitCard(SD_HandleTypeDef *hsd)
{
    HAL_SD_CardCSDTypeDef CSD;
    uint32_t errorstate = HAL_SD_ERROR_NONE;
    uint16_t sd_rca = 1U;
  
    /* Check the power State */
    if(SDIO_GetPowerState(hsd->Instance) == 0U)
    {
        /* Power off */
        return HAL_SD_ERROR_REQUEST_NOT_APPLICABLE;
    }
    if(hsd->SdCard.CardType != CARD_SECURED)
    {
        /* Send CMD2 ALL_SEND_CID */
        errorstate = SDMMC_CmdSendCID(hsd->Instance);
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            return errorstate;
        }
        else
        {
            /* Get Card identification number data */
            SDIO_GetResponse136(hsd->Instance, hsd->CID);
        }
    }
    if(hsd->SdCard.CardType != CARD_SECURED)
    {
        /* Send CMD3 SET_REL_ADDR with argument 0 */
        /* SD Card publishes its RCA. */
        errorstate = SDMMC_CmdSetRelAdd(hsd->Instance, &sd_rca);
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            return errorstate;
        }
    }
    if(hsd->SdCard.CardType != CARD_SECURED)
    {
        /* Get the SD card RCA */
        hsd->SdCard.RelCardAdd = sd_rca;

        /* Send CMD9 SEND_CSD with argument as card's RCA */
        errorstate = SDMMC_CmdSendCSD(hsd->Instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U));
        if(errorstate != HAL_SD_ERROR_NONE)
        {
            return errorstate;
        }
        else
        {
            /* Get Card Specific Data */
            SDIO_GetResponse136(hsd->Instance, hsd->CSD);
        }
    }
    /* Get the Card Class */
    hsd->SdCard.Class = (SDIO_GetResponse(hsd->Instance, SDIO_RESP2) >> 20U);
    /* Get CSD parameters */
    HAL_SD_GetCardCSD(hsd, &CSD);
    /* Select the Card */
    errorstate = SDMMC_CmdSelDesel(hsd->Instance, (uint32_t)(((uint32_t)hsd->SdCard.RelCardAdd) << 16U));
    if(errorstate != HAL_SD_ERROR_NONE)
    {
        return errorstate;
    }
    /* Configure SDIO peripheral interface */
    SDIO_Init(hsd->Instance, hsd->Init);
    /* All cards are initialized */
    return HAL_SD_ERROR_NONE;
}

/**
  * @brief  Enquires cards about their operating voltage and configures clock
  *         controls and stores SD information that will be needed in future
  *         in the SD handle.
  * @param  hsd Pointer to SD handle
  * @retval error state
  */
static uint32_t SD_PowerON(SD_HandleTypeDef *hsd)
{
    __IO uint32_t count = 0U;
    uint32_t response = 0U, validvoltage = 0U;
    uint32_t errorstate = HAL_SD_ERROR_NONE;
  
    /* CMD0: GO_IDLE_STATE */
    errorstate = SDMMC_CmdGoIdleState(hsd->Instance);
    if(errorstate != HAL_SD_ERROR_NONE)
    {
        return errorstate;
    }
    /* CMD8: SEND_IF_COND: Command available only on V2.0 cards */
    errorstate = SDMMC_CmdOperCond(hsd->Instance);
    if(errorstate != HAL_SD_ERROR_NONE)
    {
        hsd->SdCard.CardVersion = CARD_V1_X;
        /* Send ACMD41 SD_APP_OP_COND with Argument 0x80100000 */
        while(validvoltage == 0U)
        {
            if(count++ == SDMMC_MAX_VOLT_TRIAL)
            {
                return HAL_SD_ERROR_INVALID_VOLTRANGE;
            }
            /* SEND CMD55 APP_CMD with RCA as 0 */
            errorstate = SDMMC_CmdAppCommand(hsd->Instance, 0U);
            if(errorstate != HAL_SD_ERROR_NONE)
            {
                return HAL_SD_ERROR_UNSUPPORTED_FEATURE;
            }
            /* Send CMD41 */
            errorstate = SDMMC_CmdAppOperCommand(hsd->Instance, SDMMC_STD_CAPACITY);
            if(errorstate != HAL_SD_ERROR_NONE)
            {
                return HAL_SD_ERROR_UNSUPPORTED_FEATURE;
            }
            /* Get command response */
            response = SDIO_GetResponse(hsd->Instance, SDIO_RESP0);
            /* Get operating voltage*/
            validvoltage = (((response >> 31U) == 1U) ? 1U : 0U);
        }
        /* Card type is SDSC */
        hsd->SdCard.CardType = CARD_SDSC;
    }
    else
    {
        hsd->SdCard.CardVersion = CARD_V2_X;
        /* Send ACMD41 SD_APP_OP_COND with Argument 0x80100000 */
        while(validvoltage == 0U)
        {
            if(count++ == SDMMC_MAX_VOLT_TRIAL)
            {
                return HAL_SD_ERROR_INVALID_VOLTRANGE;
            }
            /* SEND CMD55 APP_CMD with RCA as 0 , 0x120*/
            errorstate = SDMMC_CmdAppCommand(hsd->Instance, 0U);
            if(errorstate != HAL_SD_ERROR_NONE)
            {
                return errorstate;
            }
            /* Send CMD41 */
            errorstate = SDMMC_CmdAppOperCommand(hsd->Instance, SDMMC_HIGH_CAPACITY);
            if(errorstate != HAL_SD_ERROR_NONE)
            {
                return errorstate;
            }
            /* Get command response */
            response = SDIO_GetResponse(hsd->Instance, SDIO_RESP0);
            /* Get operating voltage*/
            validvoltage = (((response >> 31U) == 1U) ? 1U : 0U);
        }
        if((response & SDMMC_HIGH_CAPACITY) == SDMMC_HIGH_CAPACITY) /* (response &= SD_HIGH_CAPACITY) */
        {
            hsd->SdCard.CardType = CARD_SDHC_SDXC;
        }
        else
        {
            hsd->SdCard.CardType = CARD_SDSC;
        }
    }
    return HAL_SD_ERROR_NONE;
}

/**
  * @brief  Turns the SDIO output signals off.
  * @param  hsd Pointer to SD handle
  * @retval HAL status
  */
static HAL_StatusTypeDef SD_PowerOFF(SD_HandleTypeDef *hsd)
{
    /* Set Power State to OFF */
    SDIO_PowerState_OFF(hsd->Instance);

    return HAL_OK;
}

/**
  * @brief  Send Status info command.
  * @param  hsd pointer to SD handle
  * @param  pSDstatus Pointer to the buffer that will contain the SD card status 
  *         SD Status register)
  * @retval error state
  */
static uint32_t SD_SendSDStatus(SD_HandleTypeDef *hsd, uint32_t *pSDstatus)
{
    SDIO_DataInitTypeDef config;
    uint32_t errorstate = HAL_SD_ERROR_NONE;
    uint32_t tickstart = HAL_GetTick();
    uint32_t count = 0U;
    uint32_t read_bytes = 0;
  
    /* Check SD response */
    if((SDIO_GetResponse(hsd->Instance, SDIO_RESP0) & SDMMC_CARD_LOCKED) == SDMMC_CARD_LOCKED)
    {
        return HAL_SD_ERROR_LOCK_UNLOCK_FAILED;
    }
    /* Set block size for card if it is not equal to current block size for card */
    errorstate = SDMMC_CmdBlockLength(hsd->Instance, 64U);
    if(errorstate != HAL_SD_ERROR_NONE)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_NONE;
        return errorstate;
    }
    /* Send CMD55 */
    errorstate = SDMMC_CmdAppCommand(hsd->Instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U));
    if(errorstate != HAL_SD_ERROR_NONE)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_NONE;
        return errorstate;
    }
    /* Configure the SD DPSM (Data Path State Machine) */
    config.DataTimeOut   = SDMMC_DATATIMEOUT;
    config.DataLength    = 64U;
    config.DataBlockSize = SDIO_DATABLOCK_SIZE_64B;
    SDIO_ConfigData(hsd->Instance, &config);
    /* Send ACMD13 (SD_APP_STAUS)  with argument as card's RCA */
    errorstate = SDMMC_CmdStatusRegister(hsd->Instance);
    if(errorstate != HAL_SD_ERROR_NONE)
    {
        hsd->ErrorCode |= HAL_SD_ERROR_NONE;
        return errorstate;
    }
    /* Get status data */
    if(__HAL_SD_GET_IT_STATE_EN(hsd, SD_IT_STATE_BRR))
    {
        do
        {
            /* Wait for SRS12.BRR */
            while(!__HAL_SD_GET_FLAG(hsd, SD_FLAG_BRR | SD_FLAG_EDCRC | SD_FLAG_EDT))
            {
                if((HAL_GetTick() - tickstart) >=  SDIO_CMDTIMEOUT)
                {
                    /* Clear all the static flags */
                    __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                    hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;
                    hsd->State= HAL_SD_STATE_READY;
                    return HAL_TIMEOUT;
                }
            }
            /* Get error state */
            if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_EDT))
            {
                /* Clear all the static flags */
                __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                hsd->ErrorCode |= HAL_SD_ERROR_DATA_TIMEOUT;
                hsd->State = HAL_SD_STATE_READY;
                return HAL_ERROR;
            }
            else if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_EDCRC))
            {
                /* Clear all the static flags */
                __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                hsd->ErrorCode |= HAL_SD_ERROR_DATA_CRC_FAIL;
                hsd->State = HAL_SD_STATE_READY;
                return HAL_ERROR;
            }
            /* Check BRR */
            if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_BRR))
            {
                __HAL_SD_CLEAR_FLAG(hsd, SD_FLAG_BRR);
                /* Read data from SD Rx FIFO */
                for(count = 0U; count < config.DataBlockSize/4; count++)
                {
                    SDIO_ReadFIFO(hsd->Instance, pSDstatus++);
                }
                read_bytes += config.DataBlockSize;
            }
        }while(__HAL_SD_Get_BLOCK_COUNT(hsd) > 0 && read_bytes < config.DataLength);
    }
    else
    {
        do
        {
            //Buffer Read Enable
            while(!__HAL_SD_GET_STATE(hsd, SD_STATE_BRE))
            {
                /* Get error state */
                if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_EDT))
                {
                    /* Clear all the static flags */
                    __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                    hsd->ErrorCode |= HAL_SD_ERROR_DATA_TIMEOUT;
                    hsd->State = HAL_SD_STATE_READY;
                    return HAL_ERROR;
                }
                else if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_EDCRC))
                {
                    /* Clear all the static flags */
                    __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                    hsd->ErrorCode |= HAL_SD_ERROR_DATA_CRC_FAIL;
                    hsd->State = HAL_SD_STATE_READY;
                    return HAL_ERROR;
                }
                if((HAL_GetTick() - tickstart) >=  SDIO_CMDTIMEOUT)
                {
                    /* Clear all the static flags */
                    __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                    hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;
                    hsd->State= HAL_SD_STATE_READY;
                    return HAL_TIMEOUT;
                }
            }
            /* Read data from SD Rx FIFO */
            for(count = 0U; count < config.DataBlockSize/4; count++)
            {
                SDIO_ReadFIFO(hsd->Instance, pSDstatus++);
            }
            read_bytes += config.DataBlockSize;
        }while(__HAL_SD_Get_BLOCK_COUNT(hsd) > 0 && read_bytes < config.DataLength);
    }
    /* Clear all the static status flags*/
    __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
    return HAL_SD_ERROR_NONE;
}

/**
  * @brief  Returns the current card's status.
  * @param  hsd Pointer to SD handle
  * @param  pCardStatus pointer to the buffer that will contain the SD card 
  *         status (Card Status register)  
  * @retval error state
  */
static uint32_t SD_SendStatus(SD_HandleTypeDef *hsd, uint32_t *pCardStatus)
{
    uint32_t errorstate = HAL_SD_ERROR_NONE;
    if(pCardStatus == NULL)
    {
        return HAL_SD_ERROR_PARAM;
    }
    /* Send Status command */
    errorstate = SDMMC_CmdSendStatus(hsd->Instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U));
    if(errorstate != HAL_OK)
    {
        return errorstate;
    }
    /* Get SD card status */
    *pCardStatus = SDIO_GetResponse(hsd->Instance, SDIO_RESP0);
    return HAL_SD_ERROR_NONE;
}

/**
  * @brief  Enables the SDIO wide bus mode.
  * @param  hsd pointer to SD handle
  * @retval error state
  */
static uint32_t SD_WideBus_Enable(SD_HandleTypeDef *hsd)
{
    uint32_t scr[2U] = {0U, 0U};
    uint32_t errorstate = HAL_SD_ERROR_NONE;

    if((SDIO_GetResponse(hsd->Instance, SDIO_RESP0) & SDMMC_CARD_LOCKED) == SDMMC_CARD_LOCKED)
    {
        return HAL_SD_ERROR_LOCK_UNLOCK_FAILED;
    }
    /* Get SCR Register */
    errorstate = SD_FindSCR(hsd, scr);
    if(errorstate != HAL_OK)
    {
        return errorstate;
    }
    /* If requested card supports wide bus operation */
    if((scr[1U] & SDMMC_WIDE_BUS_SUPPORT) != SDMMC_ALLZERO)
    {
        /* Send CMD55 APP_CMD with argument as card's RCA.*/
        errorstate = SDMMC_CmdAppCommand(hsd->Instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U));
        if(errorstate != HAL_OK)
        {
            return errorstate;
        }
        /* Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
        errorstate = SDMMC_CmdBusWidth(hsd->Instance, 2U);
        if(errorstate != HAL_OK)
        {
            return errorstate;
        }
        return HAL_SD_ERROR_NONE;
    }
    else
    {
        return HAL_SD_ERROR_REQUEST_NOT_APPLICABLE;
    }
}

/**
  * @brief  Disables the SDIO wide bus mode.
  * @param  hsd Pointer to SD handle
  * @retval error state
  */
static uint32_t SD_WideBus_Disable(SD_HandleTypeDef *hsd)
{
    uint32_t scr[2U] = {0U, 0U};
    uint32_t errorstate = HAL_SD_ERROR_NONE;

    if((SDIO_GetResponse(hsd->Instance, SDIO_RESP0) & SDMMC_CARD_LOCKED) == SDMMC_CARD_LOCKED)
    {
        return HAL_SD_ERROR_LOCK_UNLOCK_FAILED;
    }
    /* Get SCR Register */
    errorstate = SD_FindSCR(hsd, scr);
    if(errorstate != HAL_OK)
    {
        return errorstate;
    }
    /* If requested card supports 1 bit mode operation */
    if((scr[1U] & SDMMC_SINGLE_BUS_SUPPORT) != SDMMC_ALLZERO)
    {
        /* Send CMD55 APP_CMD with argument as card's RCA */
        errorstate = SDMMC_CmdAppCommand(hsd->Instance, (uint32_t)(hsd->SdCard.RelCardAdd << 16U));
        if(errorstate != HAL_OK)
        {
            return errorstate;
        }
        /* Send ACMD6 APP_CMD with argument as 0 for single bus mode */
        errorstate = SDMMC_CmdBusWidth(hsd->Instance, 0U);
        if(errorstate != HAL_OK)
        {
            return errorstate;
        }
        return HAL_SD_ERROR_NONE;
    }
    else
    {
        return HAL_SD_ERROR_REQUEST_NOT_APPLICABLE;
    }
}
  
  
/**
  * @brief  Finds the SD card SCR register value.
  * @param  hsd Pointer to SD handle
  * @param  pSCR pointer to the buffer that will contain the SCR value  
  * @retval error state
  */
static uint32_t SD_FindSCR(SD_HandleTypeDef *hsd, uint32_t *pSCR)
{
    SDIO_DataInitTypeDef config;
    uint32_t errorstate = HAL_SD_ERROR_NONE;
    uint32_t tickstart = HAL_GetTick();
    uint32_t index = 0U;
    uint32_t tempscr[2U] = {0U, 0U};
    uint32_t read_bytes = 0;

    /* Set Block Size To 8 Bytes */
//    errorstate = SDMMC_CmdBlockLength(hsd->Instance, 8U);
    if(errorstate != HAL_OK)
    {
        return errorstate;
    }
    /* Send CMD55 APP_CMD with argument as card's RCA */
    errorstate = SDMMC_CmdAppCommand(hsd->Instance, (uint32_t)((hsd->SdCard.RelCardAdd) << 16U));
    if(errorstate != HAL_OK)
    {
        return errorstate;
    }
    config.DataTimeOut   = SDMMC_DATATIMEOUT;
    config.DataLength    = 8U;
    config.DataBlockSize = SDIO_DATABLOCK_SIZE_8B;
    SDIO_ConfigData(hsd->Instance, &config);
    /* Send ACMD51 SD_APP_SEND_SCR with argument as 0 */
    errorstate = SDMMC_CmdSendSCR(hsd->Instance);
    if(errorstate != HAL_OK)
    {
        return errorstate;
    }
    /* Get status data */
    if(__HAL_SD_GET_IT_STATE_EN(hsd, SD_IT_STATE_BRR))
    {
        while(read_bytes < config.DataLength)
        {
            /* Wait for SRS12.BRR */
            while(!__HAL_SD_GET_FLAG(hsd, SD_FLAG_BRR | SD_FLAG_EDCRC | SD_FLAG_EDT))
            {
                if((HAL_GetTick() - tickstart) >=  SDIO_CMDTIMEOUT*10)
                {
                    /* Clear all the static flags */
                    __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                    hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;
                    hsd->State= HAL_SD_STATE_READY;
                    return HAL_TIMEOUT;
                }
            }
            /* Get error state */
            if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_EDT))
            {
                /* Clear all the static flags */
                __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                hsd->ErrorCode |= HAL_SD_ERROR_DATA_TIMEOUT;
                hsd->State = HAL_SD_STATE_READY;
                return HAL_ERROR;
            }
            else if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_EDCRC))
            {
                /* Clear all the static flags */
                __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                hsd->ErrorCode |= HAL_SD_ERROR_DATA_CRC_FAIL;
                hsd->State = HAL_SD_STATE_READY;
                return HAL_ERROR;
            }
            /* Check BRR */
            if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_BRR))
            {
                /* Read data from SD Rx FIFO */
                for(uint32_t count = 0U; count < config.DataBlockSize/4; count++)
                {
                    SDIO_ReadFIFO(hsd->Instance, &tempscr[count]);
                }
                read_bytes += config.DataBlockSize;
                __HAL_SD_CLEAR_FLAG(hsd, SD_FLAG_BRR);
            }
        }
    }
    else
    {
        while(read_bytes < config.DataLength)
        {
            //Buffer Read Enable
            while(!__HAL_SD_GET_STATE(hsd, SD_STATE_BRE))
            {
                /* Get error state */
                if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_EDT))
                {
                    /* Clear all the static flags */
                    __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                    hsd->ErrorCode |= HAL_SD_ERROR_DATA_TIMEOUT;
                    hsd->State = HAL_SD_STATE_READY;
                    return HAL_ERROR;
                }
                else if(__HAL_SD_GET_FLAG(hsd, SD_FLAG_EDCRC))
                {
                    /* Clear all the static flags */
                    __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                    hsd->ErrorCode |= HAL_SD_ERROR_DATA_CRC_FAIL;
                    hsd->State = HAL_SD_STATE_READY;
                    return HAL_ERROR;
                }
                if((HAL_GetTick() - tickstart) >=  SDIO_CMDTIMEOUT*10)
                {
                    /* Clear all the static flags */
                    __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
                    hsd->ErrorCode |= HAL_SD_ERROR_TIMEOUT;
                    hsd->State= HAL_SD_STATE_READY;
                    return HAL_TIMEOUT;
                }
            }
            /* Read data from SD Rx FIFO */
            for(uint32_t count = 0U; count < config.DataBlockSize/4; count++)
            {
                SDIO_ReadFIFO(hsd->Instance, &tempscr[count]);
            }
            read_bytes += config.DataBlockSize;
        }
    }
    /* No error flag set */
    /* Clear all the static flags */
    __HAL_SD_CLEAR_FLAG(hsd, SD_STATIC_FLAGS);
    *(pSCR + 1U) = ((tempscr[0U] & SDMMC_0TO7BITS) << 24U)  | ((tempscr[0U] & SDMMC_8TO15BITS) << 8U) |\
                   ((tempscr[0U] & SDMMC_16TO23BITS) >> 8U) | ((tempscr[0U] & SDMMC_24TO31BITS) >> 24U);
    
    *(pSCR) = ((tempscr[1U] & SDMMC_0TO7BITS) << 24U)  | ((tempscr[1U] & SDMMC_8TO15BITS) << 8U) |\
              ((tempscr[1U] & SDMMC_16TO23BITS) >> 8U) | ((tempscr[1U] & SDMMC_24TO31BITS) >> 24U);
    return HAL_SD_ERROR_NONE;
}

/**
  * @brief  Wrap up reading in non-blocking mode.
  * @param  hsd pointer to a SD_HandleTypeDef structure that contains
  *              the configuration information.
  * @retval HAL status
  */
static HAL_StatusTypeDef SD_Read_IT(SD_HandleTypeDef *hsd)
{
    uint32_t count = 0U;
    uint32_t read_cnt = hsd->Config.DataBlockSize / 4;
    uint32_t* tmp;

    tmp = (uint32_t*)hsd->pRxBuffPtr;
    __HAL_SD_READ_WAIT_ENABLE(hsd);
    /* Read data from SDIO Rx FIFO */
    for(count = 0U; count < read_cnt; count++)
    {
        SDIO_ReadFIFO(hsd->Instance, tmp + count);
    }
    hsd->pRxBuffPtr += read_cnt;
    __HAL_SD_READ_WAIT_DISABLE(hsd);
    return HAL_OK;
}

/**
  * @brief  Wrap up writing in non-blocking mode.
  * @param  hsd pointer to a SD_HandleTypeDef structure that contains
  *              the configuration information.
  * @retval HAL status
  */
static HAL_StatusTypeDef SD_Write_IT(SD_HandleTypeDef *hsd)
{
    uint32_t count = 0U;
    uint32_t write_cnt = hsd->Config.DataBlockSize / 4;
    uint32_t* tmp;

    tmp = (uint32_t*)hsd->pTxBuffPtr;
    /* Write data to SDIO Tx FIFO */
    for(count = 0U; count < write_cnt; count++)
    {
        SDIO_WriteFIFO(hsd->Instance, (tmp + count));
    }
    hsd->pTxBuffPtr += write_cnt;
    return HAL_OK;
}

#endif /* HAL_SD_MODULE_ENABLED */

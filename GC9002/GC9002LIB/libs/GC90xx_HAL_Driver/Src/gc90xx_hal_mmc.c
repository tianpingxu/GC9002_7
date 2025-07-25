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

#ifdef HAL_MMC_MODULE_ENABLED

/** @addtogroup gc90xx_HAL_Driver
  * @{
  */
/** @addtogroup MMC 
  * @{
  */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup MMC_Private_Functions MMC Private Functions
  * @{
  */
static uint32_t MMC_InitCard(MMC_HandleTypeDef *hmmc);
static uint32_t MMC_PowerON(MMC_HandleTypeDef *hmmc);                      
static uint32_t MMC_SendStatus(MMC_HandleTypeDef *hmmc, uint32_t *pCardStatus);
static HAL_StatusTypeDef MMC_PowerOFF(MMC_HandleTypeDef *hmmc);
static HAL_StatusTypeDef MMC_Write_IT(MMC_HandleTypeDef *hmmc);
static HAL_StatusTypeDef MMC_Read_IT(MMC_HandleTypeDef *hmmc);
static void MMC_DMATransmitCplt(DMA_HandleTypeDef *hdma);
static void MMC_DMAReceiveCplt(DMA_HandleTypeDef *hdma);
static void MMC_DMAError(DMA_HandleTypeDef *hdma);
static void MMC_DMATxAbort(DMA_HandleTypeDef *hdma);
static void MMC_DMARxAbort(DMA_HandleTypeDef *hdma);

/* Exported functions --------------------------------------------------------*/
/** @addtogroup MMC_Exported_Functions
  * @{
  */

/**
  * @brief  Initializes the MMC according to the specified parameters in the 
            MMC_HandleTypeDef and create the associated handle.
  * @param  hmmc Pointer to the MMC handle  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_Init(MMC_HandleTypeDef *hmmc)
{
    /* Check the MMC handle allocation */
    if(hmmc == NULL)
    {
        return HAL_ERROR;
    }

    /* Check the parameters */
    assert_param(IS_SDIO_ALL_INSTANCE(hmmc->Instance));
    assert_param(IS_SDIO_SPEED_MODE(hmmc->Init.SpeedMode));
    assert_param(IS_SDIO_CLOCK_MODE(hmmc->Init.ClockMode));
    assert_param(IS_SDIO_BUS_WIDE(hmmc->Init.BusWide));
    assert_param(IS_SDIO_CLKDIV(hmmc->Init.ClockDiv));

    if(hmmc->State == HAL_MMC_STATE_RESET)
    {
        /* Allocate lock resource and initialize it */
        hmmc->Lock = HAL_UNLOCKED;
#if (USE_HAL_MMC_REGISTER_CALLBACKS == 1)
        /* Reset Callback pointers in HAL_MMC_STATE_RESET only */
        hmmc->TxCpltCallback    = HAL_MMC_TxCpltCallback;
        hmmc->RxCpltCallback    = HAL_MMC_RxCpltCallback;
        hmmc->ErrorCallback     = HAL_MMC_ErrorCallback;
        hmmc->AbortCpltCallback = HAL_MMC_AbortCallback;

        if(hmmc->MspInitCallback == NULL)
        {
          hmmc->MspInitCallback = HAL_MMC_MspInit;
        }

        /* Init the low level hardware */
        hmmc->MspInitCallback(hmmc);
#else
        /* Init the low level hardware : GPIO, CLOCK, CORTEX...etc */
        HAL_MMC_MspInit(hmmc);
#endif
    }
    hmmc->State = HAL_MMC_STATE_BUSY;
    /* Initialize the Card parameters */
    HAL_MMC_InitCard(hmmc);
    /* Initialize the error code */
    hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
    /* Initialize the MMC operation */
    hmmc->Context = MMC_CONTEXT_NONE;
    /* Initialize the MMC state */
    hmmc->State = HAL_MMC_STATE_READY;
    return HAL_OK;
}

/**
  * @brief  Initializes the MMC Card.
  * @param  hmmc Pointer to MMC handle
  * @note   This function initializes the MMC card. It could be used when a card 
            re-initialization is needed.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_InitCard(MMC_HandleTypeDef *hmmc)
{
    uint32_t errorstate = HAL_MMC_ERROR_NONE;
    MMC_InitTypeDef Init;

    /* Default SDMMC peripheral configuration for MMC card initialization */
    Init.SpeedMode           = SDIO_Speed_Mode_Full;
    Init.ClockMode           = SDIO_Clock_Mode_10Bit_Divider;
    Init.BusWide             = SDIO_BUS_WIDE_1B;
    Init.ClockDiv            = SDIO_INIT_CLK_DIV;

    /* Initialize SDMMC peripheral interface with default configuration */
    SDIO_Init(hmmc->Instance, Init);

    /* Disable SDMMC Clock */
    __HAL_MMC_DISABLE(hmmc);

    /* Set Power State to ON */
    SDIO_PowerState_ON(hmmc->Instance);

    /* Enable SDMMC Clock */
    __HAL_MMC_ENABLE(hmmc);

    /* Required power up waiting time before starting the SD initialization
    sequence */
    HAL_Delay(2U);
  
    /* Identify card operating voltage */
    errorstate = MMC_PowerON(hmmc);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
        hmmc->State = HAL_MMC_STATE_READY;
        hmmc->ErrorCode |= errorstate;
        return HAL_ERROR;
    }

    /* Card initialization */
    errorstate = MMC_InitCard(hmmc);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
        hmmc->State = HAL_MMC_STATE_READY;
        hmmc->ErrorCode |= errorstate;
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
  * @brief  De-Initializes the MMC card.
  * @param  hmmc Pointer to MMC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_DeInit(MMC_HandleTypeDef *hmmc)
{
    /* Check the MMC handle allocation */
    if(hmmc == NULL)
    {
        return HAL_ERROR;
    }
    /* Check the parameters */
    assert_param(IS_SDIO_ALL_INSTANCE(hmmc->Instance));
    hmmc->State = HAL_MMC_STATE_BUSY;
    /* Set SD power state to off */
    MMC_PowerOFF(hmmc);
#if (USE_HAL_MMC_REGISTER_CALLBACKS == 1)
    if(hmmc->MspDeInitCallback == NULL)
    {
        hmmc->MspDeInitCallback = HAL_MMC_MspDeInit;
    }
    /* DeInit the low level hardware */
    hmmc->MspDeInitCallback(hmmc);
#else
    /* De-Initialize the MSP layer */
    HAL_MMC_MspDeInit(hmmc);
#endif
    hmmc->ErrorCode = HAL_MMC_ERROR_NONE;
    hmmc->State = HAL_MMC_STATE_RESET;

    return HAL_OK;
}

/**
  * @brief  Initializes the MMC MSP.
  * @param  hmmc Pointer to MMC handle
  * @retval None
  */
__WEAK void HAL_MMC_MspInit(MMC_HandleTypeDef *hmmc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hmmc);

    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_MMC_MspInit could be implemented in the user file
    */
}

/**
  * @brief  De-Initialize MMC MSP.
  * @param  hmmc Pointer to MMC handle
  * @retval None
  */
__WEAK void HAL_MMC_MspDeInit(MMC_HandleTypeDef *hmmc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hmmc);

    /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_MMC_MspDeInit could be implemented in the user file
    */
}

/**
  * @}
  */

/** @addtogroup MMC_Exported_Functions_Group2
 *  @brief   Data transfer functions 
 *
@verbatim   
  ==============================================================================
                        ##### IO operation functions #####
  ==============================================================================  
  [..]
    This subsection provides a set of functions allowing to manage the data 
    transfer from/to MMC card.

@endverbatim
  * @{
  */

/**
  * @brief  Check hardware error
  * @param  hmmc: Pointer to MMC handle
  * @retval HAL status
  */
static HAL_StatusTypeDef MMC_Check_Error(MMC_HandleTypeDef *hmmc)
{
    /* Get error state */

    if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_EINT))
    {
        if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_ECCRC))
        {
            hmmc->HardwareErrCode |= HAL_MMC_ERROR_HW_CMD_CRC;
        }
        else if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_ECEB))
        {
            hmmc->HardwareErrCode |= HAL_MMC_ERROR_HW_CMD_END_BIT;
        }
        else if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_ECI))
        {
            hmmc->HardwareErrCode |= HAL_MMC_ERROR_HW_CMD_Index;
        }
        else if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_EDT))
        {
            hmmc->HardwareErrCode |= HAL_MMC_ERROR_HW_DATA_TIMEOUT;
        }
        else if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_EDCRC))
        {
            hmmc->HardwareErrCode |= HAL_MMC_ERROR_HW_DATA_CRC;
        }
        else if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_EDEB))
        {
            hmmc->HardwareErrCode |= HAL_MMC_ERROR_HW_DATA_END_BIT;
        }
        else if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_ECL))
        {
            hmmc->HardwareErrCode |= HAL_MMC_ERROR_HW_Current_Limit;
        }
        else if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_EAC))
        {
            /* Clear all the static flags */
            hmmc->HardwareErrCode |= HAL_MMC_ERROR_HW_AUTO_CMD;
        }
        else if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_EADMA))
        {
            hmmc->HardwareErrCode |= HAL_MMC_ERROR_HW_ADMA;
        }
        else if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_ETNG))
        {
            hmmc->HardwareErrCode |= HAL_MMC_ERROR_HW_TUNING;
        }
        /* Clear all the error flags */
        __HAL_MMC_CLEAR_FLAG(hmmc, MMC_IT_FLAGS_OTHER_ERRORS | MMC_IT_CC | MMC_IT_TC);
        return HAL_ERROR;
    }
    return HAL_OK;
}

/**
  * @brief  Reads block(s) from a specified address in a card. The Data transfer 
  *         is managed by polling mode.
  * @note   This API should be followed by a check on the card state through
  *         HAL_MMC_GetCardState().
  * @param  hmmc: Pointer to MMC handle
  * @param  pData: pointer to the buffer that will contain the received data
  * @param  BlockAddr: Block Address from where data is to be read
  * @param  NumberOfBlocks: Number of MMC blocks to read
  * @param  Timeout: Specify timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_ReadBlocks(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAddr, uint32_t NumberOfBlocks, uint32_t Timeout)
{
    uint32_t errorstate = HAL_MMC_ERROR_NONE;
    uint32_t tickstart = HAL_GetTick();
    uint32_t count = 0U;
    hmmc->RxXferCount = 0;
    hmmc->pRxPtr = (uint32_t *)pData;
    Timeout *= (GetSystemCoreClock() / 8U /1000U);
  
    if(hmmc == NULL || NULL == pData)
    {
        hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
        return HAL_ERROR;
    }
    if((BlockAddr + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))
    {
        hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;
        return HAL_ERROR;
    }
    if(hmmc->State == HAL_MMC_STATE_READY)
    {
        hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
        while(__HAL_MMC_GET_STATE(hmmc, MMC_STATE_CICMD) || __HAL_MMC_GET_STATE(hmmc, MMC_STATE_CIDAT))
        {
            if((Timeout == 0U) || ((HAL_GetTick() - tickstart) >=  Timeout))
            {
                /* Clear all the static flags */
                __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
                hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;
                hmmc->State= HAL_MMC_STATE_READY;
                return HAL_TIMEOUT;
            }
        }
        hmmc->State = HAL_MMC_STATE_BUSY;
        /* Initialize data control register */
        hmmc->Instance->Slot->BLOCK_CSR = 0U;
        /* Check the Card capacity in term of Logical number of blocks */
        if ((hmmc->MmcCard.LogBlockNbr) < CAPACITY)
        {
            BlockAddr *= 512U;
        }
        /* Set Block Size for Card */
        errorstate = SDMMC_CmdBlockLength(hmmc->Instance, hmmc->MmcCard.BlockSize);
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
            hmmc->ErrorCode |= errorstate;
            hmmc->State = HAL_MMC_STATE_READY;
            return HAL_ERROR;
        }
        /* Configure the MMC DPSM (Data Path State Machine) */
        hmmc->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
        hmmc->Config.DataLength    = NumberOfBlocks * hmmc->MmcCard.BlockSize;
        hmmc->Config.DataBlockSize = hmmc->MmcCard.BlockSize;
        hmmc->RxXferSize = hmmc->Config.DataLength;
        SDIO_ConfigData(hmmc->Instance, &hmmc->Config);
        /* Read block(s) in polling mode */
        if(NumberOfBlocks > 1U)
        {
            hmmc->Context = MMC_CONTEXT_READ_MULTIPLE_BLOCK;
            /* Read Multi Block command */
            errorstate = SDMMC_CmdReadMultiBlock(hmmc->Instance, BlockAddr);
        }
        else
        {
            hmmc->Context = MMC_CONTEXT_READ_SINGLE_BLOCK;
            /* Read Single Block command */
            errorstate = SDMMC_CmdReadSingleBlock(hmmc->Instance, BlockAddr);
        }
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
            hmmc->ErrorCode |= errorstate;
            hmmc->State = HAL_MMC_STATE_READY;
            return HAL_ERROR;
        }
        /* Poll on SDMMC flags */
        if(__HAL_MMC_GET_IT_STATE_EN(hmmc, MMC_IT_STATE_BRR))
        {
            while(hmmc->RxXferCount < hmmc->Config.DataLength)
            {
                /* Wait for SRS12.BRR */
                while(!__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_BRR))
                {
                    /* Get error state */
                    if(MMC_Check_Error(hmmc) == HAL_ERROR)
                    {
                        /* Clear all the static flags */
                        __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
                        hmmc->ErrorCode |= HAL_SD_ERROR_HARDWARE;
                        hmmc->State = HAL_MMC_STATE_READY;
                        return HAL_ERROR;
                    }
                    if((Timeout == 0U)||((HAL_GetTick() - tickstart) >=  Timeout))
                    {
                        /* Clear all the static flags */
                        __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
                        hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;
                        hmmc->State= HAL_MMC_STATE_READY;
                        return HAL_TIMEOUT;
                    }
                }
                /* Check BRR */
                if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_BRR))
                {
                    __HAL_MMC_READ_WAIT_ENABLE(hmmc);
                    __HAL_MMC_CLEAR_FLAG(hmmc, MMC_FLAG_BRR);
                    /* Read data from SDMMC Rx FIFO */
                    for(count = 0U; count < hmmc->Config.DataBlockSize/4; count++)
                    {
                        SDIO_ReadFIFO(hmmc->Instance, hmmc->pRxPtr++);
                    }
                    __HAL_MMC_READ_WAIT_DISABLE(hmmc);
                    hmmc->RxXferCount += hmmc->Config.DataBlockSize;
                    tickstart = HAL_GetTick();
                }
            }
        }
        else
        {
            while(hmmc->RxXferCount < hmmc->Config.DataLength)
            {
                //Buffer Read Enable
                while(!__HAL_MMC_GET_STATE(hmmc, MMC_STATE_BRE))
                {
                    /* Get error state */
                    if(MMC_Check_Error(hmmc) == HAL_ERROR)
                    {
                        /* Clear all the static flags */
                        __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
                        hmmc->ErrorCode |= HAL_SD_ERROR_HARDWARE;
                        hmmc->State = HAL_MMC_STATE_READY;
                        return HAL_ERROR;
                    }
                    if((Timeout == 0U)||((HAL_GetTick() - tickstart) >=  Timeout))
                    {
                        /* Clear all the static flags */
                        __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
                        hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;
                        hmmc->State= HAL_MMC_STATE_READY;
                        return HAL_TIMEOUT;
                    }
                }
                __HAL_MMC_READ_WAIT_ENABLE(hmmc);
                __HAL_MMC_CLEAR_FLAG(hmmc, MMC_FLAG_BRR);
                /* Read data from SDMMC Rx FIFO */
                for(count = 0U; count < hmmc->Config.DataBlockSize/4; count++)
                {
                    SDIO_ReadFIFO(hmmc->Instance, hmmc->pRxPtr++);
                }
                __HAL_MMC_READ_WAIT_DISABLE(hmmc);
                hmmc->RxXferCount += hmmc->Config.DataBlockSize;
                tickstart = HAL_GetTick();
            }
        }
        if(__HAL_MMC_GET_IT_STATE_EN(hmmc, MMC_FLAG_TC))
        {
            while(!__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_TC));
        }
        else
        {
            while(__HAL_MMC_GET_STATE(hmmc, MMC_STATE_CIDAT));
        }
#if(!(defined USE_AUTO_CMD12 && USE_AUTO_CMD12))
        /* Send stop transmission command in case of multiblock read */
        if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_TC) && (NumberOfBlocks > 1U))
        {
            /* Send stop transmission command */
            errorstate = SDMMC_CmdStopTransfer(hmmc->Instance);
            if(errorstate != HAL_MMC_ERROR_NONE)
            {
                /* Clear all the static flags */
                __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
                hmmc->ErrorCode |= errorstate;
                hmmc->State = HAL_MMC_STATE_READY;
                return HAL_ERROR;
            }
        }
#endif
        /* Clear all the static flags */
        __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
        hmmc->State = HAL_MMC_STATE_READY;
        return HAL_OK;
    }
    else
    {
        hmmc->ErrorCode |= HAL_MMC_ERROR_BUSY;
        return HAL_ERROR;
    }
}

/**
  * @brief  Allows to write block(s) to a specified address in a card. The Data
  *         transfer is managed by polling mode.
  * @note   This API should be followed by a check on the card state through
  *         HAL_MMC_GetCardState().
  * @param  hmmc Pointer to MMC handle
  * @param  pData pointer to the buffer that will contain the data to transmit
  * @param  BlockAddr Block Address where data will be written
  * @param  NumberOfBlocks Number of MMC blocks to write
  * @param  Timeout Specify timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_WriteBlocks(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAddr, uint32_t NumberOfBlocks, uint32_t Timeout)
{
    uint32_t errorstate = HAL_MMC_ERROR_NONE;
    uint32_t tickstart = HAL_GetTick();
    uint32_t count = 0U;
    hmmc->TxXferCount = 0;
    hmmc->pTxPtr = (uint32_t *)pData;
    Timeout *= (GetSystemCoreClock() / 8U /1000U);
    if(hmmc == NULL || NULL == pData)
    {
        hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
        return HAL_ERROR;
    }
    if((BlockAddr + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))
    {
        hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;
        return HAL_ERROR;
    }
    if(hmmc->State == HAL_MMC_STATE_READY)
    {
        hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
        while(__HAL_MMC_GET_STATE(hmmc, MMC_STATE_CICMD) || __HAL_MMC_GET_STATE(hmmc, MMC_STATE_CIDAT))
        {
            if((Timeout == 0U)||((HAL_GetTick() - tickstart) >=  Timeout))
            {
                /* Clear all the static flags */
                __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
                hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;
                hmmc->State= HAL_MMC_STATE_READY;
                return HAL_TIMEOUT;
            }
        }
        hmmc->State = HAL_MMC_STATE_BUSY;
        /* Initialize data control register */
        hmmc->Instance->Slot->BLOCK_CSR = 0U;
        /* Check the Card capacity in term of Logical number of blocks */
        if ((hmmc->MmcCard.LogBlockNbr) < CAPACITY)
        {
            BlockAddr *= 512U;
        }
        /* Set Block Size for Card */
        errorstate = SDMMC_CmdBlockLength(hmmc->Instance, hmmc->MmcCard.BlockSize);
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
            hmmc->ErrorCode |= errorstate;
            hmmc->State = HAL_MMC_STATE_READY;
            return HAL_ERROR;
        }
        /* Configure the MMC DPSM (Data Path State Machine) */
        hmmc->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
        hmmc->Config.DataLength    = NumberOfBlocks * hmmc->MmcCard.BlockSize;
        hmmc->Config.DataBlockSize = hmmc->MmcCard.BlockSize;
        hmmc->TxXferSize = hmmc->Config.DataLength;
        SDIO_ConfigData(hmmc->Instance, &hmmc->Config);
        __HAL_MMC_IT_STATE_ENABLE(hmmc, MMC_IT_STATE_OTHER_ERRORS);
        /* Write Blocks in Polling mode */
        if(NumberOfBlocks > 1U)
        {
            hmmc->Context = MMC_CONTEXT_WRITE_MULTIPLE_BLOCK;
            /* Write Multi Block command */
            errorstate = SDMMC_CmdWriteMultiBlock(hmmc->Instance, BlockAddr);
        }
        else
        {
            hmmc->Context = MMC_CONTEXT_WRITE_SINGLE_BLOCK;
            /* Write Single Block command */
            errorstate = SDMMC_CmdWriteSingleBlock(hmmc->Instance, BlockAddr);
        }
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
            hmmc->ErrorCode |= errorstate;
            hmmc->State = HAL_MMC_STATE_READY;
            return HAL_ERROR;
        }
        if(__HAL_MMC_GET_IT_STATE_EN(hmmc, MMC_IT_STATE_BWR))
        {
            /* Poll on SDMMC flags */
            while(hmmc->TxXferCount < hmmc->Config.DataLength)
            {
                /* Wait for SRS12.BWR */
                while(!__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_BWR))
                {
                    /* Get error state */
                    if(MMC_Check_Error(hmmc) == HAL_ERROR)
                    {
                        /* Clear all the static flags */
                        __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
                        hmmc->ErrorCode |= HAL_SD_ERROR_HARDWARE;
                        hmmc->State = HAL_MMC_STATE_READY;
                        return HAL_ERROR;
                    }
                    if((Timeout == 0U)||((HAL_GetTick() - tickstart) >=  Timeout))
                    {
                        /* Clear all the static flags */
                        __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
                        hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;
                        hmmc->State= HAL_MMC_STATE_READY;
                        return HAL_TIMEOUT;
                    }
                }
                /* Check BWR */
                if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_BWR))
                {
                    __HAL_MMC_CLEAR_FLAG(hmmc, MMC_FLAG_BWR);
                    /* Write data from SDMMC Tx FIFO */
                    for(count = 0U; count < hmmc->Config.DataBlockSize/4; count++)
                    {
                        SDIO_WriteFIFO(hmmc->Instance, (hmmc->pTxPtr++));
                    }
                    hmmc->TxXferCount += hmmc->Config.DataBlockSize;
                    tickstart = HAL_GetTick();
                }
            }
        }
        else
        {
            /* Poll on SDMMC flags */
            while(hmmc->TxXferCount < hmmc->Config.DataLength)
            {
                //Buffer Read Enable
                while(!__HAL_MMC_GET_STATE(hmmc, MMC_STATE_BWE))
                {
                    /* Get error state */
                    if(MMC_Check_Error(hmmc) == HAL_ERROR)
                    {
                        /* Clear all the static flags */
                        __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
                        hmmc->ErrorCode |= HAL_SD_ERROR_HARDWARE;
                        hmmc->State = HAL_MMC_STATE_READY;
                        return HAL_ERROR;
                    }
                    if((Timeout == 0U)||((HAL_GetTick() - tickstart) >=  Timeout))
                    {
                        /* Clear all the static flags */
                        __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
                        hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;
                        hmmc->State= HAL_MMC_STATE_READY;
                        return HAL_TIMEOUT;
                    }
                }
                /* Write data from SDMMC Tx FIFO */
                for(count = 0U; count < hmmc->Config.DataBlockSize/4; count++)
                {
                    SDIO_WriteFIFO(hmmc->Instance, (hmmc->pTxPtr++));
                }
                hmmc->TxXferCount += hmmc->Config.DataBlockSize;
                tickstart = HAL_GetTick();
            }
        }
#if(!(defined USE_AUTO_CMD12 && USE_AUTO_CMD12))
        /* Send stop transmission command in case of multiblock write */
        if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_TC) && (NumberOfBlocks > 1U))
        {
            /* Send stop transmission command */
            errorstate = SDMMC_CmdStopTransfer(hmmc->Instance);
            if(errorstate != HAL_MMC_ERROR_NONE)
            {
                /* Clear all the static flags */
                __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
                hmmc->ErrorCode |= errorstate;
                hmmc->State = HAL_MMC_STATE_READY;
                return HAL_ERROR;
            }
        }
#endif
        /* Get error state */
        if(MMC_Check_Error(hmmc) == HAL_ERROR)
        {
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
            hmmc->ErrorCode |= HAL_SD_ERROR_HARDWARE;
            hmmc->State = HAL_MMC_STATE_READY;
            return HAL_ERROR;
        }
        /* Clear all the static flags */
        __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
        hmmc->State = HAL_MMC_STATE_READY;
        return HAL_OK;
    }
    else
    {
        hmmc->ErrorCode |= HAL_MMC_ERROR_BUSY;
        return HAL_ERROR;
    }
}

/**
  * @brief  Reads block(s) from a specified address in a card. The Data transfer 
  *         is managed in interrupt mode. 
  * @note   This API should be followed by a check on the card state through
  *         HAL_MMC_GetCardState().
  * @note   You could also check the IT transfer process through the MMC Rx 
  *         interrupt event.
  * @param  hmmc Pointer to MMC handle                 
  * @param  pData Pointer to the buffer that will contain the received data
  * @param  BlockAddr Block Address from where data is to be read
  * @param  NumberOfBlocks Number of blocks to read.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_ReadBlocks_IT(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAddr, uint32_t NumberOfBlocks)
{
    uint32_t errorstate = HAL_MMC_ERROR_NONE;
    uint32_t tickstart = HAL_GetTick();
    hmmc->RxXferCount = 0;
    hmmc->pRxPtr = (uint32_t *)pData;

    if(hmmc == NULL || NULL == pData)
    {
        hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
        return HAL_ERROR;
    }
    if((BlockAddr + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))
    {
        hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;
        return HAL_ERROR;
    }
    if(hmmc->State == HAL_MMC_STATE_READY)
    {
        hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
        while(__HAL_MMC_GET_STATE(hmmc, MMC_STATE_CICMD) || __HAL_MMC_GET_STATE(hmmc, MMC_STATE_CIDAT))
        {
            if((HAL_GetTick() - tickstart) >=  SDIO_CMDTIMEOUT)
            {
                /* Clear all the static flags */
                __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
                hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;
                hmmc->State= HAL_MMC_STATE_READY;
                return HAL_TIMEOUT;
            }
        }
        hmmc->State = HAL_MMC_STATE_BUSY;
        /* Initialize data control register */
        hmmc->Instance->Slot->BLOCK_CSR = 0U;
        hmmc->pRxBuffPtr = (uint32_t *)pData;
        hmmc->RxXferSize = hmmc->MmcCard.BlockSize * NumberOfBlocks;
        /* Check the Card capacity in term of Logical number of blocks */
        if ((hmmc->MmcCard.LogBlockNbr) < CAPACITY)
        {
            BlockAddr *= 512U;
        }
        /* Set Block Size for Card */
        errorstate = SDMMC_CmdBlockLength(hmmc->Instance, hmmc->MmcCard.BlockSize);
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
            hmmc->ErrorCode |= errorstate;
            hmmc->State = HAL_MMC_STATE_READY;
            return HAL_ERROR;
        }
        /* Configure the MMC DPSM (Data Path State Machine) */
        hmmc->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
        hmmc->Config.DataLength    = hmmc->MmcCard.BlockSize * NumberOfBlocks;
        hmmc->Config.DataBlockSize = hmmc->MmcCard.BlockSize;
        SDIO_ConfigData(hmmc->Instance, &hmmc->Config);
        __HAL_MMC_CLEAR_FLAG(hmmc, (MMC_IT_BRR | MMC_IT_TC | MMC_IT_OTHER_ERRORS));
        __HAL_MMC_ENABLE_IT(hmmc, (MMC_IT_BRR | MMC_IT_TC | MMC_IT_OTHER_ERRORS));
        /* Read Blocks in IT mode */
        if(NumberOfBlocks > 1U)
        {
            hmmc->Context = (MMC_CONTEXT_READ_MULTIPLE_BLOCK | MMC_CONTEXT_IT);
            /* Read Multi Block command */
            errorstate = SDMMC_CmdReadMultiBlock(hmmc->Instance, BlockAddr);
        }
        else
        {
            hmmc->Context = (MMC_CONTEXT_READ_SINGLE_BLOCK | MMC_CONTEXT_IT);
            /* Read Single Block command */
            errorstate = SDMMC_CmdReadSingleBlock(hmmc->Instance, BlockAddr);
        }
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            __HAL_MMC_DISABLE_IT(hmmc, (MMC_IT_BRR | MMC_IT_TC | MMC_IT_OTHER_ERRORS));
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
            hmmc->ErrorCode |= errorstate;
            hmmc->State = HAL_MMC_STATE_READY;
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
  *         HAL_MMC_GetCardState().
  * @note   You could also check the IT transfer process through the MMC Tx 
  *         interrupt event. 
  * @param  hmmc Pointer to MMC handle
  * @param  pData Pointer to the buffer that will contain the data to transmit
  * @param  BlockAddr Block Address where data will be written
  * @param  NumberOfBlocks Number of blocks to write
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_WriteBlocks_IT(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAddr, uint32_t NumberOfBlocks)
{
    uint32_t tickstart = HAL_GetTick();
    uint32_t errorstate = HAL_MMC_ERROR_NONE;
    hmmc->TxXferCount = 0;
    hmmc->pTxPtr = (uint32_t *)pData;
    
    if(hmmc == NULL || NULL == pData)
    {
        hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
        return HAL_ERROR;
    }
    if((BlockAddr + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))
    {
        hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;
        return HAL_ERROR;
    }
    if(hmmc->State == HAL_MMC_STATE_READY)
    {
        hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
        while(__HAL_MMC_GET_STATE(hmmc, MMC_STATE_CICMD) || __HAL_MMC_GET_STATE(hmmc, MMC_STATE_CIDAT))
        {
            if((HAL_GetTick() - tickstart) >=  SDIO_CMDTIMEOUT)
            {
                /* Clear all the static flags */
                __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
                hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;
                hmmc->State= HAL_MMC_STATE_READY;
                return HAL_TIMEOUT;
            }
        }
        hmmc->State = HAL_MMC_STATE_BUSY;
        /* Initialize data control register */
        hmmc->Instance->Slot->BLOCK_CSR = 0U;
        hmmc->pTxBuffPtr = (uint32_t *)pData;
        hmmc->TxXferSize = hmmc->MmcCard.BlockSize * NumberOfBlocks;
        /* Check the Card capacity in term of Logical number of blocks */
        if ((hmmc->MmcCard.LogBlockNbr) < CAPACITY)
        {
            BlockAddr *= 512U;
        }
        /* Set Block Size for Card */
        errorstate = SDMMC_CmdBlockLength(hmmc->Instance, hmmc->MmcCard.BlockSize);
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
            hmmc->ErrorCode |= errorstate;
            hmmc->State = HAL_MMC_STATE_READY;
            return HAL_ERROR;
        }
        /* Configure the MMC DPSM (Data Path State Machine) */
        hmmc->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
        hmmc->Config.DataLength    = hmmc->MmcCard.BlockSize * NumberOfBlocks;
        hmmc->Config.DataBlockSize = hmmc->MmcCard.BlockSize;
        SDIO_ConfigData(hmmc->Instance, &hmmc->Config);
        /* Enable transfer interrupts */
        __HAL_MMC_CLEAR_FLAG(hmmc, (MMC_IT_BWR | MMC_IT_TC | MMC_IT_OTHER_ERRORS));
        __HAL_MMC_ENABLE_IT(hmmc, (MMC_IT_BWR | MMC_IT_TC | MMC_IT_OTHER_ERRORS));
        /* Write Blocks in Polling mode */
        if(NumberOfBlocks > 1U)
        {
            hmmc->Context = (MMC_CONTEXT_WRITE_MULTIPLE_BLOCK| MMC_CONTEXT_IT);
            /* Write Multi Block command */
            errorstate = SDMMC_CmdWriteMultiBlock(hmmc->Instance, BlockAddr);
        }
        else
        {
            hmmc->Context = (MMC_CONTEXT_WRITE_SINGLE_BLOCK | MMC_CONTEXT_IT);
            /* Write Single Block command */
            errorstate = SDMMC_CmdWriteSingleBlock(hmmc->Instance, BlockAddr);
        }
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            __HAL_MMC_DISABLE_IT(hmmc, (MMC_IT_BWR | MMC_IT_TC | MMC_IT_OTHER_ERRORS));
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
            hmmc->ErrorCode |= errorstate;
            hmmc->State = HAL_MMC_STATE_READY;
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
  *         HAL_MMC_GetCardState().
  * @note   You could also check the DMA transfer process through the MMC Rx 
  *         interrupt event.
  * @param  hmmc Pointer MMC handle
  * @param  DMAInit ADMAx Descriptor
  * @param  BlockAddr Block Address from where data is to be read
  * @param  NumberOfBlocks Number of blocks to read.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_ReadBlocks_DMA(MMC_HandleTypeDef *hmmc, SDIO_DMAInitTypedef *DMAInit, uint32_t BlockAddr, uint32_t NumberOfBlocks)
{
    uint32_t errorstate = HAL_MMC_ERROR_NONE;
    uint32_t tickstart = HAL_GetTick();
    hmmc->dmaInit = DMAInit;
    hmmc->RxXferCount = 0;
    uint32_t block_size = 0;

    if(hmmc == NULL ||
      (hmmc->dmaInit->DMAx == SDIO_SDMA && hmmc->dmaInit->SDMA_Descriptor == NULL) ||
      (hmmc->dmaInit->DMAx == SDIO_ADMA1 && hmmc->dmaInit->ADMA1_DescriptorTable == NULL) ||
      (hmmc->dmaInit->DMAx == SDIO_ADMA2 && hmmc->dmaInit->ADMA2_DescriptorTable == NULL))
    {
        hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
        return HAL_ERROR;
    }
    if((BlockAddr + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))
    {
        hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;
        return HAL_ERROR;
    }
    if(hmmc->State == HAL_MMC_STATE_READY)
    {
        hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
        while(__HAL_MMC_GET_STATE(hmmc, MMC_STATE_CICMD) || __HAL_MMC_GET_STATE(hmmc, MMC_STATE_CIDAT))
        {
            if((HAL_GetTick() - tickstart) >=  SDIO_CMDTIMEOUT)
            {
                /* Clear all the static flags */
                __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
                hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;
                hmmc->State= HAL_MMC_STATE_READY;
                return HAL_TIMEOUT;
            }
        }
        hmmc->State = HAL_MMC_STATE_BUSY;
        /* Initialize data control register */
        hmmc->Instance->Slot->BLOCK_CSR = 0U;
        if(hmmc->dmaInit->DMAx == SDIO_SDMA)
        {
            block_size = hmmc->dmaInit->SDMA_Descriptor->BlockSize;
        }
        else
        {
            block_size = hmmc->MmcCard.BlockSize;
        }
        /* Check the Card capacity in term of Logical number of blocks */
        if ((hmmc->MmcCard.LogBlockNbr) < CAPACITY)
        {
            BlockAddr *= 512U;
        }
        /* Set Block Size for Card */
        errorstate = SDMMC_CmdBlockLength(hmmc->Instance, block_size);
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
            hmmc->ErrorCode |= errorstate;
            hmmc->State = HAL_MMC_STATE_READY;
            return HAL_ERROR;
        }
        if(hmmc->dmaInit->DMAx == SDIO_SDMA)
        {
            hmmc->pRxPtr = (uint32_t *)hmmc->dmaInit->SDMA_Descriptor->Address;
            __HAL_MMC_DMA_SELECT(hmmc, hmmc->dmaInit->DMAx);
            /* Set SDMA System Address */
            __HAL_MMC_SDMA_SET_ADDR(hmmc, hmmc->dmaInit->SDMA_Descriptor->Address);
            __HAL_MMC_SDMA_SET_BOUNDARY(hmmc, hmmc->dmaInit->SDMA_Descriptor->BufferBoundary);
            __HAL_MMC_DMA_SET_BURST_LENGTH(hmmc, hmmc->dmaInit->SDMA_Descriptor->BurstLength);
            /* Flush and invalidate Cache by addr */
            ccm_FlushInvalidateDCache_by_Addr((void *)hmmc->dmaInit->SDMA_Descriptor->Address, hmmc->dmaInit->SDMA_Descriptor->BlockCount * hmmc->dmaInit->SDMA_Descriptor->BlockSize);
            /* Configure the SD DPSM (Data Path State Machine) */
            hmmc->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
            hmmc->Config.DataLength    = hmmc->dmaInit->SDMA_Descriptor->BlockCount * hmmc->dmaInit->SDMA_Descriptor->BlockSize;
            hmmc->Config.DataBlockSize = hmmc->dmaInit->SDMA_Descriptor->BlockSize;
        }
        else if (hmmc->dmaInit->DMAx == SDIO_ADMA1)
        {
            hmmc->pRxPtr = (uint32_t *)(hmmc->dmaInit->ADMA1_DescriptorTable->PageAddress);
            __HAL_MMC_DMA_SELECT(hmmc, hmmc->dmaInit->DMAx);
            /* Set SDMA System Address */
            __HAL_MMC_SDMA_SET_ADDR(hmmc, hmmc->dmaInit->ADMA1_DescriptorTable->PageAddress);
            /* Set ADMA System Address */
            __HAL_MMC_ADMA_SET_ADDR(hmmc, (uint32_t)hmmc->dmaInit->ADMA1_DescriptorTable);
            /* Flush Cache by addr */
            ccm_FlushDCache_by_Addr((uint32_t *)hmmc->dmaInit->ADMA1_DescriptorTable, hmmc->dmaInit->table_size*sizeof(SDIO_ADMA1_Descriptor));
            /* Flush and invalidate Cache by addr */
            ccm_FlushInvalidateDCache_by_Addr((void *)hmmc->pRxPtr, hmmc->MmcCard.BlockSize * NumberOfBlocks);
            /* Configure the SD DPSM (Data Path State Machine) */
            hmmc->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
            hmmc->Config.DataLength    = hmmc->MmcCard.BlockSize * NumberOfBlocks;
            hmmc->Config.DataBlockSize = hmmc->MmcCard.BlockSize;
        }
        else if (hmmc->dmaInit->DMAx == SDIO_ADMA2)
        {
            hmmc->pRxPtr = (uint32_t *)hmmc->dmaInit->ADMA2_DescriptorTable->PageAddress;
            __HAL_MMC_DMA_SELECT(hmmc, hmmc->dmaInit->DMAx);
            /* Set SDMA System Address */
            __HAL_MMC_SDMA_SET_ADDR(hmmc, hmmc->dmaInit->ADMA2_DescriptorTable->PageAddress);
            /* Set ADMA System Address */
            __HAL_MMC_ADMA_SET_ADDR(hmmc, (uint32_t)hmmc->dmaInit->ADMA2_DescriptorTable);
            /* Flush Cache by addr */
            ccm_FlushDCache_by_Addr((uint32_t *)hmmc->dmaInit->ADMA2_DescriptorTable, hmmc->dmaInit->table_size*sizeof(SDIO_ADMA2_Descriptor));
            /* Flush and invalidate Cache by addr */
            ccm_FlushInvalidateDCache_by_Addr((void *)hmmc->dmaInit->ADMA2_DescriptorTable->PageAddress, hmmc->MmcCard.BlockSize * NumberOfBlocks);
            /* Configure the SD DPSM (Data Path State Machine) */
            hmmc->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
            hmmc->Config.DataLength    = hmmc->MmcCard.BlockSize * NumberOfBlocks;
            hmmc->Config.DataBlockSize = hmmc->MmcCard.BlockSize;
        }
        else
        {
            hmmc->ErrorCode |= HAL_MMC_ERROR_HW_OTHER_DMA_NOT_SUP;
            hmmc->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        /* Clear all the static flags */
        __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
        __HAL_MMC_ENABLE_IT(hmmc, (MMC_IT_TC | MMC_IT_DMAINT | MMC_IT_OTHER_ERRORS));
        SDIO_ConfigData(hmmc->Instance, &hmmc->Config);
        /* Read Blocks in DMA mode */
        if(NumberOfBlocks > 1U)
        {
            hmmc->Context = (MMC_CONTEXT_READ_MULTIPLE_BLOCK | MMC_CONTEXT_DMA);
            /* Read Multi Block command */
            errorstate = SDMMC_CmdReadMultiBlock(hmmc->Instance, BlockAddr);
        }
        else
        {
            hmmc->Context = (MMC_CONTEXT_READ_SINGLE_BLOCK | MMC_CONTEXT_DMA);
            /* Read Single Block command */
            errorstate = SDMMC_CmdReadSingleBlock(hmmc->Instance, BlockAddr);
        }
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
            hmmc->ErrorCode |= errorstate;
            hmmc->State = HAL_MMC_STATE_READY;
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
  *         HAL_MMC_GetCardState().
  * @note   You could also check the DMA transfer process through the MMC Tx 
  *         interrupt event.
  * @param  hmmc Pointer to MMC handle
  * @param  pData Pointer to the buffer that will contain the data to transmit
  * @param  BlockAddr Block Address where data will be written
  * @param  NumberOfBlocks Number of blocks to write
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_WriteBlocks_DMA(MMC_HandleTypeDef *hmmc, SDIO_DMAInitTypedef *DMAInit, uint32_t BlockAddr, uint32_t NumberOfBlocks)
{
    uint32_t errorstate = HAL_MMC_ERROR_NONE;
    uint32_t tickstart = HAL_GetTick();
    hmmc->TxXferCount = 0;
    hmmc->dmaInit = DMAInit;
    uint32_t block_size = 0;

    if(hmmc == NULL ||
       (hmmc->dmaInit->DMAx == SDIO_SDMA && hmmc->dmaInit->SDMA_Descriptor == NULL) ||
       (hmmc->dmaInit->DMAx == SDIO_ADMA1 && hmmc->dmaInit->ADMA1_DescriptorTable == NULL) ||
       (hmmc->dmaInit->DMAx == SDIO_ADMA2 && hmmc->dmaInit->ADMA2_DescriptorTable == NULL))
    {
        hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
        return HAL_ERROR;
    }
    if((BlockAddr + NumberOfBlocks) > (hmmc->MmcCard.LogBlockNbr))
    {
        hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;
        return HAL_ERROR;
    }
    if(hmmc->State == HAL_MMC_STATE_READY)
    {
        hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
        hmmc->State = HAL_MMC_STATE_BUSY;
        while(__HAL_MMC_GET_STATE(hmmc, MMC_STATE_CICMD) || __HAL_MMC_GET_STATE(hmmc, MMC_STATE_CIDAT))
        {
            if((HAL_GetTick() - tickstart) >=  SDIO_CMDTIMEOUT)
            {
                /* Clear all the static flags */
                __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
                hmmc->ErrorCode |= HAL_MMC_ERROR_TIMEOUT;
                hmmc->State= HAL_MMC_STATE_READY;
                return HAL_TIMEOUT;
            }
        }
        /* Initialize data control register */
        hmmc->Instance->Slot->BLOCK_CSR = 0U;
        if(hmmc->dmaInit->DMAx == SDIO_SDMA)
        {
            block_size = hmmc->dmaInit->SDMA_Descriptor->BlockSize;
        }
        else
        {
            block_size = hmmc->MmcCard.BlockSize;
        }
        /* Set Block Size for Card */
        errorstate = SDMMC_CmdBlockLength(hmmc->Instance, hmmc->MmcCard.BlockSize);
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
            hmmc->ErrorCode |= errorstate;
            hmmc->State = HAL_MMC_STATE_READY;
            return HAL_ERROR;
        }
        /* Check the Card capacity in term of Logical number of blocks */
        if ((hmmc->MmcCard.LogBlockNbr) < CAPACITY)
        {
            BlockAddr *= 512U;
        }
        if(hmmc->dmaInit->DMAx == SDIO_SDMA)
        {
            hmmc->pTxPtr = (uint32_t *)hmmc->dmaInit->SDMA_Descriptor->Address;
            __HAL_MMC_DMA_SELECT(hmmc, hmmc->dmaInit->DMAx);
            /* Set SDMA System Address */
            __HAL_MMC_SDMA_SET_ADDR(hmmc, hmmc->dmaInit->SDMA_Descriptor->Address);
            __HAL_MMC_SDMA_SET_BOUNDARY(hmmc, hmmc->dmaInit->SDMA_Descriptor->BufferBoundary);
            __HAL_MMC_DMA_SET_BURST_LENGTH(hmmc, hmmc->dmaInit->SDMA_Descriptor->BurstLength);
            /* Flush Cache by addr */
            ccm_FlushDCache_by_Addr((uint32_t *)hmmc->dmaInit->SDMA_Descriptor->Address, hmmc->dmaInit->SDMA_Descriptor->BlockCount * hmmc->dmaInit->SDMA_Descriptor->BlockSize);
            /* Configure the SD DPSM (Data Path State Machine) */
            hmmc->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
            hmmc->Config.DataLength    = hmmc->dmaInit->SDMA_Descriptor->BlockCount * hmmc->dmaInit->SDMA_Descriptor->BlockSize;
            hmmc->Config.DataBlockSize = hmmc->dmaInit->SDMA_Descriptor->BlockSize;
        }
        else if (hmmc->dmaInit->DMAx == SDIO_ADMA1)
        {
            hmmc->pTxPtr = (uint32_t *)(hmmc->dmaInit->ADMA1_DescriptorTable->PageAddress);
            __HAL_MMC_DMA_SELECT(hmmc, hmmc->dmaInit->DMAx);
            /* Set SDMA System Address */
            __HAL_MMC_SDMA_SET_ADDR(hmmc, hmmc->dmaInit->ADMA1_DescriptorTable->PageAddress);
            /* Set ADMA System Address */
            __HAL_MMC_ADMA_SET_ADDR(hmmc, (uint32_t)hmmc->dmaInit->ADMA1_DescriptorTable);
            /* Flush Cache by addr */
            ccm_FlushDCache_by_Addr((uint32_t *)hmmc->dmaInit->ADMA1_DescriptorTable, hmmc->dmaInit->table_size*sizeof(SDIO_ADMA1_Descriptor));
            /* Flush Cache by addr */
            ccm_FlushDCache_by_Addr((uint32_t *)hmmc->pTxPtr, hmmc->MmcCard.BlockSize * NumberOfBlocks);
            /* Configure the SD DPSM (Data Path State Machine) */
            hmmc->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
            hmmc->Config.DataLength    = hmmc->MmcCard.BlockSize * NumberOfBlocks;
            hmmc->Config.DataBlockSize = hmmc->MmcCard.BlockSize;
        }
        else if (hmmc->dmaInit->DMAx == SDIO_ADMA2)
        {
            hmmc->pTxPtr = (uint32_t *)hmmc->dmaInit->ADMA2_DescriptorTable->PageAddress;
            __HAL_MMC_DMA_SELECT(hmmc, hmmc->dmaInit->DMAx);
            /* Set SDMA System Address */
            __HAL_MMC_SDMA_SET_ADDR(hmmc, hmmc->dmaInit->ADMA2_DescriptorTable->PageAddress);
            /* Set ADMA System Address */
            __HAL_MMC_ADMA_SET_ADDR(hmmc, (uint32_t)hmmc->dmaInit->ADMA2_DescriptorTable);
            /* Flush Cache by addr */
            ccm_FlushDCache_by_Addr((uint32_t *)hmmc->dmaInit->ADMA2_DescriptorTable, hmmc->dmaInit->table_size*sizeof(SDIO_ADMA2_Descriptor));
            /* Flush Cache by addr */
            ccm_FlushDCache_by_Addr((uint32_t *)hmmc->dmaInit->ADMA2_DescriptorTable->PageAddress, hmmc->MmcCard.BlockSize * NumberOfBlocks);
            /* Configure the SD DPSM (Data Path State Machine) */
            hmmc->Config.DataTimeOut   = SDMMC_DATATIMEOUT;
            hmmc->Config.DataLength    = hmmc->MmcCard.BlockSize * NumberOfBlocks;
            hmmc->Config.DataBlockSize = hmmc->MmcCard.BlockSize;
        }
        else
        {
            hmmc->ErrorCode |= HAL_SD_ERROR_HW_OTHER_DMA_NOT_SUP;
            hmmc->State = HAL_SD_STATE_READY;
            return HAL_ERROR;
        }
        /* Clear all the static flags */
        __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
        /* Enable SD Error interrupts */
        __HAL_MMC_ENABLE_IT(hmmc, (MMC_IT_TC | MMC_IT_DMAINT | MMC_IT_EADMA));
        SDIO_ConfigData(hmmc->Instance, &hmmc->Config);
        /* Write Blocks in Polling mode */
        if(NumberOfBlocks > 1U)
        {
            hmmc->Context = (MMC_CONTEXT_WRITE_MULTIPLE_BLOCK | MMC_CONTEXT_DMA);
            /* Write Multi Block command */
            errorstate = SDMMC_CmdWriteMultiBlock(hmmc->Instance, BlockAddr);
        }
        else
        {
            hmmc->Context = (MMC_CONTEXT_WRITE_SINGLE_BLOCK | MMC_CONTEXT_DMA);
            /* Write Single Block command */
            errorstate = SDMMC_CmdWriteSingleBlock(hmmc->Instance, BlockAddr);
        }
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_MMC_DISABLE_IT(hmmc, (MMC_IT_TC | MMC_IT_DMAINT | MMC_IT_EADMA));
            __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
            hmmc->ErrorCode |= errorstate;
            hmmc->State = HAL_MMC_STATE_READY;
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
  * @brief  Erases the specified memory area of the given MMC card.
  * @note   This API should be followed by a check on the card state through
  *         HAL_MMC_GetCardState().
  * @param  hmmc Pointer to MMC handle 
  * @param  BlockStartAdd Start Block address
  * @param  BlockEndAdd End Block address
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_Erase(MMC_HandleTypeDef *hmmc, uint32_t BlockStartAdd, uint32_t BlockEndAdd)
{
    uint32_t errorstate = HAL_MMC_ERROR_NONE;
    if(hmmc->State == HAL_MMC_STATE_READY)
    {
        hmmc->ErrorCode = HAL_DMA_ERROR_NONE;
        if(BlockEndAdd < BlockStartAdd)
        {
            hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
            return HAL_ERROR;
        }
        if(BlockEndAdd > (hmmc->MmcCard.LogBlockNbr))
        {
            hmmc->ErrorCode |= HAL_MMC_ERROR_ADDR_OUT_OF_RANGE;
            return HAL_ERROR;
        }
        hmmc->State = HAL_MMC_STATE_BUSY;
        /* Check if the card command class supports erase command */
        if(((hmmc->MmcCard.Class) & SDIO_CCCC_ERASE) == 0U)
        {
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
            hmmc->ErrorCode |= HAL_MMC_ERROR_REQUEST_NOT_APPLICABLE;
            hmmc->State = HAL_MMC_STATE_READY;
            return HAL_ERROR;
        }
        if((SDIO_GetResponse(hmmc->Instance, SDIO_RESP0) & SDMMC_CARD_LOCKED) == SDMMC_CARD_LOCKED)
        {
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
            hmmc->ErrorCode |= HAL_MMC_ERROR_LOCK_UNLOCK_FAILED;
            hmmc->State = HAL_MMC_STATE_READY;
            return HAL_ERROR;
        }
        /* Check the Card capacity in term of Logical number of blocks */
        if ((hmmc->MmcCard.LogBlockNbr) < CAPACITY)
        {
            BlockStartAdd *= 512U;
            BlockEndAdd   *= 512U;
        }
        /* Send CMD35 MMC_ERASE_GRP_START with argument as addr  */
        errorstate = SDMMC_CmdEraseStartAdd(hmmc->Instance, BlockStartAdd);
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
            hmmc->ErrorCode |= errorstate;
            hmmc->State = HAL_MMC_STATE_READY;
            return HAL_ERROR;
        }
        /* Send CMD36 MMC_ERASE_GRP_END with argument as addr  */
        errorstate = SDMMC_CmdEraseEndAdd(hmmc->Instance, BlockEndAdd);
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
            hmmc->ErrorCode |= errorstate;
            hmmc->State = HAL_MMC_STATE_READY;
            return HAL_ERROR;
        }
        /* Send CMD38 ERASE */
        errorstate = SDMMC_CmdErase(hmmc->Instance);
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            /* Clear all the static flags */
            __HAL_MMC_CLEAR_FLAG(hmmc, MMC_STATIC_FLAGS);
            hmmc->ErrorCode |= errorstate;
            hmmc->State = HAL_MMC_STATE_READY;
            return HAL_ERROR;
        }
        hmmc->State = HAL_MMC_STATE_READY;
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  This function handles MMC card interrupt request.
  * @param  hmmc Pointer to MMC handle
  * @retval None
  */
void HAL_MMC_IRQHandler(MMC_HandleTypeDef *hmmc)
{
    uint32_t errorstate = HAL_MMC_ERROR_NONE;
  
    /* Check for SDIO interrupt flags */
    if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_TC) != RESET)
    {
        __HAL_MMC_DISABLE_IT(hmmc, MMC_FLAG_TC | MMC_FLAG_EDCRC | MMC_FLAG_EDT | MMC_FLAG_EDEB);
        __HAL_MMC_CLEAR_FLAG(hmmc, MMC_FLAG_TC);
        if((hmmc->Context & MMC_CONTEXT_IT) != RESET)
        {
#if(defined USE_AUTO_CMD12 && !USE_AUTO_CMD12)
            if(((hmmc->Context & MMC_CONTEXT_READ_MULTIPLE_BLOCK) != RESET) || ((hmmc->Context & MMC_CONTEXT_WRITE_MULTIPLE_BLOCK) != RESET))
            {
                errorstate = SDMMC_CmdStopTransfer(hmmc->Instance);
                if(errorstate != HAL_MMC_ERROR_NONE)
                {
                    hmmc->ErrorCode |= errorstate;
#if (USE_HAL_MMC_REGISTER_CALLBACKS == 1)
                    hmmc->ErrorCallback(hmmc);
#else
                    HAL_MMC_ErrorCallback(hmmc);
#endif
                }
            }
#endif
            hmmc->State = HAL_MMC_STATE_READY;
            if(((hmmc->Context & MMC_CONTEXT_READ_SINGLE_BLOCK) != RESET) || ((hmmc->Context & MMC_CONTEXT_READ_MULTIPLE_BLOCK) != RESET))
            {
#if (USE_HAL_MMC_REGISTER_CALLBACKS == 1)
                hmmc->RxCpltCallback(hmmc);
#else
                HAL_MMC_RxCpltCallback(hmmc);
#endif
            }
            else
            {
#if (USE_HAL_MMC_REGISTER_CALLBACKS == 1)
                hmmc->TxCpltCallback(hmmc);
#else      
                HAL_MMC_TxCpltCallback(hmmc);
#endif
            }
        }
        else if((hmmc->Context & MMC_CONTEXT_DMA) != RESET)
        {
            /* enable AutoCmd12 */
#if(defined USE_AUTO_CMD12 && !USE_AUTO_CMD12)
            if((hmmc->Context & MMC_CONTEXT_WRITE_MULTIPLE_BLOCK) != RESET)
            {
                errorstate = SDMMC_CmdStopTransfer(hmmc->Instance);
                if(errorstate != HAL_MMC_ERROR_NONE)
                {
                    hmmc->ErrorCode |= errorstate;
#if (USE_HAL_MMC_REGISTER_CALLBACKS == 1)
                    hmmc->ErrorCallback(hmmc);
#else
                    HAL_MMC_ErrorCallback(hmmc);
#endif
                }
            }
#endif
            if(((hmmc->Context & MMC_CONTEXT_READ_SINGLE_BLOCK) == RESET) && ((hmmc->Context & MMC_CONTEXT_READ_MULTIPLE_BLOCK) == RESET))
            {
                hmmc->State = HAL_MMC_STATE_READY;
#if (USE_HAL_MMC_REGISTER_CALLBACKS == 1)
                hmmc->TxCpltCallback(hmmc);
#else      
                HAL_MMC_TxCpltCallback(hmmc);
#endif
            }
        }
    }
    else if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_BWR))
    {
        __HAL_MMC_CLEAR_FLAG(hmmc, MMC_FLAG_BWR);
        MMC_Write_IT(hmmc);
    }
    else if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_BRR))
    {
        __HAL_MMC_CLEAR_FLAG(hmmc, MMC_FLAG_BRR);
        MMC_Read_IT(hmmc);
    }
    else if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_EINT))
    {
        hmmc->ErrorCode |= HAL_MMC_ERROR_HARDWARE;
        /* Set Error code */
        MMC_Check_Error(hmmc);
        /* Disable all interrupts */
        __HAL_MMC_DISABLE_IT(hmmc, MMC_IT_TC | MMC_IT_OTHER_ERRORS);
        if((hmmc->Context & MMC_CONTEXT_IT) != RESET)
        {
            /* Set the MMC state to ready to be able to start again the process */
            hmmc->State = HAL_SD_STATE_READY;
            HAL_MMC_ErrorCallback(hmmc);
        }
    }
    else if(__HAL_MMC_GET_FLAG(hmmc, MMC_FLAG_DMAINT))
    {
        if(hmmc->dmaInit->DMAx == SDIO_SDMA)
        {
            //update SDMA System Address, to resume the SDMA transaction.
            __HAL_MMC_DMA_CONTINUE_REQUEST(hmmc);
        }
        __HAL_MMC_CLEAR_FLAG(hmmc, MMC_FLAG_DMAINT);
    }
}

/**
  * @brief return the MMC state
  * @param hmmc Pointer to mmc handle
  * @retval HAL state
  */
HAL_MMC_StateTypeDef HAL_MMC_GetState(MMC_HandleTypeDef *hmmc)
{
    return hmmc->State;
}

/**
* @brief  Return the MMC error code
* @param  hmmc  Pointer to a MMC_HandleTypeDef structure that contains
  *              the configuration information.
* @retval MMC Error Code
*/
uint32_t HAL_MMC_GetError(MMC_HandleTypeDef *hmmc)
{
    return hmmc->ErrorCode;
}

/**
  * @brief Tx Transfer completed callbacks
  * @param hmmc Pointer to MMC handle
  * @retval None
  */
 __WEAK void HAL_MMC_TxCpltCallback(MMC_HandleTypeDef *hmmc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hmmc);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MMC_TxCpltCallback can be implemented in the user file
    */
}

/**
  * @brief Rx Transfer completed callbacks
  * @param hmmc Pointer MMC handle
  * @retval None
  */
__WEAK void HAL_MMC_RxCpltCallback(MMC_HandleTypeDef *hmmc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hmmc);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MMC_RxCpltCallback can be implemented in the user file
    */
}

/**
  * @brief MMC error callbacks
  * @param hmmc Pointer MMC handle
  * @retval None
  */
__WEAK void HAL_MMC_ErrorCallback(MMC_HandleTypeDef *hmmc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hmmc);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MMC_ErrorCallback can be implemented in the user file
    */
}

/**
  * @brief MMC Abort callbacks
  * @param hmmc Pointer MMC handle
  * @retval None
  */
__WEAK void HAL_MMC_AbortCallback(MMC_HandleTypeDef *hmmc)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hmmc);
    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MMC_ErrorCallback can be implemented in the user file
    */
}

#if (USE_HAL_MMC_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register a User MMC Callback
  *         To be used instead of the weak (surcharged) predefined callback 
  * @param hmmc : MMC handle
  * @param CallbackID : ID of the callback to be registered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_MMC_TX_CPLT_CB_ID    MMC Tx Complete Callback ID
  *          @arg @ref HAL_MMC_RX_CPLT_CB_ID    MMC Rx Complete Callback ID
  *          @arg @ref HAL_MMC_ERROR_CB_ID      MMC Error Callback ID
  *          @arg @ref HAL_MMC_ABORT_CB_ID      MMC Abort Callback ID
  *          @arg @ref HAL_MMC_MSP_INIT_CB_ID   MMC MspInit Callback ID 
  *          @arg @ref HAL_MMC_MSP_DEINIT_CB_ID MMC MspDeInit Callback ID  
  * @param pCallback : pointer to the Callback function
  * @retval status
  */
HAL_StatusTypeDef HAL_MMC_RegisterCallback(MMC_HandleTypeDef *hmmc, HAL_MMC_CallbackIDTypeDef CallbackId, pMMC_CallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;

  if(pCallback == NULL)
  {
    /* Update the error code */
    hmmc->ErrorCode |= HAL_MMC_ERROR_INVALID_CALLBACK;
    return HAL_ERROR;
  }

  /* Process locked */
  __HAL_LOCK(hmmc);
  
  if(hmmc->State == HAL_MMC_STATE_READY)
  {
    switch (CallbackId)
    {
    case HAL_MMC_TX_CPLT_CB_ID :
      hmmc->TxCpltCallback = pCallback;
      break;
    case HAL_MMC_RX_CPLT_CB_ID :
      hmmc->RxCpltCallback = pCallback;
      break;
    case HAL_MMC_ERROR_CB_ID :
      hmmc->ErrorCallback = pCallback;
      break;
    case HAL_MMC_ABORT_CB_ID :
      hmmc->AbortCpltCallback = pCallback;
      break;
    case HAL_MMC_MSP_INIT_CB_ID :
      hmmc->MspInitCallback = pCallback;
      break;
    case HAL_MMC_MSP_DEINIT_CB_ID :
      hmmc->MspDeInitCallback = pCallback;
      break;
    default :
      /* Update the error code */
      hmmc->ErrorCode |= HAL_MMC_ERROR_INVALID_CALLBACK; 
      /* update return status */
      status =  HAL_ERROR;
      break;
    }
  }
  else if (hmmc->State == HAL_MMC_STATE_RESET)
  {
    switch (CallbackId)
    {
    case HAL_MMC_MSP_INIT_CB_ID :
      hmmc->MspInitCallback = pCallback;
      break;
    case HAL_MMC_MSP_DEINIT_CB_ID :
      hmmc->MspDeInitCallback = pCallback;
      break;
    default :
      /* Update the error code */
      hmmc->ErrorCode |= HAL_MMC_ERROR_INVALID_CALLBACK; 
      /* update return status */
      status =  HAL_ERROR;
      break;
    }
  }
  else
  {
    /* Update the error code */
    hmmc->ErrorCode |= HAL_MMC_ERROR_INVALID_CALLBACK; 
    /* update return status */
    status =  HAL_ERROR;
  }

  /* Release Lock */
  __HAL_UNLOCK(hmmc);
  return status;
}

/**
  * @brief  Unregister a User MMC Callback
  *         MMC Callback is redirected to the weak (surcharged) predefined callback 
  * @param hmmc : MMC handle
  * @param CallbackID : ID of the callback to be unregistered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_MMC_TX_CPLT_CB_ID    MMC Tx Complete Callback ID
  *          @arg @ref HAL_MMC_RX_CPLT_CB_ID    MMC Rx Complete Callback ID
  *          @arg @ref HAL_MMC_ERROR_CB_ID      MMC Error Callback ID
  *          @arg @ref HAL_MMC_ABORT_CB_ID      MMC Abort Callback ID
  *          @arg @ref HAL_MMC_MSP_INIT_CB_ID   MMC MspInit Callback ID 
  *          @arg @ref HAL_MMC_MSP_DEINIT_CB_ID MMC MspDeInit Callback ID  
  * @retval status
  */
HAL_StatusTypeDef HAL_MMC_UnRegisterCallback(MMC_HandleTypeDef *hmmc, HAL_MMC_CallbackIDTypeDef CallbackId)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Process locked */
  __HAL_LOCK(hmmc);
  
  if(hmmc->State == HAL_MMC_STATE_READY)
  {
    switch (CallbackId)
    {
    case HAL_MMC_TX_CPLT_CB_ID :
      hmmc->TxCpltCallback = HAL_MMC_TxCpltCallback;
      break;
    case HAL_MMC_RX_CPLT_CB_ID :
      hmmc->RxCpltCallback = HAL_MMC_RxCpltCallback;
      break;
    case HAL_MMC_ERROR_CB_ID :
      hmmc->ErrorCallback = HAL_MMC_ErrorCallback;
      break;
    case HAL_MMC_ABORT_CB_ID :
      hmmc->AbortCpltCallback = HAL_MMC_AbortCallback;
      break;
    case HAL_MMC_MSP_INIT_CB_ID :
      hmmc->MspInitCallback = HAL_MMC_MspInit;
      break;
    case HAL_MMC_MSP_DEINIT_CB_ID :
      hmmc->MspDeInitCallback = HAL_MMC_MspDeInit;
      break;
    default :
      /* Update the error code */
      hmmc->ErrorCode |= HAL_MMC_ERROR_INVALID_CALLBACK; 
      /* update return status */
      status =  HAL_ERROR;
      break;
    }
  }
  else if (hmmc->State == HAL_MMC_STATE_RESET)
  {
    switch (CallbackId)
    {
    case HAL_MMC_MSP_INIT_CB_ID :
      hmmc->MspInitCallback = HAL_MMC_MspInit;
      break;
    case HAL_MMC_MSP_DEINIT_CB_ID :
      hmmc->MspDeInitCallback = HAL_MMC_MspDeInit;
      break;
    default :
      /* Update the error code */
      hmmc->ErrorCode |= HAL_MMC_ERROR_INVALID_CALLBACK; 
      /* update return status */
      status =  HAL_ERROR;
      break;
    }
  }
  else
  {
    /* Update the error code */
    hmmc->ErrorCode |= HAL_MMC_ERROR_INVALID_CALLBACK; 
    /* update return status */
    status =  HAL_ERROR;
  }

  /* Release Lock */
  __HAL_UNLOCK(hmmc);
  return status;
}
#endif


/**
  * @}
  */

/** @addtogroup MMC_Exported_Functions_Group3
 *  @brief   management functions 
 *
@verbatim   
  ==============================================================================
                      ##### Peripheral Control functions #####
  ==============================================================================  
  [..]
    This subsection provides a set of functions allowing to control the MMC card 
    operations and get the related information

@endverbatim
  * @{
  */

/**
  * @brief  Returns information the information of the card which are stored on
  *         the CID register.
  * @param  hmmc Pointer to MMC handle
  * @param  pCID Pointer to a HAL_MMC_CIDTypedef structure that  
  *         contains all CID register parameters 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_GetCardCID(MMC_HandleTypeDef *hmmc, HAL_MMC_CardCIDTypeDef *pCID)
{
    uint32_t tmp = 0U;
  
    /* Byte 0 */
    tmp = (uint8_t)((hmmc->CID[0U] & 0xFF000000U) >> 24U);
    pCID->ManufacturerID = tmp;

    /* Byte 1 */
    tmp = (uint8_t)((hmmc->CID[0U] & 0x00FF0000U) >> 16U);
    pCID->OEM_AppliID = tmp << 8U;

    /* Byte 2 */
    tmp = (uint8_t)((hmmc->CID[0U] & 0x000000FF00U) >> 8U);
    pCID->OEM_AppliID |= tmp;

    /* Byte 3 */
    tmp = (uint8_t)(hmmc->CID[0U] & 0x000000FFU);
    pCID->ProdName1 = tmp << 24U;

    /* Byte 4 */
    tmp = (uint8_t)((hmmc->CID[1U] & 0xFF000000U) >> 24U);
    pCID->ProdName1 |= tmp << 16U;

    /* Byte 5 */
    tmp = (uint8_t)((hmmc->CID[1U] & 0x00FF0000U) >> 16U);
    pCID->ProdName1 |= tmp << 8U;

    /* Byte 6 */
    tmp = (uint8_t)((hmmc->CID[1U] & 0x0000FF00U) >> 8U);
    pCID->ProdName1 |= tmp;

    /* Byte 7 */
    tmp = (uint8_t)(hmmc->CID[1U] & 0x000000FFU);
    pCID->ProdName2 = tmp;

    /* Byte 8 */
    tmp = (uint8_t)((hmmc->CID[2U] & 0xFF000000U) >> 24U);
    pCID->ProdRev = tmp;

    /* Byte 9 */
    tmp = (uint8_t)((hmmc->CID[2U] & 0x00FF0000U) >> 16U);
    pCID->ProdSN = tmp << 24U;

    /* Byte 10 */
    tmp = (uint8_t)((hmmc->CID[2U] & 0x0000FF00U) >> 8U);
    pCID->ProdSN |= tmp << 16U;

    /* Byte 11 */
    tmp = (uint8_t)(hmmc->CID[2U] & 0x000000FFU);
    pCID->ProdSN |= tmp << 8U;

    /* Byte 12 */
    tmp = (uint8_t)((hmmc->CID[3U] & 0xFF000000U) >> 24U);
    pCID->ProdSN |= tmp;

    /* Byte 13 */
    tmp = (uint8_t)((hmmc->CID[3U] & 0x00FF0000U) >> 16U);
    pCID->Reserved1   |= (tmp & 0xF0U) >> 4U;
    pCID->ManufactDate = (tmp & 0x0FU) << 8U;

    /* Byte 14 */
    tmp = (uint8_t)((hmmc->CID[3U] & 0x0000FF00U) >> 8U);
    pCID->ManufactDate |= tmp;

    /* Byte 15 */
    tmp = (uint8_t)(hmmc->CID[3U] & 0x000000FFU);
    pCID->CID_CRC   = (tmp & 0xFEU) >> 1U;
    pCID->Reserved2 = 1U;

    return HAL_OK;
}

/**
  * @brief  Returns information the information of the card which are stored on
  *         the CSD register.
  * @param  hmmc Pointer to MMC handle
  * @param  pCSD Pointer to a HAL_MMC_CardInfoTypeDef structure that  
  *         contains all CSD register parameters  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_GetCardCSD(MMC_HandleTypeDef *hmmc, HAL_MMC_CardCSDTypeDef *pCSD)
{
    uint32_t tmp = 0U;

    /* Byte 0 */
    tmp = (hmmc->CSD[0U] & 0xFF000000U) >> 24U;
    pCSD->CSDStruct      = (uint8_t)((tmp & 0xC0U) >> 6U);
    pCSD->SysSpecVersion = (uint8_t)((tmp & 0x3CU) >> 2U);
    pCSD->Reserved1      = tmp & 0x03U;

    /* Byte 1 */
    tmp = (hmmc->CSD[0U] & 0x00FF0000U) >> 16U;
    pCSD->TAAC = (uint8_t)tmp;

    /* Byte 2 */
    tmp = (hmmc->CSD[0U] & 0x0000FF00U) >> 8U;
    pCSD->NSAC = (uint8_t)tmp;

    /* Byte 3 */
    tmp = hmmc->CSD[0U] & 0x000000FFU;
    pCSD->MaxBusClkFrec = (uint8_t)tmp;

    /* Byte 4 */
    tmp = (hmmc->CSD[1U] & 0xFF000000U) >> 24U;
    pCSD->CardComdClasses = (uint16_t)(tmp << 4U);

    /* Byte 5 */
    tmp = (hmmc->CSD[1U] & 0x00FF0000U) >> 16U;
    pCSD->CardComdClasses |= (uint16_t)((tmp & 0xF0U) >> 4U);
    pCSD->RdBlockLen       = (uint8_t)(tmp & 0x0FU);

    /* Byte 6 */
    tmp = (hmmc->CSD[1U] & 0x0000FF00U) >> 8U;
    pCSD->PartBlockRead   = (uint8_t)((tmp & 0x80U) >> 7U);
    pCSD->WrBlockMisalign = (uint8_t)((tmp & 0x40U) >> 6U);
    pCSD->RdBlockMisalign = (uint8_t)((tmp & 0x20U) >> 5U);
    pCSD->DSRImpl         = (uint8_t)((tmp & 0x10U) >> 4U);
    pCSD->Reserved2       = 0; /*!< Reserved */
       
    pCSD->DeviceSize = (tmp & 0x03U) << 10U;

    /* Byte 7 */
    tmp = (uint8_t)(hmmc->CSD[1U] & 0x000000FFU);
    pCSD->DeviceSize |= (tmp) << 2U;

    /* Byte 8 */
    tmp = (uint8_t)((hmmc->CSD[2U] & 0xFF000000U) >> 24U);
    pCSD->DeviceSize |= (tmp & 0xC0U) >> 6U;

    pCSD->MaxRdCurrentVDDMin = (tmp & 0x38U) >> 3U;
    pCSD->MaxRdCurrentVDDMax = (tmp & 0x07U);

    /* Byte 9 */
    tmp = (uint8_t)((hmmc->CSD[2U] & 0x00FF0000U) >> 16U);
    pCSD->MaxWrCurrentVDDMin = (tmp & 0xE0U) >> 5U;
    pCSD->MaxWrCurrentVDDMax = (tmp & 0x1CU) >> 2U;
    pCSD->DeviceSizeMul      = (tmp & 0x03U) << 1U;
    /* Byte 10 */
    tmp = (uint8_t)((hmmc->CSD[2] & 0x0000FF00U) >> 8U);
    pCSD->DeviceSizeMul |= (tmp & 0x80U) >> 7U;

    hmmc->MmcCard.BlockNbr  = (pCSD->DeviceSize + 1U) ;
    hmmc->MmcCard.BlockNbr *= (1U << (pCSD->DeviceSizeMul + 2U));
    hmmc->MmcCard.BlockSize = 1U << (pCSD->RdBlockLen);

    hmmc->MmcCard.LogBlockNbr =  (hmmc->MmcCard.BlockNbr) * ((hmmc->MmcCard.BlockSize) / 512U);
    hmmc->MmcCard.LogBlockSize = 512U;

    pCSD->EraseGrSize = (tmp & 0x40U) >> 6U;
    pCSD->EraseGrMul  = (tmp & 0x3FU) << 1U;

    /* Byte 11 */
    tmp = (uint8_t)(hmmc->CSD[2U] & 0x000000FFU);
    pCSD->EraseGrMul     |= (tmp & 0x80U) >> 7U;
    pCSD->WrProtectGrSize = (tmp & 0x7FU);

    /* Byte 12 */
    tmp = (uint8_t)((hmmc->CSD[3U] & 0xFF000000U) >> 24U);
    pCSD->WrProtectGrEnable = (tmp & 0x80U) >> 7U;
    pCSD->ManDeflECC        = (tmp & 0x60U) >> 5U;
    pCSD->WrSpeedFact       = (tmp & 0x1CU) >> 2U;
    pCSD->MaxWrBlockLen     = (tmp & 0x03U) << 2U;

    /* Byte 13 */
    tmp = (uint8_t)((hmmc->CSD[3U] & 0x00FF0000U) >> 16U);
    pCSD->MaxWrBlockLen      |= (tmp & 0xC0U) >> 6U;
    pCSD->WriteBlockPaPartial = (tmp & 0x20U) >> 5U;
    pCSD->Reserved3           = 0U;
    pCSD->ContentProtectAppli = (tmp & 0x01U);

    /* Byte 14 */
    tmp = (uint8_t)((hmmc->CSD[3U] & 0x0000FF00U) >> 8U);
    pCSD->FileFormatGrouop = (tmp & 0x80U) >> 7U;
    pCSD->CopyFlag         = (tmp & 0x40U) >> 6U;
    pCSD->PermWrProtect    = (tmp & 0x20U) >> 5U;
    pCSD->TempWrProtect    = (tmp & 0x10U) >> 4U;
    pCSD->FileFormat       = (tmp & 0x0CU) >> 2U;
    pCSD->ECC              = (tmp & 0x03U);

    /* Byte 15 */
    tmp = (uint8_t)(hmmc->CSD[3U] & 0x000000FFU);
    pCSD->CSD_CRC   = (tmp & 0xFEU) >> 1U;
    pCSD->Reserved4 = 1U;

    return HAL_OK;
}

/**
  * @brief  Gets the MMC card info.
  * @param  hmmc Pointer to MMC handle      
  * @param  pCardInfo Pointer to the HAL_MMC_CardInfoTypeDef structure that 
  *         will contain the MMC card status information 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_GetCardInfo(MMC_HandleTypeDef *hmmc, HAL_MMC_CardInfoTypeDef *pCardInfo)
{
    pCardInfo->CardType     = (uint32_t)(hmmc->MmcCard.CardType);
    pCardInfo->Class        = (uint32_t)(hmmc->MmcCard.Class);
    pCardInfo->RelCardAdd   = (uint32_t)(hmmc->MmcCard.RelCardAdd);
    pCardInfo->BlockNbr     = (uint32_t)(hmmc->MmcCard.BlockNbr);
    pCardInfo->BlockSize    = (uint32_t)(hmmc->MmcCard.BlockSize);
    pCardInfo->LogBlockNbr  = (uint32_t)(hmmc->MmcCard.LogBlockNbr);
    pCardInfo->LogBlockSize = (uint32_t)(hmmc->MmcCard.LogBlockSize);

    return HAL_OK;
}

/**
  * @brief  Enables wide bus operation for the requested card if supported by 
  *         card.
  * @param  hmmc Pointer to MMC handle       
  * @param  WideMode Specifies the MMC card wide bus mode 
  *          This parameter can be one of the following values:
  *            @arg SDIO_BUS_WIDE_8B: 8-bit data transfer
  *            @arg SDIO_BUS_WIDE_4B: 4-bit data transfer
  *            @arg SDIO_BUS_WIDE_1B: 1-bit data transfer
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_ConfigWideBusOperation(MMC_HandleTypeDef *hmmc, uint32_t WideMode)
{
    __IO uint32_t count = 0U;
    SDIO_InitTypeDef Init;
    uint32_t errorstate = HAL_MMC_ERROR_NONE;
    uint32_t response = 0U, busy = 0U;

    /* Check the parameters */
    assert_param(IS_SDIO_BUS_WIDE(WideMode));

    /* Chnage Satte */
    hmmc->State = HAL_MMC_STATE_BUSY;

    /* Update Clock for Bus mode update */
    Init.SpeedMode           = SDIO_Speed_Mode_Full;
    Init.ClockMode           = SDIO_Clock_Mode_10Bit_Divider;
    Init.BusWide             = WideMode;
    Init.ClockDiv            = SDIO_INIT_CLK_DIV;
    /* Initialize SDIO*/
    SDIO_Init(hmmc->Instance, Init);
    
    if(WideMode == SDIO_BUS_WIDE_8B)
    {
        errorstate = SDMMC_CmdSwitch(hmmc->Instance, 0x03B70200U);
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            hmmc->ErrorCode |= errorstate;
        }
    }
    else if(WideMode == SDIO_BUS_WIDE_4B)
    {
        errorstate = SDMMC_CmdSwitch(hmmc->Instance, 0x03B70100U);
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
          hmmc->ErrorCode |= errorstate;
        }
    }
    else if(WideMode == SDIO_BUS_WIDE_1B)
    {
        errorstate = SDMMC_CmdSwitch(hmmc->Instance, 0x03B70000U);
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            hmmc->ErrorCode |= errorstate;
        }
    }
    else
    {
        /* WideMode is not a valid argument*/
        hmmc->ErrorCode |= HAL_MMC_ERROR_PARAM;
    }
  
    /* Check for switch error and violation of the trial number of sending CMD 13 */
    while(busy == 0U)
    {
        if(count++ == SDMMC_MAX_TRIAL)
        {
            hmmc->State = HAL_MMC_STATE_READY;
            hmmc->ErrorCode |= HAL_MMC_ERROR_REQUEST_NOT_APPLICABLE;
            return HAL_ERROR;
        }

        /* While card is not ready for data and trial number for sending CMD13 is not exceeded */
        errorstate = SDMMC_CmdSendStatus(hmmc->Instance, (uint32_t)(((uint32_t)hmmc->MmcCard.RelCardAdd) << 16U));
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            hmmc->ErrorCode |= errorstate;
        }

        /* Get command response */
        response = SDIO_GetResponse(hmmc->Instance, SDIO_RESP0);

        /* Get operating voltage*/
        busy = (((response >> 7U) == 1U) ? 0U : 1U);
    }
      
    /* While card is not ready for data and trial number for sending CMD13 is not exceeded */
    count = SDMMC_DATATIMEOUT;
    while((response & 0x00000100U) == 0U)
    {
        if(count-- == 0U)
        {
            hmmc->State = HAL_MMC_STATE_READY;
            hmmc->ErrorCode |= HAL_MMC_ERROR_REQUEST_NOT_APPLICABLE;
            return HAL_ERROR;
        }
        /* While card is not ready for data and trial number for sending CMD13 is not exceeded */
        errorstate = SDMMC_CmdSendStatus(hmmc->Instance, (uint32_t)(((uint32_t)hmmc->MmcCard.RelCardAdd) << 16U));
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            hmmc->ErrorCode |= errorstate;
        }
        /* Get command response */
        response = SDIO_GetResponse(hmmc->Instance, SDIO_RESP0);
    }
    if(hmmc->ErrorCode != HAL_MMC_ERROR_NONE)
    {
        /* Clear all the static flags */
        __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
        hmmc->State = HAL_MMC_STATE_READY;
        return HAL_ERROR;
    }
    else
    {
        /* Configure the SDIO peripheral */
        Init.SpeedMode           = hmmc->Init.SpeedMode;
        Init.ClockMode           = hmmc->Init.ClockMode;
        Init.BusWide             = WideMode;
        Init.ClockDiv            = hmmc->Init.ClockDiv;
        SDIO_Init(hmmc->Instance, Init);
    }
    /* Change State */
    hmmc->State = HAL_MMC_STATE_READY;
    return HAL_OK;
}


/**
  * @brief  Gets the current mmc card data state.
  * @param  hmmc pointer to MMC handle
  * @retval Card state
  */
HAL_MMC_CardStateTypeDef HAL_MMC_GetCardState(MMC_HandleTypeDef *hmmc)
{
    HAL_MMC_CardStateTypeDef cardstate =  HAL_MMC_CARD_TRANSFER;
    uint32_t errorstate = HAL_MMC_ERROR_NONE;
    uint32_t resp1 = 0U;

    errorstate = MMC_SendStatus(hmmc, &resp1);
    if(errorstate != HAL_OK)
    {
        hmmc->ErrorCode |= errorstate;
    }

    cardstate = (HAL_MMC_CardStateTypeDef)((resp1 >> 9U) & 0x0FU);

    return cardstate;
}

/**
  * @brief  Abort the current transfer and disable the MMC.
  * @param  hmmc pointer to a MMC_HandleTypeDef structure that contains
  *                the configuration information for MMC module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_Abort(MMC_HandleTypeDef *hmmc)
{
    HAL_MMC_CardStateTypeDef CardState;
    /* DIsable All interrupts */
    __HAL_MMC_DISABLE_IT(hmmc, MMC_IT_TC | MMC_IT_EDCRC | MMC_IT_EDT | MMC_IT_EDEB);
    /* Clear All flags */
    __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
    hmmc->State = HAL_MMC_STATE_READY;
    CardState = HAL_MMC_GetCardState(hmmc);
    if((CardState == HAL_MMC_CARD_RECEIVING) || (CardState == HAL_MMC_CARD_SENDING))
    {
        hmmc->ErrorCode = SDMMC_CmdStopTransfer(hmmc->Instance);
    }
    if(hmmc->ErrorCode != HAL_MMC_ERROR_NONE)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

/**
  * @brief  Abort the current transfer and disable the MMC (IT mode).
  * @param  hmmc pointer to a MMC_HandleTypeDef structure that contains
  *                the configuration information for MMC module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_MMC_Abort_IT(MMC_HandleTypeDef *hmmc)
{
    HAL_MMC_CardStateTypeDef CardState;
    /* DIsable All interrupts */
    __HAL_MMC_DISABLE_IT(hmmc, MMC_IT_TC | MMC_IT_EDCRC | MMC_IT_EDT | MMC_IT_EDEB);
    /* Clear All flags */
    __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
    /* No transfer ongoing on both DMA channels*/
    CardState = HAL_MMC_GetCardState(hmmc);
    hmmc->State = HAL_MMC_STATE_READY;
    if((CardState == HAL_MMC_CARD_RECEIVING) || (CardState == HAL_MMC_CARD_SENDING))
    {
        hmmc->ErrorCode = SDMMC_CmdStopTransfer(hmmc->Instance);
    }
    if(hmmc->ErrorCode != HAL_MMC_ERROR_NONE)
    {
        return HAL_ERROR;
    }
    else
    {
        HAL_MMC_AbortCallback(hmmc);
    }
    return HAL_OK;
}

/**
  * @}
  */
  
/**
  * @}
  */
  
/* Private function ----------------------------------------------------------*/  
/** @addtogroup MMC_Private_Functions
  * @{
  */

/**
  * @brief  DMA MMC transmit process complete callback 
  * @param  hdma DMA handle
  * @retval None
  */
static void MMC_DMATransmitCplt(DMA_HandleTypeDef *hdma)     
{
    MMC_HandleTypeDef* hmmc = (MMC_HandleTypeDef*)(hdma->Parent);
    /* Enable DATAEND Interrupt */
    __HAL_MMC_ENABLE_IT(hmmc, MMC_IT_TC);
}

/**
  * @brief  DMA MMC receive process complete callback 
  * @param  hdma DMA handle
  * @retval None
  */
static void MMC_DMAReceiveCplt(DMA_HandleTypeDef *hdma)  
{
    MMC_HandleTypeDef* hmmc = (MMC_HandleTypeDef* )(hdma->Parent);
    uint32_t errorstate = HAL_MMC_ERROR_NONE;
  
    /* Send stop command in multiblock write */
    if(hmmc->Context == (MMC_CONTEXT_READ_MULTIPLE_BLOCK | MMC_CONTEXT_DMA))
    {
        errorstate = SDMMC_CmdStopTransfer(hmmc->Instance);
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            hmmc->ErrorCode |= errorstate;
#if (USE_HAL_MMC_REGISTER_CALLBACKS == 1)
            hmmc->ErrorCallback(hmmc);
#else
            HAL_MMC_ErrorCallback(hmmc);
#endif
        }
    }
    /* Disable the DMA transfer for transmit request by setting the DMAEN bit
    in the MMC DCTRL register */
    __HAL_MMC_DMA_DISABLE(hmmc);
    /* Clear all the static flags */
    __HAL_MMC_CLEAR_FLAG(hmmc, SDIO_STATIC_FLAGS);
    hmmc->State = HAL_MMC_STATE_READY;
#if (USE_HAL_MMC_REGISTER_CALLBACKS == 1)
    hmmc->RxCpltCallback(hmmc);
#else
    HAL_MMC_RxCpltCallback(hmmc);
#endif
}

/**
  * @brief  DMA MMC communication error callback 
  * @param  hdma DMA handle
  * @retval None
  */
static void MMC_DMAError(DMA_HandleTypeDef *hdma)   
{
    MMC_HandleTypeDef* hmmc = (MMC_HandleTypeDef* )(hdma->Parent);
#if (USE_HAL_MMC_REGISTER_CALLBACKS == 1)
    hmmc->ErrorCallback(hmmc);
#else
    HAL_MMC_ErrorCallback(hmmc);
#endif

}

/**
  * @brief  DMA MMC Tx Abort callback 
  * @param  hdma DMA handle
  * @retval None
  */
static void MMC_DMATxAbort(DMA_HandleTypeDef *hdma)   
{
    UNUSED(hdma);
}

/**
  * @brief  DMA MMC Rx Abort callback 
  * @param  hdma DMA handle
  * @retval None
  */
static void MMC_DMARxAbort(DMA_HandleTypeDef *hdma)   
{
    UNUSED(hdma);
}


/**
  * @brief  Initializes the mmc card.
  * @param  hmmc Pointer to MMC handle
  * @retval MMC Card error state
  */
static uint32_t MMC_InitCard(MMC_HandleTypeDef *hmmc)
{
    HAL_MMC_CardCSDTypeDef CSD;
    uint32_t errorstate = HAL_MMC_ERROR_NONE;
    uint16_t mmc_rca = 1;

    /* Check the power State */
    if(SDIO_GetPowerState(hmmc->Instance) == 0U)
    {
        /* Power off */
        return HAL_MMC_ERROR_REQUEST_NOT_APPLICABLE;
    }
    /* Send CMD2 ALL_SEND_CID */
    errorstate = SDMMC_CmdSendCID(hmmc->Instance);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
        return errorstate;
    }
    else
    {
        /* Get Card identification number data */
        hmmc->CID[0U] = SDIO_GetResponse(hmmc->Instance, SDIO_RESP0);
        hmmc->CID[1U] = SDIO_GetResponse(hmmc->Instance, SDIO_RESP1);
        hmmc->CID[2U] = SDIO_GetResponse(hmmc->Instance, SDIO_RESP2);
        hmmc->CID[3U] = SDIO_GetResponse(hmmc->Instance, SDIO_RESP3);
    }
    /* Send CMD3 SET_REL_ADDR with argument 0 */
    /* MMC Card publishes its RCA. */
    errorstate = SDMMC_CmdSetRelAdd(hmmc->Instance, &mmc_rca);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
        return errorstate;
    }
    /* Get the MMC card RCA */
    hmmc->MmcCard.RelCardAdd = mmc_rca;
    /* Send CMD9 SEND_CSD with argument as card's RCA */
    errorstate = SDMMC_CmdSendCSD(hmmc->Instance, (uint32_t)(hmmc->MmcCard.RelCardAdd << 16U));
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
        return errorstate;
    }
    else
    {
        /* Get Card Specific Data */
        hmmc->CSD[0U] = SDIO_GetResponse(hmmc->Instance, SDIO_RESP0);
        hmmc->CSD[1U] = SDIO_GetResponse(hmmc->Instance, SDIO_RESP1);
        hmmc->CSD[2U] = SDIO_GetResponse(hmmc->Instance, SDIO_RESP2);
        hmmc->CSD[3U] = SDIO_GetResponse(hmmc->Instance, SDIO_RESP3);
    }
  
    /* Get the Card Class */
    hmmc->MmcCard.Class = (SDIO_GetResponse(hmmc->Instance, SDIO_RESP1) >> 20U);
    /* Get CSD parameters */
    HAL_MMC_GetCardCSD(hmmc, &CSD);
    /* Select the Card */
    errorstate = SDMMC_CmdSelDesel(hmmc->Instance, (uint32_t)(((uint32_t)hmmc->MmcCard.RelCardAdd) << 16U));
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
        return errorstate;
    }
    /* Configure SDIO peripheral interface */
    SDIO_Init(hmmc->Instance, hmmc->Init);
    /* All cards are initialized */
    return HAL_MMC_ERROR_NONE;
}

/**
  * @brief  Enquires cards about their operating voltage and configures clock
  *         controls and stores MMC information that will be needed in future
  *         in the MMC handle.
  * @param  hmmc Pointer to MMC handle
  * @retval error state
  */
static uint32_t MMC_PowerON(MMC_HandleTypeDef *hmmc)
{
    __IO uint32_t count = 0U;
    uint32_t response = 0U, validvoltage = 0U;
    uint32_t errorstate = HAL_MMC_ERROR_NONE;
  
    /* CMD0: GO_IDLE_STATE */
    errorstate = SDMMC_CmdGoIdleState(hmmc->Instance);
    if(errorstate != HAL_MMC_ERROR_NONE)
    {
        return errorstate;
    }

    while(validvoltage == 0U)
    {
        if(count++ == SDMMC_MAX_VOLT_TRIAL)
        {
            return HAL_MMC_ERROR_INVALID_VOLTRANGE;
        }
        /* SEND CMD1 APP_CMD with MMC_HIGH_VOLTAGE_RANGE(0xC0FF8000) as argument */
        errorstate = SDMMC_CmdOpCondition(hmmc->Instance, eMMC_HIGH_VOLTAGE_RANGE);
        if(errorstate != HAL_MMC_ERROR_NONE)
        {
            return HAL_MMC_ERROR_UNSUPPORTED_FEATURE;
        }
        /* Get command response */
        response = SDIO_GetResponse(hmmc->Instance, SDIO_RESP0);
        /* Get operating voltage*/
        validvoltage = (((response >> 31U) == 1U) ? 1U : 0U);
    }
    /* When power routine is finished and command returns valid voltage */
    if ((response & eMMC_HIGH_VOLTAGE_RANGE) == MMC_HIGH_VOLTAGE_RANGE)
    {
        /* When voltage range of the card is within 2.7V and 3.6V */
        hmmc->MmcCard.CardType = MMC_HIGH_VOLTAGE_CARD;
    }
    else
    {
        /* When voltage range of the card is within 1.65V and 1.95V or 2.7V and 3.6V */
        hmmc->MmcCard.CardType = MMC_DUAL_VOLTAGE_CARD;
    }
    return HAL_MMC_ERROR_NONE;
}

/**
  * @brief  Turns the SDIO output signals off.
  * @param  hmmc Pointer to MMC handle
  * @retval HAL status
  */
static HAL_StatusTypeDef MMC_PowerOFF(MMC_HandleTypeDef *hmmc)
{
    /* Set Power State to OFF */
    SDIO_PowerState_OFF(hmmc->Instance);
    return HAL_OK;
}

/**
  * @brief  Returns the current card's status.
  * @param  hmmc Pointer to MMC handle
  * @param  pCardStatus pointer to the buffer that will contain the MMC card 
  *         status (Card Status register)  
  * @retval error state
  */
static uint32_t MMC_SendStatus(MMC_HandleTypeDef *hmmc, uint32_t *pCardStatus)
{
    uint32_t errorstate = HAL_MMC_ERROR_NONE;

    if(pCardStatus == NULL)
    {
        return HAL_MMC_ERROR_PARAM;
    }
    /* Send Status command */
    errorstate = SDMMC_CmdSendStatus(hmmc->Instance, (uint32_t)(hmmc->MmcCard.RelCardAdd << 16U));
    if(errorstate != HAL_OK)
    {
        return errorstate;
    }
    /* Get MMC card status */
    *pCardStatus = SDIO_GetResponse(hmmc->Instance, SDIO_RESP0);
    return HAL_MMC_ERROR_NONE;
}

/**
  * @brief  Wrap up reading in non-blocking mode.
  * @param  hmmc pointer to a MMC_HandleTypeDef structure that contains
  *              the configuration information.
  * @retval HAL status
  */
static HAL_StatusTypeDef MMC_Read_IT(MMC_HandleTypeDef *hmmc)
{
    uint32_t count;
    uint32_t read_cnt = (hmmc->Instance->Slot->BLOCK_CSR & SD_BLOCK_CSR_TBS) / 4;
    uint32_t* tmp;

    tmp = (uint32_t*)hmmc->pRxBuffPtr;
    /* Read data from SDMMC Rx FIFO */
    for(count = 0U; count < read_cnt; count++)
    {
        SDIO_ReadFIFO(hmmc->Instance, tmp + count);
    }
    hmmc->pRxBuffPtr += read_cnt;
    return HAL_OK;
}

/**
  * @brief  Wrap up writing in non-blocking mode.
  * @param  hmmc pointer to a MMC_HandleTypeDef structure that contains
  *              the configuration information.
  * @retval HAL status
  */
static HAL_StatusTypeDef MMC_Write_IT(MMC_HandleTypeDef *hmmc)
{
    uint32_t count;
    uint32_t write_cnt = (hmmc->Instance->Slot->BLOCK_CSR & SD_BLOCK_CSR_TBS) / 4;
    uint32_t* tmp;

    tmp = (uint32_t*)hmmc->pTxBuffPtr;
    /* Write data to SDMMC Tx FIFO */
    for(count = 0U; count < write_cnt; count++)
    {
        SDIO_WriteFIFO(hmmc->Instance, (tmp + count));
    }
    hmmc->pTxBuffPtr += write_cnt;
    return HAL_OK;
}


#endif /* HAL_MMC_MODULE_ENABLED */

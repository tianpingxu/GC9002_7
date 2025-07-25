/*
 * Copyright (c) 2022-2050, GraphichinaSX Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-18     huyt         V1.0.1
 */
#include "gc90xx_hal.h"

#ifdef HAL_DMA_MODULE_ENABLED

#define HAL_TIMEOUT_DMA_ABORT    5U  /* 5 ms */
static void DMA_SetConfig(DMA_HandleTypeDef *hdma, DMA_ChannelType chx, uint64_t SrcAddress, uint64_t DstAddress, uint32_t DataLength);
static HAL_StatusTypeDef DMA_ChannelCheckInitStruct(DMAChannel_InitTypeDef* DMAChannel_InitStruct);
static void HAL_DMA_Callback_Init(DMA_HandleTypeDef *hdma);
static bool IS_DMA_BLOCK_TS(DMA_HandleTypeDef *hdma, uint32_t block_ts);


static bool IS_DMA_BLOCK_TS(DMA_HandleTypeDef *hdma, uint32_t block_ts)
{
    switch(hdma->Init.mChannel)
    {
        case DMA_CHANNEL1: return (block_ts <= DMA_CHANNEL_1_BLOCK_TS_MAX);
        case DMA_CHANNEL2: return (block_ts <= DMA_CHANNEL_2_BLOCK_TS_MAX);
        case DMA_CHANNEL3: return (block_ts <= DMA_CHANNEL_3_BLOCK_TS_MAX);
        case DMA_CHANNEL4: return (block_ts <= DMA_CHANNEL_4_BLOCK_TS_MAX);
        case DMA_CHANNEL5: return (block_ts <= DMA_CHANNEL_5_BLOCK_TS_MAX);
        case DMA_CHANNEL6: return (block_ts <= DMA_CHANNEL_6_BLOCK_TS_MAX);
        case DMA_CHANNEL7: return (block_ts <= DMA_CHANNEL_7_BLOCK_TS_MAX);
        case DMA_CHANNEL8: return (block_ts <= DMA_CHANNEL_8_BLOCK_TS_MAX);
        default: return false;
    }
}
static uint32_t DMA_GetBlockSize(DMA_HandleTypeDef *hdma, uint32_t DataLength)
{
    uint32_t block_size = 0;
    switch(hdma->Init.mS_TransferWidth)
    {
        case DMA_TR_WIDTH_8bits: 
            block_size = DataLength;
        break;
        case DMA_TR_WIDTH_16bits:
            block_size = DataLength/2;
        break;
        case DMA_TR_WIDTH_32bits:
            block_size = DataLength/4;
        break;
        case DMA_TR_WIDTH_64bits:
            block_size = DataLength/8;
        break;
        case DMA_TR_WIDTH_128bits:
            block_size = DataLength/16;
        break;
        case DMA_TR_WIDTH_256bits:
            block_size = DataLength/32;
        break;
        default: break;;
    }
    return block_size;
}
/**
  * @brief  Sets the DMA Transfer parameter.
  * @param  hdma       pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Stream.
  * @param  chx DMA channel
  * @param  SrcAddress The source memory Buffer address
  * @param  DstAddress The destination memory Buffer address
  * @param  DataLength The number programmed into BLOCK_TS field indicates the total number of data of width CHx_CTL.SRC_TR_WIDTH
  *                     to be transferred in a DMA block transfer
  * @retval HAL status
  */
static void DMA_SetConfig(DMA_HandleTypeDef *hdma, DMA_ChannelType chx, uint64_t SrcAddress, uint64_t DstAddress, uint32_t DataLength)
{
    uint64_t tempreg = 0;
    hdma->Init.mS_Address = SrcAddress;
    hdma->Init.mD_Address = DstAddress;
    hdma->Init.mBlockSize = DMA_GetBlockSize(hdma, DataLength);
    /* Check the parameters */
    assert_param(IS_DMA_BLOCK_TS(hdma, hdma->Init.mBlockSize));
    /* set source address. */
    tempreg = hdma->Instance->CHx[chx].SAR.reg_u64val;
    (((union _DMA_SAR *)&tempreg)->bit.SAR) = SrcAddress;
    hdma->Instance->CHx[chx].SAR.reg_u64val = tempreg;

    /* set destination address. */
    tempreg = hdma->Instance->CHx[chx].DAR.reg_u64val;
    (((union _DMA_DAR *)&tempreg)->bit.DAR) = DstAddress;
    hdma->Instance->CHx[chx].DAR.reg_u64val = tempreg;

    /* set transfer block size. Block Transfer Size = BLOCK_TS */
    tempreg = hdma->Instance->CHx[chx].CTL.reg_u64val;
    ((union _DMA_CTL *)&tempreg)->bit.BLOCK_TS = hdma->Init.mBlockSize;
    hdma->Instance->CHx[chx].CTL.reg_u64val = tempreg;
}
/**
  * @brief  Initialize the DMA struct to the specified
  *         parameters in the DMAChannel_InitTypeDef.
  * @param  DMAChannel_InitType Pointer to a DMAChannel_InitTypeDef structure that contains
  *               the configuration information for the specified DMA Stream.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_StructInit(DMAChannel_InitTypeDef* DMAChannel_InitType)
{
    assert_param(DMAChannel_InitType != NULL);
    DMAChannel_InitType->mChannel = DMA_CHANNEL1;
    DMAChannel_InitType->mS_Address = 0;/*!< Specifies Source Address.             */
    DMAChannel_InitType->mD_Address = 0;/*!< Specifies Destination Address.        */
    DMAChannel_InitType->mBlockSize = 0;/*!<[1 ~ 1024]                             */
    DMAChannel_InitType->mS_AutoReload_EN = DMA_DISABLE;
    DMAChannel_InitType->mD_AutoReload_EN = DMA_DISABLE;
    DMAChannel_InitType->mD_TransferWidth = DMA_TR_WIDTH_8bits;
    DMAChannel_InitType->mS_TransferWidth = DMA_TR_WIDTH_8bits;
    DMAChannel_InitType->mD_BurstTransactionLen = DMA_DATA_ITEM_1;
    DMAChannel_InitType->mS_BurstTransactionLen = DMA_DATA_ITEM_1;
    DMAChannel_InitType->mD_AddrInc = DMA_FIXED;
    DMAChannel_InitType->mS_AddrInc = DMA_FIXED;
    DMAChannel_InitType->mChannelPriority = DMA_MIN_CHANNEL_PRIORITY;
    DMAChannel_InitType->mTransferType_FlowControl = DMA_TT_MEM_TO_MEM_DMAC;
    DMAChannel_InitType->mD_Peripheral = 0;
    DMAChannel_InitType->mS_Peripheral = 0;
    DMAChannel_InitType->mEnable = DMA_DISABLE;
    return HAL_OK;
}
/**
  * @brief  Check the param of DMAChannel_InitTypeDef structure
  * @param  DMAChannel_InitStruct Pointer to a DMAChannel_InitTypeDef structure that contains
  *               the configuration information for the specified DMA Stream.
  * @retval 0: Parameter is invalid  1: Parameter is effective
  */
static HAL_StatusTypeDef DMA_ChannelCheckInitStruct(DMAChannel_InitTypeDef* DMAChannel_InitStruct)
{
    if(DMAChannel_InitStruct == NULL)
    {
        return HAL_ERROR;
    }
    if(DMAChannel_InitStruct->mChannel > DMA_NUM_CHANNELS)
    {
        return HAL_ERROR;
    }
    if(DMAChannel_InitStruct->mEnable > DMA_ENABLE)
    {
        return HAL_ERROR;
    }
    if(DMAChannel_InitStruct->mS_Peripheral > DMA_NUMBER_OF_HARDWARE_HS_INTERFACE ||
       DMAChannel_InitStruct->mD_Peripheral > DMA_NUMBER_OF_HARDWARE_HS_INTERFACE)
    {
        return HAL_ERROR;
    }
    if(DMAChannel_InitStruct->mTransferType_FlowControl > DMA_TT_PER_TO_PER_DST_PER)
    {
        return HAL_ERROR;
    }
    if(DMAChannel_InitStruct->mChannelPriority > DMA_MAX_CHANNEL_PRIORITY)
    {
        return HAL_ERROR;
    }
    if(DMAChannel_InitStruct->mS_BurstTransactionLen > DMA_DATA_ITEMS_1024 ||
       DMAChannel_InitStruct->mD_BurstTransactionLen > DMA_DATA_ITEMS_1024)
    {
        return HAL_ERROR;
    }
    if(DMAChannel_InitStruct->mS_TransferWidth > DMA_TR_WIDTH_32bits ||
       DMAChannel_InitStruct->mD_TransferWidth > DMA_TR_WIDTH_32bits)
    {
        return HAL_ERROR;
    }
    if(DMAChannel_InitStruct->mS_AddrInc > DMA_FIXED ||
       DMAChannel_InitStruct->mD_AddrInc > DMA_FIXED)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}
/**
  * @brief  Initialize the DMA according to the specified
  *         parameters in the DMA_InitTypeDef and create the associated handle.
  * @param  hdma Pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Stream.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Init(DMA_HandleTypeDef *hdma)
{
    uint64_t tempreg = 0;
    uint64_t u64S_MasterSelect = 0;
    uint64_t u64D_MasterSelect = 0;
    uint64_t u64S_SoftwareOrHardwareHandshaking = 0;
    uint64_t u64D_SoftwareOrHardwareHandshaking = 0;

    /* Check the DMA peripheral state */
    assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
    assert_param(DMA_ChannelCheckInitStruct(&(hdma->Init)) == HAL_OK);
    union _DMA_CTL dma_reg_DMA_CTL;
    union _DMA_CFG dma_reg_DMA_CFG;
    dma_reg_DMA_CTL.reg_u64val = hdma->Instance->CHx[hdma->Init.mChannel].CTL.reg_u64val;
    dma_reg_DMA_CFG.reg_u64val = hdma->Instance->CHx[hdma->Init.mChannel].CFG.reg_u64val;
	
    dma_reg_DMA_CTL.bit.INT_EN = 1;
    dma_reg_DMA_CTL.bit.DST_TR_WIDTH = hdma->Init.mD_TransferWidth;
    dma_reg_DMA_CTL.bit.SRC_TR_WIDTH = hdma->Init.mS_TransferWidth;
    dma_reg_DMA_CTL.bit.DINC = hdma->Init.mD_AddrInc;
    dma_reg_DMA_CTL.bit.SINC = hdma->Init.mS_AddrInc;
    dma_reg_DMA_CTL.bit.DST_MSIZE = hdma->Init.mD_BurstTransactionLen;
    dma_reg_DMA_CTL.bit.SRC_MSIZE = hdma->Init.mS_BurstTransactionLen;
    dma_reg_DMA_CTL.bit.SRC_GATHER_EN = (hdma->Init.mS_AddrInc != DMA_FIXED);
    dma_reg_DMA_CTL.bit.DST_SCATTER_EN = (hdma->Init.mD_AddrInc != DMA_FIXED);
    dma_reg_DMA_CTL.bit.TT_FC = hdma->Init.mTransferType_FlowControl;
    dma_reg_DMA_CTL.bit.DMS = 0;
    dma_reg_DMA_CTL.bit.SMS = 0;
    dma_reg_DMA_CTL.bit.LLP_DST_EN = 0;
    dma_reg_DMA_CTL.bit.LLP_SRC_EN = 0;
    dma_reg_DMA_CTL.bit.BLOCK_TS = 0;
    dma_reg_DMA_CTL.bit.DONE = 0;

    switch(hdma->Init.mTransferType_FlowControl)
    {
    case DMA_TT_MEM_TO_MEM_DMAC:
        dma_reg_DMA_CTL.bit.DMS = DMA_AHB_Master1;
        dma_reg_DMA_CTL.bit.SMS = DMA_AHB_Master1;
        dma_reg_DMA_CFG.bit.HS_SEL_DST = DMA_SoftwareHandshaking;
        dma_reg_DMA_CFG.bit.HS_SEL_SRC = DMA_SoftwareHandshaking;
        break;
    case DMA_TT_MEM_TO_PER_DMAC:
    case DMA_TT_MEM_TO_PER_PER:
        dma_reg_DMA_CTL.bit.DMS = DMA_AHB_Master1;
        dma_reg_DMA_CTL.bit.SMS = DMA_AHB_Master1;
        dma_reg_DMA_CFG.bit.HS_SEL_DST = DMA_HardwareHandshaking;
        dma_reg_DMA_CFG.bit.HS_SEL_SRC = DMA_SoftwareHandshaking;
        break;
    case DMA_TT_PER_TO_MEM_DMAC:
    case DMA_TT_PER_TO_MEM_PER:
        dma_reg_DMA_CTL.bit.DMS = DMA_AHB_Master1;
        dma_reg_DMA_CTL.bit.SMS = DMA_AHB_Master1;
        dma_reg_DMA_CFG.bit.HS_SEL_DST = DMA_SoftwareHandshaking;
        dma_reg_DMA_CFG.bit.HS_SEL_SRC = DMA_HardwareHandshaking;
        break;
    case DMA_TT_PER_TO_PER_DMAC:
    case DMA_TT_PER_TO_PER_SRC_PER:
    case DMA_TT_PER_TO_PER_DST_PER:
        dma_reg_DMA_CTL.bit.DMS = DMA_AHB_Master1;
        dma_reg_DMA_CTL.bit.SMS = DMA_AHB_Master1;
        dma_reg_DMA_CFG.bit.HS_SEL_DST = DMA_HardwareHandshaking;
        dma_reg_DMA_CFG.bit.HS_SEL_SRC = DMA_HardwareHandshaking;
        break;
    default:break;
    }

    /* Set DMA CFG Register */
    dma_reg_DMA_CFG.bit.CH_PRIOR = hdma->Init.mChannelPriority;
    dma_reg_DMA_CFG.bit.CH_SUSP = 0;
    dma_reg_DMA_CFG.bit.FIFO_EMPTY = 0;
    dma_reg_DMA_CFG.bit.LOCK_CH_L = 0;
    dma_reg_DMA_CFG.bit.LOCK_B_L = 0;
    dma_reg_DMA_CFG.bit.LOCK_CH = 0;
    dma_reg_DMA_CFG.bit.LOCK_B = 0;
    dma_reg_DMA_CFG.bit.DST_HS_POL = 0; /* ACTIVE_HIGH */
    dma_reg_DMA_CFG.bit.SRC_HS_POL = 0; /* ACTIVE_HIGH */
    dma_reg_DMA_CFG.bit.MAX_ABRST = 0;
    dma_reg_DMA_CFG.bit.RELOAD_SRC = hdma->Init.mS_AutoReload_EN;
    dma_reg_DMA_CFG.bit.RELOAD_DST = hdma->Init.mD_AutoReload_EN;
    dma_reg_DMA_CFG.bit.FCMODE = 0;
    dma_reg_DMA_CFG.bit.FIFO_MODE = 0; /* FIFO_MODE_0 */
    dma_reg_DMA_CFG.bit.PROTCTL = 0x7;
    dma_reg_DMA_CFG.bit.DS_UPD_EN = 0;
    dma_reg_DMA_CFG.bit.SS_UPD_EN = 0;
    dma_reg_DMA_CFG.bit.SRC_PER = hdma->Init.mS_Peripheral;
    dma_reg_DMA_CFG.bit.DST_PER = hdma->Init.mD_Peripheral;

    /* Allocate lock resource */
    __HAL_LOCK(hdma);
    /* Change DMA peripheral state */
    hdma->State = HAL_DMA_STATE_BUSY;
    /* Disable the peripheral */
    __HAL_DMA_CHX_DISABLE(hdma, hdma->Init.mChannel);
    /* clear the error interrupt requests*/
    __HAL_DMA_CHX_CLEAR_IT(hdma, hdma->Init.mChannel, DMA_INT_ALL);
    /* Config CTL Register */
    hdma->Instance->CHx[hdma->Init.mChannel].CTL.reg_u64val = dma_reg_DMA_CTL.reg_u64val;
    /* Config CFG Register */
    hdma->Instance->CHx[hdma->Init.mChannel].CFG.reg_u64val = dma_reg_DMA_CFG.reg_u64val;

    /* enable DMA and Int. */
    tempreg = hdma->Instance->DmaCfgReg.reg_u64val;
    if(((union _DMA_DmaCfgReg *)&tempreg)->bit.DMA_EN != 1)
    {
        ((union _DMA_DmaCfgReg *)&tempreg)->bit.DMA_EN = 1;
        hdma->Instance->DmaCfgReg.reg_u64val = tempreg;
    }
    if(hdma->Init.mEnable)
    {
        DMA_SetConfig(hdma, hdma->Init.mChannel, hdma->Init.mS_Address, hdma->Init.mD_Address, hdma->Init.mBlockSize);
    }

    __HAL_DMA_CHX_DISABLE_IT(hdma, hdma->Init.mChannel, DMA_INT_ALL);
    /* Clear all interrupt flags */
    __HAL_DMA_CHX_CLEAR_IT(hdma, hdma->Init.mChannel, DMA_INT_ALL);
    /* enable channel. */
    if(hdma->Init.mEnable)
    {
        __HAL_DMA_CHX_ENABLE(hdma, hdma->Init.mChannel);
        /* Initialize the error code */
        hdma->ErrorCode = HAL_DMA_ERROR_NONE;
        /* Initialize the DMA state */
        hdma->State = HAL_DMA_STATE_BUSY;
    }
    else
    {
        __HAL_DMA_CHX_DISABLE(hdma, hdma->Init.mChannel);
        /* Initialize the error code */
        hdma->ErrorCode = HAL_DMA_ERROR_NONE;
        /* Initialize the DMA state */
        hdma->State = HAL_DMA_STATE_READY;
    }
    HAL_DMA_Callback_Init(hdma);
	/* Process unlocked */
	__HAL_UNLOCK(hdma);
    return HAL_OK;
}
/**
  * @brief  DeInitializes the DMA peripheral
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Stream.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_DeInit(DMA_HandleTypeDef *hdma)
{
    /* Check the hdma */
    assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
    /* Check the DMA peripheral state */
    if(hdma->State == HAL_DMA_STATE_BUSY)
    {
        /* Return error status */
        return HAL_BUSY;
    }
    uint64_t tempreg = 0;
    /* Check the parameters */
    assert_param(DMA_ChannelCheckInitStruct(&(hdma->Init)) == HAL_OK);
    /* Reset DMA Streamx Source address register */
    tempreg = hdma->Instance->CHx[hdma->Init.mChannel].SAR.reg_u64val;
    (((union _DMA_SAR *)&tempreg)->bit.SAR) = 0;
    hdma->Instance->CHx[hdma->Init.mChannel].SAR.reg_u64val = tempreg;
    /* Reset DMA Streamx Destination address register */
    tempreg = hdma->Instance->CHx[hdma->Init.mChannel].DAR.reg_u64val;
    (((union _DMA_DAR *)&tempreg)->bit.DAR) = 0;
    hdma->Instance->CHx[hdma->Init.mChannel].DAR.reg_u64val = tempreg;
    /* Reset DMA Streamx block-size register */
    tempreg = hdma->Instance->CHx[hdma->Init.mChannel].CTL.reg_u64val;
    ((union _DMA_CTL*)&tempreg)->bit.BLOCK_TS = 0;
    hdma->Instance->CHx[hdma->Init.mChannel].CTL.reg_u64val = tempreg;

    __HAL_DMA_CHX_DISABLE_IT(hdma, hdma->Init.mChannel, DMA_INT_ALL);
    __HAL_DMA_CHX_CLEAR_IT(hdma, hdma->Init.mChannel, DMA_INT_ALL);
    __HAL_DMA_CHX_DISABLE(hdma, hdma->Init.mChannel);
    /* Clean all callbacks */
    hdma->XferCpltCallback = NULL;
    hdma->BlockCpltCallback = NULL;
    hdma->SourceCpltCallback = NULL;
    hdma->DestinationCpltCallback = NULL;
    hdma->XferErrorCallback = NULL;
    hdma->XferAbortCallback = NULL;
    /* Reset the error code */
    hdma->ErrorCode = HAL_DMA_ERROR_NONE;
    /* Reset the DMA state */
    hdma->State = HAL_DMA_STATE_RESET;
    /* Release Lock */
    __HAL_UNLOCK(hdma);
    return HAL_OK;
}

/**
  * @brief  Starts the DMA Transfer.
  * @param  hdma       pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Stream.
  * @param  SrcAddress The source memory Buffer address
  * @param  DstAddress The destination memory Buffer address
  * @param  DataLength The length of data to be transferred from source to destination
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Start(DMA_HandleTypeDef *hdma, uint64_t SrcAddress, uint64_t DstAddress, uint32_t DataLength)
{
    HAL_StatusTypeDef status = HAL_OK;
    if(DataLength == 0)
    {
        return HAL_OK;
    }
    /* Check the hdma */
    assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
    /* Process locked */
    __HAL_LOCK(hdma);
    if(HAL_DMA_STATE_READY == hdma->State)
    {
        /* Change DMA peripheral state */
        hdma->State = HAL_DMA_STATE_BUSY;
        /* Initialize the error code */
        hdma->ErrorCode = HAL_DMA_ERROR_NONE;
        /* Configure the source, destination address and the data length */
        DMA_SetConfig(hdma, hdma->Init.mChannel, SrcAddress, DstAddress, DataLength);
#if(defined DMA_FLUSH_INVALIDATE_DCACHE_EN && DMA_FLUSH_INVALIDATE_DCACHE_EN == 1U)
        /* Flush and Invalidate Cache */
        if(ccm_IsDCacheEnabled())
        {
#if(defined DMA_FLUSH_INVALIDATE_DCACHE_ALL_EN && DMA_FLUSH_INVALIDATE_DCACHE_ALL_EN)
            ccm_FlushInvalidateDCache();
#else
            DMA_Flush_Inval_DCache(hdma, SrcAddress, DstAddress, DataLength);
#endif
        }
#endif
        /* Enable the Peripheral */
        __HAL_DMA_CHX_ENABLE(hdma, hdma->Init.mChannel);
    }
    else
    {
        /* Return error status */
        status = HAL_BUSY;
    }
	/* Process unlocked */
	__HAL_UNLOCK(hdma);
    return status;
}
/**
  * @brief  Start the DMA Transfer with interrupt enabled.
  * @param  hdma       pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Stream.
  * @param  SrcAddress The source memory Buffer address
  * @param  DstAddress The destination memory Buffer address
  * @param  DataLength Block Transfer Size
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Start_IT(DMA_HandleTypeDef *hdma, uint64_t SrcAddress, uint64_t DstAddress, uint32_t DataLength)
{
    HAL_StatusTypeDef status = HAL_OK;
    if(DataLength == 0)
    {
        return HAL_OK;
    }
    /* Check the hdma */
    assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
    /* Process locked */
    __HAL_LOCK(hdma);
    if(HAL_DMA_STATE_READY == hdma->State)
    {
        /* Change DMA peripheral state */
        hdma->State = HAL_DMA_STATE_BUSY;
        /* Initialize the error code */
        hdma->ErrorCode = HAL_DMA_ERROR_NONE;
        /* Configure the source, destination address and the data length */
        DMA_SetConfig(hdma, hdma->Init.mChannel, SrcAddress, DstAddress, DataLength);
        /* Clear all interrupt flags at correct offset within the register */
        __HAL_DMA_CHX_CLEAR_IT(hdma, hdma->Init.mChannel, DMA_INT_ALL);
        __HAL_DMA_CHX_ENABLE_IT(hdma, hdma->Init.mChannel, DMA_INT_Tfr | DMA_INT_Block | DMA_INT_Err);
#if(defined DMA_FLUSH_INVALIDATE_DCACHE_EN && DMA_FLUSH_INVALIDATE_DCACHE_EN == 1U)
        /* Flush and Invalidate Cache */
        if(ccm_IsDCacheEnabled())
        {
#if(defined DMA_FLUSH_INVALIDATE_DCACHE_ALL_EN && DMA_FLUSH_INVALIDATE_DCACHE_ALL_EN)
            ccm_FlushInvalidateDCache();
#else
            DMA_Flush_Inval_DCache(hdma, SrcAddress, DstAddress, DataLength);
#endif
        }
#endif
        /* Enable the Peripheral */
        __HAL_DMA_CHX_ENABLE(hdma, hdma->Init.mChannel);
    }
    else
    {
        /* Return error status */
        status = HAL_BUSY;
    }
	/* Process unlocked */
	__HAL_UNLOCK(hdma);
    return status;
}
/**
  * @brief  Aborts the DMA Transfer.
  * @param  hdma   pointer to a DMA_HandleTypeDef structure that contains
  *                 the configuration information for the specified DMA Stream.
  *
  * @note  After disabling a DMA Stream, a check for wait until the DMA Stream is
  *        effectively disabled is added. If a Stream is disabled
  *        while a data transfer is ongoing, the current data will be transferred
  *        and the Stream will be effectively disabled only after the transfer of
  *        this single data is finished.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Abort(DMA_HandleTypeDef *hdma)
{
    /* Check the hdma */
    assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
    uint32_t tickstart = HAL_GetTick();
    if(hdma->State != HAL_DMA_STATE_BUSY)
    {
        hdma->ErrorCode = HAL_DMA_ERROR_NO_XFER;
        /* Process Unlocked */
        __HAL_UNLOCK(hdma);
        return HAL_ERROR;
    }
    else
    {
        /* Disable all the transfer interrupts */
        __HAL_DMA_CHX_DISABLE_IT(hdma, hdma->Init.mChannel, DMA_INT_ALL);
        /* Disable the stream */
        __HAL_DMA_CHX_DISABLE(hdma, hdma->Init.mChannel);
        /* Check if the DMA Stream is effectively disabled */
        while(__HAL_DMA_CHANNEL_IS_ENABLE(hdma))
        {
            /* Check for the Timeout */
            if((HAL_GetTick() - tickstart) > HAL_TIMEOUT_DMA_ABORT)
            {
                /* Update error code */
                hdma->ErrorCode = HAL_DMA_ERROR_TIMEOUT;
                /* Process Unlocked */
                __HAL_UNLOCK(hdma);
                /* Change the DMA state */
                hdma->State = HAL_DMA_STATE_TIMEOUT;
                return HAL_TIMEOUT;
            }
        }
        /* Clear all interrupt flags at correct offset within the register */
        __HAL_DMA_CHX_CLEAR_IT(hdma, hdma->Init.mChannel, DMA_INT_ALL);
        /* Process Unlocked */
        __HAL_UNLOCK(hdma);
        /* Change the DMA state*/
        hdma->State = HAL_DMA_STATE_READY;
    }
    return HAL_OK;
}
/**
  * @brief  Aborts the DMA Transfer in Interrupt mode.
  * @param  hdma   pointer to a DMA_HandleTypeDef structure that contains
  *                 the configuration information for the specified DMA Stream.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_Abort_IT(DMA_HandleTypeDef *hdma)
{
    /* Check the hdma */
    assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
    if(hdma->State != HAL_DMA_STATE_BUSY)
    {
        hdma->ErrorCode = HAL_DMA_ERROR_NO_XFER;
        return HAL_ERROR;
    }
    else
    {
        /* Set Abort State  */
        hdma->State = HAL_DMA_STATE_ABORT;
        /* Disable all the transfer interrupts */
        __HAL_DMA_CHX_DISABLE_IT(hdma, hdma->Init.mChannel, DMA_INT_ALL);
        /* Disable the stream */
        __HAL_DMA_CHX_DISABLE(hdma, hdma->Init.mChannel);
        /* Check if the DMA Stream is effectively disabled */
        while(__HAL_DMA_CHANNEL_IS_ENABLE(hdma));
        /* Clear all interrupt flags at correct offset within the register */
        __HAL_DMA_CHX_CLEAR_IT(hdma, hdma->Init.mChannel, DMA_INT_ALL);
        /* Change the DMA state*/
        hdma->State = HAL_DMA_STATE_READY;
    }
    return HAL_OK;
}
static void HAL_DMA_Callback_Init(DMA_HandleTypeDef *hdma)
{
    /* Check the hdma */
    assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
#if(defined USE_HAL_DMA_REGISTER_CALLBACKS && USE_HAL_DMA_REGISTER_CALLBACKS == 1)
    hdma->XferCpltCallback = HAL_DMA_XferCpltCallback;
    hdma->BlockCpltCallback = HAL_DMA_BlockCpltCallback;
    hdma->SourceCpltCallback = HAL_DMA_SourceCpltCallback;
    hdma->DestinationCpltCallback = HAL_DMA_DestinationCpltCallback;
    hdma->XferErrorCallback = HAL_DMA_XferErrorCallback;
    hdma->XferAbortCallback = HAL_DMA_XferAbortCallback;
#endif
}
#if(defined USE_HAL_DMA_REGISTER_CALLBACKS && USE_HAL_DMA_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register callbacks
  * @param  hdma                 pointer to a DMA_HandleTypeDef structure that contains
  *                               the configuration information for the specified DMA Stream.
  * @param  CallbackID           User Callback identifer
  *                               a DMA_HandleTypeDef structure as parameter.
  * @param  pCallback            pointer to private callbacsk function which has pointer to
  *                               a DMA_HandleTypeDef structure as parameter.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_RegisterCallback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID, void (* pCallback)(DMA_HandleTypeDef *_hdma))
{
    HAL_StatusTypeDef status = HAL_OK;
    /* Check the hdma */
    assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
    /* Process locked */
    __HAL_LOCK(hdma);
    if(HAL_DMA_STATE_READY == hdma->State)
    {
        switch (CallbackID)
        {
        case  HAL_DMA_XFER_CPLT_CB_ID:
            hdma->XferCpltCallback = pCallback;
            break;

        case  HAL_DMA_XFER_BLOCK_CB_ID:
            hdma->BlockCpltCallback = pCallback;
            break;

        case  HAL_DMA_XFER_SRC_CB_ID:
            hdma->SourceCpltCallback = pCallback;
            break;

        case  HAL_DMA_XFER_DST_CB_ID:
            hdma->DestinationCpltCallback = pCallback;
            break;

        case  HAL_DMA_XFER_ERROR_CB_ID:
            hdma->XferErrorCallback = pCallback;
            break;

        case  HAL_DMA_XFER_ABORT_CB_ID:
            hdma->XferAbortCallback = pCallback;
            break;

        default:
            break;
        }
    }
    else
    {
        /* Return error status */
        status =  HAL_ERROR;
    }
    /* Release Lock */
    __HAL_UNLOCK(hdma);
    return status;
}
/**
  * @brief  UnRegister callbacks
  * @param  hdma                 pointer to a DMA_HandleTypeDef structure that contains
  *                               the configuration information for the specified DMA Stream.
  * @param  CallbackID           User Callback identifer
  *                               a HAL_DMA_CallbackIDTypeDef ENUM as parameter.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DMA_UnRegisterCallback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID)
{
    HAL_StatusTypeDef status = HAL_OK;
    /* Check the hdma */
    assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
    /* Process locked */
    __HAL_LOCK(hdma);
    if(HAL_DMA_STATE_READY == hdma->State)
    {
        switch (CallbackID)
        {
        case  HAL_DMA_XFER_CPLT_CB_ID:
            hdma->XferCpltCallback = NULL;
            break;
        case  HAL_DMA_XFER_BLOCK_CB_ID:
            hdma->BlockCpltCallback = NULL;
            break;
        case  HAL_DMA_XFER_SRC_CB_ID:
            hdma->SourceCpltCallback = NULL;
            break;
        case  HAL_DMA_XFER_DST_CB_ID:
            hdma->DestinationCpltCallback = NULL;
            break;
        case  HAL_DMA_XFER_ERROR_CB_ID:
            hdma->XferErrorCallback = NULL;
            break;
        case  HAL_DMA_XFER_ABORT_CB_ID:
            hdma->XferAbortCallback = NULL;
            break;
        case   HAL_DMA_XFER_ALL_CB_ID:
            hdma->XferCpltCallback = NULL;
            hdma->BlockCpltCallback = NULL;
            hdma->SourceCpltCallback = NULL;
            hdma->DestinationCpltCallback = NULL;
            hdma->XferErrorCallback = NULL;
            hdma->XferAbortCallback = NULL;
            break;
        default:
            status = HAL_ERROR;
            break;
        }
    }
    else
    {
        status = HAL_ERROR;
    }
    /* Release Lock */
    __HAL_UNLOCK(hdma);
    return status;
}
#endif /* USE_HAL_DMA_REGISTER_CALLBACKS */
/**
  * @brief  Returns the DMA state.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Stream.
  * @retval HAL state
  */
HAL_DMA_StateTypeDef HAL_DMA_GetState(DMA_HandleTypeDef *hdma)
{
    /* Check the hdma */
    assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
    return hdma->State;
}
/**
  * @brief  Return the DMA error code
  * @param  hdma  pointer to a DMA_HandleTypeDef structure that contains
  *              the configuration information for the specified DMA Stream.
  * @retval DMA Error Code
  */
uint32_t HAL_DMA_GetError(DMA_HandleTypeDef *hdma)
{
    /* Check the hdma */
    assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
    return hdma->ErrorCode;
}

__WEAK void HAL_DMA_XferCpltCallback(DMA_HandleTypeDef * hdma)
{
    UNUSED(hdma);
}
__WEAK void HAL_DMA_BlockCpltCallback(DMA_HandleTypeDef * hdma)
{
    UNUSED(hdma);
}
__WEAK void HAL_DMA_SourceCpltCallback(DMA_HandleTypeDef * hdma)
{
    UNUSED(hdma);
}
__WEAK void HAL_DMA_DestinationCpltCallback(DMA_HandleTypeDef * hdma)
{
    UNUSED(hdma);
}
__WEAK void HAL_DMA_XferErrorCallback(DMA_HandleTypeDef * hdma)
{
    UNUSED(hdma);
}
__WEAK void HAL_DMA_XferAbortCallback(DMA_HandleTypeDef * hdma)
{
    UNUSED(hdma);
}
/**
  * @brief  Handles DMA interrupt request.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *               the configuration information for the specified DMA Stream.
  * @retval None
  */
void HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma)
{
    /* Check the hdma */
    assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
    /* Transfer Complete Interrupt management ***********************************/
    if(__HAL_DMA_CHX_GET_IT_SOURCE(hdma, hdma->Init.mChannel, DMA_INT_Tfr) && __HAL_DMA_CHX_GET_IT_STATUS(hdma, hdma->Init.mChannel, DMA_INT_Tfr))
    {
        /* Disable the stream */
        __HAL_DMA_CHX_DISABLE(hdma, hdma->Init.mChannel);
        /* Clear the transfer complete flag */
        __HAL_DMA_CHX_CLEAR_IT(hdma, hdma->Init.mChannel, DMA_INT_Tfr);
        if(HAL_DMA_STATE_ABORT == hdma->State)
        {
            /* Disable all the transfer interrupts */
            __HAL_DMA_CHX_CLEAR_IT(hdma, hdma->Init.mChannel, DMA_INT_ALL);
            /* Process Unlocked */
            __HAL_UNLOCK(hdma);
            /* Change the DMA state */
            hdma->State = HAL_DMA_STATE_READY;
#if(defined USE_HAL_DMA_REGISTER_CALLBACKS && USE_HAL_DMA_REGISTER_CALLBACKS == 1)
            if(hdma->XferAbortCallback != NULL)
            {
                hdma->XferAbortCallback(hdma);
            }
#else
            HAL_DMA_XferAbortCallback(hdma);
#endif
            return;
        }
        /* Process Unlocked */
        __HAL_UNLOCK(hdma);
        /* Change the DMA state */
        hdma->State = HAL_DMA_STATE_READY;
#if(defined USE_HAL_DMA_REGISTER_CALLBACKS && USE_HAL_DMA_REGISTER_CALLBACKS == 1)
        if(hdma->XferCpltCallback != NULL)
        {
            /* Transfer complete callback */
            hdma->XferCpltCallback(hdma);
        }
#else
        HAL_DMA_XferCpltCallback(hdma);
#endif
    }
    /* Source Transaction Complete Interrupt management ***********************************/
    if(__HAL_DMA_CHX_GET_IT_SOURCE(hdma, hdma->Init.mChannel, DMA_INT_SrcTran) && __HAL_DMA_CHX_GET_IT_STATUS(hdma, hdma->Init.mChannel, DMA_INT_SrcTran))
	{
        __HAL_DMA_CHX_CLEAR_IT(hdma, hdma->Init.mChannel, DMA_INT_SrcTran);
#if(defined USE_HAL_DMA_REGISTER_CALLBACKS && USE_HAL_DMA_REGISTER_CALLBACKS == 1)
        if(hdma->SourceCpltCallback != NULL)
        {
            hdma->SourceCpltCallback(hdma);
        }
#else
        HAL_DMA_SourceCpltCallback(hdma);
#endif
	}
    /* Destination Transaction Complete Interrupt management ***********************************/
    if(__HAL_DMA_CHX_GET_IT_SOURCE(hdma, hdma->Init.mChannel, DMA_INT_DstTran) && __HAL_DMA_CHX_GET_IT_STATUS(hdma, hdma->Init.mChannel, DMA_INT_DstTran))
	{
        __HAL_DMA_CHX_CLEAR_IT(hdma, hdma->Init.mChannel, DMA_INT_DstTran);
#if(defined USE_HAL_DMA_REGISTER_CALLBACKS && USE_HAL_DMA_REGISTER_CALLBACKS == 1)
        if(hdma->DestinationCpltCallback != NULL)
        {
            hdma->DestinationCpltCallback(hdma);
        }
#else
        HAL_DMA_DestinationCpltCallback(hdma);
#endif
	}
    /*  Block  Transfer Complete Interrupt management ***********************************/
    if(__HAL_DMA_CHX_GET_IT_SOURCE(hdma, hdma->Init.mChannel, DMA_INT_Block) && __HAL_DMA_CHX_GET_IT_STATUS(hdma, hdma->Init.mChannel, DMA_INT_Block))
	{
        __HAL_DMA_CHX_CLEAR_IT(hdma, hdma->Init.mChannel, DMA_INT_Block);
#if(defined USE_HAL_DMA_REGISTER_CALLBACKS && USE_HAL_DMA_REGISTER_CALLBACKS == 1)
        if(hdma->BlockCpltCallback != NULL)
        {
            hdma->BlockCpltCallback(hdma);
        }
#else
        HAL_DMA_BlockCpltCallback(hdma);
#endif
	}
    /*  Error Interrupt management ***********************************/
    if(__HAL_DMA_CHX_GET_IT_SOURCE(hdma, hdma->Init.mChannel, DMA_INT_Err) && __HAL_DMA_CHX_GET_IT_STATUS(hdma, hdma->Init.mChannel, DMA_INT_Err))
	{
        __HAL_DMA_CHX_CLEAR_IT(hdma, hdma->Init.mChannel, DMA_INT_Err);
        /* Change the DMA state */
        hdma->State = HAL_DMA_STATE_ERROR;
	}
    /* manage error case */
    if(hdma->ErrorCode != HAL_DMA_ERROR_NONE)
    {
        if((hdma->ErrorCode & HAL_DMA_ERROR_TE) != RESET)
        {
            hdma->State = HAL_DMA_STATE_ABORT;
            /* Disable the stream */
            __HAL_DMA_CHX_DISABLE(hdma, hdma->Init.mChannel);
            /* Process Unlocked */
            __HAL_UNLOCK(hdma);
            /* Change the DMA state */
            hdma->State = HAL_DMA_STATE_READY;
        }
#if(defined USE_HAL_DMA_REGISTER_CALLBACKS && USE_HAL_DMA_REGISTER_CALLBACKS == 1)
        if(hdma->XferErrorCallback != NULL)
        {
            /* Transfer error callback */
            hdma->XferErrorCallback(hdma);
        }
#else
        HAL_DMA_XferErrorCallback(hdma);
#endif
    }
}
/**
  * @brief  Get channel interrupt source.
  * @param  hdma DMA handle
  * @param  chx DMA channel
  * @param  int_source specifies the DMA interrupt source to check.
  *         This parameter can be one of the following values: enum DMA_INT_Source
  *            @arg DMA_INT_Tfr:
  *            @arg DMA_INT_Block:
  *            @arg DMA_INT_SrcTran:
  *            @arg DMA_INT_DstTran:
  *            @arg DMA_INT_Err:
  *            @arg DMA_INT_ALL:
  */
bool __HAL_DMA_CHX_GET_IT_SOURCE(DMA_HandleTypeDef *hdma, DMA_ChannelType chx, uint32_t int_source)
{
    /* Check the hdma */
    assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
    uint64_t tempreg = 0;
    if(int_source & DMA_INT_Tfr)
    {
        tempreg = hdma->Instance->MaskTfr.reg_u64val;
		return ((((union _DMA_MaskTfr *)&tempreg)->bit.INT_MASK & (0x1 << chx)) != RESET);
    }
    if(int_source & DMA_INT_Block)
    {
        tempreg = hdma->Instance->MaskBlock.reg_u64val;
		return ((((union _DMA_MaskBlock *)&tempreg)->bit.INT_MASK & (0x1 << chx)) != RESET);
    }
    if(int_source & DMA_INT_SrcTran)
    {
        tempreg = hdma->Instance->MaskSrcTran.reg_u64val;
		return ((((union _DMA_MaskSrcTran *)&tempreg)->bit.INT_MASK & (0x1 << chx)) != RESET);
    }
    if(int_source & DMA_INT_DstTran)
    {
        tempreg = hdma->Instance->MaskDstTran.reg_u64val;
		return ((((union _DMA_MaskDstTran *)&tempreg)->bit.INT_MASK & (0x1 << chx)) != RESET);
    }
    if(int_source & DMA_INT_Err)
    {
        tempreg = hdma->Instance->MaskErr.reg_u64val;
		return ((((union _DMA_MaskErr *)&tempreg)->bit.INT_MASK & (0x1 << chx)) != RESET);
    }
	return false;
}

/**
  * @brief  Enable channel interrupt.
  * @param  hdma DMA handle
  * @param  chx DMA channel
  * @param  int_source specifies the DMA interrupt source to check.
  *         This parameter can be one of the following values: enum DMA_INT_Source
  *            @arg DMA_INT_Tfr:
  *            @arg DMA_INT_Block:
  *            @arg DMA_INT_SrcTran:
  *            @arg DMA_INT_DstTran:
  *            @arg DMA_INT_Err:
  *            @arg DMA_INT_ALL:
  */
void __HAL_DMA_CHX_ENABLE_IT(DMA_HandleTypeDef *hdma, DMA_ChannelType chx, uint32_t int_source)
{
    uint64_t tempreg = 0;
    /* Check the hdma */
    assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
    if(int_source & DMA_INT_Tfr)
    {
        tempreg = 0;
        ((union _DMA_MaskTfr *)&tempreg)->bit.INT_MASK_WE = 0x1 << chx;
        ((union _DMA_MaskTfr *)&tempreg)->bit.INT_MASK = 0x1 << chx;
        hdma->Instance->MaskTfr.reg_u64val = tempreg;
    }
    if(int_source & DMA_INT_Block)
    {
        tempreg = 0;
        ((union _DMA_MaskBlock *)&tempreg)->bit.INT_MASK_WE = 0x1 << chx;
        ((union _DMA_MaskBlock *)&tempreg)->bit.INT_MASK = 0x1 << chx;
        hdma->Instance->MaskBlock.reg_u64val = tempreg;
    }
    if(int_source & DMA_INT_SrcTran)
    {
        tempreg = 0;
        ((union _DMA_MaskSrcTran *)&tempreg)->bit.INT_MASK_WE = 0x1 << chx;
        ((union _DMA_MaskSrcTran *)&tempreg)->bit.INT_MASK = 0x1 << chx;
        hdma->Instance->MaskSrcTran.reg_u64val = tempreg;
    }
    if(int_source & DMA_INT_DstTran)
    {
        tempreg = 0;
        ((union _DMA_MaskDstTran *)&tempreg)->bit.INT_MASK_WE = 0x1 << chx;
        ((union _DMA_MaskDstTran *)&tempreg)->bit.INT_MASK = 0x1 << chx;
        hdma->Instance->MaskDstTran.reg_u64val = tempreg;
    }
    if(int_source & DMA_INT_Err)
    {
        tempreg = 0;
        ((union _DMA_MaskErr *)&tempreg)->bit.INT_MASK_WE = 0x1 << chx;
        ((union _DMA_MaskErr *)&tempreg)->bit.INT_MASK = 0x1 << chx;
        hdma->Instance->MaskErr.reg_u64val = tempreg;
    }
}

/**
  * @brief  Disable channel interrupt.
  * @param  hdma DMA handle
  * @param  chx DMA channel
  * @param  int_source specifies the DMA interrupt source to check.
  *         This parameter can be one of the following values: enum DMA_INT_Source
  *            @arg DMA_INT_Tfr:
  *            @arg DMA_INT_Block:
  *            @arg DMA_INT_SrcTran:
  *            @arg DMA_INT_DstTran:
  *            @arg DMA_INT_Err:
  *            @arg DMA_INT_ALL:
  */
void __HAL_DMA_CHX_DISABLE_IT(DMA_HandleTypeDef *hdma, DMA_ChannelType chx, uint32_t int_source)
{
    uint64_t tempreg = 0;
    /* Check the hdma */
    assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
    if(int_source & DMA_INT_Tfr)
    {
        tempreg = 0;
        ((union _DMA_MaskTfr *)&tempreg)->bit.INT_MASK_WE = 0x1 << chx;
        hdma->Instance->MaskTfr.reg_u64val = tempreg;
    }
    if(int_source & DMA_INT_Block)
    {
        tempreg = 0;
        ((union _DMA_MaskBlock *)&tempreg)->bit.INT_MASK_WE = 0x1 << chx;
        hdma->Instance->MaskBlock.reg_u64val = tempreg;
    }
    if(int_source & DMA_INT_SrcTran)
    {
        tempreg = 0;
        ((union _DMA_MaskSrcTran *)&tempreg)->bit.INT_MASK_WE = 0x1 << chx;
        hdma->Instance->MaskSrcTran.reg_u64val = tempreg;
    }
    if(int_source & DMA_INT_DstTran)
    {
        tempreg = 0;
        ((union _DMA_MaskDstTran *)&tempreg)->bit.INT_MASK_WE = 0x1 << chx;
        hdma->Instance->MaskDstTran.reg_u64val = tempreg;
    }
    if(int_source & DMA_INT_Err)
    {
        tempreg = 0;
        ((union _DMA_MaskErr *)&tempreg)->bit.INT_MASK_WE = 0x1 << chx;
        hdma->Instance->MaskErr.reg_u64val = tempreg;
    }
}

/**
  * @brief  Get channel interrupt.
  * @param  hdma DMA handle
  * @param  chx DMA channel
  * @param  int_source specifies the DMA interrupt source to check.
  *         This parameter can be one of the following values: enum DMA_INT_Source
  *            @arg DMA_INT_Tfr:
  *            @arg DMA_INT_Block:
  *            @arg DMA_INT_SrcTran:
  *            @arg DMA_INT_DstTran:
  *            @arg DMA_INT_Err:
  *            @arg DMA_INT_ALL:
  */
bool __HAL_DMA_CHX_GET_IT_STATUS(DMA_HandleTypeDef *hdma, DMA_ChannelType chx, uint32_t int_source)
{
    /* Check the hdma */
    assert_param(IS_DMA_ALL_INSTANCE(hdma->Instance));
    if(int_source & DMA_INT_Tfr)
    {
        return ((hdma->Instance->StatusTfr & (0x1 << chx)) != RESET);
    }
    if(int_source & DMA_INT_Block)
    {
        return ((hdma->Instance->StatusBlock & (0x1 << chx)) != RESET);
    }
    if(int_source & DMA_INT_SrcTran)
    {
        return ((hdma->Instance->StatusSrcTran & (0x1 << chx)) != RESET);
    }
    if(int_source & DMA_INT_DstTran)
    {
        return ((hdma->Instance->StatusDstTran & (0x1 << chx)) != RESET);
    }
    if(int_source & DMA_INT_Err)
    {
        return ((hdma->Instance->StatusErr & (0x1 << chx)) != RESET);
    }
    return false;
}



#endif /* HAL_DMA_MODULE_ENABLED */

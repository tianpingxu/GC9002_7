/**
  ******************************************************************************
  * @file    gc9002_hal_dma.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2023-1-6
  * @brief   This file contains all the functions prototypes for the DMA
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC9002_HAL_DMA_H__
#define __GC9002_HAL_DMA_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal_def.h"

#ifdef HAL_DMA_MODULE_ENABLED

#define DMA_NUM_CHANNELS                            (4UL) //Channel count
#define DMA_MAX_CHANNEL_PRIORITY                    (DMA_NUM_CHANNELS - 1)
#define DMA_MIN_CHANNEL_PRIORITY                    (0UL)
#define DMA_OUTSTANDING_REQUEST_LIMIT_MAX           (15)
#define DMA_NUMBER_OF_HARDWARE_HS_INTERFACE         (58UL)
#define DMAX_NUM_HS_IF                              DMA_NUMBER_OF_HARDWARE_HS_INTERFACE
#define DMA_MAX_VALUE_OF_AHB_BURST_LEN              (16)
#define DMA_MAX_VALUE_OF_BURST_TRANSACTION_SIZE     (0x03)
#define DMA_MAX_VALUE_OF_BLOCK_SIZE                 (1024UL)

/* DMA_CHANNEL_X_BLOCK_TS_MAX
 */
#define DMA_CHANNEL_1_BLOCK_TS_MAX                  0x7FU
#define DMA_CHANNEL_2_BLOCK_TS_MAX                  0x7FU
#define DMA_CHANNEL_3_BLOCK_TS_MAX                  0x1FU
#define DMA_CHANNEL_4_BLOCK_TS_MAX                  0x1FU
#define DMA_CHANNEL_5_BLOCK_TS_MAX                  0x00U
#define DMA_CHANNEL_6_BLOCK_TS_MAX                  0x00U
#define DMA_CHANNEL_7_BLOCK_TS_MAX                  0x00U
#define DMA_CHANNEL_8_BLOCK_TS_MAX                  0x00U


//Transfer Type and Flow Control select
enum DMA_TT_FC_Enum
{
    DMA_TT_MEM_TO_MEM_DMAC = 0x0UL,
    DMA_TT_MEM_TO_PER_DMAC,
    DMA_TT_PER_TO_MEM_DMAC,
    DMA_TT_PER_TO_PER_DMAC,
    DMA_TT_PER_TO_MEM_PER,
    DMA_TT_PER_TO_PER_SRC_PER,
    DMA_TT_MEM_TO_PER_PER,
    DMA_TT_PER_TO_PER_DST_PER,
};

/** @defgroup DMA_HS_selection
  * @brief    DMA per selection
  */
#define DMA_HS_UART0_TX             (0UL)
#define DMA_HS_UART0_RX             (1UL)
#define DMA_HS_UART1_TX             (2UL)
#define DMA_HS_UART1_RX             (3UL)
#define DMA_HS_UART2_TX             (4UL)
#define DMA_HS_UART2_RX             (5UL)
#define DMA_HS_ADC                  (7UL)
#define DMA_HS_OTG_TX0              (10UL)
#define DMA_HS_OTG_TX1              (11UL)
#define DMA_HS_OTG_TX2              (12UL)
#define DMA_HS_OTG_RX0              (13UL)
#define DMA_HS_OTG_RX1              (14UL)
#define DMA_HS_OTG_RX2              (15UL)
#define DMA_HS_MEMORY               (0UL)//内存映射

typedef struct
{
    uint32_t mChannel : 4;                      /*!< Specifies the channel used for the specified stream. 
                                                        This parameter can be a value of @ref DMA_ChannelType, 
                                                        You need to pay attention to the number of channels [DMA_NUM_CHANNELS] */
    uint32_t mChannelPriority : 4;              /*!< Specifies Channel Priority. This parameter can be a value of [0 ~ DMA_NUM_CHANNELS-1] */
    uint32_t mS_TransferWidth : 4;              /*!< Specifies the Source data width. This parameter can be a value of @ref enum DMA_TRANSFER_DAT_WIDTH_Enum */
    uint32_t mD_TransferWidth : 4;              /*!< Specifies the Destination data width. This parameter can be a value of @ref enum DMA_TRANSFER_DAT_WIDTH_Enum */
    uint32_t mS_BurstTransactionLen : 4;        /*!< Specifies the Source Burst Transaction Length. This parameter can be a value of @ref enum DMA_BURST_MSIZE_Enum */
    uint32_t mD_BurstTransactionLen : 4;        /*!< Specifies the Destination Burst Transaction Length. This parameter can be a value of @ref enum DMA_BURST_MSIZE_Enum */
    uint32_t mS_Peripheral : 4;                 /*!< Specifies Assigns a hardware handshaking interface (0 - DMAX_NUM_HS_IF-1) to the source of Channelx if the
                                                        CHx_CFG.HS_SEL_SRC field is 0; otherwise, this field is ignored.
                                                        This parameter can be a value of [0 ~ DMAX_NUM_HS_IF]          */
    uint32_t mD_Peripheral : 4;                 /*!< Specifies Assigns a hardware handshaking interface (0 - DMAX_NUM_HS_IF-1) to the destination of Channelx if the
                                                        CHx_CFG.HS_SEL_DST field is 0; otherwise, this field is ignored.
                                                        This parameter can be a value of @ref DMA_HS_selection          */
    uint32_t mS_Address;                        /*!< Specifies Source Address. You must program the SAR address to be aligned to mS_TransferWidth */
    uint32_t mD_Address;                        /*!< Specifies Destination Address. You must program the SAR address to be aligned to mD_TransferWidth */
    uint32_t mBlockSize : 12;                   /*!< Specifies the Block Transfer Size.
                                                        DataLength The number programmed into BLOCK_TS field indicates the total number of data
                                                        of width this.mS_TransferWidth to be transferred in a DMA block transfer
                                                        This parameter can be a value of [1 ~ DMA_CHANNEL_X_BLOCK_TS_MAX], Each channel has a different value range, 
                                                        @ref DMA_CHANNEL_X_BLOCK_TS_MAX (X=1~8)*/
    uint32_t mS_AutoReload_EN : 1;              /*!< Specifies Automatic Source Reload of Source addr.
                                                        This parameter can be a value of @ref enum DMA_En_Enum.
                                                        This parameter does not exist if the configuration parameter DMAH_CHx_MULTI_BLK_EN is not selected; */
    uint32_t mD_AutoReload_EN : 1;              /*!< Specifies Automatic Source Reload of Destination addr.
                                                        This parameter can be a value of @ref enum DMA_En_Enum.
                                                        This parameter does not exist if the configuration parameter DMAH_CHx_MULTI_BLK_EN is not selected; */
    uint32_t mS_AddrInc : 2;                    /*!< Specifies the Source address register should be incremented or not.
                                                        This parameter can be a value of @ref enum DMA_Inc_Mode_Enum          */
    uint32_t mD_AddrInc : 2;                    /*!< Specifies the Destination address register should be incremented or not.
                                                        This parameter can be a value of @ref enum DMA_Inc_Mode_Enum          */
    uint32_t mTransferType_FlowControl : 4;     /*!< Specifies Transfer Type and Flow Control.Flow Control can be assigned to the DW_axi_dmac, the source peripheral, or hte destination peripheral.
                                                        The following transfer types are supported.
                                                        ¡ö Memory to Memory
                                                        ¡ö Memory to Peripheral
                                                        ¡ö Peripheral to Memory
                                                        ¡ö Peripheral to Peripheral
                                                        This parameter can be a value of @ref enum DMA_TT_FC_Enum. */
    uint32_t mEnable : 1;                       /*!< This bit is used to enable the DW_axi_dmac Channel-x.
                                                        This parameter can be a value of @ref enum DMA_En_Enum. */
    uint32_t : 11;
} DMAChannel_InitTypeDef;

/**
  * @brief  HAL DMA State structures definition
  */
typedef enum
{
  HAL_DMA_STATE_RESET             = 0x00U,  /*!< DMA not yet initialized or disabled */
  HAL_DMA_STATE_READY             = 0x01U,  /*!< DMA initialized and ready for use   */
  HAL_DMA_STATE_BUSY              = 0x02U,  /*!< DMA process is ongoing              */
  HAL_DMA_STATE_TIMEOUT           = 0x03U,  /*!< DMA timeout state                   */
  HAL_DMA_STATE_ERROR             = 0x04U,  /*!< DMA error state                     */
  HAL_DMA_STATE_ABORT             = 0x05U,  /*!< DMA Abort state                     */
}HAL_DMA_StateTypeDef;
//Distination Transfer Width select and Source Transfer Width select
//map to AXI bus awsize and AXI bus arsize.
enum DMA_TRANSFER_DAT_WIDTH_Enum
{
    DMA_TR_WIDTH_8bits = 0x0UL,
    DMA_TR_WIDTH_16bits,
    DMA_TR_WIDTH_32bits,
    DMA_TR_WIDTH_64bits,
    DMA_TR_WIDTH_128bits,
    DMA_TR_WIDTH_256bits,
};
//DMS select and SMS select
enum DMA_Master_Select_Enum
{
    DMA_AHB_Master1 = 0x0UL,// Linked Memory Address
    DMA_AHB_Master2 = 0x1UL,// Linked Peripheral Address
    DMA_AHB_Master3 = 0x1UL,// Linked Peripheral Address
    DMA_AHB_Master4 = 0x1UL,// Linked Peripheral Address
};
//DINC select and SINC select
enum DMA_Inc_Mode_Enum
{
    DMA_INCREMENTAL = 0x0UL,
    DMA_DECREMENTS = 0x1UL,
    DMA_FIXED = 0x2UL,
};
//Handshaking type select
enum DMA_Handshaking_Enum
{
     DMA_HardwareHandshaking = 0x0UL,
     DMA_SoftwareHandshaking = 0x1UL,
};

/** @defgroup DMA_Channel_selection DMA Channel selection
  * @brief    DMA channel selection
  */
typedef enum
{
    DMA_CHANNEL1 = 0x00UL,    /*!< DMA Channel 1 */
    DMA_CHANNEL2 = 0x01UL,    /*!< DMA Channel 2 */
    DMA_CHANNEL3 = 0x02UL,    /*!< DMA Channel 3 */
    DMA_CHANNEL4 = 0x03UL,    /*!< DMA Channel 4 */
    DMA_CHANNEL5 = 0x04UL,    /*!< DMA Channel 5 */
    DMA_CHANNEL6 = 0x05UL,    /*!< DMA Channel 6 */
    DMA_CHANNEL7 = 0x06UL,    /*!< DMA Channel 7 */
    DMA_CHANNEL8 = 0x07UL,    /*!< DMA Channel 8 */
}DMA_ChannelType;
#define IS_DMA_CHANNEL(CHX)   ((CHX) <= DMA_CHANNEL8)

enum DMA_BURST_MSIZE_Enum
{
    DMA_DATA_ITEM_1 = 0x0UL,
    DMA_DATA_ITEMS_4,
    DMA_DATA_ITEMS_8,
    DMA_DATA_ITEMS_16,
    DMA_DATA_ITEMS_32,
    DMA_DATA_ITEMS_64,
    DMA_DATA_ITEMS_128,
    DMA_DATA_ITEMS_256,
    DMA_DATA_ITEMS_512,
    DMA_DATA_ITEMS_1024,
};
enum DMA_En_Enum
{
    DMA_DISABLE = 0x0UL,
    DMA_ENABLE = 0x1UL,
};

/**
  * @brief  DMA handle Structure definition
  */
typedef struct __DMA_HandleTypeDef
{
    DMA_TypeDef                *Instance;                                                        /*!< Register base address                       */
    DMAChannel_InitTypeDef     Init;                                                             /*!< DMA communication parameters                */
    HAL_LockTypeDef            Lock;                                                             /*!< DMA locking object                          */
    __IO HAL_DMA_StateTypeDef  State;                                                            /*!< DMA transfer state                          */
    void                       *Parent;                                                          /*!< Parent object state                         */
    void                       *ChObj;                                                           /*!< Channel object state                        */
#if(defined USE_HAL_DMA_REGISTER_CALLBACKS && USE_HAL_DMA_REGISTER_CALLBACKS == 1)
    void                       (* XferCpltCallback)(struct __DMA_HandleTypeDef * hdma);          /*!< DMA transfer complete callback              */
    void                       (* BlockCpltCallback)(struct __DMA_HandleTypeDef * hdma);         /*!< DMA block complete callback                 */
    void                       (* SourceCpltCallback)(struct __DMA_HandleTypeDef * hdma);        /*!< DMA Source complete callback                */
    void                       (* DestinationCpltCallback)(struct __DMA_HandleTypeDef * hdma);   /*!< DMA destination complete callback           */
    void                       (* XferErrorCallback)(struct __DMA_HandleTypeDef * hdma);         /*!< DMA transfer error callback                 */
    void                       (* XferAbortCallback)(struct __DMA_HandleTypeDef * hdma);         /*!< DMA transfer Abort callback                 */
    void                       (* XferHalfCpltCallback)(struct __DMA_HandleTypeDef * hdma);      /*!< Not used, for compatibility only            */
#endif
    __IO uint32_t              ErrorCode;                                                        /*!< DMA Error code                              */
}DMA_HandleTypeDef;

/**
  * @brief  Enable the specified DMA Stream.
  * @param  hdma DMA handle
  * @param  chx DMA channel
  * @retval None
  */
__attribute__((always_inline)) static inline void __HAL_DMA_CHX_ENABLE(DMA_HandleTypeDef *hdma, DMA_ChannelType chx)
{
    uint64_t tempreg = 0;
    tempreg = hdma->Instance->CHx[chx].CFG.reg_u64val;
    ((union _DMA_CFG *)&tempreg)->bit.CH_SUSP = 0;
    hdma->Instance->CHx[chx].CFG.reg_u64val = tempreg;
    
    tempreg = 0;
    ((union _DMA_ChEnReg *)&tempreg)->bit.CH_EN_WE = (0x1UL << chx);
    ((union _DMA_ChEnReg *)&tempreg)->bit.CH_EN = (0x1UL << chx);
    hdma->Instance->ChEnReg.reg_u64val = tempreg;
    while((hdma->Instance->ChEnReg.bit.CH_EN & (0x1UL << chx)) == 0);
}
/**
  * @brief  Disable the specified DMA Stream.
  * @param  hdma DMA handle
  * @param  chx DMA channel
  * @retval None
  */
__attribute__((always_inline)) static inline void __HAL_DMA_CHX_DISABLE(DMA_HandleTypeDef *hdma, DMA_ChannelType chx)
{
    uint64_t tempreg = 0;
    tempreg = hdma->Instance->CHx[chx].CFG.reg_u64val;
    ((union _DMA_CFG *)&tempreg)->bit.CH_SUSP = 1;
    hdma->Instance->CHx[chx].CFG.reg_u64val = tempreg;
    
    while(hdma->Instance->CHx[chx].CFG.bit.FIFO_EMPTY == 0);
    
    tempreg = 0;
    ((union _DMA_ChEnReg *)&tempreg)->bit.CH_EN_WE = (0x1UL << chx);
    do
    {
        hdma->Instance->ChEnReg.reg_u64val = tempreg;
    }while((hdma->Instance->ChEnReg.bit.CH_EN & (0x1UL << chx)) != 0);
    
    tempreg = hdma->Instance->CHx[chx].CFG.reg_u64val;
    ((union _DMA_CFG *)&tempreg)->bit.CH_SUSP = 0;
    hdma->Instance->CHx[chx].CFG.reg_u64val = tempreg;
}

__attribute__((always_inline)) static inline bool __HAL_DMA_CHANNEL_IS_ENABLE(DMA_HandleTypeDef *hdma)
{
    union _DMA_ChEnReg tempreg;
    tempreg.reg_u64val = hdma->Instance->ChEnReg.reg_u64val;
    return ((tempreg.bit.CH_EN & (0x1UL << hdma->Init.mChannel)) != RESET);
}
enum DMA_INT_Source
{
    DMA_INT_Tfr     = 0x0001U,
    DMA_INT_Block   = 0x0002U,
    DMA_INT_SrcTran = 0x0004U,
    DMA_INT_DstTran = 0x0008U,
    DMA_INT_Err     = 0x0010U,
    DMA_INT_ALL     = 0x001FUL,
};
/**
  * @brief  Get channel interrupt source.
  * @param  hdma DMA handle
  * @param  int_source specifies the DMA interrupt source to check.
  *         This parameter can be one of the following values: enum DMA_INT_Source
  *            @arg DMA_INT_Tfr:
  *            @arg DMA_INT_Block:
  *            @arg DMA_INT_SrcTran:
  *            @arg DMA_INT_DstTran:
  *            @arg DMA_INT_Err:
  *            @arg DMA_INT_ALL:
  */
bool __HAL_DMA_CHX_GET_IT_SOURCE(DMA_HandleTypeDef *hdma, DMA_ChannelType chx, uint32_t int_source);
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
void __HAL_DMA_CHX_ENABLE_IT(DMA_HandleTypeDef *hdma, DMA_ChannelType chx, uint32_t int_source);
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
void __HAL_DMA_CHX_DISABLE_IT(DMA_HandleTypeDef *hdma, DMA_ChannelType chx, uint32_t int_source);
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
bool __HAL_DMA_CHX_GET_IT_STATUS(DMA_HandleTypeDef *hdma, DMA_ChannelType chx, uint32_t int_source);
/**
  * @brief  Clear channel interrupt.
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
__attribute__((always_inline)) static inline void __HAL_DMA_CHX_CLEAR_IT(DMA_HandleTypeDef *hdma, DMA_ChannelType chx, uint32_t int_source)
{
    if(int_source & DMA_INT_Tfr)
    {
        hdma->Instance->ClearTfr = (0x1 << chx);
    }
    if(int_source & DMA_INT_Block)
    {
        hdma->Instance->ClearBlock = (0x1 << chx);
    }
    if(int_source & DMA_INT_SrcTran)
    {
        hdma->Instance->ClearSrcTran = (0x1 << chx);
    }
    if(int_source & DMA_INT_DstTran)
    {
        hdma->Instance->ClearDstTran = (0x1 << chx);
    }
    if(int_source & DMA_INT_Err)
    {
        hdma->Instance->ClearErr = (0x1 << chx);
    }
}

/** @defgroup DMA_Error_Code DMA Error Code
  * @brief    DMA Error Code
  */
#define HAL_DMA_ERROR_NONE            0x00000000U    /*!< No error                               */
#define HAL_DMA_ERROR_TE              0x00000001U    /*!< Transfer error                         */
#define HAL_DMA_ERROR_FE              0x00000002U    /*!< FIFO error                             */
#define HAL_DMA_ERROR_DME             0x00000004U    /*!< Direct Mode error                      */
#define HAL_DMA_ERROR_TIMEOUT         0x00000020U    /*!< Timeout error                          */
#define HAL_DMA_ERROR_PARAM           0x00000040U    /*!< Parameter error                        */
#define HAL_DMA_ERROR_NO_XFER         0x00000080U    /*!< Abort requested with no Xfer ongoing   */
#define HAL_DMA_ERROR_NOT_SUPPORTED   0x00000100U    /*!< Not supported mode                     */

#if(defined USE_HAL_DMA_REGISTER_CALLBACKS && USE_HAL_DMA_REGISTER_CALLBACKS == 1)
/**
  * @brief  HAL DMA Error Code structure definition
  */
typedef enum
{
  HAL_DMA_XFER_CPLT_CB_ID         = 0x00U,  /*!< Full transfer          */
  HAL_DMA_XFER_BLOCK_CB_ID        = 0x01U,  /*!< Block Transfer         */
  HAL_DMA_XFER_SRC_CB_ID          = 0x02U,  /*!< Source Transfer        */
  HAL_DMA_XFER_DST_CB_ID          = 0x03U,  /*!< Destination Transfer   */
  HAL_DMA_XFER_ERROR_CB_ID        = 0x04U,  /*!< Error                  */
  HAL_DMA_XFER_ABORT_CB_ID        = 0x05U,  /*!< Abort                  */
  HAL_DMA_XFER_ALL_CB_ID          = 0x06U   /*!< All                    */
}HAL_DMA_CallbackIDTypeDef;
#endif /* USE_HAL_DMA_REGISTER_CALLBACKS */

/**
  * @brief  Returns the number of remaining data units in the current DMAy Streamx transfer.
  * @param  __HANDLE__ DMA handle
  * @retval The number of remaining data units in the current DMA Stream transfer.
  */
__attribute__((always_inline)) static inline uint32_t __HAL_DMA_CHX_GET_COUNTER(DMA_HandleTypeDef *hdma)
{
    uint64_t tempreg = hdma->Instance->CHx[hdma->Init.mChannel].CTL.reg_u64val;
    return (uint32_t)(((union _DMA_CTL *)&tempreg)->bit.BLOCK_TS);
}
#define __HAL_DMA_GET_RELOADEN(__HANDLE__)   ((__HANDLE__)->Init.mD_AutoReload_EN == DMA_DISABLE && \
                                             (__HANDLE__)->Init.mS_AutoReload_EN == DMA_DISABLE)

HAL_StatusTypeDef HAL_DMA_StructInit(DMAChannel_InitTypeDef* DMAChannel_InitType);

HAL_StatusTypeDef HAL_DMA_Init(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_DeInit(DMA_HandleTypeDef *hdma);

HAL_StatusTypeDef HAL_DMA_Start(DMA_HandleTypeDef *hdma, uint64_t SrcAddress, uint64_t DstAddress, uint32_t DataLength);
HAL_StatusTypeDef HAL_DMA_Start_IT(DMA_HandleTypeDef *hdma, uint64_t SrcAddress, uint64_t DstAddress, uint32_t DataLength);
HAL_StatusTypeDef HAL_DMA_Abort(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_Abort_IT(DMA_HandleTypeDef *hdma);

#if(defined USE_HAL_DMA_REGISTER_CALLBACKS && USE_HAL_DMA_REGISTER_CALLBACKS == 1)
HAL_StatusTypeDef HAL_DMA_RegisterCallback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID, void (* pCallback)(DMA_HandleTypeDef *_hdma));
HAL_StatusTypeDef HAL_DMA_UnRegisterCallback(DMA_HandleTypeDef *hdma, HAL_DMA_CallbackIDTypeDef CallbackID);
#endif

HAL_DMA_StateTypeDef HAL_DMA_GetState(DMA_HandleTypeDef *hdma);
uint32_t             HAL_DMA_GetError(DMA_HandleTypeDef *hdma);

void HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma);
void HAL_DMA_XferCpltCallback(DMA_HandleTypeDef * hdma);
void HAL_DMA_BlockCpltCallback(DMA_HandleTypeDef * hdma);
void HAL_DMA_SourceCpltCallback(DMA_HandleTypeDef * hdma);
void HAL_DMA_DestinationCpltCallback(DMA_HandleTypeDef * hdma);
void HAL_DMA_XferErrorCallback(DMA_HandleTypeDef * hdma);
void HAL_DMA_XferAbortCallback(DMA_HandleTypeDef * hdma);
#endif/* HAL_DMA_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC9002_HAL_DMA_H__ */

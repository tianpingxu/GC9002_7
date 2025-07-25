/**
  ******************************************************************************
  * @file    gc90xx_hal_i2s.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2022-09-18
  * @brief   This file contains all the functions prototypes for the I2S 
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC90XX_HAL_I2S_H__

#define __GC90XX_HAL_I2S_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <gc90xx_hal_def.h>
#ifdef HAL_I2S_MODULE_ENABLED

/** @addtogroup GC90xx_HAL_Driver
  * @{
  */

/** @addtogroup I2S
  * @{
  */
#define I2S_CHANNELS_MAX                        (0x4U)//
#ifndef BSP_I2S_RX_CHANNELS
#error need define BSP_I2S_RX_CHANNELS
#endif
#ifndef BSP_I2S_TX_CHANNELS
#error need define BSP_I2S_TX_CHANNELS
#endif

#ifndef BSP_I2S_TXRX_FIFO
#error need define BSP_I2S_TXRX_FIFO
#endif

#if(BSP_I2S_RX_CHANNELS > I2S_CHANNELS_MAX || BSP_I2S_TX_CHANNELS > I2S_CHANNELS_MAX)
#error RX/TX Channels Greater than I2S_CHANNELS_MAX
#endif
#if(BSP_I2S_RX_CHANNELS == 0 && BSP_I2S_TX_CHANNELS == 0)
#error BSP_I2S_RX_CHANNELS or BSP_I2S_TX_CHANNELS can not all be 0
#endif

#define I2S_RX_CHANNELS     BSP_I2S_RX_CHANNELS
#define I2S_TX_CHANNELS     BSP_I2S_TX_CHANNELS

#if(BSP_I2S_RX_CHANNELS > BSP_I2S_TX_CHANNELS)
#define I2S_CHANNELS    BSP_I2S_RX_CHANNELS
#else
#define I2S_CHANNELS    BSP_I2S_TX_CHANNELS
#endif
/* Exported types ------------------------------------------------------------*/
/** @defgroup I2S_Exported_Types I2S Exported Types
  * @{
  */

typedef enum I2S_CHANNEL
{
    I2S_CHANNEL_0 = 0x0UL,
    I2S_CHANNEL_1,
    I2S_CHANNEL_2,
    I2S_CHANNEL_3,
}I2S_ChannelType;
#define IS_I2S_CHANNEL(CH)    ((CH) < I2S_CHANNELS)
/**
  * @brief I2S Init structure definition
  */
typedef struct
{
    uint32_t Channel : 8;     /*!< Specifies the I2S channel.
                                  This parameter can be a value of @ref I2S_ChannelType */
    uint32_t Mode : 8;        /*!< Specifies the I2S operating mode.
                                  This parameter can be a value of @ref enum I2S_Mode */
    uint32_t Standard : 8;    /*!< Specifies the standard used for the I2S communication.
                                  This parameter can be a value of @ref enum I2S_Standard */
    uint32_t DataFormat : 8;  /*!< Specifies the data format for the I2S communication.
                                  This parameter can be a value of @ref enum I2S_Data_Format */
    uint32_t AudioFreq;       /*!< Specifies the frequency selected for the I2S communication.
                                  This parameter can be a value of @ref enum I2S_Audio_Frequency */
    union {
        uint32_t Resolution;
        struct {
            uint32_t RxResolution : 8;    /*!< Specifies the I2S RX Resolution.
                                            This parameter can be a value of @ref I2S_Resolution */
            uint32_t TxResolution : 8;    /*!< Specifies the I2S TX Resolution.
                                            This parameter can be a value of @ref I2S_Resolution */
            uint32_t RxTriggerLevel : 8;  /*!< Specifies the I2S program the Rx FIFO almost full interrupt/DMA Threshold Trigger Level parameter.
                                            This parameter can be a value of @ref I2S_TriggerLevel */
            uint32_t TxTriggerLevel : 8;  /*!< Specifies the I2S program the Tx FIFO almost Empty interrupt/DMA Threshold Trigger Level parameter.
                                            This parameter can be a value of @ref I2S_TriggerLevel */
        };
    };
} I2S_InitTypeDef;

/**
  * @brief  HAL State structures definition
  */
typedef enum
{
    HAL_I2S_STATE_RESET         = 0x00U,  /*!< I2S not yet initialized or disabled          */
    HAL_I2S_STATE_READY         = 0x01U,  /*!< I2S initialized and ready for use            */
    HAL_I2S_STATE_BUSY_TX       = 0x02U,  /*!< Data Transmission process is ongoing         */
    HAL_I2S_STATE_BUSY_RX       = 0x04U,  /*!< Data Reception process is ongoing            */
    HAL_I2S_STATE_BUSY_DMA_TX   = 0x20U,  /*!< Data DMA Transmission process is ongoing     */
    HAL_I2S_STATE_BUSY_DMA_RX   = 0x40U,  /*!< Data DMA Reception process is ongoing        */
    HAL_I2S_STATE_TIMEOUT       = 0x08U,  /*!< I2S timeout state                            */
    HAL_I2S_STATE_ERROR         = 0x80U   /*!< I2S error state                              */
} HAL_I2S_StateTypeDef;

/**
  * @brief I2S handle Structure definition
  */
typedef struct __I2S_HandleTypeDef
{
    I2S_TypeDef                *Instance;    /*!< I2S registers base address */
    I2S_InitTypeDef            Init;         /*!< I2S communication parameters */
    void                       *priv_obj;
    uint32_t                   TX_Word_size; /*!< the size of word(bits) */
    uint32_t                   RX_Word_size; /*!< the size of word(bits) */
    uint8_t                    *pTxBuffPtr;  /*!< Pointer to I2S Tx transfer buffer */
    __IO uint16_t              TxXferSize;   /*!< I2S Tx transfer size */
    __IO uint16_t              TxXferCount;  /*!< I2S Tx transfer Counter */
    uint8_t                    *pRxBuffPtr;  /*!< Pointer to I2S Rx transfer buffer */
    __IO uint16_t              RxXferSize;   /*!< I2S Rx transfer size */
    __IO uint16_t              RxXferCount;  /*!< I2S Rx transfer counter
                                                                    (This field is initialized at the same value as transfer size 
                                                                    at the beginning of the transfer and decremented when a sample 
                                                                    is received NbSamplesReceived = RxBufferSize-RxBufferCount) */
#if(defined BSP_I2S_TXRX_DMA_ENABLED && BSP_I2S_TXRX_DMA_ENABLED == 1U)
    DMA_HandleTypeDef          *hdmatx;      /*!< I2S Tx DMA handle parameters */
    DMA_HandleTypeDef          *hdmarx;      /*!< I2S Rx DMA handle parameters */
#endif
    __IO HAL_LockTypeDef       Lock;         /*!< I2S locking object */
    __IO HAL_I2S_StateTypeDef  State;        /*!< I2S communication state */
    __IO uint32_t              ErrorCode;    /*!< I2S Error code. This parameter can be a value of @ref I2S_Error */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
    void (* TxCpltCallback)(struct __I2S_HandleTypeDef *hi2s);             /*!< I2S Tx Completed callback          */
    void (* RxCpltCallback)(struct __I2S_HandleTypeDef *hi2s);             /*!< I2S Rx Completed callback          */
    void (* TxRxCpltCallback)(struct __I2S_HandleTypeDef *hi2s);           /*!< I2S TxRx Completed callback        */
    void (* TxHalfCpltCallback)(struct __I2S_HandleTypeDef *hi2s);         /*!< I2S Tx Half Completed callback     */
    void (* RxHalfCpltCallback)(struct __I2S_HandleTypeDef *hi2s);         /*!< I2S Rx Half Completed callback     */
    void (* TxRxHalfCpltCallback)(struct __I2S_HandleTypeDef *hi2s);       /*!< I2S TxRx Half Completed callback   */
    void (* ErrorCallback)(struct __I2S_HandleTypeDef *hi2s);              /*!< I2S Error callback                 */
    void (* MspInitCallback)(struct __I2S_HandleTypeDef *hi2s);            /*!< I2S Msp Init callback              */
    void (* MspDeInitCallback)(struct __I2S_HandleTypeDef *hi2s);          /*!< I2S Msp DeInit callback            */
#endif  /* USE_HAL_I2S_REGISTER_CALLBACKS */
} I2S_HandleTypeDef;

#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
/**
  * @brief  HAL I2S Callback ID enumeration definition
  */
typedef enum
{
    HAL_I2S_TX_COMPLETE_CB_ID             = 0x00U,    /*!< I2S Tx Completed callback ID         */
    HAL_I2S_RX_COMPLETE_CB_ID             = 0x01U,    /*!< I2S Rx Completed callback ID         */
    HAL_I2S_TX_RX_COMPLETE_CB_ID          = 0x02U,    /*!< I2S TxRx Completed callback ID       */
    HAL_I2S_TX_HALF_COMPLETE_CB_ID        = 0x03U,    /*!< I2S Tx Half Completed callback ID    */
    HAL_I2S_RX_HALF_COMPLETE_CB_ID        = 0x04U,    /*!< I2S Rx Half Completed callback ID    */
    HAL_I2S_TX_RX_HALF_COMPLETE_CB_ID     = 0x05U,    /*!< I2S TxRx Half Completed callback ID  */
    HAL_I2S_ERROR_CB_ID                   = 0x06U,    /*!< I2S Error callback ID                */
    HAL_I2S_MSPINIT_CB_ID                 = 0x07U,    /*!< I2S Msp Init callback ID             */
    HAL_I2S_MSPDEINIT_CB_ID               = 0x08U     /*!< I2S Msp DeInit callback ID           */
} HAL_I2S_CallbackIDTypeDef;

/**
  * @brief  HAL I2S Callback pointer definition
  */
typedef  void (*pI2S_CallbackTypeDef)(I2S_HandleTypeDef *hi2s); /*!< pointer to an I2S callback function */

#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */

enum
{
    I2S_DISABLE = 0x0UL,
    I2S_ENABLE,
};
/**
  * @brief WSS
  */
enum I2S_Data_Format
{
    I2S_WSS_CLOCK_CYCLES_16 = 0x0UL,
    I2S_WSS_CLOCK_CYCLES_24,
    I2S_WSS_CLOCK_CYCLES_32,
};
#define IS_I2S_DATA_FORMAT(__FORMAT__)     (((__FORMAT__) == I2S_WSS_CLOCK_CYCLES_16) || \
                                            ((__FORMAT__) == I2S_WSS_CLOCK_CYCLES_24) || \
                                            ((__FORMAT__) == I2S_WSS_CLOCK_CYCLES_32))
/**
  * @brief SCLKG Clk
  */
enum I2S_Gating_CLK
{
    NO_CLOCK_GATING = 0x0UL,
    I2S_SCLKG_CLOCK_CYCLES_12,
    I2S_SCLKG_CLOCK_CYCLES_16,
    I2S_SCLKG_CLOCK_CYCLES_20,
    I2S_SCLKG_CLOCK_CYCLES_24,
};
/**
  * @brief WLEN
  */
enum I2S_Resolution
{
    I2S_WLEN_IGNORE_WORD_LENGTH = 0x0UL,
    I2S_WLEN_RESOLUTION_12_BIT,
    I2S_WLEN_RESOLUTION_16_BIT,
    I2S_WLEN_RESOLUTION_20_BIT,
    I2S_WLEN_RESOLUTION_24_BIT,
    I2S_WLEN_RESOLUTION_32_BIT,
};
#define IS_I2S_RESOLUTION(RESOLUTION)   ((RESOLUTION) <= I2S_WLEN_RESOLUTION_32_BIT)
/**
  * @brief WORDSIZE
  */
enum
{
    I2S_WORDSIZE_RESOLUTION_12_BIT = 0x0UL,
    I2S_WORDSIZE_RESOLUTION_16_BIT,
    I2S_WORDSIZE_RESOLUTION_20_BIT,
    I2S_WORDSIZE_RESOLUTION_24_BIT,
    I2S_WORDSIZE_RESOLUTION_32_BIT,
};
/**
  * @brief RXCHDT / TXCHDT
  */
typedef enum
{
#if(BSP_I2S_TXRX_FIFO >= 1)
    I2S_TRIGGER_LEVEL_1 = 0x0UL,
#endif
#if(BSP_I2S_TXRX_FIFO >= 2)
    I2S_TRIGGER_LEVEL_2,
#endif
#if(BSP_I2S_TXRX_FIFO >= 3)
    I2S_TRIGGER_LEVEL_3,
#endif
#if(BSP_I2S_TXRX_FIFO >= 4)
    I2S_TRIGGER_LEVEL_4,
#endif
#if(BSP_I2S_TXRX_FIFO >= 5)
    I2S_TRIGGER_LEVEL_5,
#endif
#if(BSP_I2S_TXRX_FIFO >= 6)
    I2S_TRIGGER_LEVEL_6,
#endif
#if(BSP_I2S_TXRX_FIFO >= 7)
    I2S_TRIGGER_LEVEL_7,
#endif
#if(BSP_I2S_TXRX_FIFO >= 8)
    I2S_TRIGGER_LEVEL_8,
#endif
#if(BSP_I2S_TXRX_FIFO >= 9)
    I2S_TRIGGER_LEVEL_9,
#endif
#if(BSP_I2S_TXRX_FIFO >= 10)
    I2S_TRIGGER_LEVEL_10,
#endif
#if(BSP_I2S_TXRX_FIFO >= 11)
    I2S_TRIGGER_LEVEL_11,
#endif
#if(BSP_I2S_TXRX_FIFO >= 12)
    I2S_TRIGGER_LEVEL_12,
#endif
#if(BSP_I2S_TXRX_FIFO >= 13)
    I2S_TRIGGER_LEVEL_13,
#endif
#if(BSP_I2S_TXRX_FIFO >= 14)
    I2S_TRIGGER_LEVEL_14,
#endif
#if(BSP_I2S_TXRX_FIFO >= 15)
    I2S_TRIGGER_LEVEL_15,
#endif
#if(BSP_I2S_TXRX_FIFO >= 16)
    I2S_TRIGGER_LEVEL_16,
#endif
    I2S_TRIGGER_LEVEL_MAX,
}I2S_TriggerLevel;
#define IS_I2S_TRIGGER_LEVEL(LEVEL)     ((LEVEL) < I2S_TRIGGER_LEVEL_MAX)
typedef enum
{
    I2S_Soundtrack_Left = 0x01U,
    I2S_Soundtrack_Right = 0x10,
    I2S_Soundtrack_Stereo = 0x11,
}I2S_Soundtrack;
enum
{
    I2S_FIFO_DEPTH_2 = 0x0UL,
    I2S_FIFO_DEPTH_4,
    I2S_FIFO_DEPTH_8,
    I2S_FIFO_DEPTH_16,
};
enum
{
    I2S_APB_Data_Width_Bits_8 = 0x0UL,
    I2S_APB_Data_Width_Bits_16,
    I2S_APB_Data_Width_Bits_32,
};
/** @defgroup I2S_Mode I2S Mode
  * @{
  */
enum I2S_Mode
{
    I2S_MODE_SLAVE = 0x0U,
    I2S_MODE_MASTER,
};
/** @brief  Checks if I2S Mode parameter is in allowed range.
  * @param  __MODE__ specifies the I2S Mode.
  *         This parameter can be a value of @ref I2S_Mode
  * @retval None
  */
#define IS_I2S_MODE(__MODE__) (((__MODE__) == I2S_MODE_SLAVE)  || \
                               ((__MODE__) == I2S_MODE_MASTER))
/** @defgroup I2S_Standard I2S Standard
  * @{
  */
enum I2S_Standard
{
    I2S_STANDARD_PHILIPS = 0x0U,
    I2S_STANDARD_MSB,       //not support
    I2S_STANDARD_LSB,       //not support
    I2S_STANDARD_PCM_SHORT, //not support
    I2S_STANDARD_PCM_LONG,  //not support
};
#define IS_I2S_STANDARD(__STANDARD__)     (((__STANDARD__) == I2S_STANDARD_PHILIPS)   || \
                                           ((__STANDARD__) == I2S_STANDARD_MSB)       || \
                                           ((__STANDARD__) == I2S_STANDARD_LSB)       || \
                                           ((__STANDARD__) == I2S_STANDARD_PCM_SHORT) || \
                                           ((__STANDARD__) == I2S_STANDARD_PCM_LONG))
/** @defgroup I2S_Audio_Frequency I2S Audio Frequency
  * @{
  */
enum I2S_Audio_Frequency
{
    I2S_AUDIOFREQ_8K = 8000U,
    I2S_AUDIOFREQ_11K = 11025U,
    I2S_AUDIOFREQ_12K = 12000U,
    I2S_AUDIOFREQ_16K = 16000U,
    I2S_AUDIOFREQ_22K = 22050U,
    I2S_AUDIOFREQ_24K = 24000U,
    I2S_AUDIOFREQ_32K = 32000U,
    I2S_AUDIOFREQ_44K = 44100U,
    I2S_AUDIOFREQ_48K = 48000U,
    I2S_AUDIOFREQ_96K = 96000U,
    I2S_AUDIOFREQ_192K = 192000U,
    I2S_AUDIOFREQ_256K = 256000U,
};
#define IS_I2S_AUDIO_FREQ(__FREQ__)         (((__FREQ__) == I2S_AUDIOFREQ_8K) || \
                                             ((__FREQ__) == I2S_AUDIOFREQ_11K) || \
                                             ((__FREQ__) == I2S_AUDIOFREQ_12K) || \
                                             ((__FREQ__) == I2S_AUDIOFREQ_16K) || \
                                             ((__FREQ__) == I2S_AUDIOFREQ_22K) || \
                                             ((__FREQ__) == I2S_AUDIOFREQ_24K) || \
                                             ((__FREQ__) == I2S_AUDIOFREQ_32K) || \
                                             ((__FREQ__) == I2S_AUDIOFREQ_44K) || \
                                             ((__FREQ__) == I2S_AUDIOFREQ_48K) || \
                                             ((__FREQ__) == I2S_AUDIOFREQ_96K) || \
                                             ((__FREQ__) == I2S_AUDIOFREQ_192K) || \
                                             ((__FREQ__) == I2S_AUDIOFREQ_256K))

/* Exported constants --------------------------------------------------------*/
/** @defgroup I2S_Exported_Constants I2S Exported Constants
  * @{
  */
/** @defgroup I2S_Error I2S Error
  * @{
  */
#define HAL_I2S_ERROR_NONE               (0x00000000U)  /*!< No error                    */
#define HAL_I2S_ERROR_TIMEOUT            (0x00000001U)  /*!< Timeout error               */
#define HAL_I2S_ERROR_OVR                (0x00000002U)  /*!< OVR error                   */
#define HAL_I2S_ERROR_UDR                (0x00000004U)  /*!< UDR error                   */
#define HAL_I2S_ERROR_DMA                (0x00000008U)  /*!< DMA transfer error          */
#define HAL_I2S_ERROR_PRESCALER          (0x00000010U)  /*!< Prescaler Calculation error */
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
#define HAL_I2S_ERROR_INVALID_CALLBACK   (0x00000020U)  /*!< Invalid Callback error      */
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */
#define HAL_I2S_ERROR_INVALID_PARAM      (0x00000040U)  /*!< Invalid Param               */

/** @defgroup I2S_MCLK_Output I2S MCLK Output
  * @{
  */
#define I2S_MCLKOUTPUT_ENABLE            (SPI_I2SPR_MCKOE)
#define I2S_MCLKOUTPUT_DISABLE           (0x00000000U)

/** @defgroup I2S_FullDuplex_Mode I2S FullDuplex Mode
  * @{
  */
#define I2S_FULLDUPLEXMODE_DISABLE       (0x00000000U)
#define I2S_FULLDUPLEXMODE_ENABLE        (0x00000001U)

/** @defgroup I2S_Clock_Polarity I2S Clock Polarity
  * @{
  */
#define I2S_CPOL_LOW                     (0x00000000U)
#define I2S_CPOL_HIGH                    (SPI_I2SCFGR_CKPOL)

/** @defgroup I2S_Interrupts_Definition I2S Interrupts Definition
  * @{
  */
#define I2S_IT_TXFOM                    I2S_IMR_TXFOM//TX FIFO Overrun interrupt.
#define I2S_IT_TXFEM                    I2S_IMR_TXFEM//TX FIFO Empty interrupt.
#define I2S_IT_RXFOM                    I2S_IMR_RXFOM//RX FIFO Overrun interrupt.
#define I2S_IT_RXDAM                    I2S_IMR_RXDAM//RX FIFO Data Available interrupt.

/** @defgroup I2S_Flags_Definition I2S Flags Definition
  * @{
  */
#define I2S_FLAG_TXFO                   I2S_ISR_TXFO
#define I2S_FLAG_TXFE                   I2S_ISR_TXFE
#define I2S_FLAG_RXFO                   I2S_ISR_RXFO
#define I2S_FLAG_RXDA                   I2S_ISR_RXDA

#define I2S_FLAG_MASK                   (I2S_ISR_TXFO | I2S_ISR_TXFE | I2S_ISR_RXFO | I2S_ISR_RXDA)

/* Exported macros -----------------------------------------------------------*/
/** @defgroup I2S_Exported_macros I2S Exported Macros
  * @{
  */

/** @brief  Enable the specified I2S peripheral.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @retval None
  */
#define __HAL_I2S_ENABLE(__HANDLE__)                (SET_BIT((__HANDLE__)->Instance->IER, I2S_IER_IEN))

/** @brief  Disable the specified I2S peripheral.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @retval None
  */
#define __HAL_I2S_DISABLE(__HANDLE__)               (CLEAR_BIT((__HANDLE__)->Instance->IER, I2S_IER_IEN))

/** @brief  Enable Receiver block.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @retval None
  */
#define __HAL_I2S_RX_BLOCK_ENABLE(__HANDLE__)       (SET_BIT((__HANDLE__)->Instance->IRER, I2S_IRER_RXEN))
/** @brief  Disable Receiver block.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @retval None
  */
#define __HAL_I2S_RX_BLOCK_DISABLE(__HANDLE__)      (CLEAR_BIT((__HANDLE__)->Instance->IRER, I2S_IRER_RXEN))
/** @brief  Enable Transmitter block.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @retval None
  */
#define __HAL_I2S_TX_BLOCK_ENABLE(__HANDLE__)       (SET_BIT((__HANDLE__)->Instance->ITER, I2S_ITER_TXEN))
/** @brief  Disable Transmitter block.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @retval None
  */
#define __HAL_I2S_TX_BLOCK_DISABLE(__HANDLE__)      (CLEAR_BIT((__HANDLE__)->Instance->ITER, I2S_ITER_TXEN))
/** @brief  Receiver FIFO Reset.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @retval None
  */
#define __HAL_I2S_RX_FLUSH(__HANDLE__)              (SET_BIT((__HANDLE__)->Instance->RXFFR, I2S_RXFFR_RXFFR))
/** @brief  Transmitter FIFO Reset.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @retval None
  */
#define __HAL_I2S_TX_FLUSH(__HANDLE__)              (SET_BIT((__HANDLE__)->Instance->TXFFR, I2S_TXFFR_TXFFR))

/** @brief  Enable DMA Receiver block.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @retval None
  */
#define __HAL_I2S_RX_BLOCK_DMA_ENABLE(__HANDLE__)   (SET_BIT((__HANDLE__)->Instance->DMACR, I2S_DMACR_DMAEN_RXBLOCK))
/** @brief  Disable DMA Receiver block.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @retval None
  */
#define __HAL_I2S_RX_BLOCK_DMA_DISABLE(__HANDLE__)  (CLEAR_BIT((__HANDLE__)->Instance->DMACR, I2S_DMACR_DMAEN_RXBLOCK))
/** @brief  Enable DMA Transmitter block.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @retval None
  */
#define __HAL_I2S_TX_BLOCK_DMA_ENABLE(__HANDLE__)   (SET_BIT((__HANDLE__)->Instance->DMACR, I2S_DMACR_DMAEN_TXBLOCK))
/** @brief  Disable DMA Transmitter block.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @retval None
  */
#define __HAL_I2S_TX_BLOCK_DMA_DISABLE(__HANDLE__)  (CLEAR_BIT((__HANDLE__)->Instance->DMACR, I2S_DMACR_DMAEN_TXBLOCK))

/** @brief  Enable Master Clock.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @retval None
  */
#define __HAL_I2S_CLK_ENABLE(__HANDLE__)    (SET_BIT((__HANDLE__)->Instance->CER, I2S_CER_CLKEN))
/** @brief  Disable Master Clock.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @retval None
  */
#define __HAL_I2S_CLK_DISABLE(__HANDLE__)    (CLEAR_BIT((__HANDLE__)->Instance->CER, I2S_CER_CLKEN))
#define __HAL_I2S_GET_CLK_EN(__HANDLE__)     (((__HANDLE__)->Instance->CER & I2S_CER_CLKEN) != RESET)

/** @brief  Enable Receiver Channel.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @param  __CHANNEL__ specifies the I2S Channel.
  * @retval None
  */
#define __HAL_I2S_CHX_RX_ENABLE(__HANDLE__, __CHANNEL__)   (SET_BIT((__HANDLE__)->Instance->Chx[(__CHANNEL__)].RER, I2S_RER_RXCHEN))
/** @brief  Disable Receiver Channel.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @param  __CHANNEL__ specifies the I2S Channel.
  * @retval None
  */
#define __HAL_I2S_CHX_RX_DISABLE(__HANDLE__, __CHANNEL__)   (CLEAR_BIT((__HANDLE__)->Instance->Chx[(__CHANNEL__)].RER, I2S_RER_RXCHEN))
/** @brief  Enable Transmitter Channel.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @param  __CHANNEL__ specifies the I2S Channel.
  * @retval None
  */
#define __HAL_I2S_CHX_TX_ENABLE(__HANDLE__, __CHANNEL__)   (SET_BIT((__HANDLE__)->Instance->Chx[(__CHANNEL__)].TER, I2S_TER_TXCHEN))
/** @brief  Disable Transmitter Channel.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @param  __CHANNEL__ specifies the I2S Channel.
  * @retval None
  */
#define __HAL_I2S_CHX_TX_DISABLE(__HANDLE__, __CHANNEL__)   (CLEAR_BIT((__HANDLE__)->Instance->Chx[(__CHANNEL__)].TER, I2S_TER_TXCHEN))

/** @brief  Enable Receiver Channel DMA.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @param  __CHANNEL__ specifies the I2S Channel.
  * @retval None
  */
#define __HAL_I2S_CHX_RX_DMA_ENABLE(__HANDLE__, __CHANNEL__)   (SET_BIT((__HANDLE__)->Instance->DMACR, I2S_DMACR_DMAEN_RXCH_0 << ((__CHANNEL__)-I2S_CHANNEL_0)))
/** @brief  Disable Receiver Channel DMA.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @param  __CHANNEL__ specifies the I2S Channel.
  * @retval None
  */
#define __HAL_I2S_CHX_RX_DMA_DISABLE(__HANDLE__, __CHANNEL__)   (CLEAR_BIT((__HANDLE__)->Instance->DMACR, I2S_DMACR_DMAEN_RXCH_0 << ((__CHANNEL__)-I2S_CHANNEL_0)))

/** @brief  Enable Transmitter Channel DMA.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @param  __CHANNEL__ specifies the I2S Channel.
  * @retval None
  */
#define __HAL_I2S_CHX_TX_DMA_ENABLE(__HANDLE__, __CHANNEL__)   (SET_BIT((__HANDLE__)->Instance->DMACR, I2S_DMACR_DMAEN_TXCH_0 << ((__CHANNEL__)-I2S_CHANNEL_0)))
/** @brief  Disable Transmitter Channel DMA.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @param  __CHANNEL__ specifies the I2S Channel.
  * @retval None
  */
#define __HAL_I2S_CHX_TX_DMA_DISABLE(__HANDLE__, __CHANNEL__)   (CLEAR_BIT((__HANDLE__)->Instance->DMACR, I2S_DMACR_DMAEN_TXCH_0 << ((__CHANNEL__)-I2S_CHANNEL_0)))

/** @brief  Enable the specified I2S interrupts.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @param  __CHANNEL__ specifies the I2S Channel.
  * @param  __INTERRUPT__ specifies the interrupt source to enable or disable.
  *         This parameter can be one of the following values:
  *            @arg I2S_IT_TXFOM: TX FIFO Overrun interrupt.
  *            @arg I2S_IT_TXFEM: TX FIFO Empty interrupt.
  *            @arg I2S_IT_RXFOM: RX FIFO Overrun interrupt.
  *            @arg I2S_IT_RXDAM: RX FIFO Data Available interrupt.
  * @retval None
  */
#define __HAL_I2S_CHX_ENABLE_IT(__HANDLE__, __CHANNEL__, __INTERRUPT__)    (CLEAR_BIT((__HANDLE__)->Instance->Chx[(__CHANNEL__)].IMR, (__INTERRUPT__)))

/** @brief  Disable the specified I2S interrupts.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @param  __CHANNEL__ specifies the I2S Channel.
  * @param  __INTERRUPT__ specifies the interrupt source to enable or disable.
  *         This parameter can be one of the following values:
  *            @arg I2S_IT_TXFOM: TX FIFO Overrun interrupt.
  *            @arg I2S_IT_TXFEM: TX FIFO Empty interrupt.
  *            @arg I2S_IT_RXFOM: RX FIFO Overrun interrupt.
  *            @arg I2S_IT_RXDAM: RX FIFO Data Available interrupt.
  * @retval None
  */
#define __HAL_I2S_CHX_DISABLE_IT(__HANDLE__, __CHANNEL__, __INTERRUPT__)  (SET_BIT((__HANDLE__)->Instance->Chx[(__CHANNEL__)].IMR, (__INTERRUPT__)))

/** @brief  Checks if the specified I2S interrupt source is enabled or disabled.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @param  __CHANNEL__ specifies the I2S Channel.
  * @param  __INTERRUPT__ specifies the I2S interrupt source to check.
  *          This parameter can be one of the following values:
  *            @arg I2S_IT_TXFOM: TX FIFO Overrun interrupt.
  *            @arg I2S_IT_TXFEM: TX FIFO Empty interrupt.
  *            @arg I2S_IT_RXFOM: RX FIFO Overrun interrupt.
  *            @arg I2S_IT_RXDAM: RX FIFO Data Available interrupt.
  * @retval The new state of __IT__ (TRUE or FALSE).
  */
#define __HAL_I2S_CHX_GET_IT_SOURCE(__HANDLE__, __CHANNEL__, __INTERRUPT__) ((((__HANDLE__)->Instance->Chx[(__CHANNEL__)].IMR & (__INTERRUPT__)) == (__INTERRUPT__)) ? RESET : SET)
/** @brief  Checks whether the specified I2S flag is set or not.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @param  __CHANNEL__ specifies the I2S Channel.
  * @param  __FLAG__ specifies the flag to check.
  *         This parameter can be one of the following values:
  *            @arg I2S_FLAG_TXFO: Status of Data Overrun interrupt for the TX channel.
  *            @arg I2S_FLAG_TXFE: Status of Transmit Empty Trigger interrupt.
  *            @arg I2S_FLAG_RXFO: Status of Data Overrun interrupt for the RX channel.
  *            @arg I2S_FLAG_RXDA: Status of Receive Data Available interrupt.
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_I2S_CHX_GET_FLAG(__HANDLE__, __CHANNEL__, __FLAG__) ((((__HANDLE__)->Instance->Chx[(__CHANNEL__)].ISR) & (__FLAG__)) == (__FLAG__))


/** @brief Clears the I2S RX OVR pending flag.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @param  __CHANNEL__ specifies the I2S Channel.
  * @retval None
  */
#define __HAL_I2S_CHX_RX_CLEAR_OVRFLAG(__HANDLE__, __CHANNEL__)  \
do{ \
    __IO uint32_t tmpreg_ovr = 0x00U; \
    tmpreg_ovr = (__HANDLE__)->Instance->Chx[(__CHANNEL__)].ROR; \
    UNUSED(tmpreg_ovr); \
}while(0U)

/** @brief Clears the I2S TX OVR pending flag.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @param  __CHANNEL__ specifies the I2S Channel.
  * @retval None
  */
#define __HAL_I2S_CHX_TX_CLEAR_OVRFLAG(__HANDLE__, __CHANNEL__)  \
do{ \
    __IO uint32_t tmpreg_ovr = 0x00U; \
    tmpreg_ovr = (__HANDLE__)->Instance->Chx[(__CHANNEL__)].TOR; \
    UNUSED(tmpreg_ovr); \
}while(0U)

/** @brief Receive Channel FIFO Reset.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @param  __CHANNEL__ specifies the I2S Channel.
  * @retval None
  */
#define __HAL_I2S_CHX_RX_FIFO_RESET(__HANDLE__, __CHANNEL__)  (SET_BIT((__HANDLE__)->Instance->Chx[(__CHANNEL__)].RFF, I2S_RFF_RXCHFR))
/** @brief Transmit Channel FIFO Reset.
  * @param  __HANDLE__ specifies the I2S Handle.
  * @param  __CHANNEL__ specifies the I2S Channel.
  * @retval None
  */
#define __HAL_I2S_CHX_TX_FIFO_RESET(__HANDLE__, __CHANNEL__)  (SET_BIT((__HANDLE__)->Instance->Chx[(__CHANNEL__)].TFF, I2S_TFF_TXCHFR))

/* Exported functions --------------------------------------------------------*/
/** @addtogroup I2S_Exported_Functions
  * @{
  */

/** @addtogroup I2S_Exported_Functions_Group1
  * @{
  */
/* Initialization/de-initialization functions  ********************************/
void HAL_I2S_StructInit(I2S_InitTypeDef* I2S_InitStruct);
HAL_StatusTypeDef HAL_I2S_Init(I2S_HandleTypeDef *hi2s);
HAL_StatusTypeDef HAL_I2S_DeInit(I2S_HandleTypeDef *hi2s);
void HAL_I2S_MspInit(I2S_HandleTypeDef *hi2s);
void HAL_I2S_MspDeInit(I2S_HandleTypeDef *hi2s);

/* Callbacks Register/UnRegister functions  ***********************************/
#if (USE_HAL_I2S_REGISTER_CALLBACKS == 1U)
HAL_StatusTypeDef HAL_I2S_RegisterCallback(I2S_HandleTypeDef *hi2s, HAL_I2S_CallbackIDTypeDef CallbackID, pI2S_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_I2S_UnRegisterCallback(I2S_HandleTypeDef *hi2s, HAL_I2S_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_I2S_REGISTER_CALLBACKS */

/** @addtogroup I2S_Exported_Functions_Group2
  * @{
  */
/* I/O operation functions  ***************************************************/
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_I2S_Transmit(I2S_HandleTypeDef *hi2s, uint8_t *pData, uint32_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2S_Receive(I2S_HandleTypeDef *hi2s, uint8_t *pData, uint32_t Size, uint32_t Timeout);

/* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_I2S_Transmit_IT(I2S_HandleTypeDef *hi2s, uint8_t *pData, uint32_t Size);
HAL_StatusTypeDef HAL_I2S_Receive_IT(I2S_HandleTypeDef *hi2s, uint8_t *pData, uint32_t Size);
void HAL_I2S_IRQHandler(I2S_HandleTypeDef *hi2s);

/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_I2S_Transmit_DMA(I2S_HandleTypeDef *hi2s, uint8_t *pData, uint32_t Size);
HAL_StatusTypeDef HAL_I2S_Receive_DMA(I2S_HandleTypeDef *hi2s, uint8_t *pData, uint32_t Size);

HAL_StatusTypeDef HAL_I2S_DMAPause(I2S_HandleTypeDef *hi2s);
HAL_StatusTypeDef HAL_I2S_DMAResume(I2S_HandleTypeDef *hi2s);
HAL_StatusTypeDef HAL_I2S_DMAStop(I2S_HandleTypeDef *hi2s);

/* Callbacks used in non blocking modes (Interrupt and DMA) *******************/
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s);

/** @addtogroup I2S_Exported_Functions_Group3
  * @{
  */
/* Peripheral Control and State functions  ************************************/
HAL_I2S_StateTypeDef HAL_I2S_GetState(I2S_HandleTypeDef *hi2s);
uint32_t HAL_I2S_GetError(I2S_HandleTypeDef *hi2s);

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup I2S_Private_Constants I2S Private Constants
  * @{
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup I2S_Private_Macros I2S Private Macros
  * @{
  */

/** @brief  Check whether the specified SPI flag is set or not.
  * @param  __SR__  copy of I2S SR regsiter.
  * @param  __FLAG__ specifies the flag to check.
  *         This parameter can be one of the following values:
  *            @arg I2S_FLAG_RXNE: Receive buffer not empty flag
  *            @arg I2S_FLAG_TXE: Transmit buffer empty flag
  *            @arg I2S_FLAG_UDR: Underrun error flag
  *            @arg I2S_FLAG_OVR: Overrun flag
  *            @arg I2S_FLAG_CHSIDE: Channel side flag
  *            @arg I2S_FLAG_BSY: Busy flag
  * @retval SET or RESET.
  */
#define I2S_CHECK_FLAG(__SR__, __FLAG__)         ((((__SR__) & ((__FLAG__) & I2S_FLAG_MASK)) == ((__FLAG__) & I2S_FLAG_MASK)) ? SET : RESET)

/** @brief  Check whether the specified SPI Interrupt is set or not.
  * @param  __CR2__  copy of I2S CR2 regsiter.
  * @param  __INTERRUPT__ specifies the SPI interrupt source to check.
  *         This parameter can be one of the following values:
  *            @arg I2S_IT_TXE: Tx buffer empty interrupt enable
  *            @arg I2S_IT_RXNE: RX buffer not empty interrupt enable
  *            @arg I2S_IT_ERR: Error interrupt enable
  * @retval SET or RESET.
  */
#define I2S_CHECK_IT_SOURCE(__CR2__, __INTERRUPT__)      ((((__CR2__) & (__INTERRUPT__)) == (__INTERRUPT__)) ? SET : RESET)

#endif /* HAL_I2S_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC90XX_HAL_I2S_H__ */

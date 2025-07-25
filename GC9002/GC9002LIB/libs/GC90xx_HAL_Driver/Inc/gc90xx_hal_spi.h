/**
  ******************************************************************************
  * @file    gc90xx_hal_spi.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2023-1-6
  * @brief   This file contains all the functions prototypes for the SPI
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC90XX_HAL_SPI_H__

#define __GC90XX_HAL_SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <gc90xx_hal_def.h>

/** @addtogroup SPI
  * @{
  */

/* Exported types ------------------------------------------------------------*/
#define __HAL_SPI_DRIVER_ATTRIBUTE              __attribute__((optimize(0)))
/** @defgroup SPI_Exported_Types SPI Exported Types
  * @{
  */

/**
  * @brief  SPI Configuration Structure definition
  */
typedef struct
{
    uint16_t Mode;                  /*!< Specifies the SPI operating mode. This parameter can be a value of @ref SPI_mode */
    uint16_t DataSize;              /*!< Specifies the SPI data size. This parameter can be a value of @ref SPI_Data_Size */
    uint32_t FrameFormat : 16;      /*!< Specifies the SPI operating mode. This parameter can be a value of @ref SPI_FrameFormat */
    uint32_t             : 16;
    uint16_t SPO;                   /*!< Specifies the serial clock steady state.
                                        SPI_SPO_LOW ---Indicates that the SPI clock signal remains low when idle
                                        SPI_SPO_HIGH---Indicates that the SPI clock signal remains high when idle
                                         This parameter can be a value of @ref SPI_Clock_Polarity */
    uint16_t SPH;                   /*!< Specifies the clock active edge for the bit capture.
                                        This parameter can be a value of @ref SPI_Clock_Phase */
    uint16_t CPSDVR;                /*!< Specifies the Baud Rate prescaler value which will be
                                         used to configure the transmit and receive SCK clock. */
    uint16_t SCR;                   /*!< Specifies the Baud Rate prescaler value which will be
                                         used to configure the transmit and receive SCK clock. */
                                    /*!< SPI_CPSDVR & SPI_SCR must used as below
                                         SSPCLKOUT = SSPCLK / ((CPSDVR * (1 + SCR))) */
} SPI_InitTypeDef;

/**
  * @brief  HAL SPI State structure definition
  */
typedef enum
{
    HAL_SPI_STATE_RESET       = 0x00U,    /*!< Peripheral not Initialized                         */
    HAL_SPI_STATE_READY       = 0x01U,    /*!< Peripheral Initialized and ready for use           */
    HAL_SPI_STATE_BUSY        = 0x02U,    /*!< an internal process is ongoing                     */
    HAL_SPI_STATE_BUSY_TX     = 0x04U,    /*!< Data Transmission process is ongoing               */
    HAL_SPI_STATE_BUSY_RX     = 0x08U,    /*!< Data Reception process is ongoing                  */
    HAL_SPI_STATE_BUSY_DMA_TX = 0x10U,    /*!< Data DMA Transmission process is ongoing           */
    HAL_SPI_STATE_BUSY_DMA_RX = 0x20U,    /*!< Data DMA Reception process is ongoing              */
    HAL_SPI_STATE_ERROR       = 0x40U,    /*!< SPI error state                                    */
    HAL_SPI_STATE_ABORT       = 0x80U     /*!< SPI abort is ongoing                               */
} HAL_SPI_StateTypeDef;

/**
  * @brief  SPI handle Structure definition
  */
typedef struct __SPI_HandleTypeDef
{
    SPI_TypeDef                *Instance;      /*!< SPI registers base address               */
    SPI_InitTypeDef            Init;           /*!< SPI communication parameters             */

    uint8_t                    *pTxBuffPtr;    /*!< Pointer to SPI Tx transfer Buffer        */
    uint16_t                   TxXferSize;     /*!< SPI Tx Transfer size                     */
    __IO uint16_t              TxXferCount;    /*!< SPI Tx Transfer Counter                  */

    uint8_t                    *pRxBuffPtr;    /*!< Pointer to SPI Rx transfer Buffer        */
    uint16_t                   RxXferSize;     /*!< SPI Rx Transfer size                     */
    __IO uint16_t              RxXferCount;    /*!< SPI Rx Transfer Counter                  */

    void (*RxISR)(struct __SPI_HandleTypeDef *hspi);   /*!< function pointer on Rx ISR       */
    void (*TxISR)(struct __SPI_HandleTypeDef *hspi);   /*!< function pointer on Tx ISR       */

    DMA_HandleTypeDef          *hdmatx;        /*!< SPI Tx DMA Handle parameters             */
    DMA_HandleTypeDef          *hdmarx;        /*!< SPI Rx DMA Handle parameters             */

    HAL_LockTypeDef            Lock;           /*!< Locking object                           */

    __IO HAL_SPI_StateTypeDef  State;          /*!< SPI communication state                  */
    __IO HAL_SPI_StateTypeDef  RxState;        /*!< SPI state information related to Rx operations.
                                                       This parameter can be a value of @ref HAL_SPI_StateTypeDef */
    __IO uint32_t              ErrorCode;      /*!< SPI Error code                           */
    void                       *priv_obj;

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
    void (* TxCpltCallback)(struct __SPI_HandleTypeDef *hspi);             /*!< SPI Tx Completed callback          */
    void (* RxCpltCallback)(struct __SPI_HandleTypeDef *hspi);             /*!< SPI Rx Completed callback          */
    void (* TxRxCpltCallback)(struct __SPI_HandleTypeDef *hspi);           /*!< SPI TxRx Completed callback        */
    void (* ErrorCallback)(struct __SPI_HandleTypeDef *hspi);              /*!< SPI Error callback                 */
    void (* AbortCpltCallback)(struct __SPI_HandleTypeDef *hspi);          /*!< SPI Abort callback                 */
    void (* MspInitCallback)(struct __SPI_HandleTypeDef *hspi);            /*!< SPI Msp Init callback              */
    void (* MspDeInitCallback)(struct __SPI_HandleTypeDef *hspi);          /*!< SPI Msp DeInit callback            */
#endif  /* USE_HAL_SPI_REGISTER_CALLBACKS */
} SPI_HandleTypeDef;

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
/**
  * @brief  HAL SPI Callback ID enumeration definition
  */
typedef enum
{
    HAL_SPI_TX_COMPLETE_CB_ID             = 0x00U,    /*!< SPI Tx Completed callback ID         */
    HAL_SPI_RX_COMPLETE_CB_ID             = 0x01U,    /*!< SPI Rx Completed callback ID         */
    HAL_SPI_TX_RX_COMPLETE_CB_ID          = 0x02U,    /*!< SPI TxRx Completed callback ID       */
    HAL_SPI_ERROR_CB_ID                   = 0x03U,    /*!< SPI Error callback ID                */
    HAL_SPI_ABORT_CB_ID                   = 0x04U,    /*!< SPI Abort callback ID                */
    HAL_SPI_MSPINIT_CB_ID                 = 0x05U,    /*!< SPI Msp Init callback ID             */
    HAL_SPI_MSPDEINIT_CB_ID               = 0x06U     /*!< SPI Msp DeInit callback ID           */
} HAL_SPI_CallbackIDTypeDef;

/**
  * @brief  HAL SPI Callback pointer definition
  */
typedef  void (*pSPI_CallbackTypeDef)(SPI_HandleTypeDef *hspi); /*!< pointer to an SPI callback function */

#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup SPI_Exported_Constants SPI Exported Constants
  * @{
  */

#define HAL_SPI_FIFO_DEEP               (8U)
#define HAL_SPI_HALF_FIFO_DEEP          (4U)
#define SPI_DUMMY_BYTE                  (0xFFU)
#define SPI_DUMMY_WORD                  (0xFFFFU)

/** @defgroup SPI_Error_Code SPI Error Code
  * @{
  */
#define HAL_SPI_ERROR_NONE              (0x00000000U)   /*!< No error                               */
#define HAL_SPI_ERROR_MODF              (0x00000001U)   /*!< MODF error                             */
#define HAL_SPI_ERROR_CRC               (0x00000002U)   /*!< CRC error                              */
#define HAL_SPI_ERROR_OVR               (0x00000004U)   /*!< OVR error                              */
#define HAL_SPI_ERROR_FRE               (0x00000008U)   /*!< FRE error                              */
#define HAL_SPI_ERROR_DMA               (0x00000010U)   /*!< DMA transfer error                     */
#define HAL_SPI_ERROR_FLAG              (0x00000020U)   /*!< Error on RXNE/TXE/BSY Flag             */
#define HAL_SPI_ERROR_ABORT             (0x00000040U)   /*!< Error during SPI Abort procedure       */
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
#define HAL_SPI_ERROR_INVALID_CALLBACK  (0x00000080U)   /*!< Invalid Callback error                 */
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @defgroup SPI_Data_Size SPI Data Size
  * @{
  */
#define SPI_DATASIZE_4BIT               (SPI_CR0_DSS_1 | SPI_CR0_DSS_0)
#define SPI_DATASIZE_5BIT               (SPI_CR0_DSS_2)
#define SPI_DATASIZE_6BIT               (SPI_CR0_DSS_2 | SPI_CR0_DSS_0)
#define SPI_DATASIZE_7BIT               (SPI_CR0_DSS_2 | SPI_CR0_DSS_1)
#define SPI_DATASIZE_8BIT               (SPI_CR0_DSS_2 | SPI_CR0_DSS_1 | SPI_CR0_DSS_0)
#define SPI_DATASIZE_9BIT               (SPI_CR0_DSS_3)
#define SPI_DATASIZE_10BIT              (SPI_CR0_DSS_3 | SPI_CR0_DSS_0)
#define SPI_DATASIZE_11BIT              (SPI_CR0_DSS_3 | SPI_CR0_DSS_1)
#define SPI_DATASIZE_12BIT              (SPI_CR0_DSS_3 | SPI_CR0_DSS_1 | SPI_CR0_DSS_0)
#define SPI_DATASIZE_13BIT              (SPI_CR0_DSS_3 | SPI_CR0_DSS_2)
#define SPI_DATASIZE_14BIT              (SPI_CR0_DSS_3 | SPI_CR0_DSS_2 | SPI_CR0_DSS_0)
#define SPI_DATASIZE_15BIT              (SPI_CR0_DSS_3 | SPI_CR0_DSS_2 | SPI_CR0_DSS_1)
#define SPI_DATASIZE_16BIT              (SPI_CR0_DSS_3 | SPI_CR0_DSS_2 | SPI_CR0_DSS_1 | SPI_CR0_DSS_0)
/**
  * @}
  */

/** @defgroup SPI_FrameFormat
  * @{
  */
#define SPI_FRF_MOTOROLA                (0x00000000U)
#define SPI_FRF_TI                      SPI_CR0_FRF_0
#define SPI_FRF_NATIONALMICROWIRE       SPI_CR0_FRF_1
/**
  * @}
  */

/** @defgroup SPI_Clock_Polarity SPI Clock Polarity
  * @{
  */
#define SPI_SPO_LOW                     (0x00000000U)
#define SPI_SPO_HIGH                    SPI_CR0_SPO
/**
  * @}
  */

/** @defgroup SPI_Clock_Phase SPI Clock Phase
  * @{
  */
#define SPI_SPH_1EDGE                   (0x00000000U)
#define SPI_SPH_2EDGE                   SPI_CR0_SPH
/**
  * @}
  */

/** @defgroup SPI_mode SPI Normal or Loop back Mode
  * @{
  */
#define SPI_MODE_NORMAL                 (0x00000000U)
#define SPI_MODE_LOOP_BACK              SPI_CR1_LOOPBACK
/**
  * @}
  */

/** @defgroup SPI_mode SPI Master or Slave Mode
  * @{
  */
#define SPI_MODE_MASTER                 (0x00000000U)
#define SPI_MODE_SLAVE                  SPI_CR1_SLAVE
/**
  * @}
  */

/** @defgroup SPI Slave Output Disable or Enable
  * @{
  */
#define SPI_Mode_SLAVE_OUTPUT_ENABLE    (0x00000000U)
#define SPI_Mode_SLAVE_OUTPUT_DISABLE   SPI_CR1_SLAVEMODE_TXDIS
/**
  * @}
  */

/** @defgroup SPI_Interrupt_definition SPI Interrupt Definition
  * @{
  */
#define SPI_IT_TXE                      SPI_IMSC_TXIM
#define SPI_IT_RXNE                     SPI_IMSC_RXIM
#define SPI_IT_TIMEOUT                  SPI_IMSC_RTIM
#define SPI_IT_OVERRUN                  SPI_IMSC_RORIM
#define SPI_IT_ERR                      (SPI_IT_TIMEOUT | SPI_IT_OVERRUN)
/**
  * @}
  */

/** @defgroup SPI_Interrupt_Flag_definition SPI Interrupt Flag Definition
  * @{
  */
#define SPI_IT_TXE_FLAG                 SPI_RIS_TXRIS
#define SPI_IT_RXNE_FLAG                SPI_RIS_RXRIS
#define SPI_IT_RXT_FLAG                 SPI_RIS_RTRIS
#define SPI_IT_RXO_FLAG                 SPI_RIS_RORRIS
#define SPI_IT_ERR_FLAG                 (SPI_IT_RXT_FLAG | SPI_IT_RXO_FLAG)
/**
  * @}
  */

/** @defgroup SPI_Flags_definition SPI Flags Definition
  * @{
  */
#define SPI_FLAG_TXFE                   SPI_SR_TXFE   /* SPI status flag: Tx buffer empty flag           */
#define SPI_FLAG_TXNF                   SPI_SR_TXNF   /* SPI status flag: Tx buffer not full flag        */
#define SPI_FLAG_RXNE                   SPI_SR_RXNE   /* SPI status flag: Rx buffer not empty flag       */
#define SPI_FLAG_RXFF                   SPI_SR_RXFF   /* SPI status flag: Rx buffer full flag            */
#define SPI_FLAG_BSY                    SPI_SR_BSY    /* SPI status flag: Busy flag                      */
#define SPI_FLAG_MASK                   (SPI_FLAG_TXFE | SPI_FLAG_TXNF | SPI_FLAG_RXNE | SPI_FLAG_RXFF | SPI_FLAG_BSY)
/**
  * @}
  */

/** @defgroup SPI_MIS_definition SPI MIS Definition
  * @{
  */
#define SPI_MIS_TX                      SPI_MIS_TXMIS
#define SPI_MIS_RX                      SPI_MIS_RXMIS
#define SPI_MIS_RT                      SPI_MIS_RTMIS
#define SPI_MIS_ROR                     SPI_MIS_RORMIS
#define SPI_MIS_MASK                    (SPI_MIS_TX | SPI_MIS_RX | SPI_MIS_RT | SPI_MIS_ROR)
/**
  * @}
  */

/** @defgroup SPI_ICR_definition SPI ICR Definition
  * @{
  */
#define SPI_IT_ERR_CLEAR                (SPI_ICR_RORICR | SPI_ICR_RTICR)
/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup SPI_Exported_Macros SPI Exported Macros
  * @{
  */



/** @brief  Enable the specified SPI interrupts.
  * @param  __HANDLE__ specifies the SPI Handle.
  *         This parameter can be SPI where x: 0, 1, 2, 3 or 4 to select the SPI peripheral.
  * @param  __INTERRUPT__ specifies the interrupt source to enable.
  *         This parameter can be one of the following values:
  *            @arg SPI_IT_TXE: Tx buffer empty interrupt enable
  *            @arg SPI_IT_RXNE: RX buffer not empty interrupt enable
  *            @arg SPI_IT_RXT: RX timeout interrupt enable
  *            @arg SPI_IT_RXO: RX overrun interrupt enable
  * @retval None
  */
#define __HAL_SPI_ENABLE_IT(__HANDLE__, __INTERRUPT__)   SET_BIT((__HANDLE__)->Instance->IMSC, (__INTERRUPT__))

/** @brief  Disable the specified SPI interrupts.
  * @param  __HANDLE__ specifies the SPI handle.
  *         This parameter can be SPIx where x: 0, 1, 2, 3 or 4 to select the SPI peripheral.
  * @param  __INTERRUPT__ specifies the interrupt source to disable.
  *         This parameter can be one of the following values:
  *            @arg SPI_IT_TXE: Tx buffer empty interrupt enable
  *            @arg SPI_IT_RXNE: RX buffer not empty interrupt enable
  *            @arg SPI_IT_RXT: RX timeout interrupt enable
  *            @arg SPI_IT_RXO: RX overrun interrupt enable
  * @retval None
  */
#define __HAL_SPI_DISABLE_IT(__HANDLE__, __INTERRUPT__)  CLEAR_BIT((__HANDLE__)->Instance->IMSC, (__INTERRUPT__))

/** @brief  Check whether the specified SPI interrupt source is enabled or not.
  * @param  __HANDLE__ specifies the SPI Handle.
  *         This parameter can be SPI where x: 0, 1, 2, 3 or 4 to select the SPI peripheral.
  * @param  __INTERRUPT__ specifies the SPI interrupt source to check.
  *          This parameter can be one of the following values:
  *            @arg SPI_IT_TXE: Tx buffer empty interrupt enable
  *            @arg SPI_IT_RXNE: RX buffer not empty interrupt enable
  *            @arg SPI_IT_RXT: RX timeout interrupt enable
  *            @arg SPI_IT_RXO: RX overrun interrupt enable
  * @retval The new state of __IT__ (TRUE or FALSE).
  */
#define __HAL_SPI_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) ((((__HANDLE__)->Instance->IMSC\
                                                              & (__INTERRUPT__)) == (__INTERRUPT__)) ? SET : RESET)

/** @brief  Check whether the specified SPI flag is set or not.
  * @param  __HANDLE__ specifies the SPI Handle.
  *         This parameter can be SPI where x: 0, 1, 2, 3, or 4 to select the SPI peripheral.
  * @param  __FLAG__ specifies the flag to check.
  *         This parameter can be one of the following values:
  *            @arg SPI_FLAG_TXFE: Receive fifo empty
  *            @arg SPI_FLAG_TXNF: Transmit fifo not full
  *            @arg SPI_FLAG_RXNE: Receive fifo not empty
  *            @arg SPI_FLAG_RXFF: Receive fifo full
  *            @arg SPI_FLAG_BSY: Busy flag
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_SPI_GET_FLAG(__HANDLE__, __FLAG__) ((((__HANDLE__)->Instance->SR) & (__FLAG__)) == (__FLAG__))

/** @brief  Enable the SPI peripheral.
  * @param  __HANDLE__ specifies the SPI Handle.
  *         This parameter can be SPI where x: 0, 1, 2, 3 or 4 to select the SPI peripheral.
  * @retval None
  */
#define __HAL_SPI_ENABLE(__HANDLE__)  SET_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_SPE)

/** @brief  Disable the SPI peripheral.
  * @param  __HANDLE__ specifies the SPI Handle.
  *         This parameter can be SPI where x: 0, 1, 2, 3 or 4 to select the SPI peripheral.
  * @retval None
  */
#define __HAL_SPI_DISABLE(__HANDLE__) CLEAR_BIT((__HANDLE__)->Instance->CR1, SPI_CR1_SPE)

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup SPI_Private_Macros SPI Private Macros
  * @{
  */

/** @brief  Check whether the specified SPI flag is set or not.
  * @param  __SR__  copy of SPI SR register.
  * @param  __FLAG__ specifies the flag to check.
  *         This parameter can be one of the following values:
  *            @arg SPI_FLAG_TXFE: Receive fifo empty
  *            @arg SPI_FLAG_TXNF: Transmit fifo not full
  *            @arg SPI_FLAG_RXNE: Receive fifo not empty
  *            @arg SPI_FLAG_RXFF: Receive fifo full
  * @retval SET or RESET.
  */
#define SPI_CHECK_FLAG(__SR__, __FLAG__)   ((((__SR__) & ((__FLAG__) & SPI_FLAG_MASK)) == \
                                            ((__FLAG__) & SPI_FLAG_MASK)) ? SET : RESET)

#define SPI_CHECK_MIS(__MIS__, __STATUS__) ((((__MIS__) & ((__STATUS__) & SPI_MIS_MASK)) == \
                                            ((__STATUS__) & SPI_MIS_MASK)) ? SET : RESET)

#define __HAL_SPI_SET_ICR(__HANDLE__)      SET_BIT((__HANDLE__)->Instance->ICR, SPI_IT_ERR_CLEAR)

/** @brief  Check whether the specified SPI Interrupt is set or not.
  * @param  __CR2__  copy of SPI CR2 register.
  * @param  __INTERRUPT__ specifies the SPI interrupt source to check.
  *         This parameter can be one of the following values:
  *            @arg SPI_IT_TXE_FLAG: Tx buffer empty interrupt flag
  *            @arg SPI_IT_RXNE_FLAG: RX buffer not empty interrupt flag
  *            @arg SPI_IT_RXT_FLAG: RX timeout interrupt flag
  *            @arg SPI_IT_RXO_FLAG: RX overrun interrupt flag
  * @retval SET or RESET.
  */
#define SPI_CHECK_IT_SOURCE(__RIS__, __INTERRUPT__) ((((__RIS__) & (__INTERRUPT__)) == \
                                                     (__INTERRUPT__)) ? SET : RESET)

/** @brief  Checks if SPI Mode parameter is in allowed range.
  * @param  __MODE__ specifies the SPI Mode.
  *         This parameter can be a value of @ref SPI_Mode
  * @retval None
  */
#define IS_SPI_MODE(__MODE__)      (((__MODE__) == SPI_MODE_SLAVE)   || \
                                    ((__MODE__) == SPI_MODE_MASTER))

#define IS_SPI_DATASIZE(__DATASIZE__) (((__DATASIZE__) == SPI_DATASIZE_16BIT) || \
                                       ((__DATASIZE__) == SPI_DATASIZE_8BIT))

/** @brief  Checks if SPI Serial clock steady state parameter is in allowed range.
  * @param  __CPOL__ specifies the SPI serial clock steady state.
  *         This parameter can be a value of @ref SPI_Clock_Polarity
  * @retval None
  */
#define IS_SPI_CPOL(__CPOL__)      (((__CPOL__) == SPI_SPO_LOW) || \
                                    ((__CPOL__) == SPI_SPO_HIGH))

/** @brief  Checks if SPI Clock Phase parameter is in allowed range.
  * @param  __CPHA__ specifies the SPI Clock Phase.
  *         This parameter can be a value of @ref SPI_Clock_Phase
  * @retval None
  */
#define IS_SPI_CPHA(__CPHA__)      (((__CPHA__) == SPI_SPH_1EDGE) || \
                                    ((__CPHA__) == SPI_SPH_2EDGE))

#define __HAL_SPI_DMA_RX_ENABLE(__HANDLE__)      SET_BIT((__HANDLE__)->Instance->DMACR, SPI_DMACR_RXEDMACR)
#define __HAL_SPI_DMA_TX_ENABLE(__HANDLE__)      SET_BIT((__HANDLE__)->Instance->DMACR, SPI_DMACR_TXEDMACR)
#define __HAL_SPI_DMA_RX_DISABLE(__HANDLE__)     CLEAR_BIT((__HANDLE__)->Instance->DMACR, SPI_DMACR_RXEDMACR)
#define __HAL_SPI_DMA_TX_DISABLE(__HANDLE__)     CLEAR_BIT((__HANDLE__)->Instance->DMACR, SPI_DMACR_TXEDMACR)

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup SPI_Exported_Functions
  * @{
  */

/** @addtogroup SPI_Exported_Functions_Group1
  * @{
  */
/* Initialization/de-initialization functions  ********************************/
HAL_StatusTypeDef HAL_SPI_StructInit(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_DeInit(SPI_HandleTypeDef *hspi);
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi);
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi);

/* Callbacks Register/UnRegister functions  ***********************************/
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
HAL_StatusTypeDef HAL_SPI_RegisterCallback(SPI_HandleTypeDef *hspi, HAL_SPI_CallbackIDTypeDef CallbackID, pSPI_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_SPI_UnRegisterCallback(SPI_HandleTypeDef *hspi, HAL_SPI_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
/**
  * @}
  */

/** @addtogroup SPI_Exported_Functions_Group2
  * @{
  */
/* I/O operation functions  ***************************************************/
HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, const uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, const uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout);

HAL_StatusTypeDef HAL_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, const uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_Receive_IT(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_TransmitReceive_IT(SPI_HandleTypeDef *hspi, const uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);

HAL_StatusTypeDef HAL_SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, const uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi, const uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
/* Transfer Abort functions */
HAL_StatusTypeDef HAL_SPI_Abort(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_Abort_IT(SPI_HandleTypeDef *hspi);

void HAL_SPI_IRQHandler(SPI_HandleTypeDef *hspi);
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi);
void HAL_SPI_AbortCpltCallback(SPI_HandleTypeDef *hspi);

void HAL_SPI_IRQHandler(SPI_HandleTypeDef *hspi);
/**
  * @}
  */

/** @addtogroup SPI_Exported_Functions_Group3
  * @{
  */
/* Peripheral State and Error functions ***************************************/
HAL_SPI_StateTypeDef HAL_SPI_GetState(SPI_HandleTypeDef *hspi);
//uint32_t             HAL_SPI_GetError(SPI_HandleTypeDef *hspi);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __GC90XX_HAL_SPI_H__ */


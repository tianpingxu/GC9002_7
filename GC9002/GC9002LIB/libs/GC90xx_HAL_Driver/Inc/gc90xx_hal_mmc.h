/**
  ******************************************************************************
  * @file    gc90xx_hal_sd.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2022-10-18
  * @brief   This file contains all the functions prototypes for the MMC
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC90XX_HAL_MMC_H__
#define __GC90XX_HAL_MMC_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gc90xx_ll_sdmmc.h"
#ifdef HAL_MMC_MODULE_ENABLED

/** @addtogroup GC90xx_HAL_Driver
  * @{
  */

/** @defgroup MMC MMC
  * @brief MMC HAL module driver
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/ 
/** @defgroup MMC_Exported_Types MMC Exported Types
  * @{
  */

/** @defgroup MMC_Exported_Types_Group1 MMC State enumeration structure
  * @{
  */   
typedef enum
{
    HAL_MMC_STATE_RESET                  = 0x00000000U,  /*!< MMC not yet initialized or disabled  */
    HAL_MMC_STATE_READY                  = 0x00000001U,  /*!< MMC initialized and ready for use    */
    HAL_MMC_STATE_TIMEOUT                = 0x00000002U,  /*!< MMC Timeout state                    */
    HAL_MMC_STATE_BUSY                   = 0x00000003U,  /*!< MMC process ongoing                  */
    HAL_MMC_STATE_PROGRAMMING            = 0x00000004U,  /*!< MMC Programming State                */
    HAL_MMC_STATE_RECEIVING              = 0x00000005U,  /*!< MMC Receinving State                 */
    HAL_MMC_STATE_TRANSFER               = 0x00000006U,  /*!< MMC Transfert State                  */
    HAL_MMC_STATE_ERROR                  = 0x0000000FU   /*!< MMC is in error state                */
}HAL_MMC_StateTypeDef;

/** @defgroup MMC_Exported_Types_Group2 MMC Card State enumeration structure
  * @{
  */   
typedef enum
{
    HAL_MMC_CARD_READY                  = 0x00000001U,  /*!< Card state is ready                     */
    HAL_MMC_CARD_IDENTIFICATION         = 0x00000002U,  /*!< Card is in identification state         */
    HAL_MMC_CARD_STANDBY                = 0x00000003U,  /*!< Card is in standby state                */
    HAL_MMC_CARD_TRANSFER               = 0x00000004U,  /*!< Card is in transfer state               */
    HAL_MMC_CARD_SENDING                = 0x00000005U,  /*!< Card is sending an operation            */
    HAL_MMC_CARD_RECEIVING              = 0x00000006U,  /*!< Card is receiving operation information */
    HAL_MMC_CARD_PROGRAMMING            = 0x00000007U,  /*!< Card is in programming state            */
    HAL_MMC_CARD_DISCONNECTED           = 0x00000008U,  /*!< Card is disconnected                    */
    HAL_MMC_CARD_ERROR                  = 0x000000FFU   /*!< Card response Error                     */
}HAL_MMC_CardStateTypeDef;

/** @defgroup MMC_Exported_Types_Group3 MMC Handle Structure definition   
  * @{
  */
#define MMC_InitTypeDef      SDIO_InitTypeDef 
#define MMC_TypeDef          SDIO_TypeDef

/** 
  * @brief  MMC Card Information Structure definition
  */ 
typedef struct
{
    uint32_t CardType;                     /*!< Specifies the card Type                         */
    uint32_t Class;                        /*!< Specifies the class of the card class           */
    uint32_t RelCardAdd;                   /*!< Specifies the Relative Card Address             */
    uint32_t BlockNbr;                     /*!< Specifies the Card Capacity in blocks           */
    uint32_t BlockSize;                    /*!< Specifies one block size in bytes               */
    uint32_t LogBlockNbr;                  /*!< Specifies the Card logical Capacity in blocks   */
    uint32_t LogBlockSize;                 /*!< Specifies logical block size in bytes           */
}HAL_MMC_CardInfoTypeDef;

/** 
  * @brief  MMC handle Structure definition
  */ 
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)
typedef struct __MMC_HandleTypeDef
#else
typedef struct
#endif /* USE_HAL_MMC_REGISTER_CALLBACKS */
{
    MMC_TypeDef                 *Instance;        /*!< MMC registers base address            */
    MMC_InitTypeDef              Init;             /*!< MMC required parameters              */
    SDIO_DataInitTypeDef         Config;           /*!< MMC data config                      */
    HAL_LockTypeDef              Lock;             /*!< MMC locking object                   */
    uint32_t                     *pTxBuffPtr;      /*!< Pointer to MMC Tx transfer Buffer    */
    uint32_t                     *pTxPtr;          /*!< Pointer to MMC Tx transfer           */
    uint32_t                     TxXferSize;       /*!< MMC Tx Transfer size                 */
    uint32_t                     TxXferCount;      /*!< MMC Tx Transfer count                */
    uint32_t                     *pRxBuffPtr;      /*!< Pointer to MMC Rx transfer Buffer    */
    uint32_t                     *pRxPtr;          /*!< Pointer to MMC Rx transfer           */
    uint32_t                     RxXferSize;       /*!< MMC Rx Transfer size                 */
    uint32_t                     RxXferCount;      /*!< MMC Rx Transfer count                */
    __IO uint32_t                Context;          /*!< MMC transfer context                 */
    __IO HAL_MMC_StateTypeDef    State;            /*!< MMC card State                       */
    __IO uint32_t                ErrorCode;        /*!< MMC Card Error codes                 */
    __IO uint32_t                HardwareErrCode;  /*!< MMC Card Hardware Error codes        */
    SDIO_DMAInitTypedef          *dmaInit;         /*!< MMC DMA config parameters            */
    HAL_MMC_CardInfoTypeDef      MmcCard;          /*!< MMC Card information                 */
    uint32_t                     CSD[4U];          /*!< MMC card specific data table         */
    uint32_t                     CID[4U];          /*!< MMC card identification number table */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)
    void (* TxCpltCallback)                 (struct __MMC_HandleTypeDef *hmmc);
    void (* RxCpltCallback)                 (struct __MMC_HandleTypeDef *hmmc);
    void (* ErrorCallback)                  (struct __MMC_HandleTypeDef *hmmc);
    void (* AbortCpltCallback)              (struct __MMC_HandleTypeDef *hmmc);

    void (* MspInitCallback)                (struct __MMC_HandleTypeDef *hmmc);
    void (* MspDeInitCallback)              (struct __MMC_HandleTypeDef *hmmc);
#endif  
}MMC_HandleTypeDef;

/** @defgroup MMC_Exported_Types_Group4 Card Specific Data: CSD Register 
  * @{
  */
typedef struct
{
    __IO uint8_t  CSDStruct;            /*!< CSD structure                         */
    __IO uint8_t  SysSpecVersion;       /*!< System specification version          */
    __IO uint8_t  Reserved1;            /*!< Reserved                              */
    __IO uint8_t  TAAC;                 /*!< Data read access time 1               */
    __IO uint8_t  NSAC;                 /*!< Data read access time 2 in CLK cycles */
    __IO uint8_t  MaxBusClkFrec;        /*!< Max. bus clock frequency              */
    __IO uint16_t CardComdClasses;      /*!< Card command classes                  */
    __IO uint8_t  RdBlockLen;           /*!< Max. read data block length           */
    __IO uint8_t  PartBlockRead;        /*!< Partial blocks for read allowed       */
    __IO uint8_t  WrBlockMisalign;      /*!< Write block misalignment              */
    __IO uint8_t  RdBlockMisalign;      /*!< Read block misalignment               */
    __IO uint8_t  DSRImpl;              /*!< DSR implemented                       */
    __IO uint8_t  Reserved2;            /*!< Reserved                              */
    __IO uint32_t DeviceSize;           /*!< Device Size                           */
    __IO uint8_t  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min           */
    __IO uint8_t  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max           */
    __IO uint8_t  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min          */
    __IO uint8_t  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max          */
    __IO uint8_t  DeviceSizeMul;        /*!< Device size multiplier                */
    __IO uint8_t  EraseGrSize;          /*!< Erase group size                      */
    __IO uint8_t  EraseGrMul;           /*!< Erase group size multiplier           */
    __IO uint8_t  WrProtectGrSize;      /*!< Write protect group size              */
    __IO uint8_t  WrProtectGrEnable;    /*!< Write protect group enable            */
    __IO uint8_t  ManDeflECC;           /*!< Manufacturer default ECC              */
    __IO uint8_t  WrSpeedFact;          /*!< Write speed factor                    */
    __IO uint8_t  MaxWrBlockLen;        /*!< Max. write data block length          */
    __IO uint8_t  WriteBlockPaPartial;  /*!< Partial blocks for write allowed      */
    __IO uint8_t  Reserved3;            /*!< Reserved                              */
    __IO uint8_t  ContentProtectAppli;  /*!< Content protection application        */
    __IO uint8_t  FileFormatGrouop;     /*!< File format group                     */
    __IO uint8_t  CopyFlag;             /*!< Copy flag (OTP)                       */
    __IO uint8_t  PermWrProtect;        /*!< Permanent write protection            */
    __IO uint8_t  TempWrProtect;        /*!< Temporary write protection            */
    __IO uint8_t  FileFormat;           /*!< File format                           */
    __IO uint8_t  ECC;                  /*!< ECC code                              */
    __IO uint8_t  CSD_CRC;              /*!< CSD CRC                               */
    __IO uint8_t  Reserved4;            /*!< Always 1                              */
}HAL_MMC_CardCSDTypeDef;

/** @defgroup MMC_Exported_Types_Group5 Card Identification Data: CID Register
  * @{
  */
typedef struct
{
  __IO uint8_t  ManufacturerID;  /*!< Manufacturer ID       */
  __IO uint16_t OEM_AppliID;     /*!< OEM/Application ID    */
  __IO uint32_t ProdName1;       /*!< Product Name part1    */
  __IO uint8_t  ProdName2;       /*!< Product Name part2    */
  __IO uint8_t  ProdRev;         /*!< Product Revision      */
  __IO uint32_t ProdSN;          /*!< Product Serial Number */
  __IO uint8_t  Reserved1;       /*!< Reserved1             */
  __IO uint16_t ManufactDate;    /*!< Manufacturing Date    */
  __IO uint8_t  CID_CRC;         /*!< CID CRC               */
  __IO uint8_t  Reserved2;       /*!< Always 1              */

}HAL_MMC_CardCIDTypeDef;
/** 
  * @}
  */

/** @defgroup MMC_Exported_Types_Group6 MMC Card Status returned by ACMD13 
  * @{
  */
typedef struct
{
  __IO uint8_t  DataBusWidth;           /*!< Shows the currently defined data bus width                 */
  __IO uint8_t  SecuredMode;            /*!< Card is in secured mode of operation                       */
  __IO uint16_t CardType;               /*!< Carries information about card type                        */
  __IO uint32_t ProtectedAreaSize;      /*!< Carries information about the capacity of protected area   */
  __IO uint8_t  SpeedClass;             /*!< Carries information about the speed class of the card      */
  __IO uint8_t  PerformanceMove;        /*!< Carries information about the card's performance move      */
  __IO uint8_t  AllocationUnitSize;     /*!< Carries information about the card's allocation unit size  */
  __IO uint16_t EraseSize;              /*!< Determines the number of AUs to be erased in one operation */
  __IO uint8_t  EraseTimeout;           /*!< Determines the timeout for any number of AU erase          */
  __IO uint8_t  EraseOffset;            /*!< Carries information about the erase offset                 */

}HAL_MMC_CardStatusTypeDef;
/** 
  * @}
  */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)
/** @defgroup MMC_Exported_Types_Group7 MMC Callback ID enumeration definition
  * @{
  */
typedef enum
{
    HAL_MMC_TX_CPLT_CB_ID                 = 0x00U,  /*!< MMC Tx Complete Callback ID                     */
    HAL_MMC_RX_CPLT_CB_ID                 = 0x01U,  /*!< MMC Rx Complete Callback ID                     */
    HAL_MMC_ERROR_CB_ID                   = 0x02U,  /*!< MMC Error Callback ID                           */
    HAL_MMC_ABORT_CB_ID                   = 0x03U,  /*!< MMC Abort Callback ID                           */

    HAL_MMC_MSP_INIT_CB_ID                = 0x10U,  /*!< MMC MspInit Callback ID                         */
    HAL_MMC_MSP_DEINIT_CB_ID              = 0x11U   /*!< MMC MspDeInit Callback ID                       */
}HAL_MMC_CallbackIDTypeDef;

/** @defgroup MMC_Exported_Types_Group7 MMC Callback pointer definition
  * @{
  */
typedef void (*pMMC_CallbackTypeDef)           (MMC_HandleTypeDef *hmmc);

#endif

/* Exported constants --------------------------------------------------------*/
/** @defgroup MMC_Exported_Constants Exported Constants
  * @{
  */

#define BLOCKSIZE   512U        /*!< Block size is 512 bytes */
    
#define CAPACITY    0x400000U   /*!< Log Block Nuumber for 2 G bytes Cards */

/** @defgroup MMC_Exported_Constansts_Group1 MMC Error status enumeration Structure definition 
  * @{
  */
// hardware error
#define HAL_MMC_ERROR_HW_CMD_TIMEOUT             SDMMC_ERROR_HW_CMD_TIMEOUT    /*!< Command Timeout Error  */
#define HAL_MMC_ERROR_HW_CMD_CRC                 SDMMC_ERROR_HW_CMD_CRC        /*!< Command CRC Error      */
#define HAL_MMC_ERROR_HW_CMD_END_BIT             SDMMC_ERROR_HW_CMD_END_BIT    /*!< Command End Bit Error  */
#define HAL_MMC_ERROR_HW_CMD_Index               SDMMC_ERROR_HW_CMD_Index      /*!< Command Index Error    */
#define HAL_MMC_ERROR_HW_DATA_TIMEOUT            SDMMC_ERROR_HW_DATA_TIMEOUT   /*!< Data Timeout Error     */
#define HAL_MMC_ERROR_HW_DATA_CRC                SDMMC_ERROR_HW_DATA_CRC       /*!< Data CRC Error         */
#define HAL_MMC_ERROR_HW_DATA_END_BIT            SDMMC_ERROR_HW_DATA_END_BIT   /*!< Data End Bit Error     */
#define HAL_MMC_ERROR_HW_Current_Limit           SDMMC_ERROR_HW_Current_Limit  /*!< Current Limit Error    */
#define HAL_MMC_ERROR_HW_AUTO_CMD                SDMMC_ERROR_HW_AUTO_CMD       /*!< Auto CMD Error         */
#define HAL_MMC_ERROR_HW_ADMA                    SDMMC_ERROR_HW_ADMA           /*!< ADMA Error             */
#define HAL_MMC_ERROR_HW_TUNING                  SDMMC_ERROR_HW_TUNING         /*!< Tuning Error           */
#define HAL_MMC_ERROR_HW_OTHER_DMA_NOT_SUP       SDMMC_ERROR_HW_OTHER_DMA_NOT_SUP /*!< Other DMA Not Support      */
// soft error
#define HAL_MMC_ERROR_NONE                     SDMMC_ERROR_NONE                    /*!< No error                                                      */
#define HAL_MMC_ERROR_HARDWARE                 SDMMC_ERROR_HARDWARE                /*!< Hardware Error, See HardwareErrCode for details.              */
#define HAL_MMC_ERROR_DATA_CRC_FAIL            SDMMC_ERROR_DATA_CRC_FAIL           /*!< Data block sent/received (CRC check failed)                   */
#define HAL_MMC_ERROR_CMD_RSP_TIMEOUT          SDMMC_ERROR_CMD_RSP_TIMEOUT         /*!< Command response timeout                                      */
#define HAL_MMC_ERROR_DATA_TIMEOUT             SDMMC_ERROR_DATA_TIMEOUT            /*!< Data timeout                                                  */
#define HAL_MMC_ERROR_TX_UNDERRUN              SDMMC_ERROR_TX_UNDERRUN             /*!< Transmit FIFO underrun                                        */
#define HAL_MMC_ERROR_RX_OVERRUN               SDMMC_ERROR_RX_OVERRUN              /*!< Receive FIFO overrun                                          */
#define HAL_MMC_ERROR_ADDR_MISALIGNED          SDMMC_ERROR_ADDR_MISALIGNED         /*!< Misaligned address                                            */
#define HAL_MMC_ERROR_BLOCK_LEN_ERR            SDMMC_ERROR_BLOCK_LEN_ERR           /*!< Transferred block length is not allowed for the card or the 
                                                                                       number of transferred bytes does not match the block length   */
#define HAL_MMC_ERROR_ERASE_SEQ_ERR            SDMMC_ERROR_ERASE_SEQ_ERR           /*!< An error in the sequence of erase command occurs              */
#define HAL_MMC_ERROR_BAD_ERASE_PARAM          SDMMC_ERROR_BAD_ERASE_PARAM         /*!< An invalid selection for erase groups                         */
#define HAL_MMC_ERROR_WRITE_PROT_VIOLATION     SDMMC_ERROR_WRITE_PROT_VIOLATION    /*!< Attempt to program a write protect block                      */
#define HAL_MMC_ERROR_LOCK_UNLOCK_FAILED       SDMMC_ERROR_LOCK_UNLOCK_FAILED      /*!< Sequence or password error has been detected in unlock 
                                                                                       command or if there was an attempt to access a locked card    */
#define HAL_MMC_ERROR_COM_CRC_FAILED           SDMMC_ERROR_COM_CRC_FAILED          /*!< CRC check of the previous command failed                      */
#define HAL_MMC_ERROR_ILLEGAL_CMD              SDMMC_ERROR_ILLEGAL_CMD             /*!< Command is not legal for the card state                       */
#define HAL_MMC_ERROR_CARD_ECC_FAILED          SDMMC_ERROR_CARD_ECC_FAILED         /*!< Card internal ECC was applied but failed to correct the data  */
#define HAL_MMC_ERROR_CC_ERR                   SDMMC_ERROR_CC_ERR                  /*!< Internal card controller error                                */
#define HAL_MMC_ERROR_GENERAL_UNKNOWN_ERR      SDMMC_ERROR_GENERAL_UNKNOWN_ERR     /*!< General or unknown error                                      */
#define HAL_MMC_ERROR_STREAM_READ_UNDERRUN     SDMMC_ERROR_STREAM_READ_UNDERRUN    /*!< The card could not sustain data reading in stream rmode       */
#define HAL_MMC_ERROR_STREAM_WRITE_OVERRUN     SDMMC_ERROR_STREAM_WRITE_OVERRUN    /*!< The card could not sustain data programming in stream mode    */
#define HAL_MMC_ERROR_CID_CSD_OVERWRITE        SDMMC_ERROR_CID_CSD_OVERWRITE       /*!< CID/CSD overwrite error                                       */
#define HAL_MMC_ERROR_WP_ERASE_SKIP            SDMMC_ERROR_WP_ERASE_SKIP           /*!< Only partial address space was erased                         */
#define HAL_MMC_ERROR_CARD_ECC_DISABLED        SDMMC_ERROR_CARD_ECC_DISABLED       /*!< Command has been executed without using internal ECC          */
#define HAL_MMC_ERROR_ERASE_RESET              SDMMC_ERROR_ERASE_RESET             /*!< Erase sequence was cleared before executing because an out 
                                                                                       of erase sequence command was received                        */
#define HAL_MMC_ERROR_AKE_SEQ_ERR              SDMMC_ERROR_AKE_SEQ_ERR             /*!< Error in sequence of authentication                           */
#define HAL_MMC_ERROR_INVALID_VOLTRANGE        SDMMC_ERROR_INVALID_VOLTRANGE       /*!< Error in case of invalid voltage range                        */        
#define HAL_MMC_ERROR_ADDR_OUT_OF_RANGE        SDMMC_ERROR_ADDR_OUT_OF_RANGE       /*!< Error when addressed block is out of range                    */        
#define HAL_MMC_ERROR_REQUEST_NOT_APPLICABLE   SDMMC_ERROR_REQUEST_NOT_APPLICABLE  /*!< Error when command request is not applicable                  */  
#define HAL_MMC_ERROR_PARAM                    SDMMC_ERROR_INVALID_PARAMETER       /*!< the used parameter is not valid                               */  
#define HAL_MMC_ERROR_UNSUPPORTED_FEATURE      SDMMC_ERROR_UNSUPPORTED_FEATURE     /*!< Error when feature is not insupported                         */
#define HAL_MMC_ERROR_BUSY                     SDMMC_ERROR_BUSY                    /*!< Error when transfer process is busy                           */ 
#define HAL_MMC_ERROR_DMA                      SDMMC_ERROR_DMA                     /*!< Error while DMA transfer                                      */
#define HAL_MMC_ERROR_TIMEOUT                  SDMMC_ERROR_TIMEOUT                 /*!< Timeout error                                                 */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)
#define HAL_MMC_ERROR_INVALID_CALLBACK         SDMMC_ERROR_INVALID_PARAMETER       /*!< Invalid callback error                                        */
#endif
/** 
  * @}
  */
 
/** @defgroup MMC_Exported_Constansts_Group2 MMC context enumeration structure
  * @{
  */ 
#define   MMC_CONTEXT_NONE                 0x00000000U  /*!< None                             */
#define   MMC_CONTEXT_READ_SINGLE_BLOCK    0x00000001U  /*!< Read single block operation      */
#define   MMC_CONTEXT_READ_MULTIPLE_BLOCK  0x00000002U  /*!< Read multiple blocks operation   */
#define   MMC_CONTEXT_WRITE_SINGLE_BLOCK   0x00000010U  /*!< Write single block operation     */
#define   MMC_CONTEXT_WRITE_MULTIPLE_BLOCK 0x00000020U  /*!< Write multiple blocks operation  */
#define   MMC_CONTEXT_IT                   0x00000008U  /*!< Process in Interrupt mode        */
#define   MMC_CONTEXT_DMA                  0x00000080U  /*!< Process in DMA mode              */  

/**
  * @}
  */

/** @defgroup MMC_Exported_Constansts_Group3 MMC Voltage mode
  * @{
  */
/** 
  * @brief 
  */
#define MMC_HIGH_VOLTAGE_RANGE         0x80FF8000U  /*!< VALUE OF ARGUMENT            */
#define MMC_DUAL_VOLTAGE_RANGE         0x80FF8080U  /*!< VALUE OF ARGUMENT            */
#define eMMC_HIGH_VOLTAGE_RANGE        0xC0FF8000U  /*!< for eMMC > 2Gb sector mode   */
#define eMMC_DUAL_VOLTAGE_RANGE        0xC0FF8080U  /*!< for eMMC > 2Gb sector mode   */
#define MMC_INVALID_VOLTAGE_RANGE      0x0001FF01U 
/**
  * @}
  */

/** @defgroup MMC_Exported_Constansts_Group4 MMC Memory Cards
  * @{
  */
#define  MMC_HIGH_VOLTAGE_CARD     0x00000000U
#define  MMC_DUAL_VOLTAGE_CARD     0x00000001U
  
/* Exported macro ------------------------------------------------------------*/
/** @defgroup MMC_Exported_macros MMC Exported Macros
 *  @brief macros to handle interrupts and specific clock configurations
 * @{
 */
/** @brief Reset MMC handle state.
  * @param  __HANDLE__ : MMC handle.
  * @retval None
  */
#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)
#define __HAL_MMC_RESET_HANDLE_STATE(__HANDLE__)           do {                                              \
                                                               (__HANDLE__)->State = HAL_MMC_STATE_RESET; \
                                                               (__HANDLE__)->MspInitCallback = NULL;       \
                                                               (__HANDLE__)->MspDeInitCallback = NULL;     \
                                                             } while(0)
#else
#define __HAL_MMC_RESET_HANDLE_STATE(__HANDLE__)           ((__HANDLE__)->State = HAL_MMC_STATE_RESET)
#endif
 
/**
  * @brief  Enable the MMC device.
  * @retval None
  */ 
#define __HAL_MMC_ENABLE(__HANDLE__) __SDIO_ENABLE((__HANDLE__)->Instance)

/**
  * @brief  Disable the MMC device.
  * @retval None
  */
#define __HAL_MMC_DISABLE(__HANDLE__) __SDIO_DISABLE((__HANDLE__)->Instance)

/**
  * @brief  Enable the SDMMC DMA transfer.
  * @retval None
  */ 
#define __HAL_MMC_DMA_ENABLE(__HANDLE__) __SDIO_DMA_ENABLE((__HANDLE__)->Instance)

/**
  * @brief  Disable the SDMMC DMA transfer.
  * @retval None
  */
#define __HAL_MMC_DMA_DISABLE(__HANDLE__)           __SDIO_DMA_DISABLE((__HANDLE__)->Instance)
/**
  * @brief  Get the SDIO DMAx select.
  * @param  __HANDLE__ MMC Handle
  * @retval None
  */
#define __HAL_MMC_GET_DMA_SELECT(__HANDLE__)        __SDIO_GET_DMA_SELECT((__HANDLE__)->Instance)
/**
  * @brief  SDIO select DMAx.
  * @param  __HANDLE__ MMC Handle
  * @param  __DMAX__ DMAx
  * @retval None
  */
#define __HAL_MMC_DMA_SELECT(__HANDLE__, __DMAX__)  __SDIO_DMA_SELECT((__HANDLE__)->Instance, __DMAX__)
/**
  * @brief  Set the SDIO SDMA Addr.
  * @param  __HANDLE__ MMC Handle
  * @param  __ADDR__ SDMA System Address(Read/Write Buffer Address)
  * @retval None
  */
#define __HAL_MMC_SDMA_SET_ADDR(__HANDLE__, __ADDR__)  __SDIO_SDMA_SET_ADDR((__HANDLE__)->Instance, __ADDR__)
/**
  * @brief  Set the SDIO ADMA Addr.
  * @param  __HANDLE__ MMC Handle
  * @param  __ADDR__ ADMA System Address(Read/Write Buffer Address)
  * @retval None
  */
#define __HAL_MMC_ADMA_SET_ADDR(__HANDLE__, __ADDR__)  __SDIO_ADMA_SET_ADDR((__HANDLE__)->Instance, __ADDR__)
/**
  * @brief  Set SDIO Host DMA Buffer Boundary
  * @param  __HANDLE__ MMC Handle
  * @param  __BOUNDARY__ Host DMA Buffer Boundary
  * @retval None
  */
#define __HAL_MMC_SDMA_SET_BOUNDARY(__HANDLE__, __BOUNDARY__)  __SDIO_SDMA_SET_BOUNDARY((__HANDLE__)->Instance, __BOUNDARY__)
/**
  * @brief  Set SDIO Host SDMA Burst Length
  * @param  __HANDLE__ MMC Handle
  * @param  __BURST_LENGTH__ Host DMA Burst Length
  * @retval None
  */
#define __HAL_MMC_DMA_SET_BURST_LENGTH(__HANDLE__, __BURST_LENGTH__)  __SDIO_DMA_SET_BURST_LENGTH((__HANDLE__)->Instance, __BURST_LENGTH__)
/**
  * @brief  Continue Request
  * @param  __HANDLE__ MMC Handle
  * @retval None
  */
#define __HAL_MMC_TRANSFER_CONTINUE_REQUEST(__HANDLE__)  __SDIO_TRANSFER_CONTINUE_REQUEST((__HANDLE__)->Instance)
/**
  * @brief  DMA Continue Request
  * @param  __HANDLE__ MMC Handle
  * @retval None
  */
#define __HAL_MMC_DMA_CONTINUE_REQUEST(__HANDLE__)  __SDIO_DMA_CONTINUE_REQUEST((__HANDLE__)->Instance)
/**
  * @brief  Set the SDIO Blocks Count For Current Transfer(SRS1.BCCT).
  * @param  __HANDLE__ MMC Handle
  * @param  __COUNT__ specifies the interrupt pending bit to clear.
  *          This parameter can be one or a combination of the following values:0~0xFFFF
  * @retval None
  */
#define __HAL_MMC_Set_BLOCK_COUNT(__HANDLE__, __COUNT__)  __SDIO_SET_BLOCK_COUNT((__HANDLE__)->Instance, __COUNT__)
/**
  * @brief  Get the SDIO Blocks Count For Current Transfer(SRS1.BCCT).
  * @param  __HANDLE__ MMC Handle
  * @retval Blocks Count For Current Transfer
  */
#define __HAL_MMC_Get_BLOCK_COUNT(__HANDLE__)  __SDIO_Get_BLOCK_COUNT((__HANDLE__)->Instance)

#define MMC_STATE_CICMD        SDIO_STATE_CICMD
#define MMC_STATE_CIDAT        SDIO_STATE_CIDAT
#define MMC_STATE_DATLA        SDIO_STATE_DATLA
#define MMC_STATE_RTNGR        SDIO_STATE_RTNGR
#define MMC_STATE_WTA          SDIO_STATE_WTA
#define MMC_STATE_RTA          SDIO_STATE_RTA
#define MMC_STATE_BWE          SDIO_STATE_BWE
#define MMC_STATE_BRE          SDIO_STATE_BRE
#define MMC_STATE_CI           SDIO_STATE_CI
#define MMC_STATE_CSS          SDIO_STATE_CSS
#define MMC_STATE_CDL          SDIO_STATE_CDL
#define MMC_STATE_WPSL         SDIO_STATE_WPSL
#define MMC_STATE_DATSL0       SDIO_STATE_DATSL0
#define MMC_STATE_DATSL1       SDIO_STATE_DATSL1
#define MMC_STATE_DATSL2       SDIO_STATE_DATSL2
#define MMC_STATE_DATSL3       SDIO_STATE_DATSL3
#define MMC_STATE_CMDSL        SDIO_STATE_CMDSL

/**
  * @brief  Checks whether the specified SDIO state is set or not.
  * @param  __HANDLE__ MMC Handle
  * @param  __STATE__ specifies the state to check.
  *          This parameter can be one of the following values:
  *            @arg MMC_STATE_CICMD:         Command Inhibit CMD.
  *            @arg MMC_STATE_CIDAT:         Command Inhibit DAT.
  *            @arg MMC_STATE_DATLA:         DAT Line Active.
  *            @arg MMC_STATE_RTNGR:         Re-Tuning Request
  *            @arg MMC_STATE_WTA:           Write Transfer Active
  *            @arg MMC_STATE_RTA:           Read Transfer Active
  *            @arg MMC_STATE_BWE:           Buffer Write Enable
  *            @arg MMC_STATE_BRE:           Buffer Read Enable
  *            @arg MMC_STATE_CI:            Card Inserted
  *            @arg MMC_STATE_CSS:           Card State Stable
  *            @arg MMC_STATE_CDL:           Card Detect Pin Level
  *            @arg MMC_STATE_WPSL:          Write Protect Switch Pin Level
  *            @arg MMC_STATE_DATSL0:        DAT[0] Line Signal Level
  *            @arg MMC_STATE_DATSL1:        DAT[1] Line Signal Level
  *            @arg MMC_STATE_DATSL2:        DAT[2] Line Signal Level
  *            @arg MMC_STATE_DATSL3:        DAT[3] Line Signal Level
  *            @arg MMC_STATE_CMDSL:         CMD Line Signal Level
  * @retval The new state of SDIO_FLAG (SET or RESET).
  */
#define __HAL_MMC_GET_STATE(__HANDLE__, __STATE__)  __SDIO_GET_STATE((__HANDLE__)->Instance, __STATE__)

#define MMC_IT_CC          SDIO_IT_CC
#define MMC_IT_TC          SDIO_IT_TC
#define MMC_IT_BGE         SDIO_IT_BGE
#define MMC_IT_DMAINT      SDIO_IT_DMAINT
#define MMC_IT_BWR         SDIO_IT_BWR
#define MMC_IT_BRR         SDIO_IT_BRR
#define MMC_IT_CIN         SDIO_IT_CIN
#define MMC_IT_CR          SDIO_IT_CR
#define MMC_IT_CINT        SDIO_IT_CINT
#define MMC_IT_INT_A       SDIO_IT_INT_A
#define MMC_IT_INT_B       SDIO_IT_INT_B
#define MMC_IT_INT_C       SDIO_IT_INT_C
#define MMC_IT_RTNGE       SDIO_IT_RTNGE
#define MMC_IT_EINT        SDIO_IT_EINT
#define MMC_IT_ECT         SDIO_IT_ECT
#define MMC_IT_ECCRC       SDIO_IT_ECCRC
#define MMC_IT_ECEB        SDIO_IT_ECEB
#define MMC_IT_ECI         SDIO_IT_ECI
#define MMC_IT_EDT         SDIO_IT_EDT
#define MMC_IT_EDCRC       SDIO_IT_EDCRC
#define MMC_IT_EDEB        SDIO_IT_EDEB
#define MMC_IT_ECL         SDIO_IT_ECL
#define MMC_IT_EAC         SDIO_IT_EAC
#define MMC_IT_EADMA       SDIO_IT_EADMA
#define MMC_IT_ETNG        SDIO_IT_ETNG

#define MMC_STATIC_FLAGS              SDIO_STATIC_FLAGS
#define MMC_IT_FLAGS_OTHER_ERRORS     SDIO_IT_STATE_OTHER_ERRORS
#define MMC_IT_STATE_OTHER_ERRORS     SDIO_IT_STATE_OTHER_ERRORS
#define MMC_IT_STATE_ERROR            SDIO_IT_STATE_ERROR

#define MMC_IT_OTHER_ERRORS           SDIO_IT_STATE_OTHER_ERRORS
#define MMC_IT_ERROR                  SDIO_IT_STATE_ERROR

/**
  * @brief  Enable the MMC device interrupt.
  * @param  __HANDLE__ MMC Handle  
  * @param  __INTERRUPT__ specifies the SDMMC interrupt sources to be enabled.
  *         This parameter can be one or a combination of the following values:
  *            @arg MMC_IT_CC            Command Complete
  *            @arg MMC_IT_TC            Transfer Complete
  *            @arg MMC_IT_BGE           Block Gap Event
  *            @arg MMC_IT_DMAINT:       DMA Interrupt
  *            @arg MMC_IT_BWR           Buffer Write Ready
  *            @arg MMC_IT_BRR           Buffer Read Ready
  *            @arg MMC_IT_CIN           Card Insertion
  *            @arg MMC_IT_CR            Card Removal
  *            @arg MMC_IT_CINT          Card Interrupt
  *            @arg MMC_IT_INT_A         Interrupt on line A
  *            @arg MMC_IT_INT_B         Interrupt on line B
  *            @arg MMC_IT_INT_C         Interrupt on line C
  *            @arg MMC_IT_RTNGE         Re-Tuning Event
  *            @arg MMC_IT_ECT           Command Timeout Error
  *            @arg MMC_IT_ECCRC         Command CRC Error
  *            @arg MMC_IT_ECEB          Command End Bit Error
  *            @arg MMC_IT_ECI           Command Index Error
  *            @arg MMC_IT_EDT           Data Timeout Error
  *            @arg MMC_IT_EDCRC         Data CRC Error
  *            @arg MMC_IT_EDEB          Data End Bit Error
  *            @arg MMC_IT_ECL           Current Limit Error
  *            @arg MMC_IT_EAC           Auto CMD Error
  *            @arg MMC_IT_EADMA         ADMA Error
  *            @arg MMC_IT_ETNG          Tuning Error
  * @retval None
  */
#define __HAL_MMC_ENABLE_IT(__HANDLE__, __INTERRUPT__) __SDIO_ENABLE_IT((__HANDLE__)->Instance, (__INTERRUPT__))

/**
  * @brief  Disable the MMC device interrupt.
  * @param  __HANDLE__ MMC Handle   
  * @param  __INTERRUPT__ specifies the SDMMC interrupt sources to be disabled.
  *          This parameter can be one or a combination of the following values:
  *            @arg MMC_IT_CC            Command Complete
  *            @arg MMC_IT_TC            Transfer Complete
  *            @arg MMC_IT_BGE           Block Gap Event
  *            @arg MMC_IT_DMAINT:       DMA Interrupt
  *            @arg MMC_IT_BWR           Buffer Write Ready
  *            @arg MMC_IT_BRR           Buffer Read Ready
  *            @arg MMC_IT_CIN           Card Insertion
  *            @arg MMC_IT_CR            Card Removal
  *            @arg MMC_IT_CINT          Card Interrupt
  *            @arg MMC_IT_INT_A         Interrupt on line A
  *            @arg MMC_IT_INT_B         Interrupt on line B
  *            @arg MMC_IT_INT_C         Interrupt on line C
  *            @arg MMC_IT_RTNGE         Re-Tuning Event
  *            @arg MMC_IT_ECT           Command Timeout Error
  *            @arg MMC_IT_ECCRC         Command CRC Error
  *            @arg MMC_IT_ECEB          Command End Bit Error
  *            @arg MMC_IT_ECI           Command Index Error
  *            @arg MMC_IT_EDT           Data Timeout Error
  *            @arg MMC_IT_EDCRC         Data CRC Error
  *            @arg MMC_IT_EDEB          Data End Bit Error
  *            @arg MMC_IT_ECL           Current Limit Error
  *            @arg MMC_IT_EAC           Auto CMD Error
  *            @arg MMC_IT_EADMA         ADMA Error
  *            @arg MMC_IT_ETNG          Tuning Error
  * @retval None
  */
#define __HAL_MMC_DISABLE_IT(__HANDLE__, __INTERRUPT__) __SDIO_DISABLE_IT((__HANDLE__)->Instance, (__INTERRUPT__))

#define MMC_FLAG_CC       SDIO_FLAG_CC         //Command Complete
#define MMC_FLAG_TC       SDIO_FLAG_TC         //Transfer Complete
#define MMC_FLAG_BGE      SDIO_FLAG_BGE        //Block Gap Event
#define MMC_FLAG_DMAINT   SDIO_FLAG_DMAINT     //DMA Interrupt
#define MMC_FLAG_BWR      SDIO_FLAG_BWR        //Buffer Write Ready
#define MMC_FLAG_BRR      SDIO_FLAG_BRR        //Buffer Read Ready
#define MMC_FLAG_CIN      SDIO_FLAG_CIN        //Card Insertion
#define MMC_FLAG_CR       SDIO_FLAG_CR         //Card Removal
#define MMC_FLAG_CINT     SDIO_FLAG_CINT       //Card Interrupt
#define MMC_FLAG_INT_A    SDIO_FLAG_INT_A      //Interrupt on line A
#define MMC_FLAG_INT_B    SDIO_FLAG_INT_B      //Interrupt on line B
#define MMC_FLAG_INT_C    SDIO_FLAG_INT_C      //Interrupt on line C
#define MMC_FLAG_RTNGE    SDIO_FLAG_RTNGE      //Re-Tuning Event
#define MMC_FLAG_EINT     SDIO_FLAG_EINT       //Error Interrupt
#define MMC_FLAG_ECT      SDIO_FLAG_ECT        //Command Timeout Error
#define MMC_FLAG_ECCRC    SDIO_FLAG_ECCRC      //Command CRC Error
#define MMC_FLAG_ECEB     SDIO_FLAG_ECEB       //Command End Bit Error
#define MMC_FLAG_ECI      SDIO_FLAG_ECI        //Command Index Error
#define MMC_FLAG_EDT      SDIO_FLAG_EDT        //Data Timeout Error
#define MMC_FLAG_EDCRC    SDIO_FLAG_EDCRC      //Data CRC Error
#define MMC_FLAG_EDEB     SDIO_FLAG_EDEB       //Data End Bit Error
#define MMC_FLAG_ECL      SDIO_FLAG_ECL        //Current Limit Error
#define MMC_FLAG_EAC      SDIO_FLAG_EAC        //Auto CMD Error
#define MMC_FLAG_EADMA    SDIO_FLAG_EADMA      //ADMA Error
#define MMC_FLAG_ETNG     SDIO_FLAG_ETNG       //Tuning Error
/**
  * @brief  Check whether the specified MMC flag is set or not. 
  * @param  __HANDLE__ MMC Handle   
  * @param  __FLAG__ specifies the flag to check. 
  *          This parameter can be one of the following values:
  *            @arg MMC_FLAG_CC:           Command Complete
  *            @arg MMC_FLAG_TC :          Transfer Complete
  *            @arg MMC_FLAG_BGE:          Block Gap Event
  *            @arg MMC_FLAG_DMAINT:       DMA Interrupt
  *            @arg MMC_FLAG_BWR:          Buffer Write Ready
  *            @arg MMC_FLAG_BRR:          Buffer Read Ready
  *            @arg MMC_FLAG_CIN:          Card Insertion
  *            @arg MMC_FLAG_CR:           Card Removal
  *            @arg MMC_FLAG_CINT:         Card Interrupt
  *            @arg MMC_FLAG_INT_A:        Interrupt on line A
  *            @arg MMC_FLAG_INT_B:        Interrupt on line B
  *            @arg MMC_FLAG_INT_C:        Interrupt on line C
  *            @arg MMC_FLAG_RTNGE:        Re-Tuning Event
  *            @arg MMC_FLAG_EINT          Error Interrupt
  *            @arg MMC_FLAG_ECT:          Command Timeout Error
  *            @arg MMC_FLAG_ECCRC:        Command CRC Error
  *            @arg MMC_FLAG_ECEB:         Command End Bit Error
  *            @arg MMC_FLAG_ECI:          Command Index Error
  *            @arg MMC_FLAG_EDT:          Data Timeout Error
  *            @arg MMC_FLAG_EDCRC:        Data CRC Error
  *            @arg MMC_FLAG_EDEB:         Data End Bit Error
  *            @arg MMC_FLAG_ECL:          Current Limit Error
  *            @arg MMC_FLAG_EAC:          Auto CMD Error
  *            @arg MMC_FLAG_EADMA:        ADMA Error
  *            @arg MMC_FLAG_ETNG:         Tuning Error
  * @retval The new state of MMC FLAG (SET or RESET).
  */
#define __HAL_MMC_GET_FLAG(__HANDLE__, __FLAG__) __SDIO_GET_FLAG((__HANDLE__)->Instance, (__FLAG__))

/**
  * @brief  Clear the MMC's pending flags.
  * @param  __HANDLE__ MMC Handle  
  * @param  __FLAG__ specifies the flag to clear.  
  *          This parameter can be one or a combination of the following values:
  *            @arg MMC_FLAG_CC:           Command Complete
  *            @arg MMC_FLAG_TC :          Transfer Complete
  *            @arg MMC_FLAG_BGE:          Block Gap Event
  *            @arg MMC_FLAG_DMAINT:       DMA Interrupt
  *            @arg MMC_FLAG_BWR:          Buffer Write Ready
  *            @arg MMC_FLAG_BRR:          Buffer Read Ready
  *            @arg MMC_FLAG_CIN:          Card Insertion
  *            @arg MMC_FLAG_CR:           Card Removal
  *            @arg MMC_FLAG_ECT:          Command Timeout Error
  *            @arg MMC_FLAG_ECCRC:        Command CRC Error
  *            @arg MMC_FLAG_ECEB:         Command End Bit Error
  *            @arg MMC_FLAG_ECI:          Command Index Error
  *            @arg MMC_FLAG_EDT:          Data Timeout Error
  *            @arg MMC_FLAG_EDCRC:        Data CRC Error
  *            @arg MMC_FLAG_EDEB:         Data End Bit Error
  *            @arg MMC_FLAG_ECL:          Current Limit Error
  *            @arg MMC_FLAG_EAC:          Auto CMD Error
  *            @arg MMC_FLAG_EADMA:        ADMA Error
  *            @arg MMC_FLAG_ETNG:         Tuning Error
  * @retval None
  */
#define __HAL_MMC_CLEAR_FLAG(__HANDLE__, __FLAG__) __SDIO_CLEAR_FLAG((__HANDLE__)->Instance, (__FLAG__))

/**
  * @brief  Check whether the specified MMC interrupt has occurred or not.
  * @param  __HANDLE__ MMC Handle   
  * @param  __INTERRUPT__ specifies the SDMMC interrupt source to check. 
  *          This parameter can be one of the following values:
  *            @arg MMC_IT_CC:           Command Complete
  *            @arg MMC_IT_TC :          Transfer Complete
  *            @arg MMC_IT_BGE:          Block Gap Event
  *            @arg MMC_IT_DMAINT:       DMA Interrupt
  *            @arg MMC_IT_BWR:          Buffer Write Ready
  *            @arg MMC_IT_BRR:          Buffer Read Ready
  *            @arg MMC_IT_CIN:          Card Insertion
  *            @arg MMC_IT_CR:           Card Removal
  *            @arg MMC_IT_ECT:          Command Timeout Error
  *            @arg MMC_IT_ECCRC:        Command CRC Error
  *            @arg MMC_IT_ECEB:         Command End Bit Error
  *            @arg MMC_IT_ECI:          Command Index Error
  *            @arg MMC_IT_EDT:          Data Timeout Error
  *            @arg MMC_IT_EDCRC:        Data CRC Error
  *            @arg MMC_IT_EDEB:         Data End Bit Error
  *            @arg MMC_IT_ECL:          Current Limit Error
  *            @arg MMC_IT_EAC:          Auto CMD Error
  *            @arg MMC_IT_EADMA:        ADMA Error
  *            @arg MMC_IT_ETNG:         Tuning Error
  * @retval The new state of MMC IT (SET or RESET).
  */
#define __HAL_MMC_GET_IT(__HANDLE__, __INTERRUPT__)     __SDIO_GET_IT((__HANDLE__)->Instance, (__INTERRUPT__))

/**
  * @brief  Clear the MMC's interrupt pending bits.
  * @param  __HANDLE__ MMC Handle
  * @param  __INTERRUPT__ specifies the interrupt pending bit to clear. 
  *          This parameter can be one or a combination of the following values:
  *            @arg MMC_IT_CC:           Command Complete
  *            @arg MMC_IT_TC :          Transfer Complete
  *            @arg MMC_IT_BGE:          Block Gap Event
  *            @arg MMC_IT_DMAINT:       DMA Interrupt
  *            @arg MMC_IT_BWR:          Buffer Write Ready
  *            @arg MMC_IT_BRR:          Buffer Read Ready
  *            @arg MMC_IT_CIN:          Card Insertion
  *            @arg MMC_IT_CR:           Card Removal
  *            @arg MMC_IT_ECT:          Command Timeout Error
  *            @arg MMC_IT_ECCRC:        Command CRC Error
  *            @arg MMC_IT_ECEB:         Command End Bit Error
  *            @arg MMC_IT_ECI:          Command Index Error
  *            @arg MMC_IT_EDT:          Data Timeout Error
  *            @arg MMC_IT_EDCRC:        Data CRC Error
  *            @arg MMC_IT_EDEB:         Data End Bit Error
  *            @arg MMC_IT_ECL:          Current Limit Error
  *            @arg MMC_IT_EAC:          Auto CMD Error
  *            @arg MMC_IT_EADMA:        ADMA Error
  *            @arg MMC_IT_ETNG:         Tuning Error
  * @retval None
  */
#define __HAL_MMC_CLEAR_IT(__HANDLE__, __INTERRUPT__)       __SDIO_CLEAR_IT((__HANDLE__)->Instance, (__INTERRUPT__))

#define  MMC_IT_STATE_CC              SDIO_IT_STATE_CC      //Command Complete
#define  MMC_IT_STATE_TC              SDIO_IT_STATE_TC      //Transfer Complete
#define  MMC_IT_STATE_BGE             SDIO_IT_STATE_BGE     //Block Gap Event
#define  MMC_IT_STATE_DMAINT          SDIO_IT_STATE_DMAINT  //DMA Interrupt
#define  MMC_IT_STATE_BWR             SDIO_IT_STATE_BWR     //Buffer Write Ready
#define  MMC_IT_STATE_BRR             SDIO_IT_STATE_BRR     //Buffer Read Ready
#define  MMC_IT_STATE_CIN             SDIO_IT_STATE_CIN     //Card Insertion
#define  MMC_IT_STATE_CR              SDIO_IT_STATE_CR      //Card Removal
#define  MMC_IT_STATE_CINT            SDIO_IT_STATE_CINT    //Card Interrupt
#define  MMC_IT_STATE_INT_A           SDIO_IT_STATE_INT_A   //Interrupt on line A
#define  MMC_IT_STATE_INT_B           SDIO_IT_STATE_INT_B   //Interrupt on line B
#define  MMC_IT_STATE_INT_C           SDIO_IT_STATE_INT_C   //Interrupt on line C
#define  MMC_IT_STATE_RTNGE           SDIO_IT_STATE_RTNGE   //Re-Tuning Event
#define  MMC_IT_STATE_EINT            SDIO_IT_STATE_EINT    //Error Interrupt
#define  MMC_IT_STATE_ECT             SDIO_IT_STATE_ECT     //Command Timeout Error
#define  MMC_IT_STATE_ECCRC           SDIO_IT_STATE_ECCRC   //Command CRC Error
#define  MMC_IT_STATE_ECEB            SDIO_IT_STATE_ECEB    //Command End Bit Error
#define  MMC_IT_STATE_ECI             SDIO_IT_STATE_ECI     //Command Index Error
#define  MMC_IT_STATE_EDT             SDIO_IT_STATE_EDT     //Data Timeout Error
#define  MMC_IT_STATE_EDCRC           SDIO_IT_STATE_EDCRC   //Data CRC Error
#define  MMC_IT_STATE_EDEB            SDIO_IT_STATE_EDEB    //Data End Bit Error
#define  MMC_IT_STATE_ECL             SDIO_IT_STATE_ECL     //Current Limit Error
#define  MMC_IT_STATE_EAC             SDIO_IT_STATE_EAC     //Auto CMD Error
#define  MMC_IT_STATE_EADMA           SDIO_IT_STATE_EADMA   //ADMA Error
#define  MMC_IT_STATE_ETNG            SDIO_IT_STATE_ETNG    //Tuning Error
/**
  * @brief  Get the SDIO device interrupt Whether or not enable.
  * @param  __HANDLE__ MMC Handle
  * @param  __INTERRUPT__  specifies the SDIO interrupt sources to be getted.
  *          This parameter can be one or a combination of the following values:
  *            @arg MMC_IT_CC:           Command Complete
  *            @arg MMC_IT_TC :          Transfer Complete
  *            @arg MMC_IT_BGE:          Block Gap Event
  *            @arg MMC_IT_DMAINT:       DMA Interrupt
  *            @arg MMC_IT_BWR:          Buffer Write Ready
  *            @arg MMC_IT_BRR:          Buffer Read Ready
  *            @arg MMC_IT_CIN:          Card Insertion
  *            @arg MMC_IT_CR:           Card Removal
  *            @arg MMC_IT_CINT:         Card Interrupt
  *            @arg MMC_IT_INT_A:        Interrupt on line A
  *            @arg MMC_IT_INT_B:        Interrupt on line B
  *            @arg MMC_IT_INT_C:        Interrupt on line C
  *            @arg MMC_IT_RTNGE:        Re-Tuning Event
  *            @arg MMC_IT_ECT:          Command Timeout Error
  *            @arg MMC_IT_ECCRC:        Command CRC Error
  *            @arg MMC_IT_ECEB:         Command End Bit Error
  *            @arg MMC_IT_ECI:          Command Index Error
  *            @arg MMC_IT_EDT:          Data Timeout Error
  *            @arg MMC_IT_EDCRC:        Data CRC Error
  *            @arg MMC_IT_EDEB:         Data End Bit Error
  *            @arg MMC_IT_ECL:          Current Limit Error
  *            @arg MMC_IT_EAC:          Auto CMD Error
  *            @arg MMC_IT_EADMA:        ADMA Error
  *            @arg MMC_IT_ETNG:         Tuning Error
  * @retval None
  */
#define __HAL_MMC_GET_IT_EN(__HANDLE__, __INTERRUPT__)      __SDIO_GET_IT_EN((__HANDLE__)->Instance, __INTERRUPT__)
/**
  * @brief  Get the SDIO device interrupt state Whether or not enable.
  * @param  __HANDLE__ MMC Handle
  * @param  __STATE__  specifies the SDIO interrupt state to be getted.
  *          This parameter can be one or a combination of the following values:
  *            @arg MMC_IT_STATE_CC:           Command Complete
  *            @arg MMC_IT_STATE_TC :          Transfer Complete
  *            @arg MMC_IT_STATE_BGE:          Block Gap Event
  *            @arg MMC_IT_STATE_DMAINT:       DMA Interrupt
  *            @arg MMC_IT_STATE_BWR:          Buffer Write Ready
  *            @arg MMC_IT_STATE_BRR:          Buffer Read Ready
  *            @arg MMC_IT_STATE_CIN:          Card Insertion
  *            @arg MMC_IT_STATE_CR:           Card Removal
  *            @arg MMC_IT_STATE_CINT:         Card Interrupt
  *            @arg MMC_IT_STATE_INT_A:        Interrupt on line A
  *            @arg MMC_IT_STATE_INT_B:        Interrupt on line B
  *            @arg MMC_IT_STATE_INT_C:        Interrupt on line C
  *            @arg MMC_IT_STATE_RTNGE:        Re-Tuning Event
  *            @arg MMC_IT_STATE_ECT:          Command Timeout Error
  *            @arg MMC_IT_STATE_ECCRC:        Command CRC Error
  *            @arg MMC_IT_STATE_ECEB:         Command End Bit Error
  *            @arg MMC_IT_STATE_ECI:          Command Index Error
  *            @arg MMC_IT_STATE_EDT:          Data Timeout Error
  *            @arg MMC_IT_STATE_EDCRC:        Data CRC Error
  *            @arg MMC_IT_STATE_EDEB:         Data End Bit Error
  *            @arg MMC_IT_STATE_ECL:          Current Limit Error
  *            @arg MMC_IT_STATE_EAC:          Auto CMD Error
  *            @arg MMC_IT_STATE_EADMA:        ADMA Error
  *            @arg MMC_IT_STATE_ETNG:         Tuning Error
  * @retval None
  */
#define __HAL_MMC_GET_IT_STATE_EN(__HANDLE__, __STATE__)    __SDIO_GET_IT_STATE_EN((__HANDLE__)->Instance, __STATE__)
/**
  * @brief  Enable the SDIO device interrupt state Whether or not enable.
  * @param  __HANDLE__ MMC Handle
  * @param  __STATE__  specifies the SDIO interrupt state to be Enabled.
  *          This parameter can be one or a combination of the following values:
  *            @arg SDIO_IT_STATE_CC:           Command Complete
  *            @arg SDIO_IT_STATE_TC :          Transfer Complete
  *            @arg SDIO_IT_STATE_BGE:          Block Gap Event
  *            @arg SDIO_IT_STATE_DMAINT:       DMA Interrupt
  *            @arg SDIO_IT_STATE_BWR:          Buffer Write Ready
  *            @arg SDIO_IT_STATE_BRR:          Buffer Read Ready
  *            @arg SDIO_IT_STATE_CIN:          Card Insertion
  *            @arg SDIO_IT_STATE_CR:           Card Removal
  *            @arg SDIO_IT_STATE_CINT:         Card Interrupt
  *            @arg SDIO_IT_STATE_INT_A:        Interrupt on line A
  *            @arg SDIO_IT_STATE_INT_B:        Interrupt on line B
  *            @arg SDIO_IT_STATE_INT_C:        Interrupt on line C
  *            @arg SDIO_IT_STATE_RTNGE:        Re-Tuning Event
  *            @arg SDIO_IT_STATE_ECT:          Command Timeout Error
  *            @arg SDIO_IT_STATE_ECCRC:        Command CRC Error
  *            @arg SDIO_IT_STATE_ECEB:         Command End Bit Error
  *            @arg SDIO_IT_STATE_ECI:          Command Index Error
  *            @arg SDIO_IT_STATE_EDT:          Data Timeout Error
  *            @arg SDIO_IT_STATE_EDCRC:        Data CRC Error
  *            @arg SDIO_IT_STATE_EDEB:         Data End Bit Error
  *            @arg SDIO_IT_STATE_ECL:          Current Limit Error
  *            @arg SDIO_IT_STATE_EAC:          Auto CMD Error
  *            @arg SDIO_IT_STATE_EADMA:        ADMA Error
  *            @arg SDIO_IT_STATE_ETNG:         Tuning Error
  * @retval None
  */
#define __HAL_MMC_IT_STATE_ENABLE(__HANDLE__, __STATE__)     __SDIO_IT_STATE_ENABLE((__HANDLE__)->Instance, __STATE__)
/**
  * @brief  Disable the SDIO device interrupt state Whether or not enable.
  * @param  __HANDLE__ MMC Handle
  * @param  __STATE__  specifies the SDIO interrupt state to be Disable.
  *          This parameter can be one or a combination of the following values:
  *            @arg SDIO_IT_STATE_CC:           Command Complete
  *            @arg SDIO_IT_STATE_TC :          Transfer Complete
  *            @arg SDIO_IT_STATE_BGE:          Block Gap Event
  *            @arg SDIO_IT_STATE_DMAINT:       DMA Interrupt
  *            @arg SDIO_IT_STATE_BWR:          Buffer Write Ready
  *            @arg SDIO_IT_STATE_BRR:          Buffer Read Ready
  *            @arg SDIO_IT_STATE_CIN:          Card Insertion
  *            @arg SDIO_IT_STATE_CR:           Card Removal
  *            @arg SDIO_IT_STATE_CINT:         Card Interrupt
  *            @arg SDIO_IT_STATE_INT_A:        Interrupt on line A
  *            @arg SDIO_IT_STATE_INT_B:        Interrupt on line B
  *            @arg SDIO_IT_STATE_INT_C:        Interrupt on line C
  *            @arg SDIO_IT_STATE_RTNGE:        Re-Tuning Event
  *            @arg SDIO_IT_STATE_ECT:          Command Timeout Error
  *            @arg SDIO_IT_STATE_ECCRC:        Command CRC Error
  *            @arg SDIO_IT_STATE_ECEB:         Command End Bit Error
  *            @arg SDIO_IT_STATE_ECI:          Command Index Error
  *            @arg SDIO_IT_STATE_EDT:          Data Timeout Error
  *            @arg SDIO_IT_STATE_EDCRC:        Data CRC Error
  *            @arg SDIO_IT_STATE_EDEB:         Data End Bit Error
  *            @arg SDIO_IT_STATE_ECL:          Current Limit Error
  *            @arg SDIO_IT_STATE_EAC:          Auto CMD Error
  *            @arg SDIO_IT_STATE_EADMA:        ADMA Error
  *            @arg SDIO_IT_STATE_ETNG:         Tuning Error
  * @retval None
  */
#define __HAL_MMC_IT_STATE_DISABLE(__HANDLE__, __STATE__)    __SDIO_IT_STATE_DISABLE((__HANDLE__)->Instance, __STATE__)


/**
  * @brief  Enable Read Wait.
  * @param  __HANDLE__ MMC Handle
  * @retval None
  */
#define __HAL_MMC_READ_WAIT_ENABLE(__HANDLE__)   __SDIO_READ_WAIT_ENABLE((__HANDLE__)->Instance)
/**
  * @brief  Disable Read Wait.
  * @param  __HANDLE__ MMC Handle
  * @retval None
  */
#define __HAL_MMC_READ_WAIT_DISABLE(__HANDLE__)  __SDIO_READ_WAIT_DISABLE((__HANDLE__)->Instance)

/**
  * @brief  Reset Data Line.
  * @param  __HANDLE__ MMC Handle
  * @retval None
  */
#define __HAL_MMC_SOFT_RESET_DAT(__HANDLE__)  __SDIO_SOFT_RESET_DAT((__HANDLE__)->Instance)
/**
  * @brief  Reset CMD Line.
  * @param  __HANDLE__ MMC Handle
  * @retval None
  */
#define __HAL_MMC_SOFT_RESET_CMD(__HANDLE__)  __SDIO_SOFT_RESET_CMD((__HANDLE__)->Instance)
/**
  * @brief  Reset All (CMD and Data Line).
  * @param  __HANDLE__ MMC Handle
  * @retval None
  */
#define __HAL_MMC_SOFT_RESET_ALL(__HANDLE__)  __SDIO_SOFT_RESET_ALL((__HANDLE__)->Instance)

/**
  * @}
  */
  
/* Exported functions --------------------------------------------------------*/
/** @defgroup MMC_Exported_Functions MMC Exported Functions
  * @{
  */
  
/** @defgroup MMC_Exported_Functions_Group1 Initialization and de-initialization functions
  * @{
  */
HAL_StatusTypeDef HAL_MMC_Init(MMC_HandleTypeDef *hmmc);
HAL_StatusTypeDef HAL_MMC_InitCard(MMC_HandleTypeDef *hmmc);
HAL_StatusTypeDef HAL_MMC_DeInit (MMC_HandleTypeDef *hmmc);
void HAL_MMC_MspInit(MMC_HandleTypeDef *hmmc);
void HAL_MMC_MspDeInit(MMC_HandleTypeDef *hmmc);

/** @defgroup MMC_Exported_Functions_Group2 Input and Output operation functions
  * @{
  */
/* Blocking mode: Polling */
HAL_StatusTypeDef HAL_MMC_ReadBlocks(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAddr, uint32_t NumberOfBlocks, uint32_t Timeout);
HAL_StatusTypeDef HAL_MMC_WriteBlocks(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAddr, uint32_t NumberOfBlocks, uint32_t Timeout);
HAL_StatusTypeDef HAL_MMC_Erase(MMC_HandleTypeDef *hmmc, uint32_t BlockStartAdd, uint32_t BlockEndAdd);
/* Non-Blocking mode: IT */
HAL_StatusTypeDef HAL_MMC_ReadBlocks_IT(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAddr, uint32_t NumberOfBlocks);
HAL_StatusTypeDef HAL_MMC_WriteBlocks_IT(MMC_HandleTypeDef *hmmc, uint8_t *pData, uint32_t BlockAddr, uint32_t NumberOfBlocks);
/* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_MMC_ReadBlocks_DMA(MMC_HandleTypeDef *hmmc, SDIO_DMAInitTypedef *DMAInit, uint32_t BlockAddr, uint32_t NumberOfBlocks);
HAL_StatusTypeDef HAL_MMC_WriteBlocks_DMA(MMC_HandleTypeDef *hmmc, SDIO_DMAInitTypedef *DMAInit, uint32_t BlockAddr, uint32_t NumberOfBlocks);

void HAL_MMC_IRQHandler(MMC_HandleTypeDef *hmmc);

/* Callback in non blocking modes (DMA) */
void HAL_MMC_TxCpltCallback(MMC_HandleTypeDef *hmmc);
void HAL_MMC_RxCpltCallback(MMC_HandleTypeDef *hmmc);
void HAL_MMC_ErrorCallback(MMC_HandleTypeDef *hmmc);
void HAL_MMC_AbortCallback(MMC_HandleTypeDef *hmmc);

#if defined (USE_HAL_MMC_REGISTER_CALLBACKS) && (USE_HAL_MMC_REGISTER_CALLBACKS == 1U)
/* MMC callback registering/unregistering */
HAL_StatusTypeDef HAL_MMC_RegisterCallback  (MMC_HandleTypeDef *hmmc, HAL_MMC_CallbackIDTypeDef CallbackId, pMMC_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_MMC_UnRegisterCallback(MMC_HandleTypeDef *hmmc, HAL_MMC_CallbackIDTypeDef CallbackId);
#endif
  
/** @defgroup MMC_Exported_Functions_Group3 Peripheral Control functions
  * @{
  */
HAL_StatusTypeDef HAL_MMC_ConfigWideBusOperation(MMC_HandleTypeDef *hmmc, uint32_t WideMode);

/** @defgroup MMC_Exported_Functions_Group4 MMC card related functions
  * @{
  */
HAL_MMC_CardStateTypeDef HAL_MMC_GetCardState(MMC_HandleTypeDef *hmmc);
HAL_StatusTypeDef        HAL_MMC_GetCardCID(MMC_HandleTypeDef *hmmc, HAL_MMC_CardCIDTypeDef *pCID);
HAL_StatusTypeDef        HAL_MMC_GetCardCSD(MMC_HandleTypeDef *hmmc, HAL_MMC_CardCSDTypeDef *pCSD);
HAL_StatusTypeDef        HAL_MMC_GetCardInfo(MMC_HandleTypeDef *hmmc, HAL_MMC_CardInfoTypeDef *pCardInfo);


/** @defgroup MMC_Exported_Functions_Group5 Peripheral State and Errors functions
  * @{
  */
HAL_MMC_StateTypeDef HAL_MMC_GetState(MMC_HandleTypeDef *hmmc);
uint32_t HAL_MMC_GetError(MMC_HandleTypeDef *hmmc);

/** @defgroup MMC_Exported_Functions_Group6 Perioheral Abort management
  * @{
  */
HAL_StatusTypeDef HAL_MMC_Abort(MMC_HandleTypeDef *hmmc);
HAL_StatusTypeDef HAL_MMC_Abort_IT(MMC_HandleTypeDef *hmmc);

#endif /* HAL_MMC_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC90XX_HAL_MMC_H__ */

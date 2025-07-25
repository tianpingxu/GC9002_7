/**
  ******************************************************************************
  * @file    gc90xx_hal_can.h
  * @author  GraphichinaSX Team
  * @version V1.0.1
  * @date    2021/11/18
  * @brief   This file contains all the functions prototypes for the CAN 
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC90XX_HAL_CAN_H__

#define __GC90XX_HAL_CAN_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <gc90xx_hal_def.h>

#ifdef HAL_CAN_MODULE_ENABLED
#define MAX_COUNTER 1000

 /**
   * @brief  Select CAN Baud Rate
   */

 typedef enum
 {
     CAN_BaudRate_10k,
     CAN_BaudRate_20k,
     CAN_BaudRate_40k,
     CAN_BaudRate_50k,
     CAN_BaudRate_80k,
     CAN_BaudRate_100k,
     CAN_BaudRate_125k,
     CAN_BaudRate_200k,
     CAN_BaudRate_250k,
     CAN_BaudRate_400k,
     CAN_BaudRate_500k,
     CAN_BaudRate_800k,
     CAN_BaudRate_1000k
 }CAN_Baudrate_Enum;
#define IS_CAN_BAUDRATE(BAUDRATE)       ((BAUDRATE) <= CAN_BaudRate_1000k)

 typedef struct
 {
     uint32_t brp    : 6;  /* Bit Timing Segment 1 */
     uint32_t tSeg_1 : 4;  /* Bit Timing Segment 1 */
     uint32_t tSeg_2 : 3;  /* Bit Timing Segment 2 */
     uint32_t tSJW   : 2;  /* Synchronization Jump Width */
     uint32_t div    : 8;  /* The prescaler divides the system clock to get the time quanta clock tq_clk */
     uint32_t SAM    : 1;  /* Synchronization Jump Width */
     uint32_t        : 8;
 }CAN_BaudrateTypeDef;
 /**
   * @brief  HAL State structures definition
   */
 typedef enum
 {
     HAL_CAN_STATE_RESET             = 0x00U,  /*!< CAN not yet initialized or disabled */
     HAL_CAN_STATE_READY             = 0x01U,  /*!< CAN initialized and ready for use   */
     HAL_CAN_STATE_BUSY              = 0x02U,  /*!< CAN process is ongoing              */
     HAL_CAN_STATE_BUSY_TX           = 0x03U,  /*!< CAN process is ongoing              */
     HAL_CAN_STATE_BUSY_RX           = 0x04U,  /*!< CAN process is ongoing              */
     HAL_CAN_STATE_BUSY_TX_RX        = 0x05U,  /*!< CAN process is ongoing              */
     HAL_CAN_STATE_TIMEOUT           = 0x06U,  /*!< Timeout state                       */
     HAL_CAN_STATE_LISTENING         = 0x07U,  /*!< CAN receive process is ongoing      */
     HAL_CAN_STATE_SLEEP_PENDING     = 0x08U,  /*!< CAN sleep request is pending        */
     HAL_CAN_STATE_SLEEP_ACTIVE      = 0x09U,  /*!< CAN sleep mode is active            */
     HAL_CAN_STATE_ERROR             = 0x0AU   /*!< CAN error state                     */

 }HAL_CAN_StateTypeDef;
 /**
   * @brief  CAN Init Structure definition
   */
 typedef struct
 {
     uint32_t Mode;                       /*!< Specifies the CAN operating mode.
                                               This parameter can be a value of @ref CAN_operating_mode */
     CAN_Baudrate_Enum BaudRate;          /*!< Specifies the CAN BaudRate.
                                               This parameter can be a value of @ref CAN_BaudRate */
     CAN_BaudrateTypeDef timing;
 }CAN_InitTypeDef;

 /**
   * @brief  CAN filter configuration structure definition
   */
typedef struct
{
    uint32_t FilterIdHigh;          /*!< Specifies the filter identification number (MSBs for a 32-bit
                                        configuration, first one for a 16-bit configuration).
                                        This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */
    uint32_t FilterIdLow;           /*!< Specifies the filter identification number (LSBs for a 32-bit
                                        configuration, second one for a 16-bit configuration).
                                        This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */
    uint32_t FilterMaskIdHigh;      /*!< Specifies the filter mask number or identification number,
                                        according to the mode (MSBs for a 32-bit configuration,
                                        first one for a 16-bit configuration).
                                        This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */
    uint32_t FilterMaskIdLow;       /*!< Specifies the filter mask number or identification number,
                                        according to the mode (LSBs for a 32-bit configuration,
                                        second one for a 16-bit configuration).
                                        This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF. */
    uint32_t FilterBank;            /*!< Specifies the filter bank which will be initialized.
                                        For single CAN instance(14 dedicated filter banks),
                                        this parameter must be a value of @ref CAN_ACRAMR_Enum.*/
    uint32_t FilterMode;            /*!< Specifies the filter mode to be initialized.
                                        This parameter can be a value of @ref CAN_Filter_Mode */
}CAN_FilterTypeDef;
/**
  * @brief  CAN Rx message header structure definition
  */
typedef struct
{
    uint32_t StdId;    /*!< Specifies the standard identifier.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0x7FF. */
    uint32_t ExtId;    /*!< Specifies the extended identifier.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0x1FFFFFFF. */
    uint32_t IDE;      /*!< Specifies the type of identifier for the message that will be transmitted.
                          This parameter can be a value of @ref CAN_identifier_type */
    uint32_t RTR;      /*!< Specifies the type of frame for the message that will be transmitted.
                          This parameter can be a value of @ref CAN_remote_transmission_request */
    uint32_t DLC;      /*!< Specifies the length of the frame that will be transmitted.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 8. */
    uint8_t Data[8];   /*!< Contains the data to be transmitted.
                            This parameter must be a number between Min_Data = 0 and Max_Data = 0xFF */
    uint32_t Timestamp; /*!< Specifies the timestamp counter value captured on start of frame reception.
                          @note: Time Triggered Communication Mode must be enabled.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0xFFFF. */
} CAN_MsgTypeDef;
/**
 * @brief  CAN handle Structure definition
 */
typedef struct __CAN_HandleTypeDef
{
    CAN_TypeDef                 *Instance;                  /*!< Register base address */
    CAN_InitTypeDef             Init;                       /*!< CAN required parameters */
    uint32_t                    int_flag;
    CAN_MsgTypeDef              *pTxMsg;                    /*!< Transmit structure  */
    CAN_MsgTypeDef              *pRxMsg;                    /*!< Reception structure */
    HAL_LockTypeDef             Lock;                       /*!< CAN locking object */
    __IO HAL_CAN_StateTypeDef   State;                      /*!< CAN communication state */
    union
    {
        __IO uint32_t               ErrorCode;              /*!< CAN Error code.
                                                                This parameter can be a value of @ref CAN_Error_Code */
        struct
        {
            __IO uint32_t Segment_Code  : 5; // be a value of @ref enum CAN_SegmengCode
            __IO uint32_t Direction     : 1; // 1-the error occurred during reception  0-the error occurred during transmission
            __IO uint32_t Error_Code    : 2; // be a value of @ref enum CAN_ErrorCode
            __IO uint32_t               : 24;
        };
    };
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
    void (* ArbLostCallback)(struct __CAN_HandleTypeDef *hcan);     /*!< CAN ArbLost callback       */
    void (* PassiveCallback)(struct __CAN_HandleTypeDef *hcan);     /*!< CAN Passive callback       */
    void (* WakeUpCallback)(struct __CAN_HandleTypeDef *hcan);      /*!< CAN WakeUp callback        */
    void (* DataOverrunCallback)(struct __CAN_HandleTypeDef *hcan); /*!< CAN DataOverrun callback   */
    void (* ErrorCallback)(struct __CAN_HandleTypeDef *hcan);       /*!< CAN Error callback         */
    void (* BusErrorCallback)(struct __CAN_HandleTypeDef *hcan);    /*!< CAN BusError callback      */
    void (* TxCompleteCallback)(struct __CAN_HandleTypeDef *hcan);  /*!< CAN Tx Complete callback   */
    void (* RxMsgCallback)(struct __CAN_HandleTypeDef *hcan);       /*!< CAN Rx Complete callback   */
    void (* MspInitCallback)(struct __CAN_HandleTypeDef *hcan);     /*!< CAN Msp Init callback      */
    void (* MspDeInitCallback)(struct __CAN_HandleTypeDef *hcan);   /*!< CAN Msp DeInit callback    */
#endif /* (USE_HAL_CAN_REGISTER_CALLBACKS) */
} CAN_HandleTypeDef;

#define CAN_IT_BUS_ERROR            CAN_IR_BEI
#define CAN_IT_ARB_LOST             CAN_IR_ALI
#define CAN_IT_PASSIVE_ERROR        CAN_IR_EPI
#define CAN_IT_WAKE_UP              CAN_IR_WUI
#define CAN_IT_DATA_OVERRUN         CAN_IR_DOI
#define CAN_IT_ERROR_WARNING        CAN_IR_EI
#define CAN_IT_TX                   CAN_IR_TI
#define CAN_IT_RX                   CAN_IR_RI
#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
/**
  * @brief  HAL CAN common Callback ID enumeration definition
  */
typedef enum
{
    HAL_CAN_ARB_LOST_CB_ID      = 0x00U,    /*!< CAN Tx Mailbox 0 complete callback ID      */
    HAL_CAN_PASSIVE_CB_ID       = 0x01U,    /*!< CAN Passive callback ID                    */
    HAL_CAN_WAKEUP_CB_ID        = 0x02U,    /*!< CAN Wake Up fropm Rx msg callback ID       */
    HAL_CAN_DATA_OVERRUN_CB_ID  = 0x03U,    /*!< CAN Data Overrun callback ID               */
    HAL_CAN_ERROR_CB_ID         = 0x04U,    /*!< CAN Error callback ID                      */
    HAL_CAN_BUS_ERROR_CB_ID     = 0x05U,    /*!< CAN Bus Error callback ID                  */
    HAL_CAN_TX_COMPLETE_CB_ID   = 0x06U,    /*!< CAN Tx Mailbox 0 complete callback ID      */
    HAL_CAN_RX_COMPLETE_CB_ID   = 0x07U,    /*!< CAN Rx FIFO 1 full callback ID             */

    HAL_CAN_MSPINIT_CB_ID       = 0x08U,    /*!< CAN MspInit callback ID                    */
    HAL_CAN_MSPDEINIT_CB_ID     = 0x09U,    /*!< CAN MspDeInit callback ID                  */
} HAL_CAN_CallbackIDTypeDef;

/**
  * @brief  HAL CAN Callback pointer definition
  */
typedef  void (*pCAN_CallbackTypeDef)(CAN_HandleTypeDef *hcan); /*!< pointer to a CAN callback function   */

#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
/** 
  * @brief  CAN Message Struct
  */

typedef struct
{
    uint32_t StdId;//when StdId = 0, means send an Extended frame
    uint32_t ExtId;//when ExtId = 0, means send an Standard frame
    uint8_t RTR;
    uint8_t DLC;
    uint8_t Data[8];
}CanMsg;

/** 
  * @brief  Select CAN Filter Number
  */
typedef enum 
{
    CAN_ACRAMR_1 = 0x0U,
    CAN_ACRAMR_2,
    CAN_ACRAMR_3,
    CAN_ACRAMR_4,
    CAN_ACRAMR_5,
    CAN_ACRAMR_6,
    CAN_ACRAMR_7,
    CAN_ACRAMR_8,
    CAN_ACRAMR_9,
    CAN_ACRAMR_10,
    CAN_ACRAMR_11,
    CAN_ACRAMR_12,
    CAN_ACRAMR_13,
    CAN_ACRAMR_14,
    CAN_ACRAMR_15,
    CAN_ACRAMR_16,
    CAN_ACRAMR_17,
    CAN_ACRAMR_18,
    CAN_ACRAMR_19,
    CAN_ACRAMR_20,
    CAN_ACRAMR_21,
    CAN_ACRAMR_22,
    CAN_ACRAMR_23,
    CAN_ACRAMR_24,
    CAN_ACRAMR_25,
    CAN_ACRAMR_26,
    CAN_ACRAMR_27,
    CAN_ACRAMR_28,
    CAN_ACRAMR_29,
    CAN_ACRAMR_30,
    CAN_ACRAMR_NOCARE
} CAN_ACRAMR_Enum;

#define CAN_OUTPUT_MODE_NORMAL      (0x02U)
#define CAN_OUTPUT_MODE_CLOCK       (0x03U)

#define IS_CAN_FILTER_ID_HALFWORD(HALFWORD) ((HALFWORD) <= 0xFFFFU)
#define IS_CAN_FILTER_BANK_DUAL(BANK)       ((BANK) <= CAN_ACRAMR_NOCARE)
#define IS_CAN_FILTER_MODE(MODE)            (((MODE) == CAN_SingleFilter_Mode) || \
                                             ((MODE) == CAN_DualFilter_Mode))
#define IS_CAN_STDID(STDID)                 (((STDID) <= 0x7FFU) && (((STDID) & 0x7F0) != 0x7F0))
#define IS_CAN_EXTID(EXTID)                 (((EXTID) <= 0x1FFFFFFFU) && (((EXTID) & 0x1FC00000) != 0x1FC00000))

#define IS_CAN_DLC(DLC)                     ((DLC) <= 8U)
#define IS_CAN_IDTYPE(IDTYPE)               (((IDTYPE) == CAN_ID_STD) || \
                                             ((IDTYPE) == CAN_ID_EXT))
#define IS_CAN_RTR(RTR)                     (((RTR) == CAN_RTR_DATA) || \
                                             ((RTR) == CAN_RTR_REMOTE))
/** @defgroup CAN_Command_Word
  * @{
  */ 

#define    CAN_CMD_TR       CAN_CMR_TR  //CMR.0 Send request bit
#define    CAN_CMD_AT       CAN_CMR_AT  //CMR.1 Stop sending bit
#define    CAN_CMD_RRB      CAN_CMR_RRB  //CMR.2 Release receiving buffer
#define    CAN_CMD_COS      CAN_CMR_CDO  //CMR.3 Clear data overflow
#define    CAN_CMD_SRR      CAN_CMR_SRR  //CMR.4 Self-receiving mode
/** @defgroup CAN_Interrupt_Type
  * @{
  */ 

#define    Receive_IT               0x01
#define    Transmit_IT              0x02
#define    ErrorWarning_IT          0x04
#define    DataOverrun_IT           0x08
#define    WakeUp_IT                0x10
#define    ErrorPassive_IT          0x20
#define    ArbitrationLost_IT       0x40
#define    BusError_IT              0x80
/** @defgroup CAN_Output_Type
  * @{
  */ 

#define    Normal_Output            0x02
#define    Clock_Output             0x03
/** @defgroup CAN_Baud_Rate
  * @{ BTR0[0:5] means BRP   BTR1[0:3] means TSEG1    BTR1[4:6] means TSEG2  BTR0[6:7] means SJW
  *    baudRate = CAN_CLK/(2*(1 + BRP)*( 1 + (TSEG1 + 1) + (TSEG2 + 1)))
  */
#define CAN_CLK_48M
#ifdef CAN_CLK_8M
#define  CAN_BTR0_Rate_10k      0x71          //10KBPS parameter
#define  CAN_BTR1_Rate_10k      0x23          //10KBPS parameter
#define  CAN_BTR0_Rate_20k      0x58          //20KBPS parameter
#define  CAN_BTR1_Rate_20k      0x23          //20KBPS parameter
#define  CAN_BTR0_Rate_40k      0x44          //40KBPS parameter
#define  CAN_BTR1_Rate_40k      0x6B          //40KBPS parameter
#define  CAN_BTR0_Rate_50k      0x44          //50KBPS parameter
#define  CAN_BTR1_Rate_50k      0x49          //50KBPS parameter
#define  CAN_BTR0_Rate_80k      0x44          //80KBPS parameter
#define  CAN_BTR1_Rate_80k      0x34          //80KBPS parameter
#define  CAN_BTR0_Rate_100k     0x44          //100KBPS parameter
#define  CAN_BTR1_Rate_100k     0x23          //100KBPS parameter
#define  CAN_BTR0_Rate_125k     0x41          //125KBPS parameter
#define  CAN_BTR1_Rate_125k     0x49          //125KBPS parameter
#define  CAN_BTR0_Rate_200k     0x41          //200KBPS parameter
#define  CAN_BTR1_Rate_200k     0x34          //200KBPS parameter
#define  CAN_BTR0_Rate_250k     0x41          //250KBPS parameter
#define  CAN_BTR1_Rate_250k     0x23          //250KBPS parameter
#define  CAN_BTR0_Rate_400k     0x40          //400KBPS parameter
#define  CAN_BTR1_Rate_400k     0x34          //400KBPS parameter
#define  CAN_BTR0_Rate_500k     0x40          //500KBPS parameter
#define  CAN_BTR1_Rate_500k     0x23          //500KBPS parameter
#define  CAN_BTR0_Rate_800k     0x00          //800KBPS parameter
#define  CAN_BTR1_Rate_800k     0x11          //800KBPS parameter
#define  CAN_BTR0_Rate_1000k    0x00          //1000KBPS parameter
#define  CAN_BTR1_Rate_1000k    0x11          //1000KBPS parameter
#elif(defined CAN_CLK_48M)
#define  CAN_BTR0_Rate_20k      0xbb          //20KBPS parameter
#define  CAN_BTR1_Rate_20k      0x2f          //20KBPS parameter
#define  CAN_BTR0_Rate_40k      0x5d          //40KBPS parameter
#define  CAN_BTR1_Rate_40k      0x2f          //40KBPS parameter
#define  CAN_BTR0_Rate_50k      0x57          //50KBPS parameter
#define  CAN_BTR1_Rate_50k      0x2f          //50KBPS parameter
#define  CAN_BTR0_Rate_80k      0x4e          //80KBPS parameter
#define  CAN_BTR1_Rate_80k      0x2f          //80KBPS parameter
#define  CAN_BTR0_Rate_100k     0x4b          //100KBPS parameter
#define  CAN_BTR1_Rate_100k     0x2f          //100KBPS parameter
#define  CAN_BTR0_Rate_125k     0x4f          //125KBPS parameter
#define  CAN_BTR1_Rate_125k     0x18          //125KBPS parameter
#define  CAN_BTR0_Rate_200k     0x45          //200KBPS parameter
#define  CAN_BTR1_Rate_200k     0x2f          //200KBPS parameter
#define  CAN_BTR0_Rate_250k     0x47          //250KBPS parameter
#define  CAN_BTR1_Rate_250k     0x18          //250KBPS parameter
#define  CAN_BTR0_Rate_400k     0x44          //400KBPS parameter
#define  CAN_BTR1_Rate_400k     0x18          //400KBPS parameter
#define  CAN_BTR0_Rate_500k     0x43          //500KBPS parameter
#define  CAN_BTR1_Rate_500k     0x18          //500KBPS parameter
#define  CAN_BTR0_Rate_800k     0x41          //800KBPS parameter
#define  CAN_BTR1_Rate_800k     0x2a          //800KBPS parameter
#define  CAN_BTR0_Rate_1000k    0x41          //1000KBPS parameter
#define  CAN_BTR1_Rate_1000k    0x18          //1000KBPS parameter
#endif
/** @defgroup CAN_Filter_Mode
  * @{
  */
#define  CAN_SingleFilter_Mode  0x8U
#define  CAN_DualFilter_Mode    0x0U
/** @defgroup CAN_Frame_Type
  * @{
  */ 
#define  CAN_Standard           0x1
#define  CAN_Extended           0x0
/** @defgroup CAN_Error_Type
  * @{
  */ 
#define  Bit_Error          0x0 
#define  Form_Error         0x1
#define  Stuff_Error        0x2 
#define  Other_Error        0x3 
/** @defgroup CAN_OutClock_PRE
  * @{
  */ 
#define CAN_OutClock_OFF                 ((uint16_t)0xC8)
#define CAN_OutClock_PRE_1               ((uint16_t)0xC7)
#define CAN_OutClock_PRE_2               ((uint16_t)0xC0)
#define CAN_OutClock_PRE_4               ((uint16_t)0xC1)
#define CAN_OutClock_PRE_6               ((uint16_t)0xC2)
#define CAN_OutClock_PRE_8               ((uint16_t)0xC3)
#define CAN_OutClock_PRE_10              ((uint16_t)0xC4)
#define CAN_OutClock_PRE_12              ((uint16_t)0xC5)
#define CAN_OutClock_PRE_14              ((uint16_t)0xC6)

/** @defgroup CAN_operating_mode CAN Operating Mode
  * @{
  */
#define CAN_MODE_NORMAL                 (0x00U)                 /*!< Normal mode        */
#define CAN_MODE_Reset                  (CAN_MOD_RESET)         /*!< Reset  mode        */
#define CAN_MODE_Listen_Only            (CAN_MOD_LOM)           /*!< Listen Only mode   */
#define CAN_MODE_LOOPBACK               (CAN_MOD_STM)           /*!< Loopback mode      */
#define CAN_MODE_Sleep                  (CAN_MOD_SM)

#define IS_CAN_MODE(MODE)               ((MODE) == CAN_MODE_NORMAL || \
                                        (((MODE) & (CAN_MODE_Reset | CAN_MODE_Listen_Only | CAN_MODE_LOOPBACK | CAN_MODE_Sleep)) != RESET))
/** @defgroup CAN_Error_Code CAN Error Code
  * @{
  */
#define HAL_CAN_ERROR_NONE            (0x00000000U)  /*!< No error                                             */
#define HAL_CAN_ERROR_EWG             (0x00000001U)  /*!< Protocol Error Warning                               */
#define HAL_CAN_ERROR_EPV             (0x00000002U)  /*!< Error Passive                                        */
#define HAL_CAN_ERROR_BOF             (0x00000004U)  /*!< Bus-off error                                        */
#define HAL_CAN_ERROR_STF             (0x00000008U)  /*!< Stuff error                                          */
#define HAL_CAN_ERROR_FOR             (0x00000010U)  /*!< Form error                                           */
#define HAL_CAN_ERROR_ACK             (0x00000020U)  /*!< Acknowledgment error                                 */
#define HAL_CAN_ERROR_BR              (0x00000040U)  /*!< Bit recessive error                                  */
#define HAL_CAN_ERROR_BD              (0x00000080U)  /*!< Bit dominant error                                   */
#define HAL_CAN_ERROR_CRC             (0x00000100U)  /*!< CRC error                                            */
#define HAL_CAN_ERROR_RX_FOV0         (0x00000200U)  /*!< Rx FIFO0 overrun error                               */
#define HAL_CAN_ERROR_RX_FOV1         (0x00000400U)  /*!< Rx FIFO1 overrun error                               */
#define HAL_CAN_ERROR_TX_ALST         (0x00000800U)  /*!< TxMailbox 0 transmit failure due to arbitration lost */
#define HAL_CAN_ERROR_TX_TERR         (0x00001000U)  /*!< TxMailbox 1 transmit failure due to tranmit error    */
#define HAL_CAN_ERROR_TIMEOUT         (0x00002000U)  /*!< Timeout error                                        */
#define HAL_CAN_ERROR_NOT_INITIALIZED (0x00004000U)  /*!< Peripheral not initialized                           */
#define HAL_CAN_ERROR_NOT_READY       (0x00008000U)  /*!< Peripheral not ready                                 */
#define HAL_CAN_ERROR_NOT_STARTED     (0x00010000U)  /*!< Peripheral not started                               */
#define HAL_CAN_ERROR_PARAM           (0x00020000U)  /*!< Parameter error                                      */

#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
#define HAL_CAN_ERROR_INVALID_CALLBACK (0x00400000U) /*!< Invalid Callback error                               */
#endif /* USE_HAL_CAN_REGISTER_CALLBACKS */
#define HAL_CAN_ERROR_INTERNAL        (0x00800000U)  /*!< Internal error                                       */


/** @defgroup CAN_identifier_type CAN Identifier Type
  * @{
  */
#define CAN_ID_STD                      (0x00000000U)  /*!< Standard Id */
#define CAN_ID_EXT                      (0x00000001U)  /*!< Extended Id */
/** @defgroup CAN_remote_transmission_request CAN Remote Transmission Request
  * @{
  */
#define CAN_RTR_DATA                    (0x00000000U)  /*!< Data frame */
#define CAN_RTR_REMOTE                  (0x00000001U)  /*!< Remote frame */

#define CAN_FRAME_BIT_X                 (0x3U)
#define CAN_XXXX                        (0xFU)

/* Standard Frame Format, Single Filter
* ----------------------------------------------------------------------------------------------------------
* | Receive Buffer: 11b   |                12b              |          13b          |           14b        |
* ----------------------------------------------------------------------------------------------------------
* |    ID 28 ... ID 21    |ID 20...ID 18|RTR|    X X X X    |     Data Byte 1       |      Data Byte 2     |
* |                       |             |   |  not matched  |                       |                      |
* ----------------------------------------------------------------------------------------------------------
*  Filter:
* ----------------------------------------------------------------------------------------------------------
* |      ACR0[7:0]        |    ACR1[7:4]    |   ACR1[3:0]   |      ACR2[7:0]        |       ACR3[7:0]      |
* |                       |                 |    unused     |                       |                      |
* ----------------------------------------------------------------------------------------------------------
* |      AMR0[7:0]        |    AMR1[7:4]    |   AMR1[3:0]   |      AMR2[7:0]        |       AMR3[7:0]      |
* |                       |                 |    unused     |                       |                      |
* ----------------------------------------------------------------------------------------------------------
*
*  Extended Frame Format, Single Filter
* ----------------------------------------------------------------------------------------------------------
* | Receive Buffer: 11b   |        12b           |          13b           |            14b                 |
* ----------------------------------------------------------------------------------------------------------
* |    ID 28 ... ID 21    |    ID 20 ... ID 13   |     ID 12 ... ID 5     |   ID 4...ID 0    |RTR|   X X   |
* |                       |                      |                        |                  |   |   not   |
* |                       |                      |                        |                  |   | matched |
* ----------------------------------------------------------------------------------------------------------
*  Filter:
* ----------------------------------------------------------------------------------------------------------
* |      ACR0[7:0]        |      ACR1[7:0]       |       ACR2[7:0]        |      ACR3[7:2]       |ACR3[1:0]|
* |                       |                      |                        |                      |  unused |
* ----------------------------------------------------------------------------------------------------------
* |      AMR0[7:0]        |      AMR1[7:0]       |       AMR2[7:0]        |      AMR3[7:2]       |AMR3[1:0]|
* |                       |                      |                        |                      |  unused |
* ----------------------------------------------------------------------------------------------------------
* */
/**
  * @brief  Get MSB of Standard Frame ID.
  * @param  ID Standard Frame ID (11 bits).
  * @param  RTR CAN_remote_transmission_request.
  * @retval None
  */
#define CAN_GET_STD_ID_SGL_MSB(ID, RTR)          (((((ID)<<5) | ((RTR)<<4)) & 0xFFFF) | CAN_XXXX)
/**
  * @brief  Get LSB of Standard Frame ID.
  * @param  ID Standard Frame ID (11 bits).
  * @param  RTR CAN_remote_transmission_request.
  * @retval None
  */
#define CAN_GET_STD_ID_SGL_LSB(ID, RTR)          (0xFFFF)
/**
  * @brief  Get MSB of Standard Frame Mask ID.
  * @param  ID Standard Frame Mask ID (11 bits).
  * @param  RTR CAN_remote_transmission_request.
  * @retval None
  */
#define CAN_GET_STD_MSK_SGL_MSB(MSK, RTR)       ((((MSK >> 3) & 0xFF) << 8) | (((MSK & 0x7) << 5) | (RTR << 4)) | 0xF)
/**
  * @brief  Get LSB of Standard Frame Mask ID.
  * @param  ID Standard Frame Mask ID (11 bits).
  * @param  RTR CAN_remote_transmission_request.
  * @retval None
  */
#define CAN_GET_STD_MSK_SGL_LSB(MSK, RTR)       (0xFFFF)
/**
  * @brief  Get MSB of Extended Frame ID.
  * @param  ID Extended Frame ID (11 bits).
  * @param  RTR CAN_remote_transmission_request.
  * @retval None
  */
#define CAN_GET_EXT_ID_SGL_MSB(ID, RTR)          (((ID)>>13) & 0xFFFF)
#define CAN_GET_EXT_MSK_SGL_MSB                  CAN_GET_EXT_ID_SGL_MSB
/**
  * @brief  Get LSB of Extended Frame ID.
  * @param  ID Extended Frame ID (11 bits).
  * @param  RTR CAN_remote_transmission_request.
  * @retval None
  */
#define CAN_GET_EXT_ID_SGL_LSB(ID, RTR)          ((((ID)<<3) | ((RTR)<<2)) & 0xFFFF)
#define CAN_GET_EXT_MSK_SGL_LSB                  CAN_GET_EXT_ID_SGL_LSB

/* Standard Frame Format, Dual Filters
* ----------------------------------------------------------------------------------------------------------
* | Receive Buffer: 11b   |                12b              |           13b               |       14b      |
* ----------------------------------------------------------------------------------------------------------
* |    ID 28 ... ID 21    |ID 20...ID 18|RTR|    X X X X    |  Data Byte 1 |  Data Byte 1 |  Data Byte 2   |
* |                       |             |   |  not matched  |    [7:4]     |    [3:0]     |  not matched   |
* ----------------------------------------------------------------------------------------------------------
*  Filter 1:
* -------------------------------------------               -------------------------------
* |      ACR0[7:0]        |     ACR1[7:4]   |               |   ACR1[3:0]  |   ACR3[3:0]  |
* -------------------------------------------               -------------------------------
* |      AMR0[7:0]        |     AMR1[7:4]   |               |   AMR1[3:0]  |   AMR3[3:0]  |
* -------------------------------------------               -------------------------------
*  Filter 2:
* -------------------------------------------
* |      ACR2[7:0]        |     ACR3[7:4]   |
* -------------------------------------------
* |      AMR2[7:0]        |     AMR3[7:4]   |
* -------------------------------------------
*
*  Extended Frame Format, Dual Filters
* ----------------------------------------------------------------------------------------------------------
* | Receive Buffer: 11b   |        12b           |          13b          |            14b                  |
* ----------------------------------------------------------------------------------------------------------
* |    ID 28 ... ID 21    |    ID 20 ... ID 13   |     ID 12 ... ID 5    |  ID 4...ID 0  |  RTR  |   X X   |
* |                       |                      |          not          |      not      |  not  |   not   |
* |                       |                      |        matched        |    matched    |matched| matched |
* ----------------------------------------------------------------------------------------------------------
*  Filter 1:
* ------------------------------------------------
* |      ACR0[7:0]        |     ACR1[7:0]        |
* ------------------------------------------------
* |      AMR0[7:0]        |     AMR1[7:0]        |
* ------------------------------------------------
*  Filter 2:
* ------------------------------------------------
* |      ACR2[7:0]        |     ACR3[7:0]        |
* ------------------------------------------------
* |      AMR2[7:0]        |     AMR3[7:0]        |
* ------------------------------------------------
* */

#define CAN_GET_STD_ID_DUL_MSB(ID0, RTR)          (((((ID0)<<5) | ((RTR)<<4)) & 0xFFFF) | CAN_XXXX)
#define CAN_GET_STD_ID_DUL_LSB(ID1, RTR)          (((((ID1)<<5) | ((RTR)<<4)) & 0xFFFF) | CAN_XXXX)
#define CAN_GET_STD_MSK_DUL_MSB(MSK0, RTR)        (((((MSK0)<<5) | ((RTR)<<4)) & 0xFFFF) | CAN_XXXX)
#define CAN_GET_STD_MSK_DUL_LSB(MSK1, RTR)        (((((MSK0)<<5) | ((RTR)<<4)) & 0xFFFF) | CAN_XXXX)

#define CAN_GET_EXT_ID_DUL_MSB(ID0, RTR)          (((ID0)>>13) & 0xFFFF)
#define CAN_GET_EXT_ID_DUL_LSB(ID1, RTR)          (((ID1)>>13) & 0xFFFF)
#define CAN_GET_EXT_MSK_DUL_MSB(MSK0, RTR)        (((MSK0)>>13) & 0xFFFF)
#define CAN_GET_EXT_MSK_DUL_LSB(MSK1, RTR)        (((MSK0)>>13) & 0xFFFF)


#define CAN_STATUS_RX_BUFF_FULL      CAN_SR_RBS
#define CAN_STATUS_DATA_OVERRUN      CAN_SR_DOS
#define CAN_STATUS_TX_BUFF_RELEASE   CAN_SR_TBS
#define CAN_STATUS_TX_COMPLATE       CAN_SR_TCS
#define CAN_STATUS_IN_RX_MSG         CAN_SR_RS
#define CAN_STATUS_IN_TX_MSG         CAN_SR_TS
#define CAN_STATUS_ERROR             CAN_SR_ES
#define CAN_STATUS_BUS_OFF           CAN_SR_BS
/** @brief  Get Status of the specified CAN.
  * @param  __HANDLE__ specifies the CAN Handle.
  * @param  __STATUS__    specifies the interrupt source.
  *         This parameter can be one of the following values:
  *            @arg CAN_STATUS_RX_BUFF_FULL
  *            @arg CAN_STATUS_DATA_OVERRUN
  *            @arg CAN_STATUS_TX_BUFF_RELEASE
  *            @arg CAN_STATUS_TX_COMPLATE
  *            @arg CAN_STATUS_IN_RX_MSG
  *            @arg CAN_STATUS_IN_TX_MSG
  *            @arg CAN_STATUS_ERROR
  *            @arg CAN_STATUS_BUS_OFF
  * @retval None
  */
#define __HAL_CAN_GET_STATUS(__HANDLE__, __STATUS__)    (((__HANDLE__)->Instance->SR.reg & (__STATUS__)) != RESET)

#define CAN_IT_BUS_ERROR            CAN_IR_BEI
#define CAN_IT_ARB_LOST             CAN_IR_ALI
#define CAN_IT_PASSIVE_ERROR        CAN_IR_EPI
#define CAN_IT_WAKE_UP              CAN_IR_WUI
#define CAN_IT_DATA_OVERRUN         CAN_IR_DOI
#define CAN_IT_ERROR_WARNING        CAN_IR_EI
#define CAN_IT_TX                   CAN_IR_TI
#define CAN_IT_RX                   CAN_IR_RI

#define CAN_IT_ERR_ENABLE           (CAN_IT_BUS_ERROR | \
                                     CAN_IT_ARB_LOST | \
                                     CAN_IT_PASSIVE_ERROR | \
                                     CAN_IT_ERROR_WARNING)
#define CAN_IT_TX_ENABLE            (CAN_IT_BUS_ERROR | \
                                     CAN_IT_ARB_LOST | \
                                     CAN_IT_PASSIVE_ERROR | \
                                     CAN_IT_ERROR_WARNING | \
                                     CAN_IT_TX)
#define CAN_IT_RX_ENABLE            (CAN_IT_BUS_ERROR | \
                                     CAN_IT_ARB_LOST | \
                                     CAN_IT_PASSIVE_ERROR | \
                                     CAN_IT_WAKE_UP | \
                                     CAN_IT_DATA_OVERRUN | \
                                     CAN_IT_ERROR_WARNING | \
                                     CAN_IT_RX)
#define __HAL_CAN_IT_ENABLE(__HANDLE__, __IT__)     SET_BIT((__HANDLE__)->Instance->IER.reg, (__IT__))
#define __HAL_CAN_IT_DISABLE(__HANDLE__, __IT__)    CLEAR_BIT((__HANDLE__)->Instance->IER.reg, (__IT__))

//ERROR CODE (ECC[7:6])
enum CAN_ErrorCode
{
    CAN_Bit_Error,
    CAN_From_Error,
    CAN_Stuff_Error,
    CAN_Other_Error,// @ref enum CAN_SegmengCode
};
//SEGMENT CODE (ECC[4:0])
enum CAN_SegmengCode
{
    CAN_Start_of_Frame          = 0x03,//0 0 0 1 1
    CAN_ID_28_to_21             = 0x02,//0 0 0 1 0
    CAN_ID_20_to_18             = 0x06,//0 0 1 1 0
    CAN_SRTR_Bit                = 0x04,//0 0 1 0 0
    CAN_IDE_Bit                 = 0x05,//0 0 1 0 1
    CAN_ID_17_to_13             = 0x07,//0 0 1 1 1
    CAN_ID_12_to_5              = 0x0F,//0 1 1 1 1
    CAN_ID_4_to_0               = 0x0E,//0 1 1 1 0
    CAN_RTR_Bit                 = 0x0C,//0 1 1 0 0
    CAN_Reserved_Bit1           = 0x0D,//0 1 1 0 1
    CAN_Reserved_Bit0           = 0x09,//0 1 0 0 1
    CAN_Data_Length_Code        = 0x0B,//0 1 0 1 1
    CAN_Data_Field              = 0x0A,//0 1 0 1 0
    CAN_CRC_Sequence            = 0x08,//0 1 0 0 0
    CAN_CRC_Delimiter           = 0x18,//1 1 0 0 0
    CAN_Acknowledge             = 0x19,//1 1 0 0 1
    CAN_Acknowledge_Delimiter   = 0x1B,//1 1 0 1 1
    CAN_End_of_Frame            = 0x1A,//1 1 0 1 0
    CAN_Intermission            = 0x12,//1 0 0 1 0
    CAN_Active_Error_Flag       = 0x11,//1 0 0 0 1
    CAN_Passive_Error_Flag      = 0x16,//1 0 1 1 0
    CAN_Tolerate_Dominant_Bits  = 0x13,//1 0 0 1 1
    CAN_Error_Delimiter         = 0x17,//1 0 1 1 1
    CAN_Overload_Flag           = 0x1C,//1 1 1 0 0
};

/** @defgroup CAN_Exported_Functions
  * @{
  */
HAL_StatusTypeDef HAL_CAN_StructInit(CAN_HandleTypeDef *hcan);
HAL_StatusTypeDef HAL_CAN_Init(CAN_HandleTypeDef *hcan);
HAL_StatusTypeDef HAL_CAN_DeInit(CAN_HandleTypeDef *hcan);

HAL_StatusTypeDef HAL_CAN_SetBandrate(CAN_HandleTypeDef *hcan, const CAN_BaudrateTypeDef *baudrate);
HAL_StatusTypeDef HAL_CAN_SetBandrateAuto(CAN_HandleTypeDef *hcan, CAN_Baudrate_Enum baudrate);

#if USE_HAL_CAN_REGISTER_CALLBACKS == 1
/* Callbacks Register/UnRegister functions  ***********************************/
HAL_StatusTypeDef HAL_CAN_RegisterCallback(CAN_HandleTypeDef *hcan, HAL_CAN_CallbackIDTypeDef CallbackID, void (* pCallback)(CAN_HandleTypeDef *_hcan));
HAL_StatusTypeDef HAL_CAN_UnRegisterCallback(CAN_HandleTypeDef *hcan, HAL_CAN_CallbackIDTypeDef CallbackID);

#endif /* (USE_HAL_CAN_REGISTER_CALLBACKS) */

/** @addtogroup CAN_Exported_Functions_Group2 Configuration functions
 *  @brief    Configuration functions
 * @{
 */
/* Configuration functions ****************************************************/
HAL_StatusTypeDef HAL_CAN_ConfigFilter(CAN_HandleTypeDef *hcan, CAN_FilterTypeDef *sFilterConfig);
/* Control functions **********************************************************/
HAL_StatusTypeDef HAL_CAN_TxMessage(CAN_HandleTypeDef *hcan, const CAN_MsgTypeDef *pTxMsg, uint32_t Timeout);
HAL_StatusTypeDef HAL_CAN_GetRxMessage(CAN_HandleTypeDef *hcan, CAN_MsgTypeDef *pRxMsg, uint32_t Timeout);
HAL_StatusTypeDef HAL_CAN_GetRxMessage_IT(CAN_HandleTypeDef *hcan, CAN_MsgTypeDef *pRxMsg);
//HAL_StatusTypeDef HAL_CAN_TxMessage_IT(CAN_HandleTypeDef *hcan, const CAN_MsgTypeDef *pTxMsg);

/** @addtogroup CAN_Exported_Functions_Group4 Interrupts management
 *  @brief    Interrupts management
 * @{
 */
/* Interrupts management ******************************************************/
void HAL_CAN_IRQHandler(CAN_HandleTypeDef *hcan);
/** @addtogroup CAN_Exported_Functions_Group5 Callback functions
 *  @brief    Callback functions
 */
/* Callbacks functions ********************************************************/
void HAL_CAN_ArbLostCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_PassiveCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_WakeUpCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_DataOverrunCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_BusErrorCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_TxCompleteCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_RxMsgCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan);
void HAL_CAN_MspDeInit(CAN_HandleTypeDef *hcan);
/** @addtogroup CAN_Exported_Functions_Group6 Peripheral State and Error functions
 *  @brief   CAN Peripheral State functions
 * @{
 */
void HAL_CAN_SetMode(CAN_HandleTypeDef *hcan, uint32_t mode);

#endif/* HAL_CAN_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC90XX_HAL_CAN_H__ */

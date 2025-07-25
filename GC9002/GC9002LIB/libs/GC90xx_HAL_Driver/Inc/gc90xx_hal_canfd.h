/**
  ******************************************************************************
  * @file    gc90xx_hal_canfd.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2022-10-18
  * @brief   This file contains all the functions prototypes for the CANFD
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC90XX_HAL_CANFD_H__

#define __GC90XX_HAL_CANFD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <gc90xx_hal_def.h>
#ifdef HAL_CANFD_MODULE_ENABLED

enum {
    //CFG_STAT
    CANFD_BUSOFF                                  = 0x00000001,
    CANFD_TACTIVE                                 = 0x00000002,
    CANFD_RACTIVE                                 = 0x00000004,
    CANFD_TSSS                                    = 0x00000008,
    CANFD_TPSS                                    = 0x00000010,
    CANFD_LBMI                                    = 0x00000020,
    CANFD_LBME                                    = 0x00000040,
    CANFD_RESET                                   = 0x00000080,

    //TCMD
    CANFD_TSA                                     = 0x00000100,
    CANFD_TSALL                                   = 0x00000200,
    CANFD_TSONE                                   = 0x00000400,
    CANFD_TPA                                     = 0x00000800,
    CANFD_TPE                                     = 0x00001000,
    CANFD_STBY                                    = 0x00002000,
    CANFD_LOM                                     = 0x00004000,
    CANFD_TBSEL                                   = 0x00008000,

    //TCTRL
    CANFD_TSSTAT                                  = 0x00030000,
    CANFD_TTTBM                                   = 0x00100000,
    CANFD_TSMODE                                  = 0x00200000,
    CANFD_TSNEXT                                  = 0x00400000,
    CANFD_FD_ISO                                  = 0x00800000,

    //RCTRL
    CANFD_RSTAT                                   = 0x03000000,
    CANFD_RBALL                                   = 0x08000000,
    CANFD_RREL                                    = 0x10000000,
    CANFD_ROV                                     = 0x20000000,
    CANFD_ROM                                     = 0x40000000,
    CANFD_SACK                                    = 0x80000000,

    //RTIE
    CANFD_TSFF                                    = 0x00000001,
    CANFD_EIE                                     = 0x00000002,
    CANFD_TSIE                                    = 0x00000004,
    CANFD_TPIE                                    = 0x00000008,
    CANFD_RAFIE                                   = 0x00000010,
    CANFD_RFIE                                    = 0x00000020,
    CANFD_ROIE                                    = 0x00000040,
    CANFD_RIE                                     = 0x00000080,

    //RTIF
    CANFD_AIF                                     = 0x00000100,
    CANFD_EIF                                     = 0x00000200,
    CANFD_TSIF                                    = 0x00000400,
    CANFD_TPIF                                    = 0x00000800,
    CANFD_RAFIF                                   = 0x00001000,
    CANFD_RFIF                                    = 0x00002000,
    CANFD_ROIF                                    = 0x00004000,
    CANFD_RIF                                     = 0x00008000,

    //ERRINT
    CANFD_BEIF                                    = 0x00010000,
    CANFD_BEIE                                    = 0x00020000,
    CANFD_ALIF                                    = 0x00040000,
    CANFD_ALIE                                    = 0x00080000,
    CANFD_EPIF                                    = 0x00100000,
    CANFD_EPIE                                    = 0x00200000,
    CANFD_EPASS                                   = 0x00400000,
    CANFD_EWARN                                   = 0x00800000,

    //LIMIT
    CANFD_EWL                                     = 0x0f000000,
    CANFD_AFWL                                    = 0xf0000000,

    //EALCAP
    CANFD_ALC                                     = 0x0000001f,
    CANFD_KOER                                    = 0x000000e0,

    //TDC
    CANFD_SSPOFF                                  = 0x00007f00,                  
    CANFD_TDCEN                                   = 0x00008000,

    //ACFCTRL
    CANFD_ACFADR                                  = 0x0000000f,
    CANFD_SELMASK                                 = 0x00000020,

    //TIMECFG
    CANFD_TIMEEN                                  = 0x00000100,
    CANFD_TIMEPOS                                 = 0x00000200,

    //ACF_3
    CANFD_AIDE                                    = 0x20000000,
    CANFD_AIDEE                                   = 0x40000000,

    //TBSLOT
    CANFD_TBPTR                                   = 0x003f0000,
    CANFD_TBF                                     = 0x00400000,
    CANFD_TBE                                     = 0x00800000,

    //TTCFG
    CANFD_TTEN                                    = 0x01000000,
    CANFD_T_PRESC                                 = 0x06000000,
    CANFD_TTIF                                    = 0x08000000,
    CANFD_TTIE                                    = 0x10000000,
    CANFD_TEIF                                    = 0x20000000,
    CANFD_WTIF                                    = 0x40000000,
    CANFD_WTIE                                    = 0x80000000,

    //REF_MSG_3
    CANFD_REF_IDE                                 = 0x80000000,

    //TRIG_CFG_0
    CANFD_TTPTR                                   = 0x0000003f,

    //TRIG_CFG_1
    CANFD_TTYPE                                   = 0x00000700,
    CANFD_TEW                                     = 0x0000f000,
};
#define IS_CANFD_CLK(CLK)       ((CLK) == 20000000 || \
                                 ((CLK) == 43200000/11) || \
                                 (CLK) == 40000000 || \
                                 (CLK) == 80000000)
typedef enum
{
    CANFD_Frame_CAN20 = 0x0U,   //CAN 2.0B mode
    CANFD_Frame_CANFD,          //FD mode
}CANFD_FrameType;
/** 
  * @brief  CAN Message Struct
  */

/* DLC for CAN2.0B frame*/
/*
 * 0000 to 1000  <--> Payload in Bytes: 0 to 8
 * 1001 to 1111  <--> Payload in Bytes: 8
 */
/* DLC for CAN FD frame */
/*
 * 0000 to 1000  <--> Payload in Bytes: 0 to 8
 * 1001          <--> Payload in Bytes: 12
 * 1010          <--> Payload in Bytes: 16
 * 1011          <--> Payload in Bytes: 20
 * 1100          <--> Payload in Bytes: 24
 * 1101          <--> Payload in Bytes: 32
 * 1110          <--> Payload in Bytes: 48
 * 1111          <--> Payload in Bytes: 64
 */
typedef enum
{
    CANFD_DLC_0 = 0x0U,
    CANFD_DLC_1,
    CANFD_DLC_2,
    CANFD_DLC_3,
    CANFD_DLC_4,
    CANFD_DLC_5,
    CANFD_DLC_6,
    CANFD_DLC_7,
    CANFD_DLC_8,
    CANFD_DLC_12,
    CANFD_DLC_16,
    CANFD_DLC_20,
    CANFD_DLC_24,
    CANFD_DLC_32,
    CANFD_DLC_48,
    CANFD_DLC_64,
}CANFD_DLC_Enum;
typedef enum
{
    CANFD_BRS_Disable = 0x0U,
    CANFD_BRS_Enable
}CANFD_BRS_Enum;
//CAN 2.0 Remote Transmission Request
typedef enum
{
    CANFD_RTR_DATA = 0x0U,  /*!< Data frame */
    CANFD_RTR_REMOTE        /*!< Remote frame */
}CANFD_RTR_Enum;
/** @defgroup CANFD_identifier_type CANFD Identifier Type
  * @{
  */
typedef enum
{
    CANFD_ID_STD = 0x0U,    /*!< Standard Id */
    CANFD_ID_EXT            /*!< Extended Id */
}CANFD_IDE_Enum;
typedef struct
{
    uint32_t StdId;     /*!< Specifies the standard identifier.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0x7FF. */
    uint32_t ExtId;     /*!< Specifies the extended identifier.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0x1FFFFFFF. */
    uint32_t DLC;       /*!< Specifies the length of the frame that will be transmitted.
                          This parameter can be a value of @ref CANFD_DLC_Enum */
    uint32_t BRS;       /*!< Specifies Bit Rate Switch, Only CAN FD frames can switch the bitrate.
                          This parameter can be a value of @ref CANFD_BRS_Enum */
    uint32_t FDF;       /*!< Specifies CAN FD frame Select.
                          This parameter can be a value of @ref CANFD_FrameType.*/
    uint32_t RTR;       /*!< Specifies the type of frame for the message that will be transmitted.
                          This parameter can be a value of @ref CANFD_RTR_Enum */
    uint32_t IDE;       /*!< Specifies the type of identifier for the message that will be transmitted.
                          This parameter can be a value of @ref CANFD_IDE_Enum */
    uint8_t Data[64];   /*!< You should fill Data[] as needed . Fill it to 4*n Bytes, because data bus is 32bit,
                          it will be processed 32bit by 32bit. */
    uint64_t Timestamp; /*!< Specifies the timestamp counter value captured on start of frame reception.
                          @note: Time Triggered Communication Mode must be enabled.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0xFFFFFFFF. */
}CANFD_MsgTypeDef;
/*
 *  can standard forbid setting recessivity for [4-10]bits
 * */
#define IS_CANFD_STD_ID(ID)     (((ID) < 0x07FFU) && (((ID) & 0x7F0U) != 0x7F0U))
#define IS_CANFD_EXT_ID(ID)     (((ID) < 0x1FFFFFFFU) && (((ID)& 0x1FC00000U) != 0x1FC00000U))
/**
  * @brief  HAL State structures definition
  */
typedef enum
{
    HAL_CANFD_STATE_RESET             = 0x00U,  /*!< CANFD not yet initialized or disabled */
    HAL_CANFD_STATE_READY             = 0x01U,  /*!< CANFD initialized and ready for use   */
    HAL_CANFD_STATE_BUSY              = 0x02U,  /*!< CANFD process is ongoing              */
    HAL_CANFD_STATE_BUSY_TX           = 0x03U,  /*!< CANFD process is ongoing              */
    HAL_CANFD_STATE_BUSY_RX           = 0x04U,  /*!< CANFD process is ongoing              */
    HAL_CANFD_STATE_BUSY_TX_RX        = 0x05U,  /*!< CANFD process is ongoing              */
    HAL_CANFD_STATE_TIMEOUT           = 0x06U,  /*!< Timeout state                         */
    HAL_CANFD_STATE_LISTENING         = 0x07U,  /*!< CANFD receive process is ongoing      */
    HAL_CANFD_STATE_SLEEP_PENDING     = 0x08U,  /*!< CANFD sleep request is pending        */
    HAL_CANFD_STATE_SLEEP_ACTIVE      = 0x09U,  /*!< CANFD sleep mode is active            */
    HAL_CANFD_STATE_ERROR             = 0x0AU   /*!< CANFD error state                     */
}HAL_CANFD_StateTypeDef;

/* CANFD buadrate config in 40MHz can_clk*/
typedef enum
{
//    CANFD_BAUDRATE_5kbps = 5000,       //for slow baudrate
//    CANFD_BAUDRATE_10kbps = 10000,     //for slow baudrate
//    CANFD_BAUDRATE_20kbps = 20000,     //for slow baudrate
//    CANFD_BAUDRATE_50kbps = 50000,     //for slow baudrate
//    CANFD_BAUDRATE_100kbps = 100000,   //for slow baudrate
    CANFD_BAUDRATE_125kbps = 125000,   //for slow baudrate
    CANFD_BAUDRATE_250kbps = 250000,   //for slow baudrate
    
    CANFD_BAUDRATE_500kbps = 500000,   //for slow or fast baudrate
    CANFD_BAUDRATE_833kbps = 833000,   //for slow or fast baudrate
    CANFD_BAUDRATE_1mbps = 1000000,    //for slow or fast baudrate
    
    CANFD_BAUDRATE_1538kbps = 1538000, //for fast baudrate
    CANFD_BAUDRATE_2mbps = 2000000,    //for fast baudrate
    CANFD_BAUDRATE_3077kbps = 3077000, //for fast baudrate
    CANFD_BAUDRATE_4mbps = 4000000,    //for fast baudrate
    CANFD_BAUDRATE_5mbps = 5000000,    //for fast baudrate
    CANFD_BAUDRATE_6667kbps = 6667000, //for fast baudrate
    CANFD_BAUDRATE_8mbps = 8000000,    //for fast baudrate
    CANFD_BAUDRATE_10mbps = 10000000,  //for fast baudrate
}CANFD_Baudrate_Enum;
#define IS_CANFD_BAUDRATE_SLOW(BAUDRATE)    ((BAUDRATE) == CANFD_BAUDRATE_125kbps || \
                                            (BAUDRATE) == CANFD_BAUDRATE_250kbps  || \
                                            (BAUDRATE) == CANFD_BAUDRATE_500kbps  || \
                                            (BAUDRATE) == CANFD_BAUDRATE_833kbps  || \
                                            (BAUDRATE) == CANFD_BAUDRATE_1mbps)
#define IS_CANFD_BAUDRATE_HIGH(BAUDRATE)    ((BAUDRATE) == CANFD_BAUDRATE_500kbps || \
                                            (BAUDRATE) == CANFD_BAUDRATE_833kbps  || \
                                            (BAUDRATE) == CANFD_BAUDRATE_1mbps    || \
                                            (BAUDRATE) == CANFD_BAUDRATE_1538kbps || \
                                            (BAUDRATE) == CANFD_BAUDRATE_2mbps    || \
                                            (BAUDRATE) == CANFD_BAUDRATE_3077kbps || \
                                            (BAUDRATE) == CANFD_BAUDRATE_4mbps    || \
                                            (BAUDRATE) == CANFD_BAUDRATE_5mbps    || \
                                            (BAUDRATE) == CANFD_BAUDRATE_6667kbps || \
                                            (BAUDRATE) == CANFD_BAUDRATE_8mbps    || \
                                            (BAUDRATE) == CANFD_BAUDRATE_10mbps)
#define CANFD_SET_BAUDRATE_AUTO     (1U)
/**
  * @brief  Select CANFD prescaler
  */
#define CANFD_PRESCALER_0               0x0U
#define CANFD_PRESCALER_1               0x1U
#define CANFD_PRESCALER_2               0x2U
#define CANFD_PRESCALER_3               0x3U
#define IS_CANFD_PRESCALER(PRESCALER)    ((PRESCALER) == CANFD_PRESCALER_0 || \
                                          (PRESCALER) == CANFD_PRESCALER_1 || \
                                          (PRESCALER) == CANFD_PRESCALER_2 || \
                                          (PRESCALER) == CANFD_PRESCALER_3)

/** @defgroup CANFD_operating_mode CANFD Operating Mode
  * @{
  */
typedef enum
{
    CANFD_Mode_Normal = 0x0U,           /*!< Normal mode            */
    CANFD_Mode_LoopbackExternal = 0x01U,/*!< Loopback External mode */
    CANFD_Mode_LoopbackInternal = 0x02U,/*!< Loopback Internal mode */
    CANFD_Mode_Listen_Only = 0x04U,     /*!< Listen Only mode       */
    CANFD_Mode_LOOPBACKANLISTEN = 0x6U, /*!< Loopback+Listen mode   */
}CANFD_Mode_Enum;
#define IS_CANFD_MODE(MODE)     ((MODE) == CANFD_Mode_Normal || \
                                 (MODE) == CANFD_Mode_LoopbackExternal || \
                                 (MODE) == CANFD_Mode_LoopbackInternal || \
                                 (MODE) == CANFD_Mode_Listen_Only || \
                                 (MODE) == CANFD_Mode_LOOPBACKANLISTEN)
typedef enum
{
    CANFD_DISABLE = 0x0U,
    CANFD_ENABLE = 0x1U,
}CANFD_EN_Enum;
#define IS_CANFD_EN(EN)         ((EN) == CANFD_DISABLE || \
                                 (EN) == CANFD_ENABLE)

typedef struct
{
    uint8_t tSeg_1_slow;  /* Bit Timing Segment 1  (slow speed) */
    uint8_t tSeg_2_slow;  /* Bit Timing Segment 2  (slow speed) */
    uint8_t tSJW_slow;    /* Synchronization Jump Width  (slow speed) */
    uint8_t div_slow;     /* The prescaler divides the system clock to get the time quanta clock tq_clk  (slow speed) */
    uint8_t tSeg_1_fast;  /* Bit Timing Segment 1 (fast speed) */
    uint8_t tSeg_2_fast;  /* Bit Timing Segment 2 (fast speed) */
    uint8_t tSJW_fast;    /* Synchronization Jump Width (fast speed) */
    uint8_t div_fast;     /* The prescaler divides the system clock to get the time quanta clock tq_clk (fast speed) */
    uint32_t ssp_offset : 7;   /* Secondary Sample Point OFFset,if set 0, Transmitter Delay Compensation is disabled */
    uint32_t            : 25;
}CANFD_BaudrateTypeDef;
typedef struct
{
    uint32_t Mode : 4;      /*!< Specifies the CANFD operating mode.
                                This parameter can be a value of @ref CANFD_operating_mode */
    uint32_t ISO_En : 4;    /*!< Specifies the CAN FD ISO mode.
                                This parameter can be a value of @ref CANFD_EN_Enum */
    uint32_t Prescaler : 4; /*!< Specifies the CANFD Prescaler.
                                This parameter can be a value of @ref CANFD_PrescalerTypeDef. */
    uint32_t : 20;
    uint32_t Baudrate_slow; /*!< Specifies the CANFD Baudrate.
                                This parameter can be a value of @ref CANFD_Baudrate_Enum. */
    uint32_t Baudrate_fast; /*!< Specifies the CANFD Baudrate.
                                This parameter can be a value of @ref CANFD_Baudrate_Enum. */
    CANFD_BaudrateTypeDef baudrate;  
}CANFD_InitTypeDef;
/**
  * @brief  Select CANFD Filter Number
  */
typedef enum
{
    CANFD_ACRAMR_0 = 0x0U,
    CANFD_ACRAMR_1,
    CANFD_ACRAMR_2,
    CANFD_ACRAMR_3,
    CANFD_ACRAMR_4,
    CANFD_ACRAMR_5,
    CANFD_ACRAMR_6,
    CANFD_ACRAMR_7,
    CANFD_ACRAMR_8,
    CANFD_ACRAMR_9,
    CANFD_ACRAMR_10,
    CANFD_ACRAMR_11,
    CANFD_ACRAMR_12,
    CANFD_ACRAMR_13,
    CANFD_ACRAMR_14,
    CANFD_ACRAMR_15,
    CANFD_ACRAMR_NOCARE,
} CANFD_ACRAMR_Enum;
#define IS_CANFD_FILTER_ACRAMR(ACRAMR)  ((ACRAMR) >= CANFD_ACRAMR_0 || \
                                         (ACRAMR) <= CANFD_ACRAMR_NOCARE)
typedef enum
{
    CANFD_Filter_STD,
    CANFD_Filter_EXT,
    CANFD_Filter_STD_EXT,
}CANFD_Filter_Mode;
#define IS_CANFD_FILTER_MODE(MODE)      ((MODE) == CANFD_Filter_STD || \
                                         (MODE) == CANFD_Filter_EXT || \
                                         (MODE) == CANFD_Filter_STD_EXT)
#define IS_CANFD_FILTER_ID(ID)          ((ID) >= 0x00000000U || \
                                         (ID) <= 0x1FFFFFFFU)

/**
  * @brief  CANFD filter configuration structure definition
  */
typedef struct
{
   uint32_t FilterId;               /*!< Specifies the filter identification number.
                                       This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0x1FFFFFFFU. */
   uint32_t FilterMaskId;           /*!< Specifies the filter mask number or identification number,according to the mode.
                                       This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0x1FFFFFFFU. */
   uint32_t FilterBank;            /*!< Specifies the filter bank which will be initialized.
                                       For single CAN instance(14 dedicated filter banks),
                                       this parameter must be a value of @ref CANFD_ACRAMR_Enum.*/
   uint32_t FilterMode;            /*!< Specifies the filter mode to be initialized.
                                       This parameter can be a value of @ref CANFD_Filter_Mode */
}CANFD_FilterTypeDef;

/**
 * @brief  CANFD handle Structure definition
 */
typedef struct __CANFD_HandleTypeDef
{
    CANFD_TypeDef               *Instance;                                      /*!< Register base address                  */
    CANFD_InitTypeDef           Init;                                           /*!< CANFD required parameters              */
    CANFD_MsgTypeDef            *pTxMsg;                                        /*!< Transmit structure                     */
    CANFD_MsgTypeDef            *pRxMsg;                                        /*!< Reception structure                    */
#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
    void (* AbortCallback)(struct __CANFD_HandleTypeDef *hcanfd);               /*!< CANFD Abort Callback                   */
    void (* ErrorCallback)(struct __CANFD_HandleTypeDef *hcanfd);               /*!< CANFD Error Callback                   */
    void (* TxSecCpltCallback)(struct __CANFD_HandleTypeDef *hcanfd);     /*!< CANFD Tx Secondary Complete Callback   */
    void (* TxPriCpltCallback)(struct __CANFD_HandleTypeDef *hcanfd);       /*!< CANFD Tx Primary Complete Callback     */
    void (* RxBuffAlmostFullCallback)(struct __CANFD_HandleTypeDef *hcanfd);    /*!< CANFD Rx Buff Almost Full Callback     */
    void (* RxBuffFullCallback)(struct __CANFD_HandleTypeDef *hcanfd);          /*!< CANFD Rx Buff Full Callback            */
    void (* RxBuffOverrunCallback)(struct __CANFD_HandleTypeDef *hcanfd);       /*!< CANFD Rx Buff Overrun Callback         */
    void (* RxMsgCallback)(struct __CANFD_HandleTypeDef *hcanfd);               /*!< CANFD Rx Msg Callback                  */
    void (* PassiveErrorCallback)(struct __CANFD_HandleTypeDef *hcanfd);        /*!< CANFD Passive Error Callback           */
    void (* ArbitrationLostCallback)(struct __CANFD_HandleTypeDef *hcanfd);     /*!< CANFD Arbitration Lost Callback        */
    void (* BusErrorCallback)(struct __CANFD_HandleTypeDef *hcanfd);            /*!< CANFD Bus Error Callback               */

    void (* MspInitCallback)(struct __CANFD_HandleTypeDef *hcanfd);             /*!< CANFD Msp Init callback                */
    void (* MspDeInitCallback)(struct __CANFD_HandleTypeDef *hcanfd);           /*!< CANFD Msp DeInit callback              */
#endif
    HAL_LockTypeDef             Lock;                                           /*!< CANFD locking object                   */
    __IO HAL_CANFD_StateTypeDef State;                                          /*!< CANFD communication state              */
    __IO uint32_t               ErrorCode;                                      /*!< CANFD Error code.
                                                                                     This parameter can be a value of @ref CANFD_Error_Code */
}CANFD_HandleTypeDef;

#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
/**
  * @brief  HAL CANFD Callback ID enumeration definition
  */
typedef enum
{
    HAL_CANFD_ABORT_CB_ID               = 0x00U,     /*!< CANFD Abort Callback ID                 */
    HAL_CANFD_ERROR_CB_ID               = 0x01U,     /*!< CANFD Error Callback ID                 */
    HAL_CANFD_TX_SEC_COMPLETE_CB_ID     = 0x02U,     /*!< CANFD Tx Secondary Complete Callback ID */
    HAL_CANFD_TX_PRI_COMPLETE_CB_ID     = 0x03U,     /*!< CANFD Tx Primary Complete Callback ID   */
    HAL_CANFD_RX_BUFF_ALMOST_FULL_CB_ID = 0x04U,     /*!< CANFD Rx Buff Almost Full Callback ID   */
    HAL_CANFD_RX_BUFF_FULL_CB_ID        = 0x05U,     /*!< CANFD Rx Buff Full Callback ID          */
    HAL_CANFD_RX_OVERRUN_CB_ID          = 0x06U,     /*!< CANFD Rx Buff Overrun Callback ID       */
    HAL_CANFD_RX_MSG_CB_ID              = 0x07U,     /*!< CANFD Rx Msg Callback ID                */
    HAL_CANFD_PASSIVE_ERROR_CB_ID       = 0x08U,     /*!< CANFD Passive Error Callback ID         */
    HAL_CANFD_ARB_LOST_CB_ID            = 0x09U,     /*!< CANFD Arbitration Lost Callback ID      */
    HAL_CANFD_BUS_ERROR_CB_ID           = 0x0AU,     /*!< CANFD Bus Error Callback ID             */

    HAL_CANFD_MSPINIT_CB_ID             = 0x0BU,     /*!< CANFD MspInit callback ID               */
    HAL_CANFD_MSPDEINIT_CB_ID           = 0x0CU      /*!< CANFD MspDeInit callback ID             */
}HAL_CANFD_CallbackIDTypeDef;

/**
  * @brief  HAL CANFD Callback pointer definition
  */
typedef  void (*pCANFD_CallbackTypeDef)(CANFD_HandleTypeDef *huart);  /*!< pointer to an CANFD callback function */
#endif /* USE_HAL_CANFD_REGISTER_CALLBACKS */

typedef enum
{
    CANFD_STB_TSONE,
    CANFD_STB_TSALL,
}CANFD_STB_TS_SELECT;

/** @defgroup CANFD_TBSEL
  * @{
  */
typedef enum
{
    CANFD_TBSEL_PTB,
    CANFD_TBSEL_STB
}CANFD_TBSEL_TypeDef;
typedef enum
{
    CANFD_Priority_Low,
    CANFD_Priority_High
}CANFD_Priority_TypeDef;

/** @defgroup CANFD_TSMODE
  * @{
  */
typedef enum
{
    CANFD_TSMODE_FIFO,
    CANFD_TSMODE_PRIO
}CANFD_TSMODE_TypeDef;

/** @defgroup CANFD_ROM
  * @{
  */
typedef enum
{
    CANFD_ROM_OVERWRITTEN,
    CANFD_ROM_DISCARD
}CANFD_OVERFLOW_MODE_TypeDef;

 /** @defgroup CANFD_TIMEPOS
  * @{
  */
typedef enum
{
    CANFD_TIMEPOS_SOF,
    CANFD_TIMEPOS_EOF
}CANFD_TIMEPOS_TypeDef;



/** @defgroup CANFD_Error_Code CANFD Error Code
  * @{
  */
#define HAL_CANFD_ERROR_NONE            (0x00000000U)  /*!< No error                                             */
#define HAL_CANFD_ERROR_EWG             (0x00000001U)  /*!< Protocol Error Warning                               */
#define HAL_CANFD_ERROR_EPV             (0x00000002U)  /*!< Error Passive                                        */
#define HAL_CANFD_ERROR_BOF             (0x00000004U)  /*!< Bus-off error                                        */
#define HAL_CANFD_ERROR_STF             (0x00000008U)  /*!< Stuff error                                          */
#define HAL_CANFD_ERROR_FOR             (0x00000010U)  /*!< Form error                                           */
#define HAL_CANFD_ERROR_ACK             (0x00000020U)  /*!< Acknowledgment error                                 */
#define HAL_CANFD_ERROR_BR              (0x00000040U)  /*!< Bit recessive error                                  */
#define HAL_CANFD_ERROR_BD              (0x00000080U)  /*!< Bit dominant error                                   */
#define HAL_CANFD_ERROR_CRC             (0x00000100U)  /*!< CRC error                                            */
#define HAL_CANFD_ERROR_RX_FOV0         (0x00000200U)  /*!< Rx FIFO0 overrun error                               */
#define HAL_CANFD_ERROR_RX_FOV1         (0x00000400U)  /*!< Rx FIFO1 overrun error                               */
#define HAL_CANFD_ERROR_TX_ALST         (0x00000800U)  /*!< TxMailbox 0 transmit failure due to arbitration lost */
#define HAL_CANFD_ERROR_TX_TERR         (0x00001000U)  /*!< TxMailbox 1 transmit failure due to tranmit error    */
#define HAL_CANFD_ERROR_TIMEOUT         (0x00002000U)  /*!< Timeout error                                        */
#define HAL_CANFD_ERROR_NOT_INITIALIZED (0x00004000U)  /*!< Peripheral not initialized                           */
#define HAL_CANFD_ERROR_NOT_READY       (0x00008000U)  /*!< Peripheral not ready                                 */
#define HAL_CANFD_ERROR_NOT_STARTED     (0x00010000U)  /*!< Peripheral not started                               */
#define HAL_CANFD_ERROR_PARAM           (0x00020000U)  /*!< Parameter error                                      */
#define HAL_CANFD_ERROR_TB_FULL         (0x00040000U)  /*!< TX Buffer Full                                       */
#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
#define HAL_CANFD_ERROR_INVALID_CALLBACK (0x00080000U)   /*!< Invalid Callback error  */
#endif /* USE_HAL_CANFD_REGISTER_CALLBACKS */

#define __HAL_CANFD_ISO_MODE_ENABLE(_HANDLE_)       SET_BIT((_HANDLE_)->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_FD_ISO)
#define __HAL_CANFD_ISO_MODE_DISABLE(_HANDLE_)      CLEAR_BIT((_HANDLE_)->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_FD_ISO)

#define __HAL_CANFD_IS_TX_ACTIVE(_HANDLE_)          (((_HANDLE_)->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value & CANFD_TACTIVE) == CANFD_TACTIVE)
#define __HAL_CANFD_IS_RX_ACTIVE(_HANDLE_)          (((_HANDLE_)->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value & CANFD_RACTIVE) == CANFD_RACTIVE)

/**
  * @brief  enter CANFD bus off state. Writing a 1 to BUSOFF will reset TECNT and RECNT. This should be done only for debugging.
  * @param  _HANDLE_ pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval none
  */
#define __HAL_CANFD_ENTER_BUSOFF(_HANDLE_)          SET_BIT((_HANDLE_)->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_BUSOFF)
/**
  * @brief  quit CANFD bus off state. Writing a 1 to BUSOFF will reset TECNT and RECNT. This should be done only for debugging.
  * @param  _HANDLE_ pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval none
  */
#define __HAL_CANFD_QUIT_BUSOFF(_HANDLE_)           CLEAR_BIT((_HANDLE_)->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_BUSOFF)


#define CANFD_IT_EIE            CANFD_EIE      //Error Interrupt Enable
#define CANFD_IT_TSIE           CANFD_TSIE     //Transmission Secondary Interrupt Flag
#define CANFD_IT_TPIE           CANFD_TPIE     //Transmission Primary Interrupt Flag
#define CANFD_IT_RAFIE          CANFD_RAFIE    //RB Almost Full Interrupt Flag
#define CANFD_IT_RFIE           CANFD_RFIE     //RB Full Interrupt Flag
#define CANFD_IT_ROIE           CANFD_ROIE     //RB Overrun Interrupt Flag
#define CANFD_IT_RIE            CANFD_RIE      //Receive Interrupt Flag
#define CANFD_IT_BEIE           CANFD_BEIE     //Bus Error Interrupt Flag
#define CANFD_IT_ALIE           CANFD_ALIE     //Arbitration Lost Interrupt Flag
#define CANFD_IT_EPIE           CANFD_EPIE     //Error Passive Interrupt Flag

#define CANFD_IT_TTIE           CANFD_TTIE     //Time Trigger Interrupt Flag
#define CANFD_IT_WTIE           CANFD_WTIE     //Watch Trigger Interrupt Flag
#define CANFD_IT_ALL            (CANFD_IT_EIE | CANFD_IT_TSIE | CANFD_IT_TPIE | CANFD_IT_RAFIE | CANFD_IT_RFIE | \
                                 CANFD_IT_ROIE| CANFD_IT_RIE  | CANFD_IT_BEIE | CANFD_IT_ALIE  | CANFD_IT_EPIE)
#define CANFD_IT_OTHER          (CANFD_IT_TTIE | CANFD_IT_WTIE)

#define CANFD_FLAG_AIF          (CANFD_AIF  )    //Abort Interrupt Flag
#define CANFD_FLAG_EIF          (CANFD_EIF  )    //Error Interrupt Flag
#define CANFD_FLAG_TSIF         (CANFD_TSIF )    //Transmission Secondary Interrupt Flag
#define CANFD_FLAG_TPIF         (CANFD_TPIF )    //Transmission Primary Interrupt Flag
#define CANFD_FLAG_RAFIF        (CANFD_RAFIF)    //RB Almost Full Interrupt Flag
#define CANFD_FLAG_RFIF         (CANFD_RFIF )    //RB Full Interrupt Flag
#define CANFD_FLAG_ROIF         (CANFD_ROIF )    //RB Overrun Interrupt Flag
#define CANFD_FLAG_RIF          (CANFD_RIF  )    //Receive Interrupt Flag
#define CANFD_FLAG_BEIF         (CANFD_BEIF )    //Bus Error Interrupt Flag
#define CANFD_FLAG_ALIF         (CANFD_ALIF )    //Arbitration Lost Interrupt Flag
#define CANFD_FLAG_EPIF         (CANFD_EPIF )    //Error Passive Interrupt Flag
#define CANFD_FLAG_TTIF         (CANFD_TTIF )    //Time Trigger Interrupt Flag
#define CANFD_FLAG_TEIF         (CANFD_TEIF )    //Trigger Error Interrupt Flag
#define CANFD_FLAG_WTIF         (CANFD_WTIF )    //Watch Trigger Interrupt Flag

#define CANFD_ISR0_FLAG_MASK    (CANFD_FLAG_AIF  | CANFD_FLAG_EIF  | CANFD_FLAG_TSIF | CANFD_FLAG_TPIF |\
                                 CANFD_FLAG_RAFIF| CANFD_FLAG_RFIF | CANFD_FLAG_ROIF | CANFD_FLAG_RIF  |\
                                 CANFD_FLAG_BEIF | CANFD_FLAG_ALIF | CANFD_FLAG_EPIF)
#define CANFD_ISR1_FLAG_MASK    (CANFD_FLAG_TTIF | CANFD_FLAG_TEIF | CANFD_FLAG_WTIF)
#define CANFD_FLAG_ALL          (CANFD_ISR0_FLAG_MASK | CANFD_ISR1_FLAG_MASK)



/* Callbacks Register/UnRegister functions  ***********************************/
#if(defined USE_HAL_CANFD_REGISTER_CALLBACKS && USE_HAL_CANFD_REGISTER_CALLBACKS == 1)
HAL_StatusTypeDef HAL_CANFD_RegisterCallback(CANFD_HandleTypeDef *hcanfd, HAL_CANFD_CallbackIDTypeDef CallbackID, pCANFD_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_CANFD_UnRegisterCallback(CANFD_HandleTypeDef *hcanfd, HAL_CANFD_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_CANFD_REGISTER_CALLBACKS */

/** @defgroup CANFD_Exported_Functions
  * @{
  */
HAL_StatusTypeDef HAL_CANFD_StructInit(CANFD_HandleTypeDef *hcanfd);
HAL_StatusTypeDef HAL_CANFD_Init(CANFD_HandleTypeDef *hcanfd);
HAL_StatusTypeDef HAL_CANFD_DeInit(CANFD_HandleTypeDef *hcanfd);
void HAL_CANFD_MspInit(CANFD_HandleTypeDef *hcanfd);
void HAL_CANFD_MspDeInit(CANFD_HandleTypeDef *hcanfd);

/* Set Baud rate use for 5K/10K/20K/50K/100K/125K *****************************/
HAL_StatusTypeDef HAL_CANFD_SetBaudrate(CANFD_HandleTypeDef *hcanfd, const CANFD_BaudrateTypeDef *baudrate);
HAL_StatusTypeDef HAL_CANFD_SetBaudrateAuto(CANFD_HandleTypeDef *hcanfd, CANFD_Baudrate_Enum slow, CANFD_Baudrate_Enum fast);

/* Configuration functions ****************************************************/
HAL_StatusTypeDef HAL_CANFD_ConfigFilter(CANFD_HandleTypeDef *hcanfd, CANFD_FilterTypeDef *sFilterConfig);
HAL_StatusTypeDef HAL_CANFD_DisableFilter(CANFD_HandleTypeDef *hcanfd, CANFD_FilterTypeDef *sFilterConfig);
/* Control functions **********************************************************/
HAL_StatusTypeDef HAL_CANFD_AutoReTxCtrl(CANFD_HandleTypeDef *hcanfd, CANFD_TBSEL_TypeDef tb, bool enable);
HAL_StatusTypeDef HAL_CANFD_TxPrimaryMessage(CANFD_HandleTypeDef *hcanfd, CANFD_MsgTypeDef *pTxMsg, uint32_t Timeout);
HAL_StatusTypeDef HAL_CANFD_AddTxMessage(CANFD_HandleTypeDef *hcanfd, CANFD_MsgTypeDef *pTxMsg);
HAL_StatusTypeDef HAL_CANFD_AbortTxPrimaryRequest(CANFD_HandleTypeDef *hcanfd);
HAL_StatusTypeDef HAL_CANFD_AbortTxSecondaryRequest(CANFD_HandleTypeDef *hcanfd);
HAL_StatusTypeDef HAL_CANFD_GetRxMessage(CANFD_HandleTypeDef *hcanfd, CANFD_MsgTypeDef *pRxMsg, uint32_t Timeout);
HAL_StatusTypeDef HAL_CANFD_GetRxMessage_IT(CANFD_HandleTypeDef *hcanfd, CANFD_MsgTypeDef *pRxMsg);

static inline void HAL_CANFD_StartTransmit(CANFD_HandleTypeDef *hcanfd)
{
    SET_BIT(hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value, CANFD_TSONE);
}
/** @addtogroup CANFD_Exported_Functions_Group4 Interrupts management
 *  @brief    Interrupts management
 * @{
 */
/* Interrupts management ******************************************************/
void HAL_CANFD_IT_Enable(CANFD_HandleTypeDef *hcanfd, uint32_t interrupt);
void HAL_CANFD_IT_Disable(CANFD_HandleTypeDef *hcanfd, uint32_t interrupt);
uint32_t HAL_CANFD_GetITFlag(CANFD_HandleTypeDef *hcanfd, uint32_t isrflag);
void HAL_CANFD_ClearITFlag(CANFD_HandleTypeDef *hcanfd, uint32_t isrflag);
void HAL_CANFD_IRQHandler(CANFD_HandleTypeDef *hcanfd);

/* Callbacks functions ********************************************************/
void HAL_CANFD_RxMsgCallback(CANFD_HandleTypeDef *hcanfd);
void HAL_CANFD_RxBuffOverrunCallback(CANFD_HandleTypeDef *hcanfd);
void HAL_CANFD_RxBuffFullCallback(CANFD_HandleTypeDef *hcanfd);
void HAL_CANFD_RxBuffAlmostFullCallback(CANFD_HandleTypeDef *hcanfd);
void HAL_CANFD_TxPriClptCallback(CANFD_HandleTypeDef *hcanfd);
void HAL_CANFD_TxSecClptCallback(CANFD_HandleTypeDef *hcanfd);
void HAL_CANFD_ErrorCallback(CANFD_HandleTypeDef *hcanfd);
void HAL_CANFD_AbortCallback(CANFD_HandleTypeDef *hcanfd);
void HAL_CANFD_PassiveErrorCallback(CANFD_HandleTypeDef *hcanfd);
void HAL_CANFD_ArbitrationLostCallback(CANFD_HandleTypeDef *hcanfd);
void HAL_CANFD_BusErrorCallback(CANFD_HandleTypeDef *hcanfd);
/**  Receive buffer STATus
  *  @param  hcanfd pointer to a CAN_HandleTypeDef structure that contains
  *          the configuration information for the specified CANFD.
  *  @return B00 - empty
  *          B01 - > empty and < almost full (AFWL)
  *          B10 - > almost full (programmable threshold by AFWL) but not full and no overflow
  *          B11 - full (stays set in case of overflow - for overflow signaling see ROV)
  */
static inline uint32_t HAL_CANFD_GetRxBuffStatus(CANFD_HandleTypeDef *hcanfd)
{
    uint32_t tempreg = hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value;
    return (((union _CANFD_RegA0 *)&tempreg)->bits.RCTRL & 0x3);
}
void HAL_CANFD_SetErrorWarningLimit(CANFD_HandleTypeDef *hcanfd, uint8_t EWL);
void HAL_CANFD_SetAlmostFullWarningLimit(CANFD_HandleTypeDef *hcanfd, uint8_t AFWL);


void HAL_CANFD_SetMode(CANFD_HandleTypeDef *hcanfd, CANFD_Mode_Enum mode);
void HAL_CANFD_SetISOMode(CANFD_HandleTypeDef *hcanfd, uint16_t en);

#endif/* HAL_CANFD_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC90XX_HAL_CANFD_H__ */

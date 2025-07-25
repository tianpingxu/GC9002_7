/**
  ******************************************************************************
  * @file    gc90xx_hal_i2c.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2022-10-18
  * @brief   This file contains all the functions prototypes for the I2C
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC90XX_HAL_I2C_H__

#define __GC90XX_HAL_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <gc90xx_hal_def.h>

#ifdef HAL_I2C_MODULE_ENABLED

/** @defgroup I2C_Configuration_Structure_definition I2C Configuration Structure definition
  * @brief  I2C Configuration Structure definition
  * @{
  */
typedef struct
{
    uint32_t Speed          : 2;    /*!< Specifies the clock frequency.
                                            This parameter can be a value of @ref I2C_SpeedSelect */
    uint32_t GeneralCallEn  : 1;    /*!< Specifies enable general call.
                                            This parameter can be a value of @ref I2C_DISABLE/I2C_ENABLE */
    uint32_t ReStartEn      : 1;    /*!< Specifies enable RESTART.
                                            This parameter can be a value of @ref I2C_DISABLE/I2C_ENABLE */
    uint32_t AddressMode    : 1;    /*!< Specifies if 7-bit or 10-bit address mode is selected.
                                            This parameter can be a value of @ref I2C_AddrMode */
    uint32_t OwnAddress1    : 10;   /*!< Specifies the first device own address.
                                            This parameter can be a 7-bit or 10-bit address. */
    uint32_t OwnAddress2    : 7;    /*!< Optional Slave address for DW_apb_i2c when operating as a slave in SMBus Mode.
                                            This parameter can be a 7-bit address. */
    uint32_t                : 10;
    
    uint16_t ss_hcnt;   /*!< standard speed HCNT value, This parameter can be 0~65535. */
    uint16_t ss_lcnt;   /*!< standard speed LCNT value, This parameter can be 0~65535. */
    
    uint16_t fs_hcnt;   /*!< fast speed HCNT value, This parameter can be 0~65535. */
    uint16_t fs_lcnt;   /*!< fast speed LCNT value, This parameter can be 0~65535. */
                                    
    uint16_t fp_hcnt;   /*!< fast plus HCNT value, This parameter can be 0~65535. */
    uint16_t fp_lcnt;   /*!< fast plus LCNT value, This parameter can be 0~65535. */
    
    uint16_t hs_hcnt;   /*!< high speed HCNT value, This parameter can be 0~65535. */
    uint16_t hs_lcnt;   /*!< high speed LCNT value, This parameter can be 0~65535. */
    
    uint16_t rx_sda_hold_time;  /*!< Specifies the Sets the required SDA hold time in units of ic_clk period when DW_apb_i2c acts as a receiver.
                                    This parameter can be 0~255. */
    uint16_t tx_sda_hold_time;  /*!< Specifies the Sets the required SDA hold time in units of ic_clk period when DW_apb_i2c acts as a transmitter.
                                    This parameter can be 0~65535. */
    uint32_t sda_fall_ns;       
    uint32_t scl_fall_ns;
    
    uint8_t tx_fifo_threshold;  /*!< Specifies the Transmit FIFO Threshold Level.
                                        This parameter can be 0~255.
                                        Hardware does not allow this value to be set to a value larger than the depth of the buffer */
    uint8_t rx_fifo_threshold;  /*!< Specifies the Receive FIFO Threshold Level.
                                        This parameter can be 0~255.
                                        Hardware does not allow this value to be set to a value larger than the depth of the buffer
                                        eg: rx_len%(rx_fifo_threshold+1) must be equal to 0,
                                        if receive 8 bytes data, rx_fifo_threshold can set 0/1/3/7 */
    uint8_t tx_fifo_depth;      /*!< Specifies the Transmit FIFO depth. This parameter can be 0~255. */
    uint8_t rx_fifo_depth;      /*!< Specifies the Receive FIFO depth. This parameter can be 0~255. */
} I2C_InitTypeDef;

enum
{
     I2C_DISABLE = 0x0U,
     I2C_ENABLE
};
/** @brief  I2C_SpeedSelect
 */
enum I2C_SpeedSelect
{
    I2C_Speed_FastPlus = 0,     //fast mode plus (<=1000Kbit/s),
    I2C_Speed_Standard = 0x1U,  //100 kbit/s
    I2C_Speed_Fast = 0x2U,      //fast mode (<=400 kbit/s)
    I2C_Speed_High = 0x3U,      //3.4 Mbit/s
};
/** @defgroup I2C_AddrMode I2C address mode
  */
typedef enum
{
    I2C_AddrMode_7BIT = 0x0U,
    I2C_AddrMode_10BIT
}I2C_AddrModeType;
/** @defgroup I2C_SlaveDisableSel
  */
enum I2C_SlaveDisableSel
{
    I2C_SlaveEnable = 0x0U,
    I2C_SlaveDisable
};
/** @defgroup I2C_CmdType
  */
enum I2C_CmdType
{
    I2C_General_Call_Address = 0x0U,
    I2C_Start_Bit
};
/** @defgroup I2C_Cmd
  */
enum I2C_Cmd
{
    I2C_CMD_Write = 0x0U,
    I2C_CMD_Read
};
typedef enum
{
    I2C_Mode_Master = 0x0U,
    I2C_Mode_Slave,
}I2C_BusModeType;

enum I2C_CMDType
{
    I2C_CMD_Master_Write = 0x0U,
    I2C_CMD_Master_Read,
};
/**
  * @}
  */

/** @defgroup HAL_state_structure_definition HAL state structure definition
  * @brief  HAL State structure definition
  * @note  HAL I2C State value coding follow below described bitmap :
  *          b7-b6  Error information
  *             00 : No Error
  *             01 : Abort (Abort user request on going)
  *             10 : Timeout
  *             11 : Error
  *          b5     Peripheral initilisation status
  *             0  : Reset (Peripheral not initialized)
  *             1  : Init done (Peripheral initialized and ready to use. HAL I2C Init function called)
  *          b4     (not used)
  *             x  : Should be set to 0
  *          b3
  *             0  : Ready or Busy (No Listen mode ongoing)
  *             1  : Listen (Peripheral in Address Listen Mode)
  *          b2     Intrinsic process state
  *             0  : Ready
  *             1  : Busy (Peripheral busy with some configuration or internal operations)
  *          b1     Rx state
  *             0  : Ready (no Rx operation ongoing)
  *             1  : Busy (Rx operation ongoing)
  *          b0     Tx state
  *             0  : Ready (no Tx operation ongoing)
  *             1  : Busy (Tx operation ongoing)
  * @{
  */
typedef enum
{
  HAL_I2C_STATE_RESET             = 0x00U,   /*!< Peripheral is not yet Initialized         */
  HAL_I2C_STATE_READY             = 0x20U,   /*!< Peripheral Initialized and ready for use  */
  HAL_I2C_STATE_BUSY              = 0x24U,   /*!< An internal process is ongoing            */
  HAL_I2C_STATE_DMA_LISTEN        = 0x11U,
  HAL_I2C_STATE_DMA_END           = 0x10U,
  HAL_I2C_STATE_BUSY_TX           = 0x21U,   /*!< Data Transmission process is ongoing      */
  HAL_I2C_STATE_BUSY_RX           = 0x22U,   /*!< Data Reception process is ongoing         */
  HAL_I2C_STATE_LISTEN            = 0x28U,   /*!< Address Listen Mode is ongoing            */
  HAL_I2C_STATE_BUSY_TX_LISTEN    = 0x29U,   /*!< Address Listen Mode and Data Transmission
                                                 process is ongoing                         */
  HAL_I2C_STATE_BUSY_RX_LISTEN    = 0x2AU,   /*!< Address Listen Mode and Data Reception
                                                 process is ongoing                         */
  HAL_I2C_STATE_ABORT             = 0x60U,   /*!< Abort user request ongoing                */
  HAL_I2C_STATE_TIMEOUT           = 0xA0U,   /*!< Timeout state                             */
  HAL_I2C_STATE_ERROR             = 0xE0U    /*!< Error                                     */

} HAL_I2C_StateTypeDef;


/** @defgroup HAL_mode_structure_definition HAL mode structure definition
  * @brief  HAL Mode structure definition
  * @note  HAL I2C Mode value coding follow below described bitmap :\n
  *          b7     (not used)\n
  *             x  : Should be set to 0\n
  *          b6\n
  *             0  : None\n
  *             1  : Memory (HAL I2C communication is in Memory Mode)\n
  *          b5\n
  *             0  : None\n
  *             1  : Slave (HAL I2C communication is in Slave Mode)\n
  *          b4\n
  *             0  : None\n
  *             1  : Master (HAL I2C communication is in Master Mode)\n
  *          b3-b2-b1-b0  (not used)\n
  *             xxxx : Should be set to 0000
  * @{
  */
typedef enum
{
    HAL_I2C_MODE_NONE               = 0x00U,   /*!< No I2C communication on going             */
    HAL_I2C_MODE_MASTER             = 0x10U,   /*!< I2C communication is in Master Mode       */
    HAL_I2C_MODE_SLAVE              = 0x20U,   /*!< I2C communication is in Slave Mode        */
    HAL_I2C_MODE_MEM                = 0x40U    /*!< I2C communication is in Memory Mode       */
}HAL_I2C_ModeTypeDef;

/** @defgroup I2C_Error_Code_definition I2C Error Code definition
  * @brief  I2C Error Code definition
  * @{
  */
#define HAL_I2C_ERROR_NONE              0x00000000U    /*!< No error              */
#define HAL_I2C_ERROR_TX_ABRT           0x00000001U    /*!< TX_ABRT error         */
#define HAL_I2C_ERROR_ARLO              0x00000002U    /*!< ARLO error            */
#define HAL_I2C_ERROR_AF                0x00000004U    /*!< AF error              */
#define HAL_I2C_ERROR_OVR               0x00000008U    /*!< OVR error             */
#define HAL_I2C_ERROR_DMA               0x00000010U    /*!< DMA transfer error    */
#define HAL_I2C_ERROR_TIMEOUT           0x00000020U    /*!< Timeout Error         */
#define HAL_I2C_ERROR_SIZE              0x00000040U    /*!< Size Management error */
#define HAL_I2C_ERROR_DMA_PARAM         0x00000080U    /*!< DMA Parameter Error   */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
#define HAL_I2C_ERROR_INVALID_CALLBACK  0x00000100U    /*!< Invalid Callback error */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

#define STATUS_WRITE_IN_PROGRESS        0x000000001U
#define STATUS_READ_IN_PROGRESS         0x000000002U

#define I2C_MEM_STAGE_START             0x0U
#define I2C_MEM_STAGE_ADDR_MSB          0x1U
#define I2C_MEM_STAGE_ADDR_LSB          0x2U
#define I2C_MEM_STAGE_ADDR_READ         0x3U
/** @defgroup I2C_handle_Structure_definition I2C handle Structure definition
  * @brief  I2C handle Structure definition
  * @{
  */
typedef struct __I2C_HandleTypeDef
{
    I2C_TypeDef                *Instance;      /*!< I2C registers base address               */
    I2C_InitTypeDef            Init;           /*!< I2C communication parameters             */
    uint32_t                   flags;          /*!< I2C reg access flag                      */
    uint8_t                    *pBuffPtr;      /*!< Pointer to I2C transfer buffer           */
    uint8_t                    *pRxBuffPtr;    /*!< Pointer to Rx buffer                     */
    uint16_t                   cmd_cnt;        /*!< Tx cmd count                             */
    uint16_t                   cmd_max;        /*!< Tx cmd max                               */

    uint16_t                   XferSize;       /*!< I2C transfer size                        */
    __IO uint16_t              XferCount;      /*!< I2C transfer counter                     */
    __IO uint32_t              XferOptions;    /*!< I2C transfer options                     */
    __IO uint32_t              PreviousState;  /*!< I2C communication Previous state and mode
                                                   context for internal usage                */
    uint32_t                   gstatus;
    uint32_t                   tx_abort_source;
    DMA_HandleTypeDef          *hdmatx;        /*!< I2C Tx DMA handle parameters             */
    DMA_HandleTypeDef          *hdmarx;        /*!< I2C Rx DMA handle parameters             */
    HAL_LockTypeDef            Lock;           /*!< I2C locking object                       */
    __IO HAL_I2C_StateTypeDef  State;          /*!< I2C communication state                  */
    __IO HAL_I2C_StateTypeDef  DMAState;       /*!< I2C communication state                  */
    __IO HAL_I2C_ModeTypeDef   Mode;           /*!< I2C communication mode                   */
    __IO uint32_t              ErrorCode;      /*!< I2C Error code                           */
    __IO uint32_t              Devaddress;     /*!< I2C Target device address                */
    __IO uint32_t              Memaddress;     /*!< I2C Target memory address                */
    __IO uint32_t              MemaddSize;     /*!< I2C Target memory address  size          */
    __IO uint32_t              MemStage;       /*!< I2C memory transfer stage                */
    void *                     priv_obj;
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
    void (* MasterTxCpltCallback)(struct __I2C_HandleTypeDef *hi2c);           /*!< I2C Master Tx Transfer completed callback */
    void (* MasterRxCpltCallback)(struct __I2C_HandleTypeDef *hi2c);           /*!< I2C Master Rx Transfer completed callback */
    void (* SlaveTxCpltCallback)(struct __I2C_HandleTypeDef *hi2c);            /*!< I2C Slave Tx Transfer completed callback  */
    void (* SlaveRxCpltCallback)(struct __I2C_HandleTypeDef *hi2c);            /*!< I2C Slave Rx Transfer completed callback  */
    void (* ListenCpltCallback)(struct __I2C_HandleTypeDef *hi2c);             /*!< I2C Listen Complete callback              */
    void (* MemTxCpltCallback)(struct __I2C_HandleTypeDef *hi2c);              /*!< I2C Memory Tx Transfer completed callback */
    void (* MemRxCpltCallback)(struct __I2C_HandleTypeDef *hi2c);              /*!< I2C Memory Rx Transfer completed callback */
    void (* ErrorCallback)(struct __I2C_HandleTypeDef *hi2c);                  /*!< I2C Error callback                        */
    void (* AbortCpltCallback)(struct __I2C_HandleTypeDef *hi2c);              /*!< I2C Abort callback                        */

    void (* AddrCallback)(struct __I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode);  /*!< I2C Slave Address Match callback */

    void (* MspInitCallback)(struct __I2C_HandleTypeDef *hi2c);                /*!< I2C Msp Init callback                     */
    void (* MspDeInitCallback)(struct __I2C_HandleTypeDef *hi2c);              /*!< I2C Msp DeInit callback                   */
#endif  /* USE_HAL_I2C_REGISTER_CALLBACKS */
} I2C_HandleTypeDef;

/**
  * @brief  HAL I2C Callback ID enumeration definition
  */
typedef enum
{
    HAL_I2C_MASTER_TX_COMPLETE_CB_ID      = 0x00U,    /*!< I2C Master Tx Transfer completed callback ID  */
    HAL_I2C_MASTER_RX_COMPLETE_CB_ID      = 0x01U,    /*!< I2C Master Rx Transfer completed callback ID  */
    HAL_I2C_SLAVE_TX_COMPLETE_CB_ID       = 0x02U,    /*!< I2C Slave Tx Transfer completed callback ID   */
    HAL_I2C_SLAVE_RX_COMPLETE_CB_ID       = 0x03U,    /*!< I2C Slave Rx Transfer completed callback ID   */
    HAL_I2C_LISTEN_COMPLETE_CB_ID         = 0x04U,    /*!< I2C Listen Complete callback ID               */
    HAL_I2C_MEM_TX_COMPLETE_CB_ID         = 0x05U,    /*!< I2C Memory Tx Transfer callback ID            */
    HAL_I2C_MEM_RX_COMPLETE_CB_ID         = 0x06U,    /*!< I2C Memory Rx Transfer completed callback ID  */
    HAL_I2C_ERROR_CB_ID                   = 0x07U,    /*!< I2C Error callback ID                         */
    HAL_I2C_ABORT_CB_ID                   = 0x08U,    /*!< I2C Abort callback ID                         */

    HAL_I2C_MSPINIT_CB_ID                 = 0x09U,    /*!< I2C Msp Init callback ID                      */
    HAL_I2C_MSPDEINIT_CB_ID               = 0x0AU     /*!< I2C Msp DeInit callback ID                    */
}HAL_I2C_CallbackIDTypeDef;

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
/**
  * @brief  HAL I2C Callback pointer definition
  */
typedef  void (*pI2C_CallbackTypeDef)(I2C_HandleTypeDef *hi2c); /*!< pointer to an I2C callback function */
typedef  void (*pI2C_AddrCallbackTypeDef)(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode); /*!< pointer to an I2C Address Match callback function */

#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

/* Exported constants --------------------------------------------------------*/

#define I2C_COMP_TYPE_VALUE         0x44570140
#define I2C_SDA_HOLD_MIN_VERS       0x3131312A

#define I2C_ACCESS_SWAP             0x00000001
#define I2C_ACCESS_16BIT            0x00000002
#define I2C_ACCESS_INTR_MASK        0x00000004
#define I2C_ACCESS_NO_IRQ_SUSPEND   0x00000008
/** @defgroup I2C_general_call_addressing_mode I2C general call addressing mode
  * @{
  */
#define I2C_GENERALCALL_DISABLE        0x00000000U
#define I2C_GENERALCALL_ENABLE         I2C_CR1_ENGC
/**
  * @}
  */

/** @defgroup I2C_nostretch_mode I2C nostretch mode
  * @{
  */
#define I2C_NOSTRETCH_DISABLE          0x00000000U
#define I2C_NOSTRETCH_ENABLE           I2C_CR1_NOSTRETCH
/**
  * @}
  */

/** @defgroup I2C_Memory_Address_Size I2C Memory Address Size
  * @{
  */
#define I2C_MEMADD_SIZE_8BIT            0x00000001U
#define I2C_MEMADD_SIZE_16BIT           0x00000010U
/**
  * @}
  */

/** @defgroup I2C_XferDirection_definition I2C XferDirection definition
  * @{
  */
#define I2C_DIRECTION_RECEIVE           0x00000000U
#define I2C_DIRECTION_TRANSMIT          0x00000001U
/**
  * @}
  */

/** @defgroup I2C_XferOptions_definition I2C XferOptions definition
  * @{
  */
#define  I2C_FIRST_FRAME                0x00000001U
#define  I2C_FIRST_AND_NEXT_FRAME       0x00000002U
#define  I2C_NEXT_FRAME                 0x00000004U
#define  I2C_FIRST_AND_LAST_FRAME       0x00000008U
#define  I2C_LAST_FRAME_NO_STOP         0x00000010U
#define  I2C_LAST_FRAME                 0x00000020U

/* List of XferOptions in usage of :
 * 1- Restart condition in all use cases (direction change or not)
 */
#define  I2C_OTHER_FRAME                (0x00AA0000U)
#define  I2C_OTHER_AND_LAST_FRAME       (0xAA000000U)
/**
  * @}
  */

/** @defgroup I2C_Interrupt_configuration_definition I2C Interrupt configuration definition
  * @brief I2C Interrupt definition
  *        Elements values convention: 0xXXXXXXXX
  *           - XXXXXXXX  : Interrupt control mask
  * @{
  */
#define I2C_IT_RX_UNDER                 I2C_ISR_RX_UNDER
#define I2C_IT_RX_OVER                  I2C_ISR_RX_OVER
#define I2C_IT_RX_FULL                  I2C_ISR_RX_FULL
#define I2C_IT_TX_OVER                  I2C_ISR_TX_OVER
#define I2C_IT_TX_EMPTY                 I2C_ISR_TX_EMPTY
#define I2C_IT_RD_REQ                   I2C_ISR_RD_REQ
#define I2C_IT_TX_ABRT                  I2C_ISR_TX_ABRT
#define I2C_IT_RX_DONE                  I2C_ISR_RX_DONE
#define I2C_IT_ACTIVITY                 I2C_ISR_ACTIVITY
#define I2C_IT_STOP_DET                 I2C_ISR_STOP_DET
#define I2C_IT_START_DET                I2C_ISR_START_DET
#define I2C_IT_GEN_CALL                 I2C_ISR_GEN_CALL
#define I2C_IT_RESTART_DET              I2C_ISR_RESTART_DET
#define I2C_IT_MASTER_ON_HOLD           I2C_ISR_MASTER_ON_HOLD
#define I2C_IT_SCL_STUCK_AT_LOW         I2C_ISR_SCL_STUCK_AT_LOW

#define I2C_IT_DEFAULT_MASK             (I2C_IT_RX_FULL | \
                                         I2C_IT_TX_ABRT | \
                                         I2C_IT_RX_DONE | \
                                         I2C_IT_STOP_DET)
#define I2C_IT_MASTER_MASK              (I2C_IT_DEFAULT_MASK | \
                                         I2C_IT_TX_EMPTY)
#define I2C_IT_SLAVE_MASK               (I2C_IT_DEFAULT_MASK | \
                                         I2C_IT_RX_UNDER | \
                                         I2C_IT_RD_REQ)
/** @defgroup I2C_Flag_definition I2C Flag definition
  * @{
  */
#define I2C_FLAG_RX_UNDER               I2C_IT_RX_UNDER
#define I2C_FLAG_RX_OVER                I2C_IT_RX_OVER
#define I2C_FLAG_RX_FULL                I2C_IT_RX_FULL
#define I2C_FLAG_TX_OVER                I2C_IT_TX_OVER
#define I2C_FLAG_TX_EMPTY               I2C_IT_TX_EMPTY
#define I2C_FLAG_RD_REQ                 I2C_IT_RD_REQ
#define I2C_FLAG_TX_ABRT                I2C_IT_TX_ABRT
#define I2C_FLAG_RX_DONE                I2C_IT_RX_DONE
#define I2C_FLAG_ACTIVITY               I2C_IT_ACTIVITY
#define I2C_FLAG_STOP_DET               I2C_IT_STOP_DET
#define I2C_FLAG_START_DET              I2C_IT_START_DET
#define I2C_FLAG_GEN_CALL               I2C_IT_GEN_CALL
#define I2C_FLAG_RESTART_DET            I2C_IT_RESTART_DET
#define I2C_FLAG_MASTER_ON_HOLD         I2C_IT_MASTER_ON_HOLD
#define I2C_FLAG_SCL_STUCK_AT_LOW       I2C_IT_SCL_STUCK_AT_LOW

#define I2C_FLAG_ALL                    (0x7FFF)

/** @defgroup I2C_Status_definition I2C Status definition
  * @{
  */
#define I2C_STATUS_ACTIVITY                     I2C_SR_ACTIVITY
#define I2C_STATUS_TFNF                         I2C_SR_TFNF
#define I2C_STATUS_TFE                          I2C_SR_TFE
#define I2C_STATUS_RFNE                         I2C_SR_RFNE
#define I2C_STATUS_RFF                          I2C_SR_RFF
#define I2C_STATUS_MST_ACTIVITY                 I2C_SR_MST_ACTIVITY
#define I2C_STATUS_SLV_ACTIVITY                 I2C_SR_SLV_ACTIVITY
#define I2C_STATUS_MST_HOLD_TX_FIFO_EMPTY       I2C_SR_MST_HOLD_TX_FIFO_EMPTY
#define I2C_STATUS_MST_HOLD_RX_FIFO_FULL        I2C_SR_MST_HOLD_RX_FIFO_FULL
#define I2C_STATUS_SLV_HOLD_TX_FIFO_EMPTY       I2C_SR_SLV_HOLD_TX_FIFO_EMPTY
#define I2C_STATUS_SLV_HOLD_RX_FIFO_FULL        I2C_SR_SLV_HOLD_RX_FIFO_FULL
#define I2C_STATUS_SDA_STUCK_NOT_RECOVERED      I2C_SR_SDA_STUCK_NOT_RECOVERED
#define I2C_STATUS_SMBUS_QUICK_CMD_BIT          I2C_SR_SMBUS_QUICK_CMD_BIT
#define I2C_STATUS_SMBUS_SLAVE_ADDR_VALID       I2C_SR_SMBUS_SLAVE_ADDR_VALID
#define I2C_STATUS_SMBUS_SLAVE_ADDR_RESOLVED    I2C_SR_SMBUS_SLAVE_ADDR_RESOLVED
#define I2C_STATUS_SMBUS_SUSPEND_STATUS         I2C_SR_SMBUS_SUSPEND_STATUS
#define I2C_STATUS_SMBUS_ALERT_STATUS           I2C_SR_SMBUS_ALERT_STATUS

#define I2C_TX_ABRT_NOACK		  (I2C_TX_ABRT_SOURCE_7B_ADDR_NOACK | \
                               I2C_TX_ABRT_SOURCE_10ADDR1_NOACK | \
                               I2C_TX_ABRT_SOURCE_10ADDR2_NOACK | \
                               I2C_TX_ABRT_SOURCE_TXDATA_NOACK | \
                               I2C_TX_ABRT_SOURCE_GCALL_NOACK)
#define I2C_TX_ARB_LOST         I2C_TX_ABRT_SOURCE_LOST
#define I2C_TX_ABRT_GCALL_READ  I2C_TX_ABRT_SOURCE_GCALL_READ
/* Exported macros -----------------------------------------------------------*/

/** @defgroup I2C_Exported_Macros I2C Exported Macros
  * @{
  */
/** @brief  Enable or disable the specified I2C Transmit DMA Mode.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @retval None
  */
#define __HAL_I2C_DMA_TX_ENABLE(__HANDLE__)                     SET_BIT((__HANDLE__)->Instance->DMA_CR, I2C_DMA_CR_TDMAE)
#define __HAL_I2C_DMA_TX_DISABLE(__HANDLE__)                    CLEAR_BIT((__HANDLE__)->Instance->DMA_CR, I2C_DMA_CR_TDMAE)
#define __HAL_I2C_DMA_RX_ENABLE(__HANDLE__)                     SET_BIT((__HANDLE__)->Instance->DMA_CR, I2C_DMA_CR_RDMAE)
#define __HAL_I2C_DMA_RX_DISABLE(__HANDLE__)                    CLEAR_BIT((__HANDLE__)->Instance->DMA_CR, I2C_DMA_CR_RDMAE)
/** @brief  Enable or disable the specified I2C interrupts.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __INTERRUPT__ specifies the interrupt source to enable or disable.
  *         This parameter can be one of the following values:
  *            @arg I2C_IT_RX_UNDER            Receive buffer underflow interrupt.
  *            @arg I2C_IT_RX_OVER             Receive buffer overflow interrupt state.
  *            @arg I2C_IT_RX_FULL             Receive buffer full interrupt state.
  *            @arg I2C_IT_TX_OVER             Transmit buffer overflow interrupt state.
  *            @arg I2C_IT_TX_EMPTY            Transmit buffer empty interrupt state.
  *            @arg I2C_IT_RD_REQ              Slave read request interrupt state.
  *            @arg I2C_IT_TX_ABRT             Transmit abort interrupt. state.
  *            @arg I2C_IT_RX_DONE             Receive done interrupt state.
  *            @arg I2C_IT_ACTIVITY            I2C activity interrupt state.
  *            @arg I2C_IT_STOP_DET            Stop condition detect on I2C interrupt state.
  *            @arg I2C_IT_START_DET           Start condition detect on I2C interrupt state.
  *            @arg I2C_IT_GEN_CALL            General Call received interrupt state.
  *            @arg I2C_IT_RESTART_DET         Restart condition detect on I2C interrupt state.
  *            @arg I2C_IT_MASTER_ON_HOLD      Master on hold I2C interrupt state.
  *            @arg I2C_IT_SCL_STUCK_AT_LOW    SCL Stuck condition detect on I2C interrupt state.
  * @retval None
  */
#define __HAL_I2C_ENABLE_IT(__HANDLE__, __INTERRUPT__)   SET_BIT((__HANDLE__)->Instance->IMR, (__INTERRUPT__))
#define __HAL_I2C_DISABLE_IT(__HANDLE__, __INTERRUPT__)  CLEAR_BIT((__HANDLE__)->Instance->IMR, (__INTERRUPT__))
#define __HAL_I2C_DISABLE_IT_ALL(__HANDLE__)             ((__HANDLE__)->Instance->IMR = 0)

/** @brief  Get Flag of the specified I2C interrupts.
  * @param  __HANDLE__ specifies the I2C Handle.
 * @param  __FLAG__    specifies the interrupt source.
  *         This parameter can be one of the following values:
  *            @arg I2C_FLAG_RX_UNDER            Receive buffer underflow interrupt.
  *            @arg I2C_FLAG_RX_OVER             Receive buffer overflow interrupt state.
  *            @arg I2C_FLAG_RX_FULL             Receive buffer full interrupt state.
  *            @arg I2C_FLAG_TX_OVER             Transmit buffer overflow interrupt state.
  *            @arg I2C_FLAG_TX_EMPTY            Transmit buffer empty interrupt state.
  *            @arg I2C_FLAG_RD_REQ              Slave read request interrupt state.
  *            @arg I2C_FLAG_TX_ABRT             Transmit abort interrupt. state.
  *            @arg I2C_FLAG_RX_DONE             Receive done interrupt state.
  *            @arg I2C_FLAG_ACTIVITY            I2C activity interrupt state.
  *            @arg I2C_FLAG_STOP_DET            Stop condition detect on I2C interrupt state.
  *            @arg I2C_FLAG_START_DET           Start condition detect on I2C interrupt state.
  *            @arg I2C_FLAG_GEN_CALL            General Call received interrupt state.
  *            @arg I2C_FLAG_RESTART_DET         Restart condition detect on I2C interrupt state.
  *            @arg I2C_FLAG_MASTER_ON_HOLD      Master on hold I2C interrupt state.
  *            @arg I2C_FLAG_SCL_STUCK_AT_LOW    SCL Stuck condition detect on I2C interrupt state.
  * @retval None
  */
#define __HAL_I2C_GET_IT_FLAG(__HANDLE__, __FLAG__) (((__HANDLE__)->Instance->ISR & (__FLAG__)) != RESET)
#define __HAL_I2C_GET_FLAG(__HANDLE__, __FLAG__) (((__HANDLE__)->Instance->RISR & (__FLAG__)) != RESET)

/** @brief  Get Flag of the specified I2C interrupts.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @param  __FLAG__    specifies the interrupt source.
  *         This parameter can be one of the following values:
  *            @arg I2C_STATUS_ACTIVITY                     I2C Activity Status.
  *            @arg I2C_STATUS_TFNF                         Transmit FIFO Not Full.
  *            @arg I2C_STATUS_TFE                          Transmit FIFO is empty.
  *            @arg I2C_STATUS_RFNE                         Receive FIFO Not Empty.
  *            @arg I2C_STATUS_RFF                          Receive FIFO is full.
  *            @arg I2C_STATUS_MST_ACTIVITY                 Master Activity.
  *            @arg I2C_STATUS_SLV_ACTIVITY                 Slave Activity.
  *            @arg I2C_STATUS_MST_HOLD_TX_FIFO_EMPTY       Master holds the bus due to Tx FIFO is empty.
  *            @arg I2C_STATUS_MST_HOLD_RX_FIFO_FULL        Master holds the bus due to Rx FIFO is full.
  *            @arg I2C_STATUS_SLV_HOLD_TX_FIFO_EMPTY       Slave holds the bus due to Tx FIFO is empty.
  *            @arg I2C_STATUS_SLV_HOLD_RX_FIFO_FULL        Slave holds the bus due to Rx FIFO is full.
  *            @arg I2C_STATUS_SDA_STUCK_NOT_RECOVERED      SDA Stuck at low is recovered after recovery mechanism.
  *            @arg I2C_STATUS_SMBUS_QUICK_CMD_BIT          SMBUS QUICK CMD Read/write is set.
  *            @arg I2C_STATUS_SMBUS_SLAVE_ADDR_VALID       SMBUS Slave Address is Valid.
  *            @arg I2C_STATUS_SMBUS_SLAVE_ADDR_RESOLVED    SMBUS Slave Address is Resolved.
  *            @arg I2C_STATUS_SMBUS_SUSPEND_STATUS         SMBUS System is in Suspended mode.
  *            @arg I2C_STATUS_SMBUS_ALERT_STATUS           SMBUS Alert is asserted.
  * @retval None
  */
#define __HAL_I2C_GET_STATUS(__HANDLE__, __STATUS__)    ((__HANDLE__)->Instance->SR & (__STATUS__))

/** @brief  Clear the combined interrupt,all individual interrupts and the IC_TX_ABRT_SOURCE register.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @retval None
  */
#define __HAL_CLEAR_INTR(__HANDLE__)            \
do{                                             \
    uint32_t tempreg = 0;                       \
    tempreg = (__HANDLE__)->Instance->CLR_INTR; \
}while(0);
/** @brief  Clear the RX_UNDER interrupt.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @retval None
  */
#define __HAL_CLEAR_RX_UNDER(__HANDLE__)            \
do{                                                 \
    uint32_t tempreg = 0;                           \
    tempreg = (__HANDLE__)->Instance->CLR_RX_UNDER; \
}while(0);
/** @brief  Clear the RX_OVER interrupt.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @retval None
  */
#define __HAL_CLEAR_RX_OVER(__HANDLE__)           \
do{                                               \
    uint32_t tempreg = 0;                         \
    tempreg = (__HANDLE__)->Instance->CLR_RX_OVER;\
}while(0);
/** @brief  Clear the TX_OVER interrupt.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @retval None
  */
#define __HAL_CLEAR_TX_OVER(__HANDLE__)           \
do{                                               \
    uint32_t tempreg = 0;                         \
    tempreg = (__HANDLE__)->Instance->CLR_TX_OVER;\
}while(0);
/** @brief  Clear the RD_REQ interrupt.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @retval None
  */
#define __HAL_CLEAR_RD_REQ(__HANDLE__)            \
do{                                               \
    uint32_t tempreg = 0;                         \
    tempreg = (__HANDLE__)->Instance->CLR_RD_REQ; \
}while(0);
/** @brief  Clear the TX_ABRT interrupt.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @retval None
  */
#define __HAL_CLEAR_TX_ABRT(__HANDLE__)           \
do{                                               \
    uint32_t tempreg = 0;                         \
    tempreg = (__HANDLE__)->Instance->CLR_TX_ABRT;\
}while(0);
/** @brief  Clear the RX_DONE interrupt.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @retval None
  */
#define __HAL_CLEAR_RX_DONE(__HANDLE__)           \
do{                                               \
    uint32_t tempreg = 0;                         \
    tempreg = (__HANDLE__)->Instance->CLR_RX_DONE;\
}while(0);
/** @brief  Clear the ACTIVITY interrupt.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @retval None
  */
#define __HAL_CLEAR_ACTIVITY(__HANDLE__)            \
do{                                                 \
    uint32_t tempreg = 0;                           \
    tempreg = (__HANDLE__)->Instance->CLR_ACTIVITY; \
}while(0);
/** @brief  Clear the STOP_DET interrupt.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @retval None
  */
#define __HAL_CLEAR_STOP_DET(__HANDLE__)            \
do{                                                 \
    uint32_t tempreg = 0;                           \
    tempreg = (__HANDLE__)->Instance->CLR_STOP_DET; \
}while(0);
/** @brief  Clear the START_DET interrupt.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @retval None
  */
#define __HAL_CLEAR_START_DET(__HANDLE__)           \
do{                                                 \
    uint32_t tempreg = 0;                           \
    tempreg = (__HANDLE__)->Instance->CLR_START_DET;\
}while(0);
/** @brief  Clear the GEN_CALL interrupt.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @retval None
  */
#define __HAL_CLEAR_GEN_CALL(__HANDLE__)            \
do{                                                 \
    uint32_t tempreg = 0;                           \
    tempreg = (__HANDLE__)->Instance->CLR_GEN_CALL; \
}while(0);

/** @brief  Enable the specified I2C peripheral.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @retval None
  */
#define __HAL_I2C_ENABLE(__HANDLE__)                SET_BIT((__HANDLE__)->Instance->ENR, I2C_ENR_ENABLE)

/** @brief  Disable the specified I2C peripheral.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @retval None
  */
#define __HAL_I2C_DISABLE(__HANDLE__)               CLEAR_BIT((__HANDLE__)->Instance->ENR, I2C_ENR_ENABLE)

/** @brief  Enable I2C Master restart.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @retval None
  */
#define __HAL_I2C_RESTART_ENABLE(__HANDLE__)       SET_BIT((__HANDLE__)->Instance->CR, I2C_CR_RESTART_EN)
/** @brief  Disable I2C Master restart.
  * @param  __HANDLE__ specifies the I2C Handle.
  * @retval None
  */
#define __HAL_I2C_RESTART_DISABLE(__HANDLE__)      CLEAR_BIT((__HANDLE__)->Instance->CR, I2C_CR_RESTART_EN)

/* Include I2C HAL Extension module */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup I2C_Exported_Functions
  * @{
  */
/**
  * @brief  Bus mode select.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  addr_mode Bus type.
  * @retval None
  */
static inline void HAL_I2C_BusMode_Select(I2C_HandleTypeDef *hi2c, I2C_BusModeType bus_mode)
{
    if(bus_mode == I2C_Mode_Master)
    {
        SET_BIT(hi2c->Instance->CR, I2C_CR_SLAVE_DISABLE | I2C_CR_MASTER_MODE);
    }
    else
    {
        CLEAR_BIT(hi2c->Instance->CR, I2C_CR_SLAVE_DISABLE | I2C_CR_MASTER_MODE);
    }
}
/**
  * @brief  Address mode select.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  addr_mode Address type.
  * @retval None
  */
static inline void HAL_I2C_AddrMode_Select(I2C_HandleTypeDef *hi2c, I2C_AddrModeType addr_mode)
{
    if(addr_mode == I2C_AddrMode_7BIT)
    {
        CLEAR_BIT(hi2c->Instance->CR, I2C_CR_10BITADDR_MASTER | I2C_CR_10BITADDR_SLAVE);
    }
    else
    {
        SET_BIT(hi2c->Instance->CR, I2C_CR_10BITADDR_MASTER | I2C_CR_10BITADDR_SLAVE);
    }
}

static inline void HAL_I2C_SetAddress(I2C_HandleTypeDef *hi2c, uint16_t addr)
{
    MODIFY_REG(hi2c->Instance->SAR, I2C_SAR_Msk, addr & 0x3FF);
}
/** @addtogroup I2C_Exported_Functions_Group1 Initialization and de-initialization functions
  * @{
  */
/* Initialization and de-initialization functions******************************/
HAL_StatusTypeDef HAL_I2C_StructInit(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c);

 /* Callbacks Register/UnRegister functions  ***********************************/
 #if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
 HAL_StatusTypeDef HAL_I2C_RegisterCallback(I2C_HandleTypeDef *hi2c, HAL_I2C_CallbackIDTypeDef CallbackID, pI2C_CallbackTypeDef pCallback);
 HAL_StatusTypeDef HAL_I2C_UnRegisterCallback(I2C_HandleTypeDef *hi2c, HAL_I2C_CallbackIDTypeDef CallbackID);

 HAL_StatusTypeDef HAL_I2C_RegisterAddrCallback(I2C_HandleTypeDef *hi2c, pI2C_AddrCallbackTypeDef pCallback);
 HAL_StatusTypeDef HAL_I2C_UnRegisterAddrCallback(I2C_HandleTypeDef *hi2c);
 #endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
 /**
   * @}
   */

 /** @addtogroup I2C_Exported_Functions_Group2 Input and Output operation functions
   * @{
   */
#if(defined BSP_I2C_TXRX_DMA_ENABLED && BSP_I2C_TXRX_DMA_ENABLED == 1U)
#define __I2C_MASTER_TX_USE_DMA    1U
#define __I2C_MASTER_RX_USE_DMA    1U
#define __I2C_SLAVE_TX_USE_DMA     1U
#define __I2C_SLAVE_RX_USE_DMA     1U
#endif
/* IO operation functions  ****************************************************/
/******* Blocking mode: Polling */
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Slave_Transmit(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Slave_Receive(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_IsDeviceReady(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout);

/******* Non-Blocking mode: Interrupt */
HAL_StatusTypeDef HAL_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Slave_Transmit_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Slave_Receive_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Mem_Write_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Mem_Read_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && (I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U))
HAL_StatusTypeDef HAL_I2C_Master_Seq_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
HAL_StatusTypeDef HAL_I2C_Master_Seq_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Transmit_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Receive_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
#endif
HAL_StatusTypeDef HAL_I2C_EnableListen_IT(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_DisableListen_IT(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_Master_Abort_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress);
/******* Non-Blocking mode: DMA */
HAL_StatusTypeDef HAL_I2C_Master_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Master_Receive_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Slave_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Slave_Receive_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Mem_Write_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_I2C_Mem_Read_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);

#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && (I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U))
HAL_StatusTypeDef HAL_I2C_Master_Seq_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
HAL_StatusTypeDef HAL_I2C_Master_Seq_Receive_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Receive_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
#endif
/**
  * @}
  */

/** @addtogroup I2C_IRQ_Handler_and_Callbacks IRQ Handler and Callbacks
 * @{
 */
/******* I2C IRQHandler and Callbacks used in non blocking modes (Interrupt and DMA) */
void HAL_I2C_IRQHandler(I2C_HandleTypeDef *hi2c);

void HAL_I2C_XferCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_DMAXferCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode);
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c);

/** @addtogroup I2C_Exported_Functions_Group3 Peripheral State, Mode and Error functions
  * @{
  */
/* Peripheral State, Mode and Error functions  *********************************/
HAL_I2C_StateTypeDef HAL_I2C_GetState(I2C_HandleTypeDef *hi2c);
HAL_I2C_ModeTypeDef HAL_I2C_GetMode(I2C_HandleTypeDef *hi2c);
uint32_t HAL_I2C_GetError(I2C_HandleTypeDef *hi2c);

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup I2C_Private_Constants I2C Private Constants
  * @{
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup I2C_Private_Macros I2C Private Macros
  * @{
  */
#define I2C_MEM_ADD_MSB(__ADDRESS__)                       ((uint8_t)((uint16_t)(((uint16_t)((__ADDRESS__) & (uint16_t)0xFF00)) >> 8)))
#define I2C_MEM_ADD_LSB(__ADDRESS__)                       ((uint8_t)((uint16_t)((__ADDRESS__) & (uint16_t)0x00FF)))

/** @defgroup I2C_IS_RTC_Definitions I2C Private macros to check input parameters
  * @{
  */
#define IS_I2C_ADDRESS_MODE(ADDRESS)    (((ADDRESS) == I2C_AddrMode_7BIT) || \
                                         ((ADDRESS) == I2C_AddrMode_10BIT))
#define IS_I2C_GENERAL_CALL(CALL)       (((CALL) == I2C_DISABLE) || \
                                         ((CALL) == I2C_ENABLE))
#define IS_I2C_MEMADD_SIZE(SIZE)        (((SIZE) == I2C_MEMADD_SIZE_8BIT) || \
                                         ((SIZE) == I2C_MEMADD_SIZE_16BIT))
#define IS_I2C_SPEED(SPEED)             (((SPEED) == I2C_Speed_Standard) || \
                                         ((SPEED) == I2C_Speed_Fast) || \
                                         ((SPEED) == I2C_Speed_FastPlus) || \
                                         ((SPEED) == I2C_Speed_High))
#define IS_I2C_OWN_ADDRESS1(ADDRESS1)   (((ADDRESS1) & 0xFFFFFC00U) == 0U)
#define IS_I2C_OWN_ADDRESS2(ADDRESS2)   (((ADDRESS2) & 0xFFFFFF80U) == 0U)
#define IS_I2C_TRANSFER_OPTIONS_REQUEST(REQUEST)      (((REQUEST) == I2C_FIRST_FRAME)              || \
                                                       ((REQUEST) == I2C_FIRST_AND_NEXT_FRAME)     || \
                                                       ((REQUEST) == I2C_NEXT_FRAME)               || \
                                                       ((REQUEST) == I2C_FIRST_AND_LAST_FRAME)     || \
                                                       ((REQUEST) == I2C_LAST_FRAME)               || \
                                                       ((REQUEST) == I2C_LAST_FRAME_NO_STOP)       || \
                                                       IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(REQUEST))

#define IS_I2C_TRANSFER_OTHER_OPTIONS_REQUEST(REQUEST) (((REQUEST) == I2C_OTHER_FRAME)     || \
                                                        ((REQUEST) == I2C_OTHER_AND_LAST_FRAME))

#endif /* HAL_I2C_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC90XX_HAL_I2C_H__ */

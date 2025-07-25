/*
 * Copyright (c) 2022-2050, GraphichinaSX Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-18     huyt         V1.0.1
 */
#include <gc90xx_hal.h>

#ifdef HAL_I2C_MODULE_ENABLED

/** @defgroup I2C I2C
  * @brief I2C HAL module driver
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup I2C_Private_Define
  * @{
  */
#define I2C_TIMEOUT_FLAG          35U         /*!< Timeout 35 ms             */
#define I2C_TIMEOUT_BUSY_FLAG     25U         /*!< Timeout 25 ms             */
#define I2C_NO_OPTION_FRAME       0xFFFF0000U /*!< XferOptions default value */

/* Private define for @ref PreviousState usage */
#define I2C_STATE_MSK             ((uint32_t)((uint32_t)((uint32_t)HAL_I2C_STATE_BUSY_TX | (uint32_t)HAL_I2C_STATE_BUSY_RX) & (uint32_t)(~((uint32_t)HAL_I2C_STATE_READY)))) /*!< Mask State define, keep only RX and TX bits            */
#define I2C_STATE_NONE            ((uint32_t)(HAL_I2C_MODE_NONE))                                                        /*!< Default Value                                          */
#define I2C_STATE_MASTER_BUSY_TX  ((uint32_t)(((uint32_t)HAL_I2C_STATE_BUSY_TX & I2C_STATE_MSK) | (uint32_t)HAL_I2C_MODE_MASTER))            /*!< Master Busy TX, combinaison of State LSB and Mode enum */
#define I2C_STATE_MASTER_BUSY_RX  ((uint32_t)(((uint32_t)HAL_I2C_STATE_BUSY_RX & I2C_STATE_MSK) | (uint32_t)HAL_I2C_MODE_MASTER))            /*!< Master Busy RX, combinaison of State LSB and Mode enum */
#define I2C_STATE_SLAVE_BUSY_TX   ((uint32_t)(((uint32_t)HAL_I2C_STATE_BUSY_TX & I2C_STATE_MSK) | (uint32_t)HAL_I2C_MODE_SLAVE))             /*!< Slave Busy TX, combinaison of State LSB and Mode enum  */
#define I2C_STATE_SLAVE_BUSY_RX   ((uint32_t)(((uint32_t)HAL_I2C_STATE_BUSY_RX & I2C_STATE_MSK) | (uint32_t)HAL_I2C_MODE_SLAVE))             /*!< Slave Busy RX, combinaison of State LSB and Mode enum  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/** @defgroup I2C_Private_Functions I2C Private Functions
  * @{
  */
static void I2C_SetSdaHold(I2C_HandleTypeDef *hi2c);
static void I2C_SetRegAccessFlag(I2C_HandleTypeDef *hi2c);
/* Private functions to handle DMA transfer */
static void I2C_DMAXferCplt(DMA_HandleTypeDef *hdma);
static void I2C_DMAError(DMA_HandleTypeDef *hdma);
static void I2C_DMAAbort(DMA_HandleTypeDef *hdma);

static void I2C_ITError(I2C_HandleTypeDef *hi2c);
static uint32_t I2C_Read_Clear_ISR(I2C_HandleTypeDef *hi2c);

static HAL_StatusTypeDef I2C_MasterRequestConfig(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t cmd);
static HAL_StatusTypeDef I2C_SlaveRequestConfig(I2C_HandleTypeDef *hi2c);
static HAL_StatusTypeDef I2C_MasterDMARequestConfig(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t cmd);
static HAL_StatusTypeDef I2C_SlaveDMARequestConfig(I2C_HandleTypeDef *hi2c);

static HAL_StatusTypeDef I2C_MasterRequestRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Timeout);
static HAL_StatusTypeDef I2C_RequestMemoryWrite(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart);
static HAL_StatusTypeDef I2C_RequestMemoryRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart);

/* Private functions to handle flags during polling transfer */
static HAL_StatusTypeDef I2C_WaitOnFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout);
static HAL_StatusTypeDef I2C_WaitOnMasterAddressFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, uint32_t Timeout);
static HAL_StatusTypeDef I2C_WaitOnTXEFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout);
static HAL_StatusTypeDef I2C_WaitOnRXNEFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout);
static HAL_StatusTypeDef I2C_WaitOnSTOPFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout);
static HAL_StatusTypeDef I2C_IsAcknowledgeFailed(I2C_HandleTypeDef *hi2c);

/* Private functions for I2C transfer IRQ handler */
static void I2C_TXHandle_MemWrite(I2C_HandleTypeDef *hi2c);
static void I2C_TXHandle_MemRead(I2C_HandleTypeDef *hi2c);
static void I2C_TXHandle_MasterSlave(I2C_HandleTypeDef *hi2c);
static void I2C_TXHandle_MasterRX(I2C_HandleTypeDef *hi2c);
static void I2C_RXHandle(I2C_HandleTypeDef *hi2c);
static void I2C_TXHandle(I2C_HandleTypeDef *hi2c);
static void I2C_StopHandler(I2C_HandleTypeDef *hi2c);
static void I2C_AbortHandler(I2C_HandleTypeDef *hi2c);

/* Private function to Convert Specific options */
static void I2C_ConvertOtherXferOptions(I2C_HandleTypeDef *hi2c);

/* Exported functions --------------------------------------------------------*/

/** @defgroup I2C_Exported_Functions I2C Exported Functions
  * @{
  */

/** @defgroup I2C_Exported_Functions_Group1 Initialization and de-initialization functions
 *  @brief    Initialization and Configuration functions
 *
@verbatim
 ===============================================================================
              ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]  This subsection provides a set of functions allowing to initialize and
          deinitialize the I2Cx peripheral:

      (+) User must Implement HAL_I2C_MspInit() function in which he configures
          all related peripherals resources (CLOCK, GPIO, DMA, IT and NVIC).

      (+) Call the function HAL_I2C_Init() to configure the selected device with
          the selected configuration:
      (+) Call the function HAL_I2C_DeInit() to restore the default configuration
          of the selected I2Cx peripheral.

@endverbatim
  * @{
  */
HAL_StatusTypeDef HAL_I2C_StructInit(I2C_HandleTypeDef *hi2c)
{
    hi2c->Init.Speed = I2C_Speed_Standard;
    hi2c->Init.GeneralCallEn = I2C_ENABLE;
    hi2c->Init.ReStartEn = I2C_DISABLE;//This is available only if I2C_IC_EMPTYFIFO_HOLD_MASTER_EN is configured to 1
    hi2c->Init.AddressMode = I2C_AddrMode_7BIT;
    hi2c->Init.OwnAddress1 = 0x55;
    hi2c->Init.OwnAddress2 = 0x56;
    
    hi2c->Init.ss_hcnt = 0;
    hi2c->Init.ss_lcnt = 0;
    hi2c->Init.fs_hcnt = 0;
    hi2c->Init.fs_lcnt = 0;
    hi2c->Init.fp_hcnt = 0;
    hi2c->Init.fp_lcnt = 0;
    hi2c->Init.hs_hcnt = 0;
    hi2c->Init.hs_lcnt = 0;
    hi2c->Init.tx_sda_hold_time =  0;
    hi2c->Init.rx_sda_hold_time = 0;
    hi2c->Init.sda_fall_ns =  0;
    hi2c->Init.scl_fall_ns = 0;
    
    hi2c->Init.tx_fifo_threshold = 0;
    /* rx_len%(rx_fifo_threshold+1) must be equal to 0, if receive 8 bytes data, rx_fifo_threshold can set 0/1/3/7 */
    hi2c->Init.rx_fifo_threshold = 0;//Set this parameter based on the actual scenario, 
    hi2c->Init.tx_fifo_depth = BSP_I2C_TX_FIFO_DEPTH;
    hi2c->Init.rx_fifo_depth = BSP_I2C_RX_FIFO_DEPTH;
    return HAL_OK;
}

static void I2C_SetSdaHold(I2C_HandleTypeDef *hi2c)
{
    uint32_t tempreg;
    /* Write SDA hold time if supported */
    tempreg = hi2c->Instance->COMP_VERSION;
    if(tempreg >= I2C_SDA_HOLD_MIN_VERS)
    {
        /* Keep previous hold time setting if no one set it */
        tempreg = hi2c->Instance->SDA_HOLD;
        if (hi2c->Init.rx_sda_hold_time == 0)
        {
            
            hi2c->Init.rx_sda_hold_time = (tempreg & I2C_SDA_HOLD_RX_Msk) >> I2C_SDA_HOLD_RX_Pos;
        }
        /*
         * Workaround for avoiding TX arbitration lost in case I2C
         * slave pulls SDA down "too quickly" after falling egde of
         * SCL by enabling non-zero SDA RX hold. Specification says it
         * extends incoming SDA low to high transition while SCL is
         * high but it apprears to help also above issue.
         */
        if (hi2c->Init.rx_sda_hold_time == 0)
        {
            hi2c->Init.rx_sda_hold_time = 1;
        }
        MODIFY_REG(hi2c->Instance->SDA_HOLD, I2C_SDA_HOLD_RX_Msk, hi2c->Init.rx_sda_hold_time << I2C_SDA_HOLD_RX_Pos);
    }
    else
    {
        if(hi2c->Init.rx_sda_hold_time)
        {
            MODIFY_REG(hi2c->Instance->SDA_HOLD, I2C_SDA_HOLD_RX_Msk, hi2c->Init.rx_sda_hold_time << I2C_SDA_HOLD_RX_Pos);
        }
    }
    if(hi2c->Init.tx_sda_hold_time)
    {
        MODIFY_REG(hi2c->Instance->SDA_HOLD, I2C_SDA_HOLD_TX, hi2c->Init.tx_sda_hold_time << I2C_SDA_HOLD_TX_Pos);
    }
}

/**
  * Set register access flags
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * Autodetects needed register access mode and sets access flags accordingly.
  * This must be called before doing any other register access.
  */
static void I2C_SetRegAccessFlag(I2C_HandleTypeDef *hi2c)
{
    uint32_t tempreg;
    tempreg = hi2c->Instance->COMP_TYPE;
    if (tempreg == I2C_COMP_TYPE_VALUE)
    {
        /* Configure register endianess access */
        hi2c->flags |= I2C_ACCESS_SWAP;
    }
    else if (tempreg == (I2C_COMP_TYPE_VALUE & 0x0000FFFFU))
    {
        /* Configure register access mode 16bit */
        hi2c->flags |= I2C_ACCESS_16BIT;
    }
}

static uint32_t I2C_SclHcnt(uint32_t ic_clk, uint32_t tSYMBOL, uint32_t tf, int cond, int offset)
{
    //Minimum hcnt value of 5 as a seed value
    /*
     * DesignWare I2C core doesn't seem to have solid strategy to meet
     * the tHD;STA timing spec.  Configuring _HCNT based on tHIGH spec
     * will result in violation of the tHD;STA spec.
     */
    if (cond)
    {
        /*
         * Conditional expression:
         *
         *   IC_[FS]S_SCL_HCNT + (1+4+3) >= IC_CLK * tHIGH
         *
         * This is based on the DW manuals, and represents an ideal
         * configuration.  The resulting I2C bus speed will be
         * faster than any of the others.
         *
         * If your hardware is free from tHD;STA issue, try this one.
         */
        return (ic_clk * tSYMBOL + 500000) / 1000000 - 8 + offset;
    }
    else
    {
        /*
         * Conditional expression:
         *
         *   IC_[FS]S_SCL_HCNT + 3 >= IC_CLK * (tHD;STA + tf)
         *
         * This is just experimental rule; the tHD;STA period turned
         * out to be proportinal to (_HCNT + 3).  With this setting,
         * we could meet both tHIGH and tHD;STA timing specs.
         *
         * If unsure, you'd better to take this alternative.
         *
         * The reason why we need to take into account "tf" here,
         * is the same as described in i2c_dw_scl_lcnt().
         */
        return (ic_clk * (tSYMBOL + tf) + 500000) / 1000000 - 3 + offset;
    }
}

static uint32_t I2C_SclLcnt(uint32_t ic_clk, uint32_t tLOW, uint32_t tf, int offset)
{
    /*
     * Conditional expression:
     *
     *   IC_[FS]S_SCL_LCNT + 1 >= IC_CLK * (tLOW + tf)
     *
     * DW I2C core starts counting the SCL CNTs for the LOW period
     * of the SCL clock (tLOW) as soon as it pulls the SCL line.
     * In order to meet the tLOW timing spec, we need to take into
     * account the fall time of SCL signal (tf).  Default tf value
     * should be 0.3 us, for safety.
     */
    return ((ic_clk * (tLOW + tf) + 500000) / 1000000) - 1 + offset;
}

/*
 * unit: KHz
 * */
static inline uint32_t I2C_GetClock(I2C_HandleTypeDef *hi2c)
{
    uint32_t apb_clk = HAL_RCC_GetAPBClock();
    return (apb_clk / 1000);
}

/*
 *  Refer to user manual 2.15.4.5
 * */
static HAL_StatusTypeDef I2C_SetTimings(I2C_HandleTypeDef *hi2c)
{
    uint32_t sda_falling_time;
    uint32_t scl_falling_time;
    uint32_t comp_param1;
    uint32_t ic_clk;

    comp_param1 = hi2c->Instance->COMP_PARAM_1;
    /* Set standard and fast speed dividers for high/low periods */
    sda_falling_time = hi2c->Init.sda_fall_ns > 0 ? hi2c->Init.sda_fall_ns : 300; /* ns */
    scl_falling_time = hi2c->Init.scl_fall_ns > 0 ? hi2c->Init.scl_fall_ns : 300; /* ns */
    /* Calculate SCL timing parameters for standard mode if not set */
    if (hi2c->Init.ss_hcnt == 0 || hi2c->Init.ss_lcnt == 0)
    {
        ic_clk = I2C_GetClock(hi2c);
        hi2c->Init.ss_hcnt = I2C_SclHcnt(ic_clk,
                                          4000,   /* tHD;STA = tHIGH = 4.0 us */
                                          sda_falling_time,
                                          0,  /* 0: DW default, 1: Ideal */
                                          0); /* No offset */
        hi2c->Init.ss_lcnt = I2C_SclLcnt(ic_clk,
                                          4700,   /* tLOW = 4.7 us */
                                          scl_falling_time,
                                          0); /* No offset */
    }
    if(hi2c->Init.fp_hcnt == 0 && hi2c->Init.fp_lcnt == 0)
    {
        ic_clk = I2C_GetClock(hi2c);
        hi2c->Init.fp_hcnt = I2C_SclHcnt(ic_clk,
                                          260,   /* tHD;STA = tHIGH = 0.26 us */
                                          sda_falling_time,
                                          0,  /* 0: DW default, 1: Ideal */
                                          0); /* No offset */
        hi2c->Init.fp_lcnt = I2C_SclLcnt(ic_clk,
                                          500,   /* tLOW = 0.5 us */
                                          scl_falling_time,
                                          0); /* No offset */
    }
    if(hi2c->Init.Speed == I2C_Speed_FastPlus)
    {
        /*
         * Check are fast mode plus parameters available and use
         * fast mode if not.
         */
        if (hi2c->Init.fp_hcnt != 0 && hi2c->Init.fp_lcnt != 0)
        {
            hi2c->Init.fs_hcnt = hi2c->Init.fp_hcnt;
            hi2c->Init.fs_lcnt = hi2c->Init.fp_lcnt;
        }
    }
    /*
     * Calculate SCL timing parameters for fast mode if not set. They are
     * needed also in high speed mode.
     */
    if (hi2c->Init.fs_hcnt == 0 || hi2c->Init.fs_lcnt == 0)
    {
        ic_clk = I2C_GetClock(hi2c);
        hi2c->Init.fs_hcnt = I2C_SclHcnt(ic_clk,
                                         600,    /* tHD;STA = tHIGH = 0.6 us */
                                         sda_falling_time,
                                         0,  /* 0: DW default, 1: Ideal */
                                         0); /* No offset */
        hi2c->Init.fs_lcnt = I2C_SclLcnt(ic_clk,
                                         1300,   /* tLOW = 1.3 us */
                                         scl_falling_time,
                                         0); /* No offset */
    }
    /* Check is high speed possible and fall back to fast mode if not */
    if(hi2c->Init.Speed == I2C_Speed_High)
    {
        if ((comp_param1 & I2C_COMP_PARAM_MAX_SPEED_MODE) != I2C_COMP_PARAM_MAX_SPEED_MODE)
        {
            hi2c->Init.Speed = I2C_Speed_Fast;
            hi2c->Init.hs_hcnt = 0;
            hi2c->Init.hs_lcnt = 0;
        }
    }
    /* Write speed timing parameters */
    if(hi2c->Init.Speed == I2C_Speed_FastPlus)
    {//Fast Plus param only be used for user
        MODIFY_REG(hi2c->Instance->CR, I2C_CR_SPEED, I2C_Speed_Fast << I2C_CR_SPEED_Pos);
    }
    else
    {
        MODIFY_REG(hi2c->Instance->CR, I2C_CR_SPEED, hi2c->Init.Speed << I2C_CR_SPEED_Pos);
    }
    /* Write standard speed timing parameters */
    MODIFY_REG(hi2c->Instance->SS_SCL_HCNT, I2C_SS_SCL_HCNT, hi2c->Init.ss_hcnt);
    MODIFY_REG(hi2c->Instance->SS_SCL_LCNT, I2C_SS_SCL_LCNT, hi2c->Init.ss_lcnt);

    /* Write fast mode/fast mode plus timing parameters */
    MODIFY_REG(hi2c->Instance->FS_SCL_HCNT, I2C_FS_SCL_HCNT, hi2c->Init.fs_hcnt);
    MODIFY_REG(hi2c->Instance->FS_SCL_LCNT, I2C_FS_SCL_LCNT, hi2c->Init.fs_lcnt);

    /* Write high speed timing parameters if supported */
    if(hi2c->Init.hs_hcnt && hi2c->Init.hs_lcnt)
    {
        MODIFY_REG(hi2c->Instance->HS_SCL_HCNT, I2C_HS_SCL_HCNT, hi2c->Init.hs_hcnt);
        MODIFY_REG(hi2c->Instance->HS_SCL_LCNT, I2C_HS_SCL_LCNT, hi2c->Init.hs_lcnt);
    }
    return HAL_OK;
}

/**
  * @brief  Initializes the I2C according to the specified parameters
  *         in the I2C_InitTypeDef and initialize the associated handle.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c)
{
    /* Check the parameters */
    assert_param(hi2c != NULL);
    assert_param(IS_I2C_ALL_INSTANCE(hi2c->Instance));
    assert_param(IS_I2C_SPEED(hi2c->Init.Speed));
    assert_param(IS_I2C_OWN_ADDRESS1(hi2c->Init.OwnAddress1));
    assert_param(IS_I2C_ADDRESS_MODE(hi2c->Init.AddressMode));
    assert_param(IS_I2C_OWN_ADDRESS2(hi2c->Init.OwnAddress2));
    assert_param(IS_I2C_GENERAL_CALL(hi2c->Init.GeneralCallEn));

#if(defined USE_PERIPHERAL_LOCK && USE_PERIPHERAL_LOCK == 1U)
    __HAL_CHECK_PERI_LOCK((uint32_t)(hi2c->Instance));
#endif
    if (hi2c->State == HAL_I2C_STATE_RESET)
    {
        /* Allocate lock resource and initialize it */
        hi2c->Lock = HAL_UNLOCKED;
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
        /* Init the I2C Callback settings */
        hi2c->MasterTxCpltCallback = HAL_I2C_MasterTxCpltCallback; /* Legacy weak MasterTxCpltCallback */
        hi2c->MasterRxCpltCallback = HAL_I2C_MasterRxCpltCallback; /* Legacy weak MasterRxCpltCallback */
        hi2c->SlaveTxCpltCallback  = HAL_I2C_SlaveTxCpltCallback;  /* Legacy weak SlaveTxCpltCallback  */
        hi2c->SlaveRxCpltCallback  = HAL_I2C_SlaveRxCpltCallback;  /* Legacy weak SlaveRxCpltCallback  */
        hi2c->ListenCpltCallback   = HAL_I2C_ListenCpltCallback;   /* Legacy weak ListenCpltCallback   */
        hi2c->MemTxCpltCallback    = HAL_I2C_MemTxCpltCallback;    /* Legacy weak MemTxCpltCallback    */
        hi2c->MemRxCpltCallback    = HAL_I2C_MemRxCpltCallback;    /* Legacy weak MemRxCpltCallback    */
        hi2c->ErrorCallback        = HAL_I2C_ErrorCallback;        /* Legacy weak ErrorCallback        */
        hi2c->AbortCpltCallback    = HAL_I2C_AbortCpltCallback;    /* Legacy weak AbortCpltCallback    */
        hi2c->AddrCallback         = HAL_I2C_AddrCallback;         /* Legacy weak AddrCallback         */
        if (hi2c->MspInitCallback == NULL)
        {
            hi2c->MspInitCallback = HAL_I2C_MspInit; /* Legacy weak MspInit  */
        }
        /* Init the low level hardware : GPIO, CLOCK, NVIC */
        hi2c->MspInitCallback(hi2c);
#else
        /* Init the low level hardware : GPIO, CLOCK, NVIC */
        HAL_I2C_MspInit(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }
    uint32_t tempreg;
    hi2c->State = HAL_I2C_STATE_BUSY;
    /* Disable the selected I2C peripheral */
    __HAL_I2C_DISABLE(hi2c);
    I2C_SetRegAccessFlag(hi2c);
    /*---------------------------- I2Cx CR Configuration ----------------------*/
    // Select Master or Slave bus mode.
    HAL_I2C_BusMode_Select(hi2c, I2C_Mode_Master);
    // Select address mode 7 or 10 bits
    HAL_I2C_AddrMode_Select(hi2c, hi2c->Init.AddressMode);
    tempreg = hi2c->Instance->CR;
    SET_BIT(tempreg, I2C_CR_RESTART_EN);
    CLEAR_BIT(tempreg, I2C_CR_OPTIONAL_SAR_CTRL);
    if(hi2c->Init.OwnAddress2)
    {
        MODIFY_REG(hi2c->Instance->OPTIONAL_SAR, I2C_OPTIONAL_SAR, hi2c->Init.OwnAddress2);
        SET_BIT(tempreg, I2C_CR_OPTIONAL_SAR_CTRL);
    }
    hi2c->Instance->CR = tempreg;

    I2C_SetTimings(hi2c);
    I2C_SetSdaHold(hi2c);

    /* Configure Tx/Rx FIFO threshold levels */
    MODIFY_REG(hi2c->Instance->TX_TL, I2C_TX_TL, hi2c->Init.tx_fifo_threshold << I2C_TX_TL_Pos);
    MODIFY_REG(hi2c->Instance->RX_TL, I2C_RX_TL, hi2c->Init.rx_fifo_threshold << I2C_RX_TL_Pos);

    MODIFY_REG(hi2c->Instance->SDA_SETUP, I2C_SDA_SETUP, 0x64);
    MODIFY_REG(hi2c->Instance->SAR, I2C_SAR, hi2c->Init.OwnAddress1);
    
    __HAL_CLEAR_INTR(hi2c);
    I2C_Read_Clear_ISR(hi2c);
    __HAL_I2C_DISABLE_IT_ALL(hi2c);

    __HAL_I2C_DMA_TX_DISABLE(hi2c);
    __HAL_I2C_DMA_RX_DISABLE(hi2c);

    /* Enable the selected I2C peripheral */
    __HAL_I2C_ENABLE(hi2c);

    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->PreviousState = I2C_STATE_NONE;
    hi2c->Mode = HAL_I2C_MODE_NONE;

#if(defined USE_PERIPHERAL_LOCK && USE_PERIPHERAL_LOCK == 1U)
    __HAL_SET_PERI_LOCK((uint32_t)(hi2c->Instance), PERIPHERAL_INITED);
#endif
    return HAL_OK;
}

/**
  * @brief  DeInitialize the I2C peripheral.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c)
{
    /* Check the parameters */
    assert_param(hi2c != NULL);
    assert_param(IS_I2C_ALL_INSTANCE(hi2c->Instance));
    
    hi2c->State = HAL_I2C_STATE_BUSY;
    /* Disable the I2C Peripheral Clock */
    __HAL_I2C_DISABLE(hi2c);
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
    if (hi2c->MspDeInitCallback == NULL)
    {
        hi2c->MspDeInitCallback = HAL_I2C_MspDeInit; /* Legacy weak MspDeInit  */
    }
    /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
    hi2c->MspDeInitCallback(hi2c);
#else
    /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
    HAL_I2C_MspDeInit(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    hi2c->ErrorCode     = HAL_I2C_ERROR_NONE;
    hi2c->State         = HAL_I2C_STATE_RESET;
    hi2c->PreviousState = I2C_STATE_NONE;
    hi2c->Mode          = HAL_I2C_MODE_NONE;
    /* Release Lock */
    __HAL_UNLOCK(hi2c);
    return HAL_OK;
}

static uint32_t I2C_Read_Clear_ISR(I2C_HandleTypeDef *hi2c)
{
    uint32_t status = hi2c->Instance->RISR;
    if(status & I2C_FLAG_RX_UNDER)
    {
        __HAL_CLEAR_RX_UNDER(hi2c);
    }
    if(status & I2C_FLAG_RX_OVER)
    {
        __HAL_CLEAR_RX_OVER(hi2c);
    }
    if(status & I2C_FLAG_TX_OVER)
    {
        __HAL_CLEAR_TX_OVER(hi2c);
    }
    if(status & I2C_FLAG_RD_REQ)
    {
        __HAL_CLEAR_RD_REQ(hi2c);
    }
    if(status & I2C_FLAG_TX_ABRT)
    {
        hi2c->tx_abort_source = hi2c->Instance->TX_ABRT_SOURCE;
        __HAL_CLEAR_TX_ABRT(hi2c);
    }
    if(status & I2C_FLAG_RX_DONE)
    {
        __HAL_CLEAR_RX_DONE(hi2c);
    }
    if(status & I2C_FLAG_ACTIVITY)
    {
        __HAL_CLEAR_ACTIVITY(hi2c);
    }
    if(status & I2C_FLAG_STOP_DET)
    {
        __HAL_CLEAR_STOP_DET(hi2c);
    }
    if(status & I2C_FLAG_START_DET)
    {
        __HAL_CLEAR_START_DET(hi2c);
    }
    if(status & I2C_FLAG_GEN_CALL)
    {
        __HAL_CLEAR_GEN_CALL(hi2c);
    }
    return status;
}

/**
  * @brief  Initialize the I2C MSP.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @retval None
  */
__WEAK void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MspInit could be implemented in the user file
    */
}

/**
  * @brief  DeInitialize the I2C MSP.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @retval None
  */
__WEAK void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MspDeInit could be implemented in the user file
    */
}

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register a User I2C Callback
  *         To be used instead of the weak predefined callback
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  CallbackID ID of the callback to be registered
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_I2C_MASTER_TX_COMPLETE_CB_ID Master Tx Transfer completed callback ID
  *          @arg @ref HAL_I2C_MASTER_RX_COMPLETE_CB_ID Master Rx Transfer completed callback ID
  *          @arg @ref HAL_I2C_SLAVE_TX_COMPLETE_CB_ID Slave Tx Transfer completed callback ID
  *          @arg @ref HAL_I2C_SLAVE_RX_COMPLETE_CB_ID Slave Rx Transfer completed callback ID
  *          @arg @ref HAL_I2C_LISTEN_COMPLETE_CB_ID Listen Complete callback ID
  *          @arg @ref HAL_I2C_MEM_TX_COMPLETE_CB_ID Memory Tx Transfer callback ID
  *          @arg @ref HAL_I2C_MEM_RX_COMPLETE_CB_ID Memory Rx Transfer completed callback ID
  *          @arg @ref HAL_I2C_ERROR_CB_ID Error callback ID
  *          @arg @ref HAL_I2C_ABORT_CB_ID Abort callback ID
  *          @arg @ref HAL_I2C_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_I2C_MSPDEINIT_CB_ID MspDeInit callback ID
  * @param  pCallback pointer to the Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_RegisterCallback(I2C_HandleTypeDef *hi2c, HAL_I2C_CallbackIDTypeDef CallbackID, pI2C_CallbackTypeDef pCallback)
{
    HAL_StatusTypeDef status = HAL_OK;
    if(pCallback == NULL)
    {
        /* Update the error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;
        return HAL_ERROR;
    }
    /* Process locked */
    __HAL_LOCK(hi2c);

    if (HAL_I2C_STATE_READY == hi2c->State)
    {
        switch (CallbackID)
        {
        case HAL_I2C_MASTER_TX_COMPLETE_CB_ID :
            hi2c->MasterTxCpltCallback = pCallback;
        break;

        case HAL_I2C_MASTER_RX_COMPLETE_CB_ID :
            hi2c->MasterRxCpltCallback = pCallback;
        break;

        case HAL_I2C_SLAVE_TX_COMPLETE_CB_ID :
            hi2c->SlaveTxCpltCallback = pCallback;
        break;

        case HAL_I2C_SLAVE_RX_COMPLETE_CB_ID :
            hi2c->SlaveRxCpltCallback = pCallback;
        break;

        case HAL_I2C_LISTEN_COMPLETE_CB_ID :
            hi2c->ListenCpltCallback = pCallback;
        break;

        case HAL_I2C_MEM_TX_COMPLETE_CB_ID :
            hi2c->MemTxCpltCallback = pCallback;
        break;

        case HAL_I2C_MEM_RX_COMPLETE_CB_ID :
            hi2c->MemRxCpltCallback = pCallback;
        break;

        case HAL_I2C_ERROR_CB_ID :
            hi2c->ErrorCallback = pCallback;
        break;

        case HAL_I2C_ABORT_CB_ID :
            hi2c->AbortCpltCallback = pCallback;
        break;

        case HAL_I2C_MSPINIT_CB_ID :
            hi2c->MspInitCallback = pCallback;
        break;

        case HAL_I2C_MSPDEINIT_CB_ID :
            hi2c->MspDeInitCallback = pCallback;
        break;

        default :
            /* Update the error code */
            hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status =  HAL_ERROR;
        break;
        }
    }
    else if (HAL_I2C_STATE_RESET == hi2c->State)
    {
        switch (CallbackID)
        {
        case HAL_I2C_MSPINIT_CB_ID :
            hi2c->MspInitCallback = pCallback;
        break;

        case HAL_I2C_MSPDEINIT_CB_ID :
            hi2c->MspDeInitCallback = pCallback;
        break;

        default:
            /* Update the error code */
            hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status =  HAL_ERROR;
        break;
        }
    }
    else
    {
        /* Update the error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status =  HAL_ERROR;
    }
    /* Release Lock */
    __HAL_UNLOCK(hi2c);
    return status;
}

/**
  * @brief  Unregister an I2C Callback
  *         I2C callback is redirected to the weak predefined callback
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  CallbackID ID of the callback to be unregistered
  *         This parameter can be one of the following values:
  *         This parameter can be one of the following values:
  *          @arg @ref HAL_I2C_MASTER_TX_COMPLETE_CB_ID Master Tx Transfer completed callback ID
  *          @arg @ref HAL_I2C_MASTER_RX_COMPLETE_CB_ID Master Rx Transfer completed callback ID
  *          @arg @ref HAL_I2C_SLAVE_TX_COMPLETE_CB_ID Slave Tx Transfer completed callback ID
  *          @arg @ref HAL_I2C_SLAVE_RX_COMPLETE_CB_ID Slave Rx Transfer completed callback ID
  *          @arg @ref HAL_I2C_LISTEN_COMPLETE_CB_ID Listen Complete callback ID
  *          @arg @ref HAL_I2C_MEM_TX_COMPLETE_CB_ID Memory Tx Transfer callback ID
  *          @arg @ref HAL_I2C_MEM_RX_COMPLETE_CB_ID Memory Rx Transfer completed callback ID
  *          @arg @ref HAL_I2C_ERROR_CB_ID Error callback ID
  *          @arg @ref HAL_I2C_ABORT_CB_ID Abort callback ID
  *          @arg @ref HAL_I2C_MSPINIT_CB_ID MspInit callback ID
  *          @arg @ref HAL_I2C_MSPDEINIT_CB_ID MspDeInit callback ID
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_UnRegisterCallback(I2C_HandleTypeDef *hi2c, HAL_I2C_CallbackIDTypeDef CallbackID)
{
    HAL_StatusTypeDef status = HAL_OK;
    /* Process locked */
    __HAL_LOCK(hi2c);
    if (HAL_I2C_STATE_READY == hi2c->State)
    {
        switch (CallbackID)
        {
        case HAL_I2C_MASTER_TX_COMPLETE_CB_ID :
            hi2c->MasterTxCpltCallback = HAL_I2C_MasterTxCpltCallback; /* Legacy weak MasterTxCpltCallback */
        break;

        case HAL_I2C_MASTER_RX_COMPLETE_CB_ID :
            hi2c->MasterRxCpltCallback = HAL_I2C_MasterRxCpltCallback; /* Legacy weak MasterRxCpltCallback */
        break;

        case HAL_I2C_SLAVE_TX_COMPLETE_CB_ID :
            hi2c->SlaveTxCpltCallback = HAL_I2C_SlaveTxCpltCallback;   /* Legacy weak SlaveTxCpltCallback  */
        break;

        case HAL_I2C_SLAVE_RX_COMPLETE_CB_ID :
            hi2c->SlaveRxCpltCallback = HAL_I2C_SlaveRxCpltCallback;   /* Legacy weak SlaveRxCpltCallback  */
        break;

        case HAL_I2C_LISTEN_COMPLETE_CB_ID :
            hi2c->ListenCpltCallback = HAL_I2C_ListenCpltCallback;     /* Legacy weak ListenCpltCallback   */
        break;

        case HAL_I2C_MEM_TX_COMPLETE_CB_ID :
            hi2c->MemTxCpltCallback = HAL_I2C_MemTxCpltCallback;       /* Legacy weak MemTxCpltCallback    */
        break;

        case HAL_I2C_MEM_RX_COMPLETE_CB_ID :
            hi2c->MemRxCpltCallback = HAL_I2C_MemRxCpltCallback;       /* Legacy weak MemRxCpltCallback    */
        break;

        case HAL_I2C_ERROR_CB_ID :
            hi2c->ErrorCallback = HAL_I2C_ErrorCallback;               /* Legacy weak ErrorCallback        */
        break;

        case HAL_I2C_ABORT_CB_ID :
            hi2c->AbortCpltCallback = HAL_I2C_AbortCpltCallback;       /* Legacy weak AbortCpltCallback    */
        break;

        case HAL_I2C_MSPINIT_CB_ID :
            hi2c->MspInitCallback = HAL_I2C_MspInit;                   /* Legacy weak MspInit              */
        break;

        case HAL_I2C_MSPDEINIT_CB_ID :
            hi2c->MspDeInitCallback = HAL_I2C_MspDeInit;               /* Legacy weak MspDeInit            */
        break;

        default:
            /* Update the error code */
            hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status =  HAL_ERROR;
        break;
    }
    }
    else if (HAL_I2C_STATE_RESET == hi2c->State)
    {
        switch (CallbackID)
        {
        case HAL_I2C_MSPINIT_CB_ID :
            hi2c->MspInitCallback = HAL_I2C_MspInit;                   /* Legacy weak MspInit              */
        break;

        case HAL_I2C_MSPDEINIT_CB_ID :
            hi2c->MspDeInitCallback = HAL_I2C_MspDeInit;               /* Legacy weak MspDeInit            */
        break;

        default:
            /* Update the error code */
            hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status =  HAL_ERROR;
        break;
    }
    }
    else
    {
        /* Update the error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status =  HAL_ERROR;
    }
    /* Release Lock */
    __HAL_UNLOCK(hi2c);
    return status;
}

/**
  * @brief  Register the Slave Address Match I2C Callback
  *         To be used instead of the weak HAL_I2C_AddrCallback() predefined callback
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  pCallback pointer to the Address Match Callback function
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_RegisterAddrCallback(I2C_HandleTypeDef *hi2c, pI2C_AddrCallbackTypeDef pCallback)
{
    HAL_StatusTypeDef status = HAL_OK;
    if(pCallback == NULL)
    {
        /* Update the error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;
        return HAL_ERROR;
    }
    /* Process locked */
    __HAL_LOCK(hi2c);

    if (HAL_I2C_STATE_READY == hi2c->State)
    {
        hi2c->AddrCallback = pCallback;
    }
    else
    {
        /* Update the error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status =  HAL_ERROR;
    }
    /* Release Lock */
    __HAL_UNLOCK(hi2c);
    return status;
}

/**
  * @brief  UnRegister the Slave Address Match I2C Callback
  *         Info Ready I2C Callback is redirected to the weak HAL_I2C_AddrCallback() predefined callback
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_UnRegisterAddrCallback(I2C_HandleTypeDef *hi2c)
{
    HAL_StatusTypeDef status = HAL_OK;
    /* Process locked */
    __HAL_LOCK(hi2c);
    if (HAL_I2C_STATE_READY == hi2c->State)
    {
        hi2c->AddrCallback = HAL_I2C_AddrCallback; /* Legacy weak AddrCallback  */
    }
    else
    {
        /* Update the error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status =  HAL_ERROR;
    }
    /* Release Lock */
    __HAL_UNLOCK(hi2c);
    return status;
}

#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup I2C_Exported_Functions_Group2 Input and Output operation functions
 *  @brief   Data transfers functions
 *
@verbatim
 ===============================================================================
                      ##### IO operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the I2C data
    transfers.

    (#) There are two modes of transfer:
       (++) Blocking mode : The communication is performed in the polling mode.
            The status of all data processing is returned by the same function
            after finishing transfer.
       (++) No-Blocking mode : The communication is performed using Interrupts
            or DMA. These functions return the status of the transfer startup.
            The end of the data processing will be indicated through the
            dedicated I2C IRQ when using Interrupt mode or the DMA IRQ when
            using DMA mode.

    (#) Blocking mode functions are :
        (++) HAL_I2C_Master_Transmit()
        (++) HAL_I2C_Master_Receive()
        (++) HAL_I2C_Slave_Transmit()
        (++) HAL_I2C_Slave_Receive()
        (++) HAL_I2C_Mem_Write()
        (++) HAL_I2C_Mem_Read()
        (++) HAL_I2C_IsDeviceReady()

    (#) No-Blocking mode functions with Interrupt are :
        (++) HAL_I2C_Master_Transmit_IT()
        (++) HAL_I2C_Master_Receive_IT()
        (++) HAL_I2C_Slave_Transmit_IT()
        (++) HAL_I2C_Slave_Receive_IT()
        (++) HAL_I2C_Mem_Write_IT()
        (++) HAL_I2C_Mem_Read_IT()
        (++) HAL_I2C_Master_Seq_Transmit_IT()
        (++) HAL_I2C_Master_Seq_Receive_IT()
        (++) HAL_I2C_Slave_Seq_Transmit_IT()
        (++) HAL_I2C_Slave_Seq_Receive_IT()
        (++) HAL_I2C_EnableListen_IT()
        (++) HAL_I2C_DisableListen_IT()
        (++) HAL_I2C_Master_Abort_IT()

    (#) No-Blocking mode functions with DMA are :
        (++) HAL_I2C_Master_Transmit_DMA()
        (++) HAL_I2C_Master_Receive_DMA()
        (++) HAL_I2C_Slave_Transmit_DMA()
        (++) HAL_I2C_Slave_Receive_DMA()
        (++) HAL_I2C_Mem_Write_DMA()
        (++) HAL_I2C_Mem_Read_DMA()
        (++) HAL_I2C_Master_Seq_Transmit_DMA()
        (++) HAL_I2C_Master_Seq_Receive_DMA()
        (++) HAL_I2C_Slave_Seq_Transmit_DMA()
        (++) HAL_I2C_Slave_Seq_Receive_DMA()

    (#) A set of Transfer Complete Callbacks are provided in non Blocking mode:
        (++) HAL_I2C_MasterTxCpltCallback()
        (++) HAL_I2C_MasterRxCpltCallback()
        (++) HAL_I2C_SlaveTxCpltCallback()
        (++) HAL_I2C_SlaveRxCpltCallback()
        (++) HAL_I2C_MemTxCpltCallback()
        (++) HAL_I2C_MemRxCpltCallback()
        (++) HAL_I2C_AddrCallback()
        (++) HAL_I2C_ListenCpltCallback()
        (++) HAL_I2C_ErrorCallback()
        (++) HAL_I2C_AbortCpltCallback()

@endverbatim
  * @{
  */
/**
  * @brief  Transmits in master mode an amount of data in blocking mode.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    uint32_t tempreg = 0;
    uint32_t tx_limit = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
    uint8_t need_restart = 0;
    if(hi2c->Init.ReStartEn)
    {
        need_restart = 1;
    }
#endif
    if(Size == 0)
    {
        return HAL_OK;
    }
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        /* Process Locked */
        __HAL_LOCK(hi2c);
        /* Wait until Master activity flag is reset */
        if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_STATUS_MST_ACTIVITY, SET, I2C_TIMEOUT_BUSY_FLAG) != HAL_OK)
        {
            return HAL_BUSY;
        }
        hi2c->State       = HAL_I2C_STATE_BUSY_TX;
        hi2c->Mode        = HAL_I2C_MODE_MASTER;
        hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;
        /* Prepare transfer parameters */
        hi2c->pBuffPtr    = pData;
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = 0;
        /* Master request configuration */
        I2C_MasterRequestConfig(hi2c, DevAddress, I2C_CMD_Master_Write);
        while (hi2c->XferCount > 0U)
        {
            tx_limit = hi2c->Init.tx_fifo_depth - hi2c->Instance->TXFLR;
            while(tx_limit > 0 && hi2c->XferCount > 0U)
            {
                /* Write data to DATA_CMD */
                tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
                tempreg |= (hi2c->XferCount == 1 ? I2C_DATA_CMD_STOP : 0);
                tempreg |= (need_restart == 1 ? I2C_DATA_CMD_RESTART : 0);
                need_restart = 0;
#endif
                tempreg |= I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos;
                tempreg |= (uint32_t)(*hi2c->pBuffPtr);
                hi2c->Instance->DATA_CMD = tempreg;
                tx_limit--;
                /* Increment Buffer pointer */
                hi2c->pBuffPtr++;
                /* Update counter */
                hi2c->XferCount--;
            }
            /* Wait until TXE flag is set */
            if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout) != HAL_OK)
            {
                return HAL_ERROR;
            }
        }
        /* Wait until STOP flag is set */
        if (I2C_WaitOnSTOPFlagUntilTimeout(hi2c, Timeout) != HAL_OK)
        {
            return HAL_ERROR;
        }
        hi2c->State = HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;
        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Receives in master mode an amount of data in blocking mode.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    /* Init tickstart for timeout management*/
    uint32_t tickstart = HAL_GetTick();
    uint32_t tx_limit = 0;
    uint32_t rx_valid = 0;
    uint32_t tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
    uint8_t need_restart = 0;
    if(hi2c->Init.ReStartEn)
    {
        need_restart = 1;
    }
#endif

    if(Size == 0)
    {
        return HAL_OK;
    }
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        /* Process Locked */
        __HAL_LOCK(hi2c);
        /* Wait until Master activity flag is reset */
        if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_STATUS_MST_ACTIVITY, SET, I2C_TIMEOUT_BUSY_FLAG) != HAL_OK)
        {
            return HAL_BUSY;
        }
        hi2c->State       = HAL_I2C_STATE_BUSY_RX;
        hi2c->Mode        = HAL_I2C_MODE_MASTER;
        hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;
        /* Prepare transfer parameters */
        hi2c->pBuffPtr    = pData;
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = Size;
        /* Master request configuration */
        I2C_MasterRequestConfig(hi2c, DevAddress, I2C_CMD_Master_Read);
        while (hi2c->XferCount > 0U)
        {
            tx_limit = hi2c->Init.tx_fifo_depth - hi2c->Instance->TXFLR;
            while(tx_limit > 0 && hi2c->cmd_cnt < hi2c->cmd_max)
            {
                /* Write CMD to DATA_CMD */
                tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
                if(hi2c->cmd_cnt == hi2c->cmd_max - 1)
                {
                    tempreg |= I2C_DATA_CMD_STOP;
                }
                tempreg |= need_restart == 1 ? I2C_DATA_CMD_RESTART : 0;
                need_restart = 0;
#endif
                tempreg |= I2C_CMD_Master_Read << I2C_DATA_CMD_CMD_Pos;
                hi2c->Instance->DATA_CMD = tempreg;
                tx_limit--;
                hi2c->cmd_cnt++;
            }
            rx_valid = hi2c->Instance->RXFLR;
            for (; rx_valid > 0 && hi2c->XferCount > 0U; rx_valid--)
            {
                /* Read data from DR */
                *hi2c->pBuffPtr = (uint8_t)(hi2c->Instance->DATA_CMD & 0xFF);
                /* Increment Buffer pointer */
                hi2c->pBuffPtr++;

                /* Update counter */
                hi2c->XferCount--;
            }
            if(Timeout != HAL_MAX_DELAY)
            {
                if((HAL_GetTick() - tickstart) > Timeout)
                {
                    hi2c->State = HAL_I2C_STATE_READY;
                    hi2c->Mode = HAL_I2C_MODE_NONE;
                    /* Process Unlocked */
                    __HAL_UNLOCK(hi2c);
                    return HAL_TIMEOUT;
                }
            }
        }
        /* Wait until STOP flag is set */
        if (I2C_WaitOnSTOPFlagUntilTimeout(hi2c, Timeout) != HAL_OK)
        {
            return HAL_ERROR;
        }
        hi2c->State = HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;
        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Transmits in slave mode an amount of data in blocking mode.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Transmit(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    uint32_t tx_limit;
    uint32_t tempreg;

    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U))
        {
            return  HAL_ERROR;
        }
        /* Process Locked */
        __HAL_LOCK(hi2c);
        /* Wait until Slave activity flag is reset */
        if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_STATUS_SLV_ACTIVITY, SET, I2C_TIMEOUT_BUSY_FLAG) != HAL_OK)
        {
            return HAL_BUSY;
        }
        hi2c->State       = HAL_I2C_STATE_BUSY_TX;
        hi2c->Mode        = HAL_I2C_MODE_SLAVE;
        hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;

        /* Prepare transfer parameters */
        hi2c->pBuffPtr    = pData;
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = 0;
        /* Slave request config */
        I2C_SlaveRequestConfig(hi2c);
        /* Generate ACK for a General Call */
        SET_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
        while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RD_REQ) == RESET)
        {
            __NOP();
        }
        while (hi2c->XferCount > 0U)
        {
            tx_limit = hi2c->Init.tx_fifo_depth - hi2c->Instance->TXFLR;
            while(tx_limit > 0 && hi2c->XferCount > 0U)
            {
                /* Write data to DATA_CMD */
                tempreg = 0;
                tempreg |= (uint32_t)(*hi2c->pBuffPtr);
                hi2c->Instance->DATA_CMD = tempreg;
                tx_limit--;
                /* Increment Buffer pointer */
                hi2c->pBuffPtr++;
                /* Update counter */
                hi2c->XferCount--;
            }
            /* Wait until TXE flag is set */
            if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout) != HAL_OK)
            {
                /* Generate NACK for a General Call */
                CLEAR_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
                return HAL_ERROR;
            }
        }
        /* Wait until STOP flag is set */
        if (I2C_WaitOnSTOPFlagUntilTimeout(hi2c, Timeout) != HAL_OK)
        {
            return HAL_ERROR;
        }
        /* Generate NACK for a General Call */
        CLEAR_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
        hi2c->State = HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;
        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Receive in slave mode an amount of data in blocking mode
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Receive(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    uint32_t rx_valid = 0;
    uint32_t tempreg = 0;

    if(Size == 0)
    {
        return HAL_OK;
    }
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        /* Process Locked */
        __HAL_LOCK(hi2c);
        /* Wait until Slave activity flag is reset */
        if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_STATUS_SLV_ACTIVITY, SET, I2C_TIMEOUT_BUSY_FLAG) != HAL_OK)
        {
            return HAL_BUSY;
        }
        hi2c->State       = HAL_I2C_STATE_BUSY_RX;
        hi2c->Mode        = HAL_I2C_MODE_MASTER;
        hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;
        /* Prepare transfer parameters */
        hi2c->pBuffPtr    = pData;
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = 0;
        /* Slave request config */
        if (I2C_SlaveRequestConfig(hi2c) != HAL_OK)
        {
            return HAL_ERROR;
        }
        /* Generate ACK for a General Call */
        SET_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
        while (hi2c->XferCount > 0U)
        {
            /* Wait until RFNE flag is set */
            if (I2C_WaitOnRXNEFlagUntilTimeout(hi2c, Timeout) != HAL_OK)
            {
                /* Generate NACK for a General Call */
                CLEAR_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
                return HAL_ERROR;
            }
            rx_valid = hi2c->Instance->RXFLR;
            for (; rx_valid > 0 && hi2c->XferCount > 0U; rx_valid--)
            {
                /* Read data from DR */
                *hi2c->pBuffPtr = (uint8_t)(hi2c->Instance->DATA_CMD & 0xFF);
                /* Increment Buffer pointer */
                hi2c->pBuffPtr++;

                /* Update counter */
                hi2c->XferCount--;
            }
        }
        /* Wait until STOP flag is set */
        if (I2C_WaitOnSTOPFlagUntilTimeout(hi2c, Timeout) != HAL_OK)
        {
            /* Generate ACK for a General Call */
            CLEAR_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
            return HAL_ERROR;
        }
        /* Generate ACK for a General Call */
        CLEAR_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
        I2C_Read_Clear_ISR(hi2c);
        hi2c->State = HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;
        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Transmit in master mode an amount of data in non-blocking mode with Interrupt
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    __IO uint32_t count = 0U;

    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        /* Wait until BUSY flag is reset */
        count = I2C_TIMEOUT_BUSY_FLAG * (GetSystemCoreClock() / 25U / 1000U);
        while (__HAL_I2C_GET_STATUS(hi2c, I2C_STATUS_MST_ACTIVITY) != RESET)
        {
            count--;
            if (count == 0U)
            {
                hi2c->PreviousState       = I2C_STATE_NONE;
                hi2c->State               = HAL_I2C_STATE_READY;
                hi2c->Mode                = HAL_I2C_MODE_NONE;
                hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;
                /* Process Unlocked */
                __HAL_UNLOCK(hi2c);
                return HAL_ERROR;
            }
        }
        /* Process Locked */
        __HAL_LOCK(hi2c);
        hi2c->State     = HAL_I2C_STATE_BUSY_TX;
        hi2c->Mode      = HAL_I2C_MODE_MASTER;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

        /* Prepare transfer parameters */
        hi2c->pBuffPtr    = pData;
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->Devaddress  = DevAddress;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = 0;
        /* Master request configuration */
        I2C_MasterRequestConfig(hi2c, DevAddress, I2C_CMD_Master_Write);
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_MASTER_MASK);
        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Receive in master mode an amount of data in non-blocking mode with Interrupt
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    __IO uint32_t count = 0U;
    uint32_t tempreg = 0;
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        /* Wait until BUSY flag is reset */
        count = I2C_TIMEOUT_BUSY_FLAG * (GetSystemCoreClock() / 25U / 1000U);
        while (__HAL_I2C_GET_STATUS(hi2c, I2C_STATUS_MST_ACTIVITY) != RESET)
        {
            count--;
            if (count == 0U)
            {
                hi2c->PreviousState       = I2C_STATE_NONE;
                hi2c->State               = HAL_I2C_STATE_READY;
                hi2c->Mode                = HAL_I2C_MODE_NONE;
                hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;
                /* Process Unlocked */
                __HAL_UNLOCK(hi2c);
                return HAL_ERROR;
            }
        }
        /* Process Locked */
        __HAL_LOCK(hi2c);

        hi2c->State     = HAL_I2C_STATE_BUSY_RX;
        hi2c->Mode      = HAL_I2C_MODE_MASTER;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

        /* Prepare transfer parameters */
        hi2c->pBuffPtr    = pData;
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->Devaddress  = DevAddress;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = Size;
        /* Master request configuration */
        I2C_MasterRequestConfig(hi2c, DevAddress, I2C_CMD_Master_Read);
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_MASTER_MASK);
        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Transmit in slave mode an amount of data in non-blocking mode with Interrupt
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Transmit_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size)
{

    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U))
        {
            return  HAL_ERROR;
        }
        /* Process Locked */
        __HAL_LOCK(hi2c);
        hi2c->State     = HAL_I2C_STATE_BUSY_TX;
        hi2c->Mode      = HAL_I2C_MODE_SLAVE;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
        /* Prepare transfer parameters */
        hi2c->pBuffPtr    = pData;
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = 0;
        /* Slave request config */
        I2C_SlaveRequestConfig(hi2c);
        /* Generate ACK for a General Call */
        SET_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_SLAVE_MASK);
        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Receive in slave mode an amount of data in non-blocking mode with Interrupt
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Receive_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size)
{

    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U))
        {
            return  HAL_ERROR;
        }
        /* Process Locked */
        __HAL_LOCK(hi2c);
        hi2c->State     = HAL_I2C_STATE_BUSY_RX;
        hi2c->Mode      = HAL_I2C_MODE_SLAVE;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
        /* Prepare transfer parameters */
        hi2c->pBuffPtr    = pData;
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = 0;//不需要写DATA_CMD寄存器
        /* Slave request config */
        I2C_SlaveRequestConfig(hi2c);
        /* Generate ACK for a General Call */
        SET_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_SLAVE_MASK);
        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Transmit in master mode an amount of data in non-blocking mode with DMA
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
#if(defined __I2C_MASTER_TX_USE_DMA && __I2C_MASTER_TX_USE_DMA == 1U)
    __IO uint32_t count = 0U;
    HAL_StatusTypeDef dmaxferstatus;

    if(!(hi2c->Instance->COMP_PARAM_1 & I2C_COMP_PARAM_HAS_DMA) || pData == NULL || Size == 0)
    {
        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_SIZE;
        return HAL_ERROR;
    }
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        /* Process Locked */
        __HAL_LOCK(hi2c);
        /* Wait until BUSY flag is reset */
        count = I2C_TIMEOUT_BUSY_FLAG * (GetSystemCoreClock() / 25U / 1000U);
        while (__HAL_I2C_GET_STATUS(hi2c, I2C_STATUS_MST_ACTIVITY) != RESET)
        {
            count--;
            if (count == 0U)
            {
                hi2c->PreviousState       = I2C_STATE_NONE;
                hi2c->State               = HAL_I2C_STATE_READY;
                hi2c->Mode                = HAL_I2C_MODE_NONE;
                hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;
                /* Process Unlocked */
                __HAL_UNLOCK(hi2c);
                return HAL_ERROR;
            }
        }
        /* Prepare transfer parameters */
        hi2c->pBuffPtr  = pData;
        hi2c->DMAState  = HAL_I2C_STATE_DMA_LISTEN;
        hi2c->State     = HAL_I2C_STATE_BUSY_TX;
        hi2c->Mode      = HAL_I2C_MODE_MASTER;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->Devaddress  = DevAddress;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = 0;
        HAL_DMA_RegisterCallback(hi2c->hdmatx, HAL_DMA_XFER_CPLT_CB_ID, I2C_DMAXferCplt);
        HAL_DMA_RegisterCallback(hi2c->hdmatx, HAL_DMA_XFER_ERROR_CB_ID, I2C_DMAError);
        HAL_DMA_RegisterCallback(hi2c->hdmatx, HAL_DMA_XFER_ABORT_CB_ID, I2C_DMAAbort);
        /* Master request configuration */
        I2C_MasterDMARequestConfig(hi2c, DevAddress, I2C_CMD_Master_Write);
        /* Enable the DMA stream */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmatx, (uint32_t)hi2c->pBuffPtr, (uint32_t)&hi2c->Instance->DATA_CMD, hi2c->XferSize);
        if (dmaxferstatus == HAL_OK)
        {
            // enable transmit FIFO DMA channel
            __HAL_I2C_DMA_TX_ENABLE(hi2c);
//            __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_MASTER_MASK);
            /* Process Unlocked */
            __HAL_UNLOCK(hi2c);
        }
        else
        {
            /* Update I2C state */
            hi2c->State     = HAL_I2C_STATE_READY;
            hi2c->Mode      = HAL_I2C_MODE_NONE;
            /* Update I2C error code */
            hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;
            /* Process Unlocked */
            __HAL_UNLOCK(hi2c);
            return HAL_ERROR;
        }
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
#else
    (void)hi2c;
    (void)DevAddress;
    (void)pData;
    (void)Size;
    return HAL_ERROR;
#endif
}

/**
  * @brief  Receive in master mode an amount of data in non-blocking mode with DMA
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Receive_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
#if(defined __I2C_MASTER_RX_USE_DMA && __I2C_MASTER_RX_USE_DMA == 1U)
    __IO uint32_t count = 0U;
    HAL_StatusTypeDef dmaxferstatus;

    if(!(hi2c->Instance->COMP_PARAM_1 & I2C_COMP_PARAM_HAS_DMA) || pData == NULL || Size == 0)
    {
        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_SIZE;
        return HAL_ERROR;
    }
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        /* Process Locked */
        __HAL_LOCK(hi2c);
        /* Wait until BUSY flag is reset */
        count = I2C_TIMEOUT_BUSY_FLAG * (GetSystemCoreClock() / 25U / 1000U);
        while (__HAL_I2C_GET_STATUS(hi2c, I2C_STATUS_MST_ACTIVITY) != RESET)
        {
            count--;
            if (count == 0U)
            {
                hi2c->PreviousState       = I2C_STATE_NONE;
                hi2c->State               = HAL_I2C_STATE_READY;
                hi2c->Mode                = HAL_I2C_MODE_NONE;
                hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;
                /* Process Unlocked */
                __HAL_UNLOCK(hi2c);
                return HAL_ERROR;
            }
        }
        /* Prepare transfer parameters */
        hi2c->pRxBuffPtr  = pData;
        hi2c->DMAState    = HAL_I2C_STATE_DMA_LISTEN;
        hi2c->State       = HAL_I2C_STATE_BUSY_RX;
        hi2c->Mode        = HAL_I2C_MODE_MASTER;
        hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->Devaddress  = DevAddress;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = Size;
        /* Master request configuration */
        I2C_MasterDMARequestConfig(hi2c, DevAddress, I2C_CMD_Master_Read);

        HAL_DMA_RegisterCallback(hi2c->hdmarx, HAL_DMA_XFER_CPLT_CB_ID, I2C_DMAXferCplt);
        HAL_DMA_RegisterCallback(hi2c->hdmarx, HAL_DMA_XFER_ERROR_CB_ID, I2C_DMAError);
        HAL_DMA_RegisterCallback(hi2c->hdmarx, HAL_DMA_XFER_ABORT_CB_ID, I2C_DMAAbort);
        /* Enable the DMA stream */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmarx, (uint32_t)&hi2c->Instance->DATA_CMD, (uint32_t)hi2c->pRxBuffPtr, hi2c->XferSize);
        if (dmaxferstatus == HAL_OK)
        {
            /* Generate ACK for a General Call */
            SET_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
            // enable transmit FIFO DMA channel
            __HAL_I2C_DMA_RX_ENABLE(hi2c);
            __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_MASTER_MASK);
            /* Process Unlocked */
            __HAL_UNLOCK(hi2c);
        }
        else
        {
            /* Update I2C state */
            hi2c->State     = HAL_I2C_STATE_READY;
            hi2c->Mode      = HAL_I2C_MODE_NONE;
            /* Update I2C error code */
            hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;
            /* Process Unlocked */
            __HAL_UNLOCK(hi2c);
            return HAL_ERROR;
        }
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
#else
    (void)hi2c;
    (void)DevAddress;
    (void)pData;
    (void)Size;
    return HAL_ERROR;
#endif
}

/**
  * @brief  Transmit in slave mode an amount of data in non-blocking mode with DMA
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size)
{
#if(defined __I2C_SLAVE_TX_USE_DMA && __I2C_SLAVE_TX_USE_DMA == 1U)
    HAL_StatusTypeDef dmaxferstatus = HAL_OK;
    uint16_t *tempPtr = NULL;

    if(!(hi2c->Instance->COMP_PARAM_1 & I2C_COMP_PARAM_HAS_DMA) || (pData == NULL) || (Size == 0U))
    {
        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_SIZE;
        return HAL_ERROR;
    }
    if (hi2c->State == HAL_I2C_STATE_READY || hi2c->State == HAL_I2C_STATE_LISTEN)
    {
        /* Process Locked */
        __HAL_LOCK(hi2c);
        /* Wait until Slave activity flag is reset */
        if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_STATUS_SLV_ACTIVITY, SET, I2C_TIMEOUT_BUSY_FLAG) != HAL_OK)
        {
            return HAL_BUSY;
        }
        hi2c->pBuffPtr  = pData;
        hi2c->DMAState  = HAL_I2C_STATE_DMA_LISTEN;
        hi2c->State     = HAL_I2C_STATE_BUSY_TX;
        hi2c->Mode      = HAL_I2C_MODE_SLAVE;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
        /* Prepare transfer parameters */
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = 0;
        HAL_DMA_RegisterCallback(hi2c->hdmatx, HAL_DMA_XFER_CPLT_CB_ID, I2C_DMAXferCplt);
        HAL_DMA_RegisterCallback(hi2c->hdmatx, HAL_DMA_XFER_ERROR_CB_ID, I2C_DMAError);
        HAL_DMA_RegisterCallback(hi2c->hdmatx, HAL_DMA_XFER_ABORT_CB_ID, I2C_DMAAbort);
        /* Slave request config */
        I2C_SlaveDMARequestConfig(hi2c);
        /* Enable the DMA stream */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmatx, (uint32_t)hi2c->pBuffPtr, (uint32_t)&hi2c->Instance->DATA_CMD, hi2c->XferSize);
        if (dmaxferstatus == HAL_OK)
        {
            /* Generate ACK for a General Call */
            SET_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
            // enable transmit FIFO DMA channel
            //__HAL_I2C_DMA_TX_ENABLE(hi2c);
            __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_SLAVE_MASK);
            /* Process Unlocked */
            __HAL_UNLOCK(hi2c);
            return HAL_OK;
        }
        else
        {
            /* Update I2C state */
            hi2c->State     = HAL_I2C_STATE_READY;
            hi2c->Mode      = HAL_I2C_MODE_NONE;
            /* Update I2C error code */
            hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;
            /* Process Unlocked */
            __HAL_UNLOCK(hi2c);
            return HAL_ERROR;
        }
    }
    else
    {
        return HAL_BUSY;
    }
#else
    (void)hi2c;
    (void)pData;
    (void)Size;
    return HAL_ERROR;
#endif
}

/**
  * @brief  Receive in slave mode an amount of data in non-blocking mode with DMA
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Receive_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size)
{
#if(defined __I2C_SLAVE_RX_USE_DMA && __I2C_SLAVE_RX_USE_DMA == 1U)
    HAL_StatusTypeDef dmaxferstatus;

    if(!(hi2c->Instance->COMP_PARAM_1 & I2C_COMP_PARAM_HAS_DMA) || (pData == NULL) || (Size == 0U))
    {
        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_SIZE;
        return HAL_ERROR;
    }
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        /* Process Locked */
        __HAL_LOCK(hi2c);
        /* Wait until Slave activity flag is reset */
        if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_STATUS_SLV_ACTIVITY, SET, I2C_TIMEOUT_BUSY_FLAG) != HAL_OK)
        {
            return HAL_BUSY;
        }
        hi2c->DMAState  = HAL_I2C_STATE_DMA_LISTEN;
        hi2c->State     = HAL_I2C_STATE_BUSY_RX;
        hi2c->Mode      = HAL_I2C_MODE_SLAVE;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
        /* Prepare transfer parameters */
        hi2c->pRxBuffPtr  = pData;
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = 0;
        HAL_DMA_RegisterCallback(hi2c->hdmarx, HAL_DMA_XFER_CPLT_CB_ID, I2C_DMAXferCplt);
        HAL_DMA_RegisterCallback(hi2c->hdmarx, HAL_DMA_XFER_ERROR_CB_ID, I2C_DMAError);
        HAL_DMA_RegisterCallback(hi2c->hdmarx, HAL_DMA_XFER_ABORT_CB_ID, I2C_DMAAbort);
        /* Slave request config */
        I2C_SlaveRequestConfig(hi2c);
        /* Enable the DMA stream */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmarx, (uint32_t)&hi2c->Instance->DATA_CMD, (uint32_t)hi2c->pRxBuffPtr, hi2c->XferSize);
        if (dmaxferstatus == HAL_OK)
        {
            /* Generate ACK for a General Call */
            SET_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
            // enable transmit FIFO DMA channel
            __HAL_I2C_DMA_RX_ENABLE(hi2c);
//            __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_SLAVE_MASK);
            /* Process Unlocked */
            __HAL_UNLOCK(hi2c);
            return HAL_OK;
        }
        else
        {
            /* Update I2C state */
            hi2c->State     = HAL_I2C_STATE_READY;
            hi2c->Mode      = HAL_I2C_MODE_NONE;
            /* Update I2C error code */
            hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;
            /* Process Unlocked */
            __HAL_UNLOCK(hi2c);
            return HAL_ERROR;
        }
    }
    else
    {
        return HAL_BUSY;
    }
#else
    (void)hi2c;
    (void)pData;
    (void)Size;
    return HAL_ERROR;
#endif
}

/**
  * @brief  Write an amount of data in blocking mode to a specific memory address
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    /* Init tickstart for timeout management*/
    uint32_t tickstart = HAL_GetTick();
    uint32_t tempreg = 0;
    uint32_t tx_limit = 0;
    uint8_t mem_addr_send_cnt = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
    uint8_t need_restart = 0;
    if(hi2c->Init.ReStartEn)
    {
        need_restart = 1;
    }
#endif

    /* Check the parameters */
    assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));

    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        /* Process Locked */
        __HAL_LOCK(hi2c);
        /* Wait until BUSY flag is reset */
        if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_STATUS_MST_ACTIVITY, SET, I2C_TIMEOUT_BUSY_FLAG) != HAL_OK)
        {
            return HAL_BUSY;
        }
        hi2c->State     = HAL_I2C_STATE_BUSY_TX;
        hi2c->Mode      = HAL_I2C_MODE_MEM;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
        /* Prepare transfer parameters */
        hi2c->pBuffPtr    = pData;
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = 0;
        
        /* Send Slave Address and Memory Address */
        if (I2C_RequestMemoryWrite(hi2c, DevAddress, MemAddress, MemAddSize, Timeout, tickstart) != HAL_OK)
        {
            return HAL_ERROR;
        }
        /* Master request configuration */
        // I2C_MasterRequestConfig(hi2c, DevAddress, I2C_CMD_Master_Write);
        while (hi2c->XferCount > 0U)
        {
            tx_limit = hi2c->Init.tx_fifo_depth - hi2c->Instance->TXFLR;
            while(tx_limit > 0 && hi2c->XferCount > 0U)
            {
                /* Write data to DATA_CMD */
                tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
                tempreg |= hi2c->XferCount == 1 ? I2C_DATA_CMD_STOP : 0;
                need_restart = 0;
#endif
                tempreg |= I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos;
                tempreg |= (uint32_t)(*hi2c->pBuffPtr);
                hi2c->Instance->DATA_CMD = tempreg;
                tx_limit--;
                /* Increment Buffer pointer */
                hi2c->pBuffPtr++;
                /* Update counter */
                hi2c->XferCount--;
            }
            /* Wait until TXE flag is set */
            if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout) != HAL_OK)
            {
                return HAL_ERROR;
            }
        }
        /* Wait until STOP flag is set */
        if (I2C_WaitOnSTOPFlagUntilTimeout(hi2c, Timeout) != HAL_OK)
        {
            return HAL_ERROR;
        }
        hi2c->State = HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;
        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Read an amount of data in blocking mode from a specific memory address
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    /* Init tickstart for timeout management*/
    uint32_t tickstart = HAL_GetTick();
    uint32_t tx_limit;
    uint32_t rx_valid;
    uint32_t tempreg;
    uint8_t mem_addr_send_cnt = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
    uint8_t need_restart = 0;
    if(hi2c->Init.ReStartEn)
    {
        need_restart = 1;
    }
#endif

    /* Check the parameters */
    assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        /* Process Locked */
        __HAL_LOCK(hi2c);
        /* Wait until Master activity flag is reset */
        if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_STATUS_MST_ACTIVITY, SET, I2C_TIMEOUT_BUSY_FLAG) != HAL_OK)
        {
            return HAL_BUSY;
        }
        hi2c->State       = HAL_I2C_STATE_BUSY_RX;
        hi2c->Mode        = HAL_I2C_MODE_MEM;
        hi2c->ErrorCode   = HAL_I2C_ERROR_NONE;
        /* Prepare transfer parameters */
        hi2c->pBuffPtr    = pData;
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = Size;
        /* Send Slave Address and Memory Address */
        if (I2C_RequestMemoryRead(hi2c, DevAddress, MemAddress, MemAddSize, Timeout, tickstart) != HAL_OK)
        {
            return HAL_ERROR;
        }
        /* Master request configuration */
        I2C_MasterRequestConfig(hi2c, DevAddress, I2C_CMD_Master_Read);
        while (hi2c->XferCount > 0U)
        {
            tx_limit = hi2c->Init.tx_fifo_depth - hi2c->Instance->TXFLR;
            while(tx_limit > 0 && hi2c->cmd_cnt < hi2c->cmd_max)
            {
                /* Write CMD to DATA_CMD */
                tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
                if(hi2c->cmd_cnt == hi2c->cmd_max - 1)
                {
                    tempreg |= I2C_DATA_CMD_STOP;
                }
                tempreg |= need_restart == 1 ? I2C_DATA_CMD_RESTART : 0;
                need_restart = 0;
#endif
                tempreg |= I2C_CMD_Master_Read << I2C_DATA_CMD_CMD_Pos;
                hi2c->Instance->DATA_CMD = tempreg;
                tx_limit--;
                hi2c->cmd_cnt++;
            }
            rx_valid = hi2c->Instance->RXFLR;
            for (; rx_valid > 0 && hi2c->XferCount > 0U; rx_valid--)
            {
                /* Read data from DR */
                *hi2c->pBuffPtr = (uint8_t)(hi2c->Instance->DATA_CMD & 0xFF);
                /* Increment Buffer pointer */
                hi2c->pBuffPtr++;
                /* Update counter */
                hi2c->XferCount--;
            }
            if(Timeout != HAL_MAX_DELAY)
            {
                if((HAL_GetTick() - tickstart) > Timeout)
                {
                    hi2c->State = HAL_I2C_STATE_READY;
                    hi2c->Mode = HAL_I2C_MODE_NONE;
                    /* Process Unlocked */
                    __HAL_UNLOCK(hi2c);
                    return HAL_TIMEOUT;
                }
            }
        }
        /* Wait until STOP flag is set */
        if (I2C_WaitOnSTOPFlagUntilTimeout(hi2c, Timeout) != HAL_OK)
        {
            return HAL_ERROR;
        }
        hi2c->State = HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;
        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Write an amount of data in non-blocking mode with Interrupt to a specific memory address
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Mem_Write_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
    __IO uint32_t count = 0U;
    uint32_t tx_limit = 0;
    uint32_t tempreg = 0;
    uint32_t mem_addr_send_cnt = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
    uint8_t need_restart = 0;
    if(hi2c->Init.ReStartEn)
    {
        need_restart = 1;
    }
#endif
    /* Check the parameters */
    assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));
    if (Size == 0U)
    {
        return HAL_ERROR;
    }
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        /* Process Locked */
        __HAL_LOCK(hi2c);
        /* Wait until BUSY flag is reset */
        count = I2C_TIMEOUT_BUSY_FLAG * (GetSystemCoreClock() / 25U / 1000U);
        while (__HAL_I2C_GET_STATUS(hi2c, I2C_STATUS_MST_ACTIVITY) != RESET)
        {
            count--;
            if (count == 0U)
            {
                hi2c->PreviousState       = I2C_STATE_NONE;
                hi2c->State               = HAL_I2C_STATE_READY;
                hi2c->Mode                = HAL_I2C_MODE_NONE;
                hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;
                /* Process Unlocked */
                __HAL_UNLOCK(hi2c);
                return HAL_ERROR;
            }
        }
        hi2c->State     = HAL_I2C_STATE_BUSY_TX;
        hi2c->Mode      = HAL_I2C_MODE_MEM;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
        /* Prepare transfer parameters */
        hi2c->pBuffPtr    = pData;
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->Devaddress  = DevAddress;
        hi2c->Memaddress  = MemAddress;
        hi2c->MemaddSize  = MemAddSize;
        hi2c->MemStage = I2C_MEM_STAGE_START;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = 0;
        /* Master request configuration */
        I2C_MasterRequestConfig(hi2c, DevAddress, I2C_CMD_Master_Write);
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_MASTER_MASK);
        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Read an amount of data in non-blocking mode with Interrupt from a specific memory address
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Mem_Read_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
    __IO uint32_t count = 0U;
    uint32_t tx_limit = 0;
    uint32_t tempreg = 0;
    uint32_t mem_addr_send_cnt = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
    uint8_t need_restart = 0;
    if(hi2c->Init.ReStartEn)
    {
        need_restart = 1;
    }
#endif
    /* Check the parameters */
    assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));
    if (Size == 0U)
    {
        return HAL_ERROR;
    }
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        /* Process Locked */
        __HAL_LOCK(hi2c);
        /* Wait until BUSY flag is reset */
        count = I2C_TIMEOUT_BUSY_FLAG * (GetSystemCoreClock() / 25U / 1000U);
        while (__HAL_I2C_GET_STATUS(hi2c, I2C_STATUS_MST_ACTIVITY) != RESET)
        {
            count--;
            if (count == 0U)
            {
                hi2c->PreviousState       = I2C_STATE_NONE;
                hi2c->State               = HAL_I2C_STATE_READY;
                hi2c->Mode                = HAL_I2C_MODE_NONE;
                hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;
                /* Process Unlocked */
                __HAL_UNLOCK(hi2c);
                return HAL_ERROR;
            }
        }
        hi2c->State     = HAL_I2C_STATE_BUSY_RX;
        hi2c->Mode      = HAL_I2C_MODE_MEM;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

        /* Prepare transfer parameters */
        hi2c->pBuffPtr    = pData;
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->Devaddress  = DevAddress;
        hi2c->Memaddress  = MemAddress;
        hi2c->MemaddSize  = MemAddSize;
        hi2c->MemStage  = I2C_MEM_STAGE_START;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = Size;
        /* Master request configuration */
        I2C_MasterRequestConfig(hi2c, DevAddress, I2C_CMD_Master_Write);
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_MASTER_MASK);
        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Write an amount of data in non-blocking mode with DMA to a specific memory address
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Mem_Write_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
#if(defined BSP_I2C_TXRX_DMA_ENABLED && BSP_I2C_TXRX_DMA_ENABLED == 1U)
    __IO uint32_t count = 0U;
    HAL_StatusTypeDef dmaxferstatus;
    uint16_t *tempPtr = NULL;
    uint32_t tx_limit = 0;
    uint32_t tempreg = 0;
    uint32_t mem_addr_send_cnt = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
    uint8_t need_restart = 0;
    if(hi2c->Init.ReStartEn)
    {
        need_restart = 1;
    }
#endif
    /* Check the parameters */
    assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));

    if(!(hi2c->Instance->COMP_PARAM_1 & I2C_COMP_PARAM_HAS_DMA) || Size == 0)
    {
        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_SIZE;
        return HAL_ERROR;
    }
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        /* Process Locked */
        __HAL_LOCK(hi2c);
        /* Wait until BUSY flag is reset */
        count = I2C_TIMEOUT_BUSY_FLAG * (GetSystemCoreClock() / 25U / 1000U);
        while (__HAL_I2C_GET_STATUS(hi2c, I2C_STATUS_MST_ACTIVITY) != RESET)
        {
            count--;
            if (count == 0U)
            {
                hi2c->PreviousState       = I2C_STATE_NONE;
                hi2c->State               = HAL_I2C_STATE_READY;
                hi2c->Mode                = HAL_I2C_MODE_NONE;
                hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;
                /* Process Unlocked */
                __HAL_UNLOCK(hi2c);
                return HAL_ERROR;
            }
        }
        /* Prepare transfer parameters */
        hi2c->pBuffPtr  = pData;
        hi2c->DMAState  = HAL_I2C_STATE_DMA_LISTEN;
        hi2c->State     = HAL_I2C_STATE_BUSY_TX;
        hi2c->Mode      = HAL_I2C_MODE_MEM;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = 0;
        HAL_DMA_RegisterCallback(hi2c->hdmatx, HAL_DMA_XFER_CPLT_CB_ID, I2C_DMAXferCplt);
        HAL_DMA_RegisterCallback(hi2c->hdmatx, HAL_DMA_XFER_ERROR_CB_ID, I2C_DMAError);
        HAL_DMA_RegisterCallback(hi2c->hdmatx, HAL_DMA_XFER_ABORT_CB_ID, I2C_DMAAbort);
        /* Master request configuration */
        I2C_MasterDMARequestConfig(hi2c, DevAddress, I2C_CMD_Master_Write);
        while(1)
        {
            tx_limit = hi2c->Init.tx_fifo_depth - hi2c->Instance->TXFLR;
            if(tx_limit > 0)
            {
                /* If Memory address size is 8Bit */
                if (MemAddSize == I2C_MEMADD_SIZE_8BIT)
                {
                    /* Send Memory Address */
                    tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
                    tempreg |= (need_restart == 1 ? I2C_DATA_CMD_RESTART : 0);
                    need_restart = 0;
#endif
                    tempreg |= (I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos);
                    tempreg |= I2C_MEM_ADD_LSB(MemAddress);
                    hi2c->Instance->DATA_CMD = tempreg;
                    break;
                }
                /* If Memory address size is 16Bit */
                else
                {
                    if(mem_addr_send_cnt == 0)
                    {
                        /* Send MSB of Memory Address */
                        tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
                        tempreg |= (need_restart == 1 ? I2C_DATA_CMD_RESTART : 0);
                        need_restart = 0;
#endif
                        tempreg |= (I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos);
                        tempreg |= I2C_MEM_ADD_MSB(MemAddress);
                        hi2c->Instance->DATA_CMD = tempreg;
                        mem_addr_send_cnt ++;
                    }
                    else if(mem_addr_send_cnt == 1)
                    {
                        /* Send MSB of Memory Address */
                        tempreg = 0;
                        tempreg |= (I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos);
                        tempreg |= I2C_MEM_ADD_LSB(MemAddress);
                        hi2c->Instance->DATA_CMD = tempreg;
                        mem_addr_send_cnt ++;
                        break;
                    }
                }
            }
        }
        /* Enable the DMA stream */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmatx, (uint32_t)hi2c->pBuffPtr, (uint32_t)&hi2c->Instance->DATA_CMD, hi2c->XferSize);
        if (dmaxferstatus == HAL_OK)
        {
            // enable transmit FIFO DMA channel
            __HAL_I2C_DMA_TX_ENABLE(hi2c);
//            __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_MASTER_MASK);
            /* Process Unlocked */
            __HAL_UNLOCK(hi2c);
            return HAL_OK;
        }
        else
        {
            /* Update I2C state */
            hi2c->State     = HAL_I2C_STATE_READY;
            hi2c->Mode      = HAL_I2C_MODE_NONE;
            /* Update I2C error code */
            hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;
            /* Process Unlocked */
            __HAL_UNLOCK(hi2c);
            return HAL_ERROR;
        }
    }
    else
    {
        return HAL_BUSY;
    }
#else
    (void)hi2c;
    (void)DevAddress;
    (void)MemAddress;
    (void)MemAddSize;
    (void)pData;
    (void)Size;
    return HAL_ERROR;
#endif
}

/**
  * @brief  Reads an amount of data in non-blocking mode with DMA from a specific memory address.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be read
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Mem_Read_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size)
{
#if(defined BSP_I2C_TXRX_DMA_ENABLED && BSP_I2C_TXRX_DMA_ENABLED == 1U)
    uint32_t tx_limit = 0;
    uint32_t tempreg = 0;
    uint32_t mem_addr_send_cnt = 0;
    __IO uint32_t count = 0U;
    HAL_StatusTypeDef dmaxferstatus;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
    uint8_t need_restart = 0;
    if(hi2c->Init.ReStartEn)
    {
        need_restart = 1;
    }
#endif

    /* Check the parameters */
    assert_param(IS_I2C_MEMADD_SIZE(MemAddSize));
    if(!(hi2c->Instance->COMP_PARAM_1 & I2C_COMP_PARAM_HAS_DMA) || Size == 0)
    {
        /* Update I2C error code */
        hi2c->ErrorCode |= HAL_I2C_ERROR_SIZE;
        return HAL_ERROR;
    }
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        /* Process Locked */
        __HAL_LOCK(hi2c);
        /* Wait until BUSY flag is reset */
        count = I2C_TIMEOUT_BUSY_FLAG * (GetSystemCoreClock() / 25U / 1000U);
        while (__HAL_I2C_GET_STATUS(hi2c, I2C_STATUS_MST_ACTIVITY) != RESET)
        {
            count--;
            if (count == 0U)
            {
                hi2c->PreviousState       = I2C_STATE_NONE;
                hi2c->State               = HAL_I2C_STATE_READY;
                hi2c->Mode                = HAL_I2C_MODE_NONE;
                hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;
                /* Process Unlocked */
                __HAL_UNLOCK(hi2c);
                return HAL_ERROR;
            }
        }
        /* Prepare transfer parameters */
        hi2c->pRxBuffPtr = pData;
        hi2c->DMAState  = HAL_I2C_STATE_DMA_LISTEN;
        hi2c->State     = HAL_I2C_STATE_BUSY_RX;
        hi2c->Mode      = HAL_I2C_MODE_MEM;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
        /* Prepare transfer parameters */
        hi2c->XferCount   = Size;
        hi2c->XferSize    = hi2c->XferCount;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        hi2c->MemStage    = I2C_MEM_STAGE_ADDR_READ;
        hi2c->cmd_cnt = 0;
        hi2c->cmd_max = Size;
        /* Send Slave Address and Memory Address */
        if (I2C_RequestMemoryRead(hi2c, DevAddress, MemAddress, MemAddSize, I2C_TIMEOUT_FLAG, HAL_GetTick()) != HAL_OK)
        {
            return HAL_ERROR;
        }

        HAL_DMA_RegisterCallback(hi2c->hdmarx, HAL_DMA_XFER_CPLT_CB_ID, I2C_DMAXferCplt);
        HAL_DMA_RegisterCallback(hi2c->hdmarx, HAL_DMA_XFER_ERROR_CB_ID, I2C_DMAError);
        HAL_DMA_RegisterCallback(hi2c->hdmarx, HAL_DMA_XFER_ABORT_CB_ID, I2C_DMAAbort);
        /* Master request configuration */
        I2C_MasterDMARequestConfig(hi2c, DevAddress, I2C_CMD_Master_Read);
        /* Enable the DMA stream */
        dmaxferstatus = HAL_DMA_Start_IT(hi2c->hdmarx, (uint32_t)&hi2c->Instance->DATA_CMD, (uint32_t)hi2c->pRxBuffPtr, hi2c->XferSize);
        if (dmaxferstatus == HAL_OK)
        {
            /* Generate NACK for a General Call */
            SET_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
            // enable receiver FIFO DMA channel
            __HAL_I2C_DMA_RX_ENABLE(hi2c);
            __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_MASTER_MASK);
            /* Process Unlocked */
            __HAL_UNLOCK(hi2c);
        }
        else
        {
            /* Update I2C state */
            hi2c->State     = HAL_I2C_STATE_READY;
            hi2c->Mode      = HAL_I2C_MODE_NONE;
            /* Update I2C error code */
            hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;
            /* Process Unlocked */
            __HAL_UNLOCK(hi2c);
            return HAL_ERROR;
        }
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
#else
    (void)hi2c;
    (void)DevAddress;
    (void)MemAddress;
    (void)MemAddSize;
    (void)pData;
    (void)Size;
    return HAL_ERROR;
#endif
}

/**
  * @brief  Checks if target device is ready for communication.
  * @note   This function is used with Memory devices
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  Trials Number of trials
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_IsDeviceReady(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout)
{
    uint32_t I2C_Trials = 1U;
    uint32_t tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
    uint8_t need_restart = 0;
    if(hi2c->Init.ReStartEn)
    {
        need_restart = 1;
    }
#endif

    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        __HAL_LOCK(hi2c);
        /* Wait until BUSY flag is reset */
        if (I2C_WaitOnFlagUntilTimeout(hi2c, I2C_STATUS_MST_ACTIVITY, SET, I2C_TIMEOUT_BUSY_FLAG) != HAL_OK)
        {
            return HAL_BUSY;
        }
        hi2c->State = HAL_I2C_STATE_BUSY;
        hi2c->ErrorCode = HAL_I2C_ERROR_NONE;
        hi2c->XferOptions = I2C_NO_OPTION_FRAME;
        /* Master request configuration */
        I2C_MasterRequestConfig(hi2c, DevAddress, I2C_CMD_Master_Write);
        do
        {
            /* Send Memory Address */
            tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
            tempreg |= need_restart == 1 ? I2C_DATA_CMD_RESTART : 0;
            need_restart = 0;
#endif
            tempreg |= I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos;
            hi2c->Instance->DATA_CMD = tempreg;
            /* Wait until STOP flag is set */
            if (I2C_WaitOnSTOPFlagUntilTimeout(hi2c, Timeout) == HAL_OK)
            {
                hi2c->State = HAL_I2C_STATE_READY;
                /* Process Unlocked */
                __HAL_UNLOCK(hi2c);
                return HAL_OK;
            }
            /* Increment Trials */
            I2C_Trials++;
        }while (I2C_Trials < Trials);
        hi2c->State = HAL_I2C_STATE_READY;
        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);
        return HAL_ERROR;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Sequential transmit in master I2C mode an amount of data in non-blocking mode with Interrupt.
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Seq_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
    /* Check the parameters */
    assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));
    (void)hi2c;
    (void)DevAddress;
    (void)pData;
    (void)Size;
    (void)XferOptions;
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Sequential transmit in master I2C mode an amount of data in non-blocking mode with DMA.
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Seq_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
    /* Check the parameters */
    assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));
    (void)hi2c;
    (void)DevAddress;
    (void)pData;
    (void)Size;
    (void)XferOptions;
    if(!(hi2c->Instance->COMP_PARAM_1 & I2C_COMP_PARAM_HAS_DMA))
    {
        return HAL_ERROR;
    }
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Sequential receive in master I2C mode an amount of data in non-blocking mode with Interrupt
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Seq_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
    /* Check the parameters */
    assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));
    (void)hi2c;
    (void)DevAddress;
    (void)pData;
    (void)Size;
    (void)XferOptions;
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Sequential receive in master mode an amount of data in non-blocking mode with DMA
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Seq_Receive_DMA(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
    /* Check the parameters */
    assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));
    if(!(hi2c->Instance->COMP_PARAM_1 & I2C_COMP_PARAM_HAS_DMA))
    {
        return HAL_ERROR;
    }
    (void)hi2c;
    (void)DevAddress;
    (void)pData;
    (void)Size;
    (void)XferOptions;
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Sequential transmit in slave mode an amount of data in non-blocking mode with Interrupt
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Transmit_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
    /* Check the parameters */
    assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));
    (void)hi2c;
    (void)pData;
    (void)Size;
    (void)XferOptions;
    if (((uint32_t)hi2c->State & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)
    {
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Sequential transmit in slave mode an amount of data in non-blocking mode with DMA
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
    /* Check the parameters */
    assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));
    (void)hi2c;
    (void)pData;
    (void)Size;
    (void)XferOptions;
    if(!(hi2c->Instance->COMP_PARAM_1 & I2C_COMP_PARAM_HAS_DMA))
    {
        return HAL_ERROR;
    }
    if (((uint32_t)hi2c->State & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)
    {
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Sequential receive in slave mode an amount of data in non-blocking mode with Interrupt
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Receive_IT(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
    /* Check the parameters */
    assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));
    (void)hi2c;
    (void)pData;
    (void)Size;
    (void)XferOptions;
    if (((uint32_t)hi2c->State & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)
    {
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Sequential receive in slave mode an amount of data in non-blocking mode with DMA
  * @note   This interface allow to manage repeated start condition when a direction change during transfer
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  pData Pointer to data buffer
  * @param  Size Amount of data to be sent
  * @param  XferOptions Options of Transfer, value of @ref I2C_XferOptions_definition
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Slave_Seq_Receive_DMA(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t XferOptions)
{
    HAL_StatusTypeDef dmaxferstatus;

    /* Check the parameters */
    assert_param(IS_I2C_TRANSFER_OPTIONS_REQUEST(XferOptions));
    (void)hi2c;
    (void)pData;
    (void)Size;
    (void)XferOptions;
    if(!(hi2c->Instance->COMP_PARAM_1 & I2C_COMP_PARAM_HAS_DMA))
    {
        return HAL_ERROR;
    }
    if (((uint32_t)hi2c->State & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)
    {
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Enable the Address listen mode with Interrupt.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_EnableListen_IT(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->State == HAL_I2C_STATE_READY)
    {
        /* Process Locked */
        __HAL_LOCK(hi2c);
        hi2c->State = HAL_I2C_STATE_LISTEN;
        hi2c->Mode = HAL_I2C_MODE_SLAVE;
        /* Slave request config */
        I2C_SlaveRequestConfig(hi2c);
        /* Enable Address Acknowledge */
        SET_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_GEN_CALL | I2C_IT_RD_REQ);
        /* Process Locked */
        __HAL_UNLOCK(hi2c);
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Disable the Address listen mode with Interrupt.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_DisableListen_IT(I2C_HandleTypeDef *hi2c)
{
    /* Disable Address listen mode only if a transfer is not ongoing */
    if (hi2c->State == HAL_I2C_STATE_LISTEN)
    {
        /* Process Locked */
        __HAL_LOCK(hi2c);
        /* Slave request config */
        I2C_SlaveRequestConfig(hi2c);
        /* Enable Address Acknowledge */
        CLEAR_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
        __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_GEN_CALL | I2C_IT_RD_REQ);
        hi2c->State = HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;
        /* Process Locked */
        __HAL_UNLOCK(hi2c);
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Abort a master I2C IT or DMA process communication with Interrupt.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_I2C_Master_Abort_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(DevAddress);

    /* Abort Master transfer during Receive or Transmit process    */
    if (hi2c->Mode == HAL_I2C_MODE_MASTER)
    {
        /* Process Locked */
        __HAL_LOCK(hi2c);
        hi2c->PreviousState = I2C_STATE_NONE;
        hi2c->State = HAL_I2C_STATE_ABORT;
        hi2c->XferCount = 0U;
        /* Disable EVT, BUF and ERR interrupt */
        __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_MASTER_MASK);
        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);
        /* Call the corresponding callback to inform upper layer of End of Transfer */
        I2C_ITError(hi2c);
        return HAL_OK;
    }
    else
    {
        /* Wrong usage of abort function */
        /* This function should be used only in case of abort monitored by master device */
        return HAL_ERROR;
    }
}

/**
  * @brief  I2C interrupts error process
  * @param  hi2c I2C handle.
  * @retval None
  */
static void I2C_ITError(I2C_HandleTypeDef *hi2c)
{
//     /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
//     HAL_I2C_StateTypeDef CurrentState = hi2c->State;
//     if ((hi2c->Mode == HAL_I2C_MODE_MASTER) && (CurrentState == HAL_I2C_STATE_BUSY_RX))
//     {
//         /* Disable Pos bit in I2C CR1 when error occurred in Master/Mem Receive IT Process */
//         hi2c->Instance->CR1 &= ~I2C_CR1_POS;
//     }
//     if (((uint32_t)CurrentState & (uint32_t)HAL_I2C_STATE_LISTEN) == (uint32_t)HAL_I2C_STATE_LISTEN)
//     {
//         /* keep HAL_I2C_STATE_LISTEN */
//         hi2c->PreviousState = I2C_STATE_NONE;
//         hi2c->State = HAL_I2C_STATE_LISTEN;
//     }
//     else
//     {
//         /* If state is an abort treatment on going, don't change state */
//         /* This change will be do later */
//         if ((READ_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN) != I2C_CR2_DMAEN) && (CurrentState != HAL_I2C_STATE_ABORT))
//         {
//             hi2c->State = HAL_I2C_STATE_READY;
//         }
//         hi2c->PreviousState = I2C_STATE_NONE;
//         hi2c->Mode = HAL_I2C_MODE_NONE;
//     }

//     /* Abort DMA transfer */
//     if (READ_BIT(hi2c->Instance->CR2, I2C_CR2_DMAEN) == I2C_CR2_DMAEN)
//     {
//         hi2c->Instance->CR2 &= ~I2C_CR2_DMAEN;

//         if (hi2c->hdmatx->State != HAL_DMA_STATE_READY)
//         {
//             /* Set the DMA Abort callback :
//             will lead to call HAL_I2C_ErrorCallback() at end of DMA abort procedure */
//             hi2c->hdmatx->XferAbortCallback = I2C_DMAAbort;

//             if (HAL_DMA_Abort_IT(hi2c->hdmatx) != HAL_OK)
//             {
//                 /* Disable I2C peripheral to prevent dummy data in buffer */
//                 __HAL_I2C_DISABLE(hi2c);

//                 hi2c->State = HAL_I2C_STATE_READY;

//                 /* Call Directly XferAbortCallback function in case of error */
//                 hi2c->hdmatx->XferAbortCallback(hi2c->hdmatx);
//             }
//         }
//         else
//         {
//             /* Set the DMA Abort callback :
//             will lead to call HAL_I2C_ErrorCallback() at end of DMA abort procedure */
//             hi2c->hdmarx->XferAbortCallback = I2C_DMAAbort;

//             if (HAL_DMA_Abort_IT(hi2c->hdmarx) != HAL_OK)
//             {
//                 /* Store Last receive data if any */
//                 if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == SET)
//                 {
//                     /* Read data from DR */
//                     *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

//                     /* Increment Buffer pointer */
//                     hi2c->pBuffPtr++;
//                 }

//                 /* Disable I2C peripheral to prevent dummy data in buffer */
//                 __HAL_I2C_DISABLE(hi2c);

//                 hi2c->State = HAL_I2C_STATE_READY;

//                 /* Call Directly hi2c->hdmarx->XferAbortCallback function in case of error */
//                 hi2c->hdmarx->XferAbortCallback(hi2c->hdmarx);
//             }
//         }
//     }
//     else if (hi2c->State == HAL_I2C_STATE_ABORT)
//     {
//         hi2c->State = HAL_I2C_STATE_READY;
//         hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

//         /* Store Last receive data if any */
//         if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == SET)
//         {
//             /* Read data from DR */
//             *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

//             /* Increment Buffer pointer */
//             hi2c->pBuffPtr++;
//         }

//         /* Disable I2C peripheral to prevent dummy data in buffer */
//         __HAL_I2C_DISABLE(hi2c);

//         /* Call the corresponding callback to inform upper layer of End of Transfer */
// #if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
//         hi2c->AbortCpltCallback(hi2c);
// #else
//         HAL_I2C_AbortCpltCallback(hi2c);
// #endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
//     }
//     else
//     {
//         /* Store Last receive data if any */
//         if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == SET)
//         {
//             /* Read data from DR */
//             *hi2c->pBuffPtr = (uint8_t)hi2c->Instance->DR;

//             /* Increment Buffer pointer */
//             hi2c->pBuffPtr++;
//         }

//         /* Call user error callback */
// #if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
//         hi2c->ErrorCallback(hi2c);
// #else
//         HAL_I2C_ErrorCallback(hi2c);
// #endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
//     }
//     /* STOP Flag is not set after a NACK reception */
//     /* So may inform upper layer that listen phase is stopped */
//     /* during NACK error treatment */
//     CurrentState = hi2c->State;
//     if (((hi2c->ErrorCode & HAL_I2C_ERROR_AF) == HAL_I2C_ERROR_AF) && (CurrentState == HAL_I2C_STATE_LISTEN))
//     {
//         /* Disable EVT, BUF and ERR interrupt */
//         __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR);

//         hi2c->XferOptions   = I2C_NO_OPTION_FRAME;
//         hi2c->PreviousState = I2C_STATE_NONE;
//         hi2c->State         = HAL_I2C_STATE_READY;
//         hi2c->Mode          = HAL_I2C_MODE_NONE;

//         /* Call the Listen Complete callback, to inform upper layer of the end of Listen usecase */
// #if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
//         hi2c->ListenCpltCallback(hi2c);
// #else
//         HAL_I2C_ListenCpltCallback(hi2c);
// #endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
//     }
}
/**
  * @}
  */

/** @defgroup I2C_IRQ_Handler_and_Callbacks IRQ Handler and Callbacks
 * @{
 */

static void I2C_RXHandle(I2C_HandleTypeDef *hi2c)
{
    uint32_t rx_valid = 0;
    uint32_t len = 0;
    // Master or Mem or Slave RX process
    if(hi2c->State == HAL_I2C_STATE_BUSY_RX)
    {
        len = hi2c->XferCount;
        rx_valid = hi2c->Instance->RXFLR;
        for (; len > 0 && rx_valid > 0; len--, rx_valid--)
        {
            /* Read data from DR */
            *hi2c->pBuffPtr = (uint8_t)(hi2c->Instance->DATA_CMD & 0xFF);
            /* Increment Buffer pointer */
            hi2c->pBuffPtr++;
            /* Update counter */
            hi2c->XferCount--;
        }
        hi2c->gstatus &= ~STATUS_WRITE_IN_PROGRESS;
    }
}
static void I2C_TXHandle_MemWrite(I2C_HandleTypeDef *hi2c)
{
    uint32_t tempreg = 0;
    int tx_limit = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
    uint8_t need_restart = 0;
    if((hi2c->gstatus | STATUS_WRITE_IN_PROGRESS) != STATUS_WRITE_IN_PROGRESS)
    {
        if(hi2c->Init.ReStartEn)
        {
            need_restart = 1;
        }
    }
#endif
    tx_limit = hi2c->Init.tx_fifo_depth - hi2c->Instance->TXFLR;
    while(tx_limit > 0 && hi2c->XferCount > 0U)
    {
        if (hi2c->MemStage < 2U)
        {
            /* If Memory address size is 8Bit */
            if(hi2c->MemaddSize == I2C_MEMADD_SIZE_8BIT)
            {
                /* Send Memory Address */
                tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
                tempreg |= (need_restart == 1 ? I2C_DATA_CMD_RESTART : 0);
                need_restart = 0;
#endif
                tempreg |= (I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos);
                tempreg |= I2C_MEM_ADD_LSB(hi2c->Memaddress);
                hi2c->Instance->DATA_CMD = tempreg;
                hi2c->MemStage = 2;
                tx_limit--;
            }
            /* If Memory address size is 16Bit */
            else if(hi2c->MemaddSize == I2C_MEMADD_SIZE_16BIT)
            {
                if(hi2c->MemStage == 0)
                {
                    /* Send MSB of Memory Address */
                    tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
                    tempreg |= (need_restart == 1 ? I2C_DATA_CMD_RESTART : 0);
                    need_restart = 0;
#endif
                    tempreg |= (I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos);
                    tempreg |= I2C_MEM_ADD_MSB(hi2c->Memaddress);
                    hi2c->Instance->DATA_CMD = tempreg;
                    hi2c->MemStage ++;
                    tx_limit--;
                }
                else if(hi2c->MemStage == 1)
                {
                    /* Send MSB of Memory Address */
                    tempreg = 0;
                    tempreg |= (I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos);
                    tempreg |= I2C_MEM_ADD_LSB(hi2c->Memaddress);
                    hi2c->Instance->DATA_CMD = tempreg;
                    hi2c->MemStage ++;
                    tx_limit--;
                }
            }
        }
        else
        {
            /* Write data to DATA_CMD */
            tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
            tempreg |= hi2c->XferCount == 1 ? I2C_DATA_CMD_STOP : 0;
            tempreg |= need_restart == 1 ? I2C_DATA_CMD_RESTART : 0;
            need_restart = 0;
#endif
            tempreg |= I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos;
            tempreg |= (uint32_t)(*hi2c->pBuffPtr);
            hi2c->Instance->DATA_CMD = tempreg;
            tx_limit--;
            /* Increment Buffer pointer */
            hi2c->pBuffPtr++;
            /* Update counter */
            hi2c->XferCount--;
        }
    }
    if(hi2c->XferCount > 0U)
    {
        /* more bytes to be written */
        hi2c->gstatus |= STATUS_WRITE_IN_PROGRESS;
    }
    else
    {
        hi2c->gstatus &= ~STATUS_WRITE_IN_PROGRESS;
    }
}
static void I2C_TXHandle_MemRead(I2C_HandleTypeDef *hi2c)
{
    uint32_t tempreg = 0;
    int tx_limit = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
    uint8_t need_restart = 0;
    if((hi2c->gstatus | STATUS_WRITE_IN_PROGRESS) != STATUS_WRITE_IN_PROGRESS)
    {
        if(hi2c->Init.ReStartEn)
        {
            need_restart = 1;
        }
    }
#endif
    tx_limit = hi2c->Init.tx_fifo_depth - hi2c->Instance->TXFLR;
    while(tx_limit > 0 && hi2c->cmd_cnt < hi2c->cmd_max)
    {
        if (hi2c->MemStage == I2C_MEM_STAGE_START || 
            hi2c->MemStage == I2C_MEM_STAGE_ADDR_MSB)
        {
            /* If Memory address size is 8Bit */
            if(hi2c->MemaddSize == I2C_MEMADD_SIZE_8BIT)
            {
                /* Send Memory Address */
                tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
                tempreg |= (need_restart == 1 ? I2C_DATA_CMD_RESTART : 0);
                need_restart = 0;
#endif
                tempreg |= (I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos);
                tempreg |= I2C_MEM_ADD_LSB(hi2c->Memaddress);
                hi2c->Instance->DATA_CMD = tempreg;
                hi2c->MemStage = I2C_MEM_STAGE_ADDR_LSB;
                tx_limit--;
            }
            /* If Memory address size is 16Bit */
            else if(hi2c->MemaddSize == I2C_MEMADD_SIZE_16BIT)
            {
                if(hi2c->MemStage == I2C_MEM_STAGE_START)
                {
                    /* Send MSB of Memory Address */
                    tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
                    tempreg |= (need_restart == 1 ? I2C_DATA_CMD_RESTART : 0);
                    need_restart = 0;
#endif
                    tempreg |= (I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos);
                    tempreg |= I2C_MEM_ADD_MSB(hi2c->Memaddress);
                    hi2c->Instance->DATA_CMD = tempreg;
                    hi2c->MemStage = I2C_MEM_STAGE_ADDR_MSB;
                    tx_limit--;
                }
                else if(hi2c->MemStage == I2C_MEM_STAGE_ADDR_MSB)
                {
                    /* Send MSB of Memory Address */
                    tempreg = 0;
                    tempreg |= (I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos);
                    tempreg |= I2C_MEM_ADD_LSB(hi2c->Memaddress);
                    hi2c->Instance->DATA_CMD = tempreg;
                    hi2c->MemStage = I2C_MEM_STAGE_ADDR_LSB;
                    tx_limit--;
                }
            }
        }
        else if(hi2c->MemStage == I2C_MEM_STAGE_ADDR_READ)
        {
            /* Write CMD to DATA_CMD */
            tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
            if(hi2c->cmd_cnt == hi2c->cmd_max - 1)
            {
                tempreg |= I2C_DATA_CMD_STOP;
            }
            tempreg |= need_restart == 1 ? I2C_DATA_CMD_RESTART : 0;
            need_restart = 0;
#endif
            tempreg |= I2C_CMD_Master_Read << I2C_DATA_CMD_CMD_Pos;
            hi2c->Instance->DATA_CMD = tempreg;
            tx_limit--;
            hi2c->cmd_cnt++;
        }
        else// wait stop signal send over
        {
            __HAL_I2C_DISABLE_IT_ALL(hi2c);
            __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_STOP_DET);
            break;
        }
    }
    if(hi2c->XferCount > 0U)
    {
        /* more bytes to be written */
        hi2c->gstatus |= STATUS_WRITE_IN_PROGRESS;
    }
    else
    {
        hi2c->gstatus &= ~STATUS_WRITE_IN_PROGRESS;
    }
}
static void I2C_TXHandle_MasterSlave(I2C_HandleTypeDef *hi2c)
{
    uint32_t tempreg = 0;
    int tx_limit = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
    uint8_t need_restart = 0;
    if((hi2c->gstatus | STATUS_WRITE_IN_PROGRESS) != STATUS_WRITE_IN_PROGRESS)
    {
        if(hi2c->Init.ReStartEn)
        {
            need_restart = 1;
        }
    }
#endif
    tx_limit = hi2c->Init.tx_fifo_depth - hi2c->Instance->TXFLR;
    while(tx_limit > 0 && hi2c->XferCount > 0U)
    {
        /* Write data to DATA_CMD */
        tempreg = 0;
        if(hi2c->Mode != HAL_I2C_MODE_SLAVE)
        {
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
            tempreg |= hi2c->XferCount == 1 ? I2C_DATA_CMD_STOP : 0;
            tempreg |= need_restart == 1 ? I2C_DATA_CMD_RESTART : 0;
            need_restart = 0;
#endif
            tempreg |= I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos;
        }
        tempreg |= (uint32_t)(*hi2c->pBuffPtr);
        hi2c->Instance->DATA_CMD = tempreg;
        tx_limit--;
        /* Increment Buffer pointer */
        hi2c->pBuffPtr++;
        /* Update counter */
        hi2c->XferCount--;
    }
    if(hi2c->XferCount > 0U)
    {
        /* more bytes to be written */
        hi2c->gstatus |= STATUS_WRITE_IN_PROGRESS;
    }
    else
    {
        hi2c->gstatus &= ~STATUS_WRITE_IN_PROGRESS;
    }
}
static void I2C_TXHandle_MasterRX(I2C_HandleTypeDef *hi2c)
{
    uint32_t tempreg = 0;
    int tx_limit = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
    uint8_t need_restart = 0;
    if((hi2c->gstatus | STATUS_WRITE_IN_PROGRESS) != STATUS_WRITE_IN_PROGRESS)
    {
        if(hi2c->Init.ReStartEn)
        {
            need_restart = 1;
        }
    }
#endif
    tx_limit = hi2c->Init.tx_fifo_depth - hi2c->Instance->TXFLR;
    while(tx_limit > 0 && hi2c->cmd_cnt < hi2c->cmd_max)
    {
        /* Write CMD to DATA_CMD */
        tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
        if(hi2c->cmd_cnt == hi2c->cmd_max - 1)
        {
            tempreg |= I2C_DATA_CMD_STOP;
        }
        tempreg |= need_restart == 1 ? I2C_DATA_CMD_RESTART : 0;
        need_restart = 0;
#endif
        tempreg |= I2C_CMD_Master_Read << I2C_DATA_CMD_CMD_Pos;
        hi2c->Instance->DATA_CMD = tempreg;
        tx_limit--;
        hi2c->cmd_cnt++;
    }
    if(hi2c->XferCount > 0U)
    {
        /* more bytes to be written */
        hi2c->gstatus |= STATUS_WRITE_IN_PROGRESS;
    }
    else
    {
        hi2c->gstatus &= ~STATUS_WRITE_IN_PROGRESS;
    }
}
static void I2C_TXHandle(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->State == HAL_I2C_STATE_BUSY_TX && hi2c->cmd_max == 0)
    {
        //Master/Slave TX process
        if(hi2c->Mode == HAL_I2C_MODE_MASTER || hi2c->Mode == HAL_I2C_MODE_SLAVE)
        {
            I2C_TXHandle_MasterSlave(hi2c);
        }
        //Mem Write process
        else if(hi2c->Mode == HAL_I2C_MODE_MEM)
        {
            I2C_TXHandle_MemWrite(hi2c);
        }
    }
    //Master RX process, need Write to IC_DATA_CMD to push Read command Tx FIFO
    else if(hi2c->State == HAL_I2C_STATE_BUSY_RX && hi2c->cmd_max != 0)
    {
        if(hi2c->Mode == HAL_I2C_MODE_MASTER)
        {
            I2C_TXHandle_MasterRX(hi2c);
        }
        //Mem Read process
        else if(hi2c->Mode == HAL_I2C_MODE_MEM)
        {
            I2C_TXHandle_MemRead(hi2c);
        }
    }
}
static void I2C_Ready(I2C_HandleTypeDef *hi2c)
{
    hi2c->State = HAL_I2C_STATE_READY;
    /* Generate NACK for a General Call */
    CLEAR_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
    hi2c->State = HAL_I2C_STATE_READY;
    hi2c->MemStage = I2C_MEM_STAGE_START;
    __HAL_I2C_DISABLE_IT_ALL(hi2c);
}
static void I2C_StopHandler(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->XferCount == 0)
    {
        if(hi2c->Mode == HAL_I2C_MODE_MASTER)
        {
            if(hi2c->State == HAL_I2C_STATE_BUSY_TX)
            {
                I2C_Ready(hi2c);
#if (defined USE_HAL_I2C_REGISTER_CALLBACKS && USE_HAL_I2C_REGISTER_CALLBACKS == 1)
                hi2c->MasterTxCpltCallback(hi2c);
#else
                HAL_I2C_MasterTxCpltCallback(hi2c);
#endif
            }
            else if (hi2c->State == HAL_I2C_STATE_BUSY_RX)
            {
                I2C_Ready(hi2c);
#if (defined USE_HAL_I2C_REGISTER_CALLBACKS && USE_HAL_I2C_REGISTER_CALLBACKS == 1)
                hi2c->MasterRxCpltCallback(hi2c);
#else
                HAL_I2C_MasterRxCpltCallback(hi2c);
#endif
            }
        }
        else if (hi2c->Mode == HAL_I2C_MODE_SLAVE)
        {
            if(hi2c->State == HAL_I2C_STATE_BUSY_TX)
            {
                I2C_Ready(hi2c);
#if (defined USE_HAL_I2C_REGISTER_CALLBACKS && USE_HAL_I2C_REGISTER_CALLBACKS == 1)
                hi2c->SlaveTxCpltCallback(hi2c);
#else
                HAL_I2C_SlaveTxCpltCallback(hi2c);
#endif
            }
            else if (hi2c->State == HAL_I2C_STATE_BUSY_RX)
            {
                I2C_Ready(hi2c);
#if (defined USE_HAL_I2C_REGISTER_CALLBACKS && USE_HAL_I2C_REGISTER_CALLBACKS == 1)
                hi2c->SlaveRxCpltCallback(hi2c);
#else
                HAL_I2C_SlaveRxCpltCallback(hi2c);
#endif
            }
        }
        else if (hi2c->Mode == HAL_I2C_MODE_MEM)
        {
            if(hi2c->State == HAL_I2C_STATE_BUSY_TX)
            {
                I2C_Ready(hi2c);
#if (defined USE_HAL_I2C_REGISTER_CALLBACKS && USE_HAL_I2C_REGISTER_CALLBACKS == 1)
                hi2c->MemTxCpltCallback(hi2c);
#else
                HAL_I2C_MemTxCpltCallback(hi2c);
#endif
            }
            else if (hi2c->State == HAL_I2C_STATE_BUSY_RX)
            {
                I2C_Ready(hi2c);
#if (defined USE_HAL_I2C_REGISTER_CALLBACKS && USE_HAL_I2C_REGISTER_CALLBACKS == 1)
                hi2c->MemRxCpltCallback(hi2c);
#else
                HAL_I2C_MemRxCpltCallback(hi2c);
#endif
            }
        }
    }
}
static void I2C_AbortHandler(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->tx_abort_source & I2C_TX_ABRT_SOURCE_USER_ABRT)
    {
#if (defined USE_HAL_I2C_REGISTER_CALLBACKS && USE_HAL_I2C_REGISTER_CALLBACKS == 1)
        hi2c->AbortCpltCallback(hi2c);
#else
        HAL_I2C_AbortCpltCallback(hi2c);
#endif
    }
    else
    {
        CLEAR_BIT(hi2c->Instance->CR, I2C_CR_RESTART_EN);
        CLEAR_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
        __HAL_I2C_DISABLE_IT(hi2c, (I2C_IT_MASTER_MASK | I2C_IT_SLAVE_MASK));
        hi2c->State = HAL_I2C_STATE_READY;
        hi2c->ErrorCode = HAL_I2C_ERROR_TX_ABRT;
#if (defined USE_HAL_I2C_REGISTER_CALLBACKS && USE_HAL_I2C_REGISTER_CALLBACKS == 1)
        hi2c->ErrorCallback(hi2c);
#else
        HAL_I2C_ErrorCallback(hi2c);
#endif
    }
}
/**
  * @brief  This function handles I2C event interrupt request.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_IRQHandler(I2C_HandleTypeDef *hi2c)
{
    uint32_t status = I2C_Read_Clear_ISR(hi2c);
    HAL_I2C_ModeTypeDef CurrentMode = hi2c->Mode;

    /* Master or Memory mode selected */
    if (CurrentMode == HAL_I2C_MODE_MASTER || CurrentMode == HAL_I2C_MODE_MEM)
    {
        /* I2C in mode Receiver -----------------------------------------------*/
        if((status & (I2C_FLAG_RX_FULL | I2C_FLAG_RX_OVER | I2C_FLAG_RX_DONE)) && (hi2c->DMAState == HAL_I2C_STATE_RESET))
        {
            I2C_RXHandle(hi2c);
        }
        /* I2C in mode Transmitter --------------------------------------------------*/
        if(status & I2C_FLAG_TX_EMPTY)
        {
            if((hi2c->DMAState == HAL_I2C_STATE_DMA_END) &&
               (hi2c->State == HAL_I2C_STATE_BUSY_TX) &&
               (hi2c->cmd_cnt > 0 && hi2c->cmd_cnt == hi2c->cmd_max))
            {
                CLEAR_BIT(hi2c->Instance->CR, I2C_CR_RESTART_EN);
                CLEAR_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
                __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_TX_EMPTY);
                hi2c->State = HAL_I2C_STATE_READY;
                hi2c->DMAState = HAL_I2C_STATE_RESET;
                if(CurrentMode == HAL_I2C_MODE_MASTER)
                {
#if (defined USE_HAL_I2C_REGISTER_CALLBACKS && USE_HAL_I2C_REGISTER_CALLBACKS == 1)
                    hi2c->MasterTxCpltCallback(hi2c);
#else
                    HAL_I2C_MasterTxCpltCallback(hi2c);
#endif
                } else if (CurrentMode == HAL_I2C_MODE_MEM) {
#if (defined USE_HAL_I2C_REGISTER_CALLBACKS && USE_HAL_I2C_REGISTER_CALLBACKS == 1)
                    hi2c->MemTxCpltCallback(hi2c);
#else
                    HAL_I2C_MemTxCpltCallback(hi2c);
#endif
                }
            } else {
                I2C_TXHandle(hi2c);
                if(hi2c->cmd_cnt > 0 && hi2c->cmd_cnt == hi2c->cmd_max)
                {
                    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_TX_EMPTY);
                }
            }
        }
    }
    /* Slave mode selected */
    else if (CurrentMode == HAL_I2C_MODE_SLAVE) 
    {
        /* I2C in mode Transmitter --------------------------------------------------*/
        if(status & (I2C_FLAG_TX_EMPTY | I2C_FLAG_RD_REQ))
        {
            if((hi2c->DMAState == HAL_I2C_STATE_DMA_LISTEN))
            {
                if((status & I2C_FLAG_TX_EMPTY) && ((hi2c->State & HAL_I2C_STATE_BUSY_TX) == HAL_I2C_STATE_BUSY_TX))
                {
                    __HAL_I2C_DMA_TX_ENABLE(hi2c);
                }
                else if((status & I2C_FLAG_RD_REQ) && ((hi2c->State & HAL_I2C_STATE_BUSY_RX) == HAL_I2C_STATE_BUSY_RX))
                {
                    __HAL_I2C_DMA_RX_ENABLE(hi2c);
                }
            }
            else if((hi2c->DMAState == HAL_I2C_STATE_DMA_END))
            {
                if((status & I2C_FLAG_TX_EMPTY))
                {
                    if((hi2c->State & HAL_I2C_STATE_LISTEN) == HAL_I2C_STATE_LISTEN)
                    {
                        hi2c->State = HAL_I2C_STATE_LISTEN;
                    } else {
                        CLEAR_BIT(hi2c->Instance->CR, I2C_CR_RESTART_EN);
                        CLEAR_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
                        hi2c->State = HAL_I2C_STATE_READY;
                    }
                    __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_TX_EMPTY);
                    hi2c->DMAState = HAL_I2C_STATE_RESET;
#if (defined USE_HAL_I2C_REGISTER_CALLBACKS && USE_HAL_I2C_REGISTER_CALLBACKS == 1)
                    hi2c->SlaveTxCpltCallback(hi2c);
#else
                    HAL_I2C_SlaveTxCpltCallback(hi2c);
#endif
                }
            } else if(hi2c->State == HAL_I2C_STATE_LISTEN && (status & I2C_FLAG_RD_REQ)) {
#if (defined USE_HAL_I2C_REGISTER_CALLBACKS && USE_HAL_I2C_REGISTER_CALLBACKS == 1)
                hi2c->ListenCpltCallback(hi2c);
#else
                HAL_I2C_ListenCpltCallback(hi2c);
#endif
            } else {
                I2C_TXHandle(hi2c);
            }
        }
        /* I2C in mode Receiver -----------------------------------------------*/
        if(status & (I2C_FLAG_RX_FULL | I2C_FLAG_RX_OVER | I2C_FLAG_RX_DONE))
        {
            I2C_RXHandle(hi2c);
        }
    }
    if(status & I2C_FLAG_TX_ABRT)
    {
        I2C_AbortHandler(hi2c);
        /* Generate NACK for a General Call */
        CLEAR_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
        hi2c->State = HAL_I2C_STATE_READY;
        __HAL_I2C_DISABLE_IT_ALL(hi2c);
    }
    if(status & I2C_FLAG_STOP_DET)
    {
        I2C_StopHandler(hi2c);
        if(hi2c->MemStage == I2C_MEM_STAGE_ADDR_LSB)
        {
            hi2c->MemStage = I2C_MEM_STAGE_ADDR_READ;
            /* Mem: Read mode, Master request configuration */
            I2C_MasterRequestConfig(hi2c, hi2c->Devaddress, I2C_CMD_Master_Read);
            __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_MASTER_MASK);
        }
    }
}

/**
  * @brief  Master Xfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__WEAK void HAL_I2C_DMAXferCpltCallback(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MasterTxCpltCallback could be implemented in the user file
    */
}
/**
  * @brief  Master Xfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__WEAK void HAL_I2C_XferCpltCallback(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MasterTxCpltCallback could be implemented in the user file
    */
}
/**
  * @brief  Master Tx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__WEAK void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MasterTxCpltCallback could be implemented in the user file
    */
}

/**
  * @brief  Master Rx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__WEAK void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MasterRxCpltCallback could be implemented in the user file
    */
}

/** @brief  Slave Tx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__WEAK void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_SlaveTxCpltCallback could be implemented in the user file
    */
}

/**
  * @brief  Slave Rx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__WEAK void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_SlaveRxCpltCallback could be implemented in the user file
    */
}

/**
  * @brief  Slave Address Match callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  TransferDirection Master request Transfer Direction (Write/Read), value of @ref I2C_XferDirection_definition
  * @param  AddrMatchCode Address Match Code
  * @retval None
  */
__WEAK void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);
    UNUSED(TransferDirection);
    UNUSED(AddrMatchCode);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_AddrCallback() could be implemented in the user file
    */
}

/**
  * @brief  Listen Complete callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__WEAK void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_ListenCpltCallback() could be implemented in the user file
    */
}

/**
  * @brief  Memory Tx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__WEAK void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MemTxCpltCallback could be implemented in the user file
    */
}

/**
  * @brief  Memory Rx Transfer completed callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__WEAK void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_MemRxCpltCallback could be implemented in the user file
    */
}

/**
  * @brief  I2C error callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__WEAK void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_ErrorCallback could be implemented in the user file
    */
}

/**
  * @brief  I2C abort callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
__WEAK void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hi2c);

    /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_I2C_AbortCpltCallback could be implemented in the user file
    */
}

/**
  * @}
  */

/** @defgroup I2C_Exported_Functions_Group3 Peripheral State, Mode and Error functions
 *  @brief   Peripheral State, Mode and Error functions
  *
@verbatim
 ===============================================================================
            ##### Peripheral State, Mode and Error functions #####
 ===============================================================================
    [..]
    This subsection permit to get in run-time the status of the peripheral
    and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  Return the I2C handle state.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval HAL state
  */
HAL_I2C_StateTypeDef HAL_I2C_GetState(I2C_HandleTypeDef *hi2c)
{
    /* Return I2C handle state */
    return hi2c->State;
}

/**
  * @brief  Returns the I2C Master, Slave, Memory or no mode.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @retval HAL mode
  */
HAL_I2C_ModeTypeDef HAL_I2C_GetMode(I2C_HandleTypeDef *hi2c)
{
    return hi2c->Mode;
}

/**
  * @brief  Return the I2C error code.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *              the configuration information for the specified I2C.
  * @retval I2C Error Code
  */
uint32_t HAL_I2C_GetError(I2C_HandleTypeDef *hi2c)
{
    return hi2c->ErrorCode;
}

/** @addtogroup I2C_Private_Functions
  * @{
  */

/**
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  cmd I2C_CMD_Master_Read or I2C_CMD_Master_Write
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_MasterRequestConfig(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t cmd)
{
    uint32_t tempreg = hi2c->Instance->TAR;
    /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
    uint32_t CurrentXferOptions = hi2c->XferOptions;

    __HAL_I2C_DISABLE(hi2c);
    HAL_I2C_BusMode_Select(hi2c, I2C_Mode_Master);
    SET_BIT(hi2c->Instance->CR, I2C_CR_RESTART_EN);
    if(cmd == I2C_CMD_Master_Read)
    {
        SET_BIT(tempreg, I2C_TAR_DEVICE_ID);//I2C_TAR_SPECIAL
    }
    else
    {
        CLEAR_BIT(tempreg, I2C_TAR_SPECIAL | I2C_TAR_DEVICE_ID);
    }
    /* Generate Start condition if first transfer */
    if ((CurrentXferOptions == I2C_FIRST_AND_LAST_FRAME) ||
        (CurrentXferOptions == I2C_FIRST_FRAME) ||
        (CurrentXferOptions == I2C_NO_OPTION_FRAME))
    {
        /* Generate Start */
        SET_BIT(tempreg, I2C_TAR_GC_OR_START);
    }
    else if (hi2c->PreviousState == I2C_STATE_MASTER_BUSY_RX)
    {
        /* Generate ReStart */
        SET_BIT(tempreg, I2C_TAR_GC_OR_START);
    }
    else
    {
        /* Do nothing */
        CLEAR_BIT(tempreg, I2C_TAR_GC_OR_START);
    }
    MODIFY_REG(tempreg, I2C_TAR_10BITADDR_MASTER_Msk, hi2c->Init.AddressMode == I2C_AddrMode_10BIT ? I2C_TAR_10BITADDR_MASTER : 0);
    MODIFY_REG(tempreg, I2C_TAR_Msk, DevAddress & 0x3FFU);
    hi2c->Instance->TAR = tempreg;
    __HAL_I2C_DISABLE_IT_ALL(hi2c);
    /* Dummy read to avoid the register getting stuck on Bay Trail */
    tempreg = hi2c->Instance->ENSR;
    /* Clear and enable interrupts */
    __HAL_CLEAR_INTR(hi2c);
    __HAL_I2C_ENABLE(hi2c);
    return HAL_OK;
}
/**
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_MasterDMARequestConfig(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t cmd)
{
    uint32_t tempreg = 0;
    /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
    uint32_t CurrentXferOptions = hi2c->XferOptions;

    __HAL_I2C_DISABLE(hi2c);
    HAL_I2C_BusMode_Select(hi2c, I2C_Mode_Master);
    SET_BIT(hi2c->Instance->CR, I2C_CR_RESTART_EN);
    tempreg = hi2c->Instance->TAR;
    if(cmd == I2C_CMD_Master_Read)
    {
        SET_BIT(tempreg, I2C_TAR_DEVICE_ID);//I2C_TAR_SPECIAL
    }
    else
    {
        CLEAR_BIT(tempreg, I2C_TAR_SPECIAL | I2C_TAR_DEVICE_ID);
    }
    /* Generate Start condition if first transfer */
    if ((CurrentXferOptions == I2C_FIRST_AND_LAST_FRAME) ||
        (CurrentXferOptions == I2C_FIRST_FRAME) ||
        (CurrentXferOptions == I2C_NO_OPTION_FRAME))
    {
        /* Generate Start */
        SET_BIT(tempreg, I2C_TAR_GC_OR_START);
    }
    else if (hi2c->PreviousState == I2C_STATE_MASTER_BUSY_RX)
    {
        /* Generate ReStart */
        SET_BIT(tempreg, I2C_TAR_GC_OR_START);
    }
    else
    {
        /* Do nothing */
        CLEAR_BIT(tempreg, I2C_TAR_GC_OR_START);
    }
    MODIFY_REG(tempreg, I2C_TAR_10BITADDR_MASTER_Msk, hi2c->Init.AddressMode == I2C_AddrMode_10BIT ? I2C_TAR_10BITADDR_MASTER : 0);
    MODIFY_REG(tempreg, I2C_TAR_Msk, DevAddress & 0x3FFU);
    hi2c->Instance->TAR = tempreg;
    hi2c->Instance->DMA_TDLR = hi2c->Init.tx_fifo_threshold;
    hi2c->Instance->DMA_RDLR = hi2c->Init.rx_fifo_threshold;
    __HAL_I2C_DISABLE_IT_ALL(hi2c);
    /* Dummy read to avoid the register getting stuck on Bay Trail */
    tempreg = hi2c->Instance->ENSR;
    /* Clear and enable interrupts */
    __HAL_CLEAR_INTR(hi2c);
    __HAL_I2C_ENABLE(hi2c);
    return HAL_OK;
}
/**
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_SlaveRequestConfig(I2C_HandleTypeDef *hi2c)
{
    uint32_t tempreg = hi2c->Instance->TAR;
    /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
    uint32_t CurrentXferOptions = hi2c->XferOptions;

    __HAL_I2C_DISABLE(hi2c);
    HAL_I2C_BusMode_Select(hi2c, I2C_Mode_Slave);
    __HAL_I2C_DISABLE_IT_ALL(hi2c);
    __HAL_I2C_ENABLE(hi2c);
    /* Dummy read to avoid the register getting stuck on Bay Trail */
    tempreg = hi2c->Instance->ENSR;
    /* Clear and enable interrupts */
    __HAL_CLEAR_INTR(hi2c);
    return HAL_OK;
}
static HAL_StatusTypeDef I2C_SlaveDMARequestConfig(I2C_HandleTypeDef *hi2c)
{
    uint32_t tempreg = hi2c->Instance->TAR;
    /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
    uint32_t CurrentXferOptions = hi2c->XferOptions;

    __HAL_I2C_DISABLE(hi2c);
    HAL_I2C_BusMode_Select(hi2c, I2C_Mode_Slave);
    SET_BIT(hi2c->Instance->CR, I2C_CR_RESTART_EN);
    hi2c->Instance->DMA_TDLR = hi2c->Init.tx_fifo_threshold;
    hi2c->Instance->DMA_RDLR = hi2c->Init.rx_fifo_threshold;
    __HAL_I2C_DISABLE_IT_ALL(hi2c);
    __HAL_I2C_ENABLE(hi2c);
    /* Dummy read to avoid the register getting stuck on Bay Trail */
    tempreg = hi2c->Instance->ENSR;
    /* Clear and enable interrupts */
    __HAL_CLEAR_INTR(hi2c);
    return HAL_OK;
}

/**
  * @brief  Master sends target device address for read request.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_MasterRequestRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Timeout)
{
    return HAL_OK;
}

/**
  * @brief  Master sends target device address followed by internal memory address for write request.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_RequestMemoryWrite(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart)
{
    uint32_t tx_limit;
    uint32_t rx_valid;
    uint32_t tempreg;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
    uint8_t need_restart = 0;
    if(hi2c->Init.ReStartEn)
    {
        need_restart = 1;
    }
#endif
    uint8_t mem_addr_send_cnt = 0;
    /* Master request configuration */
    I2C_MasterRequestConfig(hi2c, DevAddress, I2C_CMD_Master_Write);
    while(1)
    {
        tx_limit = hi2c->Init.tx_fifo_depth - hi2c->Instance->TXFLR;
        if(tx_limit > 0)
        {
            if(mem_addr_send_cnt < 2)
            {
                /* If Memory address size is 8Bit */
                if(MemAddSize == I2C_MEMADD_SIZE_8BIT)
                {
                    /* Send Memory Address */
                    tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
                    tempreg |= (need_restart == 1 ? I2C_DATA_CMD_RESTART : 0);
                    need_restart = 0;
#endif
                    tempreg |= (I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos);
                    tempreg |= I2C_MEM_ADD_LSB(MemAddress);
                    hi2c->Instance->DATA_CMD = tempreg;
                    mem_addr_send_cnt = 2;
                }
                /* If Memory address size is 16Bit */
                else if(MemAddSize == I2C_MEMADD_SIZE_16BIT)
                {
                    if(mem_addr_send_cnt == 0)
                    {
                        /* Send MSB of Memory Address */
                        tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
                        tempreg |= (need_restart == 1 ? I2C_DATA_CMD_RESTART : 0);
                        need_restart = 0;
#endif
                        tempreg |= (I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos);
                        tempreg |= I2C_MEM_ADD_MSB(MemAddress);
                        hi2c->Instance->DATA_CMD = tempreg;
                        mem_addr_send_cnt ++;
                    }
                    else if(mem_addr_send_cnt == 1)
                    {
                        /* Send MSB of Memory Address */
                        tempreg = 0;
                        tempreg |= (I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos);
                        tempreg |= I2C_MEM_ADD_LSB(MemAddress);
                        hi2c->Instance->DATA_CMD = tempreg;
                        mem_addr_send_cnt ++;
                    }
                }
            }
            else
            {
                /* Wait until TXE flag is set */
                if(I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout) != HAL_OK)
                {
                    return HAL_ERROR;
                }
                break;
            }
        }
        if(Timeout != HAL_MAX_DELAY)
        {
            if((HAL_GetTick() - Tickstart) > Timeout)
            {
                hi2c->State = HAL_I2C_STATE_READY;
                hi2c->Mode = HAL_I2C_MODE_NONE;
                /* Process Unlocked */
                __HAL_UNLOCK(hi2c);
                return HAL_TIMEOUT;
            }
        }
    }
    return HAL_OK;
}

/**
  * @brief  Master sends target device address followed by internal memory address for read request.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  MemAddress Internal memory address
  * @param  MemAddSize Size of internal memory address
  * @param  Timeout Timeout duration
  * @param  Tickstart Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_RequestMemoryRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint32_t Timeout, uint32_t Tickstart)
{
    uint32_t tx_limit;
    uint32_t rx_valid;
    uint32_t tempreg;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
    uint8_t need_restart = 0;
    if(hi2c->Init.ReStartEn)
    {
        need_restart = 1;
    }
#endif
    uint8_t mem_addr_send_cnt = 0;
    /* Master request configuration */
    I2C_MasterRequestConfig(hi2c, DevAddress, I2C_CMD_Master_Write);
    while(1)
    {
        tx_limit = hi2c->Init.tx_fifo_depth - hi2c->Instance->TXFLR;
        if(tx_limit > 0)
        {
            if(mem_addr_send_cnt < 2)
            {
                /* If Memory address size is 8Bit */
                if(MemAddSize == I2C_MEMADD_SIZE_8BIT)
                {
                    /* Send Memory Address */
                    tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
                    tempreg |= (need_restart == 1 ? I2C_DATA_CMD_RESTART : 0);
                    need_restart = 0;
#endif
                    tempreg |= (I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos);
                    tempreg |= I2C_MEM_ADD_LSB(MemAddress);
                    hi2c->Instance->DATA_CMD = tempreg;
                    mem_addr_send_cnt = 2;
                }
                /* If Memory address size is 16Bit */
                else if(MemAddSize == I2C_MEMADD_SIZE_16BIT)
                {
                    if(mem_addr_send_cnt == 0)
                    {
                        /* Send MSB of Memory Address */
                        tempreg = 0;
#if(defined I2C_IC_EMPTYFIFO_HOLD_MASTER_EN && I2C_IC_EMPTYFIFO_HOLD_MASTER_EN == 1U)
                        tempreg |= (need_restart == 1 ? I2C_DATA_CMD_RESTART : 0);
                        need_restart = 0;
#endif
                        tempreg |= (I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos);
                        tempreg |= I2C_MEM_ADD_MSB(MemAddress);
                        hi2c->Instance->DATA_CMD = tempreg;
                        mem_addr_send_cnt ++;
                    }
                    else if(mem_addr_send_cnt == 1)
                    {
                        /* Send MSB of Memory Address */
                        tempreg = 0;
                        tempreg |= (I2C_CMD_Master_Write << I2C_DATA_CMD_CMD_Pos);
                        tempreg |= I2C_MEM_ADD_LSB(MemAddress);
                        hi2c->Instance->DATA_CMD = tempreg;
                        mem_addr_send_cnt ++;
                    }
                }
            }
            else
            {
                /* Wait until TXE flag is set */
                if (I2C_WaitOnTXEFlagUntilTimeout(hi2c, Timeout) != HAL_OK)
                {
                    return HAL_ERROR;
                }
                break;
            }
        }
        if(Timeout != HAL_MAX_DELAY)
        {
            if((HAL_GetTick() - Tickstart) > Timeout)
            {
                hi2c->State = HAL_I2C_STATE_READY;
                hi2c->Mode = HAL_I2C_MODE_NONE;
                /* Process Unlocked */
                __HAL_UNLOCK(hi2c);
                return HAL_TIMEOUT;
            }
        }
    }
    return HAL_OK;
}

/**
  * @brief  DMA I2C process complete callback.
  * @param  hdma DMA handle
  * @retval None
  */
static void I2C_DMAXferCplt(DMA_HandleTypeDef *hdma)
{
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */

    /* Declaration of temporary variable to prevent undefined behavior of volatile usage */
    HAL_I2C_StateTypeDef CurrentState = hi2c->State;
    HAL_I2C_ModeTypeDef CurrentMode   = hi2c->Mode;
    uint32_t CurrentXferOptions       = hi2c->XferOptions;

    /* Clear Complete callback */
    hi2c->hdmatx->XferCpltCallback = NULL;
    hi2c->hdmarx->XferCpltCallback = NULL;
    /* Disable DMA Request */
    CLEAR_BIT(hi2c->Instance->DMA_CR, I2C_DMA_CR_TDMAE | I2C_DMA_CR_RDMAE);
    hi2c->XferCount = 0U;

    if((CurrentState & HAL_I2C_STATE_BUSY_TX) == HAL_I2C_STATE_BUSY_TX)
    {
        hi2c->DMAState = HAL_I2C_STATE_DMA_END;
        __HAL_I2C_ENABLE_IT(hi2c, I2C_IT_TX_EMPTY);
    }
    else if ((CurrentState & HAL_I2C_STATE_BUSY_RX) == HAL_I2C_STATE_BUSY_RX)
    {
        if (CurrentMode == HAL_I2C_MODE_SLAVE)
        {
            if((CurrentState & HAL_I2C_STATE_LISTEN) == HAL_I2C_STATE_LISTEN)
            {
                hi2c->State = HAL_I2C_STATE_LISTEN;
            } else {
                CLEAR_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
                __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_SLAVE_MASK);
                hi2c->State = HAL_I2C_STATE_READY;
            }
            hi2c->DMAState = HAL_I2C_STATE_RESET;
            hi2c->PreviousState = I2C_STATE_NONE;
            /* Call the corresponding callback to inform upper layer of End of Transfer */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
            hi2c->SlaveRxCpltCallback(hi2c);
#else
            HAL_I2C_SlaveTxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
        }
        else if (CurrentMode != HAL_I2C_MODE_NONE)
        {
            __HAL_I2C_DISABLE_IT(hi2c, I2C_IT_MASTER_MASK);
            hi2c->Mode = HAL_I2C_MODE_NONE;
            hi2c->State = HAL_I2C_STATE_READY;
            hi2c->DMAState = HAL_I2C_STATE_RESET;
            hi2c->PreviousState = I2C_STATE_NONE;
            if (CurrentMode == HAL_I2C_MODE_MEM)
            {
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
                hi2c->MemRxCpltCallback(hi2c);
#else
                HAL_I2C_MemRxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
            } else {
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
                hi2c->MasterRxCpltCallback(hi2c);
#else
                HAL_I2C_MasterRxCpltCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
            }
        }
    }
}

/**
  * @brief  DMA I2C communication error callback.
  * @param  hdma DMA handle
  * @retval None
  */
static void I2C_DMAError(DMA_HandleTypeDef *hdma)
{
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent; /* Derogation MISRAC2012-Rule-11.5 */

    /* Clear Complete callback */
    hi2c->hdmatx->XferCpltCallback = NULL;
    hi2c->hdmarx->XferCpltCallback = NULL;

    /* Ignore DMA FIFO error */
    if (HAL_DMA_GetError(hdma) != HAL_DMA_ERROR_FE)
    {
        CLEAR_BIT(hi2c->Instance->ACK_GENERAL_CALL, I2C_ACK_GENERAL_CALL);
        /* Disable DMA Request */
        CLEAR_BIT(hi2c->Instance->DMA_CR, I2C_DMA_CR_TDMAE | I2C_DMA_CR_RDMAE);

        hi2c->XferCount = 0U;
        hi2c->State = HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;
        hi2c->ErrorCode |= HAL_I2C_ERROR_DMA;

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
        hi2c->ErrorCallback(hi2c);
#else
        HAL_I2C_ErrorCallback(hi2c);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
    }
}

/**
  * @brief DMA I2C communication abort callback
  *        (To be called at end of DMA Abort procedure).
  * @param hdma DMA handle.
  * @retval None
  */
static void I2C_DMAAbort(DMA_HandleTypeDef *hdma)
{
    UNUSED(hdma);
}

/**
  * @brief  This function handles I2C Communication Timeout.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  Flag specifies the I2C flag to check.
  * @param  Status The new Flag status (SET or RESET).
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout)
{
    uint32_t tickstart = HAL_GetTick();
    /* Wait until flag is set */
    while (__HAL_I2C_GET_STATUS(hi2c, Flag) == Status)
    {
        /* Check for the Timeout */
        if (Timeout != HAL_MAX_DELAY)
        {
            if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))
            {
                hi2c->PreviousState     = I2C_STATE_NONE;
                hi2c->State             = HAL_I2C_STATE_READY;
                hi2c->Mode              = HAL_I2C_MODE_NONE;
                hi2c->ErrorCode         |= HAL_I2C_ERROR_TIMEOUT;
                /* Process Unlocked */
                __HAL_UNLOCK(hi2c);
                return HAL_ERROR;
            }
        }
    }
    return HAL_OK;
}
/**
  * @brief  This function handles I2C Communication Timeout for Master addressing phase.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for I2C module
  * @param  Flag specifies the I2C flag to check.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnMasterAddressFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, uint32_t Timeout)
{
    (void)hi2c;
    (void)Flag;
    (void)Timeout;
    return HAL_OK;
}

/**
  * @brief  This function handles I2C Communication Timeout for specific usage of TXE flag.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnTXEFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout)
{
    uint32_t Tickstart = HAL_GetTick();
    while (__HAL_I2C_GET_STATUS(hi2c, I2C_STATUS_TFE) == RESET)
    {
        /* Check for the Timeout */
        if (Timeout != HAL_MAX_DELAY)
        {
            if (((HAL_GetTick() - Tickstart) > Timeout) || (Timeout == 0U))
            {
                hi2c->PreviousState       = I2C_STATE_NONE;
                hi2c->State               = HAL_I2C_STATE_READY;
                hi2c->Mode                = HAL_I2C_MODE_NONE;
                hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;
                /* Process Unlocked */
                __HAL_UNLOCK(hi2c);
                return HAL_ERROR;
            }
        }
    }
    return HAL_OK;
}

/**
  * @brief  This function handles I2C Communication Timeout for specific usage of STOP flag.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnSTOPFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout)
{
    uint32_t tickstart = HAL_GetTick();
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOP_DET) == RESET)
    {
        /* Check for the Timeout */
        if (Timeout != HAL_MAX_DELAY)
        {
            if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))
            {
                hi2c->PreviousState       = I2C_STATE_NONE;
                hi2c->State               = HAL_I2C_STATE_READY;
                hi2c->Mode                = HAL_I2C_MODE_NONE;
                hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;
                /* Process Unlocked */
                __HAL_UNLOCK(hi2c);
                return HAL_ERROR;
            }
        }
    }
    /* Check if a NACK is detected */
    return I2C_IsAcknowledgeFailed(hi2c);
}

/**
  * @brief  This function handles I2C Communication Timeout for specific usage of RXNE flag.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnRXNEFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout)
{
    uint32_t tickstart = HAL_GetTick();
    while (__HAL_I2C_GET_STATUS(hi2c, I2C_STATUS_RFNE) == RESET)
    {
        /* Check for the Timeout */
        if (Timeout != HAL_MAX_DELAY)
        {
            if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))
            {
                hi2c->PreviousState       = I2C_STATE_NONE;
                hi2c->State               = HAL_I2C_STATE_READY;
                hi2c->Mode                = HAL_I2C_MODE_NONE;
                hi2c->ErrorCode           |= HAL_I2C_ERROR_TIMEOUT;
                /* Process Unlocked */
                __HAL_UNLOCK(hi2c);
                return HAL_ERROR;
            }
        }
    }
    return HAL_OK;
}

/**
  * @brief  This function handles Acknowledge failed detection during an I2C Communication.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_IsAcknowledgeFailed(I2C_HandleTypeDef *hi2c)
{
    uint32_t status = I2C_Read_Clear_ISR(hi2c);
    if (status & I2C_FLAG_TX_ABRT)
    {
        if(hi2c->tx_abort_source & I2C_TX_ABRT_NOACK)
        {
            hi2c->PreviousState       = I2C_STATE_NONE;
            hi2c->State               = HAL_I2C_STATE_READY;
            hi2c->Mode                = HAL_I2C_MODE_NONE;
            hi2c->ErrorCode           |= (HAL_I2C_ERROR_TX_ABRT | HAL_I2C_ERROR_AF);
            /* Process Unlocked */
            __HAL_UNLOCK(hi2c);
            return HAL_ERROR;
        }
    }
    return HAL_OK;
}

/**
  * @brief  Convert I2Cx OTHER_xxx XferOptions to functionnal XferOptions.
  * @param  hi2c I2C handle.
  * @retval None
  */
static void I2C_ConvertOtherXferOptions(I2C_HandleTypeDef *hi2c)
{
    /* if user set XferOptions to I2C_OTHER_FRAME            */
    /* it request implicitly to generate a restart condition */
    /* set XferOptions to I2C_FIRST_FRAME                    */
    if (hi2c->XferOptions == I2C_OTHER_FRAME)
    {
        hi2c->XferOptions = I2C_FIRST_FRAME;
    }
    /* else if user set XferOptions to I2C_OTHER_AND_LAST_FRAME */
    /* it request implicitly to generate a restart condition    */
    /* then generate a stop condition at the end of transfer    */
    /* set XferOptions to I2C_FIRST_AND_LAST_FRAME              */
    else if (hi2c->XferOptions == I2C_OTHER_AND_LAST_FRAME)
    {
        hi2c->XferOptions = I2C_FIRST_AND_LAST_FRAME;
    }
    else
    {
        /* Nothing to do */
    }
}

#endif /* HAL_I2C_MODULE_ENABLED */

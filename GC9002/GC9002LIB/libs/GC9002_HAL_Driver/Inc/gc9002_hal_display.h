/**
  ******************************************************************************
  * @file    gc9002_hal_display.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2023-1-6
  * @brief   This file contains all the functions prototypes for the DISPLAY
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC9002_HAL_DISPLAY_H__
#define __GC9002_HAL_DISPLAY_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal_def.h"

#ifdef HAL_DISPLAY_MODULE_ENABLED

/* display regs
 **/
typedef struct
{
    volatile uint32_t enable            : 1;
    volatile uint32_t invert_oclk       : 1;
    volatile uint32_t invert_hs         : 1;
    volatile uint32_t invert_vs         : 1;
    volatile uint32_t update_ram        : 1;
    volatile uint32_t fg_enable         : 1;
    volatile uint32_t dithering         : 1;
    volatile uint32_t input_color_mode  : 3;
    volatile uint32_t output_color_mode : 3;
    volatile uint32_t interface_type    : 3;
    volatile uint32_t running_type      : 2;
    volatile uint32_t tearing_en        : 1;
    volatile uint32_t update_en         : 1;
    volatile uint32_t clk_spi_div       : 4;
    volatile uint32_t icg_en            : 1;
    volatile uint32_t lcd_mv            : 1;
    uint32_t                            : 6;
}display_cntl_struct;
union _disp_cntl
{
	volatile uint32_t reg_u32val;            /*!< Offset: 0x00  (R/W) */
	display_cntl_struct bit;
};
union _disp_cmd_fifo_input
{
	volatile uint32_t reg_u32val;            /*!< Offset: 0x44  (R/W) */
	struct
    {
        volatile uint32_t cmd_data      : 8;
        volatile uint32_t cmd_dcx       : 1;
        volatile uint32_t cmd_last      : 1;
        volatile uint32_t cmd_rd        : 1;
        volatile uint32_t cmd_fifo_clr  : 1;
        volatile uint32_t cmd_rd_size   : 4;
        volatile uint32_t data_fifo_clr : 1;
        uint32_t                        : 15;
    }bit;
};
union _disp_fifo_status
{
	volatile uint32_t reg_u32val;            /*!< Offset: 0x4c  (R/W) */
	struct
    {
        volatile uint32_t cmd_fifo_full     : 1;
        volatile uint32_t cmd_fifo_empty    : 1;
        volatile uint32_t cmd_fifo_fw       : 1;
        volatile uint32_t data_fifo_full    : 1;
        volatile uint32_t data_fifo_empty   : 1;
        volatile uint32_t data_fifo_fw      : 1;
        volatile uint32_t data_fifo_er      : 1;
        uint32_t                            : 25;
    }bit;
};
union _disp_set_rc_inst
{
	volatile uint32_t reg_u32val;            /*!< Offset: 0x58  (R/W) */
	struct
    {
        volatile uint32_t set_row_inst  : 8;
        volatile uint32_t set_col_inst  : 8;
        volatile uint32_t wr_ram_inst   : 8;
        volatile uint32_t inst_type_sel : 8;
    }bit;
};
union _disp_set_wqspi_inst
{
	volatile uint32_t reg_u32val;            /*!< Offset: 0x5c  (R/W) */
	struct
    {
        volatile uint32_t wr_cmd_inst    : 8;
        volatile uint32_t wr_color_inst  : 8;
        volatile uint32_t wr_4color_inst : 8;
        volatile uint32_t wr_4p_inst     : 8;
    }bit;
};
union _disp_set_Streaming_inst
{
	volatile uint32_t reg_u32val;            /*!< Offset: 0x64  (R/W) */
	struct
    {
        volatile uint32_t hs_inst           : 8;
        volatile uint32_t vs_inst           : 8;
        volatile uint32_t qspi_cmd_type     : 2;
        uint32_t                            : 2;
        volatile uint32_t qspi_color_type   : 2;
        uint32_t                            : 2;
        volatile uint32_t qspi_rd_type      : 2;
        uint32_t                            : 6;
    }bit;
};
union _disp_ccm_matrix
{
	volatile uint32_t reg_u32val;            /*!< Offset: 0x1c  (R/W) */
	struct
    {
        volatile uint32_t M0 : 8;
        volatile uint32_t M1 : 8;
        volatile uint32_t M2 : 8;
        volatile uint32_t M3 : 8;
    }bit;
};
union _disp_yct_matrix
{
	volatile uint32_t reg_u32val;            /*!< Offset: 0x1c  (R/W) */
	struct
    {
        volatile uint32_t Mcg1 : 8;
        volatile uint32_t Mcg0 : 8;
        volatile uint32_t Mco1 : 8;
        volatile uint32_t Mco0 : 8;
    }bit;
};
typedef struct _disp_y_table
{
	volatile uint32_t reg_u32val;            /*!< Offset: 0x1c  (R/W) */
	struct
    {
        volatile uint32_t Y0 : 8;
        volatile uint32_t Y1 : 8;
        volatile uint32_t Y2 : 8;
        volatile uint32_t Y3 : 8;
    }bit;
}DISPLAY_Y_Table;
typedef struct
{
    union _disp_cntl display_cntl;          //reg0  0x0
    volatile uint32_t interrupt_mask;       //reg1  0x4
    volatile uint32_t interrupt;            //reg2  0x8
    volatile uint32_t timing_param0;        //reg3  0xc
    volatile uint32_t timing_param1;        //reg4  0x10
    volatile uint32_t timing_param2;        //reg5  0x14
    volatile uint32_t timing_param3;        //reg6  0x18
    union _disp_ccm_matrix ccm_matrix_r;    //reg7  0x1c
    union _disp_ccm_matrix ccm_matrix_g;    //reg8  0x20
    union _disp_ccm_matrix ccm_matrix_b;    //reg9  0x24
    union
    {
        union _disp_yct_matrix yct_matrix;
        volatile uint32_t adjust_ram_param; //reg10 0x28
    };    
    volatile uint32_t base_addr;                //reg11 0x2c
    volatile uint32_t total_byte_cnt;           //reg12 0x30
    volatile uint32_t row_update_area;          //reg13 0x34
    volatile uint32_t col_update_area;          //reg14 0x38
    volatile uint32_t reserved_r15;             //reg15 0x3c  
    volatile uint32_t burst_length;             //reg16 0x40
    union _disp_cmd_fifo_input cmd_fifo_input;  //reg17 0x44
    volatile uint32_t data_fifo_output;         //reg18 0x48
    union _disp_fifo_status fifo_status;        //reg19 0x4c
    volatile uint32_t fifo_cntl;                //reg20 0x50
    volatile uint32_t tearing_int_interval;     //reg21 0x54
    union _disp_set_rc_inst set_rc_inst;        //reg22 0x58
    union _disp_set_wqspi_inst set_wqspi_inst;  //reg23 0x5c
    volatile uint32_t lcd_rst_cntl;             //reg24 0x60
    union _disp_set_Streaming_inst set_Streaming_inst; //reg25 0x64
    volatile uint32_t set_rqspi_inst;           //reg26 0x68
    volatile uint32_t rd_capture_dly;           //reg27 0x6c
    volatile uint32_t tearing_threshold;        //reg28 0x70
    uint32_t reserved_r74_fc[35];               //reg28 0x74~0xfc
    DISPLAY_Y_Table Y_Table[64];                //Y Table 0x100~0x1FC
}DISPLAY_TypeDef;

/* DISPLAY definitions */
#define DISPLAY_BASE        (0x40103000U)
#define DISPLAY             ((DISPLAY_TypeDef *)DISPLAY_BASE)



typedef struct
{
    uint16_t hpixels;
    uint16_t hsyncpw;
    uint16_t hfnprch;
    uint16_t hbkprch;
    uint16_t vlines;
    uint16_t vsyncpw;
    uint16_t vfnprch;
    uint16_t vbkprch;
}DISPLAY_TimingParamType;
typedef struct
{
    uint16_t start_row;
    uint16_t end_row;
    uint16_t start_col;
    uint16_t end_col;
}DISPLAY_UpdateAreaType;
typedef struct
{
    uint32_t base_addr;
    uint32_t total_byte_cnt;
    uint32_t burst_length;
}DISPLAY_MasterParamType;
typedef struct
{
    uint32_t wr_cmd_inst    : 8;
    uint32_t wr_color_inst  : 8;
    uint32_t wr_4color_inst : 8;
    uint32_t wr_4p_inst     : 8;
}DISPLAY_WqspiInstType;
/* qspi_cmd_type/qspi_color_type/qspi_rd_type：
 * 0-1 wire cmd 1 wire address 1 wire data
 * 1-1 wire cmd 1 wire address 4 wire data
 * 2-1 wire cmd 4 wire address 4 wire data
 * 3-4 wire cmd 4 wire address 4 wire data
 **/
typedef enum
{
    DISPLAY_QSPI_1_1_1 = 0U,
    DISPLAY_QSPI_1_1_4 = 1U,
    DISPLAY_QSPI_1_4_4 = 2U,
    DISPLAY_QSPI_4_4_4 = 3U,
}DISPLAY_QspiBusType;
typedef struct
{
    uint32_t hs_inst          : 8;//发送hs指令字
    uint32_t vs_inst          : 8;//发送vs指令字
    uint32_t qspi_cmd_type    : 2;/*!< qspi接口输出命令格式. @ref DISPLAY_QspiBusType */
    uint32_t                  : 2;
    uint32_t qspi_color_type  : 2;/*!< qspi接口输出设置颜色格式. @ref DISPLAY_QspiBusType */
    uint32_t                  : 2;
    uint32_t qspi_rd_type     : 2;/*!< qspi接口输入设置格式. @ref DISPLAY_QspiBusType */
    uint32_t                  : 6;
}DISPLAY_StreamingInstType;
typedef struct
{
    uint32_t set_row_inst  : 8;/*!< 设置起始、结束行的命令的命令字 */
    uint32_t set_col_inst  : 8;/*!< 设置起始、结束列的命令的命令字 */
    uint32_t wr_ram_inst   : 8;/*!< 开始写缓存的命令字 */
    uint32_t inst_type_sel : 8;/*!< 发送命令的格式选择，目前有3种
                                    0代表地址格式是00xx00，写缓存的数据紧跟写缓存命令.
                                    1代表地址格式是00xxyy，其中yy是帧数，写缓存命令后还需要发送0xa5，帧数固定为0. 
                                    2代表地址格式是00xxyy，其中yy是帧数，用于单色屏的blinking. */
}DISPLAY_RcInstType;

typedef struct
{
    DISPLAY_TimingParamType timing_param;
    DISPLAY_UpdateAreaType update_area;
    DISPLAY_MasterParamType master_param;
    DISPLAY_RcInstType rc_inst;
    DISPLAY_WqspiInstType wqspi_inst;
    DISPLAY_StreamingInstType streaming_inst;
    uint32_t tearing_threshold;     /*!< Sets thresholds for row or column tearing.  */
    
    struct 
    {
        uint32_t rqspi_inst  : 8;      /*!< the read instruction word.      */
        uint32_t disp_duty_l : 9;      /*!< display duty low.               */
        uint32_t disp_duty_h : 9;      /*!< display duty high.              */
        uint32_t             : 6;
    };
    struct 
    {
        uint32_t disp_qspi_duty_l : 9;  /*!< display qspi duty low.         */
        uint32_t disp_qspi_duty_h : 9;  /*!< display qspi duty high.        */
        uint32_t                  : 14;
    };
    struct
    {
        uint32_t invert_oclk_en    : 1;/*!< The output clock is reversed.
                                            This parameter can be a value of @ref DISPLAY_EN_Enum */
        uint32_t invert_hs         : 1;/*!< hs output reverse phase.
                                            This parameter can be a value of @ref DISPLAY_EN_Enum */
        uint32_t invert_vs         : 1;/*!< vs output reverse phase.
                                            This parameter can be a value of @ref DISPLAY_EN_Enum */
        uint32_t update_ram        : 1;/*!< bypass ccm.
                                            This parameter can be a value of @ref DISPLAY_EN_Enum */
        uint32_t fg_enable         : 1;/*!< timing gen enable.
                                            This parameter can be a value of @ref DISPLAY_EN_Enum */
        uint32_t dithering         : 1;/*!< dithering enable.
                                            This parameter can be a value of @ref DISPLAY_EN_Enum */
        uint32_t input_color_mode  : 3;/*!< input color mode.
                                            This parameter can be a value of @ref DISPLAY_InputColorMode */
        uint32_t output_color_mode : 3;/*!< output color mode.
                                            This parameter can be a value of @ref DISPLAY_OutputColorMode */
        uint32_t interface_type    : 3;/*!< interface type.
                                            This parameter can be a value of @ref DISPLAY_InterfaceType */
        uint32_t running_type      : 2;/*!< running type.
                                            This parameter can be a value of @ref DISPLAY_Mode */
        uint32_t tearing_en        : 1;/*!< tearing enable.
                                            This parameter can be a value of @ref DISPLAY_EN_Enum */
        uint32_t update_en         : 1;/*!< Refresh the specified area of the screen enable, this parameter is valid only when running_type=1.
                                            This parameter can be a value of @ref DISPLAY_EN_Enum */
        uint32_t clk_spi_div       : 4;/*!< SPI and 8080 clock frequency division, as opposed to axi bus clock frequency division.
                                            This parameter can be a value of @ref DISPLAY_Div_Enum */
        uint32_t icg_en            : 1;/*!< Clock gating enable.
                                            This parameter can be a value of @ref DISPLAY_EN_Enum */
        uint32_t lcd_mv            : 1;/*!< Sets the default input point sequence for slaveRAM mode.
                                            This parameter can be a value of @ref DISPLAY_Sequence_Enum */
        uint32_t                   : 7;
    };
    uint32_t para_extra;            /*!< Keep extra para of Display  */
}DISPLAY_InitTypeDef;
typedef enum
{
    DISPLAY_DIV_2 = 0U,
    DISPLAY_DIV_4 = 1U,
    DISPLAY_DIV_6 = 2U,
    DISPLAY_DIV_8 = 3U,
    DISPLAY_DIV_10 = 4U,
    DISPLAY_DIV_12 = 5U,
    DISPLAY_DIV_14 = 6U,
    DISPLAY_DIV_16 = 7U,
    DISPLAY_DIV_18 = 8U,
    DISPLAY_DIV_20 = 9U,
    DISPLAY_DIV_22 = 10U,
    DISPLAY_DIV_24 = 11U,
    DISPLAY_DIV_26 = 12U,
    DISPLAY_DIV_28 = 13U,
    DISPLAY_DIV_30 = 14U,
    DISPLAY_DIV_32 = 15U,
}DISPLAY_Div_Enum;
typedef enum
{
    DISPLAY_Seq_ROW = 0U,
    DISPLAY_Seq_COLUMN = 1U,
}DISPLAY_Sequence_Enum;
typedef enum
{
    DISPLAY_DISABLE = 0U,
    DISPLAY_ENABLE = 1U,
}DISPLAY_EN_Enum;
/* Note: All Settings are converted RGB565, except 0,1, and 4.
 **/
typedef enum
{
    DISPLAY_InputColorMode_RGBA8888 = 0U,
    DISPLAY_InputColorMode_RGB565 = 1U,
    DISPLAY_InputColorMode_RGB888 = 2U,
    DISPLAY_InputColorMode_SFBC = 3U,
    DISPLAY_InputColorMode_Y8 = 4U,
}DISPLAY_InputColorMode;
/* Note: RGB666 applies only to the 8080-8bit output interface
 * For 8080-8bit output interface, RGB666 and RGB888 configuration display effect, because the lcd driver chip does not support RGB888
 **/
typedef enum
{
    DISPLAY_OutputColorMode_Y8 = 0U,
    DISPLAY_OutputColorMode_Y4 = 1U,
    DISPLAY_OutputColorMode_Y2 = 2U,
    DISPLAY_OutputColorMode_Y1 = 3U,
    DISPLAY_OutputColorMode_RGB565 = 4U,
    DISPLAY_OutputColorMode_RGB666 = 5U,
    DISPLAY_OutputColorMode_RGB888 = 6U,
}DISPLAY_OutputColorMode;
typedef enum
{
    DISPLAY_InterfaceType_SPI_LINE3 = 0U,
    DISPLAY_InterfaceType_SPI_LINE4 = 1U,
    DISPLAY_InterfaceType_QSPI = 3U,
    DISPLAY_InterfaceType_8BIT = 4U,
    DISPLAY_InterfaceType_24BIT = 5U,
}DISPLAY_InterfaceType;
typedef enum
{
    DISPLAY_Mode_MasterActive = 0U,
    DISPLAY_Mode_MasterPassive = 1U,
    DISPLAY_Mode_SlavePassive = 2U,
    DISPLAY_Mode_Configuration = 3U,
}DISPLAY_Mode;

typedef enum
{
    HAL_DISPLAY_STATE_RESET             = 0x00U,   /*!< Peripheral is not yet Initialized         */
    HAL_DISPLAY_STATE_READY             = 0x02U,   /*!< Peripheral Initialized and ready for use  */
    HAL_DISPLAY_STATE_BUSY              = 0x04U,   /*!< An internal process is ongoing            */
    HAL_DISPLAY_STATE_TIMEOUT           = 0x08U,   /*!< Timeout state                             */
    HAL_DISPLAY_STATE_ERROR             = 0xE0U    /*!< Error                                     */
} HAL_DISPLAY_StateTypeDef;
/**
  * @brief  DISPLAY handle Structure definition
  */
typedef struct __DISPLAY_HandleTypeDef
{
    DISPLAY_TypeDef                *Instance;      /*!< DISPLAY registers base address               */
    DISPLAY_InitTypeDef            Init;           /*!< DISPLAY communication parameters             */
    uint32_t                       status;         /*!< Interrupt status                             */
    HAL_LockTypeDef                Lock;           /*!< Locking object                               */
    __IO HAL_DISPLAY_StateTypeDef  State;          /*!< DISPLAY communication state                  */
    __IO uint32_t                  ErrorCode;      /*!< DISPLAY Error code                           */
#if(defined USE_HAL_DISPLAY_REGISTER_CALLBACKS && USE_HAL_DISPLAY_REGISTER_CALLBACKS == 1U)
    void (* MspInitCallback)(struct __DISPLAY_HandleTypeDef *hdisplay);                 /*!< DISPLAY Msp Init callback          */
    void (* MspDeInitCallback)(struct __DISPLAY_HandleTypeDef *hdisplay);               /*!< DISPLAY Msp DeInit callback        */
    void (* data_fifo_er_Callback)(struct __DISPLAY_HandleTypeDef *hdisplay);           /*!< DISPLAY interrupt callback         */
    void (* data_fifo_fw_Callback)(struct __DISPLAY_HandleTypeDef *hdisplay);           /*!< DISPLAY interrupt callback         */
    void (* cmd_fifo_fw_Callback)(struct __DISPLAY_HandleTypeDef *hdisplay);            /*!< DISPLAY interrupt callback         */
    void (* data_fifo_not_empty_Callback)(struct __DISPLAY_HandleTypeDef *hdisplay);    /*!< DISPLAY interrupt callback         */
    void (* cmd_fifo_done_Callback)(struct __DISPLAY_HandleTypeDef *hdisplay);          /*!< DISPLAY interrupt callback         */
    void (* tearing_int_Callback)(struct __DISPLAY_HandleTypeDef *hdisplay);            /*!< DISPLAY interrupt callback         */
    void (* fifo_empty_rd_Callback)(struct __DISPLAY_HandleTypeDef *hdisplay);          /*!< DISPLAY interrupt callback         */
    void (* vs_int_Callback)(struct __DISPLAY_HandleTypeDef *hdisplay);                 /*!< DISPLAY interrupt callback         */
#endif  /* USE_HAL_DISPLAY_REGISTER_CALLBACKS */
}DISPLAY_HandleTypeDef;

/** @defgroup DISPLAY_Error_Code_definition DISPLAY Error Code definition
  * @brief  DISPLAY Error Code definition
  * @{
  */
#define HAL_DISPLAY_ERROR_NONE              0x00000000U    /*!< No error              */
#if(defined USE_HAL_DISPLAY_REGISTER_CALLBACKS && USE_HAL_DISPLAY_REGISTER_CALLBACKS == 1U)
#define HAL_DISPLAY_ERROR_INVALID_CALLBACK  0x00000100U    /*!< Invalid Callback error */
#endif /* USE_HAL_DISPLAY_REGISTER_CALLBACKS */

/* Callbacks Register/UnRegister functions  ***********************************/
#if(defined USE_HAL_DISPLAY_REGISTER_CALLBACKS && USE_HAL_DISPLAY_REGISTER_CALLBACKS == 1U)
/**
  * @brief  HAL DISPLAY Callback ID enumeration definition
  */
typedef enum
{
    HAL_DISPLAY_DATA_FIFO_ER_CB_ID = 0x01U,     /*!< DISPLAY Interrupt callback ID      */
    HAL_DISPLAY_DATA_FIFO_FW_CB_ID,             /*!< DISPLAY Interrupt callback ID      */
    HAL_DISPLAY_CMD_FIFO_FW_CB_ID,              /*!< DISPLAY Interrupt callback ID      */
    HAL_DISPLAY_DATA_FIFO_NOT_EMPTY_ID,         /*!< DISPLAY Interrupt callback ID      */
    HAL_DISPLAY_CMD_FIFO_DONE_ID,               /*!< DISPLAY Interrupt callback ID      */
    HAL_DISPLAY_TEARING_INT_CB_ID,              /*!< DISPLAY Interrupt callback ID      */
    HAL_DISPLAY_FIFO_EMPTY_RD_CB_ID,            /*!< DISPLAY Interrupt callback ID      */
    HAL_DISPLAY_VS_INT_CB_ID ,                  /*!< DISPLAY Interrupt callback ID      */
    HAL_DISPLAY_MSPINIT_CB_ID,                  /*!< DISPLAY Msp Init callback ID       */
    HAL_DISPLAY_MSPDEINIT_CB_ID                 /*!< DISPLAY Msp DeInit callback ID     */
}HAL_DISPLAY_CallbackIDTypeDef;
typedef void (*pDISPLAY_CallbackTypeDef)(DISPLAY_HandleTypeDef *hdisplay); /*!< pointer to an display callback function */

HAL_StatusTypeDef HAL_DISPLAY_RegisterCallback(DISPLAY_HandleTypeDef *hdisplay, HAL_DISPLAY_CallbackIDTypeDef CallbackID, pDISPLAY_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_DISPLAY_UnRegisterCallback(DISPLAY_HandleTypeDef *hdisplay, HAL_DISPLAY_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_DISPLAY_REGISTER_CALLBACKS */


static inline void __HAL_DISPLAY_ENABLE(DISPLAY_HandleTypeDef *hdisplay)
{
    SET_BIT(hdisplay->Instance->display_cntl.reg_u32val, 0x1);
}
static inline void __HAL_DISPLAY_DISABLE(DISPLAY_HandleTypeDef *hdisplay)
{
    CLEAR_BIT(hdisplay->Instance->display_cntl.reg_u32val, 0x1);
}
typedef enum
{
    DISPLAY_REG = 0U,
    DISPLAY_DATA = 1U,
}DISPLAY_CmdType;
/**
  * @brief  Example Query whether the lcd data FIFO is empty.
  * @param  hdisplay pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @retval bool true: data fifo empty, otherwise the contrary.
  */
static inline bool HAL_DISPLAY_IsLCDDataFifoEmpty(DISPLAY_HandleTypeDef *hdisplay)
{
    return (hdisplay->Instance->fifo_status.bit.data_fifo_empty != 0);
}
/**
  * @brief  Update set.
  * @param  hdisplay pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  enable true-enable, false-disable.
  * @retval void.
  */
static inline void HAL_DISPLAY_UpdateSet(DISPLAY_HandleTypeDef *hdisplay, bool enable)
{
    union _disp_cntl tempreg;
    tempreg.reg_u32val = hdisplay->Instance->display_cntl.reg_u32val;
    tempreg.bit.update_en = (enable ? 1 : 0);
    hdisplay->Instance->display_cntl.reg_u32val = tempreg.reg_u32val;
}
/**
  * @brief  dithering enable.
  * @param  hdisplay pointer to a DISPLAY_TypeDef structure of the specified display peripheral.
  * @param  enable true-enable, false-disable.
  * @retval void.
  */
static inline void HAL_DISPLAY_DitheringSet(DISPLAY_HandleTypeDef *hdisplay, bool enable)
{
    union _disp_cntl tempreg;
    tempreg.reg_u32val = hdisplay->Instance->display_cntl.reg_u32val;
    tempreg.bit.dithering = (enable ? 1 : 0);
    hdisplay->Instance->display_cntl.reg_u32val = tempreg.reg_u32val;
}
typedef enum
{
    DISPLAY_IT_vs_int               = 0x01U,
    DISPLAY_IT_fifo_empty_rd        = 0x02U,
    DISPLAY_IT_tearing_int          = 0x04U,
    DISPLAY_IT_cmd_fifo_done        = 0x10U,
    DISPLAY_IT_data_fifo_not_empty  = 0x20U,
    DISPLAY_IT_cmd_fifo_fw          = 0x40U,
    DISPLAY_IT_data_fifo_fw         = 0x80U,
    DISPLAY_IT_data_fifo_er         = 0x100U,
    DISPLAY_IT_ALL                  = 0x1FFU,
}DISPLAY_InterruptType;
static inline void __HAL_DISPLAY_IT_ENABLE(DISPLAY_HandleTypeDef *hdisplay, DISPLAY_InterruptType interrupt)
{
    hdisplay->Instance->interrupt_mask |= (uint32_t)interrupt;
}
static inline void __HAL_DISPLAY_IT_DISABLE(DISPLAY_HandleTypeDef *hdisplay, DISPLAY_InterruptType interrupt)
{
    hdisplay->Instance->interrupt_mask &= ~((uint32_t)(interrupt));
}
static inline bool __HAL_DISPLAY_GET_IT_FLAG(DISPLAY_HandleTypeDef *hdisplay, DISPLAY_InterruptType interrupt)
{
    return ((hdisplay->Instance->interrupt & ((uint32_t)(interrupt))) != 0);
}
static inline void __HAL_DISPLAY_CLEAR_IT_FLAG(DISPLAY_HandleTypeDef *hdisplay, DISPLAY_InterruptType interrupt)
{
    hdisplay->Instance->interrupt &= ~((uint32_t)(interrupt));
}
typedef struct
{
    //red in red
    uint8_t Mr0; /*!< Fixed-point integer: 0.1.7 , range: 0~1.992 */
    //red in green
    uint8_t Mr1; /*!< Fixed-point integer: 0.1.7 , range: 0~1.992 */
    //red in blue
    uint8_t Mr2; /*!< Fixed-point integer: 0.1.7 , range: 0~1.992 */
    uint8_t Mr3; /*!< Fixed-point integer: 1.7.0 , range: -128~128 */

    //green in red
    uint8_t Mg0; /*!< Fixed-point integer: 0.1.7 , range: 0~1.992 */
    //green in green
    uint8_t Mg1; /*!< Fixed-point integer: 0.1.7 , range: 0~1.992 */
    //green in blue
    uint8_t Mg2; /*!< Fixed-point integer: 0.1.7 , range: 0~1.992 */
    uint8_t Mg3; /*!< Fixed-point integer: 1.7.0 , range: -128~128 */

    //blue in red
    uint8_t Mb0; /*!< Fixed-point integer: 0.1.7 , range: 0~1.992 */
    //blue in green
    uint8_t Mb1; /*!< Fixed-point integer: 0.1.7 , range: 0~1.992 */
    //blue in blue
    uint8_t Mb2; /*!< Fixed-point integer: 0.1.7 , range: 0~1.992 */
    uint8_t Mb3; /*!< Fixed-point integer: 1.7.0 , range: -128~128 */
}DISPLAY_CCM_Type;
typedef struct
{
    uint8_t Mco0; /*!< Fixed-point integer: 1.1.6 , range: -2~1.984 */
    uint8_t Mco1; /*!< Fixed-point integer: 1.1.6 , range: -2~1.984 */
    uint8_t Mcg0; /*!< Fixed-point integer: 1.1.6 , range: -2~1.984 */
    uint8_t Mcg1; /*!< Fixed-point integer: 1.1.6 , range: -2~1.984 */
}DISPLAY_YCT_Type;




void HAL_DISPLAY_StructInit(DISPLAY_InitTypeDef *DISPLAY_InitStruct);
HAL_StatusTypeDef HAL_DISPLAY_Init(DISPLAY_HandleTypeDef *hdisplay);
HAL_StatusTypeDef HAL_DISPLAY_DeInit(DISPLAY_HandleTypeDef *hdisplay);
HAL_StatusTypeDef HAL_DISPLAY_SwitchMode(DISPLAY_HandleTypeDef *hdisplay, DISPLAY_Mode mode);
HAL_StatusTypeDef HAL_DISPLAY_SetDiv(DISPLAY_HandleTypeDef *hdisplay, DISPLAY_Div_Enum div);
void HAL_DISPLAY_SetFrameBuffer(DISPLAY_HandleTypeDef *hdisplay, uint32_t base_addr, uint32_t bytes, uint32_t burst_length);

void HAL_DISPLAY_SendCmd(DISPLAY_HandleTypeDef *hdisplay, DISPLAY_CmdType cmd_type, uint8_t reg_or_data);
void HAL_DISPLAY_Reset(DISPLAY_HandleTypeDef *hdisplay);
void HAL_DISPLAY_Set(DISPLAY_HandleTypeDef *hdisplay);

void HAL_DISPLAY_MspInit(DISPLAY_HandleTypeDef *hdisplay);
void HAL_DISPLAY_MspDeInit(DISPLAY_HandleTypeDef *hdisplay);
void HAL_DISPLAY_IRQCB_vs_int(DISPLAY_HandleTypeDef *hdisplay);
void HAL_DISPLAY_IRQCB_fifo_empty_rd(DISPLAY_HandleTypeDef *hdisplay);
void HAL_DISPLAY_IRQCB_tearing_int(DISPLAY_HandleTypeDef *hdisplay);
void HAL_DISPLAY_IRQCB_cmd_fifo_done(DISPLAY_HandleTypeDef *hdisplay);
void HAL_DISPLAY_IRQCB_data_fifo_not_empty(DISPLAY_HandleTypeDef *hdisplay);
void HAL_DISPLAY_IRQCB_cmd_fifo_fw(DISPLAY_HandleTypeDef *hdisplay);
void HAL_DISPLAY_IRQCB_data_fifo_fw(DISPLAY_HandleTypeDef *hdisplay);
void HAL_DISPLAY_IRQCB_data_fifo_er(DISPLAY_HandleTypeDef *hdisplay);

HAL_StatusTypeDef HAL_DISPLAY_ReadReg(DISPLAY_HandleTypeDef *hdisplay, uint32_t regID, uint8_t *pdata, uint32_t read_byte, uint32_t Timeout);

void HAL_DISPLAY_GetCCM(DISPLAY_HandleTypeDef *hdisplay, DISPLAY_CCM_Type *ccm);
void HAL_DISPLAY_SetCCM(DISPLAY_HandleTypeDef *hdisplay, const DISPLAY_CCM_Type *ccm);
void HAL_DISPLAY_GetYCT(DISPLAY_HandleTypeDef *hdisplay, DISPLAY_YCT_Type *yct);
void HAL_DISPLAY_SetYCT(DISPLAY_HandleTypeDef *hdisplay, const DISPLAY_YCT_Type *yct);

void HAL_DISPLAY_IRQHandler(DISPLAY_HandleTypeDef *hdisplay);

#endif/* HAL_DISPLAY_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC9002_HAL_DISPLAY_H__ */
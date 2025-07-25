/**
  ******************************************************************************
  * @file    gc9002_hal_ahmi.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2023-1-6
  * @brief   This file contains all the functions prototypes for the AHMI
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC9002_HAL_AHMI_H__
#define __GC9002_HAL_AHMI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal_def.h"

#ifdef HAL_AHMI_MODULE_ENABLED

union _AHMI_CONTROL
{
	volatile uint32_t reg_u32val;            /*!< Offset: 0x08  (R/W) */
	struct
    {
        volatile uint32_t AHMI_CORE_RESET   : 1;
        volatile uint32_t DRAM_CACHE_RESET  : 1;
         uint32_t                           : 26;
        volatile uint32_t Gate_CLK_EN       : 1;
        uint32_t                            : 3;
    }bit;
};
union _AHMI_DRAW_TILE_BOX
{
	volatile uint32_t reg_u32val;            /*!< Offset: 0x1C  (R/W) */
	struct
    {
        volatile uint32_t X_START_TILE  : 8;
        volatile uint32_t Y_START_TILE  : 8;
        volatile uint32_t X_STOP_TILE   : 8;
        volatile uint32_t Y_STOP_TILE   : 8;
    }bit;
};
union _AHMI_FRAM_SIZE
{
	volatile uint32_t reg_u32val;            /*!< Offset: 0x30  (R/W) */
	struct
    {
        volatile uint32_t FRAME_WIDTH   : 16;
        volatile uint32_t FRAME_HEIGHT  : 16;
    }bit;
};
union _AHMI_START
{
	volatile uint32_t reg_u32val;            /*!< Offset: 0x0C  (R/W) */
	struct
    {
        volatile uint32_t GPE_Start : 1;
        uint32_t                    : 3;
        volatile uint32_t alpha_reverse_in_byte : 1;
        uint32_t                    : 27;
    }bit;
};
union _AHMI_Y8_WEIGHT
{
	volatile uint32_t reg_u32val;            /*!< Offset: 0x0C  (R/W) */
	struct
    {
        volatile uint32_t YR_W : 9;
        volatile uint32_t YG_W : 9;
        volatile uint32_t YB_W : 9;
        uint32_t               : 5;
    }bit;
};
union _AHMI_SR
{
	volatile uint32_t reg_u32val;            /*!< Offset: 0x10  (R/W) */
	struct
    {
        volatile uint32_t DrawDone              : 1;
        volatile uint32_t SBReadTimeOver        : 1;
        volatile uint32_t SBEndNotFound         : 1;
        volatile uint32_t DramCacheReadTimeOver : 1;
        volatile uint32_t DrawCallTimeOver      : 1;
        uint32_t                                : 26;
        volatile uint32_t Altor32Done           : 1;
    }bit;
};
union _AHMI_IMR
{
	volatile uint32_t reg_u32val;            /*!< Offset: 0x10  (R/W) */
	struct
    {
        volatile uint32_t DrawDone              : 1;
        volatile uint32_t SBReadTimeOver        : 1;
        volatile uint32_t SBEndNotFound         : 1;
        volatile uint32_t DramCacheReadTimeOver : 1;
        volatile uint32_t DrawCallTimeOver      : 1;
        uint32_t                                : 26;
        volatile uint32_t IRQ_EN                : 1;
    }bit;
};

typedef struct
{
    volatile uint32_t SYSTEM_INF;               //reg00;
    volatile uint32_t DEVICE_VERSION;           //reg04;
    union _AHMI_CONTROL CONTROL;                //reg08;
    union _AHMI_START START;                    //reg0c;
    union _AHMI_SR SR;                          //reg10;
    union _AHMI_IMR IMR;                        //reg14;
    volatile uint32_t OUTPUT_ROTATION;          //reg18;
    union _AHMI_DRAW_TILE_BOX DRAW_TILE_BOX;    //reg1c;
    volatile uint32_t MAX_ELE_SB;               //reg20;
    volatile uint32_t RESERVED24;               //reg24;
    volatile uint32_t FB_BASE_ADDR;             //reg28;
    volatile uint32_t INTERRUPT;                //reg2c;
    union _AHMI_FRAM_SIZE FRAM_SIZE;            //reg30;
    volatile uint32_t OUTPUT_BIT;               //reg34;
    volatile uint32_t FILTER_RANGE;             //reg38;
    volatile uint32_t RESERVED3c;               //reg3c;
    volatile uint32_t RESERVED40;               //reg40;
    volatile uint32_t RESERVED44;               //reg44;
    volatile uint32_t RESERVED48;               //reg48;
    volatile uint32_t RESERVED4c;               //reg4c;
    volatile uint32_t FILTER_COUNT;             //reg50;
    volatile uint32_t RESERVED54;               //reg54;
    volatile uint32_t RESERVED58;               //reg58;
    volatile uint32_t RESERVED5c;               //reg5c;
    volatile uint32_t CORE0_RASTER_COUNT;       //reg60;
    volatile uint32_t CORE1_RASTER_COUNT;       //reg64;
    volatile uint32_t RESERVED68;               //reg68;
    volatile uint32_t RESERVED6c;               //reg6c;
    volatile uint32_t CORE0_DRAW_INF;           //reg70;
    volatile uint32_t CORE1_DRAW_INF;           //reg74;
    volatile uint32_t RESERVED78;               //reg78;
    volatile uint32_t RESERVED7C;               //reg7c;
    volatile uint32_t CORE0_CACHE_BUSY_COUNT_L; //reg80;
    volatile uint32_t CORE0_CACHE_BUSY_COUNT_H; //reg84; 
    volatile uint32_t CORE1_CACHE_BUSY_COUNT_L; //reg88;
    volatile uint32_t CORE1_CACHE_BUSY_COUNT_H; //reg8c;
    volatile uint32_t RESERVED90;               //reg90;
    volatile uint32_t RESERVED94;               //reg94;
    volatile uint32_t RESERVED98;               //reg98;
    volatile uint32_t RESERVED9c;               //reg9c;
    volatile uint32_t CORE0_DRAW_PIXEL_COUNT_L; //rega0; 
    volatile uint32_t CORE0_DRAW_PIXEL_COUNT_H; //rega4; 
    volatile uint32_t CORE1_DRAW_PIXEL_COUNT_L; //rega8;
    volatile uint32_t CORE1_DRAW_PIXEL_COUNT_H; //regac;
    volatile uint32_t RESERVEDb0;               //regb0;
    volatile uint32_t RESERVEDb4;               //regb4;
    volatile uint32_t MAX_DRAW_CALL_CYCLE;      //regb8;
    volatile uint32_t MAX_DRAM_CACHE_REQ_CYCLE; //regbc;
    volatile uint32_t ENABLE_WINDOW_CUT;        //regc0;
    volatile uint32_t WINDOW_COORDINATE0;       //regc4;
    volatile uint32_t WINDOW_COORDINATE1;       //regc8;
    volatile uint32_t WINDOW_COORDINATE2;       //regcc;
    volatile uint32_t WINDOW_COORDINATE3;       //regd0;
    union _AHMI_Y8_WEIGHT Y8_WEIGHT;            //regd4;
    volatile uint32_t SRAM_BASE_ADDR;           //regd8;
}AHMI_TypeDef;

/* AHMI definitions */
#define AHMI_BASE           (0x40104000U)
#define AHMI                ((AHMI_TypeDef *)AHMI_BASE)


#define ANGLE0               0x0 
#define ANGLE90              0x1
#define ANGLE180             0x2
#define ANGLE270             0x3
#define OUT_32BIT            0x0
#define OUT_16BIT            0x1


enum AHMI_INT_Source
{
    AHMI_INT_DrawDone               = 0x00000001U,
    AHMI_INT_SBReadTimeOver         = 0x00000002U,
    AHMI_INT_SBEndNotFound          = 0x00000004U,
    AHMI_INT_DramCacheReadTimeOver  = 0x00000008U,
    AHMI_INT_DrawCallTimeOver       = 0x00000010U,
    AHMI_INT_Altor32Done            = 0x80000000U,
};
#define AHMI_IRQ_ENABLE             (0x80000000U)
#define __AHMI_GET_IT_STATUS(_STATUS_)          ((AHMI->SR.reg_u32val & (_STATUS_)) != RESET)
#define __AHMI_IT_ENABLE(_IT_)                  SET_BIT(AHMI->IMR.reg_u32val, (_IT_))
#define __AHMI_IT_DISABLE(_IT_)                 CLEAR_BIT(AHMI->IMR.reg_u32val, ((_IT_) | AHMI_IRQ_ENABLE))

typedef enum
{
    AHMI_Rotation_0 = 0U,   //0 degree
    AHMI_Rotation_90,       //90 degree
    AHMI_Rotation_180,      //180 degree
    AHMI_Rotation_270,      //270 degree
}AHMI_RotationEnum;
typedef enum
{
    AHMI_DISABLE = 0U,
    AHMI_ENABLE = 1U,
}AHMI_EN_Enum;
typedef enum
{
    AHMI_RGBA8888 = 0U,
    AHMI_RGB565 = 1U,
    AHMI_RGB888 = 2U,
    AHMI_SFBC = 3U,
    AHMI_Y8 = 4U,
}AHMI_OutputBit_Enum;
typedef struct
{
    struct
    {
        uint32_t Gate_CLK_EN    : 1; 
        uint32_t DramCacheReset : 1;
        uint32_t AHMI_CoreReset : 1;
        uint32_t GPE_START      : 1; 
        uint32_t ROTATION       : 2;//@ref AHMI_RotationEnum
        uint32_t FilterRange    : 8; 
        uint32_t                : 18;
    };
    struct
    {
        uint32_t YR_W : 9;
        uint32_t YG_W : 9;
        uint32_t YB_W : 9;
        uint32_t      : 5;
    };
    struct
    {
        uint8_t X_START_TILE;   /* X direction of start tile */
        uint8_t X_STOP_TILE;    /* X direction of end tile */
        uint8_t Y_START_TILE;   /* Y direction of start tile */
        uint8_t Y_STOP_TILE;    /* Y direction of end tile */
    };
    struct
    {
        uint16_t FRAME_HEIGHT;  /* frame hight */
        uint16_t FRAME_WIDTH;   /* frame width */
    };
    uint32_t MAX_SIZE_OF_SB_BUFFR;  /* maximum elements of source buffer */
    uint32_t SB_BASE_ADDR;          /* source buffer base address */
    uint32_t FB_BASE_ADDR;          /* frame buffer base address */
    uint32_t M_DRAW_CALL_CYCLE;
    uint32_t M_CACHE_REQ_CYCLE;
    uint32_t OUTPUT_COLOR_BITS; /*the format of output, @ref AHMI_OutputBit_Enum*/
}AHMI_InitTypeDef;



/** @defgroup AHMI_Error_Code_definition AHMI Error Code definition
  * @brief  AHMI Error Code definition
  * @{
  */
#define HAL_AHMI_ERROR_NONE              0x00000000U    /*!< No error              */
#if(defined USE_HAL_AHMI_REGISTER_CALLBACKS && USE_HAL_AHMI_REGISTER_CALLBACKS == 1U)
#define HAL_AHMI_ERROR_INVALID_CALLBACK  0x00000100U    /*!< Invalid Callback error */
#endif /* USE_HAL_AHMI_REGISTER_CALLBACKS */

typedef enum
{
    HAL_AHMI_STATE_RESET             = 0x00U,   /*!< Peripheral is not yet Initialized         */
    HAL_AHMI_STATE_READY             = 0x02U,   /*!< Peripheral Initialized and ready for use  */
    HAL_AHMI_STATE_BUSY              = 0x04U,   /*!< An internal process is ongoing            */
    HAL_AHMI_STATE_TIMEOUT           = 0x08U,   /*!< Timeout state                             */
    HAL_AHMI_STATE_ERROR             = 0xE0U    /*!< Error                                     */
} HAL_AHMI_StateTypeDef;

/**
  * @brief  AHMI handle Structure definition
  */
typedef struct __AHMI_HandleTypeDef
{
    AHMI_TypeDef                *Instance;      /*!< AHMI registers base address                */
    AHMI_InitTypeDef            Init;           /*!< AHMI communication parameters              */
    uint32_t                    status;         /*!< Interrupt status                           */
    HAL_LockTypeDef             Lock;           /*!< Locking object                             */
    __IO HAL_AHMI_StateTypeDef  State;          /*!< AHMI communication state                   */
    __IO uint32_t               ErrorCode;      /*!< AHMI Error code                            */
#if(defined USE_HAL_AHMI_REGISTER_CALLBACKS && USE_HAL_AHMI_REGISTER_CALLBACKS == 1U)
    void (* ISRCallback)(struct __AHMI_HandleTypeDef *hahmi);    /*!< AHMI interrupt callback          */
#endif  /* USE_HAL_AHMI_REGISTER_CALLBACKS */
}AHMI_HandleTypeDef;

/* Callbacks Register/UnRegister functions  ***********************************/
#if(defined USE_HAL_AHMI_REGISTER_CALLBACKS && USE_HAL_AHMI_REGISTER_CALLBACKS == 1U)
/**
  * @brief  HAL AHMI Callback ID enumeration definition
  */
typedef enum
{
    HAL_AHMI_ISR_CB_ID       = 0x01U,        /*!< AHMI Interrupt callback ID      */
}HAL_AHMI_CallbackIDTypeDef;
typedef void (*pAHMI_CallbackTypeDef)(AHMI_HandleTypeDef *hahmi); /*!< pointer to an display callback function */

HAL_StatusTypeDef HAL_AHMI_RegisterCallback(AHMI_HandleTypeDef *hahmi, HAL_AHMI_CallbackIDTypeDef CallbackID, pAHMI_CallbackTypeDef pCallback);
HAL_StatusTypeDef HAL_AHMI_UnRegisterCallback(AHMI_HandleTypeDef *hahmi, HAL_AHMI_CallbackIDTypeDef CallbackID);
#endif /* USE_HAL_AHMI_REGISTER_CALLBACKS */



static inline void HAL_AHMI_GpeStart(void)
{
    union _AHMI_START tempreg;
    tempreg.reg_u32val = AHMI->START.reg_u32val;
    tempreg.bit.GPE_Start = 1;
    AHMI->START.reg_u32val = tempreg.reg_u32val;
}

void HAL_AHMI_StructInit(AHMI_InitTypeDef *AHMI_InitStruct);
HAL_StatusTypeDef HAL_AHMI_Init(AHMI_HandleTypeDef *hahmi);


void HAL_AHMI_IRQHandler(AHMI_HandleTypeDef *hahmi);

#endif/* HAL_AHMI_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC9002_HAL_AHMI_H__ */
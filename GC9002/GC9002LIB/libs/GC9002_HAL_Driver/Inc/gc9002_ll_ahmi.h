/**
  ******************************************************************************
  * @file    gc9002_ll_ahmi.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2023-1-6
  * @brief   This file contains all the functions prototypes for the AHMI
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC9002_LL_AHMI_H__
#define __GC9002_LL_AHMI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal_def.h"

#ifdef HAL_AHMI_MODULE_ENABLED

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
    AHMI_Rotation_0,
    AHMI_Rotation_90,
    AHMI_Rotation_180,
    AHMI_Rotation_270,
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
        uint32_t ROTATION       : 2;//0: 0 degree  1: 90 degree   2: 180 degree   3: 270 degree
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
        uint8_t X_START_TILE;
        uint8_t X_STOP_TILE;
        uint8_t Y_START_TILE;
        uint8_t Y_STOP_TILE;
    };
    struct
    {
        uint16_t FRAME_HEIGHT;
        uint16_t FRAME_WIDTH;
    };
    uint32_t MAX_SIZE_OF_SB_BUFFR;
    uint32_t SB_BASE_ADDR;
    uint32_t FB_BASE_ADDR;
    uint32_t M_DRAW_CALL_CYCLE;
    uint32_t M_CACHE_REQ_CYCLE;
    uint32_t OUTPUT_COLOR_BITS;//b000 rgba8888   b001 rgb565   b010 rgb888    b011 sfbc    b100 y8
}AHMI_InitTypeDef;



void AHMI_StructInit(AHMI_InitTypeDef *AHMI_InitStruct);
void AHMI_Init(AHMI_InitTypeDef *AHMI_InitStruct);
void AHMI_GpeStart(void);

#endif /* HAL_AHMI_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC9002_LL_AHMI_H__ */
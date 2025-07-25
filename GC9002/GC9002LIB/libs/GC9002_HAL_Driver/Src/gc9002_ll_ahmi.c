/**
  ******************************************************************************
  * @file    gc9002_hal_display.c
  * @author  GraphichinaSX Team
  * @version V1.0.1
  * @date    2023/04/11
  * @brief   This file provides all the DISPLAY firmware functions.
  ******************************************************************************
  */
#include "gc90xx_hal.h"
#ifdef HAL_AHMI_MODULE_ENABLED

void AHMI_Init(AHMI_InitTypeDef *AHMI_InitStruct)
{
    uint32_t tempreg = 0;
    ((union _AHMI_CONTROL *)&tempreg)->bit.Gate_CLK_EN = AHMI_InitStruct->Gate_CLK_EN;
    ((union _AHMI_CONTROL *)&tempreg)->bit.AHMI_CORE_RESET = AHMI_InitStruct->AHMI_CoreReset;
    ((union _AHMI_CONTROL *)&tempreg)->bit.DRAM_CACHE_RESET = AHMI_InitStruct->DramCacheReset;
    AHMI->CONTROL.reg_u32val = tempreg;

    tempreg = 0;
    ((union _AHMI_DRAW_TILE_BOX *)&tempreg)->bit.X_START_TILE = AHMI_InitStruct->X_START_TILE;
    ((union _AHMI_DRAW_TILE_BOX *)&tempreg)->bit.Y_START_TILE = AHMI_InitStruct->Y_START_TILE;
    ((union _AHMI_DRAW_TILE_BOX *)&tempreg)->bit.X_STOP_TILE = AHMI_InitStruct->X_STOP_TILE;
    ((union _AHMI_DRAW_TILE_BOX *)&tempreg)->bit.Y_STOP_TILE = AHMI_InitStruct->Y_STOP_TILE;
    AHMI->DRAW_TILE_BOX.reg_u32val	= tempreg;
    
    tempreg = 0;
    ((union _AHMI_FRAM_SIZE *)&tempreg)->bit.FRAME_WIDTH = AHMI_InitStruct->FRAME_WIDTH;
    ((union _AHMI_FRAM_SIZE *)&tempreg)->bit.FRAME_HEIGHT = AHMI_InitStruct->FRAME_HEIGHT;
    AHMI->FRAM_SIZE.reg_u32val = tempreg;

    tempreg = 0;
    ((union _AHMI_Y8_WEIGHT *)&tempreg)->bit.YB_W = AHMI_InitStruct->YB_W;
    ((union _AHMI_Y8_WEIGHT *)&tempreg)->bit.YG_W = AHMI_InitStruct->YG_W;
    ((union _AHMI_Y8_WEIGHT *)&tempreg)->bit.YR_W = AHMI_InitStruct->YR_W;
    AHMI->Y8_WEIGHT.reg_u32val = tempreg;

    AHMI->FILTER_RANGE      = AHMI_InitStruct->FilterRange;
    AHMI->OUTPUT_BIT        = AHMI_InitStruct->OUTPUT_COLOR_BITS;
    AHMI->OUTPUT_ROTATION   = AHMI_InitStruct->ROTATION;
    AHMI->MAX_ELE_SB        = AHMI_InitStruct->MAX_SIZE_OF_SB_BUFFR;
    // AHMI->SB_BASE_ADDR      = AHMI_InitStruct->SB_BASE_ADDR;
    AHMI->FB_BASE_ADDR      = AHMI_InitStruct->FB_BASE_ADDR;
    AHMI->MAX_DRAW_CALL_CYCLE      = AHMI_InitStruct->M_DRAW_CALL_CYCLE;
    AHMI->MAX_DRAM_CACHE_REQ_CYCLE = AHMI_InitStruct->M_CACHE_REQ_CYCLE;
};

void AHMI_StructInit(AHMI_InitTypeDef *AHMI_InitStruct)
{   
    assert_param(AHMI_InitStruct != NULL);
	AHMI_InitStruct->Gate_CLK_EN             = AHMI_ENABLE;
	AHMI_InitStruct->DramCacheReset          = AHMI_DISABLE;
	AHMI_InitStruct->AHMI_CoreReset          = AHMI_DISABLE;
    AHMI_InitStruct->ROTATION                = AHMI_Rotation_0;
    AHMI_InitStruct->FilterRange             = 128;
    AHMI_InitStruct->X_START_TILE            = 0;  
    AHMI_InitStruct->X_STOP_TILE             = 14;//FRAME_WIDTH/16-1
	AHMI_InitStruct->Y_START_TILE            = 0;
    AHMI_InitStruct->Y_STOP_TILE             = 19;//FRAME_HEIGHT/16-1
    AHMI_InitStruct->MAX_SIZE_OF_SB_BUFFR    = 280;
    // AHMI_InitStruct->SB_BASE_ADDR	         = 0x20000000;
	AHMI_InitStruct->FB_BASE_ADDR            = 0x30000000; //slave:0x30000000;master:0x20000100;
    AHMI_InitStruct->FRAME_HEIGHT            = 320; 
	AHMI_InitStruct->FRAME_WIDTH             = 240;
    AHMI_InitStruct->OUTPUT_COLOR_BITS       = AHMI_RGB565;
    AHMI_InitStruct->YR_W                    = 85;//1/3
    AHMI_InitStruct->YG_W                    = 85;//1/3
    AHMI_InitStruct->YB_W                    = 85;//1/3
    AHMI_InitStruct->M_DRAW_CALL_CYCLE       = 0x00000100;
    AHMI_InitStruct->M_CACHE_REQ_CYCLE       = 0x00000200;
}
void AHMI_GpeStart(void)
{
    union _AHMI_START tempreg;
    tempreg.reg_u32val = AHMI->START.reg_u32val;
    tempreg.bit.GPE_Start = 1;
    AHMI->START.reg_u32val = tempreg.reg_u32val;
}

#endif /* HAL_AHMI_MODULE_ENABLED */

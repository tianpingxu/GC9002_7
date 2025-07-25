/**
  ******************************************************************************
  * @file    gc9002_hal_ahmi.c
  * @author  GraphichinaSX Team
  * @version V1.0.1
  * @date    2023/04/17
  * @brief   This file provides all the AHMI firmware functions.
  ******************************************************************************
    */
#include "gc90xx_hal.h"
#ifdef HAL_AHMI_MODULE_ENABLED



void HAL_AHMI_StructInit(AHMI_InitTypeDef *AHMI_InitStruct)
{
    assert_param(AHMI_InitStruct != NULL);
    AHMI_InitStruct->Gate_CLK_EN          = AHMI_ENABLE;
    AHMI_InitStruct->DramCacheReset       = AHMI_DISABLE;
    AHMI_InitStruct->AHMI_CoreReset       = AHMI_DISABLE;
    AHMI_InitStruct->ROTATION             = AHMI_Rotation_0;
    AHMI_InitStruct->FilterRange          = 128;
    AHMI_InitStruct->X_START_TILE         = 0;  
    AHMI_InitStruct->X_STOP_TILE          = 14;//FRAME_WIDTH/16-1
    AHMI_InitStruct->Y_START_TILE         = 0;
    AHMI_InitStruct->Y_STOP_TILE          = 19;//FRAME_HEIGHT/16-1
    AHMI_InitStruct->MAX_SIZE_OF_SB_BUFFR = 280;
    // AHMI_InitStruct->SB_BASE_ADDR	      = 0x20000000;
    AHMI_InitStruct->FB_BASE_ADDR         = 0x30000000; //slave:0x30000000;master:0x20000100;
    AHMI_InitStruct->FRAME_HEIGHT         = 320; 
    AHMI_InitStruct->FRAME_WIDTH          = 240;
    AHMI_InitStruct->OUTPUT_COLOR_BITS    = AHMI_RGB565;
    AHMI_InitStruct->YR_W                 = 85;//1/3
    AHMI_InitStruct->YG_W                 = 85;//1/3
    AHMI_InitStruct->YB_W                 = 85;//1/3
    AHMI_InitStruct->M_DRAW_CALL_CYCLE    = 0x00000100;
    AHMI_InitStruct->M_CACHE_REQ_CYCLE    = 0x00000200;
}
HAL_StatusTypeDef HAL_AHMI_Init(AHMI_HandleTypeDef *hahmi)
{
    /* Check the AHMI handle allocation */
    if(hahmi == NULL)
    {
        return HAL_ERROR;
    }
    uint32_t tempreg = 0;
    if(hahmi->State == HAL_AHMI_STATE_RESET)
    {
        /* Allocate lock resource and initialize it */
        __HAL_LOCK(hahmi);
    }
    hahmi->State = HAL_AHMI_STATE_BUSY;
    ((union _AHMI_CONTROL *)&tempreg)->bit.Gate_CLK_EN = hahmi->Init.Gate_CLK_EN;
    ((union _AHMI_CONTROL *)&tempreg)->bit.AHMI_CORE_RESET = hahmi->Init.AHMI_CoreReset;
    ((union _AHMI_CONTROL *)&tempreg)->bit.DRAM_CACHE_RESET = hahmi->Init.DramCacheReset;
    AHMI->CONTROL.reg_u32val = tempreg;

    tempreg = 0;
    ((union _AHMI_DRAW_TILE_BOX *)&tempreg)->bit.X_START_TILE = hahmi->Init.X_START_TILE;
    ((union _AHMI_DRAW_TILE_BOX *)&tempreg)->bit.Y_START_TILE = hahmi->Init.Y_START_TILE;
    ((union _AHMI_DRAW_TILE_BOX *)&tempreg)->bit.X_STOP_TILE = hahmi->Init.X_STOP_TILE;
    ((union _AHMI_DRAW_TILE_BOX *)&tempreg)->bit.Y_STOP_TILE = hahmi->Init.Y_STOP_TILE;
    AHMI->DRAW_TILE_BOX.reg_u32val	= tempreg;
    
    tempreg = 0;
    ((union _AHMI_FRAM_SIZE *)&tempreg)->bit.FRAME_WIDTH = hahmi->Init.FRAME_WIDTH;
    ((union _AHMI_FRAM_SIZE *)&tempreg)->bit.FRAME_HEIGHT = hahmi->Init.FRAME_HEIGHT;
    AHMI->FRAM_SIZE.reg_u32val = tempreg;

    tempreg = 0;
    ((union _AHMI_Y8_WEIGHT *)&tempreg)->bit.YB_W = hahmi->Init.YB_W;
    ((union _AHMI_Y8_WEIGHT *)&tempreg)->bit.YG_W = hahmi->Init.YG_W;
    ((union _AHMI_Y8_WEIGHT *)&tempreg)->bit.YR_W = hahmi->Init.YR_W;
    AHMI->Y8_WEIGHT.reg_u32val = tempreg;

    hahmi->Instance->FILTER_RANGE      = hahmi->Init.FilterRange;
    hahmi->Instance->OUTPUT_BIT        = hahmi->Init.OUTPUT_COLOR_BITS;
    hahmi->Instance->OUTPUT_ROTATION   = hahmi->Init.ROTATION;
    hahmi->Instance->MAX_ELE_SB        = hahmi->Init.MAX_SIZE_OF_SB_BUFFR;
    // hahmi->Instance->SB_BASE_ADDR      = hahmi->Init.SB_BASE_ADDR;// 9002 be written 0x0200C000
    hahmi->Instance->FB_BASE_ADDR      = hahmi->Init.FB_BASE_ADDR;
    hahmi->Instance->MAX_DRAW_CALL_CYCLE      = hahmi->Init.M_DRAW_CALL_CYCLE;
    hahmi->Instance->MAX_DRAM_CACHE_REQ_CYCLE = hahmi->Init.M_CACHE_REQ_CYCLE;
    hahmi->ErrorCode = HAL_AHMI_ERROR_NONE;
    hahmi->State     = HAL_AHMI_STATE_READY;
    __HAL_UNLOCK(hahmi);
    return HAL_OK;
}

#if(defined USE_HAL_AHMI_REGISTER_CALLBACKS && USE_HAL_AHMI_REGISTER_CALLBACKS == 1U)
HAL_StatusTypeDef HAL_AHMI_RegisterCallback(AHMI_HandleTypeDef *hahmi, HAL_AHMI_CallbackIDTypeDef CallbackID, pAHMI_CallbackTypeDef pCallback)
{
    HAL_StatusTypeDef status = HAL_OK;
    if(pCallback == NULL)
    {
        /* Update the error code */
        hahmi->ErrorCode |= HAL_AHMI_ERROR_INVALID_CALLBACK;
        return HAL_ERROR;
    }
    /* Process locked */
    __HAL_LOCK(hahmi);
    if (HAL_AHMI_STATE_READY == hahmi->State)
    {
        switch (CallbackID)
        {
        case HAL_AHMI_ISR_CB_ID:
            hahmi->ISRCallback = pCallback;
        break;
        default :
            /* Update the error code */
            hahmi->ErrorCode |= HAL_AHMI_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status = HAL_ERROR;
        break;
        }
    }
    else if (HAL_AHMI_STATE_RESET == hahmi->State)
    {
        switch (CallbackID)
        {
        default:
            /* Update the error code */
            hahmi->ErrorCode |= HAL_AHMI_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status = HAL_ERROR;
        break;
        }
    }
    else
    {
        /* Update the error code */
        hahmi->ErrorCode |= HAL_AHMI_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status =  HAL_ERROR;
    }
    /* Release Lock */
    __HAL_UNLOCK(hahmi);
    return status;
}
HAL_StatusTypeDef HAL_AHMI_UnRegisterCallback(AHMI_HandleTypeDef *hahmi, HAL_AHMI_CallbackIDTypeDef CallbackID)
{
    HAL_StatusTypeDef status = HAL_OK;
    /* Process locked */
    __HAL_LOCK(hahmi);
    if (HAL_AHMI_STATE_READY == hahmi->State)
    {
        switch (CallbackID)
        {
        case HAL_AHMI_ISR_CB_ID:
            hahmi->ISRCallback = HAL_AHMI_IRQHandler;
        break;
        default:
            /* Update the error code */
            hahmi->ErrorCode |= HAL_AHMI_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status =  HAL_ERROR;
        break;
    }
    }
    else if (HAL_AHMI_STATE_RESET == hahmi->State)
    {
        switch (CallbackID)
        {
        default:
            /* Update the error code */
            hahmi->ErrorCode |= HAL_AHMI_ERROR_INVALID_CALLBACK;
            /* Return error status */
            status =  HAL_ERROR;
        break;
    }
    }
    else
    {
        /* Update the error code */
        hahmi->ErrorCode |= HAL_AHMI_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status =  HAL_ERROR;
    }
    /* Release Lock */
    __HAL_UNLOCK(hahmi);
    return status;
}
#endif

void HAL_AHMI_IRQHandler(AHMI_HandleTypeDef *hahmi)
{
    hahmi->status = hahmi->Instance->SR.reg_u32val;
#if(defined USE_HAL_AHMI_REGISTER_CALLBACKS && USE_HAL_AHMI_REGISTER_CALLBACKS == 1U)
    if(hahmi->ISRCallback)
    {
        hahmi->ISRCallback(hahmi);
    }
#else
    HAL_AHMI_ISR(hahmi);
#endif
}

/**
  * @brief  Interrupt callback.
  * @param  hahmi pointer to a AHMI_HandleTypeDef structure that contains
  *               the configuration information for AHMI module.
  * @retval None
  */
__WEAK void HAL_AHMI_ISR(AHMI_HandleTypeDef *hahmi)
{

}

#endif /* HAL_AHMI_MODULE_ENABLED */
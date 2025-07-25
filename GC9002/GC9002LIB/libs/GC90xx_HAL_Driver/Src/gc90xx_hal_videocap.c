/*
 * Copyright (c) 2022-2050, GraphichinaSX Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-18     huyt         V1.0.1
 */
#include <gc90xx_hal.h>

#ifdef HAL_VIDEOCAP_MODULE_ENABLED


HAL_StatusTypeDef HAL_VIDEOCAP_StructInit(VIDEOCAP_HandleTypeDef *hvideocap)
{
    if(hvideocap == NULL)
    {
        return HAL_ERROR;
    }
    /* BT656_InitStruct members default value */
    hvideocap->Init.ColorBit        = VIDEOCAP_COLOR_16BIT;
    hvideocap->Init.CaptureMode     = BT656_MODE_DVP;
    hvideocap->Init.DvpMode         = VIDEOCAP_DVP_YUV422;
    hvideocap->Init.YUVQuantization = VIDEOCAP_YUV_FullRange;
    hvideocap->Init.FieldSampleMode = VIDEOCAP_Sample_ODD_Filed;
    hvideocap->Init.DE_Pol          = VIDEOCAP_DE_Pol_High;
    hvideocap->Init.HS_Pol          = VIDEOCAP_HS_Pol_High;
    hvideocap->Init.VS_Pol          = VIDEOCAP_VS_Pol_Low;
    hvideocap->Init.YCBCR_POS       = VIDEOCAP_Y1_CB_Y0_CR;
    hvideocap->Init.DownscaleSample = VIDEOCAP_DownSample_Disable;
    hvideocap->Init.MaxAddrSize     = 0;
    hvideocap->Init.CH0FieldBaseAddr= 0;
    hvideocap->Init.FrameNumCnt     = 0;
    hvideocap->Init.MaxADCFrameNum  = 0;
    return HAL_OK;
}
/**
  * @brief  Initializes the BT656 module.
  * @param  BT656_InitStruct: pointer to a BT656_InitTypeDef structure
  *         that contains the configuration information for the BT565.
  * @retval status
  */
HAL_StatusTypeDef HAL_VIDEOCAP_Init(VIDEOCAP_HandleTypeDef *hvideocap)
{
    if(hvideocap == NULL)
    {
        return HAL_ERROR;
    }
    uint32_t tmpreg = 0;
    HAL_VIDEOCAP_MspInit(hvideocap);
    /*------------------ BT656 InterruptEnable configuration ------------*/
    hvideocap->Instance->IER = 0;
    hvideocap->Instance->ISR = 0;
    /*------------------ BT656 MaxAddr configuration --------------------*/
    hvideocap->Instance->MaxSize = hvideocap->Init.MaxAddrSize;
    /*------------------ BT656 BaseAddr configuration -------------------*/
    hvideocap->Instance->CH0BaseAddr = hvideocap->Init.CH0FieldBaseAddr;
    /*------------------ BT656 FrameNumCnt configuration -----------*/
    hvideocap->Instance->FrameNumCnt = hvideocap->Init.FrameNumCnt;
    /*------------------ BT656 MaxADCFrameNum configuration ----------*/
    hvideocap->Instance->MaxADCFrameNum = hvideocap->Init.MaxADCFrameNum;
    /*------------------ BT656 CR configuration -------------------------*/
    tmpreg = 0;
    tmpreg |= (hvideocap->Init.ColorBit |
               hvideocap->Init.CaptureMode |
               hvideocap->Init.DvpMode |
               hvideocap->Init.YUVQuantization |
               hvideocap->Init.FieldSampleMode |
               hvideocap->Init.DE_Pol |
               hvideocap->Init.HS_Pol |
               hvideocap->Init.VS_Pol |
               hvideocap->Init.YCBCR_POS |
               hvideocap->Init.DownscaleSample);
    hvideocap->Instance->CR = tmpreg;
    return HAL_OK;
}

/**
  * @brief  Set the BT656 baseaddr.
  * @param  None.
  * @retval None
  */
HAL_StatusTypeDef HAL_VIDEOCAP_SetBaseAddress(VIDEOCAP_HandleTypeDef *hvideocap, uint32_t address)
{
    if(hvideocap == NULL)
    {
        return HAL_ERROR;
    }
    /* Config base address of chunnel. */
    hvideocap->Instance->CH0BaseAddr = address;
    return HAL_OK;
}


/**
  * @brief  Disable the BT656 module.
  * @param  None.
  * @retval None
  */
HAL_StatusTypeDef HAL_VIDEOCAP_Disable(VIDEOCAP_HandleTypeDef *hvideocap)
{
    if(hvideocap == NULL)
    {
        return HAL_ERROR;
    }
    CLEAR_BIT(hvideocap->Instance->CR, BT656_ENABLE);
    return HAL_OK;
}


/**
  * @brief  Start the BT565 module.
  * @param  None.
  * @retval None
  */
HAL_StatusTypeDef HAL_VIDEOCAP_Start(VIDEOCAP_HandleTypeDef *hvideocap)
{
    if(hvideocap == NULL)
    {
        return HAL_ERROR;
    }
    SET_BIT(hvideocap->Instance->CR, BT656_ENABLE);
    return HAL_OK;
}

/**
  * @brief set BT656 sample mode.
  * @param mode: specified sample mode.
  *              This parameter can be: VIDEOCAP_Sample_ODD_Filed, VIDEOCAP_Sample_EVEN_Filed, VIDEOCAP_Sample_BOTH_Filed.
  * @retval None
  */
HAL_StatusTypeDef HAL_VIDEOCAP_SetSampleMode(VIDEOCAP_HandleTypeDef *hvideocap, VIDEOCAP_SampleMode_Enum mode)
{
    if(hvideocap == NULL)
    {
        return HAL_ERROR;
    }
    MODIFY_REG(hvideocap->Instance->CR, VIDEOCAP_SAMPLE_FIELD_MASK, mode);
    return HAL_OK;
}

/**
  * @brief Get specified channel field parameter
  * @param Height: field height
  * @param Width: field width
  * @retval None
  */
HAL_StatusTypeDef HAL_VIDEOCAP_GetParam(VIDEOCAP_HandleTypeDef *hvideocap, uint16_t *Width, uint16_t *Height)
{
    if(hvideocap == NULL || Height == NULL || Width == NULL)
    {
        return HAL_ERROR;
    }
    *Width = hvideocap->Instance->VideoWidth;
    *Height = hvideocap->Instance->VideoHeight;
    return HAL_OK;
}

/**
  * @brief  VIDEOCAP MSP Init.
  * @param  hvideocap  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified VIDEOCAP module.
  * @retval None
  */
__WEAK void HAL_VIDEOCAP_MspInit(VIDEOCAP_HandleTypeDef *hvideocap)
{

}
#endif /* HAL_VIDEOCAP_MODULE_ENABLED */

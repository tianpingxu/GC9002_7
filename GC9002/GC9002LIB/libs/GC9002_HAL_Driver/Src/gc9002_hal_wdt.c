/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-02-01     huyt         the first version
 */
#include "gc90xx_hal.h"
#ifdef HAL_WDT_MODULE_ENABLED

/**
  * @brief  Initialize the WDT according to the specified.
  *         parameters in the WDT_InitTypeDef of  associated handle.
  * @param  hwdt  pointer to a WDT_HandleTypeDef structure that contains
  *                the configuration information for the specified WDT module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_WDT_StructInit(WDT_HandleTypeDef *hwdt)
{
	/* Check the WDT handle allocation */
	if (hwdt == NULL)
	{
		return HAL_ERROR;
	}
	hwdt->Init.ResetMode = WDT_INTERRUPT_MODE;
	hwdt->Init.ResetPulseLen = WDT_PCLK_CYCLES_32;
	hwdt->Init.Counter = WDT_Timeout_2G_CLOCKS;
	/* Return function status */
	return HAL_OK;
}
/**
  * @brief  Initialize the WDT according to the specified.
  *         parameters in the WDT_InitTypeDef of  associated handle.
  * @param  hwdt  pointer to a WDT_HandleTypeDef structure that contains
  *                the configuration information for the specified WDT module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_WDT_Init(WDT_HandleTypeDef *hwdt)
{
	/* Check the parameters */
	assert_param(IS_WDT_ALL_INSTANCE(hwdt->Instance));
	assert_param(IS_WDT_RMOD(hwdt->Init.ResetMode));
	assert_param(IS_WDT_PULSE_LEN(hwdt->Init.ResetPulseLen));
	assert_param(IS_WDT_COUNTER(hwdt->Init.Counter));

#if (USE_HAL_WDT_REGISTER_CALLBACKS == 1)
	/* Reset Callback pointers */
	if(hwdt->IRQCallback == NULL)
	{
		hwdt->IRQCallback = HAL_WDT_IRQCallback;
	}
	if(hwdt->MspInitCallback == NULL)
	{
		hwdt->MspInitCallback = HAL_WDT_MspInit;
	}
	if(hwdt->MspDeInitCallback == NULL)
	{
		hwdt->MspDeInitCallback = HAL_WDT_MspDeInit;
	}
	/* Init the low level hardware */
	hwdt->MspInitCallback(hwdt);
#else
	/* Init the low level hardware */
	HAL_WDT_MspInit(hwdt);
#endif
	uint32_t tempreg = 0;
	
	tempreg = hwdt->Instance->CR.reg_u32val;
	((union _WDT_CR *)&tempreg)->bit.RPL = hwdt->Init.ResetPulseLen;
	((union _WDT_CR *)&tempreg)->bit.RMOD = hwdt->Init.ResetMode;
	hwdt->Instance->CR.reg_u32val = tempreg;
	
	hwdt->Instance->TORR.reg_u32val = 0;
	tempreg = 0;
	((union _WDT_TORR *)&tempreg)->bit.TOP_INIT = hwdt->Init.Counter;
	((union _WDT_TORR *)&tempreg)->bit.TOP = hwdt->Init.Counter;
	hwdt->Instance->TORR.reg_u32val = tempreg;
	
	/* Return function status */
	return HAL_OK;
}
/**
  * @brief  Set WatchDogTimer Timeout Period
  * @param  hwdt  pointer to a WDT_HandleTypeDef structure that contains
  *                the configuration information for the specified WDT module.
  * @param  timeout: timeout period
  * @retval None
  */
void HAL_WDT_SetTimeoutPeriod(WDT_HandleTypeDef *hwdt, WDT_Timeout_TypeDef period)
{
    assert_param(IS_WDT_ALL_INSTANCE(hwdt->Instance));
    assert_param(IS_WDT_TIMEOUT_PERIOD(period));

	uint32_t tempreg = hwdt->Instance->TORR.reg_u32val;
	((union _WDT_TORR *)&tempreg)->bit.TOP = period;
	hwdt->Instance->TORR.reg_u32val = tempreg;
}

HAL_StatusTypeDef HAL_WDT_Refresh(WDT_HandleTypeDef *hwdt)
{
	/* Check the parameters */
	assert_param(IS_WDT_ALL_INSTANCE(hwdt->Instance));
	/* Write to WDT CRR the WDT Counter value to refresh with */
	WRITE_REG(hwdt->Instance->CRR, WDT_RESET_CMD);
	/* Return function status */
	return HAL_OK;
}
void HAL_WDT_IRQHandler(WDT_HandleTypeDef *hwdt)
{
	__HAL_WDT_IT_CLEAR(hwdt);
	HAL_WDT_Refresh(hwdt);
	if(hwdt->IRQCallback)
	{
		hwdt->IRQCallback(hwdt);
	}
}
__WEAK void HAL_WDT_IRQCallback(WDT_HandleTypeDef *hwdt)
{
	UNUSED(hwdt);
}
__WEAK void HAL_WDT_MspInit(WDT_HandleTypeDef *hwdt)
{
	UNUSED(hwdt);
}
__WEAK void HAL_WDT_MspDeInit(WDT_HandleTypeDef *hwdt)
{
	UNUSED(hwdt);
}

HAL_StatusTypeDef HAL_WDT_RegisterCallback(WDT_HandleTypeDef *hwdt, HAL_WDT_CallbackIDTypeDef CallbackID, pWDT_CallbackTypeDef pCallback)
{
	/* Check the WDT handle allocation */
	if (hwdt == NULL)
	{
		return HAL_ERROR;
	}
	HAL_StatusTypeDef status = HAL_OK;
	if (pCallback == NULL)
	{
		return HAL_ERROR;
  	}
    switch (CallbackID)
    {
	case HAL_WDT_IRQ_CB_ID :
		hwdt->IRQCallback = pCallback;
		break;
	case HAL_WDT_MSPINIT_CB_ID :
		hwdt->MspInitCallback = pCallback;
		break;
	case HAL_WDT_MSPDEINIT_CB_ID :
		hwdt->MspDeInitCallback = pCallback;
		break;
	default : /* Return error status */
        status =  HAL_ERROR;
        break;
    }
  	return status;
}
HAL_StatusTypeDef HAL_WDT_UnRegisterCallback(WDT_HandleTypeDef *hwdt, HAL_WDT_CallbackIDTypeDef CallbackID)
{
	/* Check the WDT handle allocation */
	if (hwdt == NULL)
	{
		return HAL_ERROR;
	}
	HAL_StatusTypeDef status = HAL_OK;
    switch (CallbackID)
    {
	case HAL_WDT_IRQ_CB_ID :
		hwdt->IRQCallback = HAL_WDT_IRQCallback;
		break;
	case HAL_WDT_MSPINIT_CB_ID :
		hwdt->MspInitCallback = HAL_WDT_MspInit;
		break;
	case HAL_WDT_MSPDEINIT_CB_ID :
		hwdt->MspDeInitCallback = HAL_WDT_MspDeInit;
		break;
	default : /* Return error status */
        status =  HAL_ERROR;
        break;
    }
  	return status;
}
/**
  * @brief  Enable WDT
  * @param  hwdt  pointer to a WDT_HandleTypeDef structure that contains
  *                the configuration information for the specified WDT module.
  * @retval None
  */
void HAL_WDT_Enable(WDT_HandleTypeDef *hwdt)
{
	uint32_t tempreg = 0;
	/* Check the parameters */
	assert_param(IS_WDT_ALL_INSTANCE(hwdt->Instance));
	
	tempreg = hwdt->Instance->CR.reg_u32val;
	((union _WDT_CR *)&tempreg)->bit.WDT_EN = 1;
	hwdt->Instance->CR.reg_u32val = tempreg;
}
/**
  * @brief  Disable WDT
  * @param  hwdt  pointer to a WDT_HandleTypeDef structure that contains
  *                the configuration information for the specified WDT module.
  * @retval None
  */
void HAL_WDT_Disable(WDT_HandleTypeDef *hwdt)
{
	uint32_t tempreg = 0;
	/* Check the parameters */
	assert_param(IS_WDT_ALL_INSTANCE(hwdt->Instance));
	
	tempreg = hwdt->Instance->CR.reg_u32val;
	((union _WDT_CR *)&tempreg)->bit.WDT_EN = 0;
	hwdt->Instance->CR.reg_u32val = tempreg;
}

/**
  * @brief  Get WDT Current Counter Value
  * @param  hwdt  pointer to a WDT_HandleTypeDef structure that contains
  *                the configuration information for the specified WDT module.
  * @retval current counter value
  */
uint32_t HAL_WDT_GetCurrentCounterValue(WDT_HandleTypeDef *hwdt)
{
	/* Check the parameters */
	assert_param(IS_WDT_ALL_INSTANCE(hwdt->Instance));
	return hwdt->Instance->CCVR;
}

uint32_t HAL_WDT_GetTimeoutCounterValue(WDT_HandleTypeDef *hwdt)
{
    uint32_t tempreg = 0;
	/* Check the parameters */
	assert_param(IS_WDT_ALL_INSTANCE(hwdt->Instance));
    tempreg = hwdt->Instance->TORR.bit.TOP;
    switch(tempreg)
    {
        case 0: return 64000;
        case 1: return 128000;
        case 2: return 256000;
        case 3: return 512000;
        case 4: return 1000000;
        case 5: return 2000000;
        case 6: return 4000000;
        case 7: return 8000000;
        case 8: return 16000000;
        case 9: return 32000000;
        case 10: return 64000000;
        case 11: return 128000000;
        case 12: return 256000000;
        case 13: return 512000000;
        case 14: return 1000000000;
        case 15: return 2000000000;
        default: return 2000000000;
    }
    return 2000000000;
}
#endif /* HAL_WDT_MODULE_ENABLED */

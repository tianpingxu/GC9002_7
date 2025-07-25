/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-02-01     huyt        the first version
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx.h"
#include "core_rv32.h"
#include "template_def.h"
#include "csi_irq.h"

#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_WDT_

#define TEST_CASE	2//0-不喂狗   1-延时喂狗    2-中断喂狗


static WDT_HandleTypeDef hwdg = {.Instance = WDT};

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_WDT_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_WDT_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();
	
    HAL_WDT_IRQHandler(&hwdg);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
int main(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA, GPIO_InitStruct.Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_InitStruct.Pin, GPIO_PIN_SET);
	
	HAL_WDT_StructInit(&hwdg);
#if(TEST_CASE == 0)
	hwdg.Init.ResetMode = WDT_RESET_MODE;
    hwdg.Init.Counter = WDT_Timeout_2M_CLOCKS;
	HAL_WDT_Init(&hwdg);
	HAL_WDT_Enable(&hwdg);
    HAL_WDT_SetTimeoutPeriod(&hwdg, WDT_Timeout_128M_CLOCKS);
    HAL_WDT_Refresh(&hwdg);
#elif(TEST_CASE == 1)
	HAL_WDT_Init(&hwdg);
	HAL_WDT_Enable(&hwdg);
    HAL_WDT_Refresh(&hwdg);
	while(1)
	{
		while(HAL_WDT_GetCurrentCounterValue(&hwdg) < 0x1F)
		{
			HAL_WDT_Refresh(&hwdg);
		}
	}
#elif(TEST_CASE == 2)
	hwdg.Init.ResetMode = WDT_INTERRUPT_MODE;
    hwdg.Init.Counter = WDT_Timeout_2M_CLOCKS;
	HAL_WDT_Init(&hwdg);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(WDT_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, TEMPLATE_WDT_IRQHandler);
#else
    csi_clic_register_irq(WDT_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, TEMPLATE_WDT_IRQHandler);
#endif
	HAL_WDT_Enable(&hwdg);
    HAL_WDT_Refresh(&hwdg);
#endif
    while (1)
    {
		HAL_GPIO_TogglePin(GPIOA, GPIO_InitStruct.Pin);
		HAL_Delay(100);
    }
    return 0;
}


#endif/* _TEMPLATE_WDT_ */
#endif/* BSP_DEBUG */

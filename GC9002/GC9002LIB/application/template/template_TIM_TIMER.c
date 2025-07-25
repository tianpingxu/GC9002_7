/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-13     huyt        the first version
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx.h"
#include "core_rv32.h"
#include "template_def.h"
#include "csi_irq.h"
#include "csi_irq_vectors.h"

#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_TIM_TIMER_

#define TEST_TIMx               TIM0
#define TEST_TIMx_CHANNEL       TIM_CHANNEL_2
#define TEST_TIMx_IRQn          TIM0_IRQn
#define TEST_TIMx_IRQHandler    TEMPLATE_TIM0_IRQHandler

#define TEST_GPIO               GPIOB
#define TEST_GPIO_PIN           GPIO_PIN_17

static TIM_HandleTypeDef htim = {.Instance = TEST_TIMx};

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_TIM0_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_TIM0_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    HAL_TIM_IRQHandler(&htim);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_TIM1_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_TIM1_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    HAL_TIM_IRQHandler(&htim);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM0 || htim->Instance == TIM1)
    {
        HAL_GPIO_TogglePin(TEST_GPIO, TEST_GPIO_PIN);   
    }
}
int main(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = TEST_GPIO_PIN;
	HAL_GPIO_Init(TEST_GPIO, &GPIO_InitStruct);
	HAL_GPIO_WritePin(TEST_GPIO, TEST_GPIO_PIN, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(TEST_GPIO, TEST_GPIO_PIN, GPIO_PIN_SET);
	
	htim.Instance = TEST_TIMx;
	htim.Init.Prescaler = HAL_RCC_GetAPBClock() - 1;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = 1000 - 1;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim.Init.RepetitionCounter = 0xFFFE;
	htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	
	HAL_TIM_Base_Init(&htim);
	
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(TEST_TIMx_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEST_TIMx_IRQHandler);
#else
    csi_clic_register_irq(TEST_TIMx_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEST_TIMx_IRQHandler);
#endif
	HAL_TIM_Base_Start_IT(&htim);
	
    while (1)
    {
		HAL_Delay(100);
    }
    return 0;
}


#endif/* _TEMPLATE_TIM_TIMER_ */
#endif/* BSP_DEBUG */

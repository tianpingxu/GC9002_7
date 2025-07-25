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

#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_TIM_PWM_

#define TEST_TIMx               TIM0
#define TEST_TIMx_CHANNEL       TIM_CHANNEL_2
#define TEST_TIMx_IRQn          TIM0_IRQn
#define TEST_TIMx_IRQHandler    TEMPLATE_TIM0_IRQHandler

static TIM_HandleTypeDef handle_tim = {.Instance = TEST_TIMx};

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

    HAL_TIM_IRQHandler(&handle_tim);

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

    HAL_TIM_IRQHandler(&handle_tim);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    
}
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* Peripheral clock enable
    PB13     ------> TIM0_2 -AG25	    VIDEO_D3
    PB17     ------> TIM1_2 -AK24       VIDEO_D7
    */
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
	GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF3;
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
	GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF2;
	GPIO_InitStruct.Pin = GPIO_PIN_17;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
static void MX_TIMx_Init(void);
int main(void)
{
    MX_TIMx_Init();
    HAL_TIM_PWM_Start(&handle_tim, TEST_TIMx_CHANNEL);
    while (1)
    {
		HAL_Delay(100);
    }
    return 0;
}

/* TEST_TIMx init function */
static void MX_TIMx_Init(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    handle_tim.Instance = TEST_TIMx;
    handle_tim.Init.Prescaler = HAL_RCC_GetAPBClock()/1000 - 1;
    handle_tim.Init.CounterMode = TIM_COUNTERMODE_UP;
    handle_tim.Init.Period = 200;  //自动重装载值
    handle_tim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    handle_tim.Init.RepetitionCounter = 0;
    handle_tim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(&handle_tim) != HAL_OK)
    {
        while(1);
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&handle_tim, &sClockSourceConfig) != HAL_OK)
    {
        while(1);
    }
    if (HAL_TIM_PWM_Init(&handle_tim) != HAL_OK)
    {
        while(1);
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 100;      //比较值
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH; //输出极性
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(&handle_tim, &sConfigOC, TEST_TIMx_CHANNEL) != HAL_OK)
    {
        while(1);
    }
}

#endif/* _TEMPLATE_TIM_PWM_ */
#endif/* BSP_DEBUG */

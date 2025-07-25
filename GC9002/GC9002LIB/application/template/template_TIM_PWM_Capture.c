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
#ifdef  _TEMPLATE_TIM_PWM_CAPTURE_

#define TEST_PWM_OUT_TIMx               TIM0
#define TEST_PWM_OUT_TIMx_CHANNEL       TIM_CHANNEL_2
#define TEST_PWM_OUT_TIMx_IRQn          TIM0_IRQn
#define TEST_PWM_OUT_TIMx_IRQHandler    TEMPLATE_TIM0_IRQHandler

#define TEST_ENCODER_TIMx               TIM1
#define TEST_ENCODER_TIMx_CHANNEL       TIM_CHANNEL_2
#define TEST_ENCODER_TIMx_IRQn          TIM1_IRQn
#define TEST_ENCODER_TIMx_IRQHandler    TEMPLATE_TIM1_IRQHandler

static TIM_HandleTypeDef handle_tim_out = {.Instance = TEST_PWM_OUT_TIMx};
static TIM_HandleTypeDef handle_tim_in = {.Instance = TEST_ENCODER_TIMx};

static GPIO_InitTypeDef s_GPIO_InitStruct = {0};
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

    HAL_TIM_IRQHandler(&handle_tim_out);

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

    HAL_TIM_IRQHandler(&handle_tim_in);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//	HAL_GPIO_TogglePin(GPIOB, s_GPIO_InitStruct.Pin);
}

/* Captured Values */
uint32_t               uwIC2Value1 = 0;
uint32_t               uwIC2Value2 = 0;
uint32_t               uwDiffCapture = 0;
/* Capture index */
uint16_t               uhCaptureIndex = 0;
/* Frequency Value */
uint32_t               uwFrequency = 0;
/**
*  @brief 定时器中断回调函数
*  @note  htim 定时器句柄
*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
        if(uhCaptureIndex == 0)
        {
            /* Get the 1st Input Capture value */
            uwIC2Value1 = HAL_TIM_ReadCapturedValue(htim, TEST_ENCODER_TIMx_CHANNEL);
            uhCaptureIndex = 1;
        }
        else if(uhCaptureIndex == 1)
        {
            /* Get the 2nd Input Capture value */
            uwIC2Value2 = HAL_TIM_ReadCapturedValue(htim, TEST_ENCODER_TIMx_CHANNEL); 

            /* Capture computation */
            if (uwIC2Value2 > uwIC2Value1)
            {
                uwDiffCapture = (uwIC2Value2 - uwIC2Value1); 
            }
            else  /* (uwIC2Value2 <= uwIC2Value1) */
            {
                uwDiffCapture = ((0xFFFF - uwIC2Value1) + uwIC2Value2); 
            }
            /* Frequency computation: for this example TIMx (TIM0/1) is clocked by 2*APBclk */      
            uwFrequency = ((HAL_RCC_GetAPBClock()/(htim->Init.Prescaler+1)) / uwDiffCapture);
            uhCaptureIndex = 0;
        } 
    }
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
static void TEST_PWM_OUT_TIMx_Init(void);
static void TEST_Encoder_TIMx_Init(void);
int main(void)
{
    TEST_PWM_OUT_TIMx_Init();
    TEST_Encoder_TIMx_Init();
    
    while (1)
    {
		HAL_Delay(100);
    }
    return 0;
}

/* TEST_PWM_OUT_TIMx init function */
static void TEST_PWM_OUT_TIMx_Init(void)
{
    TIM_OC_InitTypeDef sConfigOC = {0};

    /*
     * 频率(CK_CNT)   = 48MHz/(4799+1)    = 10000 Hz
     * 周期(CK_CNT)   = 1/10000 Hz        = 0.1 ms
     * 周期(计数值200)= 0.1 ms * 200      = 20 ms
     * 频率(pwm)      = 1/0.02 s          = 50 Hz
     * */
    handle_tim_out.Instance = TEST_PWM_OUT_TIMx;
    handle_tim_out.Init.Prescaler = HAL_RCC_GetAPBClock()/10000 - 1;
    handle_tim_out.Init.CounterMode = TIM_COUNTERMODE_UP;
    handle_tim_out.Init.Period = 200;  //自动重装载值
    handle_tim_out.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    handle_tim_out.Init.RepetitionCounter = 0;
    handle_tim_out.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(&handle_tim_out) != HAL_OK)
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
    if (HAL_TIM_PWM_ConfigChannel(&handle_tim_out, &sConfigOC, TEST_PWM_OUT_TIMx_CHANNEL) != HAL_OK)
    {
        while(1);
    }
    HAL_TIM_PWM_Start(&handle_tim_out, TEST_PWM_OUT_TIMx_CHANNEL);
}
/* TEST_ENCODER_TIMx init function */
static void TEST_Encoder_TIMx_Init(void)
{
    TIM_IC_InitTypeDef sConfigIC = {0};

    /*
     * 频率(CK_CNT)   = 48MHz/(4799+1)    = 100000 Hz
     * 周期(CK_CNT)   = 1/100000 Hz       = 0.01 ms    =0.00001 s
     * 周期(计数值x)  = 0.01 ms * x       = 0.01*x ms
     * 频率(pwm)      = 1/(0.00001*x) s   = 100000/x Hz
     * */
    handle_tim_in.Instance = TEST_ENCODER_TIMx;
    handle_tim_in.Init.Prescaler = HAL_RCC_GetAPBClock()/100000 - 1;
    handle_tim_in.Init.CounterMode = TIM_COUNTERMODE_UP;
    handle_tim_in.Init.Period = 0xFFFF;  //自动重装载值
    handle_tim_in.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    handle_tim_in.Init.RepetitionCounter = 0;
    handle_tim_in.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_IC_Init(&handle_tim_in) != HAL_OK)
    {
        while(1);
    }
    sConfigIC.ICPolarity = TIM_ICPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0x0; //滤波设置, 经几个周期跳变认定波形稳定
    if (HAL_TIM_IC_ConfigChannel(&handle_tim_in, &sConfigIC, TEST_ENCODER_TIMx_CHANNEL) != HAL_OK)
    {
        while(1);
    }
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(TEST_ENCODER_TIMx_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEST_ENCODER_TIMx_IRQHandler);
#else
    csi_clic_register_irq(TEST_ENCODER_TIMx_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEST_ENCODER_TIMx_IRQHandler);
#endif
    if(HAL_TIM_IC_Start_IT(&handle_tim_in, TEST_ENCODER_TIMx_CHANNEL) != HAL_OK)
    {
        while(1);
    }
}

#endif/* _TEMPLATE_TIM_PWM_CAPTURE_ */
#endif/* BSP_DEBUG */

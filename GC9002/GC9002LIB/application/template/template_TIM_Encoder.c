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
#ifdef  _TEMPLATE_TIM_ENCODER_

#define TEST_PWM_OUT_TIMx               TIM0
#define TEST_PWM_OUT_A_TIMx_CHANNEL     TIM_CHANNEL_1
#define TEST_PWM_OUT_B_TIMx_CHANNEL     TIM_CHANNEL_2
#define TEST_PWM_OUT_TIMx_IRQn          TIM0_IRQn
#define TEST_PWM_OUT_TIMx_IRQHandler    TEMPLATE_TIM0_IRQHandler
 
/* 0x01 - x2模式 A向
 * 0x10 - x2模式 B向
 * 0x11 - x4模式 AB向
 * */
#define TEST_ENCODER_FLAG               0x11

#define TEST_ENCODER_TIMx               TIM0
#define TEST_ENCODER_A_TIMx_CHANNEL     TIM_CHANNEL_1
#define TEST_ENCODER_B_TIMx_CHANNEL     TIM_CHANNEL_2
#define TEST_ENCODER_TIMx_IRQn          TIM0_IRQn
#define TEST_ENCODER_TIMx_IRQHandler    TEMPLATE_TIM0_IRQHandler


#define AUTO_RELOAD_VALUE   0x7FFF

#define TEST_TIM            0

static TIM_HandleTypeDef handle_tim_out = {.Instance = TEST_PWM_OUT_TIMx};
static TIM_HandleTypeDef handle_tim_encoder = {.Instance = TEST_ENCODER_TIMx};

static uint32_t s_tim_cnt = 0;

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

    s_tim_cnt = handle_tim_encoder.Instance->CNT;
#if(TEST_TIM == 0)
    HAL_TIM_IRQHandler(&handle_tim_encoder);
#else
    HAL_TIM_IRQHandler(&handle_tim_out);
#endif

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

#if(TEST_TIM == 1)
    HAL_TIM_IRQHandler(&handle_tim_encoder);
#endif

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

static volatile int over_under_flowcount = 0;
static uint32_t count = 0;
/**
*  @brief 定时器中断回调函数
*  @note  htim 定时器句柄
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim))
    {
        over_under_flowcount--;
    }
    else
    {
        over_under_flowcount++;
    }
}
#if(TEST_TIM == 1)
#define TEST_OUT_A_GPIOx                GPIOB
#define TEST_OUT_A_GPIOx_PINx           GPIO_PIN_12
#define TEST_OUT_B_GPIOx                GPIOB
#define TEST_OUT_B_GPIOx_PINx           GPIO_PIN_13

#define TEST_ENCODER_GPIOx              GPIOB
#define TEST_ENCODER_GPIO_PINx          (GPIO_PIN_16 | GPIO_PIN_17)

#else

#define TEST_OUT_A_GPIOx                GPIOB
#define TEST_OUT_A_GPIOx_PINx           GPIO_PIN_16
#define TEST_OUT_B_GPIOx                GPIOB
#define TEST_OUT_B_GPIOx_PINx           GPIO_PIN_17

#define TEST_ENCODER_GPIOx              GPIOB
#define TEST_ENCODER_GPIO_PINx          (GPIO_PIN_12 | GPIO_PIN_13)
#endif


void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* Peripheral clock enable
    PB12     ------> TIM0_1 -K30	    EX2_IO6
    PB13     ------> TIM0_2 -AG25	    VIDEO_D3
    PB16     ------> TIM1_1 -AC24       VIDEO_D6
    PB17     ------> TIM1_2 -AK24       VIDEO_D7
    */
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
#if(TEST_TIM == 1)
	GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF2;
#else
	GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF3;
#endif
	GPIO_InitStruct.Pin = TEST_ENCODER_GPIO_PINx;
	HAL_GPIO_Init(TEST_ENCODER_GPIOx, &GPIO_InitStruct);
}
static void TEST_Encoder_TIMx_Init(void);
int main(void)
{
    TEST_Encoder_TIMx_Init();
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
	GPIO_InitStruct.Pin = TEST_OUT_A_GPIOx_PINx;
	HAL_GPIO_Init(TEST_OUT_A_GPIOx, &GPIO_InitStruct);
    
	GPIO_InitStruct.Pin = TEST_OUT_B_GPIOx_PINx;
	HAL_GPIO_Init(TEST_OUT_B_GPIOx, &GPIO_InitStruct);
    while (1)
    {
#if(0)
        HAL_GPIO_WritePin(TEST_OUT_A_GPIOx, TEST_OUT_A_GPIOx_PINx, GPIO_PIN_RESET);
        HAL_Delay(200);
        HAL_GPIO_WritePin(TEST_OUT_B_GPIOx, TEST_OUT_B_GPIOx_PINx, GPIO_PIN_RESET);
        HAL_Delay(200);
        HAL_GPIO_WritePin(TEST_OUT_A_GPIOx, TEST_OUT_A_GPIOx_PINx, GPIO_PIN_SET);
        HAL_Delay(200);
        HAL_GPIO_WritePin(TEST_OUT_B_GPIOx, TEST_OUT_B_GPIOx_PINx, GPIO_PIN_SET);
        HAL_Delay(200);
#else
        HAL_GPIO_WritePin(TEST_OUT_B_GPIOx, TEST_OUT_B_GPIOx_PINx, GPIO_PIN_RESET);
        HAL_Delay(200);
        HAL_GPIO_WritePin(TEST_OUT_A_GPIOx, TEST_OUT_A_GPIOx_PINx, GPIO_PIN_RESET);
        HAL_Delay(200);
        HAL_GPIO_WritePin(TEST_OUT_B_GPIOx, TEST_OUT_B_GPIOx_PINx, GPIO_PIN_SET);
        HAL_Delay(200);
        HAL_GPIO_WritePin(TEST_OUT_A_GPIOx, TEST_OUT_A_GPIOx_PINx, GPIO_PIN_SET);
        HAL_Delay(200);
#endif
        count = __HAL_TIM_GET_COUNTER(&handle_tim_encoder);
//        count = __HAL_TIM_GET_COUNTER(&handle_tim_encoder) + over_under_flowcount * AUTO_RELOAD_VALUE;
        printf("encoder cnt: %u\n", count);
    }
    return 0;
}

/* TEST_ENCODER_TIMx init function */
static void TEST_Encoder_TIMx_Init(void)
{
    TIM_IC_InitTypeDef sConfigIC = {0};
    TIM_Encoder_InitTypeDef sConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    
    /*
     * 频率(CK_CNT)   = 48MHz/(4799+1)    = 100000 Hz
     * 周期(CK_CNT)   = 1/100000 Hz       = 0.01 ms    =0.00001 s
     * */
    handle_tim_encoder.Instance = TEST_ENCODER_TIMx;
    handle_tim_encoder.Init.Prescaler = HAL_RCC_GetAPBClock()/100000 - 1;
    handle_tim_encoder.Init.CounterMode = TIM_COUNTERMODE_UP;// TIM_COUNTERMODE_UP;
    handle_tim_encoder.Init.Period = AUTO_RELOAD_VALUE;  //自动重装载值
    handle_tim_encoder.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    handle_tim_encoder.Init.RepetitionCounter = 0;
    handle_tim_encoder.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    
#if(defined TEST_ENCODER_FLAG && TEST_ENCODER_FLAG == 0x01)     //x2模式 - A向
    sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
#elif(defined TEST_ENCODER_FLAG && TEST_ENCODER_FLAG == 0x10)   //x2模式 - B向
    sConfig.EncoderMode = TIM_ENCODERMODE_TI2;
#else                                                           //x4模式 - A+B 向
    sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
#endif
    sConfig.IC1Polarity = TIM_ICPOLARITY_FALLING;//下降沿 向上计数   上升沿 向下计数
    sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC1Filter = 3;
    sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
    sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
    sConfig.IC2Filter = 3;
    if (HAL_TIM_Encoder_Init(&handle_tim_encoder, &sConfig) != HAL_OK)
    {
        while(1);
    }    
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(TEST_ENCODER_TIMx_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEST_ENCODER_TIMx_IRQHandler);
#else
    csi_clic_register_irq(TEST_ENCODER_TIMx_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEST_ENCODER_TIMx_IRQHandler);
#endif
    /* clear update flag */
    __HAL_TIM_CLEAR_FLAG(&handle_tim_encoder, TIM_FLAG_UPDATE);
    /* enable update request source */
    __HAL_TIM_URS_ENABLE(&handle_tim_encoder);
    /* Enable the TIM Update interrupt */
    __HAL_TIM_ENABLE_IT(&handle_tim_encoder, TIM_IT_UPDATE);
    HAL_TIM_Encoder_Start_IT(&handle_tim_encoder, TIM_CHANNEL_ALL);
}

#endif/* _TEMPLATE_TIM_ENCODER_ */
#endif/* BSP_DEBUG */

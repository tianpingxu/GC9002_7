/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     template_ADC.c
 * @brief    ADC test code
 * @version  V1.0
 * @date     8. Feb 2023
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx.h"
#include "core_rv32.h"
#include "template_def.h"
#include "csi_kernel.h"
#include "csi_irq.h"
#include "csi_irq_vectors.h"

#include "FreeRTOS.h" 
#include "task.h"
#include "timers.h"
#include "semphr.h"

//ADC_ON置0即失能ADC后，FIFOSTATUS寄存器值为0x0101_0100(初始状态值), ADC_ON置1后，FIFOSTATUS值变为0x0100_0000
//为防止ADC使用过程中导致的误判，初始化会使能ADC，解初始化才会失能ADC，中间过程不失能ADC

//若设置FIFOSIZE为N(N<=15)，则单次或者连续模式下采集了N个数据后（中间不读取结果），FIFOSTATUS寄存器的AFULL位将被置1
//若FIFO中当前有N个采集结果，则连续读取N次DOUT寄存器后（中间不再新增采集数据），FIFOSTATUS寄存器值将变为0x0100_0000

//单次采集无须调用STOP接口，只有在连续采集模式下才需要调用STOP接口

//若FIFO中有N个数据，则需要读取N次，DONE才会清零

#if((defined BSP_DEBUG) && (BSP_DEBUG == TEST_TEMPLE_BSP))
#ifdef _TEMPLATE_ADC_

//#define TEST_01  //single adc sample test
#define TEST_02  //continuous adc sample test
//#define TEST_03  //adc single adc sample interrupt test
//#define TEST_04  //adc continuous adc sample interrupt test
//#define TEST_05  //adc channel8 test
//#define TEST_06  //adc fifo test

#define TASK_STK_SIZE      512
#define TASK_PRIOR_TASK10  10
#define TASK_PRIOR_TASK20  11
#define TIME_QUANTA        100

k_task_handle_t g_uwTask10;

static ADC_HandleTypeDef handle_adc = {.Instance = ADC};

#if defined TEST_01

void task10(void *p)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin = GPIO_PIN_16;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    HAL_ADC_StructInit(&handle_adc);
    handle_adc.Init.Channel = ADC_CHANNEL_3;
    HAL_ADC_Init(&handle_adc);
    
    for(;;) {
        HAL_ADC_Start(&handle_adc);
        __HAL_ADC_WAIT_DONE(&handle_adc);
        
        uint16_t val = 0;
        HAL_ADC_GetValue(&handle_adc, &val, 1);
        UNUSED(val);

        csi_kernel_delay(pdMS_TO_TICKS(500));
    }
}

#elif defined TEST_02

void task10(void *p)
{
    uint32_t reg = 0;
    uint32_t cnt = 0;
    uint16_t val[10];

    HAL_ADC_StructInit(&handle_adc);
    handle_adc.Init.Mode = ADC_CONVERT_CONTINUE_MODE;
    handle_adc.Init.Channel = ADC_CHANNEL_8;
    handle_adc.Init.FullSize = 10;
    HAL_ADC_Init(&handle_adc);
    
    HAL_ADC_Start(&handle_adc);
    while(HAL_IS_BIT_SET(handle_adc.Instance->FIFOSTATUS, ADC_FIFOSTA_AFULL) == 0);

    for(;;) {
        if(HAL_IS_BIT_SET(handle_adc.Instance->FIFOSTATUS, ADC_FIFOSTA_AFULL))
        {
            HAL_ADC_Stop(&handle_adc);
            reg = handle_adc.Instance->FIFOSTATUS;
            
            HAL_ADC_GetValue(&handle_adc, val, 10);
            reg = handle_adc.Instance->FIFOSTATUS;
            
            csi_kernel_delay(pdMS_TO_TICKS(5000));
            HAL_ADC_Start(&handle_adc);
        }
    }
}

#elif defined TEST_03

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
void My_ADC_IRQHandler(void)
#else
ATTRIBUTE_ISR void ADC_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    HAL_ADC_IRQHandler(&handle_adc);
    
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

void ADC_ConvertComplete(ADC_HandleTypeDef *hadc)
{
    if(READ_BIT(handle_adc.Instance->DONE, ADC_DONE))
    {
        uint16_t val = 0;
        HAL_ADC_GetValue(hadc, &val, 1);
        UNUSED(val);
    }
}

void task10(void *p)
{
    HAL_ADC_StructInit(&handle_adc);
    handle_adc.Init.Channel = ADC_CHANNEL_14;
    HAL_ADC_Init(&handle_adc);
    
    HAL_ADC_RegisterCallback(&handle_adc, HAL_ADC_CONVERSION_COMPLETE_CB_ID, ADC_ConvertComplete);
    
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(ADC_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, My_ADC_IRQHandler);
#else
    csi_clic_register_irq(ADC_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, ADC_IRQHandler);
#endif
    
    for(;;) {
        HAL_ADC_Start_IT(&handle_adc);

        csi_kernel_delay(pdMS_TO_TICKS(500));
    }
}

#elif defined TEST_04

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
void My_ADC_IRQHandler(void)
#else
ATTRIBUTE_ISR void ADC_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    HAL_ADC_IRQHandler(&handle_adc);
    
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

void ADC_ConvertComplete(ADC_HandleTypeDef *hadc)
{
    static uint32_t reg = 0;
    uint16_t val[5] = {0, 0, 0, 0, 0};
    if(HAL_IS_BIT_SET(handle_adc.Instance->FIFOSTATUS, ADC_FIFOSTA_AFULL))
    {
        HAL_ADC_Stop_IT(&handle_adc);
        reg = handle_adc.Instance->FIFOSTATUS;
        HAL_ADC_GetValue(hadc, val, 5);
        reg = handle_adc.Instance->FIFOSTATUS;
    }
}

void task10(void *p)
{
    uint32_t reg = 0;
    uint32_t cnt = 0;
    
    HAL_ADC_StructInit(&handle_adc);
    handle_adc.Init.Mode = ADC_CONVERT_CONTINUE_MODE;
    handle_adc.Init.Channel = ADC_CHANNEL_14;
    handle_adc.Init.FullSize = 5;
    HAL_ADC_Init(&handle_adc);
    
    HAL_ADC_RegisterCallback(&handle_adc, HAL_ADC_CONVERSION_COMPLETE_CB_ID, ADC_ConvertComplete);
    
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(ADC_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, My_ADC_IRQHandler);
#else
    csi_clic_register_irq(ADC_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, ADC_IRQHandler);
    __enable_excp_irq();
#endif

    reg = handle_adc.Instance->FIFOSTATUS;
    HAL_ADC_Start_IT(&handle_adc);
    
    for(;;) {
        csi_kernel_delay(pdMS_TO_TICKS(5000));
        uint32_t reg = handle_adc.Instance->FIFOSTATUS;
        HAL_ADC_Start_IT(&handle_adc);
    }
}

#elif defined TEST_05

void task10(void *p)
{
    HAL_ADC_StructInit(&handle_adc);
    handle_adc.Init.Channel = ADC_CHANNEL_8;
    HAL_ADC_Init(&handle_adc);
    
    for(;;) {
        HAL_ADC_Start(&handle_adc);
        __HAL_ADC_WAIT_DONE(&handle_adc);
        
        uint16_t val = 0;
        HAL_ADC_GetValue(&handle_adc, &val, 1);
        UNUSED(val);

        csi_kernel_delay(pdMS_TO_TICKS(500));
    }
}

#elif defined TEST_06

void task10(void *p)
{
    uint32_t reg = 0;
    uint16_t val[5] = {0, 0, 0, 0, 0};
    
    HAL_ADC_StructInit(&handle_adc);
    handle_adc.Init.Channel = ADC_CHANNEL_14;
    handle_adc.Init.FullSize = 3;
    HAL_ADC_Init(&handle_adc);
    
    for(;;) {
        for(int i=0; i<5; i++)
        {
            HAL_ADC_Start(&handle_adc);
            __HAL_ADC_WAIT_DONE(&handle_adc);
            reg = handle_adc.Instance->FIFOSTATUS;
        }

#if 0
        //通过单次读取可观察到FiFOSTATUS寄存器的变化
        for(int i=0; i<5; i++)
        {
            HAL_ADC_GetValue(&handle_adc, &val[i], 1);
            reg = handle_adc.Instance->FIFOSTATUS;
        }
#else
        //一次读取完毕，结果与单次读取效果一样
        HAL_ADC_GetValue(&handle_adc, val, 5);
        reg = handle_adc.Instance->FIFOSTATUS;
#endif

        csi_kernel_delay(pdMS_TO_TICKS(500));
    }
}

#else

#endif

int main(void)
{
    printf("start to execute main fun. \n");
    csi_kernel_init();
    
    csi_kernel_task_new((k_task_entry_t)task10, "task10", NULL, TASK_PRIOR_TASK10, TIME_QUANTA, NULL, TASK_STK_SIZE, &g_uwTask10);
    if (g_uwTask10 == NULL) {
        csi_kernel_sched_resume(0);
        printf("Fail to create task10! \r\n");
    }
    
    csi_kernel_start();
    
    while(1)
    {
        printf("Hello FreeRTOS.\n");
        HAL_Delay(100*1000);
    }
    return 0;
}

#endif 
#endif
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
#ifdef _TEMPLATE_ADC_DMA_

#define TEST_TASK_STK_SIZE      512
#define TEST_TASK_PRIOR         10
#define TEST_TIME_QUANTA        100

static k_task_handle_t g_uwTask_test;
static k_task_handle_t g_uwTask_idle;

static ADC_HandleTypeDef handle_adc;
static DMA_HandleTypeDef adc_hdma;

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_DMA_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_DMA_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

	HAL_DMA_IRQHandler(handle_adc.hdma);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
static void DMA_Init_Test(void)
{
    adc_hdma.Instance = DMA;
    HAL_DMA_StructInit(&(adc_hdma.Init));
	__HAL_LINKDMA(&handle_adc, hdma, adc_hdma);
    adc_hdma.Init.mChannel = DMA_CHANNEL1;
    adc_hdma.Init.mS_Address = ((uint32_t)&(handle_adc.Instance->DOUT));/*!< Specifies Source Address.             */
    adc_hdma.Init.mD_Address = 0;/*!< Specifies Destination Address.        */
    adc_hdma.Init.mBlockSize = 0;
    adc_hdma.Init.mS_AutoReload_EN = DMA_DISABLE;
    adc_hdma.Init.mD_AutoReload_EN = DMA_DISABLE;
    adc_hdma.Init.mS_TransferWidth = DMA_TR_WIDTH_16bits;
    adc_hdma.Init.mD_TransferWidth = DMA_TR_WIDTH_16bits;//16
    adc_hdma.Init.mS_BurstTransactionLen = DMA_DATA_ITEM_1;
    adc_hdma.Init.mD_BurstTransactionLen = DMA_DATA_ITEM_1;
    adc_hdma.Init.mS_AddrInc = DMA_FIXED;
    adc_hdma.Init.mD_AddrInc = DMA_INCREMENTAL;
    adc_hdma.Init.mChannelPriority = DMA_MIN_CHANNEL_PRIORITY;
    adc_hdma.Init.mTransferType_FlowControl = DMA_TT_PER_TO_MEM_DMAC;
    adc_hdma.Init.mS_Peripheral = DMA_HS_ADC;
    adc_hdma.Init.mD_Peripheral = DMA_HS_MEMORY;
    adc_hdma.Init.mEnable = DMA_DISABLE;
    HAL_DMA_Init(&adc_hdma);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(DMA_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEMPLATE_DMA_IRQHandler);
#else
    csi_clic_register_irq(DMA_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEMPLATE_DMA_IRQHandler);
#endif
}
#define TEST_ADC_BUFFER_SIZE        32
static uint16_t s_val[TEST_ADC_BUFFER_SIZE];


void TEST_HAL_ADC_ConvertComplete(ADC_HandleTypeDef *hadc)
{
    for (int i = 0; i < TEST_ADC_BUFFER_SIZE; i++)
    {
        // printf("s_val[%d]: %d\n", i, s_val[i]);
        printf("s_val[%d]: %d\n", i, HAL_ADC_ValueToCelsius(s_val[i]));
        s_val[i] = 0;
    }
    printf("\n");
}
void TEST_HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
    printf("ADC DMA Get Value error.\n");
}
void task_test(void *p)
{
    uint32_t reg = 0;
    uint32_t cnt = 0;
#if(1)
    handle_adc.Instance = ADC;
    HAL_ADC_StructInit(&handle_adc);
    handle_adc.Init.ClockDiv = 0x2;
    handle_adc.Init.Mode = ADC_CONVERT_CONTINUE_MODE;
    handle_adc.Init.Channel = ADC_CHANNEL_TS;
    handle_adc.Init.FullSize = 0xF;
    HAL_ADC_Init(&handle_adc);
    HAL_ADC_RegisterCallback(&handle_adc, HAL_ADC_CONVERSION_COMPLETE_CB_ID, TEST_HAL_ADC_ConvertComplete);
    HAL_ADC_RegisterCallback(&handle_adc, HAL_ADC_ERROR_CB_ID, TEST_HAL_ADC_ErrorCallback);
    DMA_Init_Test();
    HAL_ADC_Start_DMA(&handle_adc, s_val, sizeof(s_val));
#endif
    for(;;) {
        printf("task_adc_dma...\n");
        csi_kernel_delay(pdMS_TO_TICKS(3000));
        HAL_ADC_Start_DMA(&handle_adc, s_val, sizeof(s_val));
    }
}
void task_idle(void *p)
{
    for(;;) {
        printf("task_idle...\n");
        csi_kernel_delay(pdMS_TO_TICKS(3000));
    }
}
int main(void)
{
    printf("start to execute main fun. \n");
    csi_kernel_init();
    
    csi_kernel_task_new((k_task_entry_t)task_test, "task_test", NULL, TEST_TASK_PRIOR, TEST_TIME_QUANTA, NULL, TEST_TASK_STK_SIZE, &g_uwTask_test);
    if (g_uwTask_test == NULL) {
        csi_kernel_sched_resume(0);
        printf("Fail to create task_test! \r\n");
    }
    csi_kernel_task_new((k_task_entry_t)task_idle, "task_idle", NULL, TEST_TASK_PRIOR, TEST_TIME_QUANTA, NULL, TEST_TASK_STK_SIZE, &g_uwTask_idle);
    if (g_uwTask_idle == NULL) {
        csi_kernel_sched_resume(0);
        printf("Fail to create task_idle! \r\n");
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
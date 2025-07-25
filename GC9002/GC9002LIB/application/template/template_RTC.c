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
#include "csi_config.h"
#include "gc90xx.h"
#include "core_rv32.h"
#include "template_def.h"
#include "csi_irq.h"
#include "csi_irq_vectors.h"

#ifndef CONFIG_KERNEL_NONE
#include "csi_kernel.h"
#include "FreeRTOS.h" 
#include "task.h"
#endif

#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_RTC_

static RTC_HandleTypeDef handle_rtc = {.Instance = RTC};

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_RTC_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_RTC_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();
    
	__NOP();

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
int main(void)
{	
    RTC_TimeTypeDef time;
    HAL_RTC_Struct_Init(&handle_rtc);
    HAL_RTC_Init(&handle_rtc);
    
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(RTC_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, TEMPLATE_RTC_IRQHandler);
#else
    csi_clic_register_irq(RTC_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, TEMPLATE_RTC_IRQHandler);
#endif
    // __HAL_RTC_ENABLE_IT(&handle_rtc, RTC_IT_ALL);
    while(1)
    {
        HAL_Delay(1000);
        HAL_RTC_GetTime(&handle_rtc, &time, RTC_FORMAT_BCD);
        printf("SetTime:0x%8x\n", handle_rtc.Init.time.value);
        printf("NowTime:0x%8x\n", time.value);
        
        time.value = 0x00012525;
        HAL_RTC_SetTime(&handle_rtc, &time, RTC_FORMAT_BCD);
        HAL_RTC_GetTime(&handle_rtc, &time, RTC_FORMAT_BCD);
        printf("SetTime:0x%8x\n", handle_rtc.Init.time.value);
        printf("NowTime:0x%8x\n", time.value);
    }
    return 0;
}


#endif/* _TEMPLATE_RTC_ */
#endif/* BSP_DEBUG */

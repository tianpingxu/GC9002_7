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
#ifdef  _TEMPLATE_RTC_ALARM_

static RTC_HandleTypeDef handle_rtc = {.Instance = RTC};


static RTC_TimeTypeDef time;
static RTC_AlarmTypeDef sAlarm;

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

	HAL_RTC_AlarmIRQHandler(&handle_rtc);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

void TETS_HAL_RTC_AlarmEventCallback(RTC_HandleTypeDef *hrtc)
{
    HAL_RTC_GetTime(&handle_rtc, &time, RTC_FORMAT_BCD);
    printf("NowTime:0x%8x\n", time.value);
}
int main(void)
{
    printf("Hello World\n");
    HAL_RTC_Struct_Init(&handle_rtc);
    
    handle_rtc.Init.date.value = 0x20001230;
    handle_rtc.Init.time.value = 0x00125958;
    handle_rtc.Init.format = RTC_FORMAT_BCD;
    HAL_RTC_Init(&handle_rtc);
    HAL_RTC_RegisterCallback(&handle_rtc, HAL_RTC_ALARM_EVENT_CB_ID, TETS_HAL_RTC_AlarmEventCallback);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(RTC_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 0, 3, TEMPLATE_RTC_IRQHandler);
#else
    csi_clic_register_irq(RTC_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 0, 3, TEMPLATE_RTC_IRQHandler);
#endif
	
    while(1)
    {
        HAL_RTC_GetTime(&handle_rtc, &sAlarm.time, RTC_FORMAT_BCD);
        
        sAlarm.time.value = 0x00110158;
        HAL_RTC_SetTime(&handle_rtc, &sAlarm.time, RTC_FORMAT_BCD);
        
        sAlarm.time.value = 0x00110202;
        sAlarm.AlarmMask = RTC_ALARMMASK_ALL;//RTC_ALARMMASK_HOURS | RTC_ALARMMASK_MINUTES | RTC_ALARMMASK_SECONDS;
        HAL_RTC_SetAlarm(&handle_rtc, &sAlarm, RTC_FORMAT_BCD);
        while(1)
        {
            HAL_Delay(1000);
        }
    }
    return 0;
}

#endif/* _TEMPLATE_RTC_ALARM_ */
#endif/* BSP_DEBUG */

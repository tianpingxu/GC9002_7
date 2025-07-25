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
#include "main.h"
#include "gc90xx.h"
#include "core_rv32.h"
#include "template_def.h"
#include "csi_irq.h"
#include "csi_irq_vectors.h"

#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_TEST_

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
    
    __HAL_RTC_DISABLE(&handle_rtc);
	HAL_RTC_AlarmIRQHandler(&handle_rtc);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
static void TEST_HAL_RTC_AlarmEventCallback(RTC_HandleTypeDef *hrtc)
{
    HAL_RTC_GetTime(&handle_rtc, &time, RTC_FORMAT_BCD);
    printf("NowTime:0x%8x\n", time.value);
}
static void TEST01(void)
{
    uint32_t mstatus = __get_MSTATUS();
    __disable_irq();
    mstatus = __get_MSTATUS();
    __enable_irq();
    mstatus = __get_MSTATUS();

    uint32_t val = *(uint32_t *)0x08000000;
    printf("val = 0x%x\n", val);
    *(uint32_t *)0x08000000 = 0xFFFFFFFF;
    *(uint8_t *)0x08000000 = 0x12;
    val = *(uint32_t *)0x08000000;
    printf("val = 0x%x\n", val);
    while(1);
}
static void TEST02(void)
{
    HAL_RTC_Struct_Init(&handle_rtc);
    
    handle_rtc.Init.date.value = 0x20001230;
    handle_rtc.Init.time.value = 0x00125958;
    handle_rtc.Init.format = RTC_FORMAT_BCD;
    HAL_RTC_Init(&handle_rtc);
    HAL_RTC_RegisterCallback(&handle_rtc, HAL_RTC_ALARM_EVENT_CB_ID, TEST_HAL_RTC_AlarmEventCallback);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(RTC_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEMPLATE_RTC_IRQHandler);
#else
    csi_clic_register_irq(RTC_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEMPLATE_RTC_IRQHandler);
#endif
	
    while(1)
    {
        HAL_RTC_GetTime(&handle_rtc, &sAlarm.time, RTC_FORMAT_BCD);
        
        sAlarm.time.value = 0x00110158;
        HAL_RTC_SetTime(&handle_rtc, &sAlarm.time, RTC_FORMAT_BCD);
        
        sAlarm.time.value = 0x00110202;
        sAlarm.AlarmMask = RTC_ALARMMASK_HOURS | RTC_ALARMMASK_MINUTES | RTC_ALARMMASK_SECONDS;
        HAL_RTC_SetAlarm(&handle_rtc, &sAlarm, RTC_FORMAT_BCD);
        
        kernel_start_in_debug();
        while(1)
        {
            HAL_Delay(100);
        }
    }
}
#include "display.h"
DISPLAY_MasterParamType *param_ptr;
DISPLAY_MasterParamType param;
void TEST03(void)
{
    while (1)
    {
        uint16_t tempreg = HAL_UART_ReceiveData(CONFIG_CONSOLE_DEBUG_UART);
        switch(tempreg)
        {
            case 0x01://pop
            if(Display::DisplayPop(&param_ptr))
            {
                printf("pop 0x%x success\r\n", param_ptr->base_addr);
            }
            else
            {
                printf("pop failed\r\n");
            }
            break;
            case 0x02://push 0x02
            case 0x03://push 0x03
            case 0x04://push 0x04
            param.base_addr = tempreg;
            if(Display::DisplayPush(&param))
            {
                printf("push 0x%x success\r\n", tempreg);
            }
            else
            {
                printf("push 0x%x faileed\r\n", tempreg);
            }
            break;
            case 0x05://Get State 0x02
            param.base_addr = 0x02;
            if(Display::GetDisplayDone(&param))
            {
                printf("Display 0x02 Done\r\n");
            }
            else
            {
                printf("Display 0x02 Wait\r\n");
            }
            break;
            case 0x06://Get State 0x03
            param.base_addr = 0x03;
            if(Display::GetDisplayDone(&param))
            {
                printf("Display 0x03 Done\r\n");
            }
            else
            {
                printf("Display 0x03 Wait\r\n");
            }
            break;
            case 0x07://Get State 0x04
            param.base_addr = 0x04;
            if(Display::GetDisplayDone(&param))
            {
                printf("Display 0x04 Done\r\n");
            }
            else
            {
                printf("Display 0x04 Wait\r\n");
            }
            break;
            default: break;
        }
    }
}
int main(void)
{
    // TEST01();
    // TEST02();
    TEST03();
    while (1)
    {
		HAL_Delay(100);
    }
    return 0;
}

#endif/* _TEMPLATE_TEST_ */
#endif/* BSP_DEBUG */
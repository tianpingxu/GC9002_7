/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-08-04     huyt       the first version
 */
#include "csi_config.h"
#include "gc90xx_hal.h"
#include "system.h"

static HAL_TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT;  /* 1KHz */

/**
  * @brief This function provides minimum delay (in milliseconds) based
  *        on variable incremented.
  * @note In the default implementation , SysTick timer is the source of time base.
  *       It is used to generate interrupts at regular time intervals where uwTick
  *       is incremented.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @param Delay specifies the delay time length, in milliseconds.
  * @retval None
  */
__WEAK void HAL_Delay(uint32_t Delay)
{
    uint32_t tickstart = HAL_GetTick();
    uint32_t wait = Delay;

    /* Add a freq to guarantee minimum wait */
    if (wait < HAL_MAX_DELAY)
    {
        wait += (uint32_t)(uwTickFreq);
    }

    while((HAL_GetTick() - tickstart) < wait)
    {
        __NOP();
    }
}
extern volatile uint32_t g_sys_tick;
uint32_t HAL_GetTick(void)
{
	/* 计算 CoreTIM 走过的时钟数 */
	// return csi_coret_get_value_u64();
    return g_sys_tick;
}
#include <stdio.h>
void assert_failed(uint8_t* file, uint32_t line)
{
    printf("FILE:%s, LINE: %u\n", file, line);
    while(1);
}

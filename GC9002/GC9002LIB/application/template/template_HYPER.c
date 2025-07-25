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
#ifdef  _TEMPLATE_HYPER_

#define TEST_HYPER_ADDR     0x100
#define TEST_DATA_LEN       0x100

int main(void)
{
    uint8_t tx_buff[TEST_DATA_LEN];
    uint8_t rx_buff[TEST_DATA_LEN];
    HYPER_HandleTypeDef handle_hyper;
    for(int i = 0; i < TEST_DATA_LEN; i++)
    {
        tx_buff[i] = i+1;
        rx_buff[i] = 0;
    }
	handle_hyper.Instance = HYPER;
    handle_hyper.MapAHBAddrBase = HYPER_DATA_BASE;
    if(HAL_HYPER_Init(&handle_hyper) != HAL_OK)
    {
        printf("HAL_HYPER_Init error \n");
        while (1);
    }
    HAL_HYPER_Write(&handle_hyper, TEST_HYPER_ADDR, (uint32_t *)tx_buff, TEST_DATA_LEN);
    HAL_HYPER_Read(&handle_hyper, TEST_HYPER_ADDR, (uint32_t *)rx_buff, TEST_DATA_LEN);
    if(memcmp(tx_buff, rx_buff, TEST_DATA_LEN) != 0)
    {
        printf("memcmp error \n");
        while(1);
    }
    while (1)
    {
		HAL_Delay(1000);
    }
    return 0;
}

#endif/* _TEMPLATE_HYPER_ */
#endif/* BSP_DEBUG */

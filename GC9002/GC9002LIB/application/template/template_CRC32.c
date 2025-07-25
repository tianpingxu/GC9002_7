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
#ifdef  _TEMPLATE_CRC32_

#define CRC32_TEST_BUFF_LEN	512
int main(void)
{
    uint32_t result_bsp = 0;
    uint32_t result_soft_le = 0;
    uint8_t buff[CRC32_TEST_BUFF_LEN];
    for(int i = 0; i < CRC32_TEST_BUFF_LEN; i++)
    {
        buff[i] = i;
    }
    // BSP CRC32
    HAL_CRC32_Begin((uint32_t *)buff, CRC32_TEST_BUFF_LEN);
    HAL_CRC32_GetResult(&result_bsp, HAL_MAX_DELAY);
    printf("BSP CRC32 result_bsp = %u\n", result_bsp);
    // Soft CRC32
    result_soft_le = crc32_le(buff, CRC32_TEST_BUFF_LEN, 0xFFFFFFFF);
    printf("result_soft_le = %u\n", result_soft_le);
    while (1)
    {
        HAL_Delay(1000);
    }
    return 0;
}


#endif/* _TEMPLATE_CRC32_ */
#endif/* BSP_DEBUG */

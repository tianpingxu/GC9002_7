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
#ifdef  _TEMPLATE_I2S_RXTX_

#define BLOCK_START_ADDR         0     /* Block start address      */
#define NUM_OF_BLOCKS            8   /* Total number of blocks   */
#define NUM_OF_PAGES             12   /* Total number of blocks   */
#define NUM_OF_PAGES_USE         NUM_OF_BLOCKS  /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */

static I2S_HandleTypeDef hi2shandle = {.Instance = I2S};
//static uint8_t tx_buff[1024] =
//{
//    0xAA, 0x55, 0xF0, 0x11, 0x11, 0x1F, 0x55, 0xAA,
//    0xAA, 0x55, 0xF0, 0x11, 0x11, 0x1F, 0x55, 0xAA,
//    0xAA, 0x55, 0xF0, 0x11, 0x11, 0x1F, 0x55, 0xAA,
//    0xAA, 0x55, 0xF0, 0x11, 0x11, 0x1F, 0x55, 0xAA,
//    0xAA, 0x55, 0xF0, 0x11, 0x11, 0x1F, 0x55, 0xAA,
//    0xAA, 0x55, 0xF0, 0x11, 0x11, 0x1F, 0x55, 0xAA,
//    0xAA, 0x55, 0xF0, 0x11, 0x11, 0x1F, 0x55, 0xAA,
//    0xAA, 0x55, 0xF0, 0x11, 0x11, 0x1F, 0x55, 0xAA,
//};
static uint32_t tx_buff[1024];
int main(void)
{
    HAL_I2S_StructInit(&hi2shandle.Init);
	hi2shandle.Init.Mode = I2S_MODE_MASTER;
    HAL_I2S_Init(&hi2shandle);
    tx_buff[0] = 0xAAAAAAAA;
    tx_buff[255] = 0xBBBBBBBB;
    for (int32_t i = 1; i < 255; i++) {
        if(i%2)
            tx_buff[i] = 0x11223344;
        else
            tx_buff[i] = 0x55667788;
    }
	while(1)
	{        
		HAL_I2S_Transmit(&hi2shandle, (uint8_t *)tx_buff, sizeof(tx_buff), HAL_MAX_DELAY);
		HAL_Delay(500);
	}
    return 1;
}
#endif/* _TEMPLATE_I2S_RXTX_ */
#endif/* BSP_DEBUG */


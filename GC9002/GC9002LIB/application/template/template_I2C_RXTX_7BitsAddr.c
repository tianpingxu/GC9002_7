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
#ifdef  _TEMPLATE_I2C_RXTX_7BITSADDR_

static I2C_HandleTypeDef hi2c0 = {.Instance = I2C0};
static I2C_HandleTypeDef hi2c1 = {.Instance = I2C1};
static uint8_t TX_Buff[32] = {0x32, 0x33, 0x34, 0x35, 0x32, 0x33, 0x34, 0x35, 0x32, 0x33, 0x34, 0x35, 0x32, 0x33, 0x34, 0x35};
static uint8_t RX_Buff[32] = {0x32, 0x33, 0x34, 0x35, 0x32, 0x33, 0x34, 0x35, 0x32, 0x33, 0x34, 0x35, 0x32, 0x33, 0x34, 0x35};
#define TEST_MODE  0//0-Master TX/RX    1-Slave TX/RX
#if(TEST_MODE == 0)
#define Addr_Self  0x56
#define Addr_Dest  0x57
#else
#define Addr_Self  0x57
#define Addr_Dest  0x56
#endif

int main(void)
{
	I2C_HandleTypeDef *hi2c = &hi2c1;
    
    HAL_I2C_StructInit(hi2c);
    hi2c->Init.OwnAddress1 = Addr_Self;
    hi2c->Init.OwnAddress2 = Addr_Self;
    HAL_I2C_Init(hi2c);

    while(1)
    {
#if(TEST_MODE == 0)
        if(HAL_I2C_Master_Transmit(hi2c, Addr_Dest, TX_Buff, 16, 0xFFFFFFF) == HAL_OK)
        {
            printf("I2C Master TX buff:{%x, %x, %x, %x}\n", TX_Buff[0], TX_Buff[1], TX_Buff[2], TX_Buff[3]);
        }
        else
        {
            printf("I2C Master TX Failed\n");
        }
        HAL_Delay(1000);
        memset(RX_Buff, 0, sizeof(RX_Buff));
        if(HAL_I2C_Master_Receive(hi2c, Addr_Dest, RX_Buff, 16, 0xFFFFFFF) == HAL_OK)
        {
            printf("I2C Master RX buff:{%x, %x, %x, %x}\n", RX_Buff[0], RX_Buff[1], RX_Buff[2], RX_Buff[3]);
        }
        else
        {
            printf("I2C Master RX Failed\n");
        }
        HAL_Delay(1000);
#elif(TEST_MODE == 1)
        memset(RX_Buff, 0, sizeof(RX_Buff));
        if(HAL_I2C_Slave_Receive(hi2c, RX_Buff, 16, HAL_MAX_DELAY) == HAL_OK)
        {
            printf("I2C Slave RX buff:{%x, %x, %x, %x}\n", RX_Buff[0], RX_Buff[1], RX_Buff[2], RX_Buff[3]);
        }
        else
        {
            printf("I2C Slave RX Failed\n");
        }
        if(HAL_I2C_Slave_Transmit(hi2c, RX_Buff, 16, HAL_MAX_DELAY) == HAL_OK)
        {
            printf("I2C Slave TX buff:{%x, %x, %x, %x}\n", RX_Buff[0], RX_Buff[1], RX_Buff[2], RX_Buff[3]);
        }
        else
        {
            printf("I2C Slave TX Failed\n");
        }
//        HAL_Delay(1000);
#endif
    }
    return 0;
}

#endif/* _TEMPLATE_I2C_RXTX_7BITSADDR_ */
#endif/* BSP_DEBUG */

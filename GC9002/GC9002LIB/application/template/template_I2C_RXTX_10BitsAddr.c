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
#ifdef  _TEMPLATE_I2C_RXTX_10BITSADDR_

static I2C_HandleTypeDef hi2c0 = {.Instance = I2C0};
static I2C_HandleTypeDef hi2c1 = {.Instance = I2C1};

#define ADDR_MODE 10
#if(ADDR_MODE == 7)
#define Addr_Self_Slave  0x55
#define Addr_Self  0x56
#define Addr_Dest  0x57
#else
#define Addr_Self_Slave  0x355
#define Addr_Self  0x356
#define Addr_Dest  0x357
#endif
static uint8_t TX_Buff[32] = {0x32, 0x33, 0x34, 0x35};
#define TEST_MODE  1//0-Master TX    1-Master RX   2-Slave TX    3-Slave RX

int main(void)
{
	I2C_HandleTypeDef *hi2c = &hi2c0;
    HAL_I2C_StructInit(hi2c);
    hi2c->Init.OwnAddress1 = Addr_Self;
    hi2c->Init.OwnAddress2 = Addr_Self_Slave;
#if(ADDR_MODE == 10)
    hi2c->Init.AddressMode = I2C_AddrMode_10BIT;
#endif
    HAL_I2C_Init(hi2c);
    while(1)
    {
#if(TEST_MODE == 0)
        if(HAL_I2C_Master_Transmit(hi2c, Addr_Dest, TX_Buff, 4, 100000) == HAL_OK)
        {
            printf("I2C Master TX buff:{%x, %x, %x, %x}\n", TX_Buff[0], TX_Buff[1], TX_Buff[2], TX_Buff[3]);
        }
        else
        {
            printf("I2C Master TX Failed\n");
        }
		HAL_Delay(500);
#elif(TEST_MODE == 1)
        if(HAL_I2C_Master_Receive(hi2c, Addr_Dest, TX_Buff, 1, 1000000) == HAL_OK)
        {
            printf("I2C Master RX buff:{%x, %x, %x, %x}\n", TX_Buff[0], TX_Buff[1], TX_Buff[2], TX_Buff[3]);
        }
        else
        {
            printf("I2C Master RX Failed\n");
        }
		HAL_Delay(500);
#elif(TEST_MODE == 2)
        if(HAL_I2C_Slave_Transmit(hi2c, TX_Buff, 4, HAL_MAX_DELAY) == HAL_OK)
        {
            printf("I2C Slave TX buff:{%x, %x, %x, %x}\n", TX_Buff[0], TX_Buff[1], TX_Buff[2], TX_Buff[3]);
        }
        else
        {
            printf("I2C Slave TX Failed\n");
        }
#else
        if(HAL_I2C_Slave_Receive(hi2c, TX_Buff, 4, HAL_MAX_DELAY) == HAL_OK)
        {
            printf("I2C Slave RX buff:{%x, %x, %x, %x}\n", TX_Buff[0], TX_Buff[1], TX_Buff[2], TX_Buff[3]);
        }
        else
        {
            printf("I2C Slave RX Failed\n");
        }
#endif
        HAL_Delay(100);
    }
    return 0;
}

#endif/* _TEMPLATE_I2C_RXTX_10BITSADDR_ */
#endif/* BSP_DEBUG */

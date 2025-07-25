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
#ifdef  _TEMPLATE_I2C_MEM_

static I2C_HandleTypeDef hi2c0 = {.Instance = I2C0};

#define Addr_Self  0x56
#define Addr_Dest  0x51
static uint8_t TX_Buff[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
static uint8_t RX_Buff[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* AT24C01/02每页有8个字节 
 * AT24C04/08A/16每页有16个字节 
 * AT24C01/02/04/08/16是一个 1K/2K/4K/8K/16K 位串行 CMOS，内部含有 128/256/512/1024/2048 个 8 位字节
 * AT24C01 有一个 8 字节页写缓冲器， AT24C02/04/08/16 有一个 16 字节页写缓冲器
 * 注意：在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数不能超过EEPROM页的大小
 */
#define EEPROM_PAGESIZE           16			
int main(void)
{
	I2C_HandleTypeDef *hi2c = &hi2c0;
    
    HAL_I2C_StructInit(hi2c);
    hi2c->Init.OwnAddress1 = Addr_Self;
    hi2c->Init.OwnAddress2 = Addr_Self;
    HAL_I2C_Init(hi2c);

    while(1)
    {
        for (int i = 0; i < sizeof(TX_Buff); i++)
        {
            TX_Buff[i] = i+1;
        }
        
        if(HAL_I2C_Mem_Write(hi2c, Addr_Dest, 0x00, I2C_MEMADD_SIZE_8BIT, TX_Buff, 16, HAL_MAX_DELAY) != HAL_OK)
        {
            printf("HAL_I2C_Mem_Write error\n");
            while(1);
        }
        while(HAL_I2C_IsDeviceReady(hi2c, Addr_Dest, 300, HAL_MAX_DELAY) == HAL_TIMEOUT);
        if(HAL_I2C_Mem_Write(hi2c, Addr_Dest, 0x10, I2C_MEMADD_SIZE_8BIT, &TX_Buff[16], 16, HAL_MAX_DELAY) != HAL_OK)
        {
            printf("HAL_I2C_Mem_Write error\n");
            while(1);
        }
        while(HAL_I2C_IsDeviceReady(hi2c, Addr_Dest, 300, HAL_MAX_DELAY) == HAL_TIMEOUT);

        if(HAL_I2C_Mem_Read(hi2c, Addr_Dest, 0x00, I2C_MEMADD_SIZE_8BIT, RX_Buff, 32, HAL_MAX_DELAY) != HAL_OK)
        {
            printf("HAL_I2C_Mem_Read error\n");
            while(1);
        }
        while(1)
        {
            HAL_Delay(100);
        }
    }
    return 0;
}

#endif/* _TEMPLATE_I2C_MEM_ */
#endif/* BSP_DEBUG */

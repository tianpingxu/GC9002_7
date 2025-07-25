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
#ifdef  _TEMPLATE_I2C_MEM_IT_

#define TEST_IIC_INSTANCE       1
#if(defined TEST_IIC_INSTANCE && TEST_IIC_INSTANCE == 0U)
#define TEST_IIC_IRQNX          I2C0_IRQn
#else
#define TEST_IIC_IRQNX          I2C1_IRQn
#endif
static I2C_HandleTypeDef hi2c0 = {.Instance = I2C0};
static I2C_HandleTypeDef hi2c1 = {.Instance = I2C1};
static volatile uint32_t sem_flag = 0;
#define Addr_Self  0x56
#define Addr_Dest  0x51
static uint8_t TX_Buff[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
static uint8_t RX_Buff[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_I2Cx_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_I2Cx_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();
    
#if(defined TEST_IIC_INSTANCE && TEST_IIC_INSTANCE == 0U)
    HAL_I2C_IRQHandler(&hi2c0);
#else
    HAL_I2C_IRQHandler(&hi2c1);
#endif

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
static void TEST_HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
}
static void TEST_HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
}
static void TEST_HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
}
static void TEST_HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
}
static void TEST_HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    sem_flag = 1;
}
static void TEST_HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    sem_flag = 1;
}
static void TEST_HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    sem_flag = 1;
}
/* AT24C01/02每页有8个字节 
 * AT24C04/08A/16每页有16个字节 
 * AT24C01/02/04/08/16是一个 1K/2K/4K/8K/16K 位串行 CMOS，内部含有 128/256/512/1024/2048 个 8 位字节
 * AT24C01 有一个 8 字节页写缓冲器， AT24C02/04/08/16 有一个 16 字节页写缓冲器
 * 注意：在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数不能超过EEPROM页的大小
 */
#define EEPROM_PAGESIZE           8
int main(void)
{
    printf("Hello\n");
#if(defined TEST_IIC_INSTANCE && TEST_IIC_INSTANCE == 0U)
 	I2C_HandleTypeDef *hi2c = &hi2c0;
#else
 	I2C_HandleTypeDef *hi2c = &hi2c1;
#endif
    HAL_I2C_StructInit(hi2c);
    hi2c->Init.OwnAddress1 = Addr_Self;
    hi2c->Init.OwnAddress2 = Addr_Self;
    hi2c->Init.rx_fifo_threshold = 0;//Set this parameter based on the actual scenario, 
    HAL_I2C_Init(hi2c);
    HAL_I2C_RegisterCallback(hi2c, HAL_I2C_MEM_TX_COMPLETE_CB_ID, TEST_HAL_I2C_MemTxCpltCallback);
    HAL_I2C_RegisterCallback(hi2c, HAL_I2C_MEM_RX_COMPLETE_CB_ID, TEST_HAL_I2C_MemRxCpltCallback);
    HAL_I2C_RegisterCallback(hi2c, HAL_I2C_ERROR_CB_ID, TEST_HAL_I2C_ErrorCallback);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(TEST_IIC_IRQNX, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, TEMPLATE_I2Cx_IRQHandler);
#else
    csi_clic_register_irq(TEST_IIC_IRQNX, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, TEMPLATE_I2Cx_IRQHandler);
#endif

    uint32_t addr = 0;
    while(1)
    {
        for (int i = 0; i < sizeof(TX_Buff); i++)
        {
            TX_Buff[i] = i+1;
        }
        sem_flag = 0;
        addr = 0;
        if(HAL_I2C_Mem_Write_IT(hi2c, Addr_Dest, addr, I2C_MEMADD_SIZE_8BIT, &TX_Buff[addr], EEPROM_PAGESIZE) != HAL_OK)
        {
            printf("HAL_I2C_Mem_Write_IT error\n");
            while(1);
        }
        while(sem_flag == 0)
        {
            HAL_Delay(10);
        }
        while(HAL_I2C_IsDeviceReady(hi2c, Addr_Dest, 300, HAL_MAX_DELAY) == HAL_TIMEOUT);
        sem_flag = 0;
        addr += EEPROM_PAGESIZE;
        if(HAL_I2C_Mem_Write_IT(hi2c, Addr_Dest, addr, I2C_MEMADD_SIZE_8BIT, &TX_Buff[addr], EEPROM_PAGESIZE) != HAL_OK)
        {
            printf("HAL_I2C_Mem_Write_IT error\n");
            while(1);
        }
        while(sem_flag == 0)
        {
            HAL_Delay(10);
        }
        while(HAL_I2C_IsDeviceReady(hi2c, Addr_Dest, 300, HAL_MAX_DELAY) == HAL_TIMEOUT);

        sem_flag = 0;
        addr = 0;
        if(HAL_I2C_Mem_Read_IT(hi2c, Addr_Dest, addr, I2C_MEMADD_SIZE_8BIT, &RX_Buff[addr], EEPROM_PAGESIZE) != HAL_OK)
        {
            printf("HAL_I2C_Mem_Read_IT error\n");
            while(1);
        }
        while(sem_flag == 0)
        {
            HAL_Delay(10);
        }
        
        sem_flag = 0;
        addr += EEPROM_PAGESIZE;
        if(HAL_I2C_Mem_Read_IT(hi2c, Addr_Dest, addr, I2C_MEMADD_SIZE_8BIT, &RX_Buff[addr], EEPROM_PAGESIZE) != HAL_OK)
        {
            printf("HAL_I2C_Mem_Read_IT error\n");
            while(1);
        }
        while(sem_flag == 0)
        {
            HAL_Delay(10);
        }
        if(memcmp(TX_Buff, RX_Buff, EEPROM_PAGESIZE*2) == 0)
        {
            printf("EEPROM test success.\n");
        } else {
            printf("EEPROM test failed.\n");
        }
        while(1)
        {
            HAL_Delay(100);
        }
    }
    return 0;
}

#endif/* _TEMPLATE_I2C_MEM_IT_ */
#endif/* BSP_DEBUG */

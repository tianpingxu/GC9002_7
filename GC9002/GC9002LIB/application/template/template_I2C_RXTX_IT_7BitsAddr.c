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
#ifdef  _TEMPLATE_I2C_RXTX_IT_7BITSADDR_

static I2C_HandleTypeDef hi2c0 = {.Instance = I2C0};
static I2C_HandleTypeDef hi2c1 = {.Instance = I2C1};

#define Addr_Self  0x56
#define Addr_Dest  0x57
static volatile uint8_t sem_flag = 0;
static uint8_t TX_Buff[32] = {0x32, 0x33, 0x34, 0x35, 0x32, 0x33, 0x34, 0x35, 0x32, 0x33, 0x34, 0x35, 0x32, 0x33, 0x34, 0x35};

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_I2C0_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_I2C0_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();
    HAL_I2C_IRQHandler(&hi2c0);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

static void TEST_HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    sem_flag = 1;
}
static void TEST_HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    sem_flag = 1;
}
static void TEST_HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    sem_flag = 1;
}
static void TEST_HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    sem_flag = 1;
}
static void TEST_HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    sem_flag = 1;
}

int main(void)
{
	I2C_HandleTypeDef *hi2c = &hi2c0;
    HAL_I2C_StructInit(hi2c);
    hi2c->Init.OwnAddress1 = Addr_Self;
    hi2c->Init.OwnAddress2 = Addr_Self;
    HAL_I2C_Init(hi2c);
    HAL_I2C_RegisterCallback(hi2c, HAL_I2C_MASTER_TX_COMPLETE_CB_ID, TEST_HAL_I2C_MasterTxCpltCallback);
    HAL_I2C_RegisterCallback(hi2c, HAL_I2C_MASTER_RX_COMPLETE_CB_ID, TEST_HAL_I2C_MasterRxCpltCallback);
    HAL_I2C_RegisterCallback(hi2c, HAL_I2C_SLAVE_TX_COMPLETE_CB_ID, TEST_HAL_I2C_SlaveTxCpltCallback);
    HAL_I2C_RegisterCallback(hi2c, HAL_I2C_SLAVE_RX_COMPLETE_CB_ID, TEST_HAL_I2C_SlaveRxCpltCallback);
    HAL_I2C_RegisterCallback(hi2c, HAL_I2C_ERROR_CB_ID, TEST_HAL_I2C_ErrorCallback);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(I2C0_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, TEMPLATE_I2C0_IRQHandler);
#else
    csi_clic_register_irq(I2C0_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, TEMPLATE_I2C0_IRQHandler);
#endif

    while(1)
    {
		sem_flag = 0;
        HAL_I2C_Master_Transmit_IT(hi2c, Addr_Dest, TX_Buff, 16);
		while(sem_flag == 0);
        HAL_Delay(100);
		sem_flag = 0;
        HAL_I2C_Master_Receive_IT(hi2c, Addr_Dest, TX_Buff, 16);
		while(sem_flag == 0);
		sem_flag = 0;
        HAL_I2C_Slave_Receive_IT(hi2c, TX_Buff, 16);
		while(sem_flag == 0);
		sem_flag = 0;
        HAL_I2C_Slave_Transmit_IT(hi2c, TX_Buff, 16);
		while(sem_flag == 0);
    }
    while (1)
    {
		HAL_Delay(100);
    }
    return 0;
}

#endif/* _TEMPLATE_I2C_RXTX_IT_7BITSADDR_ */
#endif/* BSP_DEBUG */

/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-02-06     huyt        the first version
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx.h"
#include "core_rv32.h"
#include "template_def.h"
#include "csi_irq.h"

#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_SPI_


#define TEST_SPI_IT     1

static SPI_HandleTypeDef hspi;
//static uint8_t data_tx[64];

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_SPI0_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_SPI0_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    HAL_SPI_IRQHandler(&hspi);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
#define TEST_SPIx		SPI0
int main(void)
{
    printf("hello");
	hspi.Instance = TEST_SPIx;
	hspi.Init.Mode = SPI_MODE_MASTER | SPI_MODE_NORMAL;
	hspi.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi.Init.FrameFormat = SPI_FRF_MOTOROLA;
	hspi.Init.SPO = SPI_SPO_HIGH;
	hspi.Init.SPH = SPI_SPH_2EDGE;
	hspi.Init.CPSDVR = 0x10; // 时钟预分频系数 取值范围2-254之前的偶数
	hspi.Init.SCR = 0x7; // Clock pre-frequency dividing parameters     bitrate = Fspiclk/(CPSDVR*(1 + SCR))
	HAL_SPI_Init(&hspi);
    
    //注：SPI外设发送和接收分别有8个16位的DR，所以SPI中断才会有半空半满之说
    //    即深度为8的FIFO，发送时若FIFO可用空间大于等于4则会产生中断，接收时若FIFO已经存储了4个以上的数据会产生中断
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(SPI0_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEMPLATE_SPI0_IRQHandler);
#else
    csi_clic_register_irq(SPI0_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEMPLATE_SPI0_IRQHandler);
#endif
    uint8_t *data_tx = (uint8_t *)0x2001000;
    uint8_t *data_rx = (uint8_t *)0x2002000;
	while (1)
    {
#if(!TEST_SPI_IT)
		memset(data_tx, 'H', 64);
		HAL_SPI_Transmit(&hspi, data_tx, 32, HAL_MAX_DELAY);
        HAL_Delay(1000);
		memset(data_tx, 'Y', 64);
		HAL_SPI_Transmit(&hspi, data_tx, 32, HAL_MAX_DELAY);
        HAL_Delay(1000);
#else
		memset(data_tx, 'H', 64);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive_IT(&hspi, data_tx, data_rx, 32);
        HAL_Delay(1000);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
        while(1)
        {
            HAL_Delay(1000);
        }
        HAL_Delay(1000);
		memset(data_tx, 'Y', 64);
        HAL_SPI_TransmitReceive_IT(&hspi, data_tx, data_rx, 32);
        HAL_Delay(1000);
#endif
    }
    return 0;
}


#endif/* _TEMPLATE_SPI_ */
#endif/* BSP_DEBUG */

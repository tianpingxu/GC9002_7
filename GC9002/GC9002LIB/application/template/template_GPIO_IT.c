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
#ifdef  _TEMPLATE_GPIO_IT_

static uint32_t int_flag;
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_GPIOB_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_GPIOB_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();
    
    int_flag = HAL_GPIO_GetInterruptStatus(GPIOB, GPIO_PIN_ALL);
    
    HAL_GPIO_ClearInterruptFlag(GPIOB, GPIO_PIN_ALL);
    printf("%X\n", int_flag);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
int main(void)
{
    printf("Hello\n");
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
        
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT | GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
	GPIO_InitStruct.Pin = GPIO_PIN_16;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(GPIOB_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, TEMPLATE_GPIOB_IRQHandler);
#else
    csi_clic_register_irq(GPIOB_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, TEMPLATE_GPIOB_IRQHandler);
#endif
    while (1)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_InitStruct.Pin, GPIO_PIN_SET);
		HAL_Delay(100);
        HAL_GPIO_WritePin(GPIOB, GPIO_InitStruct.Pin, GPIO_PIN_RESET);
		HAL_Delay(100);
    }
    return 0;
}


#endif/* _TEMPLATE_GPIO_IT_ */
#endif/* BSP_DEBUG */

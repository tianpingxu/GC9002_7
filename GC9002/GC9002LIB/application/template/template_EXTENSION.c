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
#include "csi_irq_vectors.h"

#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_EXTENSION_

static uint64_t unsigned_dividend = 5;	//被除数
static uint32_t unsigned_divisor = 2;	//除数
static uint64_t unsigned_quotient = 0;	//商
static uint32_t unsigned_remainder = 0;	//余数
	
static int64_t signed_dividend = -5;	//被除数
static int32_t signed_divisor = 2;		//除数
static int64_t signed_quotient = 0;		//商
static int32_t signed_remainder = 0;	//余数
	
static uint32_t x_i;
static uint32_t y_i;
static uint32_t z_i;
static uint32_t x_o;
static uint32_t y_o;
static uint32_t z_o;
#define TEST_IT     0

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_EXTENSION_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_EXTENSION_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    HAL_EXTENSION_IRQHandler();

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
void HAL_EXTENSION_DivCpltCallback(void)
{
    HAL_EXTENSION_GetDivVal(&signed_quotient, &unsigned_remainder);
}
void HAL_EXTENSION_CordicCpltCallback(void)
{
    HAL_EXTENSION_GetCordicVal(&x_o, &y_o, &z_o);
}
void HAL_EXTENSION_DivErrCallback(void)
{
}

int main(void)
{
    while (1)
    {
#if(!TEST_IT)
		HAL_EXTENSION_DivUnsigned(unsigned_dividend, unsigned_divisor, &unsigned_quotient, &unsigned_remainder);
		HAL_EXTENSION_DivSigned(signed_dividend, signed_divisor, &signed_quotient, &signed_remainder);
		
		x_i = 0x10000;
		y_i = 0;
		z_i = 30 << 24;
		HAL_EXTENSION_CordicRotation(x_i, y_i, z_i, &x_o, &y_o, &z_o);
		HAL_EXTENSION_CordicVectoring(x_i, y_i, z_i, &x_o, &y_o, &z_o);
		
		x_i = 0x10000;
		y_i = 2 << 16;
		z_i = 0;
		HAL_EXTENSION_CordicRotation(x_i, y_i, z_i, &x_o, &y_o, &z_o);
		HAL_EXTENSION_CordicVectoring(x_i, y_i, z_i, &x_o, &y_o, &z_o);
#else
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
        csi_clic_register_irq(EXTENSION_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, TEMPLATE_EXTENSION_IRQHandler);
#else
        csi_clic_register_irq(EXTENSION_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, TEMPLATE_EXTENSION_IRQHandler);
#endif
        
		HAL_EXTENSION_DivUnsigned_IT(unsigned_dividend, unsigned_divisor);
		HAL_Delay(500);
		HAL_EXTENSION_DivSigned_IT(signed_dividend, signed_divisor);
		HAL_Delay(500);
        
		x_i = 0x10000;
		y_i = 0;
		z_i = 30 << 24;
		HAL_EXTENSION_CordicRotation_IT(x_i, y_i, z_i);
		HAL_Delay(500);
		HAL_EXTENSION_CordicVectoring_IT(x_i, y_i, z_i);
		HAL_Delay(500);
#endif
    }
    return 0;
}


#endif/* _TEMPLATE_EXTENSION_ */
#endif/* BSP_DEBUG */

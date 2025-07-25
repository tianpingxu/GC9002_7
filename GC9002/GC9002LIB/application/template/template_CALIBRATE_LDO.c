/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     template_GMAC.c
 * @brief    GMAC test code
 * @version  V1.0
 * @date     8. Feb 2023
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx.h"
#include "csi_motor.h"
#include "core_rv32.h"
#include "template_def.h"
#include "csi_kernel.h"


#if((defined BSP_DEBUG) && (BSP_DEBUG == TEST_TEMPLE_BSP))
#ifdef _TEMPLATE_LDO_


int main(void)
{
    printf("start to execute main fun. \n");

    HAL_StatusTypeDef ret = HAL_PMU_VTR12_Calibrate(200);
    if(ret != HAL_OK)
    {
        printf("PMU VTR12 Calibrate success.\n");
        while(1);
    }
	ret = HAL_PMU_VTR3318_Calibrate(200);
    if(ret != HAL_OK)
    {
        printf("PMU VTR3318 Calibrate success.\n");
        while(1);
    }
    printf("PMU Calibrate success.\n");

    while(1)
    {
        HAL_Delay(100);
    }
    return 0;
}

#endif 
#endif
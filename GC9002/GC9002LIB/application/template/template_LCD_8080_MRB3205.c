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
#ifdef  _TEMPLATE_LCD_8080_MRB3205_
#include "display_mrb3205.h"


unsigned int sourcebuffer[] =
{
    0x80000130,
    0x00000000,
    0x00000000,
    0x01900190,
    0xffff0000,
    0x19190000,
              
    0x80008030,
    0x00000000,
    0x00000000,
    0x01680168,
    0xff000000,
    0x16160000,
              
    0x80008038,
    0x00000000,
    0x00000000,
    0x00100010,
    0xffff0000,
    0x19320000,
              
    0x80000030,
    0xfff40000,
    0x00000000,
    0x01680168,
    0xffffffff,
    0x16160000,
              
    0x80008030,
    0x00000000,
    0x00000000,
    0x01680168,
    0xffff0000,
    0x16160000,
};

unsigned int sourcebuffer2[] =
{
    0x80008130,
    0x00000000,
    0x00000000,
    0x01900320,
    0xffffffff,
    0x19320000,

    0x80008050,
    0x00000000,
    0x00000000,
    0x01900320,
    0x0017b000,
    0x19320000,
};
int main(void)
{
    unsigned int *ptr;
    int  tmp, i;
    DISPLAY_HandleTypeDef hdisplay;
    display_mrb3205_init(&hdisplay, NULL);
    display_mrb3205_slave_passive(&hdisplay, NULL);
	while(1)
	{
		tmp = -(i << 16);
		sourcebuffer[6*1+1] = tmp;
		i++;
		if(i >= 50)
        {
			i = 0;
        }
		ptr = (unsigned int *)DRV_AHMI_SOURCE_BUFFER_BASE_ADDE;
        for(tmp = 0; tmp < 30; tmp++)
        {
            *ptr = sourcebuffer[tmp];
            ptr++;
        }
        HAL_AHMI_GpeStart();
        mdelay(50);
        while(!__AHMI_GET_IT_STATUS(AHMI_INT_DrawDone));
    }
    return 0;
}
#endif/* _TEMPLATE_LCD_8080_MRB3205_ */
#endif/* BSP_DEBUG */
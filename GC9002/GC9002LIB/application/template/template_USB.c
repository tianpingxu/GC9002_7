/*
 * Copyright (c) 2023-2053, GraphichinaSX Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-3-9       mazy         the first version
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx_hal.h"
#include "usb_config.h"
#include "csi_motor.h"
#include "core_rv32.h"
#include "template_def.h"
#include "csi_kernel.h"
#include "FreeRTOS.h" 
#include "task.h"


//注：测试USB功能时需要在gc9002_hal_usb.c文件设置USE_USB_TEST_TASK宏定义为 1
//	  V1.1硬件上需要短接J20，同时R61电阻需要正常焊接，然后PB12需要拉高
//	  V2.0硬件上需要短接J20，同时R61电阻需要正常焊接，然后PB17需要拉高


#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_USB_

k_task_handle_t g_uwTaskUsb;

#define TASK_USB_STK_SIZE   4096
#define TASK_USB_PRIOR      10
#define TIME_QUANTA         100

void usb_test(void* pvParameters);

void Dump_USB_All_Reg(void)
{
	uint8_t *pt = (uint8_t *)(USB_BASE);
	printf("====");
	for(int i=0; i<0x20; i++)
	{
		if((i % 8) == 0)
		{
			printf("\n");
		}
		printf("%02X ", pt[i]);
	}
	printf("\n");
}

void task_usb(void *p)
{
	usb_test(NULL);
	
    for(;;) {
        printf("task_usb ........\n");
		csi_kernel_delay(pdMS_TO_TICKS(1000));
    }
}

int main(void)
{	
    printf("usb test.\n");
	csi_kernel_init();
	
	csi_kernel_task_new((k_task_entry_t)task_usb, "task_usb", NULL, TASK_USB_PRIOR, TIME_QUANTA, NULL, TASK_USB_STK_SIZE, &g_uwTaskUsb);
	if (g_uwTaskUsb == NULL) {
		csi_kernel_sched_resume(0);
		printf("Fail to create task_usb! \r\n");
	}
	
	csi_kernel_start();

	
	while(1)
	{
		printf("Hello FreeRTOS.\n");
		HAL_Delay(100*1000);
	}
	return 0;
}

#endif /* _TEMPLATE_USB_ */
#endif /* TEST_TEMPLE_BSP */
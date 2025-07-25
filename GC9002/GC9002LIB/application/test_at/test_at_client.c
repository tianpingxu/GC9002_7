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
#include <at_device.h>
#include <test_at_def.h>

#if(BSP_DEBUG == TEST_AT)
#ifdef  _TEST_AT_CLIENT_

#define ECR6600_SAMPLE_DEIVCE_NAME      "e103w11"
#define ECR6600_SAMPLE_CLIENT_NAME      "e103w11"
#define ECR6600_SAMPLE_WIFI_SSID        "wifi-e103w11"
#define ECR6600_SAMPLE_WIFI_PASSWORD    "123456"
#define ECR6600_SAMPLE_RECV_BUFF_LEN    128

#define TASK_STK_SIZE     1024
#define TASK_PRIOR_TASK1  10
#define TASK_PRIOR_TASK2  11
#define TASK_PRIOR_TASK3  12
#define TIME_QUANTA 100
static k_task_handle_t g_uwTask1;
static k_task_handle_t g_uwTask2;
static void task1(void *p)
{
    at_command_init();
    struct at_device *dev = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, ECR6600_SAMPLE_DEIVCE_NAME);
    while(1)
    {
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_AT, NULL, NULL, 0);
        csi_kernel_delay(pdMS_TO_TICKS(1000));
    }
}
static void task2(void *p)
{
    while(1)
    {
        csi_kernel_delay(pdMS_TO_TICKS(1000));
    }
}
int main(void)
{
#ifndef CONFIG_KERNEL_NONE
    csi_kernel_init();
    csi_kernel_task_new((k_task_entry_t)task1, "task1", NULL, TASK_PRIOR_TASK1, TIME_QUANTA, NULL, TASK_STK_SIZE, &g_uwTask1);
    if (g_uwTask1 == NULL) {
        csi_kernel_sched_resume(0);
        printf("Fail to create task1! \r\n");
    }
    // csi_kernel_task_new((k_task_entry_t)task2, "task2", NULL, TASK_PRIOR_TASK2, TIME_QUANTA, NULL, TASK_STK_SIZE, &g_uwTask2);
    // if (g_uwTask2 == NULL) {
    //     csi_kernel_sched_resume(0);
    //     printf("Fail to create task2! \r\n");
    // }
    csi_kernel_start();
#endif
    return 0;
}

#endif /* _TEST_AT_CLIENT_ */
#endif/* BSP_DEBUG */

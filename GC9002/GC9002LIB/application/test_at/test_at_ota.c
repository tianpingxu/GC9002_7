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
#include <at_main.h>
#include <test_at_def.h>

#if(BSP_DEBUG == TEST_AT)
#ifdef  _TEST_AT_OTA_

#define ECR6600_SAMPLE_DEIVCE_NAME      "e103w11"
#define ECR6600_SAMPLE_CLIENT_NAME      "e103w11"
#define ECR6600_SAMPLE_WIFI_SSID        "wifi-e103w11"
#define ECR6600_SAMPLE_WIFI_PASSWORD    "123456"
#define ECR6600_SAMPLE_RECV_BUFF_LEN    128

#define TASK_STK_SIZE     2048
#define TASK_PRIOR_TASK1  10
#define TASK_PRIOR_TASK2  11
#define TASK_PRIOR_TASK3  12
#define TIME_QUANTA 100
static k_task_handle_t g_uwTask1;
static k_task_handle_t g_uwTask2;

#define TEST_OTA_CIUPDATE  	    //OTA 升级              ["AT+CIUPDATE"]
#define TEST_OTA_CICHANGE	    //系统切换			    ["AT+CICHANGE"]

#define TEST_OTA_INTERFACE    1U
#if(defined TEST_OTA_INTERFACE && TEST_OTA_INTERFACE == 1U)
static void task1(void *p)
{
    int result = 0;
    char buff[512];
    at_command_init();
    struct at_device *dev = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, ECR6600_SAMPLE_DEIVCE_NAME);
    while(1)
    {
        #ifdef TEST_OTA_CIUPDATE
        result = at_ota_CIUPDATE(dev, "http://10.15.12.226/ota.bin");
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_OTA_CICHANGE
        result = at_ota_CICHANGE(dev, 1);
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        result = at_ota_CICHANGE(dev, 0);
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
    }
}
#else
static void task1(void *p)
{
    char buff[512];
    at_command_init();
    struct at_device *dev = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, ECR6600_SAMPLE_DEIVCE_NAME);
    while(1)
    {
        #ifdef TEST_OTA_CIUPDATE
        at_device_control(dev, AT_DEVICE_CTRL_OTA_CIUPDATE, "http://10.15.12.226/ota.bin", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_OTA_CICHANGE
        at_device_control(dev, AT_DEVICE_CTRL_OTA_CICHANGE, "1", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        at_device_control(dev, AT_DEVICE_CTRL_OTA_CICHANGE, "0", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
    }
}
#endif
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
    csi_kernel_start();
#endif
    return 0;
}

#endif /* _TEST_AT_OTA_ */
#endif/* BSP_DEBUG */

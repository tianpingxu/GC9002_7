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
#ifdef  _TEST_AT_BLE_

#define TEST_DEIVCE_NAME      "e103w11"
#define TEST_CLIENT_NAME      "e103w11"
#define TEST_WIFI_SSID        "wifi-e103w11"
#define TEST_WIFI_PASSWORD    "123456"
#define TEST_RECV_BUFF_LEN    128

#define TASK_STK_SIZE     2048
#define TASK_PRIOR_TASK1  10
#define TASK_PRIOR_TASK2  11
#define TASK_PRIOR_TASK3  12
#define TIME_QUANTA 100
static k_task_handle_t g_uwTask1;
static k_task_handle_t g_uwTask2;

// #define TEST_BASIC_AT
// #define TEST_BLE_BLEADDR        //Bluetooth LE 设备地址查询 ["AT+BLEADDR"]
#define TEST_BLE_BLEADVSTART    //开始 Bluetooth LE 广播 ["AT+BLEADVSTART"]
// #define TEST_BLE_BLEADVSTOP		//停止 Bluetooth LE 广播 ["AT+BLEADVSTOP"]
// #define TEST_BLE_BLEDISCONN		//断开 Bluetooth LE 连接 ["AT+BLEDISCONN"]
// #define TEST_BLE_BLENAME        //查询/设置 Bluetooth LE 设备名称 ["AT+BLENAME"]
// #define TEST_BLE_BLESCAN        //使能 Bluetooth LE 扫描(AT+BLESCAN) ["AT+BLESCAN"]

static void task1(void *p)
{
    int result = 0;
    char buff[512];
    at_command_init();
    struct at_device *dev = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, TEST_DEIVCE_NAME);
    while(1)
    {
        #ifdef TEST_BASIC_AT
        result = at_basic_AT(dev);
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BLE_BLEADDR
        result = at_ble_BLEADDR(dev, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BLE_BLEADVSTART
        result = at_ble_BLEADVSTART(dev);
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BLE_BLEADVSTOP
        result = at_ble_BLEADVSTOP(dev);
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BLE_BLEDISCONN
        result = at_ble_BLEDISCONN(dev);
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BLE_BLENAME
        result = at_ble_BLENAME_get(dev, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(100));
        result = at_ble_BLENAME_set(dev, "E103W11");
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        result = at_ble_BLENAME_get(dev, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(100));
        #endif
        #ifdef TEST_BLE_BLESCAN
        {
            unsigned int rssi;
            char addr[32];
            unsigned char *data;
            unsigned int data_size;
            result = at_ble_BLESCAN(dev, &rssi, addr, sizeof(addr), (unsigned char *)buff, sizeof(buff));
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
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
    csi_kernel_start();
#endif
    return 0;
}

#endif /* _TEST_AT_BLE_ */
#endif/* BSP_DEBUG */

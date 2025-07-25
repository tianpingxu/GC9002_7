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
#ifdef  _TEST_AT_TCP_SEND_

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

#define TEST_OTA_INTERFACE    1U
#if(defined TEST_OTA_INTERFACE && TEST_OTA_INTERFACE == 1U)
static void test(void)
{
    extern int netdev_cmd_ping(char* target_name, rt_uint32_t times, rt_size_t size);
    netdev_cmd_ping("www.baidu.com", 4, 0);
    while(1);
}
/* 测试条件：
 * 1.正确连接WiFi
 * 2.使用网络调试助手创建TCP Server，并开启监听
 *      IP地址ipconfig查询PC的无线局域网适配器WLAN的自动分配的ip地址
 *      端口号任意填写
 **/
#define TEST_TCP_SERVER_IP      "192.168.3.13"
#define TEST_TCP_SERVER_PORT    10012
static void task1(void *p)
{
    int result = 0;
    char buff[512];
    at_command_init();
    struct at_device *dev = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, TEST_DEIVCE_NAME);
    while(1)
    {
        extern int netdev_cmd_ping(char* target_name, rt_uint32_t times, rt_size_t size);
        result = netdev_cmd_ping("www.baidu.com", 4, 0);
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        result = at_tcpip_CIPMUX_set(dev, 0);
        while(1)
        {
            result = at_tcpip_CIPSTART_tcp(dev, 0, TEST_TCP_SERVER_IP, TEST_TCP_SERVER_PORT, 0);
            if(RT_EOK == result)
            {
                break;
            }
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        char str[] = "Hello World\r\n";
        result = at_tcpip_CIPSEND(dev, 0, str, strlen(str), 0, TEST_TCP_SERVER_IP, TEST_TCP_SERVER_PORT);
        if(RT_EOK != result)
        {
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        /* E103W11不支持 AT+CIPRECVMODE 和 AT+CIPRECVDATA 指令，支持MCU被动接收E103W11的socket 数据
         * 当E103W11接收到Socket数据后，会直接发送给MCU，AT Client 通过urc方式处理接收的数据
         * 格式：单连接 +IPD,<link ID>,<len>
         *       多连接 +IPD,<len>
         */
        // result = at_tcpip_CIPRECVMODE_set(dev, 1);
        // char data[16];
        // result = at_tcpip_CIPRECVDATA(dev, 0, 0, sizeof(data), data);
        struct s_tcpip_status sta;
        result = at_tcpip_CIPSTATUS(dev, &sta);
        while(1)
        {
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
    }
}
#else
static void task1(void *p)
{
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

#endif /* _TEST_AT_TCP_SEND_ */
#endif/* BSP_DEBUG */

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
#ifdef  _TEST_AT_WIFI_

#define TEST_DEIVCE_NAME      "e103w11"
#define TEST_CLIENT_NAME      "e103w11"
#define TEST_WIFI_SSID        "HUAWEI-1B9HG9"
#define TEST_WIFI_PASSWORD    "huyangtao"
#define TEST_RECV_BUFF_LEN    128

#define TASK_STK_SIZE     3072
#define TASK_PRIOR_TASK1  10
#define TASK_PRIOR_TASK2  11
#define TASK_PRIOR_TASK3  12
#define TIME_QUANTA 100
static k_task_handle_t g_uwTask1;
static k_task_handle_t g_uwTask2;

#define TEST_BASIC_AT
#define TEST_WIFI_CWMODE   		//设置 Wi-Fi 工作模式   ["AT+CWMODE"]
// #define TEST_WIFI_CWJAP			//连接到 AP				["AT+CWJAP"]
#define TEST_WIFI_CWLAP    		//扫描 AP               ["AT+CWLAP"]
// #define TEST_WIFI_CWQAP    		//与 AP 断开连接        ["AT+CWQAP"]
// #define TEST_WIFI_CWSAP			//设置 AP 模式          ["AT+CWSAP"]
// #define TEST_WIFI_CWDHCP		//设置 DHCP 模式      	["AT+CWDHCP"]
// #define TEST_WIFI_CWDHCPS		//设置 DHCP 规则和网关	["AT+CWDHCPS"]
// #define TEST_WIFI_CWAUTOCONN	//设置自动连接        	["AT+CWAUTOCONN"]

#define TEST_WIFI_INTERFACE    1U
#if(defined TEST_WIFI_INTERFACE && TEST_WIFI_INTERFACE == 1U)
static void task1(void *p)
{
    int result = 0;
    at_command_init();
    struct at_device *dev = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, TEST_DEIVCE_NAME);
    while(1)
    {
        #ifdef TEST_BASIC_AT
        result = at_basic_AT(dev);
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_WIFI_CWMODE
        {
            unsigned int mode_cur;
            unsigned int mode_def;
            result = at_wifi_CWMODE_get(dev, &mode_cur, &mode_def);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
            // result = at_wifi_CWMODE_set(dev, 1, false);
            // csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_WIFI_CWJAP
        {
            struct at_wifi_CWJAP cur;
            struct at_wifi_CWJAP def;
            result = at_wifi_CWJAP_get(dev, &cur, &def);
            csi_kernel_delay(pdMS_TO_TICKS(1000));

            result = at_wifi_CWJAP_set(dev, TEST_WIFI_SSID, TEST_WIFI_PASSWORD, true);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_WIFI_CWLAP
        {
            static struct at_wifi_CWLAP cwlap[20];//定义 static 使用全局栈，不使用任务栈
            result = at_wifi_CWLAP_get(dev, cwlap, sizeof(cwlap)/sizeof(struct at_wifi_CWLAP));
            csi_kernel_delay(pdMS_TO_TICKS(1000));
            // char *ssid = "";
            // char *bssid = "";
            // result = at_wifi_CWLAP_set(dev, ssid, bssid);
            // csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_WIFI_CWQAP
        {
            result = at_wifi_CWJAP_set(dev, TEST_WIFI_SSID, TEST_WIFI_PASSWORD, true);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
            result = at_wifi_CWQAP(dev);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_WIFI_CWSAP
        {
            result = at_wifi_CWMODE_set(dev, 3, false);
            csi_kernel_delay(pdMS_TO_TICKS(1000));

            struct at_wifi_CWSAP ap_mode;
            sprintf(ap_mode.ssid, TEST_WIFI_SSID);
            sprintf(ap_mode.passwd, TEST_WIFI_PASSWORD);
            ap_mode.ch = 1;
            ap_mode.auth_mode = 3;
            ap_mode.max_con = 8;
            ap_mode.hidden_ssid = 0;
            result = at_wifi_CWSAP_set(dev, ap_mode, true);
            csi_kernel_delay(pdMS_TO_TICKS(1000));

            memset(ap_mode.ssid, 0, sizeof(struct at_wifi_CWSAP));
            result = at_wifi_CWSAP_get(dev, &ap_mode);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
            
        }
        #endif
        #ifdef TEST_WIFI_CWDHCP
        {
            unsigned int SoftAP_status_cur;
            unsigned int Station_status_cur; 
            unsigned int SoftAP_status_def;
            unsigned int Station_status_def;
            result = at_wifi_CWDHCP_set(dev, 1, 1, true);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
            result = at_wifi_CWDHCP_get(dev, &SoftAP_status_cur, &Station_status_cur, &SoftAP_status_def, &Station_status_def);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_WIFI_CWDHCPS
        {
            struct at_wifi_CWDHCPS cur, def;
            result = at_wifi_CWDHCPS_get(dev, &cur, &def);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
            // result = at_wifi_CWDHCPS_set(dev, 1, 120, "\"192.168.10.100\"", "\"192.168.10.120\"", false);
            // csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_WIFI_CWAUTOCONN
        result = at_wifi_CWAUTOCONN(dev, true);
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
    }
}
#else
static void task1(void *p)
{
    char buff[512];
    at_command_init();
    struct at_device *dev = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, TEST_DEIVCE_NAME);
    while(1)
    {
        #ifdef TEST_WIFI_CWMODE
        at_device_control(dev, AT_DEVICE_CTRL_WIFI_CWMODE, NULL, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        at_device_control(dev, AT_DEVICE_CTRL_WIFI_CWMODE, "1,\"s.y\"", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_WIFI_CWJAP
        at_device_control(dev, AT_DEVICE_CTRL_WIFI_CWJAP, NULL, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        at_device_control(dev, AT_DEVICE_CTRL_WIFI_CWJAP, "\"MI_6\",\"12345678\",\"s.y\"", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_WIFI_CWLAP
        at_device_control(dev, AT_DEVICE_CTRL_WIFI_CWLAP, NULL, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        at_device_control(dev, AT_DEVICE_CTRL_WIFI_CWLAP, "\"MI_6\",\"20:47:da:49:1e:26\"", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_WIFI_CWQAP
        at_device_control(dev, AT_DEVICE_CTRL_WIFI_CWQAP, NULL, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_WIFI_CWSAP
        at_device_control(dev, AT_DEVICE_CTRL_WIFI_CWSAP, NULL, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        at_device_control(dev, AT_DEVICE_CTRL_WIFI_CWSAP, "\"ESWIN\",\"12345678\",5,4,8,0,\"s.y\"", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_WIFI_CWDHCP
        at_device_control(dev, AT_DEVICE_CTRL_WIFI_CWDHCP, NULL, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        at_device_control(dev, AT_DEVICE_CTRL_WIFI_CWDHCP, "1,1,\"s.y\"", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_WIFI_CWDHCPS
        at_device_control(dev, AT_DEVICE_CTRL_WIFI_CWDHCPS, NULL, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        at_device_control(dev, AT_DEVICE_CTRL_WIFI_CWDHCPS, "1,120,\"192.168.10.100\",\"192.168.10.120\",\"s.y\"", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_WIFI_CWAUTOCONN
        at_device_control(dev, AT_DEVICE_CTRL_WIFI_CWAUTOCONN, "0", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        at_device_control(dev, AT_DEVICE_CTRL_WIFI_CWAUTOCONN, "1", buff, sizeof(buff));
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

#endif /* _TEST_AT_WIFI_ */
#endif/* BSP_DEBUG */

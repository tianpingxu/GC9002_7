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
#ifdef  _TEST_AT_TCPIP_

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

#define TEST_BASIC_AT
// #define TEST_TCPIP_CIPSTATUS		//查询网络连接状态      ["AT+CIPSTATUS"]
// #define TEST_TCPIP_CIPMUX     		//设置连接类型          ["AT+CIPMUX"]
// #define TEST_TCPIP_CIPSTART		    //创建TCP/UDP/SSL 客户端["AT+CIPSTART"]
// #define TEST_TCPIP_CIPSEND 			//发送数据              ["AT+CIPSEND"]
// #define TEST_TCPIP_CIPCLOSE    		//关闭 TCP/UDP/SSL 连接 ["AT+CIPCLOSE"]
// #define TEST_TCPIP_CIPSERVER		//创建 TCP/UDP 服务器   ["AT+CIPSERVER"]
// #define TEST_TCPIP_CIPMODE    		//设置传输模式(正常模式或透传模式）["AT+CIPMODE"]
// #define TEST_TCPIP_SAVETRANSLINK	//保存传输链接并启用自动链接["AT+SAVETRANSLINK"]
// #define TEST_TCPIP_CIPRECVMODE		//设置自动接收数据模式  ["AT+CIPRECVMODE"]
// #define TEST_TCPIP_CIPRECVDATA		//获得 socket 数据(被动模式)  ["AT+CIPRECVDATA"]
// #define TEST_TCPIP_PING        	    //获得 socket 数据(被动模式)  ["AT+PING"]
// #define TEST_TCPIP_CIPSTAMAC		//设置 station mac 地址 ["AT+CIPSTAMAC"]
// #define TEST_TCPIP_CIPSTA			//设置 station 静态 IP  ["AT+CIPSTA"]
// #define TEST_TCPIP_CIPAP			//配置 softap IP 地址   ["AT+CIPAP"]

#define TEST_OTA_INTERFACE    1U
#if(defined TEST_OTA_INTERFACE && TEST_OTA_INTERFACE == 1U)
static void test(void)
{
    extern int netdev_cmd_ping(char* target_name, rt_uint32_t times, rt_size_t size);
    netdev_cmd_ping("www.baidu.com", 4, 0);
    while(1);
}
static void task1(void *p)
{
    int result = 0;
    char buff[512];
    at_command_init();
    struct at_device *dev = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, TEST_DEIVCE_NAME);
    test();
    while(1)
    {
        #ifdef TEST_BASIC_AT
        result = at_basic_AT(dev);
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        // at_socket();
        #ifdef TEST_TCPIP_CIPSTATUS
        {
            struct s_tcpip_status status;
            result = at_tcpip_CIPSTATUS(dev, &status);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif // TEST_TCPIP_CIPSTATUS
        #ifdef TEST_TCPIP_CIPMUX
        {
            unsigned int link_mux;
            result = at_tcpip_CIPMUX_get(dev, &link_mux);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
            result = at_tcpip_CIPMUX_set(dev, link_mux);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_TCPIP_CIPSTART
        {   //TCP
            unsigned int link_mux = 0;
            unsigned int link_id = 0;
            char *remote_ip = "192.168.101.110";
            unsigned int remote_port = 1000;
            unsigned int keep_alive_time;
            result = at_tcpip_CIPSTART_tcp(dev, link_id, remote_ip, remote_port, keep_alive_time);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        {   //UDP
            unsigned int link_mux = 0;
            unsigned int link_id = 0;
            char *remote_ip = "192.168.101.110";
            unsigned int remote_port = 1000;
            unsigned int local_port = 0;
            unsigned int mode = 0;
            result = at_tcpip_CIPSTART_udp(dev, link_mux, link_id, remote_ip, remote_port, local_port, mode);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        {   //SSL
            unsigned int link_mux = 0;
            unsigned int link_id = 0;
            char *remote_ip = "192.168.101.110";
            unsigned int remote_port = 1000;
            unsigned int keep_alive_time = 0;
            result = at_tcpip_CIPSTART_ssl(dev, link_mux, link_id, remote_ip, remote_port, keep_alive_time);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_TCPIP_CIPSEND
        {
            unsigned int link_mux = 0;
            unsigned int link_id = 0;
            char buff[128];
            unsigned int size = sizeof(buff);
            unsigned int type = 0;
            char *remote_ip = "192.168.101.110";
            unsigned int remote_port = 1000;
            result = at_tcpip_CIPSEND(dev, link_mux, link_id, buff, size, type, remote_ip, remote_port);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_TCPIP_CIPCLOSE
        {
            unsigned int link_mux = 0;
            unsigned int link_id = 0;
            result = at_tcpip_CIPCLOSE(dev, link_mux, link_id);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_TCPIP_CIPSERVER
        {
            unsigned int mode = 0;//0：关闭服务器 1：建⽴服务器
            unsigned int port = 0;//端⼝号，默认为 333
            unsigned int type = 0;//0-TCP 服务器  1-UDP 服务器
            result = at_tcpip_CIPSERVER(dev, mode, port, type);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_TCPIP_CIPMODE
        {
            unsigned int mode = 0;//0：普通传输模式 1：透传模式，仅⽀持 TCP 单连接和 UDP 固定通信对端的情况
            result = at_tcpip_CIPMODE_get(dev, &mode);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
            result = at_tcpip_CIPMODE_set(dev, mode);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_TCPIP_SAVETRANSLINK
        {   //TCP
            unsigned int mode = 0;//0：取消开机透传 1：保存开机进⼊透传模式
            char *remote_ip = "192.168.6.110";//远端 IP 或者域名
            unsigned int remote_port = 1002;//远端端⼝
            unsigned int keep_alive_time = 0;
            result = at_tcpip_SAVETRANSLINK_tcp(dev, mode, remote_ip, remote_port, keep_alive_time);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        {   //UDP
            unsigned int mode = 0;//0：取消开机透传 1：保存开机进⼊透传模式
            char *remote_ip = "192.168.6.110";//远端 IP 或者域名
            unsigned int remote_port = 1002;//远端端⼝
            unsigned int local_port = 1005;//开机进⼊UDP 传输时，使⽤的本地端⼝
            result = at_tcpip_SAVETRANSLINK_udp(dev, mode, remote_ip, remote_port, local_port);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_TCPIP_CIPRECVMODE
        {
            /* 0：主动模式；收到 TCP 数据时，通过+IPD 主动将数据通过串⼝发给 MCU
             * 1：被动模式；默认使⽤2920 bytes 的 buffer⽤于缓存接收到的 TCP 数据，被动等待 MCU 读取；
             *    接收buffer 满后，将阻塞对端 TCP 发送数据
             */
            unsigned int mode = 0;
            result = at_tcpip_CIPRECVMODE_get(dev, &mode);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
            result = at_tcpip_CIPRECVMODE_set(dev, mode);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_TCPIP_CIPRECVDATA
        {
            char data[128];
            unsigned int link_mux = 0;
            unsigned int link_id = 0;
            unsigned int len = sizeof(data);
            result = at_tcpip_CIPRECVDATA(dev, link_mux, link_id, len, data);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_TCPIP_PING
        {
            char *ip = "www.baidu.com";
            result = at_tcpip_PING(dev, ip);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_TCPIP_CIPSTAMAC
        {
            char mac_cur[16];
            char mac_def[16];
            result = at_tcpip_CIPSTAMAC_get(dev, mac_cur, sizeof(mac_cur), mac_def, sizeof(mac_def));
            csi_kernel_delay(pdMS_TO_TICKS(1000));
            result = at_tcpip_CIPSTAMAC_set(dev, mac_cur, false);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_TCPIP_CIPSTA
        {
            struct s_tcpip_net_info cur;
            struct s_tcpip_net_info def;
            result = at_tcpip_CIPSTA_get(dev, &cur, &def);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
            result = at_tcpip_CIPSTA_set(dev, &cur, false);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_TCPIP_CIPAP
        {
            struct s_tcpip_net_info cur;
            struct s_tcpip_net_info def
            result = at_tcpip_CIPAP_get(dev, &cur, &def);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
            result = at_tcpip_CIPAP_get(dev, cur, false);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
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

#endif /* _TEST_AT_TCPIP_ */
#endif/* BSP_DEBUG */

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
#ifdef  _TEST_AT_BASIC_

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

#define TEST_BASIC_AT			//测试 AT 启动          ["AT"]
// #define TEST_BASIC_RST			//重启模块              ["AT+RST"]
// #define TEST_BASIC_GMR			//查询版本信息          ["AT+GMR"]
// #define TEST_BASIC_CMD			//列出所有 AT 命令      ["AT+CMD"]
// #define TEST_BASIC_RESTORE		//恢复出厂设置功能      ["AT+RESTORE"]
#define TEST_BASIC_UART 	    //查询 UART 配置        ["AT+UART?"]
// #define TEST_BASIC_SYSIOSETCFG	//设置 GPIO 口工作模式  ["AT+SYSIOSETCFG"]
// #define TEST_BASIC_SYSIOGETCFG	//查询 GPIO 口工作模式  ["AT+SYSIOGETCFG"]
// #define TEST_BASIC_SYSGPIODIR	//设置 GPIO 口方向      ["AT+SYSGPIODIR"]
// #define TEST_BASIC_SYSGPIOWRITE //设置 GPIO 输出电平    ["AT+SYSGPIOWRITE"]
// #define TEST_BASIC_SYSGPIOREAD	//读取 GPIO 状态        ["AT+SYSGPIOREAD"]
// #define TEST_BASIC_ATE			//关闭回显功能          ["ATE"]
// #define TEST_BASIC_SYSRAM		//查询系统堆大小        ["AT+SYSRAM"]
// #define TEST_BASIC_SYSTEMP		//查询芯片内部温度      ["AT+SYSTEMP"]
// #define TEST_BASIC_SYSFLASH	    //读/写/擦 Flash 分区   ["AT+SYSFLASH"]

#define TEST_BASIC_INTERFACE    1U
#if(defined TEST_BASIC_INTERFACE && TEST_BASIC_INTERFACE == 1U)
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
        #ifdef TEST_BASIC_RST
        result = at_basic_RST(dev);
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_GMR
        memset(buff, 0, sizeof(buff));
        result = at_basic_GMR(dev, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_CMD
        memset(buff, 0, sizeof(buff));
        result = at_basic_CMD(dev, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_RESTORE
        result = at_basic_RESTORE(dev);
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_UART
        {
            struct at_uart_config cur;
            struct at_uart_config def;
            result = at_basic_UART_get(dev, &cur, &def);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
            // result = at_basic_UART_set(dev, cur, false);
            // csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_BASIC_SYSIOSETCFG
        {
            unsigned int pin = 13;
            unsigned int mode = 1;
            unsigned int pull = 1;
            result = at_basic_SYSIOSETCFG(dev, pin, mode, pull);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_BASIC_SYSIOGETCFG
        {
            unsigned int pin = 13;
            unsigned int mode = 0;
            unsigned int pull = 0;
            result = at_basic_SYSIOGETCFG(dev, pin, &mode, &pull);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_BASIC_SYSGPIODIR
        {
            unsigned int pin = 13;
            unsigned int dir = 1;
            result = at_basic_SYSGPIODIR(dev, pin, dir);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_BASIC_SYSGPIOWRITE
        {
            unsigned int pin = 13;
            unsigned int level = 0;
            result = at_basic_SYSGPIOWRITE(dev, pin, level);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
            level = 1;
            result = at_basic_SYSGPIOWRITE(dev, pin, level);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_BASIC_SYSGPIOREAD
        {
            unsigned int pin = 13;
            unsigned int dir = 0;
            unsigned int level = 0;
            result = at_basic_SYSGPIOREAD(dev, pin, &dir, &level);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_BASIC_ATE
        result = at_basic_ATE(dev, true);
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        result = at_basic_ATE(dev, false);
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_SYSRAM
        {
            unsigned int release_heap;
            unsigned int release_heap_min;
            result = at_basic_SYSRAM(dev, &release_heap, &release_heap_min);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_BASIC_SYSTEMP
        {
            unsigned int temperature;
            result = at_basic_SYSTEMP(dev, &temperature);
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
        #endif
        #ifdef TEST_BASIC_SYSFLASH
        {
            char partition[] = "nv_amt";
            unsigned int offset = 0;
            unsigned int length = 4096;
            unsigned char data[16];
            memset(data, 0x3A, sizeof(data));
            // at_basic_SYSFLASH_get(dev, buff, sizeof(buff));
            result = at_basic_SYSFLASH_erase(dev, partition, offset, length);
            result = at_basic_SYSFLASH_write(dev, partition, offset, data, sizeof(data));
            result = at_basic_SYSFLASH_read(dev, partition, offset, data, sizeof(data));
            csi_kernel_delay(pdMS_TO_TICKS(1000));
        }
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
        #ifdef TEST_BASIC_AT
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_AT, NULL, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_RST
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_RST, NULL, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_GMR
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_GMR, NULL, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_CMD
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_CMD, NULL, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_RESTORE
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_RESTORE, NULL, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_UART
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_UART, NULL, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_UART, "115200,8,1,0,0,\"s.y\"", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_SYSIOSETCFG
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_SYSIOSETCFG, "13,1,1", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_SYSIOGETCFG
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_SYSIOGETCFG, "13", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_SYSGPIODIR
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_SYSGPIODIR, "13,1", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_SYSGPIOWRITE
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_SYSGPIOWRITE, "13,1", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_SYSGPIOREAD
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_SYSGPIOREAD, "13", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_ATE
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_ATE, "0", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_ATE, "1", buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_SYSRAM
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_SYSRAM, NULL, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_SYSTEMP
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_SYSTEMP, NULL, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
        #ifdef TEST_BASIC_SYSFLASH
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_SYSFLASH, NULL, buff, sizeof(buff));
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        at_device_control(dev, AT_DEVICE_CTRL_BASIC_SYSFLASH, "1,\"nv_amt\",0,10", buff, sizeof(buff));
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

#endif /* _TEST_AT_BASIC_ */
#endif/* BSP_DEBUG */

/*
 * File      : at_tcpip_command.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-31     huyt         first version
 */

#ifndef __AT_CONFIG_E103W11_H__
#define __AT_CONFIG_E103W11_H__

#ifdef __cplusplus
extern "C" {
#endif

#if(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)

#define E103W11_WAIT_CONNECT_TIME      5000
#define E103W11_THREAD_STACK_SIZE      2048
#define E103W11_THREAD_PRIORITY        (RT_THREAD_PRIORITY_MAX / 2)

#define E103W11_MODULE_SEND_MAX_SIZE   2048

#define AT_TIMEOUT_NORMAL           50000
#define AT_TIMEOUT_UPDATE           1000000
#define AT_TIMEOUT_SYS_SW           1000000

#define AT_DEVICE_INTERFACE_BASIC		1U
#define AT_DEVICE_INTERFACE_OTA			1U
#define AT_DEVICE_INTERFACE_WIFI		1U
#define AT_DEVICE_INTERFACE_TCPIP		1U
#define AT_DEVICE_INTERFACE_BLE			1U
#define AT_DEVICE_INTERFACE_MQTT		1U

#define AT_WIFI_SSID_LENGTH             (32U)
#define AT_WIFI_PASSWORD_LENGTH         (32U)
#define AT_WIFI_IP_LENGTH               (16U)
#define AT_WIFI_MAC_ADDR_LENGTH         (24U)

#endif /* AT_DEVICE_USING_E103W11 */

#ifdef __cplusplus
}
#endif

#endif /* __AT_CONFIG_E103W11_H__ */


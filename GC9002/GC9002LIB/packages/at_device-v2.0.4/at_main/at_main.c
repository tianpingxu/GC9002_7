/*
 * File      : at_sample_ecr6600.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-05-10     chenyong     first version
 */
#include <at_main.h>

#define LOG_TAG                        "at.main"
#include <at_log.h>

#if(defined AT_DEVICE_USING_ECR6600 && AT_DEVICE_USING_ECR6600 == 1U)
#define TEST_DEIVCE_NAME      "ecr6600"
#define TEST_CLIENT_NAME      "ecr6600"
#define TEST_WIFI_SSID        "wifi-ecr6600"
#define TEST_WIFI_PASSWORD    "123456"
#define TEST_RECV_BUFF_LEN    128

static struct at_device_ecr6600 ecr6600 =
{
    TEST_DEIVCE_NAME,
    TEST_CLIENT_NAME,

    TEST_WIFI_SSID,
    TEST_WIFI_PASSWORD,
    TEST_RECV_BUFF_LEN,
};

int ecr6600_device_register(void)
{
    struct at_device_ecr6600 *ecr6600_p = &ecr6600;

    return at_device_register(&(ecr6600_p->device),
                              ecr6600_p->device_name,
                              ecr6600_p->client_name,
                              AT_DEVICE_CLASS_ECR6600,
                              (void *)ecr6600_p);
}
#elif(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)
#define TEST_DEIVCE_NAME      "e103w11"
#define TEST_CLIENT_NAME      "e103w11"
#define TEST_WIFI_SSID        "HUAWEI-1B9HG9"
#define TEST_WIFI_PASSWORD    "huyangtao"
#define TEST_RECV_BUFF_LEN    512

static struct at_device_e103w11 e103w11 =
{
    TEST_DEIVCE_NAME,
    TEST_CLIENT_NAME,

    TEST_WIFI_SSID,
    TEST_WIFI_PASSWORD,
    TEST_RECV_BUFF_LEN,
};

int e103w11_device_register(void)
{
    struct at_device_e103w11 *e103w11_p = &e103w11;

    return at_device_register(&(e103w11_p->device),
                              e103w11_p->device_name,
                              e103w11_p->client_name,
                              AT_DEVICE_CLASS_E103W11,
                              (void *)e103w11_p);
}
#endif

void at_command_init(void)
{
#if(defined AT_DEVICE_USING_ECR6600 && AT_DEVICE_USING_ECR6600 == 1U)
    // board
    ecr6600_device_class_register();
    // app
    ecr6600_device_register();
    
    struct at_device *device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, TEST_DEIVCE_NAME);
    at_register_basic_commands(device);
    at_register_ota_commands(device);
    at_register_wifi_commands(device);
    at_register_tcpip_commands(device);
    at_register_ble_commands(device);
    at_register_mqtt_commands(device);
#elif(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)
    // board
    e103w11_device_class_register();
    // app
    e103w11_device_register();
    
    struct at_device *device = at_device_get_by_name(AT_DEVICE_NAMETYPE_CLIENT, TEST_DEIVCE_NAME);
    at_register_basic_commands(device);
    at_register_ota_commands(device);
    at_register_wifi_commands(device);
    at_register_tcpip_commands(device);
    at_register_ble_commands(device);
    at_register_mqtt_commands(device);
#endif
}
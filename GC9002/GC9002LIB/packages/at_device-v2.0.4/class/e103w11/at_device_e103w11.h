/*
 * File      : at_device_e103w11.h
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
 * 2019-05-16     chenyong     first version
 */

#ifndef __AT_DEVICE_E103W11_H__
#define __AT_DEVICE_E103W11_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <at_device.h>

#if(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)

#include "e103w11/at_config_e103w11.h"
#include <e103w11/basic_command/at_basic_command.h>
#include <e103w11/ota_command/at_ota_command.h>
#include <e103w11/wifi_command/at_wifi_command.h>
#include <e103w11/tcpip_command/at_tcpip_command.h>
#include <e103w11/ble_command/at_ble_command.h>
#include <e103w11/mqtt_command/at_mqtt_command.h>

/* The maximum number of sockets supported by the e103w11 device */
#define AT_DEVICE_E103W11_SOCKETS_NUM  5

struct at_device_e103w11
{
    char *device_name;
    char *client_name;

    char *wifi_ssid;
    char *wifi_password;
    size_t recv_line_num;
    struct at_device device;

    void *user_data;
};

#if(defined AT_USING_SOCKET && AT_USING_SOCKET == 1U)

/* e103w11 device socket initialize */
int e103w11_socket_init(struct at_device *device);
/* e103w11 device class socket register */
int e103w11_socket_class_register(struct at_device_class *class);
#endif /* AT_USING_SOCKET */

int e103w11_device_class_register(void);


int e103w11_device_cmd_no_result(struct at_device *device, const char *cmd, int timeout);
int e103w11_device_cmd_ret_string(struct at_device *device, const char *cmd, int timeout, 
                                  unsigned int resp_line, char *buff_str, unsigned int size);


#endif /* AT_DEVICE_USING_E103W11 */

#ifdef __cplusplus
}
#endif

#endif /* __AT_DEVICE_E103W11_H__ */

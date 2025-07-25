/*
 * File      : at_ota_command.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-31     huyt         first version
 */

#ifndef __AT_OTA_COMMAND_H__
#define __AT_OTA_COMMAND_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "at_device.h"
#include "e103w11/at_config_e103w11.h"
#include "e103w11/at_device_e103w11.h"
#if(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)


void at_register_ota_commands(struct at_device *device);

#if(defined AT_DEVICE_INTERFACE_OTA && AT_DEVICE_INTERFACE_OTA == 1U)
int at_ota_CIUPDATE(struct at_device *device, const char *url);
int at_ota_CICHANGE(struct at_device *device, unsigned int check);
#endif /* AT_DEVICE_INTERFACE_OTA */


#endif /* AT_DEVICE_USING_E103W11 */

#ifdef __cplusplus
}
#endif

#endif /* __AT_OTA_COMMAND_H__ */
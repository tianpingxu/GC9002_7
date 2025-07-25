/*
 * File      : at_ble_command.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-31     huyt         first version
 */

#ifndef __AT_BLE_COMMAND_H__
#define __AT_BLE_COMMAND_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "at_device.h"
#include "e103w11/at_config_e103w11.h"
#if(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)


void at_register_ble_commands(struct at_device *device);

#if(defined AT_DEVICE_INTERFACE_BLE && AT_DEVICE_INTERFACE_BLE == 1U)
int at_ble_BLEADDR(struct at_device *device, char *addr, unsigned int size);
int at_ble_BLEADVSTART(struct at_device *device);
int at_ble_BLEADVSTOP(struct at_device *device);
int at_ble_BLEDISCONN(struct at_device *device);
int at_ble_BLENAME_get(struct at_device *device, char *name, unsigned int size);
int at_ble_BLENAME_set(struct at_device *device, const char *name);
int at_ble_BLESCAN(struct at_device *device, 
				   unsigned int *rssi, 
				   char *addr, 
				   unsigned int addr_size,
				   unsigned char *data,
				   unsigned int data_size);
#endif /* AT_DEVICE_INTERFACE_BLE */

#endif /* AT_DEVICE_USING_E103W11 */

#ifdef __cplusplus
}
#endif

#endif /* __AT_BLE_COMMAND_H__ */
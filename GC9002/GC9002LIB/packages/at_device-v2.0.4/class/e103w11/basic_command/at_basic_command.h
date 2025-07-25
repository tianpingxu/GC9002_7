/*
 * File      : at_basic_command.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-31     huyt         first version
 */

#ifndef __AT_BASIC_COMMAND_H__
#define __AT_BASIC_COMMAND_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "at_device.h"
#include "e103w11/at_config_e103w11.h"
#if(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)



void at_register_basic_commands(struct at_device *device);

#if(defined AT_DEVICE_INTERFACE_BASIC && AT_DEVICE_INTERFACE_BASIC == 1U)
int at_basic_AT(struct at_device *device);
int at_basic_RST(struct at_device *device);
int at_basic_GMR(struct at_device *device, char *buff, unsigned int size);
int at_basic_CMD(struct at_device *device, char *buff, unsigned int size);
int at_basic_RESTORE(struct at_device *device);
struct at_uart_config
{
	unsigned int baudrate;
	unsigned int databits;
	unsigned int stopbits;
	unsigned int parity;
	unsigned int control;
};
int at_basic_UART_get(struct at_device *device, 
				  struct at_uart_config *cur, 
				  struct at_uart_config *def);
int at_basic_UART_set(struct at_device *device, 
				  const struct at_uart_config cur, 
				  bool save_flash);
int at_basic_SYSIOSETCFG(struct at_device *device, unsigned int pin, unsigned int mode, unsigned int pull);
int at_basic_SYSIOGETCFG(struct at_device *device, unsigned int pin, unsigned int *mode, unsigned int *pull);
int at_basic_SYSGPIODIR(struct at_device *device, unsigned int pin, unsigned int dir);
int at_basic_SYSGPIOWRITE(struct at_device *device, unsigned int pin, unsigned int level);
int at_basic_SYSGPIOREAD(struct at_device *device, unsigned int pin, unsigned int *dir, unsigned int *level);
int at_basic_ATE(struct at_device *device, bool enable);
int at_basic_SYSRAM(struct at_device *device, unsigned int *release_heap, unsigned int *release_heap_min);
int at_basic_SYSTEMP(struct at_device *device, unsigned int *temperature);
int at_basic_SYSFLASH_get(struct at_device *device, char *buff, unsigned int size);
int at_basic_SYSFLASH_erase(struct at_device *device, const char *partition, unsigned int offset, unsigned int length);
int at_basic_SYSFLASH_write(struct at_device *device, 
                            char *partition, unsigned int offset, 
                            const unsigned char *data, unsigned int size);
int at_basic_SYSFLASH_read(struct at_device *device, 
                            char *partition, unsigned int offset, 
                            unsigned char *data, unsigned int size);
#endif /* AT_DEVICE_INTERFACE_BASIC */


#endif /* AT_DEVICE_USING_E103W11 */

#ifdef __cplusplus
}
#endif

#endif /* __AT_BASIC_COMMAND_H__ */
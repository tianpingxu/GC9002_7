/*
 * File      : at_main.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-05-31     huyt         first version
 */

#ifndef __AT_MAIN_H__
#define __AT_MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <at_device.h>

#if(defined AT_DEVICE_USING_ECR6600 && AT_DEVICE_USING_ECR6600 == 1U)
#include <ecr6600/at_device_ecr6600.h>
#endif /* AT_DEVICE_USING_ECR6600 */

#if(defined AT_DEVICE_USING_E103W11 && AT_DEVICE_USING_E103W11 == 1U)
#include <e103w11/at_device_e103w11.h>
#endif /* AT_DEVICE_USING_E103W11 */

void at_command_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __AT_MAIN_H__ */

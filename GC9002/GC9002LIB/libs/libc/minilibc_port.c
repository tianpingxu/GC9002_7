/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     minilibc_port.c
 * @brief    minilibc port
 * @version  V1.0
 * @date     26. Dec 2017
 ******************************************************************************/

#include <stdio.h>
#include <csi_config.h>
#ifndef CONFIG_KERNEL_NONE
#include <csi_kernel.h>
#endif

#include <gc90xx_hal_conf.h>


__attribute__((weak)) int write(int __fd, __const void *__buf, int __n)
{
    return 0;
}

int fputc(int ch, FILE *stream)
{
#if(defined CONFIG_PRINTF_ENABLE && CONFIG_PRINTF_ENABLE == 1U)
    (void)stream;
    if (ch == '\n')
	{
		HAL_UART_SendData(CONFIG_CONSOLE_DEBUG_UART, '\r');
    }
	HAL_UART_SendData(CONFIG_CONSOLE_DEBUG_UART, ch);
#endif /* CONFIG_PRINTF_ENABLE */
    return 0;
}

int fgetc(FILE *stream)
{
    (void)stream;
#if(defined CONFIG_PRINTF_ENABLE && CONFIG_PRINTF_ENABLE == 1U)
    return (int)(HAL_UART_ReceiveData(CONFIG_CONSOLE_DEBUG_UART) & 0xFF);
#else
    return 0;
#endif /* CONFIG_PRINTF_ENABLE */
}

int os_critical_enter(unsigned int *lock)
{
    (void)lock;
#ifndef CONFIG_KERNEL_NONE
    csi_kernel_sched_suspend();
#endif

    return 0;
}

int os_critical_exit(unsigned int *lock)
{
    (void)lock;
#ifndef CONFIG_KERNEL_NONE
    csi_kernel_sched_resume(0);
#endif

    return 0;
}

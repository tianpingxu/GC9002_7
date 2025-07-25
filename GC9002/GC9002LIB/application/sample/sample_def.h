/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-02-18     huyt       the first version
 */
#ifndef __SAMPLE_DEF_H__
#define __SAMPLE_DEF_H__

#include <main.h>

#if(defined BSP_DEBUG && BSP_DEBUG == TEST_SAMPLE_CSI)

//#define _SAMPLE_CSI_ADC_
//#define _SAMPLE_CSI_MOTOR_PWM_
#define _SAMPLE_CSI_MOTOR_TIMER_
//#define _SAMPLE_CSI_CRC_
//#define _SAMPLE_CSI_TRIANGLE_
//#define _SAMPLE_CSI_DMA_
//#define _SAMPLE_CSI_I2S_
//#define _SAMPLE_CSI_SPIFLASH_
//#define _SAMPLE_CSI_QSPI_
//#define _SAMPLE_CSI_PWM_
//#define _SAMPLE_CSI_IIC_
//#define _SAMPLE_CSI_SPI_
//#define _SAMPLE_CSI_RTC_

// #define _SAMPLE_CSI_WDT_
//#define _SAMPLE_CSI_TIMER_
//#define _SAMPLE_CSI_TICK_

//#define _SAMPLE_CSI_UART_
    
//#define _SAMPLE_CSI_PIN_
//#define _SAMPLE_CSI_GPIO_PIN_
//#define _SAMPLE_CSI_GPIO_

#endif /* (defined BSP_DEBUG && BSP_DEBUG) */
#endif /* __SAMPLE_DEF_H__ */

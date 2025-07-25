/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-08-18     huyt       the first version
 */
#ifndef __TEMPLATE_DEF_H__
#define __TEMPLATE_DEF_H__

#include <main.h>

#if(defined BSP_DEBUG && BSP_DEBUG == TEST_TEMPLE_BSP)

//#define _TEMPLATE_LCD_QSPI_ST77903_
//#define _TEMPLATE_LCD_SPI_ST7735S_
//#define _TEMPLATE_LCD_QSPI_SJXD_
//#define _TEMPLATE_LCD_8080_MRB3205_
//#define _TEMPLATE_LCD_888_
//#define _TEMPLATE_DISPLAY_
//#define _TEMPLATE_VIDEO_
// #define _TEMPLATE_HYPER_
// #define _TEMPLATE_DMA_
// #define _TEMPLATE_USB_
//#define _TEMPLATE_GMAC_
// #define _TEMPLATE_LDO_
// #define _TEMPLATE_ADC_
// #define _TEMPLATE_ADC_DMA_

//#define _TEMPLATE_TEST_
//#define _TEMPLATE_SPI_

// #define _TEMPLATE_TIM_TIMER_
// #define _TEMPLATE_TIM_PWM_
// #define _TEMPLATE_TIM_PWM_CAPTURE_
//#define _TEMPLATE_TIM_ENCODER_

//  #define _TEMPLATE_WDT_

// #define _TEMPLATE_EXTENSION_
//#define _TEMPLATE_CRC32_

//#define _TEMPLATE_VIDEOCAP_DEV_

//#define _TEMPLATE_GPIO_DEV_
// #define _TEMPLATE_GPIO_IT_

//#define _TEMPLATE_CANFD_DEV_
//#define _TEMPLATE_CANFD_LOOPBACK_IT_
// #define _TEMPLATE_CANFD_IT_
//#define _TEMPLATE_CANFD_NORMAL_
//#define _TEMPLATE_CANFD_LOOPBACK_

//#define _TEMPLATE_CAN_IT_
//#define _TEMPLATE_CAN_DEV_
//#define _TEMPLATE_CAN_LOOPBACK_

//#define _TEMPLATE_QSPI_FLASH_

//#define _TEMPLATE_RTC_
//#define _TEMPLATE_RTC_ALARM_

//#define _TEMPLATE_MIPI_DSIH_API_

// #define _TEMPLATE_I2C_MEM_IT_
//#define _TEMPLATE_I2C_MEM_
//#define _TEMPLATE_I2C_RXTX_7BITSADDR_
//#define _TEMPLATE_I2C_RXTX_10BITSADDR_
// #define _TEMPLATE_I2C_RXTX_IT_7BITSADDR_
//#define _TEMPLATE_I2C_RXTX_DMA_7BITSADDR_
//#define _TEMPLATE_I2C_DRV_
// #define _TEMPLATE_I2C_SLAVER_


//#define _TEMPLATE_I2S_RXTX_
//#define _TEMPLATE_I2S_RXTX_IT_
//#define _TEMPLATE_I2S_RXTX_DMA_

//#define _TEMPLATE_SDIO_SDCARD_WRITEREAD_
//#define _TEMPLATE_SDIO_SDCARD_INTERRUPT_
//#define _TEMPLATE_SDIO_SDCARD_SDMA_
//#define _TEMPLATE_SDIO_SDCARD_ADMA1_  /* ADMA1 页和描述符定义空间必须是4kB对齐，页的大小默认是4kB */
//#define _TEMPLATE_SDIO_SDCARD_ADMA2_
//#define _TEMPLATE_SDCARD_DRV_SDIO_
//#define _TEMPLATE_SDIO_HOST_

// #define _TEMPLATE_UART_DMA_RX_
//#define _TEMPLATE_UART_DMA_TX_
// #define _TEMPLATE_UART_IDLE_DMA_RX_
// #define _TEMPLATE_UART_IT_


//#define BSP_UART3_RX_USING_DMA
//#define BSP_UART3_TX_USING_DMA

#define _TEMPLATE_LOWPOWER_

#ifdef _TEMPLATE_DISPLAY_
    #ifndef HAL_DISPLAY_MODULE_ENABLED
    #error need defined HAL_DISPLAY_MODULE_ENABLED to use these lcd test cases
    #endif
#endif
#if((defined _TEMPLATE_LCD_QSPI_ST77903_) || \
    (defined _TEMPLATE_LCD_SPI_ST7735S_)  || \
    (defined _TEMPLATE_LCD_QSPI_SJXD_)    || \
    (defined _TEMPLATE_LCD_8080_MRB3205_))
    #ifndef HAL_AHMI_MODULE_ENABLED
    #error need defined HAL_AHMI_MODULE_ENABLED to use these lcd test cases
    #endif
#endif

#endif /* (defined BSP_DEBUG && BSP_DEBUG) */
#endif /* __TEMPLATE_DEF_H__ */

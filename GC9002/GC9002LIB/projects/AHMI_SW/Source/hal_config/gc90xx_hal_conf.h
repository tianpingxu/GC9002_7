/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-25     huyt       the first version
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC90XX_HAL_CONF_H__
#define __GC90XX_HAL_CONF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* ########################## Module Selection ############################## */
/**
  * @brief This is the list of modules to be used in the HAL driver
  */

#define HAL_MODULE_ENABLED
#define HAL_CONFIG_SECTION_SRAM         ".flashdriver"
#define HAL_ATTRIBUTE_SECTION_RAM      __attribute__((used, section(HAL_CONFIG_SECTION_SRAM)))
/* Bootloader needs this to call RCC interface, AHMI not need */
#define HAL_CONFIG_SYSCLK            0U
#define HAL_CONFIG_LOWPOWER          0U

// #define HAL_MMALLOC_ENABLED
#define HAL_CCM_MODULE_ENABLED
#define HAL_HYPER_MODULE_ENABLED
#define HAL_CRC32_MODULE_ENABLED
#define HAL_EXTENSION_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
//#define HAL_SD_MODULE_ENABLED
//#define HAL_MMC_MODULE_ENABLED
#define HAL_I2S_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
#define HAL_ADC_MODULE_ENABLED
// #define HAL_CAN_MODULE_ENABLED
#define HAL_CANFD_MODULE_ENABLED
#define HAL_RTC_MODULE_ENABLED
#define HAL_PMU_MODULE_ENABLED
#define HAL_QSPI_MODULE_ENABLED
#define HAL_QSPI_FLASH_MODULE_ENABLED
#define HAL_ETH_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#define HAL_VIDEOCAP_MODULE_ENABLED
#define HAL_WDT_MODULE_ENABLED
#define HAL_USB_MODULE_ENABLED
#define HAL_DISPLAY_MODULE_ENABLED
//#define HAL_AHMI_MODULE_ENABLED

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the
  *        HAL drivers code
  */
#define USE_FULL_ASSERT    1U

/* ################## Ethernet peripheral configuration ##################### */
/* Section 1 : Ethernet peripheral configuration */

/* MAC ADDRESS: MAC_ADDR0:MAC_ADDR1:MAC_ADDR2:MAC_ADDR3:MAC_ADDR4:MAC_ADDR5 */
#define MAC_ADDR0   0x00U
#define MAC_ADDR1   0x55U
#define MAC_ADDR2   0x7BU
#define MAC_ADDR3   0xB5U
#define MAC_ADDR4   0x7DU
#define MAC_ADDR5   0xF7U

/* Definition of the Ethernet driver buffers size and count */
#define ETH_RX_BUF_SIZE                ETH_MAX_PACKET_SIZE /* buffer size for receive               */
#define ETH_TX_BUF_SIZE                ETH_MAX_PACKET_SIZE /* buffer size for transmit              */
#define ETH_RXBUFNB                    ((uint32_t)4U)       /* 16 Rx buffers of size ETH_RX_BUF_SIZE  */
#define ETH_TXBUFNB                    ((uint32_t)4U)       /* 16 Tx buffers of size ETH_TX_BUF_SIZE  */

/* Section 2: PHY configuration section */

/* LAN8720A_PHY_ADDRESS Address*/
#define LAN8720A_PHY_ADDRESS            0x00U
/* PHY Reset delay these values are based on a 1 ms Systick interrupt*/
#define PHY_RESET_DELAY                 ((uint32_t)0x0000000FU)
/* PHY Configuration delay */
#define PHY_CONFIG_DELAY                ((uint32_t)0x0000000FU)

#define PHY_READ_TO                     ((uint32_t)0x0000FFFFU)
#define PHY_WRITE_TO                    ((uint32_t)0x0000FFFFU)

/* Section 3: Common PHY Registers */

#define PHY_BCR                         ((uint16_t)0x0000U)    /*!< Transceiver Basic Control Register   */
#define PHY_BSR                         ((uint16_t)0x0001U)    /*!< Transceiver Basic Status Register    */

#define PHY_RESET                       ((uint16_t)0x8000U)  /*!< PHY Reset */
#define PHY_LOOPBACK                    ((uint16_t)0x4000U)  /*!< Select loop-back mode */
#define PHY_FULLDUPLEX_100M             ((uint16_t)0x2100U)  /*!< Set the full-duplex mode at 100 Mb/s */
#define PHY_HALFDUPLEX_100M             ((uint16_t)0x2000U)  /*!< Set the half-duplex mode at 100 Mb/s */
#define PHY_FULLDUPLEX_10M              ((uint16_t)0x0100U)  /*!< Set the full-duplex mode at 10 Mb/s  */
#define PHY_HALFDUPLEX_10M              ((uint16_t)0x0000U)  /*!< Set the half-duplex mode at 10 Mb/s  */
#define PHY_AUTONEGOTIATION             ((uint16_t)0x1000U)  /*!< Enable auto-negotiation function     */
#define PHY_RESTART_AUTONEGOTIATION     ((uint16_t)0x0200U)  /*!< Restart auto-negotiation function    */
#define PHY_POWERDOWN                   ((uint16_t)0x0800U)  /*!< Select the power down mode           */
#define PHY_ISOLATE                     ((uint16_t)0x0400U)  /*!< Isolate PHY from MII                 */

#define PHY_AUTONEGO_COMPLETE           ((uint16_t)0x0020U)  /*!< Auto-Negotiation process completed   */
#define PHY_LINKED_STATUS               ((uint16_t)0x0004U)  /*!< Valid link established               */
#define PHY_JABBER_DETECTION            ((uint16_t)0x0002U)  /*!< Jabber condition detected            */

/* Section 4: Extended PHY Registers */
#define PHY_SR                          ((uint16_t)0x1FU)    /*!< PHY status register Offset                      */

#define PHY_SPEED_STATUS                ((uint16_t)0x000CU)  /*!< PHY Speed mask                                  */
#define PHY_SPEED_STATUS_10M            ((uint16_t)0x0004U)  /*!< PHY Speed 10M                                   */
#define PHY_SPEED_STATUS_100M           ((uint16_t)0x0008U)  /*!< PHY Speed 100M                                  */
#define PHY_DUPLEX_STATUS               ((uint16_t)0x0010U)  /*!< PHY Duplex mask                                 */

/* ########################### System Configuration ######################### */
#define USE_PERIPHERAL_LOCK                     0U
#define USE_FPGA_SOC                            0U
#define USE_RTOS                                0U
#if(defined USE_FPGA_SOC && USE_FPGA_SOC == 1U)
#define USE_FPGA_SOC_FREQ                       96000000
#endif

/* =========================================================================================================================== */
/* ================                                  Macro defined switch                                     ================ */
/* =========================================================================================================================== */
#define DMA_FLUSH_INVALIDATE_DCACHE_EN          (0U)// 9002 dma Don't need Flush DCache
#define DMA_FLUSH_INVALIDATE_DCACHE_ALL_EN      (0U)
#define DRAM_CACHE_ENABLED                      (0U)
#define DMAH_CHx_MULTI_BLK_EN                   (0U)
#define DMAH_CHx_CTL_WB_EN                      (0U)
#define DMAH_CHx_STAT_SRC                       (0U)
/**
 * @brief UART Configuration
 */
#define BSP_UART_BAUDRATE_MAX                   (3000000)
/**
  * @brief If use I2S bus Implement write protection, must set 1
  */
#define USE_HAL_QSPI_WRITE_PROTECTION           (1U)
#define USE_HAL_QSPI_WRITE_LOW_DIV              (8U)
/**
  * @brief I2S Configuration
  */
#if(defined CHIP_GC9005)
#define BSP_I2S_TXRX_FIFO                       (16U)
#define BSP_I2S_RX_CHANNELS                     (2U)
#define BSP_I2S_TX_CHANNELS                     (1U)
#define BSP_I2S_TXRX_DMA_ENABLED                (1U) /* If I2S support DMA, please set 1U */
#elif(defined CHIP_GC9002)
#define BSP_I2S_TXRX_FIFO                       (8U)
#define BSP_I2S_RX_CHANNELS                     (0U)
#define BSP_I2S_TX_CHANNELS                     (1U)
#define BSP_I2S_TXRX_DMA_ENABLED                (0U) /* If I2S support DMA, please set 1U */
#endif
/**
  * @brief I2C Configuration
  */
#define I2C_IC_EMPTYFIFO_HOLD_MASTER_EN         (0U)
#define BSP_I2C_TXRX_DMA_ENABLED                (0U) /* If I2C support DMA, please set 1U */
#define BSP_I2C_TX_FIFO_DEPTH                   (8U)
#define BSP_I2C_RX_FIFO_DEPTH                   (8U)
/**
* @brief CANFD Configuration
*/
#define BSP_CANFD_STB_SLOTS                     (10U)
#define BSP_CANFD_PTB_SLOTS                     (1U)
// CANFD_PrescalerTypeDef
#define BSP_CANFD_PRESCALER_6                   CANFD_PRESCALER_0
#define BSP_CANFD_PRESCALER_11                  CANFD_PRESCALER_1
#define BSP_CANFD_PRESCALER_12                  CANFD_PRESCALER_2
#define BSP_CANFD_PRESCALER_24                  CANFD_PRESCALER_3

 /**
 * @brief GPIO Configuration
 */
#define BSP_GPIO_USE_PHY                        (0U)
 /**
 * @brief callback Configuration
 */
#define USE_HAL_UART_REGISTER_CALLBACKS         (1U) /* UART register callback disabled      */
#define USE_HAL_RTC_REGISTER_CALLBACKS          (1U)
#define USE_HAL_TIM_REGISTER_CALLBACKS          (1U)
#define USE_HAL_I2S_REGISTER_CALLBACKS          (1U)
#define USE_HAL_SPI_REGISTER_CALLBACKS          (1U)
#define USE_HAL_CANFD_REGISTER_CALLBACKS        (1U)
#define USE_HAL_I2C_REGISTER_CALLBACKS          (1U)
#define USE_HAL_DISPLAY_REGISTER_CALLBACKS      (1U)
#define USE_HAL_AHMI_REGISTER_CALLBACKS         (1U)
#define USE_HAL_ADC_REGISTER_CALLBACKS          (1U)
#define USE_HAL_WDT_REGISTER_CALLBACKS          (1U)
#define USE_HAL_DMA_REGISTER_CALLBACKS          (1U)




/* Includes ------------------------------------------------------------------*/

#include "mmalloc.h"
/**
  * @brief Include module's header file
  */
#ifdef HAL_RCC_MODULE_ENABLED
#include "gc9002_hal_rcc.h"
#endif /* HAL_RCC_MODULE_ENABLED */

#ifdef HAL_CCM_MODULE_ENABLED
#include "gc9002_hal_ccm.h"
#endif /* HAL_CCM_MODULE_ENABLED */

#ifdef HAL_HYPER_MODULE_ENABLED
#include "gc9002_hal_hyper.h"
#endif /* HAL_HYPER_MODULE_ENABLED */

#ifdef HAL_CRC32_MODULE_ENABLED
#include "gc9002_hal_crc32.h"
#endif /* HAL_CRC32_MODULE_ENABLED */

#ifdef HAL_EXTENSION_MODULE_ENABLED
#include "gc9002_hal_extension.h"
#endif /* HAL_EXTENSION_MODULE_ENABLED */

#ifdef HAL_GPIO_MODULE_ENABLED
#include "gc90xx_hal_gpio.h"
#endif /* HAL_GPIO_MODULE_ENABLED */

#ifdef HAL_DMA_MODULE_ENABLED
#include "gc9002_hal_dma.h"
#endif /* HAL_DMA_MODULE_ENABLED */

#ifdef HAL_UART_MODULE_ENABLED
#include "gc90xx_hal_uart.h"
#endif /* HAL_UART_MODULE_ENABLED */

#ifdef HAL_SD_MODULE_ENABLED
#include "gc90xx_hal_sd.h"
#endif /* HAL_SD_MODULE_ENABLED */

#ifdef HAL_MMC_MODULE_ENABLED
#include "gc9002_hal_mmc.h"
#endif /* HAL_MMC_MODULE_ENABLED */

#ifdef HAL_I2S_MODULE_ENABLED
#include "gc90xx_hal_i2s.h"
#endif /* HAL_I2S_MODULE_ENABLED */

#ifdef HAL_I2C_MODULE_ENABLED
#include "gc90xx_hal_i2c.h"
#endif /* HAL_I2C_MODULE_ENABLED */

#ifdef HAL_MIPI_MODULE_ENABLED
#include "gc9002_hal_mipi.h"
#include "gc9002_hal_mipiPHY.h"
#include "gc9002_hal_mipiHostCtrl.h"
#include "gc9002_hal_mipiDataConvert.h"
#include "dwc_mipi_dsi_host.h"
//#include "mipi_dsih_dphy.h"
//#include "mipi_dsih_hal.h"
//#include "mipi_dsih_api.h"
#endif

#ifdef HAL_ETH_MODULE_ENABLED
#include "gc9002_hal_eth.h"
#endif /* HAL_ETH_MODULE_ENABLED */

#ifdef HAL_CAN_MODULE_ENABLED
#include "gc90xx_hal_can.h"
#endif /* HAL_CAN_MODULE_ENABLED */

#ifdef HAL_CANFD_MODULE_ENABLED
#include "gc90xx_hal_canfd.h"
#endif /* HAL_CANFD_MODULE_ENABLED */

#ifdef HAL_ADC_MODULE_ENABLED
#include "gc9002_hal_adc.h"
#endif /* HAL_ADC_MODULE_ENABLED */

#ifdef HAL_DAC_MODULE_ENABLED
#include "gc9002_hal_dac.h"
#endif /* HAL_DAC_MODULE_ENABLED */

#ifdef HAL_I2S_MODULE_ENABLED
#include "gc90xx_hal_i2s.h"
#endif /* HAL_I2S_MODULE_ENABLED */

#ifdef HAL_RTC_MODULE_ENABLED
#include "gc90xx_hal_rtc.h"
#endif /* HAL_RTC_MODULE_ENABLED */

#ifdef HAL_FLASH_MODULE_ENABLED
#include "gc9002_hal_flash.h"
#endif /* HAL_FLASH_MODULE_ENABLED */

#ifdef HAL_QSPI_MODULE_ENABLED
#include "gc90xx_ll_qspi.h"
#endif /* HAL_QSPI_MODULE_ENABLED */

#ifdef HAL_PMU_MODULE_ENABLED
#include "gc9002_hal_pmu.h"
#endif /* HAL_PMU_MODULE_ENABLED */
 
#ifdef HAL_ETH_MODULE_ENABLED
 #include "gc9002_hal_eth.h"
#endif /* HAL_ETH_MODULE_ENABLED */

#ifdef HAL_SPI_MODULE_ENABLED
#include "gc90xx_ll_spi.h"
#include "gc90xx_hal_spi.h"
#endif /* HAL_SPI_MODULE_ENABLED */

#ifdef HAL_TIM_MODULE_ENABLED
#include "gc90xx_hal_tim.h"
#include "gc90xx_hal_tim_ex.h"
#endif /* HAL_TIMER_MODULE_ENABLED */

#ifdef HAL_WDT_MODULE_ENABLED
#include "gc9002_hal_wdt.h"
#endif /* HAL_WDT_MODULE_ENABLED */

#ifdef HAL_SHAREDRAM_MODULE_ENABLED
 #include "gc9002_hal_sharedRam.h"
#endif /* HAL_SHAREDRAM_MODULE_ENABLED */

#ifdef HAL_VIDEOCAP_MODULE_ENABLED
#include "gc90xx_hal_videocap.h"
#endif /* HAL_VIDEOCAP_MODULE_ENABLED */

#ifdef HAL_USB_MODULE_ENABLED
#include "gc9002_hal_usb.h"
#endif /* HAL_USB_MODULE_ENABLED */

#ifdef HAL_DISPLAY_MODULE_ENABLED
#include "gc9002_hal_display.h"
#include "gc9002_ll_display.h"
#endif /* HAL_DISPLAY_MODULE_ENABLED */

#ifdef HAL_AHMI_MODULE_ENABLED
// #include "gc9002_ll_ahmi.h"
#include "gc9002_hal_ahmi.h"
#endif /* HAL_AHMI_MODULE_ENABLED */


#ifdef __cplusplus
}
#endif

#endif /* __GC90XX_HAL_CONF_H__ */

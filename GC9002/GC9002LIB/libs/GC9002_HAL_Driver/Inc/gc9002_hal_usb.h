/**
  ******************************************************************************
  * @file    gc9002_hal_USB.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    2022-10-18
  * @brief   This file contains all the functions prototypes for the RTC
  *          firmware library.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC9002_HAL_USB_H__
#define __GC9002_HAL_USB_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gc90xx_hal_def.h"
#include <stdint.h>

#ifdef HAL_USB_MODULE_ENABLED

uint32_t checkMSDDevice(void);     
struct usbh_msc *msc_dev_probe(uint32_t timeOutCnt);

#if _USE_USB_TEST_TASK
void usb_test(void* pvParameters);
#endif
     
#endif /* HAL_USB_MODULE_ENABLED */
#ifdef __cplusplus
 }
#endif
#endif /* __GC9002_HAL_USB_H__ */

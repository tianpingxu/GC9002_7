/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-28     Administrator       the first version
 */
#ifndef APP_AHMI_TOUCH_STTOUCH_H_
#define APP_AHMI_TOUCH_STTOUCH_H_

#include "stdint.h"
#include <stdio.h>
#include <string.h>
#include "bsp_i2c_touch.h"
#include "AHMITouchDefine.h"
#include "capacitiveTouch_Interface.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TOUCH_IC_ST
#define     ST1633I_I2C_ADDR 0xAA

#define ST_ADDR_LENGTH    1

/* 表示读数据 */
#define ST_I2C_M_RD        0x0001

/*
* 存储I2C通讯的信息
* @addr：  从设备的I2C设备地址
* @flags: 控制标志
* @len：  读写数据的长度
* @buf：  存储读写数据的指针
**/
struct ST_i2c_msg {
   uint8_t addr;       /*从设备的I2C设备地址 */
   uint16_t flags; /*控制标志*/
   uint16_t len;       /*读写数据的长度           */
   uint8_t *buf;       /*存储读写数据的指针 */
};

typedef enum
{
    FIRMWARE_VERSION,
    STATUS_REG,
    DEVICE_CONTROL_REG,
    TIMEOUT_TO_IDLE_REG,
    XY_RESOLUTION_HIGH,
    X_RESOLUTION_LOW,
    Y_RESOLUTION_LOW,
    DEVICE_CONTROL_REG2 = 0x09,
    FIRMWARE_REVISION_3 = 0x0C,
    FIRMWARE_REVISION_2,
    FIRMWARE_REVISION_1,
    FIRMWARE_REVISION_0,
    FINGERS,
    KEYS_REG,
    XY0_COORD_H,
    X0_COORD_L,
    Y0_COORD_L,
    I2C_PROTOCOL = 0x3E,
    MAX_NUM_TOUCHES,
    DATA_0_HIGH,
    DATA_0_LOW,
    MISC_Info = 0xF0,
    MISC_CONTROL = 0xF1,
    SMART_WAKE_UP_REG = 0xF2,
    CHIP_ID = 0xF4,
    XY_CHS = 0xF5,
    PAGE_REG = 0xff,
} RegisterOffset;

#define X_RES_H_SHFT 4
#define X_RES_H_BMSK 0xf
#define Y_RES_H_SHFT 0
#define Y_RES_H_BMSK 0xf



uint8_t ST_u8GetConState(void);
uint8_t ST_vGetTouchInfo(uint32_t *px, uint32_t *py, uint32_t *state);
uint8_t STTouch_vInit(void);






#endif

#ifdef __cplusplus
}
#endif

#endif /* APP_AHMI_TOUCH_STTOUCH_H_ */

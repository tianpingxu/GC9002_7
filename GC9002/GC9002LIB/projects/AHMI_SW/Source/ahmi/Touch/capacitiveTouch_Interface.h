/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-07-05     Administrator       the first version
 */
#ifndef CAPACITIVETOUCH_INTERFACE_H_
#define CAPACITIVETOUCH_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/* The following macro definition can only take effect once at a time ! */
//#define    TOUCH_IC_CST820
//#define    TOUCH_IC_CST836U
#define    TOUCH_IC_GT9XX
//#define      TOUCH_IC_ST



#define    TOUCH_STATE_RELEASE  0
#define    TOUCH_STATE_PRESS    1

#define    TOUCH_ERR_CODE_OK            0
#define    TOUCH_ERR_CODE_NO_CONNECT    1


struct capacitive_touch_interface
{
    uint8_t (*init)(void); /*初始化*/
    uint8_t (*get_connect_state)(void);/*获取通讯状态查看是否正常连接触摸屏*/
    void (*press_callback)(void);/*手指按下回调函数*/
    void (*release_callback)(void);/*手指松开回调函数*/
    uint8_t (*get_touch_info)(uint32_t *px, uint32_t *py, uint32_t *state);/*获取触摸信息*/
    void (*additional_touch_info_process)(uint32_t *px, uint32_t *py);/*对获取的触摸坐标进行处理比如说对需要查黑的屏*/
    void(*err_process)(uint32_t err_code);/*错误处理*/
};

typedef struct capacitive_touch_interface* capacitive_touch_interface_t;

void Capacitivetouch_vInit(void);
void Capacitivetouch_vMain(void);
//extern capacitive_touch_interface_t pCapacitiveTouch;

#ifdef __cplusplus
}
#endif
#endif /* CAPACITIVETOUCH_INTERFACE_H_ */

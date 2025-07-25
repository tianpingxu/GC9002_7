/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-07-05     Administrator       the first version
 */
#include "cst836u.h"
#include "cst820.h"
#include "gt9xx.h"
#include "STTouch.h"
#include "capacitiveTouch_Interface.h"
#include "AHMITouch.h"

struct capacitive_touch_interface capacitive_touch;
capacitive_touch_interface_t pCapacitiveTouch = &capacitive_touch;

/*初始化
返回 1：初始化成功  0：初始化失败
*/
uint8_t capacitive_touch_interface_init(void)
{
    uint8_t res = 0;

#if defined(TOUCH_IC_CST836U)
    res = CST836U_vInit();
#elif defined(TOUCH_IC_CST820)
    res = CST820_vInit();
#elif defined(TOUCH_IC_GT9XX)
    int32_t ret;

    ret = GTP_Init_Panel();

    if (ret == 0)
    {
        res = 1;
    }
#elif defined(TOUCH_IC_ST)
    //res = STTouch_vInit();
#endif

    return res;
}

/*获取通讯状态查看是否正常连接触摸屏*/
uint8_t capacitive_touch_interface_get_connect_state(void)
{
    uint8_t con_state = 0;

#if defined(TOUCH_IC_CST836U)
    con_state = CST836U_u8GetConState();
#elif defined(TOUCH_IC_CST820)
    con_state = CST820_u8GetConState();
#elif defined(TOUCH_IC_GT9XX)

#elif defined(TOUCH_IC_ST)
    con_state = ST_u8GetConState();
#endif

    return con_state;
}

/*手指按下回调函数*/
void capacitive_touch_interface_press_callback(void)
{


}

/*手指松开回调函数*/
void capacitive_touch_interface_release_callback(void)
{


}

/*获取触摸信息
返回 1：获取成功  0：获取失败
 */
uint8_t capacitive_touch_interface_get_touch_info(uint32_t *px, uint32_t *py, uint32_t *state)
{
    uint8_t res = 0;

#if defined(TOUCH_IC_CST836U)
    res = CST836U_vGetTouchInfo(px, py, state);
#elif defined(TOUCH_IC_CST820)
    res = CST820_vGetTouchInfo(px, py, state);
#elif defined(TOUCH_IC_GT9XX)
    res = GT9xxTouch_vMain(px, py, state);
#elif defined(TOUCH_IC_ST)
    //res = ST_vGetTouchInfo(px, py, state);
#endif

    return res;
}

/*对获取的触摸坐标进行额外处理
返回
 */
void capacitive_touch_interface_additional_touch_info_process(uint32_t *px, uint32_t *py)
{
    uint8_t res = 0;
#if defined(TOUCH_IC_CST836U)

#elif defined(TOUCH_IC_CST836U)

#elif defined(TOUCH_IC_GT9XX)
	//*px = *px + 106;
#elif defined(TOUCH_IC_ST)
    *px = *px + 60;
#endif
}

void capacitive_touch_interface_err_process(uint32_t err_code)
{
#if defined(TOUCH_IC_CST836U)
    switch(err_code)
    {
        case TOUCH_ERR_CODE_NO_CONNECT:
            break;
        default:
            break;
    }
#elif defined(TOUCH_IC_CST820)
    switch(err_code)
    {
        case TOUCH_ERR_CODE_NO_CONNECT:
            break;
        default:
            break;
    }
#elif defined(TOUCH_IC_GT9XX)
    switch(err_code)
    {
        case TOUCH_ERR_CODE_NO_CONNECT:
            break;
        default:
            break;
    }
#elif defined(TOUCH_ST)
    switch(err_code)
    {
        case TOUCH_ERR_CODE_NO_CONNECT:
            break;
        default:
            break;
    }
#endif
}

void Capacitivetouch_vInit(void)
{
    pCapacitiveTouch->init = capacitive_touch_interface_init;
    pCapacitiveTouch->get_connect_state = capacitive_touch_interface_get_connect_state;
    pCapacitiveTouch->press_callback = capacitive_touch_interface_press_callback;
    pCapacitiveTouch->release_callback = capacitive_touch_interface_release_callback;
    pCapacitiveTouch->get_touch_info = capacitive_touch_interface_get_touch_info;
    pCapacitiveTouch->additional_touch_info_process = capacitive_touch_interface_additional_touch_info_process;
    pCapacitiveTouch->err_process = capacitive_touch_interface_err_process;
	
	/*  Configure touch screen for CapacitiveTouch */
	if (pCapacitiveTouch->init)
	{
		if (pCapacitiveTouch->init() == 0)
		{
			if (pCapacitiveTouch->err_process)
			{
				pCapacitiveTouch->err_process(TOUCH_ERR_CODE_NO_CONNECT);
			}
		}
	}
}

extern TouchFSM g_TM_stTouchFsm;
void Capacitivetouch_vMain(void)
{
	uint32_t x,y,state;
	
	if (pCapacitiveTouch->get_touch_info)
	{
		pCapacitiveTouch->get_touch_info(&x, &y, &state);

		if (pCapacitiveTouch->additional_touch_info_process)
		{
			pCapacitiveTouch->additional_touch_info_process(&x, &y);
		}

		g_TM_stTouchFsm.mPosi.pos_x = x;
		g_TM_stTouchFsm.mPosi.pos_y = y;

		if (state == TOUCH_STATE_PRESS)
		{
			g_TM_stTouchFsm.mPosi.event = TOUCHED;

			if (pCapacitiveTouch->press_callback)
			{
				pCapacitiveTouch->press_callback();
			}
		}
		else
		{
			g_TM_stTouchFsm.mPosi.event = NOTOUCH;

			if(pCapacitiveTouch->release_callback)
			{
				pCapacitiveTouch->release_callback();
			}
		}
	}
}





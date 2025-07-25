/**
  ******************************************************************************
  * @file    gc90x_sharememory.c
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    12-April-2019
  * @brief   This file provides all the ShareMemory firmware functions.
  ******************************************************************************
	*/
	
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
//#include "CPU1_CMSDK_CM0.h"
#include "gc90x_shmemmanage.h"
#include "AHMIInterface.h"
#include "publicDefine.h"
#include "gc90x_initial.h"
//#include "shmem_touch.h"
#include "read_reg.h"

/**
  * @brief  Prepare Data to be send.
  * @param  OPCode: Operation code number
  *        This parameter can be: READWRITE or To be expanded.
	* @param  DataNumber: Number of data transferred
	* @param  *pID: An array pointing to the ID
	* @param  *pData: An array pointing to data corresponding ID
	* @retval None
  */


uint32_t ShareMemory_SendDataFrame(uint32_t response, uint32_t DataNumber, SendValueType* pData, uint32_t wait, uint32_t sendFramecbfunc, uint32_t responsecbfunc)
{
   printf("pData->mID = %d\r\n", pData->mID);

    switch(pData->mID) {
        // dock栏处理
        case DOCK_HOME_REG:
            dock_home_handle();
            break;
        case DOCK_CLOCK_REG:
            dock_clock_handle();
            break;
        case DOCK_LIGHT_REG:
            dock_light_handle();
            break;
        case DOCK_FAN_REG:
            dock_fan_handle();
            break;  
        case DOCK_SANITATION_REG:
            dock_sanitation_handle();
            break;
        case DOCK_ION_REG:
            dock_ion_handle();
            break;
        case DOCK_CURTAIN_REG:
            dock_curtain_handle();
            break;
        case DOCK_SET_REG:
            dock_set_handle();
            break;
        case UP_WINDOW_REG:
            up_window_handle();
            break;
        //倒计时处理
        case COUNTDOWN_START_REG:
            countdown_start();
            break;
        case COUNTDOWN_STOP_REG:
            countdown_stop();
            break;
        case COUNTDOWN_END_REG:
            countdown_end();
            break;
        //模式处理
        case SENCE_MEET_MODE_REG:
            sence_mode_meet();
            break;
        case SENCE_PHONE_MODE_REG:
            sence_mode_phone();
            break;
        case SENCE_WORK_MODE_REG:
            sence_mode_work();
            break;
        case SENCE_RELAX_MODE_REG:
            sence_mode_relax();
            break;
        //密码处理
        case KEYBOARD_O_REG:
            password_handle(0);
            break;
        case KEYBOARD_1_REG:
            password_handle(1);
            break;
        case KEYBOARD_2_REG:
            password_handle(2);
            break;
        case KEYBOARD_3_REG:
            password_handle(3);
            break;
        case KEYBOARD_4_REG:
            password_handle(4);
            break;
        case KEYBOARD_5_REG:
            password_handle(5);
            break;
        case KEYBOARD_6_REG:
            password_handle(6);
            break;
        case KEYBOARD_7_REG:
            password_handle(7);
            break;
        case KEYBOARD_8_REG:
            password_handle(8);
            break;
        case KEYBOARD_9_REG:
            password_handle(9);
            break;
        case KEYBOARD_DEL_REG:
            password_handle(10);
            break;
        case KEYBOARD_BACK_REG:
            password_back_handle();
            break;
        case BTN_BACK_REG:
            set_back_handle();
            break;
        // 设置处理
        case SET_DEFAULT_REG:
            set_default_handle();
            break;
        case SET_LANGUAGE_REG:
            set_language_handle();
            break;
        case SET_DEV_INFO_REG:
            set_dev_info_handle();
            break;
        case SET_DEV_RODAR_REG:
            set_dev_rodar_handle();
            break;
        case SET_AUTO_VENT_REG:
            set_auto_vent_handle();
            break;
        // 参数保存
        case PARAME_SET_SAVE_YES_REG:
            parame_change_save(1);
            break;
        case PARAME_SET_SAVE_NO_REG:
            parame_change_save(0);
            break;
        // 语言处理
        case LANGUAGE_ENGLISH_REG:
            language_choose_handle(0);
            break;
        case LANGUAGE_CHINESE_REG:
            language_choose_handle(1);
            break;
        // 通风处理
        case AUTO_VENT_ONOFF_REG:
            auto_onoff_handle();
            break;
        case SET_SCREEN_OFF_REG:
            screen_onoff_handle();
            break;
        default:
            break;
    }

   return 0;
}

/**
  * @brief  Prepare Log Data to be send.
  * @param  OPCode: Operation code number
  *        This parameter can be: READWRITE or To be expanded.
	* @param  DataNumber: Number of data transferred
	* @param  *pID: An array pointing to the ID
	* @param  *pData: An array pointing to data corresponding ID
	* @retval None
  */
uint32_t ShareMemory_SendLogFrame(uint32_t response, uint32_t DataNumber, SendValueType* pData, uint32_t wait, uint32_t sendFramecbfunc, uint32_t responsecbfunc)
{
    return 0;
}

/**
  * @brief  Prepare Data to be send.
  * @param  OPCode: Operation code string
  *        This parameter can be: READWRITE or To be expanded.
	* @param  DataNumber: Number of data transferred
	* @param  *pID: An array pointing to the ID
	* @param  *pData: An array pointing to data corresponding ID
	* @retval None
  */
uint32_t ShareMemory_SendStringFrame(uint32_t response, uint32_t ID, uint32_t DataNumber, uint8_t* pData, uint32_t wait, uint32_t sendFramecbfunc, uint32_t responsecbfunc)
{
    return 0;
}


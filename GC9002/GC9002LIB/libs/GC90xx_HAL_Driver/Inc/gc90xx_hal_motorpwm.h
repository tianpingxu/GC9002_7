/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-08-15     huyt       the first version
 */
#ifndef __GC90XX_HAL_MOTORPWM_H__
#define __GC90XX_HAL_MOTORPWM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <gc90xx_hal_def.h>
#ifdef HAL_MOTORPWM_MODULE_ENABLED

enum motor_pres
{
    motor_pwm_pres1 = 0,
    motor_pwm_pres2,
    motor_pwm_pres4,
    motor_pwm_pres8,
};
enum motor_output_mode
{
    Half_H_bridge_mode_MnCxM = 0,
    Half_H_bridge_mode_MnCxP,
    Full_H_bridge_mode,
    Dual_Full_H_bridge_mode,
};
enum pwm_align_mode
{
    PWM_Channel_disabled = 0,
    PWM_Left_aligned,
    PWM_Right_aligned,
    PWM_Center_aligned,
};


#endif /* HAL_MOTORPWM_MODULE_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __GC90XX_HAL_MOTORPWM_H__ */

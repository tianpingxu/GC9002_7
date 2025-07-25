/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_pwm.h
 * @brief    header File for PWM Driver
 * @version  V1.0
 * @date     2023/02/27
 * @model    pwm
 ******************************************************************************/
#ifndef __CSI_PWM_H__
#define __CSI_PWM_H__

#include <csi_common.h>
#include <csi_irq.h>
#if(defined CONFIG_CSI_DRV_MOTOR_PWM && CONFIG_CSI_DRV_MOTOR_PWM == 1U)
#include <csi_motor.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_PWM_ENABLED

#if(defined CONFIG_CSI_DRV_MOTOR_PWM && CONFIG_CSI_DRV_MOTOR_PWM == 1U)
#define CSI_MOTOR_PWM_CNT       MOTOR_NUM_CHANNELS // Peripheral motor count
#endif
#define CSI_PWM_CNT             2 // Peripheral timer count

#define CSI_PWM_PERIOD_MIN      2
#define CSI_PWM_PULSE_WIDTH_MIN 1
enum CSI_Motor_PWM_Idx
{
    CSI_MOTOR_PWM_IDX0 = 2U,
    CSI_MOTOR_PWM_IDX1,
    CSI_MOTOR_PWM_IDX2,
    CSI_MOTOR_PWM_IDX3,
};


#define CSI_IS_PWM_CHANNEL(CHANNEL)     ((CHANNEL) > 0 && (CHANNEL) <= 4)
#define CSI_MOTOR_PWM_CH_MAX            (0xFF)

typedef enum
{
    PWM_POLARITY_HIGH,  //高电平
    PWM_POLARITY_LOW,   //低电平
}csi_pwm_polarity_t;
typedef enum
{
    PWM_CAPTURE_POLARITY_POSEDGE,   //上升沿捕获类型
    PWM_CAPTURE_POLARITY_NEGEDGE,   //下降沿捕获类型
    PWM_CAPTURE_POLARITY_BOTHEDGE,  //边沿捕获类型
}csi_pwm_capture_polarity_t;
typedef enum
{
    PWM_EVENT_CAPTURE_POSEDGE,      //上升沿事件类型
    PWM_EVENT_CAPTURE_NEGEDGE,      //下降沿事件类型
    PWM_EVENT_CAPTURE_BOTHEDGE,     //边沿事件类型
    PWM_EVENT_ERROR,                //错误事件类型
}csi_pwm_event_t;

typedef struct _csi_pwm
{
    csi_dev_t dev;      //csi Device unity handle
    void (*callback)(struct _csi_pwm *pwm, csi_pwm_event_t event, uint32_t ch, uint32_t time_us, void *arg);//User callback function
    void *arg;          //Callback function argument
    uint32_t cnt;
    uint32_t count;
    csi_pwm_capture_polarity_t polarity;
    void *priv;         //Device private variable
    uint8_t motor_pwm_ch;
}csi_pwm_t;

csi_error_t csi_pwm_init(csi_pwm_t *pwm, uint32_t idx);
void        csi_pwm_uninit(csi_pwm_t *pwm);
csi_error_t csi_pwm_out_config(csi_pwm_t *pwm,
                               uint32_t channel,
                               uint32_t period_us,
                               uint32_t pulse_width_us,
                               csi_pwm_polarity_t polarity);
csi_error_t csi_pwm_out_start(csi_pwm_t *pwm, uint32_t channel);
void        csi_pwm_out_stop(csi_pwm_t *pwm, uint32_t channel);
csi_error_t csi_pwm_capture_config(csi_pwm_t *pwm,
                                   uint32_t channel,
                                   csi_pwm_capture_polarity_t polarity,
                                   uint32_t freq,
                                   uint32_t count);
csi_error_t csi_pwm_capture_start(csi_pwm_t *pwm, uint32_t channel);
void        csi_pwm_capture_stop(csi_pwm_t *pwm, uint32_t channel);
csi_error_t csi_pwm_attach_callback(csi_pwm_t *pwm, void(*callback)(struct _csi_pwm *pwm, csi_pwm_event_t event, uint32_t ch, uint32_t time_us, void *arg), void *arg);
void        csi_pwm_detach_callback(csi_pwm_t *pwm);
bool        csi_pwm_get_status(csi_pwm_t *pwm, uint32_t channel);

#endif /* CONFIG_CSI_DRV_PWM_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_PWM_H__ */
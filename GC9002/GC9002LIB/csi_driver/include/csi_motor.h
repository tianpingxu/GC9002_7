/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     csi_motor.h
 * @brief    header file for motor driver
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
#ifndef __CSI_MOTOR_H
#define __CSI_MOTOR_H

#include <csi_common.h>
#include <csi_irq.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CSI_MOTOR_CNT          	MOTOR_NUM_CHANNELS // Peripheral motor channel count
#define MOTOR_NUM_CHANNELS		4
#define IS_MOTOR_CHANNEL(CHX)   ((CHX) <= (MOTOR_NUM_CHANNELS-1))
#define FREQ_TO_CYCLE(freq) 	(HAL_RCC_GetAHBClock()/freq)

typedef enum
{
    MOTOR_EVENT_DONE,      	//运动完成
    MOTOR_EVENT_WARNING,  	//
    MOTOR_EVENT_DIV_BY_0,	//除0
    MOTOR_EVENT_TIME_END,	//
}csi_motor_event_t;

typedef enum
{
    MOTOR_MODE_H_STATIC,  	//表示在一个 PWM 的电平变化周期内，高电平宽度恒定，持续时间为 ST_VALUE 个tick；低电平的持续时间由积分器算出
    MOTOR_MODE_L_STATIC,  	//表示在一个 PWM 的电平变化周期内，低电平宽度恒定，持续时间为 ST_VALUE 个tick；高电平的持续时间由积分器算出
    MOTOR_MODE_TOT_STATIC,	//表示 PWM 的电平变化周期数恒定为 ST_VALUE 个 tick；高电平的持续时间由积分器算出
    MOTOR_MODE_DUTY_STATIC,	//表示 PWM 的占空恒为 50%‐50%，此时 ST_VALUE 无效；高低电平的持续时间均由积分器算出
}csi_motor_mode_t;

/** @defgroup MOTOR_Channel_selection MOTOR Channel selection
  * @brief    MOTOR channel selection
  */
typedef enum
{
    MOTOR_CHANNEL0 = 0x00UL,    /*!< MOTOR Channel 1 */
    MOTOR_CHANNEL1 = 0x01UL,    /*!< MOTOR Channel 2 */
    MOTOR_CHANNEL2 = 0x02UL,    /*!< MOTOR Channel 3 */
    MOTOR_CHANNEL3 = 0x03UL,    /*!< MOTOR Channel 4 */
}MOTOR_ChannelType;

typedef struct _csi_motor
{
    csi_dev_t dev;      //csi Device unity handle
    void (*callback)(struct _csi_motor *motor, csi_motor_event_t event, void *arg);//User callback function
    void *arg;          //Callback function argument
    bool autoreload;
    void *priv;         //Device private variable
}csi_motor_t;

typedef struct {
	 bool new_cmd;
	 bool data_ready;
	 uint8_t cur_stage;
	 uint8_t next_stage;
} csi_motor_status;

extern bool csi_motor_chx_init_S(MOTOR_ChannelType chx, uint32_t start_speed, uint32_t stop_speed, uint32_t total_step);
extern bool csi_motor_chx_init_T(MOTOR_ChannelType chx, uint32_t start_speed, uint32_t stop_speed, uint32_t total_step);
extern bool csi_motor_chx_init_L(MOTOR_ChannelType chx, uint32_t speed, uint32_t total_step);
extern void csi_motor_intr_handle(void);
extern void csi_motor_chx_run(MOTOR_ChannelType chx);
extern void csi_motor_chx_stop(MOTOR_ChannelType chx);
extern void csi_motor_chx_reset(MOTOR_ChannelType chx);

csi_error_t csi_motor_pwm_chx_init(MOTOR_ChannelType chx, uint32_t speed, csi_motor_mode_t mode, uint32_t st_value, uint32_t warn_value, bool intr_en);
csi_error_t csi_motor_pwm_chx_config(MOTOR_ChannelType chx, uint32_t period_us, uint32_t pulse_high_us, uint32_t flag);
extern void csi_motor_pwm_chx_start(MOTOR_ChannelType chx);
extern void csi_motor_pwm_chx_stop(MOTOR_ChannelType chx);
extern bool csi_motor_pwm_chx_get_status(MOTOR_ChannelType chx);

extern void csi_motor_timer_chx_config(MOTOR_ChannelType chx, uint32_t timeout_us);

extern csi_motor_status motor_status[4];



csi_error_t csi_motor_init(csi_motor_t *motor, uint32_t idx);
void 		csi_motor_uninit(csi_motor_t *motor);
csi_error_t csi_motor_timer_start(csi_motor_t *motor, uint32_t timeout_us);
csi_error_t csi_motor_timer_start_autoreload(csi_motor_t *motor, uint32_t timeout_us);
void 		csi_motor_timer_stop(csi_motor_t *motor);
csi_error_t csi_motor_timer_attach_callback(csi_motor_t *motor, void(*callback)(struct _csi_motor *motor, csi_motor_event_t event, void *arg), void *arg);
void 		csi_motor_timer_detach_callback(csi_motor_t *motor);


#ifdef __cplusplus
}
#endif

#endif /* __CSI_MOTOR_H */


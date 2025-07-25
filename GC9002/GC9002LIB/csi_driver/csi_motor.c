/*
 * Copyright (C) 2022-2019 Suxianwei Group Holding Limited
 */


/******************************************************************************
 * @file     csi_motor.c
 * @brief    CSI Source File for motor Driver
 * @version  V1.0
 * @date     12. Dec 2022
 ******************************************************************************/

#include <csi_config.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <csi_irq.h>
#include <soc.h>
#include <csi_core.h>
#include <csi_motor.h>
#include "gc90xx_hal.h"

csi_motor_status motor_status[4];
uint32_t motor_num_s=0;
uint32_t motor_num_e=3;

static float getTotalTime(int32_t startSpeed, int32_t endSpeed, int32_t totalStep) 
{
	float totalTime = 0.0;
	totalTime = ((2.0 * (float)totalStep) / ((float)(startSpeed + endSpeed)));
	return totalTime;
}
static bool getTotalTickBits(float total_Time, uint8_t *total_tick_bits, uint16_t *tick_div) 
{
	uint32_t total_sys_tick = 0;
	float ttick_div = 0.0;
	bool result = false;
	uint8_t bits = 0;
	total_sys_tick = HAL_RCC_GetAHBClock() * total_Time;
	for(bits = 3; bits >= 0; bits--)
	{
		ttick_div = (float)(total_sys_tick/(pow(2, (bits*2+14))));
		if((32.0 <= ttick_div) && (ttick_div <= 65535.0))
		{
			*total_tick_bits = bits;
			*tick_div = (uint16_t)ttick_div;
			return true;
		}	
	}
	// printf("Parameter totalTime:%f error", total_Time);
	return false;
}

//--------------Timer start-----------------
static csi_motor_t *s_obj_arr[MOTOR_NUM_CHANNELS];
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_MOTOR_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_MOTOR_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    uint32_t tempisr = 0;
	tempisr = MOTOR->ISR.reg_u32val;
	for(int i = 0; i < MOTOR_NUM_CHANNELS; i++)
	{
		if(tempisr & (0x2 << (4*i)))
		{
			if(s_obj_arr[i])
			{
				if(s_obj_arr[i]->callback)
				{
					s_obj_arr[i]->callback(s_obj_arr[i], MOTOR_EVENT_WARNING, s_obj_arr[i]->arg);
				}
				if(s_obj_arr[i]->autoreload == false)
				{
					csi_motor_timer_stop(s_obj_arr[i]);
				}
			}
		}
	}
    MOTOR->ISR.reg_u32val = 0;

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
static uint8_t irq_register_flag = 0;
void csi_motor_mem_init(void)
{
	for(int i = 0; i < MOTOR_NUM_CHANNELS; i++)
	{
		s_obj_arr[i] = 0;
	}
}
csi_error_t csi_motor_init(csi_motor_t *motor, uint32_t idx)
{
    if(motor == NULL || idx >= CSI_MOTOR_CNT)
    {
        return CSI_ERROR;
    }
	HANDLE_REG_BASE(motor) = MOTOR_BASE;
	HANDLE_IRQ_NUM(motor) = MOTOR_IRQn;
	HANDLE_DEV_IDX(motor) = idx;
	HANDLE_IRQ_HANDLER(motor) = (void *)CSI_MOTOR_IRQHandler;
    motor->priv = s_obj_arr;
	s_obj_arr[idx] = motor;
	if(irq_register_flag == 0)
	{
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
		csi_clic_register_irq(HANDLE_IRQ_NUM(motor), CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, HANDLE_IRQ_HANDLER(motor));
#else
		csi_clic_register_irq(HANDLE_IRQ_NUM(motor), CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, HANDLE_IRQ_HANDLER(motor));
#endif
		irq_register_flag = 1;
	}
    return CSI_OK;
}
void csi_motor_uninit(csi_motor_t *motor)
{
    if(motor == NULL || HANDLE_DEV_IDX(motor) >= CSI_MOTOR_CNT)
    {
        return;
    }
	s_obj_arr[HANDLE_DEV_IDX(motor)] = NULL;
    HANDLE_REG_BASE(motor) = 0;
    HANDLE_IRQ_NUM(motor) = 0;
    HANDLE_DEV_IDX(motor) = 0;
    HANDLE_IRQ_HANDLER(motor) = NULL;
    motor->priv = NULL;
}
static csi_error_t motor_timer_start(csi_motor_t *motor, uint32_t timeout_us, bool autoreload)
{
    if(motor == NULL || HANDLE_DEV_IDX(motor) >= CSI_MOTOR_CNT)
    {
        return CSI_ERROR;
    }
	csi_error_t ret = CSI_OK;
	uint32_t warn_value = 0;
	uint32_t speed = 0;
	uint32_t st_value = 0;
	uint32_t period_us = 2;
	uint32_t pulse_high_us = 1;

	speed = 1000000/pulse_high_us;
	st_value = (HAL_RCC_GetAHBClock()/1000000)*period_us;
	warn_value = (timeout_us/2) + (timeout_us%2);
	warn_value = (warn_value > 0 ? warn_value : 1);
	ret = csi_motor_pwm_chx_init(HANDLE_DEV_IDX(motor), speed, MOTOR_MODE_TOT_STATIC, st_value, warn_value, true);
	if(ret != CSI_OK)
	{
		return ret;
	}
	motor->autoreload = autoreload;
	csi_motor_pwm_chx_start(HANDLE_DEV_IDX(motor));
    return CSI_OK;
}
csi_error_t csi_motor_timer_start(csi_motor_t *motor, uint32_t timeout_us)
{
    if(motor == NULL || HANDLE_DEV_IDX(motor) >= CSI_MOTOR_CNT)
    {
        return CSI_ERROR;
    }
	return motor_timer_start(motor, timeout_us, false);
}
csi_error_t csi_motor_timer_start_autoreload(csi_motor_t *motor, uint32_t timeout_us)
{
    if(motor == NULL || HANDLE_DEV_IDX(motor) >= CSI_MOTOR_CNT)
    {
        return CSI_ERROR;
    }
	return motor_timer_start(motor, timeout_us, true);
}
void csi_motor_timer_stop(csi_motor_t *motor)
{
    if(motor == NULL || HANDLE_DEV_IDX(motor) >= CSI_MOTOR_CNT)
    {
        return;
    }
	motor->autoreload = false;
	csi_motor_pwm_chx_stop(HANDLE_DEV_IDX(motor));
}
csi_error_t csi_motor_timer_attach_callback(csi_motor_t *motor, void(*callback)(struct _csi_motor *motor, csi_motor_event_t event, void *arg), void *arg)
{
    if(motor == NULL)
    {
        return CSI_ERROR;
    }
    motor->callback = callback;
    motor->arg = arg;
    return CSI_OK;
}
void csi_motor_timer_detach_callback(csi_motor_t *motor)
{
    if(motor == NULL)
    {
        return;
    }
    motor->callback = NULL;
    motor->arg = NULL;
}
//--------------Timer end-----------------

//--------------PWM start-----------------
csi_error_t csi_motor_pwm_chx_config(MOTOR_ChannelType chx, uint32_t period_us, uint32_t pulse_high_us, uint32_t flag)
{
	uint32_t speed = 0;
	uint32_t st_value = 0;
	uint32_t pulse_high_us_in = pulse_high_us;

	if(flag == 0)
	{
		pulse_high_us = period_us - pulse_high_us;
	}
	if(pulse_high_us > 1000000)
	{
		pulse_high_us = 1000000;
	}
	if(period_us == 1 && pulse_high_us)
	{
		return false;
	}
	if(pulse_high_us != 0)
	{
		speed = 1000000/pulse_high_us;
	}
	else
	{
		speed = 1000000;
	}
	st_value = (HAL_RCC_GetAHBClock()/1000000)*period_us;
	if(period_us == pulse_high_us_in || pulse_high_us_in == 0)
	{
		if(flag != 0) {
			return csi_motor_pwm_chx_init(chx, speed, MOTOR_MODE_H_STATIC, 0, 1000, false);
		} else {
			return csi_motor_pwm_chx_init(chx, speed, MOTOR_MODE_L_STATIC, 0, 1000, false);
		}
	}
	//高电平持续时间 1/speed s
	//周期 st_value 个tick；
	return csi_motor_pwm_chx_init(chx, speed, MOTOR_MODE_TOT_STATIC, st_value, 1000, false);
}
csi_error_t csi_motor_pwm_chx_init(MOTOR_ChannelType chx, uint32_t speed, csi_motor_mode_t mode, uint32_t st_value, uint32_t warn_value, bool intr_en)
{
	float total_time = 0.0;
	uint32_t tempreg = 0;
	uint32_t us_tick;
	uint32_t h_sum_tick;
	uint16_t tick_div;
	uint8_t total_tick_bits;
	
	assert_param(IS_MOTOR_CHANNEL(chx));
	
	MOTOR->SYS_TICK = HAL_RCC_GetAHBClock();
	//根据总步数，计算并设置改变速度总次数total_tick和每步要花费的系统时钟个数tick_div
	total_time = getTotalTime(speed, speed, 2*speed);
	if(getTotalTickBits(total_time, &total_tick_bits, &tick_div) != true)
	{
		return CSI_ERROR;
	}
	//设置pwm模式
	//00： H_STATIC，表示 PWM 的高电平宽度恒定，持续时间为 ST_VALUE 个 tick；
	//01： L_STATIC，表示 PWM 的低电平宽度恒定，持续时间为 ST_VALUE 个 tick；
	//10： TOT_STATIC，表示 PWM 的总周期数恒定为 ST_VALUE 个 tick；
	//11： DUTY_STATIC，表示 PWM 的占空恒为 50%。
	tempreg = 0;
	((union _MOT_CTRL *)&tempreg)->bit.mode = mode;
	((union _MOT_CTRL *)&tempreg)->bit.auto_stop = 0;
	((union _MOT_CTRL *)&tempreg)->bit.auto_restart = 1;
	((union _MOT_CTRL *)&tempreg)->bit.CH_enable = 0;
	((union _MOT_CTRL *)&tempreg)->bit.total_tick = total_tick_bits;
	MOTOR->CH[chx].CTRL.reg_u32val = tempreg;
	MOTOR->CH[chx].TICK_DIV = tick_div;
	MOTOR->CH[chx].ST_VALUE = st_value;
	MOTOR->CH[chx].Y0 = speed;
	MOTOR->CH[chx].Y1 = speed;
	MOTOR->CH[chx].DY0 = 0;
	MOTOR->CH[chx].D2Y0 = 0;
	MOTOR->CH[chx].D3Y = 0;
	MOTOR->CH[chx].WARN_STEP_REQ = warn_value;
	MOTOR->CH[chx].TOTAL_STEP_REQ = 2*speed;
	//motor_status[chx].data_ready=true;
	if(intr_en)
	{
		MOTOR->ISR.reg_u32val &= ~(0x2 << (chx*4));
		MOTOR->IER.reg_u32val |= (0x2 << (chx*4));
	}
	return CSI_OK;
}

void csi_motor_pwm_chx_start(MOTOR_ChannelType chx)
{
	uint32_t tempreg = 0;
	tempreg = MOTOR->CH[chx].CTRL.reg_u32val;
	((union _MOT_CTRL *)&tempreg)->bit.CH_enable = 1;
	((union _MOT_CTRL *)&tempreg)->bit.CH_newcmd = 1;
	MOTOR->CH[chx].CTRL.reg_u32val = tempreg;
}
void csi_motor_pwm_chx_stop(MOTOR_ChannelType chx)
{
	uint32_t tempreg = 0;
	tempreg = MOTOR->CH[chx].CTRL.reg_u32val;
	((union _MOT_CTRL *)&tempreg)->bit.CH_enable = 0;
	((union _MOT_CTRL *)&tempreg)->bit.CH_newcmd = 0;
	MOTOR->CH[chx].CTRL.reg_u32val = tempreg;
	motor_status[chx].data_ready=false;
}
bool csi_motor_pwm_chx_get_status(MOTOR_ChannelType chx)
{
	return (MOTOR->CH[chx].CTRL.bit.CH_enable == 1);
}

//--------------PWM end-----------------



bool csi_motor_chx_init_S(MOTOR_ChannelType chx,uint32_t start_speed,uint32_t stop_speed,uint32_t total_step)
{
	float total_time = 0.0;
	uint32_t tempreg = 0;
	uint8_t total_tick_bits;
	uint16_t tick_div;

	//根据总步数，计算并设置改变速度总次数total_tick和每步要花费的系统时钟个数tick_div
	total_time = getTotalTime(start_speed,stop_speed,total_step);
	if(getTotalTickBits(total_time, &total_tick_bits, &tick_div) != true)
	{
		return CSI_ERROR;
	}
	//设置pwm模式
	tempreg = MOTOR->CH[chx].CTRL.reg_u32val;
	((union _MOT_CTRL *)&tempreg)->bit.mode = MOTOR_MODE_DUTY_STATIC;
	((union _MOT_CTRL *)&tempreg)->bit.auto_stop = 1;
	((union _MOT_CTRL *)&tempreg)->bit.auto_restart = 1;
	((union _MOT_CTRL *)&tempreg)->bit.CH_enable = 1;
	((union _MOT_CTRL *)&tempreg)->bit.total_tick = total_tick_bits;
	MOTOR->CH[chx].CTRL.reg_u32val = tempreg;
	MOTOR->CH[chx].TICK_DIV = tick_div;
	MOTOR->CH[chx].Y0 = start_speed;
	MOTOR->CH[chx].Y1 = stop_speed;
	MOTOR->CH[chx].DY0 = 0;
	MOTOR->CH[chx].D2Y0 = 6*(stop_speed - start_speed);
	MOTOR->CH[chx].D3Y = 12*(start_speed - stop_speed);
	MOTOR->CH[chx].WARN_STEP_REQ = 1000;
	MOTOR->CH[chx].TOTAL_STEP_REQ = total_step;
	//motor_status[chx].data_ready=true;
	
	//启动motor中断
	MOTOR->ISR.reg_u32val &= ~(0xF << (chx*4));
	MOTOR->IER.reg_u32val |= (0xD << (chx*4));	
	return true;
}
bool csi_motor_chx_init_T(MOTOR_ChannelType chx,uint32_t start_speed,uint32_t stop_speed,uint32_t total_step)
{
	uint32_t tempreg = 0;
	float total_time = 0.0;
	uint8_t total_tick_bits;
	uint16_t tick_div;
	//根据总步数，计算并设置改变速度总次数total_tick和每步要花费的系统时钟个数tick_div
	total_time = getTotalTime(start_speed, stop_speed, total_step);
	if(getTotalTickBits(total_time, &total_tick_bits, &tick_div) != true)
	{
		return CSI_ERROR;
	}

	//设置pwm模式
	tempreg = MOTOR->CH[chx].CTRL.reg_u32val;
	((union _MOT_CTRL *)&tempreg)->bit.mode = MOTOR_MODE_DUTY_STATIC;
	((union _MOT_CTRL *)&tempreg)->bit.auto_stop = 1;
	((union _MOT_CTRL *)&tempreg)->bit.auto_restart = 1;
	((union _MOT_CTRL *)&tempreg)->bit.CH_enable = 1;
	((union _MOT_CTRL *)&tempreg)->bit.total_tick = total_tick_bits;
	MOTOR->CH[chx].CTRL.reg_u32val = tempreg;
	MOTOR->CH[chx].TICK_DIV = tick_div;
	MOTOR->CH[chx].Y0 = start_speed;
	MOTOR->CH[chx].Y1 = stop_speed;
	MOTOR->CH[chx].DY0 = (stop_speed*stop_speed-start_speed*start_speed)/(2*total_step);
	MOTOR->CH[chx].D2Y0 = 0;
	MOTOR->CH[chx].D3Y = 0;
	MOTOR->CH[chx].WARN_STEP_REQ = 1000;
	MOTOR->CH[chx].TOTAL_STEP_REQ = total_step;
	//motor_status[chx].data_ready=true;
	
	//启动motor中断
	MOTOR->ISR.reg_u32val &= ~(0xF << (chx*4));
	MOTOR->IER.reg_u32val |= (0xF << (chx*4));	
	return true;
}
bool csi_motor_chx_init_L(MOTOR_ChannelType chx, uint32_t speed, uint32_t total_step)
{
	uint32_t tempreg = 0;
	float total_time = 0.0;
	uint8_t total_tick_bits;
	uint16_t tick_div;
	//根据总步数，计算并设置改变速度总次数total_tick和每步要花费的系统时钟个数tick_div
	total_time = getTotalTime(speed, speed, total_step);
	if(getTotalTickBits(total_time, &total_tick_bits, &tick_div) != true)
	{
		return CSI_ERROR;
	}

	//设置pwm模式
	tempreg = MOTOR->CH[chx].CTRL.reg_u32val;
	((union _MOT_CTRL *)&tempreg)->bit.mode = MOTOR_MODE_DUTY_STATIC;
	((union _MOT_CTRL *)&tempreg)->bit.auto_stop = 1;
	((union _MOT_CTRL *)&tempreg)->bit.auto_restart = 1;
	((union _MOT_CTRL *)&tempreg)->bit.CH_enable = 1;
	((union _MOT_CTRL *)&tempreg)->bit.total_tick = total_tick_bits;
	MOTOR->CH[chx].CTRL.reg_u32val = tempreg;
	MOTOR->CH[chx].TICK_DIV = tick_div;
	MOTOR->CH[chx].Y0 = speed;
	MOTOR->CH[chx].Y1 = speed;
	MOTOR->CH[chx].DY0 = 0;
	MOTOR->CH[chx].D2Y0 = 0;
	MOTOR->CH[chx].D3Y = 0;
	MOTOR->CH[chx].WARN_STEP_REQ = 1000;
	MOTOR->CH[chx].TOTAL_STEP_REQ = total_step;
	//motor_status[chx].data_ready=true;
	
	//启动motor中断
	MOTOR->ISR.reg_u32val &= ~(0xF << (chx*4));
	MOTOR->IER.reg_u32val |= (0xF << (chx*4));	
	return true;
}

void csi_motor_intr_handle(void)
{
	union _MOT_IR flag;
	uint8_t intr_flag_done,intr_flag_warn,intr_flag_div0,intr_flag_speed;
	
	flag.reg_u32val = MOTOR->ISR.reg_u32val;
	MOTOR->ISR.reg_u32val = 0;
	
	for(int i=motor_num_s; i <= motor_num_e; i++)
	{
		intr_flag_done = flag.reg_u32val >> (i*4+0);
		if(intr_flag_done&0x01)
		{
			if(motor_status[i].next_stage==12)
			{
				motor_status[i].cur_stage = 12;
				motor_status[i].next_stage=21;
				//csi_motor_chx_init_L(i,10000,10000);
				csi_motor_chx_init_S(i,10000,100,10000);
				
			}else if(motor_status[i].next_stage==21)
			{
				motor_status[i].cur_stage = 21;
				motor_status[i].next_stage=12;
				csi_motor_chx_init_S(i,100,10000,10000);
			}
			printf("ch%d done intr, cur_stage:%d, next_stage:%d\n",i,motor_status[i].cur_stage,motor_status[i].next_stage);
		}
		intr_flag_warn = flag.reg_u32val >> (i*4+1);
		if(intr_flag_warn&0x01)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11,!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11));
			printf("ch%d warn intr, step cnt:%d sum step cnt:%d\n", i, MOTOR->CH[i].WARN_STEP_CNT, MOTOR->CH[i].STEP_CNT);
		}
		intr_flag_div0 = flag.reg_u32val >> (i*4+2);
		if(intr_flag_div0&0x01)
		{
			printf("ch%d div 0 intr!\n",i);
		}
		intr_flag_speed = flag.reg_u32val >> (i*4+3);
		if(intr_flag_speed&0x01)
		{
			//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11,!HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11));
			printf("ch%d reach speed intr, step cnt:%d sum step cnt:%d\n", i, MOTOR->CH[i].WARN_STEP_CNT, MOTOR->CH[i].STEP_CNT);
		}
	}
}

void csi_motor_chx_run(MOTOR_ChannelType chx)
{
    uint32_t tempreg = 0;
	tempreg = MOTOR->CH[chx].CTRL.reg_u32val;
	((union _MOT_CTRL *)&tempreg)->bit.CH_enable = 1;
	((union _MOT_CTRL *)&tempreg)->bit.CH_newcmd = 1;
	MOTOR->CH[chx].CTRL.reg_u32val = tempreg;
}

void csi_motor_chx_stop(MOTOR_ChannelType chx)
{
	uint32_t tempreg = 0;
	//关闭motor中断
	MOTOR->IER.reg_u32val &= ~(0xF << (4*chx));
	tempreg = MOTOR->CH[chx].CTRL.reg_u32val;
	((union _MOT_CTRL *)&tempreg)->bit.CH_enable = 0;
	((union _MOT_CTRL *)&tempreg)->bit.CH_newcmd = 0;
	MOTOR->CH[chx].CTRL.reg_u32val = tempreg;
	motor_status[chx].data_ready = false;
}

void csi_motor_chx_reset(MOTOR_ChannelType chx)
{
	uint32_t tempreg = 0;
	tempreg = MOTOR->CH[chx].CTRL.reg_u32val;
	((union _MOT_CTRL *)&tempreg)->bit.CH_enable = 0;
	MOTOR->CH[chx].CTRL.reg_u32val = tempreg;
	motor_status[chx].data_ready=false;
}

/* 电机测试代码示例
extern int done_cnt;
extern int run_cnt;
extern void MOTOR_IRQHandler(void);
extern uint32_t motor_num_s;
extern uint32_t motor_num_e;

void HAL_Motor_MspInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	HAL_GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
	// start位,PB8，IO2-13-7 传感器输入
	//GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//0位，PB9，IO3-15-8 传感器输入
	//GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// end位，PB10，IO4-17-9 传感器输入 
	//GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
	
	// dir，PB11，IO5-19-10 方向输出 
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	//GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_InitStruct.Pin, GPIO_PIN_SET);
	
	//HAL_GPIO_WritePin(GPIOB, GPIO_InitStruct.Pin, GPIO_PIN_RESET);
	
	// PWM,PB12，IO6-21-11 输出 CH0
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF7;
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	//HAL_GPIO_WritePin(GPIOB, GPIO_InitStruct.Pin, GPIO_PIN_SET);
	
	// PWM,PB18，IO7-23-12 输出 CH2
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF7;
	GPIO_InitStruct.Pin = GPIO_PIN_18;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// PWM,PB19，IO8-25-13 输出 CH3
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF7;
	GPIO_InitStruct.Pin = GPIO_PIN_19;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
	
	// PWM,PB13，U19-13-3 输出 CH1 
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF7;
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void test_case1()//只覆盖50%pwm占空比模式
{
    printf("-------test case1 start--------\n");
	
	HAL_Motor_MspInit();
	//drv_irq_register(40, MOTOR_IRQHandler);
	drv_irq_enable(40);
	for(int i=motor_num_s;i<=motor_num_e;i++){
		csi_motor_chx_reset(i);
	}
	
	for(int i=motor_num_s;i<=motor_num_e;i++){
		csi_motor_chx_init_S(i,100,10000,5000);
		
	}
	
	for(int i=motor_num_s;i<=motor_num_e;i++){
		csi_motor_chx_run(i);
		MDELAY(30);
		motor_status[i].cur_stage = 12;
		motor_status[i].next_stage=21;
	}

	for(int i=motor_num_s;i<=motor_num_e;i++){
		//csi_motor_chx_init_L(i,10000,10000);
		csi_motor_chx_init_S(i,10000,100,5000);
	}
	
	GPIO_PinState PinState;
	PinState =GPIO_PIN_RESET;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, PinState);
}
void test_case2(uint8_t mode)//可以指定pwm工作模式
{
    printf("-------test case2 start--------\n");	
	HAL_Motor_MspInit();
	//drv_irq_register(40, MOTOR_IRQHandler);
	drv_irq_enable(40);
	for(int i=motor_num_s;i<=motor_num_e;i++){
		csi_motor_chx_reset(i);
	}
	
	for(int i=motor_num_s;i<=motor_num_e;i++){
		csi_motor_chx_mode_init_S(i,mode,100,10000,5000);
		
	}
	
	for(int i=motor_num_s;i<=motor_num_e;i++){
		csi_motor_chx_run(i);
		MDELAY(30);
		motor_status[i].cur_stage = 120+mode;
		motor_status[i].next_stage=210+mode;
	}

	for(int i=motor_num_s;i<=motor_num_e;i++){
		//csi_motor_chx_init_L(i,10000,10000);
		csi_motor_chx_mode_init_S(i,mode,10000,100,5000);
	}
	
	GPIO_PinState PinState;
	PinState =GPIO_PIN_RESET;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, PinState);
}

void test_case3()//4种模式全覆盖
{
    printf("-------test case3 start--------\n");
	
	HAL_Motor_MspInit();
	drv_irq_enable(40);
	for(int i=motor_num_s;i<=motor_num_e;i++){
		csi_motor_chx_reset(i);
	}
	
	for(int i=motor_num_s;i<=motor_num_e;i++){
		csi_motor_chx_mode_init_S(i,0,100,10000,500);
		
	}
	
	for(int i=motor_num_s;i<=motor_num_e;i++){
		csi_motor_chx_run(i);
		MDELAY(30);
		motor_status[i].cur_stage = 1200;
		motor_status[i].next_stage=2100;
	}

	for(int i=motor_num_s;i<=motor_num_e;i++){
		csi_motor_chx_mode_init_S(i,1,10000,100,500);
	}
	
	GPIO_PinState PinState;
	PinState =GPIO_PIN_RESET;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, PinState);
}*/
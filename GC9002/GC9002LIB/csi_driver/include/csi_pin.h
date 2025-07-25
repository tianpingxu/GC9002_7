/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_pin.h
 * @brief    header File for PIN Driver
 * @version  V1.0
 * @date     2023/02/24
 * @model    pin
 ******************************************************************************/
#ifndef __CSI_PIN_H__
#define __CSI_PIN_H__

#include <csi_gpio_pin.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_PIN_ENABLED

typedef csi_gpio_mode_t csi_pin_mode_t;
typedef enum
{
    PIN_SPEED_LV0,
    PIN_SPEED_LV1,
    PIN_SPEED_LV2,
    PIN_SPEED_LV3,
}csi_pin_speed_t;
typedef enum
{
    PIN_DRIVE_LV0,
    PIN_DRIVE_LV1,
    PIN_DRIVE_LV2,
    PIN_DRIVE_LV3,
}csi_pin_drive_t;


csi_error_t csi_pin_set_mux(pin_name_t pin_name, pin_func_t pin_func);
pin_func_t  csi_pin_get_mux(pin_name_t pin_name);

csi_error_t csi_pin_set_mode(pin_name_t pin_name, csi_pin_mode_t mode);
csi_error_t csi_pin_set_speed(pin_name_t pin_name, csi_pin_speed_t speed);

csi_error_t csi_pin_set_wakeup(pin_name_t pin_name, bool enable);
csi_error_t csi_pin_set_drive(pin_name_t pin_name, csi_pin_drive_t drive);

uint32_t    csi_pin_get_gpio_devidx(pin_name_t pin_name);
//not support
uint32_t    csi_pin_get_uart_devidx(pin_name_t pin_name);
uint32_t    csi_pin_get_iic_devidx(pin_name_t pin_name);
uint32_t    csi_pin_get_spi_devidx(pin_name_t pin_name);
uint32_t    csi_pin_get_i2s_devidx(pin_name_t pin_name);

uint32_t    csi_pin_get_gpio_channel(pin_name_t pin_name);

//not support
uint32_t    csi_pin_get_pwm_channel(pin_name_t pin_name);
uint32_t    csi_pin_get_adc_channel(pin_name_t pin_name);

/**
  \brief  Get pin name
  \param [in]  gpio_idx  0/1        (GPIOA/B).
  \param [in]  channel   0x1<<X     (GPIO_Pin0~GPIO_Pin31).
  \param [out] pin_name_t .
 */
pin_name_t  csi_pin_get_pinname_by_gpio(uint8_t gpio_idx, uint32_t channel);

#endif /* CONFIG_CSI_DRV_PIN_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_PIN_H__ */
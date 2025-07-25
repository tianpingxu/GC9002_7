/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_gpio_pin.h
 * @brief    header File for GPIO PIN Driver
 * @version  V1.0
 * @date     2023/02/16
 * @model    gpio pin
 ******************************************************************************/
#ifndef __CSI_GPIO_PIN_H__
#define __CSI_GPIO_PIN_H__

#include <csi_gpio.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_GPIO_PIN_ENABLED

/* GC9002 GPIO driver */
struct pin_index
{
    int id;
    uint32_t gpio;  //GPIOA/B
    uint32_t pin;   //GPIO_PIN_0~GPIO_PIN_31
};

typedef struct _csi_gpio_pin
{
    csi_gpio_t gpio;    //csi Device unity handle
    uint32_t pin_idx;   //Pin idx, reference pin_name_t
    void(*callback)(struct _csi_gpio_pin pin, void *arg);//User callback function
    void *arg;          //Callback function argument
}csi_gpio_pin_t;

const struct pin_index *csi_get_pin_struct_use_id(uint32_t id);
const struct pin_index *csi_get_pin_struct_use_port(uint32_t port, uint32_t pin);


csi_error_t csi_gpio_pin_init(csi_gpio_pin_t *pin, pin_name_t pin_name);
csi_error_t csi_gpio_pin_uninit(csi_gpio_pin_t *pin);
csi_error_t csi_gpio_pin_dir(csi_gpio_pin_t *pin, csi_gpio_dir_t dir);

csi_error_t csi_gpio_pin_mode(csi_gpio_pin_t *pin, csi_gpio_mode_t mode);
csi_error_t csi_gpio_pin_irq_mode(csi_gpio_pin_t *pin, csi_gpio_irq_mode_t mode);
csi_error_t csi_gpio_pin_irq_enable(csi_gpio_pin_t *pin, bool enable);

//Do not support
csi_error_t csi_gpio_pin_debonce(csi_gpio_pin_t *pin, bool enable);

void                    csi_gpio_pin_write(csi_gpio_pin_t *pin, csi_gpio_pin_state_t value);
csi_gpio_pin_state_t    csi_gpio_pin_read(csi_gpio_pin_t *pin);
void                    csi_gpio_pin_toggle(csi_gpio_pin_t *pin);
csi_error_t             csi_gpio_pin_attach_callback(csi_gpio_pin_t *pin, void(*callback)(void *args), void *arg);
void                    csi_gpio_pin_detach_callback(csi_gpio_pin_t *pin);


#endif /* CONFIG_CSI_DRV_GPIO_PIN_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_GPIO_PIN_H__ */
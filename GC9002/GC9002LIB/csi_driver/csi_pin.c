#include "csi_pin.h"
#ifdef CONFIG_CSI_DRV_PIN_ENABLED


csi_error_t csi_pin_set_mux(pin_name_t pin_name, pin_func_t pin_func)
{
    const struct pin_index *index = csi_get_pin_struct_use_id(pin_name);
    if (index == NULL)
    {
        return CSI_ERROR;
    }
    HAL_GPIO_SetALTF((GPIO_TypeDef *)index->gpio, index->pin, pin_func);
    return CSI_OK;
}
pin_func_t csi_pin_get_mux(pin_name_t pin_name)
{
    const struct pin_index *index = csi_get_pin_struct_use_id(pin_name);
    if (index == NULL)
    {
        return CSI_ERROR;
    }
    return (pin_func_t)HAL_GPIO_GetALTF((GPIO_TypeDef *)index->gpio, index->pin);
}
csi_error_t csi_pin_set_mode(pin_name_t pin_name, csi_pin_mode_t mode)
{
    const struct pin_index *index = csi_get_pin_struct_use_id(pin_name);
    if (index == NULL)
    {
        return CSI_ERROR;
    }
    uint32_t pin_mode;
    uint32_t pin_pull;
    switch(mode)
    {
        case GPIO_MODE_PULLNONE:{
            /* input setting: not pull. */
            pin_mode = GPIO_MODE_INPUT;
            pin_pull = GPIO_NOPULL;
        }break;
        case GPIO_MODE_PULLUP:{
            /* input setting: pull up. */
            pin_mode = GPIO_MODE_INPUT;
            pin_pull = GPIO_PULLUP;
        }break;
        case GPIO_MODE_PULLDOWN:{
            /* input setting: pull down. */
            pin_mode = GPIO_MODE_INPUT;
            pin_pull = GPIO_PULLDOWN;
        }break;
        case GPIO_MODE_OPEN_DRAIN:{
            /* output setting: open drain. */
            pin_mode = GPIO_MODE_OUTPUT_OD;
            pin_pull = GPIO_NOPULL;
        }break;
        case GPIO_MODE_PUSH_PULL:{
            /* output setting: push pull. */
            pin_mode = GPIO_MODE_OUTPUT_PP;
            pin_pull = GPIO_NOPULL;
        }break;
        default:
        return CSI_ERROR;
    }
    HAL_GPIO_SetMode((GPIO_TypeDef *)index->gpio, index->pin, pin_mode, pin_pull);
    return CSI_OK;
}

csi_error_t csi_pin_set_speed(pin_name_t pin_name, csi_pin_speed_t speed)
{
    const struct pin_index *index = csi_get_pin_struct_use_id(pin_name);
    if (index == NULL)
    {
        return CSI_ERROR;
    }
    switch(speed)
    {
        case PIN_SPEED_LV0:
        case PIN_SPEED_LV1:
            HAL_GPIO_SetSpeed((GPIO_TypeDef *)index->gpio, index->pin, GPIO_SPEED_FREQ_LOW);
        break;
        case PIN_SPEED_LV2:
        case PIN_SPEED_LV3:
            HAL_GPIO_SetSpeed((GPIO_TypeDef *)index->gpio, index->pin, GPIO_SPEED_FREQ_HIGH);
        break;
        default:
        return CSI_ERROR;
    }
    return CSI_OK;
}
csi_error_t csi_pin_set_wakeup(pin_name_t pin_name, bool enable)
{
    (void)pin_name;
    (void)enable;
    return CSI_UNSUPPORTED;
}
csi_error_t csi_pin_set_drive(pin_name_t pin_name, csi_pin_drive_t drive)
{
    const struct pin_index *index = csi_get_pin_struct_use_id(pin_name);
    if (index == NULL)
    {
        return CSI_ERROR;
    }
    uint32_t pin_drive;
    switch(drive)
    {
        case PIN_DRIVE_LV0:
            pin_drive = GPIO_DRIVER_STRENGTH_0;
        break;
        case PIN_DRIVE_LV1:
            pin_drive = GPIO_DRIVER_STRENGTH_1;
        break;
        case PIN_DRIVE_LV2:
            pin_drive = GPIO_DRIVER_STRENGTH_2;
        break;
        case PIN_SPEED_LV3:
            pin_drive = GPIO_DRIVER_STRENGTH_3;
        break;
        default:
        return CSI_ERROR;
    }
    HAL_GPIO_SetDrive((GPIO_TypeDef *)index->gpio, index->pin, pin_drive);
    return CSI_OK;
}

uint32_t csi_pin_get_gpio_devidx(pin_name_t pin_name)
{
    const struct pin_index *index = csi_get_pin_struct_use_id(pin_name);
    if (index == NULL)
    {
        return 0xFFFFFFFFU;
    }
    return index->gpio;
}
uint32_t csi_pin_get_uart_devidx(pin_name_t pin_name)
{
    (void)pin_name;
    return 0xFFFFFFFFU;
}
uint32_t csi_pin_get_iic_devidx(pin_name_t pin_name)
{
    (void)pin_name;
    return 0xFFFFFFFFU;
}
uint32_t csi_pin_get_spi_devidx(pin_name_t pin_name)
{
    (void)pin_name;
    return 0xFFFFFFFFU;
}
uint32_t csi_pin_get_i2s_devidx(pin_name_t pin_name)
{
    (void)pin_name;
    return 0xFFFFFFFFU;
}

uint32_t csi_pin_get_gpio_channel(pin_name_t pin_name)
{
    const struct pin_index *index = csi_get_pin_struct_use_id(pin_name);
    if (index == NULL)
    {
        return 0xFFFFFFFFU;
    }
    return index->pin;
}
uint32_t csi_pin_get_pwm_channel(pin_name_t pin_name)
{
    (void)pin_name;
    return 0xFFFFFFFFU;
}
uint32_t csi_pin_get_adc_channel(pin_name_t pin_name)
{
    (void)pin_name;
    return 0xFFFFFFFFU;
}

/**
  \brief  Get pin name
  \param [in]  gpio_idx  0/1        (GPIOA/B).
  \param [in]  channel   0x1<<X     (GPIO_Pin0~GPIO_Pin31).
  \param [out] pin_name_t .
 */
pin_name_t csi_pin_get_pinname_by_gpio(uint8_t gpio_idx, uint32_t channel)
{
    const struct pin_index *index = NULL;
    switch(gpio_idx)
    {
        case 0:
            index = csi_get_pin_struct_use_port(GPIOA_BASE, channel);
        break;
        case 1:
            index = csi_get_pin_struct_use_port(GPIOB_BASE, channel);
        break;
        default:
        break;
    }
    if(index == NULL)
    {
        return (pin_name_t)(index->id);
    }
    return PA0;
}


#endif /* CONFIG_CSI_DRV_PIN_ENABLED */
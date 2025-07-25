#include "csi_gpio.h"
#ifdef CONFIG_CSI_DRV_GPIO_ENABLED

struct pin_irq_hdr pin_irq_hdr_tab[CSI_GPIO_PORT_NUM] =
{
    {   0, 0, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,}, 
    },
    {   0, 0, 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,}, 
    },
};

const struct pin_irq_map g_pin_irq_map[CSI_GPIO_PORT_NUM] =
{
    {(uint32_t)GPIOA, GPIOA_IRQn},
    {(uint32_t)GPIOB, GPIOB_IRQn},
};

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_GPIOA_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_GPIOA_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();
    
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_GPIOB_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_GPIOB_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();
    
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
csi_error_t csi_gpio_init(csi_gpio_t *gpio, uint32_t port_idx)
{
    if(gpio == NULL || port_idx >= CSI_GPIO_CNT)
    {
        return CSI_ERROR;
    }
    if(port_idx == 0)
    {
        HANDLE_REG_BASE(gpio) = GPIOA_BASE;
        HANDLE_IRQ_NUM(gpio) = GPIOA_IRQn;
        HANDLE_DEV_IDX(gpio) = 0;
        HANDLE_IRQ_HANDLER(gpio) = (void *)CSI_GPIOA_IRQHandler;
    }
    else if(port_idx == 1)
    {
        HANDLE_REG_BASE(gpio) = GPIOB_BASE;
        HANDLE_IRQ_NUM(gpio) = GPIOB_IRQn;
        HANDLE_DEV_IDX(gpio) = 1;
        HANDLE_IRQ_HANDLER(gpio) = (void *)CSI_GPIOB_IRQHandler;
    }
    return CSI_OK;
}
csi_error_t csi_gpio_uninit(csi_gpio_t *gpio)
{
    if(gpio == NULL)
    {
        return CSI_ERROR;
    }
    HANDLE_REG_BASE(gpio) = 0;
    HANDLE_IRQ_NUM(gpio) = 0;
    HANDLE_DEV_IDX(gpio) = 0;
    HANDLE_IRQ_HANDLER(gpio) = NULL;
    return CSI_OK;
}
csi_error_t csi_gpio_dir(csi_gpio_t *gpio, uint32_t pin_mask, csi_gpio_dir_t dir)
{
    if(gpio == NULL)
    {
        return CSI_ERROR;
    }
    HAL_GPIO_SetDir((GPIO_TypeDef *)HANDLE_REG_BASE(gpio), pin_mask, (dir == GPIO_DIRECTION_INPUT ? GPIO_DIR_INPUT : GPIO_DIR_OUTPUT));
    return CSI_OK;
}
csi_error_t csi_gpio_mode(csi_gpio_t *gpio, uint32_t pin_mask, csi_gpio_mode_t mode)
{
    if(gpio == NULL)
    {
        return CSI_ERROR;
    }
    GPIO_InitTypeDef GPIO_InitStruct;
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    /* Configure GPIO_InitStructure */
    GPIO_InitStruct.Pin = pin_mask;
    switch(mode)
    {
        case GPIO_MODE_PULLNONE:{
            /* input setting: not pull. */
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
        }break;
        case GPIO_MODE_PULLUP:{
            /* input setting: pull up. */
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_PULLUP;
        }break;
        case GPIO_MODE_PULLDOWN:{
            /* input setting: pull down. */
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        }break;
        case GPIO_MODE_OPEN_DRAIN:{
            /* output setting: open drain. */
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
        }break;
        case GPIO_MODE_PUSH_PULL:{
            /* output setting: push pull. */
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
        }break;
        default:
        return CSI_ERROR;
    }
    HAL_GPIO_Init((GPIO_TypeDef *)HANDLE_REG_BASE(gpio), &GPIO_InitStruct);
    return CSI_OK;
}
csi_error_t csi_gpio_irq_mode(csi_gpio_t *gpio, uint32_t pin_mask, csi_gpio_irq_mode_t mode)
{
    if(gpio == NULL)
    {
        return CSI_ERROR;
    }
    int32_t irqindex = port2bitno(HANDLE_REG_BASE(gpio));
    if (irqindex < 0 || irqindex >= ITEM_NUM(g_pin_irq_map))
    {
        return CSI_ERROR;
    }
    pin_irq_hdr_tab[irqindex].gpiox = HANDLE_REG_BASE(gpio);
    pin_irq_hdr_tab[irqindex].mode = mode;
    return CSI_OK;
}
csi_error_t csi_gpio_irq_enable(csi_gpio_t *gpio, uint32_t pin_mask, bool enable)
{
    if(gpio == NULL)
    {
        return CSI_ERROR;
    }
    int32_t irqindex = port2bitno(HANDLE_REG_BASE(gpio));
    if (irqindex < 0 || irqindex >= ITEM_NUM(g_pin_irq_map))
    {
        return CSI_ERROR;
    }
    GPIO_InitTypeDef GPIO_InitStruct;
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    /* Configure GPIO_InitStructure */
    GPIO_InitStruct.Pin = pin_mask;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    switch (pin_irq_hdr_tab[irqindex].mode)
    {
    case GPIO_IRQ_MODE_RISING_EDGE:
        GPIO_InitStruct.Mode |= GPIO_MODE_IT_RISING;
        break;
    case GPIO_IRQ_MODE_FALLING_EDGE:
        GPIO_InitStruct.Mode |= GPIO_MODE_IT_FALLING;
        break;
    case GPIO_IRQ_MODE_BOTH_EDGE:
        GPIO_InitStruct.Mode |= GPIO_MODE_IT_RISING_FALLING;
        break;
    case GPIO_IRQ_MODE_LOW_LEVEL:
        GPIO_InitStruct.Mode |= GPIO_MODE_IT_LOW;
        break;
    case GPIO_IRQ_MODE_HIGH_LEVEL:
        GPIO_InitStruct.Mode |= GPIO_MODE_IT_HIGH;
        break;
    default:
        return CSI_ERROR;
    }
    if (enable)
    {
        HAL_GPIO_SetInterrupt((GPIO_TypeDef *)HANDLE_REG_BASE(gpio), &GPIO_InitStruct, ENABLE);
        /* enable and set interrupt priority */
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
        csi_clic_register_irq(HANDLE_IRQ_NUM(gpio), CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, HANDLE_IRQ_HANDLER(gpio));
#else
        csi_clic_register_irq(HANDLE_IRQ_NUM(gpio), CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, HANDLE_IRQ_HANDLER(gpio));
#endif
    }
    else
    {
        HAL_GPIO_SetInterrupt((GPIO_TypeDef *)HANDLE_REG_BASE(gpio), &GPIO_InitStruct, DISABLE);
    }
    return CSI_OK;
}
csi_error_t csi_gpio_debonce(csi_gpio_t *gpio, uint32_t pin_mask, bool enable)
{
    (void)gpio;
    (void)pin_mask;
    (void)enable;
    return CSI_UNSUPPORTED;
}
void  csi_gpio_write(csi_gpio_t *gpio, uint32_t pin_mask, csi_gpio_pin_state_t value)
{
    if(gpio == NULL)
    {
        return;
    }
    HAL_GPIO_WritePin((GPIO_TypeDef *)HANDLE_REG_BASE(gpio), pin_mask, (value == GPIO_PIN_LOW ? GPIO_PIN_RESET : GPIO_PIN_SET));
}
uint32_t csi_gpio_read(csi_gpio_t *gpio, uint32_t pin_mask)
{
    if(gpio == NULL)
    {
        return CSI_ERROR;
    }
    return (HAL_GPIO_ReadPin((GPIO_TypeDef *)HANDLE_REG_BASE(gpio), pin_mask) == GPIO_PIN_RESET ? GPIO_PIN_LOW: GPIO_PIN_HIGH);
}
void csi_gpio_toggle(csi_gpio_t *gpio, uint32_t pin_mask)
{
    if(gpio == NULL)
    {
        return;
    }
    HAL_GPIO_TogglePin((GPIO_TypeDef *)HANDLE_REG_BASE(gpio), pin_mask);
}
csi_error_t csi_gpio_attach_callback(csi_gpio_t *gpio, void *callback, void *arg)
{
    (void)arg;
    if(gpio == NULL || callback == NULL)
    {
        return CSI_ERROR;
    }
    HANDLE_IRQ_HANDLER(gpio) = callback;
    return CSI_OK;
}
void csi_gpio_detach_callback(csi_gpio_t *gpio)
{
    (void)gpio;
}

#endif /* CONFIG_CSI_DRV_GPIO_ENABLED */
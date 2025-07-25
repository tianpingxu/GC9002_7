#include "csi_gpio_pin.h"
#ifdef CONFIG_CSI_DRV_GPIO_PIN_ENABLED


#define __GC9002_PIN(index, gpio, gpio_index)                               \
    {                                                                       \
        index, (uint32_t)GPIO##gpio, GPIO_PIN_##gpio_index                  \
    }

static const struct pin_index pins[] =
{
    __GC9002_PIN(0, A, 0),
    __GC9002_PIN(1, A, 1),
    __GC9002_PIN(2, A, 2),
    __GC9002_PIN(3, A, 3),
    __GC9002_PIN(4, A, 4),
    __GC9002_PIN(5, A, 5),
    __GC9002_PIN(6, A, 6),
    __GC9002_PIN(7, A, 7),
    __GC9002_PIN(8, A, 8),
    __GC9002_PIN(9, A, 9),
    __GC9002_PIN(10, A, 10),
    __GC9002_PIN(11, A, 11),
    __GC9002_PIN(12, A, 12),
    __GC9002_PIN(13, A, 13),
    __GC9002_PIN(14, A, 14),
    __GC9002_PIN(15, A, 15),
    __GC9002_PIN(16, A, 16),
    __GC9002_PIN(17, A, 17),
    __GC9002_PIN(18, A, 18),
    __GC9002_PIN(19, A, 19),
    __GC9002_PIN(20, A, 20),
    __GC9002_PIN(21, A, 21),
    __GC9002_PIN(22, A, 22),
    __GC9002_PIN(23, A, 23),
    __GC9002_PIN(24, A, 24),
    __GC9002_PIN(25, A, 25),
    __GC9002_PIN(26, A, 26),
    __GC9002_PIN(27, A, 27),
    __GC9002_PIN(28, A, 28),
    __GC9002_PIN(29, A, 29),
    __GC9002_PIN(30, A, 30),
    __GC9002_PIN(31, A, 31),

    __GC9002_PIN(32, B, 0),
    __GC9002_PIN(33, B, 1),
    __GC9002_PIN(34, B, 2),
    __GC9002_PIN(35, B, 3),
    __GC9002_PIN(36, B, 4),
    __GC9002_PIN(37, B, 5),
    __GC9002_PIN(38, B, 6),
    __GC9002_PIN(39, B, 7),
    __GC9002_PIN(40, B, 8),
    __GC9002_PIN(41, B, 9),
    __GC9002_PIN(42, B, 10),
    __GC9002_PIN(43, B, 11),
    __GC9002_PIN(44, B, 12),
    __GC9002_PIN(45, B, 13),
    __GC9002_PIN(46, B, 14),
    __GC9002_PIN(47, B, 15),
    __GC9002_PIN(48, B, 16),
    __GC9002_PIN(49, B, 17),
    __GC9002_PIN(50, B, 18),
    __GC9002_PIN(51, B, 19),
    __GC9002_PIN(52, B, 20),
    __GC9002_PIN(53, B, 21),
    __GC9002_PIN(54, B, 22),
    __GC9002_PIN(55, B, 23),
    __GC9002_PIN(56, B, 24),
    __GC9002_PIN(57, B, 25),
    __GC9002_PIN(58, B, 26),
    __GC9002_PIN(59, B, 27),
    __GC9002_PIN(60, B, 28),
    __GC9002_PIN(61, B, 29),
    __GC9002_PIN(62, B, 30),
    __GC9002_PIN(63, B, 31),
};
    
static inline void pin_irq_hdr_invoke(int irqno, int index)
{
    if (pin_irq_hdr_tab[irqno].hdr[index])
    {
        pin_irq_hdr_tab[irqno].hdr[index](pin_irq_hdr_tab[irqno].args[index]);
    }
}
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_GPIOA_EXTI_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_GPIOA_EXTI_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();
    uint32_t int_flag = HAL_GPIO_GetInterruptStatus(GPIOA, GPIO_PIN_ALL);
    for (uint32_t i = 0; i < 32; ++i)
    {
        if((int_flag >> i) & 0x1U)
        {
            pin_irq_hdr_invoke(0, i);
        }
    }
    HAL_GPIO_ClearInterruptFlag(GPIOA, GPIO_PIN_ALL);
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_GPIOB_EXTI_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_GPIOB_EXTI_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();
    uint32_t int_flag = HAL_GPIO_GetInterruptStatus(GPIOB, GPIO_PIN_ALL);
    for (uint32_t i = 0; i < 32; ++i)
    {
        if((int_flag >> i) & 0x1U)
        {
            pin_irq_hdr_invoke(1, i);
        }
    }
    HAL_GPIO_ClearInterruptFlag(GPIOB, GPIO_PIN_ALL);
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
    

const struct pin_index *csi_get_pin_struct_use_id(uint32_t id)
{
    const struct pin_index *index = NULL;
    if (id < ITEM_NUM(pins))
    {
        index = &pins[id];
        if (index->id == -1)
            index = NULL;
    }
    else
    {
        index = NULL;
    }
    return index;
};
const struct pin_index *csi_get_pin_struct_use_port(uint32_t port, uint32_t pin)
{
    const struct pin_index *index = NULL;
    for(int i = 0; i < ITEM_NUM(pins); i++)
    {
        index = &pins[i];
        if(index->gpio == port && index->pin == pin)
        {
            break;
        }
    }
    return index;
};

static const struct pin_irq_map *get_pin_irq_map(uint32_t gpiox)
{
    int32_t mapindex = port2bitno(gpiox);
    if (mapindex < 0 || mapindex >= ITEM_NUM(g_pin_irq_map))
    {
        return NULL;
    }
    return &g_pin_irq_map[mapindex];
};
static uint32_t get_port_index(uint32_t gpio)
{
    if(gpio == (uint32_t)GPIOA_BASE)
    {
        return 0;
    }
    else if(gpio == (uint32_t)GPIOB_BASE)
    {
        return 1;
    }
    return 0;
}
csi_error_t csi_gpio_pin_init(csi_gpio_pin_t *pin, pin_name_t pin_name)
{
    csi_error_t ret = CSI_OK;
    if (pin == NULL)
    {
        return CSI_ERROR;
    }
    pin->pin_idx = (uint32_t)pin_name;
    const struct pin_index *index = csi_get_pin_struct_use_id(pin->pin_idx);
    if (index == NULL)
    {
        return CSI_ERROR;
    }
    ret = csi_gpio_init(&(pin->gpio), get_port_index(index->gpio));
    if(ret != CSI_OK)
    {
        return CSI_ERROR;
    }
    return csi_gpio_attach_callback(&(pin->gpio), (void *)(index->gpio == (uint32_t)GPIOA ? CSI_GPIOA_EXTI_IRQHandler: CSI_GPIOB_EXTI_IRQHandler), (void *)NULL);
}
csi_error_t csi_gpio_pin_uninit(csi_gpio_pin_t *pin)
{
    pin->pin_idx = PIN_NOT_USED;
    csi_gpio_uninit(&(pin->gpio));
    return CSI_OK;
}
csi_error_t csi_gpio_pin_dir(csi_gpio_pin_t *pin, csi_gpio_dir_t dir)
{
    if (pin == NULL)
    {
        return CSI_ERROR;
    }
    const struct pin_index *index = csi_get_pin_struct_use_id(pin->pin_idx);
    if (index == NULL)
    {
        return CSI_ERROR;
    }
    return csi_gpio_dir(&(pin->gpio), index->pin, dir);
}
csi_error_t csi_gpio_pin_mode(csi_gpio_pin_t *pin, csi_gpio_mode_t mode)
{
    if (pin == NULL)
    {
        return CSI_ERROR;
    }
    const struct pin_index *index = csi_get_pin_struct_use_id(pin->pin_idx);
    if (index == NULL)
    {
        return CSI_ERROR;
    }
    return csi_gpio_mode(&(pin->gpio), index->pin, mode);
}
csi_error_t csi_gpio_pin_irq_mode(csi_gpio_pin_t *pin, csi_gpio_irq_mode_t mode)
{
    if (pin == NULL)
    {
        return CSI_ERROR;
    }
    const struct pin_index *index = csi_get_pin_struct_use_id(pin->pin_idx);
    if (index == NULL)
    {
        return CSI_ERROR;
    }
    csi_gpio_irq_mode(&(pin->gpio), index->pin, mode);
    return CSI_OK;
}
csi_error_t csi_gpio_pin_irq_enable(csi_gpio_pin_t *pin, bool enable)
{
    if (pin == NULL)
    {
        return CSI_ERROR;
    }
    const struct pin_index *index = csi_get_pin_struct_use_id(pin->pin_idx);
    if (index == NULL)
    {
        return CSI_ERROR;
    }
    return csi_gpio_irq_enable(&(pin->gpio), index->pin, enable);
}
csi_error_t csi_gpio_pin_debonce(csi_gpio_pin_t *pin, bool enable)
{
    if (pin == NULL)
    {
        return CSI_ERROR;
    }
    const struct pin_index *index = csi_get_pin_struct_use_id(pin->pin_idx);
    if (index == NULL)
    {
        return CSI_ERROR;
    }
    return csi_gpio_debonce(&(pin->gpio), index->pin, enable);
}

void csi_gpio_pin_write(csi_gpio_pin_t *pin, csi_gpio_pin_state_t value)
{
    if (pin == NULL)
    {
        return;
    }
    const struct pin_index *index = csi_get_pin_struct_use_id(pin->pin_idx);
    if (index == NULL)
    {
        return;
    }
    csi_gpio_write(&(pin->gpio), index->pin, value);
}
csi_gpio_pin_state_t csi_gpio_pin_read(csi_gpio_pin_t *pin)
{
    if (pin == NULL)
    {
        return GPIO_PIN_LOW;
    }
    const struct pin_index *index = csi_get_pin_struct_use_id(pin->pin_idx);
    if (index == NULL)
    {
        return GPIO_PIN_LOW;
    }
    return (csi_gpio_read(&(pin->gpio), index->pin) == GPIO_PIN_LOW ? GPIO_PIN_LOW: GPIO_PIN_HIGH);
}
void csi_gpio_pin_toggle(csi_gpio_pin_t *pin)
{
    if (pin == NULL)
    {
        return;
    }
    const struct pin_index *index = csi_get_pin_struct_use_id(pin->pin_idx);
    if (index == NULL)
    {
        return;
    }
    csi_gpio_toggle(&(pin->gpio), index->pin);
}

csi_error_t csi_gpio_pin_attach_callback(csi_gpio_pin_t *pin, void(*callback)(void *args), void *arg)
{
    int32_t irqindex = -1;
    if (pin == NULL)
    {
        return CSI_ERROR;
    }
    const struct pin_index *index = csi_get_pin_struct_use_id(pin->pin_idx);
    if (index == NULL)
    {
        return CSI_ERROR;
    }
    irqindex = port2bitno(index->gpio);
    if (irqindex < 0 || irqindex >= ITEM_NUM(g_pin_irq_map))
    {
        return CSI_ERROR;
    }
    pin_irq_hdr_tab[irqindex].gpiox = index->gpio;
    pin_irq_hdr_tab[irqindex].hdr[index->id % 32] = callback;
    pin_irq_hdr_tab[irqindex].args[index->id % 32] = arg;
    return CSI_OK;
}
void csi_gpio_pin_detach_callback(csi_gpio_pin_t *pin)
{
    int32_t irqindex = -1;
    if (pin == NULL)
    {
        return;
    }
    const struct pin_index *index = csi_get_pin_struct_use_id(pin->pin_idx);
    if (index == NULL)
    {
        return;
    }
    irqindex = port2bitno(index->gpio);
    if (irqindex < 0 || irqindex >= ITEM_NUM(g_pin_irq_map))
    {
        return;
    }
    pin_irq_hdr_tab[irqindex].gpiox = index->gpio;
    pin_irq_hdr_tab[irqindex].hdr[index->id % 32] = NULL;
    pin_irq_hdr_tab[irqindex].args[index->id % 32] = NULL;
}

#endif /* CONFIG_CSI_DRV_GPIO_PIN_ENABLED */
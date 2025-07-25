#include "csi_timer.h"
#ifdef CONFIG_CSI_DRV_TIMER_ENABLED
/*
 * Timer 的时基为：微秒 us
 * */
static TIM_HandleTypeDef handle_tim[CSI_TIMER_CNT];

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_TIMER0_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_TIMER0_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    HAL_TIM_IRQHandler(&handle_tim[0]);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_TIMER1_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_TIMER1_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    HAL_TIM_IRQHandler(&handle_tim[1]);
    
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
static void CSI_HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim != NULL)
    {
        if(htim->priv_obj != NULL)
        {
            if(((csi_timer_t *)htim->priv_obj) != NULL)
            {
                ((csi_timer_t *)htim->priv_obj)->callback((csi_timer_t *)htim->priv_obj, ((csi_timer_t *)htim->priv_obj)->arg);
            }
        }
    }
}
csi_error_t csi_timer_init(csi_timer_t *timer, uint32_t idx)
{
    if(timer == NULL || idx >= CSI_TIMER_CNT)
    {
        return CSI_ERROR;
    }
    if(idx == 0)
    {
        HANDLE_REG_BASE(timer) = TIM0_BASE;
        HANDLE_IRQ_NUM(timer) = TIM0_IRQn;
        HANDLE_DEV_IDX(timer) = 0;
        HANDLE_IRQ_HANDLER(timer) = (void *)CSI_TIMER0_IRQHandler;
    }
    else if(idx == 1)
    {
        HANDLE_REG_BASE(timer) = TIM1_BASE;
        HANDLE_IRQ_NUM(timer) = TIM1_IRQn;
        HANDLE_DEV_IDX(timer) = 1;
        HANDLE_IRQ_HANDLER(timer) = (void *)CSI_TIMER1_IRQHandler;
    }
    handle_tim[HANDLE_DEV_IDX(timer)].priv_obj = timer;
	timer->priv = (TIM_HandleTypeDef *)&handle_tim[HANDLE_DEV_IDX(timer)];
    ((TIM_HandleTypeDef *)(timer->priv))->Instance = (TIM_TypeDef *)HANDLE_REG_BASE(timer);
    return CSI_OK;
}
void csi_timer_uninit(csi_timer_t *timer)
{
    if(timer == NULL)
    {
        return;
    }
    HANDLE_REG_BASE(timer) = 0;
    HANDLE_IRQ_NUM(timer) = 0;
    HANDLE_DEV_IDX(timer) = 0;
    HANDLE_IRQ_HANDLER(timer) = NULL;
    timer->priv = NULL;
}

static csi_error_t timer_start(csi_timer_t *timer, uint32_t timeout_us, bool autoreload_enable)
{
    if(timer == NULL)
    {
        return CSI_ERROR;
    }
    if(timer->priv == NULL)
    {
        return CSI_ERROR;
    }
    if(timeout_us <= 0xFFFF)
    {
        ((TIM_HandleTypeDef *)(timer->priv))->Init.Prescaler = HAL_RCC_GetAPBClock()/1000000 - 1;
        ((TIM_HandleTypeDef *)(timer->priv))->Init.Period = timeout_us;
    }
    else if(timeout_us > 65535 && timeout_us <= 655350)//0xFFFF*10
    {
        ((TIM_HandleTypeDef *)(timer->priv))->Init.Prescaler = HAL_RCC_GetAPBClock()/100000 - 1;
        ((TIM_HandleTypeDef *)(timer->priv))->Init.Period = timeout_us/10;
    }
    else if(timeout_us > 655350 && timeout_us < 6553500)//0xFFFF*100
    {
        ((TIM_HandleTypeDef *)(timer->priv))->Init.Prescaler = HAL_RCC_GetAPBClock()/10000 - 1;
        ((TIM_HandleTypeDef *)(timer->priv))->Init.Period = timeout_us/100;
    }
    else if(timeout_us > 6553500 && timeout_us < 65535000)//0xFFFF*1000
    {
        ((TIM_HandleTypeDef *)(timer->priv))->Init.Prescaler = HAL_RCC_GetAPBClock()/1000 - 1;
        ((TIM_HandleTypeDef *)(timer->priv))->Init.Period = timeout_us/1000;
    }
    else
    {
        return CSI_ERROR;
    }
	((TIM_HandleTypeDef *)(timer->priv))->Init.CounterMode = TIM_COUNTERMODE_UP;
	((TIM_HandleTypeDef *)(timer->priv))->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	((TIM_HandleTypeDef *)(timer->priv))->Init.RepetitionCounter = 0;
	((TIM_HandleTypeDef *)(timer->priv))->Init.AutoReloadPreload = (autoreload_enable ? TIM_AUTORELOAD_PRELOAD_ENABLE : TIM_AUTORELOAD_PRELOAD_DISABLE);
	HAL_TIM_Base_Init((TIM_HandleTypeDef *)(timer->priv));
    HAL_TIM_RegisterCallback((TIM_HandleTypeDef *)(timer->priv), HAL_TIM_PERIOD_ELAPSED_CB_ID, CSI_HAL_TIM_PeriodElapsedCallback);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(HANDLE_IRQ_NUM(timer), CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, HANDLE_IRQ_HANDLER(timer));
#else
    csi_clic_register_irq(HANDLE_IRQ_NUM(timer), CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, HANDLE_IRQ_HANDLER(timer));
#endif
	HAL_TIM_Base_Start_IT((TIM_HandleTypeDef *)(timer->priv));
    return CSI_OK;
}
csi_error_t csi_timer_start(csi_timer_t *timer, uint32_t timeout_us)
{
    if(timer == NULL)
    {
        return CSI_ERROR;
    }
    if(timer->priv == NULL)
    {
        return CSI_ERROR;
    }
    return timer_start(timer, timeout_us, false);
}

csi_error_t csi_timer_start_autoreload(csi_timer_t *timer, uint32_t timeout_us)
{
    if(timer == NULL)
    {
        return CSI_ERROR;
    }
    if(timer->priv == NULL)
    {
        return CSI_ERROR;
    }
    return timer_start(timer, timeout_us, true);
}
void csi_timer_stop(csi_timer_t *timer)
{
    if(timer == NULL)
    {
        return;
    }
    if(timer->priv == NULL)
    {
        return;
    }
    HAL_TIM_Base_Stop_IT((TIM_HandleTypeDef *)(timer->priv));
}
uint32_t csi_timer_get_remaining_value(csi_timer_t *timer)
{
    if(timer == NULL)
    {
        return 0;
    }
    if(timer->priv == NULL)
    {
        return 0;
    }
    return (__HAL_TIM_GET_AUTORELOAD((TIM_HandleTypeDef *)(timer->priv)) - __HAL_TIM_GET_COUNTER((TIM_HandleTypeDef *)(timer->priv)));
}
uint32_t csi_timer_get_load_value(csi_timer_t *timer)
{
    if(timer == NULL)
    {
        return 0;
    }
    if(timer->priv == NULL)
    {
        return 0;
    }
    return __HAL_TIM_GET_COUNTER((TIM_HandleTypeDef *)(timer->priv));
}
bool csi_timer_is_running(csi_timer_t *timer)
{
    if(timer == NULL)
    {
        return false;
    }
    if(timer->priv == NULL)
    {
        return false;
    }
    return __HAL_GET_TIM_IS_ENABLED((TIM_HandleTypeDef *)(timer->priv));
}
csi_error_t csi_timer_attach_callback(csi_timer_t *timer, void(*callback)(struct _csi_timer *timer, void *args), void *arg)
{    
    if(timer == NULL)
    {
        return CSI_ERROR;
    }
    timer->callback = callback;
    timer->arg = arg;
    return CSI_OK;
}
void csi_timer_detach_callback(csi_timer_t *timer)
{    
    if(timer == NULL)
    {
        return;
    }
    timer->callback = NULL;
    timer->arg = NULL;
}

#endif /* CONFIG_CSI_DRV_TIMER_ENABLED */
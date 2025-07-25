#include <csi_pwm.h>
#ifdef CONFIG_CSI_DRV_PWM_ENABLED

static TIM_HandleTypeDef handle_tim[CSI_PWM_CNT];

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_PWM0_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_PWM0_IRQHandler(void)
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
static void CSI_PWM1_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_PWM1_IRQHandler(void)
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
            if(((csi_pwm_t *)htim->priv_obj)->callback != NULL)
            {
                ((csi_pwm_t *)htim->priv_obj)->callback((csi_pwm_t *)htim->priv_obj, 
                                                        (((csi_pwm_t *)htim->priv_obj)->polarity == PWM_CAPTURE_POLARITY_POSEDGE ? 
                                                                                                    PWM_EVENT_CAPTURE_POSEDGE : 
                                                                                                    PWM_EVENT_CAPTURE_NEGEDGE),
                                                        htim->Channel,  
                                                        __HAL_TIM_GET_COUNTER(htim), 
                                                        ((csi_pwm_t *)htim->priv_obj)->arg);
            }
        }
    }
}
static void CSI_HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim != NULL)
    {
        if(htim->priv_obj != NULL)
        {
            if(((csi_pwm_t *)htim->priv_obj)->callback != NULL)
            {
                if(((csi_pwm_t *)htim->priv_obj)->cnt++ >= ((csi_pwm_t *)htim->priv_obj)->count)
                {
                    ((csi_pwm_t *)htim->priv_obj)->callback((csi_pwm_t *)htim->priv_obj, 
                                                            (((csi_pwm_t *)htim->priv_obj)->polarity == PWM_CAPTURE_POLARITY_POSEDGE ? 
                                                                                                        PWM_EVENT_CAPTURE_POSEDGE : 
                                                                                                        PWM_EVENT_CAPTURE_NEGEDGE),
                                                            htim->Channel,  
                                                            __HAL_TIM_GET_COUNTER(htim), 
                                                            ((csi_pwm_t *)htim->priv_obj)->arg);
                }
            }
        }
    }
}
static void CSI_HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim)
{
    if(htim != NULL)
    {
        if(htim->priv_obj != NULL)
        {
            if(((csi_pwm_t *)htim->priv_obj)->callback != NULL)
            {
                ((csi_pwm_t *)htim->priv_obj)->callback((csi_pwm_t *)htim->priv_obj, 
                                                        PWM_EVENT_ERROR,
                                                        htim->Channel,  
                                                        __HAL_TIM_GET_COUNTER(htim), 
                                                        ((csi_pwm_t *)htim->priv_obj)->arg);
            }
        }
    }
}
static uint32_t get_channel_user(uint32_t channel)
{
    switch(channel)
    {
        case 1: channel = TIM_CHANNEL_1;
        break;
        case 2: channel = TIM_CHANNEL_2;
        break;
        case 3: channel = TIM_CHANNEL_3;
        break;
        case 4: channel = TIM_CHANNEL_4;
        break;
        default: break;
    }
    return channel;
}

csi_error_t csi_pwm_init(csi_pwm_t *pwm, uint32_t idx)
{
#if(defined CONFIG_CSI_DRV_MOTOR_PWM && CONFIG_CSI_DRV_MOTOR_PWM == 1U)
    if(pwm == NULL || idx >= (CSI_PWM_CNT + CSI_MOTOR_PWM_CNT))
#else
    if(pwm == NULL || idx >= CSI_PWM_CNT)
#endif
    {
        return CSI_ERROR;
    }
    if(idx == 0)
    {
        HANDLE_REG_BASE(pwm) = TIM0_BASE;
        HANDLE_IRQ_NUM(pwm) = TIM0_IRQn;
        HANDLE_DEV_IDX(pwm) = 0;
        HANDLE_IRQ_HANDLER(pwm) = (void *)CSI_PWM0_IRQHandler;
    }
    else if(idx == 1)
    {
        HANDLE_REG_BASE(pwm) = TIM1_BASE;
        HANDLE_IRQ_NUM(pwm) = TIM1_IRQn;
        HANDLE_DEV_IDX(pwm) = 1;
        HANDLE_IRQ_HANDLER(pwm) = (void *)CSI_PWM1_IRQHandler;
    }
    else if (idx >= 2 && idx < 2+CSI_MOTOR_PWM_CNT)
    {
        pwm->motor_pwm_ch = idx - 2;
        return CSI_OK;
    }
    pwm->motor_pwm_ch = CSI_MOTOR_PWM_CH_MAX;
    handle_tim[HANDLE_DEV_IDX(pwm)].priv_obj = pwm;
	pwm->priv = (TIM_HandleTypeDef *)&handle_tim[HANDLE_DEV_IDX(pwm)];
    ((TIM_HandleTypeDef *)(pwm->priv))->Instance = (TIM_TypeDef *)HANDLE_REG_BASE(pwm);
    return CSI_OK;
}
void csi_pwm_uninit(csi_pwm_t *pwm)
{
    if(pwm == NULL)
    {
        return;
    }
    HANDLE_REG_BASE(pwm) = 0;
    HANDLE_IRQ_NUM(pwm) = 0;
    HANDLE_DEV_IDX(pwm) = 0;
    HANDLE_IRQ_HANDLER(pwm) = NULL;
    pwm->priv = NULL;
}

csi_error_t csi_pwm_out_config(csi_pwm_t *pwm,
                               uint32_t channel,
                               uint32_t period_us,
                               uint32_t pulse_width_us,
                               csi_pwm_polarity_t polarity)
{
    if(pwm == NULL || period_us < CSI_PWM_PERIOD_MIN)
    {
        return CSI_ERROR;
    }
    if(period_us < pulse_width_us)
    {
        pulse_width_us = period_us;
    }
    if(pulse_width_us == 0)
    {
        pulse_width_us = period_us;
        polarity = (polarity == PWM_POLARITY_HIGH ? PWM_POLARITY_LOW : PWM_POLARITY_HIGH);
    }
    if(pwm->motor_pwm_ch < CSI_MOTOR_PWM_CNT)
    {
        if(polarity == PWM_POLARITY_HIGH)
        {
            return csi_motor_pwm_chx_config((MOTOR_ChannelType)pwm->motor_pwm_ch, period_us, pulse_width_us, 1);
        }
        else if(polarity == PWM_POLARITY_LOW)
        {
            return csi_motor_pwm_chx_config((MOTOR_ChannelType)pwm->motor_pwm_ch, period_us, pulse_width_us, 0);
        }
        return CSI_ERROR;
    }
    if(!CSI_IS_PWM_CHANNEL(channel))
    {
        return CSI_ERROR;
    }
    if(pwm->priv == NULL)
    {
        return CSI_ERROR;
    }
    TIM_OC_InitTypeDef sConfigOC = {0};
    period_us -= 1;
    if(period_us <= 0xFFFF)
    {
        ((TIM_HandleTypeDef *)(pwm->priv))->Init.Prescaler = HAL_RCC_GetAPBClock()/1000000 - 1;
        ((TIM_HandleTypeDef *)(pwm->priv))->Init.Period = period_us;
    }
    else if(period_us > 65535 && period_us <= 655350)//0xFFFF*10
    {
        ((TIM_HandleTypeDef *)(pwm->priv))->Init.Prescaler = HAL_RCC_GetAPBClock()/100000 - 1;
        ((TIM_HandleTypeDef *)(pwm->priv))->Init.Period = period_us/10;
    }
    else if(period_us > 655350 && period_us <= 6553500)//0xFFFF*100
    {
        ((TIM_HandleTypeDef *)(pwm->priv))->Init.Prescaler = HAL_RCC_GetAPBClock()/10000 - 1;
        ((TIM_HandleTypeDef *)(pwm->priv))->Init.Period = period_us/100;
    }
    else if(period_us > 6553500 && period_us <= 65535000)//0xFFFF*1000
    {
        ((TIM_HandleTypeDef *)(pwm->priv))->Init.Prescaler = HAL_RCC_GetAPBClock()/1000 - 1;
        ((TIM_HandleTypeDef *)(pwm->priv))->Init.Period = period_us/1000;
    }
    else
    {
        return CSI_ERROR;
    }
    /*
     * 频率(CK_CNT)   = 48MHz/(4799+1)    = 10000 Hz
     * 周期(CK_CNT)   = 1/10000 Hz        = 0.1 ms
     * 周期(计数值200)= 0.1 ms * 200      = 20 ms
     * 频率(pwm)      = 1/0.02 s          = 50 Hz
     * */
    ((TIM_HandleTypeDef *)(pwm->priv))->Init.CounterMode = TIM_COUNTERMODE_UP;
    ((TIM_HandleTypeDef *)(pwm->priv))->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    ((TIM_HandleTypeDef *)(pwm->priv))->Init.RepetitionCounter = 0;
    ((TIM_HandleTypeDef *)(pwm->priv))->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init((TIM_HandleTypeDef *)(pwm->priv)) != HAL_OK)
    {
        return CSI_ERROR;
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    uint64_t val = (uint64_t)pulse_width_us*((uint64_t)((TIM_HandleTypeDef *)(pwm->priv))->Init.Period);
    sConfigOC.Pulse = (uint32_t)(val/period_us);    //比较值
    sConfigOC.OCPolarity = (polarity == PWM_POLARITY_HIGH ? TIM_OCPOLARITY_HIGH : TIM_OCPOLARITY_LOW);  //输出极性
    sConfigOC.OCNPolarity = (polarity == PWM_POLARITY_HIGH ? TIM_OCNPOLARITY_HIGH : TIM_OCNPOLARITY_LOW);
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel((TIM_HandleTypeDef *)(pwm->priv), &sConfigOC, get_channel_user(channel)) != HAL_OK)
    {
        return CSI_ERROR;
    }
    HAL_TIM_RegisterCallback((TIM_HandleTypeDef *)(pwm->priv), HAL_TIM_PERIOD_ELAPSED_CB_ID, CSI_HAL_TIM_PeriodElapsedCallback);
    HAL_TIM_RegisterCallback((TIM_HandleTypeDef *)(pwm->priv), HAL_TIM_IC_CAPTURE_CB_ID, CSI_HAL_TIM_IC_CaptureCallback);
    HAL_TIM_RegisterCallback((TIM_HandleTypeDef *)(pwm->priv), HAL_TIM_ERROR_CB_ID, CSI_HAL_TIM_ErrorCallback);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(HANDLE_IRQ_NUM(pwm), CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, HANDLE_IRQ_HANDLER(pwm));
#else
    csi_clic_register_irq(HANDLE_IRQ_NUM(pwm), CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, HANDLE_IRQ_HANDLER(pwm));
#endif
    return CSI_OK;
}
csi_error_t csi_pwm_out_start(csi_pwm_t *pwm, uint32_t channel)
{
    if(pwm == NULL)
    {
        return CSI_ERROR;
    }
    if(pwm->motor_pwm_ch < CSI_MOTOR_PWM_CNT)
    {
        csi_motor_pwm_chx_start(pwm->motor_pwm_ch);
        return CSI_OK;
    }
    if(!CSI_IS_PWM_CHANNEL(channel))
    {
        return CSI_ERROR;
    }
    if(pwm->priv == NULL || HANDLE_DEV_IDX(pwm) >= CSI_PWM_CNT)
    {
        return CSI_ERROR;
    }
    if(pwm->callback == NULL)
    {
        if(HAL_TIM_PWM_Start((TIM_HandleTypeDef *)(pwm->priv), get_channel_user(channel)) != HAL_OK)
        {
            return CSI_ERROR;
        }
    }
    else
    {
        if(HAL_TIM_PWM_Start_IT((TIM_HandleTypeDef *)(pwm->priv), get_channel_user(channel)) != HAL_OK)
        {
            return CSI_ERROR;
        }
    }
    return CSI_OK;
}
void csi_pwm_out_stop(csi_pwm_t *pwm, uint32_t channel)
{
    if(pwm == NULL)
    {
        return;
    }
    if(pwm->motor_pwm_ch < CSI_MOTOR_PWM_CNT)
    {
        csi_motor_pwm_chx_stop(pwm->motor_pwm_ch);
        return;
    }
    if(!CSI_IS_PWM_CHANNEL(channel))
    {
        return;
    }
    if(pwm->priv == NULL || HANDLE_DEV_IDX(pwm) >= CSI_PWM_CNT)
    {
        return;
    }
    if(pwm->callback == NULL)
    {
        if(HAL_TIM_PWM_Stop((TIM_HandleTypeDef *)(pwm->priv), get_channel_user(channel)) != HAL_OK)
        {
            return;
        }
    }
    else
    {
        if(HAL_TIM_PWM_Stop_IT((TIM_HandleTypeDef *)(pwm->priv), get_channel_user(channel)) != HAL_OK)
        {
            return;
        }
    }
}

csi_error_t csi_pwm_capture_config(csi_pwm_t *pwm,
                                   uint32_t channel,
                                   csi_pwm_capture_polarity_t polarity,
                                   uint32_t freq,
                                   uint32_t count)
{
    if(pwm == NULL || !CSI_IS_PWM_CHANNEL(channel) || freq == 0)
    {
        return CSI_ERROR;
    }
    if(pwm->priv == NULL || HANDLE_DEV_IDX(pwm) >= CSI_PWM_CNT)
    {
        return CSI_ERROR;
    }
    if(pwm->motor_pwm_ch != CSI_MOTOR_PWM_CH_MAX)
    {
        return CSI_ERROR;
    }
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_IC_InitTypeDef sConfigIC = {0};

    pwm->cnt = 0;
    pwm->count = count;
    pwm->polarity = polarity;
    switch(polarity)
    {
        case PWM_CAPTURE_POLARITY_POSEDGE:
            sConfigIC.ICPolarity = TIM_ICPOLARITY_RISING;
        break;
        case PWM_CAPTURE_POLARITY_NEGEDGE:
            sConfigIC.ICPolarity = TIM_ICPOLARITY_FALLING;
        break;
        case PWM_CAPTURE_POLARITY_BOTHEDGE:
            sConfigIC.ICPolarity = TIM_ICPOLARITY_BOTHEDGE;
        break;
        default: return CSI_ERROR;
    }
    /*
     * 频率(CK_CNT)   = 48MHz/(4799+1)    = 100000 Hz
     * 周期(CK_CNT)   = 1/100000 Hz       = 0.01 ms    =0.00001 s
     * 周期(计数值x)  = 0.01 ms * x       = 0.01*x ms
     * 频率(pwm)      = 1/(0.00001*x) s   = 100000/x Hz
     * */
    ((TIM_HandleTypeDef *)(pwm->priv))->Init.Prescaler = HAL_RCC_GetAPBClock()/freq - 1;
    if(((TIM_HandleTypeDef *)(pwm->priv))->Init.Prescaler > 0xFFFF)
    {
        return CSI_ERROR;
    }
    ((TIM_HandleTypeDef *)(pwm->priv))->Init.CounterMode = TIM_COUNTERMODE_UP;
    ((TIM_HandleTypeDef *)(pwm->priv))->Init.Period = 0xFFFF;  //自动重装载值
    ((TIM_HandleTypeDef *)(pwm->priv))->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    ((TIM_HandleTypeDef *)(pwm->priv))->Init.RepetitionCounter = 0;
    ((TIM_HandleTypeDef *)(pwm->priv))->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_IC_Init((TIM_HandleTypeDef *)(pwm->priv)) != HAL_OK)
    {
        return CSI_ERROR;
    }
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0x0; //滤波设置, 经几个周期跳变认定波形稳定
    if (HAL_TIM_IC_ConfigChannel((TIM_HandleTypeDef *)(pwm->priv), &sConfigIC, get_channel_user(channel)) != HAL_OK)
    {
        return CSI_ERROR;
    }
    HAL_TIM_RegisterCallback((TIM_HandleTypeDef *)(pwm->priv), HAL_TIM_PERIOD_ELAPSED_CB_ID, CSI_HAL_TIM_PeriodElapsedCallback);
    HAL_TIM_RegisterCallback((TIM_HandleTypeDef *)(pwm->priv), HAL_TIM_IC_CAPTURE_CB_ID, CSI_HAL_TIM_IC_CaptureCallback);
    csi_clic_register_irq(HANDLE_IRQ_NUM(pwm), CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, HANDLE_IRQ_HANDLER(pwm));
    return CSI_OK;
}
csi_error_t csi_pwm_capture_start(csi_pwm_t *pwm, uint32_t channel)
{
    if(pwm == NULL || !CSI_IS_PWM_CHANNEL(channel))
    {
        return CSI_ERROR;
    }
    if(pwm->priv == NULL || HANDLE_DEV_IDX(pwm) >= CSI_PWM_CNT)
    {
        return CSI_ERROR;
    }
    if(pwm->motor_pwm_ch != CSI_MOTOR_PWM_CH_MAX)
    {
        return CSI_ERROR;
    }
    if(HAL_TIM_IC_Start_IT((TIM_HandleTypeDef *)(pwm->priv), get_channel_user(channel)) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
void csi_pwm_capture_stop(csi_pwm_t *pwm, uint32_t channel)
{
    if(pwm == NULL || !CSI_IS_PWM_CHANNEL(channel))
    {
        return;
    }
    if(pwm->priv == NULL || HANDLE_DEV_IDX(pwm) >= CSI_PWM_CNT)
    {
        return;
    }
    if(pwm->motor_pwm_ch != CSI_MOTOR_PWM_CH_MAX)
    {
        return;
    }
    if(HAL_TIM_IC_Stop_IT((TIM_HandleTypeDef *)(pwm->priv), get_channel_user(channel)) != HAL_OK)
    {
        return;
    }
}
csi_error_t csi_pwm_attach_callback(csi_pwm_t *pwm, void(*callback)(struct _csi_pwm *pwm, csi_pwm_event_t event, uint32_t ch, uint32_t time_us, void *arg), void *arg)
{
    if(pwm == NULL || callback == NULL)
    {
        return CSI_ERROR;
    }
    pwm->callback = callback;
    pwm->arg = arg;
    return CSI_OK;
}
void csi_pwm_detach_callback(csi_pwm_t *pwm)
{
    if(pwm == NULL)
    {
        return;
    }
    pwm->callback = NULL;
    pwm->arg = NULL;
}
bool csi_pwm_get_status(csi_pwm_t *pwm, uint32_t channel)
{
    if(pwm == NULL || !CSI_IS_PWM_CHANNEL(channel))
    {
        return false;
    }
    if(pwm->motor_pwm_ch < CSI_MOTOR_PWM_CNT)
    {
        return csi_motor_pwm_chx_get_status(pwm->motor_pwm_ch);
    }
	if(!CSI_IS_PWM_CHANNEL(channel))
    {
        return false;
    }
    if(pwm->priv == NULL || HANDLE_DEV_IDX(pwm) >= CSI_PWM_CNT)
    {
        return false;
    }
    if(TIM_CHANNEL_STATE_GET((TIM_HandleTypeDef *)(pwm->priv), get_channel_user(channel)) == HAL_TIM_CHANNEL_STATE_BUSY)
    {
        return true;
    }
    return false;
}



#endif /* CONFIG_CSI_DRV_PWM_ENABLED */
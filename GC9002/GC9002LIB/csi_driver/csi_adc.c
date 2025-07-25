#include <csi_adc.h>
#ifdef CONFIG_CSI_DRV_ADC_ENABLED

static ADC_HandleTypeDef handle_adc = {.Instance = ADC};

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_ADC_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_ADC_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();
    HAL_ADC_IRQHandler(&handle_adc);
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

static void CSI_HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc != NULL)
    {
        if(hadc->priv_obj != NULL)
        {
            if(((csi_adc_t *)hadc->priv_obj)->dma == NULL)
            {
                if(hadc->pBuffPtr != NULL && hadc->XferCount < hadc->XferSize)
                {
                    if(HAL_IS_BIT_SET(hadc->Instance->FIFOSTATUS, ADC_FIFOSTA_EMPTY))
                    {
                        return;
                    }
                    uint32_t count = ((hadc->Instance->FIFOSTATUS & ADC_FIFOSTA_DATA_COUNT) >> ADC_FIFOSTA_DATA_COUNT_Pos);
                    for (int i = 0; i < count; i++)
                    {
                        *(hadc->pBuffPtr) = (uint16_t)(hadc->Instance->DOUT & 0x00000FFF);
                        // HAL_ADC_GetValue(hadc, hadc->pBuffPtr, 1);
                        hadc->pBuffPtr++;
                        hadc->XferCount+=2;
                        if(hadc->XferCount == hadc->XferSize)
                        {
                            HAL_ADC_Stop_IT(hadc);
                            hadc->pBuffPtr -= hadc->XferSize;
                            if(((csi_adc_t *)hadc->priv_obj)->callback != NULL)
                            {
                                ((csi_adc_t *)hadc->priv_obj)->callback((csi_adc_t *)hadc->priv_obj, ADC_EVENT_CONVERT_COMPLETE, ((csi_adc_t *)hadc->priv_obj)->arg);
                            }
                            break;
                        }
                    }
                }
            }
            else
            {
                if(((csi_adc_t *)hadc->priv_obj)->callback != NULL)
                {
                    ((csi_adc_t *)hadc->priv_obj)->callback((csi_adc_t *)hadc->priv_obj, ADC_EVENT_CONVERT_COMPLETE, ((csi_adc_t *)hadc->priv_obj)->arg);
                }
            }
        }
    }
}
static void CSI_HAL_ADC_AlmostFullCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc != NULL)
    {
        if(hadc->priv_obj != NULL)
        {
            if(((csi_adc_t *)hadc->priv_obj)->callback != NULL)
            {
                ((csi_adc_t *)hadc->priv_obj)->callback((csi_adc_t *)hadc->priv_obj, ADC_EVENT_CONVERT_HALF_DONE, ((csi_adc_t *)hadc->priv_obj)->arg);
            }
        }
    }
}
static void CSI_HAL_ADC_ErrorCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc != NULL)
    {
        if(hadc->priv_obj != NULL)
        {
            if(((csi_adc_t *)hadc->priv_obj)->callback != NULL)
            {
                ((csi_adc_t *)hadc->priv_obj)->callback((csi_adc_t *)hadc->priv_obj, ADC_EVENT_ERROR, ((csi_adc_t *)hadc->priv_obj)->arg);
            }
        }
    }
}

csi_error_t csi_adc_init(csi_adc_t *adc, uint32_t idx)
{
    if(adc == NULL || idx >= CSI_ADC_CNT)
    {
        return CSI_ERROR;
    }
    if(idx == 0)
    {
        HANDLE_REG_BASE(adc) = ADC_BASE;
        HANDLE_IRQ_NUM(adc) = ADC_IRQn;
        HANDLE_DEV_IDX(adc) = 0;
        HANDLE_IRQ_HANDLER(adc) = (void *)CSI_ADC_IRQHandler;
    }
    handle_adc.Instance = ADC;
    adc->priv = (void *)&handle_adc;
    ((ADC_HandleTypeDef *)adc->priv)->priv_obj = adc;
    HAL_ADC_StructInit(&handle_adc);
    handle_adc.Init.FullSize = 0;
    HAL_ADC_Init((ADC_HandleTypeDef *)adc->priv);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(HANDLE_IRQ_NUM(adc), CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, HANDLE_IRQ_HANDLER(adc));
#else
    csi_clic_register_irq(HANDLE_IRQ_NUM(adc), CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, HANDLE_IRQ_HANDLER(adc));
#endif
    HAL_ADC_RegisterCallback(&handle_adc, HAL_ADC_CONVERSION_COMPLETE_CB_ID, CSI_HAL_ADC_ConvCpltCallback);
    HAL_ADC_RegisterCallback(&handle_adc, HAL_ADC_ALMOST_FULL_CB_ID, CSI_HAL_ADC_AlmostFullCpltCallback);
    HAL_ADC_RegisterCallback(&handle_adc, HAL_ADC_ERROR_CB_ID, CSI_HAL_ADC_ErrorCallback);
    return CSI_OK;
}
void csi_adc_uninit(csi_adc_t *adc)
{
    if(adc == NULL)
    {
        return;
    }
    HANDLE_REG_BASE(adc) = 0;
    HANDLE_IRQ_NUM(adc) = 0;
    HANDLE_DEV_IDX(adc) = 0;
    HANDLE_IRQ_HANDLER(adc) = NULL;
    adc->priv = NULL;
    HAL_ADC_DeInit(&handle_adc);
}
csi_error_t csi_adc_set_buffer(csi_adc_t *adc, uint32_t *data, uint32_t num)
{
    if(adc == NULL || data == NULL || num == 0)
    {
        return CSI_ERROR;
    }
    if(adc->priv == NULL)
    {
        return CSI_ERROR;
    }
    adc->data = data;
    adc->size = num;
    ((ADC_HandleTypeDef *)adc->priv)->pBuffPtr = (uint16_t *)data;
    ((ADC_HandleTypeDef *)adc->priv)->XferSize = num;
    return CSI_OK;
}
csi_error_t csi_adc_start(csi_adc_t *adc)
{
    if(adc == NULL)
    {
        return CSI_ERROR;
    }
    if(HAL_ADC_Start((ADC_HandleTypeDef *)adc->priv) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
csi_error_t csi_adc_start_async(csi_adc_t *adc)
{
    if(adc == NULL)
    {
        return CSI_ERROR;
    }
    if(adc->dma == NULL)
    {
        ((ADC_HandleTypeDef *)adc->priv)->pBuffPtr = (uint16_t *)adc->data;
        ((ADC_HandleTypeDef *)adc->priv)->XferSize = (uint16_t)adc->size;
        ((ADC_HandleTypeDef *)adc->priv)->XferCount = 0;
        if(HAL_ADC_Start_IT((ADC_HandleTypeDef *)adc->priv) != HAL_OK)
        {
            return CSI_ERROR;
        }
    }
    else
    {
        if(HAL_ADC_Start_DMA((ADC_HandleTypeDef *)adc->priv, (uint16_t *)adc->data, adc->size) != HAL_OK)
        {
            return CSI_ERROR;
        }
    }
    return CSI_OK;
}
csi_error_t csi_adc_stop(csi_adc_t *adc)
{
    if(adc == NULL)
    {
        return CSI_ERROR;
    }
    if(HAL_ADC_Stop((ADC_HandleTypeDef *)adc->priv) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
csi_error_t csi_adc_stop_async(csi_adc_t *adc)
{
    if(adc == NULL)
    {
        return CSI_ERROR;
    }
    if(HAL_ADC_Stop_IT((ADC_HandleTypeDef *)adc->priv) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
csi_error_t csi_adc_channel_enable(csi_adc_t *adc, uint8_t ch_id, bool is_enable)
{
    if(adc == NULL)
    {
        return CSI_ERROR;
    }
    if(!IS_ADC_CHANNEL(ch_id))
    {
        return CSI_ERROR;
    }
    if(is_enable)
    {
        if(HAL_ADC_SetChannel((ADC_HandleTypeDef *)adc->priv, ch_id) != HAL_OK)
        {
            return CSI_ERROR;
        }
    }
    return CSI_OK;
}
csi_error_t csi_adc_channel_sampling_time(csi_adc_t *adc, uint8_t ch_id, uint16_t clock_num)
{
    if(adc == NULL)
    {
        return CSI_ERROR;
    }
    (void)ch_id;
    if(HAL_ADC_SetSampleTime((ADC_HandleTypeDef *)adc->priv, clock_num) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
csi_error_t csi_adc_sampling_time(csi_adc_t *adc, uint16_t clock_num)
{
    if(adc == NULL)
    {
        return CSI_ERROR;
    }
    if(HAL_ADC_SetSampleTime((ADC_HandleTypeDef *)adc->priv, clock_num) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
csi_error_t csi_adc_continue_mode(csi_adc_t *adc, bool is_enable)
{
    if(adc == NULL)
    {
        return CSI_ERROR;
    }
    if(HAL_ADC_ContinueModeCtrl((ADC_HandleTypeDef *)adc->priv, is_enable) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
uint32_t csi_adc_freq_div(csi_adc_t *adc, uint32_t div)
{
    if(adc == NULL)
    {
        return 0;
    }
    if(HAL_ADC_SetFreqDiv((ADC_HandleTypeDef *)adc->priv, div) != HAL_OK)
    {
        return 0;
    }
    return (HAL_RCC_GetAPBClock()/(div+1)/2);
}
uint32_t csi_adc_get_freq(csi_adc_t *adc)
{
    if(adc == NULL)
    {
        return 0;
    }
    return (HAL_RCC_GetAPBClock()/(((ADC_HandleTypeDef *)adc->priv)->Init.ClockDiv+1)/2);
}
int32_t csi_adc_read(csi_adc_t *adc)
{
    if(adc == NULL)
    {
        return 0;
    }
    if(adc->priv == NULL)
    {
        return CSI_ERROR;
    }
    uint16_t value = 0;
    __HAL_ADC_WAIT_DONE((ADC_HandleTypeDef *)adc->priv);
    HAL_ADC_GetValue((ADC_HandleTypeDef *)adc->priv, &value, 1);
    return value;
}
csi_error_t csi_adc_get_state(csi_adc_t *adc, csi_state_t *state)
{
    if(adc == NULL || state == NULL)
    {
        return CSI_ERROR;
    }
    *state = adc->state;
    return CSI_OK;
}
csi_error_t csi_adc_attach_callback(csi_adc_t *adc, void *callback, void *arg)
{
    if(adc == NULL || callback == NULL)
    {
        return CSI_ERROR;
    }
    adc->callback = callback;
    adc->arg = arg;
    return CSI_OK;
}
void csi_adc_detach_callback(csi_adc_t *adc)
{
    if(adc == NULL)
    {
        return;
    }
    adc->callback = NULL;
}
csi_error_t csi_adc_link_dma(csi_adc_t *adc, csi_dma_ch_t *dma)
{
    if(adc == NULL)
    {
        return CSI_ERROR;
    }
    if(adc->priv == NULL)
    {
        return CSI_ERROR;
    }
    adc->dma = dma;
    if(dma != NULL)
    {
        ((ADC_HandleTypeDef *)adc->priv)->hdma = (DMA_HandleTypeDef *)adc->dma->priv;
        ((DMA_HandleTypeDef *)dma->priv)->Parent = adc->priv;
    }
    return CSI_OK;
}






#endif /* CONFIG_CSI_DRV_ADC_ENABLED */
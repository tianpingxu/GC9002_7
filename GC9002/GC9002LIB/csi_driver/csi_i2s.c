#include <csi_i2s.h>
#ifdef CONFIG_CSI_DRV_I2S_ENABLED

static I2S_HandleTypeDef handle_i2s = {.Instance = I2S};

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_I2S_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_I2S_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();
    HAL_I2S_IRQHandler(&handle_i2s);
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
static void CSI_HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    if(hi2s != NULL)
    {
        if(hi2s->priv_obj != NULL)
        {
            if(((csi_i2s_t *)hi2s->priv_obj)->callback != NULL)
            {
                ((csi_i2s_t *)hi2s->priv_obj)->callback((csi_i2s_t *)hi2s->priv_obj, I2S_EVENT_TRANSMIT_COMPLETE, ((csi_i2s_t *)hi2s->priv_obj)->arg);
            }
        }
    }
}
static void CSI_HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    if(hi2s != NULL)
    {
        if(hi2s->priv_obj != NULL)
        {
            if(((csi_i2s_t *)hi2s->priv_obj)->callback != NULL)
            {
                ((csi_i2s_t *)hi2s->priv_obj)->callback((csi_i2s_t *)hi2s->priv_obj, I2S_EVENT_RECEIVE_COMPLETE, ((csi_i2s_t *)hi2s->priv_obj)->arg);
            }
        }
    }
}
static void CSI_HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s)
{
    if(hi2s != NULL)
    {
        if(hi2s->priv_obj != NULL)
        {
            if(((csi_i2s_t *)hi2s->priv_obj)->callback != NULL)
            {
                ((csi_i2s_t *)hi2s->priv_obj)->callback((csi_i2s_t *)hi2s->priv_obj, I2S_EVENT_ERROR, ((csi_i2s_t *)hi2s->priv_obj)->arg);
            }
        }
    }
}

csi_error_t csi_i2s_init(csi_i2s_t *i2s, uint32_t idx)
{
    if(i2s == NULL || idx >= CSI_IIS_CNT)
    {
        return CSI_ERROR;
    }
    if(idx == 0)
    {
        HANDLE_REG_BASE(i2s) = I2S_BASE;
        HANDLE_IRQ_NUM(i2s) = I2S_IRQn;
        HANDLE_DEV_IDX(i2s) = 0;
        HANDLE_IRQ_HANDLER(i2s) = (void *)CSI_I2S_IRQHandler;
    }
    i2s->priv = (void *)&handle_i2s;
    HAL_I2S_Init((I2S_HandleTypeDef *)i2s->priv);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(HANDLE_IRQ_NUM(i2s), CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, HANDLE_IRQ_HANDLER(i2s));
#else
    csi_clic_register_irq(HANDLE_IRQ_NUM(i2s), CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, HANDLE_IRQ_HANDLER(i2s));
#endif
    return CSI_OK;
}
void csi_i2s_uninit(csi_i2s_t *i2s)
{
    if(i2s == NULL)
    {
        return;
    }
    HANDLE_REG_BASE(i2s) = 0;
    HANDLE_IRQ_NUM(i2s) = 0;
    HANDLE_DEV_IDX(i2s) = 0;
    HANDLE_IRQ_HANDLER(i2s) = NULL;
    i2s->priv = NULL;
    HAL_I2S_DeInit(&handle_i2s);
}
void csi_i2s_enable(csi_i2s_t *i2s, bool en)
{
    if(i2s == NULL)
    {
        return;
    }
    if(i2s->priv == NULL)
    {
        return;
    }
    if(en)
    {
        __HAL_I2S_ENABLE((I2S_HandleTypeDef *)i2s->priv);
    }
    else
    {
        __HAL_I2S_DISABLE((I2S_HandleTypeDef *)i2s->priv);
    }
}
csi_error_t csi_i2s_format(csi_i2s_t *i2s, csi_i2s_format_t *format)
{
    if(i2s == NULL || format == NULL)
    {
        return CSI_ERROR;
    }
    if(i2s->priv == NULL)
    {
        return CSI_ERROR;
    }
    I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *)i2s->priv;
    HAL_I2S_StructInit(&(hi2s->Init));
    hi2s->Init.Channel = I2S_CHANNEL_0;
    hi2s->Init.Mode = format->mode;
    hi2s->Init.Standard = (enum I2S_Standard)format->protocol;
    switch(format->width)
    {
        case I2S_SAMPLE_WIDTH_16BIT:
            hi2s->Init.RxResolution = I2S_WLEN_RESOLUTION_16_BIT;
            hi2s->Init.TxResolution = I2S_WLEN_RESOLUTION_16_BIT;
            hi2s->Init.DataFormat = I2S_WSS_CLOCK_CYCLES_16;
        break;
        case I2S_SAMPLE_WIDTH_24BIT:
            hi2s->Init.RxResolution = I2S_WLEN_RESOLUTION_24_BIT;
            hi2s->Init.TxResolution = I2S_WLEN_RESOLUTION_24_BIT;
            hi2s->Init.DataFormat = I2S_WSS_CLOCK_CYCLES_24;
        break;
        case I2S_SAMPLE_WIDTH_32BIT:
            hi2s->Init.RxResolution = I2S_WLEN_RESOLUTION_32_BIT;
            hi2s->Init.TxResolution = I2S_WLEN_RESOLUTION_32_BIT;
            hi2s->Init.DataFormat = I2S_WSS_CLOCK_CYCLES_32;
        break;
        default: return CSI_ERROR;
    }
    hi2s->Init.RxTriggerLevel = I2S_TRIGGER_LEVEL_1;
    hi2s->Init.TxTriggerLevel = I2S_TRIGGER_LEVEL_1;
    switch(format->rate)
    {
        case I2S_SAMPLE_RATE_8000:
            hi2s->Init.AudioFreq = I2S_AUDIOFREQ_8K;
        break;
        case I2S_SAMPLE_RATE_11025:
            hi2s->Init.AudioFreq = I2S_AUDIOFREQ_11K;
        break;
        case I2S_SAMPLE_RATE_12000:
            hi2s->Init.AudioFreq = I2S_AUDIOFREQ_12K;
        break;
        case I2S_SAMPLE_RATE_16000:
            hi2s->Init.AudioFreq = I2S_AUDIOFREQ_16K;
        break;
        case I2S_SAMPLE_RATE_22050:
            hi2s->Init.AudioFreq = I2S_AUDIOFREQ_22K;
        break;
        case I2S_SAMPLE_RATE_24000:
            hi2s->Init.AudioFreq = I2S_AUDIOFREQ_24K;
        break;
        case I2S_SAMPLE_RATE_32000:
            hi2s->Init.AudioFreq = I2S_AUDIOFREQ_32K;
        break;
        case I2S_SAMPLE_RATE_44100:
            hi2s->Init.AudioFreq = I2S_AUDIOFREQ_44K;
        break;
        case I2S_SAMPLE_RATE_48000:
            hi2s->Init.AudioFreq = I2S_AUDIOFREQ_48K;
        break;
        case I2S_SAMPLE_RATE_96000:
            hi2s->Init.AudioFreq = I2S_AUDIOFREQ_96K;
        break;
        case I2S_SAMPLE_RATE_192000:
            hi2s->Init.AudioFreq = I2S_AUDIOFREQ_192K;
        break;
        case I2S_SAMPLE_RATE_256000:
            hi2s->Init.AudioFreq = I2S_AUDIOFREQ_256K;
        break;
        default: return CSI_ERROR;
    }
    (void)format->polarity;//WSCLK电平极性对应声道设置
    (void)format->sclk_nfs;//MCLK的频率设置SCLK的频率设置
    (void)format->mclk_nfs;//MCLK的频率设置
    HAL_I2S_RegisterCallback(hi2s, HAL_I2S_TX_COMPLETE_CB_ID, CSI_HAL_I2S_TxCpltCallback);
    HAL_I2S_RegisterCallback(hi2s, HAL_I2S_RX_COMPLETE_CB_ID, CSI_HAL_I2S_RxCpltCallback);
    HAL_I2S_RegisterCallback(hi2s, HAL_I2S_ERROR_CB_ID, CSI_HAL_I2S_ErrorCallback);
    return CSI_OK;
}
csi_error_t csi_i2s_tx_select_sound_channel(csi_i2s_t *i2s, csi_i2s_sound_channel_t ch)
{
    if(i2s == NULL)
    {
        return CSI_ERROR;
    }
    if(i2s->priv == NULL)
    {
        return CSI_ERROR;
    }
    switch(ch)
    {
        case I2S_LEFT_CHANNEL:
        break;
        case I2S_RIGHT_CHANNEL:
        break;
        case I2S_LEFT_RIGHT_CHANNEL:
        break;
        default: return CSI_ERROR;
    }
    return CSI_OK;
}
csi_error_t csi_i2s_rx_select_sound_channel(csi_i2s_t *i2s, csi_i2s_sound_channel_t ch)
{
    if(i2s == NULL)
    {
        return CSI_ERROR;
    }
    if(i2s->priv == NULL)
    {
        return CSI_ERROR;
    }
    switch(ch)
    {
        case I2S_LEFT_CHANNEL:
        break;
        case I2S_RIGHT_CHANNEL:
        break;
        case I2S_LEFT_RIGHT_CHANNEL:
        break;
        default: return CSI_ERROR;
    }
    return CSI_OK;
}
csi_error_t csi_i2s_tx_link_dma(csi_i2s_t *i2s, csi_dma_ch_t *tx_dma)
{
    (void)i2s;
    (void)tx_dma;
    return CSI_UNSUPPORTED;
}
csi_error_t csi_i2s_rx_link_dma(csi_i2s_t *i2s, csi_dma_ch_t *rx_dma)
{
    (void)i2s;
    (void)rx_dma;
    return CSI_UNSUPPORTED;
}
void csi_i2s_tx_set_buffer(csi_i2s_t *i2s, ringbuffer_t *buffer)
{
    if(i2s == NULL || buffer == NULL)
    {
        return;
    }
    i2s->tx_buf = buffer;
}
void csi_i2s_rx_set_buffer(csi_i2s_t *i2s, ringbuffer_t *buffer)
{
    if(i2s == NULL || buffer == NULL)
    {
        return;
    }
    i2s->rx_buf = buffer;
}
csi_error_t csi_i2s_tx_set_period(csi_i2s_t *i2s, uint32_t period)
{
    (void)i2s;
    (void)period;
    return CSI_UNSUPPORTED;
}
csi_error_t csi_i2s_rx_set_period(csi_i2s_t *i2s, uint32_t period)
{
    (void)i2s;
    (void)period;
    return CSI_UNSUPPORTED;
}
uint32_t csi_i2s_tx_buffer_avail(csi_i2s_t *i2s)
{
    (void)i2s;
    return 0;
}
uint32_t csi_i2s_rx_buffer_avail(csi_i2s_t *i2s)
{
    (void)i2s;
    return 0;
}
csi_error_t csi_i2s_rx_buffer_reset(csi_i2s_t *i2s)
{
    (void)i2s;
    return CSI_OK;
}
uint32_t csi_i2s_send(csi_i2s_t *i2s, const void *data, uint32_t size)
{
    if(i2s == NULL || data == NULL || size == 0)
    {
        return 0;
    }
    if(HAL_I2S_Transmit((I2S_HandleTypeDef *)i2s->priv, (uint8_t *)data, size, 0xFFFFFFFF) != HAL_OK)
    {
        return 0;
    }
    return size;
}
uint32_t    csi_i2s_send_async(csi_i2s_t *i2s, const void *data, uint32_t size)
{
    if(i2s == NULL || data == NULL || size == 0)
    {
        return 0;
    }
    if(HAL_I2S_Transmit_IT((I2S_HandleTypeDef *)i2s->priv, (uint8_t *)data, size) != HAL_OK)
    {
        return 0;
    }
    return size;
}
uint32_t csi_i2s_receive(csi_i2s_t *i2s, void *data, uint32_t size)
{
    if(i2s == NULL || data == NULL || size == 0)
    {
        return 0;
    }
    if(HAL_I2S_Receive((I2S_HandleTypeDef *)i2s->priv, (uint8_t *)data, size, 0xFFFFFFFF) != HAL_OK)
    {
        return 0;
    }
    return size;
}
uint32_t csi_i2s_receive_async(csi_i2s_t *i2s, void *data, uint32_t size)
{
    if(i2s == NULL || data == NULL || size == 0)
    {
        return 0;
    }
    if(HAL_I2S_Receive_IT((I2S_HandleTypeDef *)i2s->priv, (uint8_t *)data, size) != HAL_OK)
    {
        return 0;
    }
    return size;
}
csi_error_t csi_i2s_send_start(csi_i2s_t *i2s)
{
    if(i2s == NULL)
    {
        return CSI_ERROR;
    }
    return CSI_UNSUPPORTED;
}
csi_error_t csi_i2s_send_resume(csi_i2s_t *i2s)
{
    if(i2s == NULL)
    {
        return CSI_ERROR;
    }
    return CSI_UNSUPPORTED;
}
csi_error_t csi_i2s_send_pause(csi_i2s_t *i2s)
{
    if(i2s == NULL)
    {
        return CSI_ERROR;
    }
    return CSI_UNSUPPORTED;
}
csi_error_t csi_i2s_send_stop(csi_i2s_t *i2s)
{
    if(i2s == NULL)
    {
        return CSI_ERROR;
    }
    return CSI_UNSUPPORTED;
}
csi_error_t csi_i2s_receive_start(csi_i2s_t *i2s)
{
    if(i2s == NULL)
    {
        return CSI_ERROR;
    }
    return CSI_UNSUPPORTED;
}
csi_error_t csi_i2s_receive_stop(csi_i2s_t *i2s)
{
    if(i2s == NULL)
    {
        return CSI_ERROR;
    }
    return CSI_UNSUPPORTED;
}
csi_error_t csi_i2s_attach_callback(csi_i2s_t *i2s, void(*callback)(struct _csi_i2s *i2s, csi_i2s_event_t event, void *arg), void *arg)
{
    if(i2s == NULL)
    {
        return CSI_ERROR;
    }
    i2s->callback = callback;
    i2s->arg = arg;
    return CSI_OK;
}
csi_error_t csi_i2s_get_state(csi_i2s_t *i2s, csi_state_t *state)
{
    if(i2s == NULL || state == NULL)
    {
        return CSI_ERROR;
    }
    *state = i2s->state;
    return CSI_OK;
}




#endif /* CONFIG_CSI_DRV_I2S_ENABLED */
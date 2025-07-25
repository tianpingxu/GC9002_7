#include <csi_spi.h>
#ifdef CONFIG_CSI_DRV_SPI_ENABLED

static SPI_HandleTypeDef handle_spi[CSI_SPI_CNT];

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_SPI0_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_SPI0_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    HAL_SPI_IRQHandler(&handle_spi[0]);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_SPI1_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_SPI1_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    HAL_SPI_IRQHandler(&handle_spi[1]);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_SPI2_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_SPI2_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    HAL_SPI_IRQHandler(&handle_spi[2]);
    
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
static void CSI_HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi != NULL)
    {
        if(hspi->priv_obj != NULL)
        {
            if(((csi_spi_t *)hspi->priv_obj)->callback != NULL)
            {
                ((csi_spi_t *)hspi->priv_obj)->callback((csi_spi_t *)hspi->priv_obj, SPI_EVENT_TRANSMIT_COMPLETE, ((csi_spi_t *)hspi->priv_obj)->arg);
            }
        }
    }
}
static void CSI_HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi != NULL)
    {
        if(hspi->priv_obj != NULL)
        {
            if(((csi_spi_t *)hspi->priv_obj)->callback != NULL)
            {
                ((csi_spi_t *)hspi->priv_obj)->callback((csi_spi_t *)hspi->priv_obj, SPI_EVENT_RECEIVE_COMPLETE, ((csi_spi_t *)hspi->priv_obj)->arg);
            }
        }
    }
}
static void CSI_HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi != NULL)
    {
        if(hspi->priv_obj != NULL)
        {
            if(((csi_spi_t *)hspi->priv_obj)->callback != NULL)
            {
                ((csi_spi_t *)hspi->priv_obj)->callback((csi_spi_t *)hspi->priv_obj, SPI_EVENT_ERROR, ((csi_spi_t *)hspi->priv_obj)->arg);
            }
        }
    }
}
static void CSI_HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi != NULL)
    {
        if(hspi->priv_obj != NULL)
        {
            if(((csi_spi_t *)hspi->priv_obj)->callback != NULL)
            {
                ((csi_spi_t *)hspi->priv_obj)->callback((csi_spi_t *)hspi->priv_obj, SPI_EVENT_TRANSMIT_RECEIVE_COMPLETE, ((csi_spi_t *)hspi->priv_obj)->arg);
            }
        }
    }
}
csi_error_t csi_spi_init(csi_spi_t *spi, uint32_t idx)
{
    if(spi == NULL || idx >= CSI_SPI_CNT)
    {
        return CSI_ERROR;
    }
    if(idx == 0)
    {
        HANDLE_REG_BASE(spi) = SPI0_BASE;
        HANDLE_IRQ_NUM(spi) = SPI0_IRQn;
        HANDLE_DEV_IDX(spi) = 0;
        HANDLE_IRQ_HANDLER(spi) = (void *)CSI_SPI0_IRQHandler;
    }
    else if(idx == 1)
    {
        HANDLE_REG_BASE(spi) = SPI1_BASE;
        HANDLE_IRQ_NUM(spi) = SPI1_IRQn;
        HANDLE_DEV_IDX(spi) = 1;
        HANDLE_IRQ_HANDLER(spi) = (void *)CSI_SPI1_IRQHandler;
    }
    else if(idx == 2)
    {
        HANDLE_REG_BASE(spi) = SPI2_BASE;
        HANDLE_IRQ_NUM(spi) = SPI2_IRQn;
        HANDLE_DEV_IDX(spi) = 2;
        HANDLE_IRQ_HANDLER(spi) = (void *)CSI_SPI2_IRQHandler;
    }
    handle_spi[idx].priv_obj = spi;
    spi->priv = (void *)&handle_spi[idx];
    ((SPI_HandleTypeDef *)spi->priv)->Instance = (SPI_TypeDef *)HANDLE_REG_BASE(spi);
    
	((SPI_HandleTypeDef *)spi->priv)->Init.Mode = SPI_MODE_MASTER | SPI_MODE_NORMAL;
	((SPI_HandleTypeDef *)spi->priv)->Init.DataSize = SPI_DATASIZE_8BIT;
	((SPI_HandleTypeDef *)spi->priv)->Init.FrameFormat = SPI_FRF_MOTOROLA;
	((SPI_HandleTypeDef *)spi->priv)->Init.SPO = SPI_SPO_HIGH;
	((SPI_HandleTypeDef *)spi->priv)->Init.SPH = SPI_SPH_2EDGE;
	((SPI_HandleTypeDef *)spi->priv)->Init.CPSDVR = 0x10; // 时钟预分频系数 取值范围2-254之前的偶数
	((SPI_HandleTypeDef *)spi->priv)->Init.SCR = 0x7;
    HAL_SPI_Init((SPI_HandleTypeDef *)(spi->priv));
    HAL_SPI_RegisterCallback((SPI_HandleTypeDef *)(spi->priv), HAL_SPI_TX_COMPLETE_CB_ID, CSI_HAL_SPI_TxCpltCallback);
    HAL_SPI_RegisterCallback((SPI_HandleTypeDef *)(spi->priv), HAL_SPI_RX_COMPLETE_CB_ID, CSI_HAL_SPI_RxCpltCallback);
    HAL_SPI_RegisterCallback((SPI_HandleTypeDef *)(spi->priv), HAL_SPI_ERROR_CB_ID, CSI_HAL_SPI_ErrorCallback);
    HAL_SPI_RegisterCallback((SPI_HandleTypeDef *)(spi->priv), HAL_SPI_TX_RX_COMPLETE_CB_ID, CSI_HAL_SPI_TxRxCpltCallback);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(HANDLE_IRQ_NUM(spi), CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, HANDLE_IRQ_HANDLER(spi));
#else
    csi_clic_register_irq(HANDLE_IRQ_NUM(spi), CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, HANDLE_IRQ_HANDLER(spi));
#endif
    return CSI_OK;
}
void csi_spi_uninit(csi_spi_t *spi)
{
    if(spi == NULL)
    {
        return;
    }
    HANDLE_REG_BASE(spi) = 0;
    HANDLE_IRQ_NUM(spi) = 0;
    HANDLE_DEV_IDX(spi) = 0;
    HANDLE_IRQ_HANDLER(spi) = NULL;
    spi->priv = NULL;
}
csi_error_t csi_spi_attach_callback(csi_spi_t *spi, void(*callback)(struct _csi_spi *spi, csi_spi_event_t event, void *arg), void *arg)
{
    if(spi == NULL || callback == NULL)
    {
        return CSI_ERROR;
    }
    spi->callback = callback;
    spi->arg = arg;
    return CSI_OK;
}
void csi_spi_detach_callback(csi_spi_t *spi)
{
    if(spi == NULL)
    {
        return;
    }
    spi->callback = NULL;
    spi->arg = NULL;
}
csi_error_t csi_spi_mode(csi_spi_t *spi, csi_spi_mode_t mode)
{
    if(spi == NULL || HANDLE_REG_BASE(spi) == 0)
    {
        return CSI_ERROR;
    }
    if(spi->priv == NULL)
    {
        return CSI_ERROR;
    }
    if(mode == SPI_MASTER)
    {
        ((SPI_HandleTypeDef *)spi->priv)->Init.Mode = SPI_MODE_MASTER;
    }
    else if(mode == SPI_SLAVE)
    {
        ((SPI_HandleTypeDef *)spi->priv)->Init.Mode = SPI_MODE_SLAVE;
    }
    LL_SPI_SetMode((SPI_TypeDef *)HANDLE_REG_BASE(spi), ((SPI_HandleTypeDef *)spi->priv)->Init.Mode);
//    HAL_SPI_Init((SPI_HandleTypeDef *)(spi->priv));
    return CSI_OK;
}
csi_error_t csi_spi_cp_format(csi_spi_t *spi, csi_spi_cp_format_t format)
{
    if(spi == NULL || HANDLE_REG_BASE(spi) == 0)
    {
        return CSI_ERROR;
    }
    if(spi->priv == NULL)
    {
        return CSI_ERROR;
    }
    switch(format)
    {
        case SPI_FORMAT_CPOL0_CPHA0:
            ((SPI_HandleTypeDef *)spi->priv)->Init.SPO = SPI_SPO_LOW;
            ((SPI_HandleTypeDef *)spi->priv)->Init.SPH = SPI_SPH_1EDGE;
        break;
        case SPI_FORMAT_CPOL0_CPHA1:
            ((SPI_HandleTypeDef *)spi->priv)->Init.SPO = SPI_SPO_LOW;
            ((SPI_HandleTypeDef *)spi->priv)->Init.SPH = SPI_SPH_2EDGE;
        break;
        case SPI_FORMAT_CPOL1_CPHA0:
            ((SPI_HandleTypeDef *)spi->priv)->Init.SPO = SPI_SPO_HIGH;
            ((SPI_HandleTypeDef *)spi->priv)->Init.SPH = SPI_SPH_1EDGE;
        break;
        case SPI_FORMAT_CPOL1_CPHA1:
            ((SPI_HandleTypeDef *)spi->priv)->Init.SPO = SPI_SPO_HIGH;
            ((SPI_HandleTypeDef *)spi->priv)->Init.SPH = SPI_SPH_2EDGE;
        break;
        default: return CSI_ERROR;
    }
    LL_SPI_SetClockPolarity((SPI_TypeDef *)HANDLE_REG_BASE(spi), ((SPI_HandleTypeDef *)spi->priv)->Init.SPO);
    LL_SPI_SetClockPhase((SPI_TypeDef *)HANDLE_REG_BASE(spi), ((SPI_HandleTypeDef *)spi->priv)->Init.SPH);
//    HAL_SPI_Init((SPI_HandleTypeDef *)(spi->priv));
    return CSI_OK;
}
csi_error_t csi_spi_frame_len(csi_spi_t *spi, csi_spi_frame_len_t length)
{
    if(spi == NULL || HANDLE_REG_BASE(spi) == 0)
    {
        return CSI_ERROR;
    }
    if(spi->priv == NULL)
    {
        return CSI_ERROR;
    }
    switch(length)
    {
        case SPI_FRAME_LEN_4:
            ((SPI_HandleTypeDef *)spi->priv)->Init.DataSize = SPI_DATASIZE_4BIT;
        break;
        case SPI_FRAME_LEN_5:
            ((SPI_HandleTypeDef *)spi->priv)->Init.DataSize = SPI_DATASIZE_5BIT;
        break;
        case SPI_FRAME_LEN_6:
            ((SPI_HandleTypeDef *)spi->priv)->Init.DataSize = SPI_DATASIZE_6BIT;
        break;
        case SPI_FRAME_LEN_7:
            ((SPI_HandleTypeDef *)spi->priv)->Init.DataSize = SPI_DATASIZE_7BIT;
        break;
        case SPI_FRAME_LEN_8:
            ((SPI_HandleTypeDef *)spi->priv)->Init.DataSize = SPI_DATASIZE_8BIT;
        break;
        case SPI_FRAME_LEN_9:
            ((SPI_HandleTypeDef *)spi->priv)->Init.DataSize = SPI_DATASIZE_9BIT;
        break;
        case SPI_FRAME_LEN_10:
            ((SPI_HandleTypeDef *)spi->priv)->Init.DataSize = SPI_DATASIZE_10BIT;
        break;
        case SPI_FRAME_LEN_11:
            ((SPI_HandleTypeDef *)spi->priv)->Init.DataSize = SPI_DATASIZE_11BIT;
        break;
        case SPI_FRAME_LEN_12:
            ((SPI_HandleTypeDef *)spi->priv)->Init.DataSize = SPI_DATASIZE_12BIT;
        break;
        case SPI_FRAME_LEN_13:
            ((SPI_HandleTypeDef *)spi->priv)->Init.DataSize = SPI_DATASIZE_13BIT;
        break;
        case SPI_FRAME_LEN_14:
            ((SPI_HandleTypeDef *)spi->priv)->Init.DataSize = SPI_DATASIZE_14BIT;
        break;
        case SPI_FRAME_LEN_15:
            ((SPI_HandleTypeDef *)spi->priv)->Init.DataSize = SPI_DATASIZE_15BIT;
        break;
        case SPI_FRAME_LEN_16:
            ((SPI_HandleTypeDef *)spi->priv)->Init.DataSize = SPI_DATASIZE_16BIT;
        break;
        default: return CSI_ERROR;
    }
    LL_SPI_SetDataWidth((SPI_TypeDef *)HANDLE_REG_BASE(spi), ((SPI_HandleTypeDef *)spi->priv)->Init.DataSize);
//    HAL_SPI_Init((SPI_HandleTypeDef *)(spi->priv));
    return CSI_OK;
}
uint32_t csi_spi_baud(csi_spi_t *spi, uint32_t baud)
{
    if(spi == NULL || HANDLE_REG_BASE(spi) == 0)
    {
        return 0;
    }
    if(spi->priv == NULL)
    {
        return 0;
    }
    /* bitrate = Fspiclk/(CPSDVR*(1 + SCR))
     * */
    uint32_t freq = (uint32_t)drv_get_apb_freq();
    uint32_t div = (freq/baud);
    uint32_t temp_cpsdvr = 0;
    uint32_t temp_scr = 0;
    for(uint32_t i = 2; i <= 254; i+=2)
    {
        temp_scr = div/i - 1;
        if(temp_scr <= 255)
        {
            temp_cpsdvr = i;
            break;
        }
    }
    if((temp_cpsdvr < 2) || (temp_cpsdvr > 254) || (temp_scr > 255))
    {
        return 0;
    }
    ((SPI_HandleTypeDef *)spi->priv)->Init.CPSDVR = temp_cpsdvr;    // 时钟预分频系数 取值范围2-254之前的偶数
    ((SPI_HandleTypeDef *)spi->priv)->Init.SCR = temp_scr;          // 0‐255
    LL_SPI_SetBaudRatePrescaler((SPI_TypeDef *)HANDLE_REG_BASE(spi), temp_cpsdvr, temp_scr);
//    HAL_SPI_Init((SPI_HandleTypeDef *)(spi->priv));
    return baud;
}
int32_t csi_spi_send(csi_spi_t *spi, const void *data, uint32_t size, uint32_t timeout)
{
    if(spi == NULL || data == NULL || size == 0)
    {
        return 0;
    }
    if(spi->priv == NULL)
    {
        return 0;
    }
    spi->tx_data = (void *)data;
    spi->tx_size = size;
    if(HAL_SPI_Transmit((SPI_HandleTypeDef *)(spi->priv), (uint8_t *)data, size, timeout) != HAL_OK)
    {
        return 0;
    }
    return size;
}
csi_error_t csi_spi_send_async(csi_spi_t *spi, const void *data, uint32_t size)
{
    if(spi == NULL || data == NULL || size == 0)
    {
        return CSI_ERROR;
    }
    if(spi->priv == NULL)
    {
        return CSI_ERROR;
    }
    spi->tx_data = (void *)data;
    spi->tx_size = size;
    if(HAL_SPI_Transmit_IT((SPI_HandleTypeDef *)(spi->priv), (uint8_t *)data, size) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
uint32_t csi_spi_receive(csi_spi_t *spi, void *data, uint32_t size, uint32_t timeout)
{
    if(spi == NULL || data == NULL || size == 0)
    {
        return 0;
    }
    if(spi->priv == NULL)
    {
        return 0;
    }
    spi->rx_data = (void *)data;
    spi->rx_size = size;
    if(HAL_SPI_Receive((SPI_HandleTypeDef *)(spi->priv), (uint8_t *)data, size, timeout) != HAL_OK)
    {
        return 0;
    }
    return size;
}
csi_error_t csi_spi_receive_async(csi_spi_t *spi, void *data, uint32_t size)
{
    if(spi == NULL || data == NULL || size == 0)
    {
        return CSI_ERROR;
    }
    if(spi->priv == NULL)
    {
        return CSI_ERROR;
    }
    spi->rx_data = (void *)data;
    spi->rx_size = size;
    if(HAL_SPI_Receive_IT((SPI_HandleTypeDef *)(spi->priv), (uint8_t *)data, size) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
void csi_spi_select_slave(csi_spi_t *spi, int slave_num)
{
    (void)spi;
    (void)slave_num;
}
csi_error_t csi_spi_link_dma(csi_spi_t *spi, csi_dma_ch_t *tx_dma, csi_dma_ch_t *rx_dma)
{
    (void)spi;
    (void)tx_dma;
    (void)rx_dma;
    return CSI_UNSUPPORTED;
}
csi_error_t csi_spi_get_state(csi_spi_t *spi, csi_state_t *state)
{
    (void)spi;
    (void)state;
    return CSI_UNSUPPORTED;
}







#endif /* CONFIG_CSI_DRV_SPI_ENABLED */
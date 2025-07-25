#include "csi_uart.h"
#ifdef CONFIG_CSI_DRV_UART_ENABLED

static UART_HandleTypeDef handle_uart[CSI_UART_CNT];

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_UART0_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_UART0_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    HAL_UART_IRQHandler(&handle_uart[0]);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_UART1_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_UART1_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    HAL_UART_IRQHandler(&handle_uart[1]);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_UART2_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_UART2_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    HAL_UART_IRQHandler(&handle_uart[2]);
    
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


static void CSI_HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart != NULL)
    {
        if(huart->priv_obj != NULL)
        {
            if(((csi_uart_t *)huart->priv_obj)->callback != NULL)
            {
                ((csi_uart_t *)huart->priv_obj)->callback((csi_uart_t *)huart->priv_obj, UART_EVENT_SEND_COMPLETE, ((csi_uart_t *)huart->priv_obj)->arg);
            }
        }
    }
}
static void CSI_HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart != NULL)
    {
        if(huart->priv_obj != NULL)
        {
            if(((csi_uart_t *)huart->priv_obj)->callback != NULL)
            {
                ((csi_uart_t *)huart->priv_obj)->callback((csi_uart_t *)huart->priv_obj, UART_EVENT_RECEIVE_COMPLETE, ((csi_uart_t *)huart->priv_obj)->arg);
            }
        }
    }
}
static void CSI_HAL_UART_IDLECallback(UART_HandleTypeDef *huart)
{
    if(huart != NULL)
    {
        if(huart->priv_obj != NULL)
        {
            if(((csi_uart_t *)huart->priv_obj)->callback != NULL)
            {
                ((csi_uart_t *)huart->priv_obj)->callback((csi_uart_t *)huart->priv_obj, UART_EVENT_IDLE, ((csi_uart_t *)huart->priv_obj)->arg);
            }
        }
    }
}

csi_error_t csi_uart_init(csi_uart_t *uart, uint32_t idx)
{
    if(uart == NULL || idx >= CSI_UART_CNT)
    {
        return CSI_ERROR;
    }
    if(idx == 0)
    {
        HANDLE_REG_BASE(uart) = UART0_BASE;
        HANDLE_IRQ_NUM(uart) = UART0_IRQn;
        HANDLE_DEV_IDX(uart) = 0;
        HANDLE_IRQ_HANDLER(uart) = (void *)CSI_UART0_IRQHandler;
    }
    else if(idx == 1)
    {
        HANDLE_REG_BASE(uart) = UART1_BASE;
        HANDLE_IRQ_NUM(uart) = UART1_IRQn;
        HANDLE_DEV_IDX(uart) = 1;
        HANDLE_IRQ_HANDLER(uart) = (void *)CSI_UART1_IRQHandler;
    }
    else if(idx == 2)
    {
        HANDLE_REG_BASE(uart) = UART2_BASE;
        HANDLE_IRQ_NUM(uart) = UART2_IRQn;
        HANDLE_DEV_IDX(uart) = 2;
        HANDLE_IRQ_HANDLER(uart) = (void *)CSI_UART2_IRQHandler;
    }
    handle_uart[idx].priv_obj = uart;
    uart->priv = (void *)&handle_uart[idx];
    uart->tx_dma = NULL;
    uart->rx_dma = NULL;
    ((UART_HandleTypeDef *)uart->priv)->Instance = (UART_TypeDef *)HANDLE_REG_BASE(uart);
    HAL_UART_StructInit(&(((UART_HandleTypeDef *)(uart->priv))->Init));
    HAL_UART_Init((UART_HandleTypeDef *)(uart->priv));
    HAL_UART_RegisterCallback((UART_HandleTypeDef *)(uart->priv), HAL_UART_TX_COMPLETE_CB_ID, CSI_HAL_UART_TxCpltCallback);
    HAL_UART_RegisterCallback((UART_HandleTypeDef *)(uart->priv), HAL_UART_RX_COMPLETE_CB_ID, CSI_HAL_UART_RxCpltCallback);
    HAL_UART_RegisterCallback((UART_HandleTypeDef *)(uart->priv), HAL_UART_IDLE_CB_ID, CSI_HAL_UART_IDLECallback);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(HANDLE_IRQ_NUM(uart), CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, HANDLE_IRQ_HANDLER(uart));
#else
    csi_clic_register_irq(HANDLE_IRQ_NUM(uart), CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, HANDLE_IRQ_HANDLER(uart));
#endif
    return CSI_OK;
}
void csi_uart_uninit(csi_uart_t *uart)
{
    if(uart == NULL)
    {
        return;
    }
    HANDLE_REG_BASE(uart) = 0;
    HANDLE_IRQ_NUM(uart) = 0;
    HANDLE_DEV_IDX(uart) = 0;
    HANDLE_IRQ_HANDLER(uart) = NULL;
    uart->priv = NULL;
}

csi_error_t csi_uart_baud(csi_uart_t *uart, uint32_t baud)
{
    if(uart == NULL)
    {
        return CSI_ERROR;
    }
    if(uart->priv == NULL)
    {
        return CSI_ERROR;
    }
    ((UART_HandleTypeDef *)uart->priv)->Init.BaudRate = baud;
    HAL_UART_Init((UART_HandleTypeDef *)(uart->priv));
    return CSI_OK;
}
csi_error_t csi_uart_format(csi_uart_t *uart, csi_uart_data_bits_t data_bits, csi_uart_parity_t parity, csi_uart_stop_bits_t stop_bits)
{
    if(uart == NULL)
    {
        return CSI_ERROR;
    }
    if(uart->priv == NULL)
    {
        return CSI_ERROR;
    }
    switch(data_bits)
    {
        case UART_DATA_BITS_5: break;
        case UART_DATA_BITS_6: break;
        case UART_DATA_BITS_7: break;
        case UART_DATA_BITS_8:
            ((UART_HandleTypeDef *)uart->priv)->Init.WordLength = UART_WORDLENGTH_8B;
        break;
        case UART_DATA_BITS_9:
            ((UART_HandleTypeDef *)uart->priv)->Init.WordLength = UART_WORDLENGTH_9B;
        break;
        default:
        break;
    }
    switch(parity)
    {
        case CSI_UART_PARITY_NONE: 
            ((UART_HandleTypeDef *)uart->priv)->Init.Parity = UART_PARITY_NONE;
        break;
        case CSI_UART_PARITY_EVEN:
            ((UART_HandleTypeDef *)uart->priv)->Init.Parity = UART_PARITY_EVEN;
        break;
        case CSI_UART_PARITY_ODD: 
            ((UART_HandleTypeDef *)uart->priv)->Init.Parity = UART_PARITY_ODD;
        break;
        default:
        break;
    }
    switch(stop_bits)
    {
        case UART_STOP_BITS_1: 
            ((UART_HandleTypeDef *)uart->priv)->Init.StopBits = UART_STOPBITS_1;
        break;
        case UART_STOP_BITS_2:
            ((UART_HandleTypeDef *)uart->priv)->Init.StopBits = UART_STOPBITS_2;
        break;
        case UART_STOP_BITS_1_5: 
            ((UART_HandleTypeDef *)uart->priv)->Init.StopBits = UART_STOPBITS_1_5;
        break;
        default:
        break;
    }
    HAL_UART_Init((UART_HandleTypeDef *)(uart->priv));
    return CSI_OK;
}
csi_error_t csi_uart_flowctrl(csi_uart_t *uart,  csi_uart_flowctrl_t flowctrl)
{
    if(uart == NULL)
    {
        return CSI_ERROR;
    }
    if(uart->priv == NULL)
    {
        return CSI_ERROR;
    }
    switch(flowctrl)
    {
        case UART_FLOWCTRL_NONE: 
            ((UART_HandleTypeDef *)uart->priv)->Init.HwFlowCtl = UART_HWCONTROL_NONE;
        break;
        case UART_FLOWCTRL_RTS:
            ((UART_HandleTypeDef *)uart->priv)->Init.HwFlowCtl = UART_HWCONTROL_RTS;
        break;
        case UART_FLOWCTRL_CTS: 
            ((UART_HandleTypeDef *)uart->priv)->Init.HwFlowCtl = UART_HWCONTROL_CTS;
        break;
        case UART_FLOWCTRL_RTS_CTS: 
            ((UART_HandleTypeDef *)uart->priv)->Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
        break;
        default:
        break;
    }
    HAL_UART_Init((UART_HandleTypeDef *)(uart->priv));
    return CSI_OK;
}

csi_error_t csi_uart_idle_ctrl(csi_uart_t *uart, bool enable)
{
    if(uart == NULL)
    {
        return CSI_ERROR;
    }
    if(enable)
    {
        __HAL_UART_CLEAR_IDLEFLAG((UART_HandleTypeDef *)(uart->priv));
        __HAL_UART_ENABLE_IT((UART_HandleTypeDef *)(uart->priv), UART_IT_IDLE);
    }
    else
    {
        __HAL_UART_CLEAR_IDLEFLAG((UART_HandleTypeDef *)(uart->priv));
        __HAL_UART_DISABLE_IT((UART_HandleTypeDef *)(uart->priv), UART_IT_IDLE);
    }
    return CSI_OK;
}
uint32_t csi_uart_get_counter(csi_uart_t *uart)
{
    if(uart == NULL)
    {
        return CSI_ERROR;
    }
    return (((UART_HandleTypeDef *)uart->priv)->RxXferSize - ((UART_HandleTypeDef *)uart->priv)->RxXferCount);
}
csi_error_t csi_uart_attach_callback(csi_uart_t *uart, void(*callback)(struct _csi_uart *uart, csi_uart_event_t event, void *args), void *arg)
{
    if(uart == NULL || callback == NULL)
    {
        return CSI_ERROR;
    }
    if(uart->priv == NULL)
    {
        return CSI_ERROR;
    }
    uart->callback = callback;
    uart->arg = arg;
    return CSI_OK;
}
void csi_uart_detach_callback(csi_uart_t *uart)
{
    if(uart == NULL)
    {
        return;
    }
    if(uart->priv == NULL)
    {
        return;
    }
    uart->callback = NULL;
    uart->arg = NULL;
}
csi_error_t csi_uart_link_dma(csi_uart_t *uart, csi_dma_ch_t *tx_dma, csi_dma_ch_t *rx_dma)
{
    if(uart == NULL)
    {
        return CSI_ERROR;
    }
    if(uart->priv == NULL)
    {
        return CSI_ERROR;
    }
    uart->tx_dma = tx_dma;
    uart->rx_dma = rx_dma;
    if(uart->tx_dma != NULL)
    {
        ((UART_HandleTypeDef *)uart->priv)->hdmatx = (DMA_HandleTypeDef *)uart->tx_dma->priv;
        ((DMA_HandleTypeDef *)tx_dma->priv)->Parent = uart->priv;
    }
    if(uart->rx_dma != NULL)
    {
        ((UART_HandleTypeDef *)uart->priv)->hdmarx = (DMA_HandleTypeDef *)uart->rx_dma->priv;
        ((DMA_HandleTypeDef *)rx_dma->priv)->Parent = uart->priv;
        __HAL_UART_CLEAR_IDLEFLAG((UART_HandleTypeDef *)(uart->priv));
        __HAL_UART_ENABLE_IT((UART_HandleTypeDef *)(uart->priv), UART_IT_IDLE);
    }
    return CSI_OK;
}

int32_t csi_uart_send(csi_uart_t *uart, const void *data, uint32_t size, uint32_t timeout)
{
    if(uart == NULL || data == NULL)
    {
        return CSI_ERROR;
    }
    if(uart->priv == NULL)
    {
        return CSI_ERROR;
    }
    uart->tx_data = (uint8_t *)data;
    uart->tx_size = size;
    if(HAL_UART_Transmit((UART_HandleTypeDef *)uart->priv, uart->tx_data, uart->tx_size, timeout) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return size;
}
csi_error_t csi_uart_send_async(csi_uart_t *uart, const void *data, uint32_t size)
{
    if(uart == NULL || data == NULL)
    {
        return CSI_ERROR;
    }
    if(uart->priv == NULL)
    {
        return CSI_ERROR;
    }
    uart->tx_data = (uint8_t *)data;
    uart->tx_size = size;
    // DMA TX
    if(uart->tx_dma != NULL)
    {
        if(HAL_UART_Transmit_DMA((UART_HandleTypeDef *)uart->priv, uart->tx_data, uart->tx_size) != HAL_OK)
        {
            return CSI_ERROR;
        }
    }
    // IT TX
    else 
    {
        if(HAL_UART_Transmit_IT((UART_HandleTypeDef *)uart->priv, uart->tx_data, uart->tx_size) != HAL_OK)
        {
            return CSI_ERROR;
        }
    }
    return CSI_OK;
}

int32_t csi_uart_receive(csi_uart_t *uart, void *data, uint32_t size, uint32_t timeout)
{
    if(uart == NULL || data == NULL)
    {
        return CSI_ERROR;
    }
    if(uart->priv == NULL)
    {
        return CSI_ERROR;
    }
    uart->rx_data = (uint8_t *)data;
    uart->rx_size = size;
    if(HAL_UART_Receive((UART_HandleTypeDef *)uart->priv, uart->rx_data, uart->rx_size, timeout) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return size;
}
csi_error_t csi_uart_receive_async(csi_uart_t *uart, void *data, uint32_t size)
{
    if(uart == NULL || data == NULL)
    {
        return CSI_ERROR;
    }
    if(uart->priv == NULL)
    {
        return CSI_ERROR;
    }
    uart->rx_data = (uint8_t *)data;
    uart->rx_size = size;
    // DMA RX
    if(uart->rx_dma != NULL)
    {
        if(HAL_UART_Receive_DMA((UART_HandleTypeDef *)uart->priv, uart->rx_data, uart->rx_size) != HAL_OK)
        {
            return CSI_ERROR;
        }
    }
    else// IT RX
    {
        if(HAL_UART_Receive_IT((UART_HandleTypeDef *)uart->priv, uart->rx_data, uart->rx_size) != HAL_OK)
        {
            return CSI_ERROR;
        }
    }
    return CSI_OK;
}
uint8_t csi_uart_getc(csi_uart_t *uart)
{
    if(uart == NULL)
    {
        return CSI_ERROR;
    }
    if(uart->priv == NULL)
    {
        return CSI_ERROR;
    }
    return HAL_UART_ReceiveData(((UART_HandleTypeDef *)uart->priv)->Instance) & 0xFF;
}
void csi_uart_putc(csi_uart_t *uart, uint8_t ch)
{
    if(uart == NULL)
    {
        return;
    }
    if(uart->priv == NULL)
    {
        return;
    }
    HAL_UART_SendData(((UART_HandleTypeDef *)uart->priv)->Instance, ch);
}

#endif /* CONFIG_CSI_DRV_UART_ENABLED */
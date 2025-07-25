#include "at_hal.h"
#if(defined CONFIG_AT_CLIENT_ENABLE && CONFIG_AT_CLIENT_ENABLE == 1U)

static struct device s_dev;

static void CHECK_RETURN(csi_error_t ret)
{
    if (ret != CSI_OK) {
        while(1);
    }
}

device_t at_device_find(const char *name)
{
    rt_size_t len = rt_strlen(name); 
    rt_memcpy(s_dev.name, name, (len > RT_DEVICE_NAME_MAX ? RT_DEVICE_NAME_MAX : len));
    return &s_dev;
}
static rt_uint32_t at_hal_get_uart_counter(device_t dev, struct _csi_uart *uart)
{
    rt_uint32_t counter = csi_uart_get_counter(uart);
    if(counter >= dev->get_index)
    {
        return (counter - dev->get_index);
    }
    return 0;
}
static void uart_event_cb(struct _csi_uart *uart, csi_uart_event_t event, void *arg){
    device_t dev = (device_t)arg;
    switch (event) {
        case UART_EVENT_SEND_COMPLETE:
            break;
        case UART_EVENT_RECEIVE_COMPLETE:
        {
            if(dev->oflag & AT_DEVICE_FLAG_INT_RX)
            {
                rt_uint32_t counter = at_hal_get_uart_counter(dev, uart);
                if(counter != 0 && dev->get_index < AT_DEVICE_RX_BUFFER_SIZE)
                {
                    ringbuffer_in(dev->rb_p, &dev->rx_buff[dev->get_index], counter);
                    dev->get_index = 0;
                    csi_uart_receive_async(uart, &dev->rx_buff[0], AT_DEVICE_RX_BUFFER_SIZE);
                    if(dev->rx_indicate)
                    {
                        dev->rx_indicate(dev, 1);
                    }
                }
            }
            #if(defined AT_DEVICE_RX_USING_DMA && AT_DEVICE_RX_USING_DMA == 1U)
            else if(dev->oflag & AT_DEVICE_FLAG_DMA_RX)
            {
                csi_uart_receive_async(uart, &dev->rx_dma_buff[0], AT_DEVICE_RX_DMA_BUFFER_SIZE);
            }
            #endif
        }break;
        case UART_EVENT_IDLE:
        {
            #if(defined AT_DEVICE_RX_USING_DMA && AT_DEVICE_RX_USING_DMA == 1)
            if(dev->oflag & AT_DEVICE_FLAG_DMA_RX)
            {
                rt_uint32_t counter = csi_dma_ch_get_counter(uart->rx_dma);
                if(counter > AT_DEVICE_RX_DMA_BUFFER_SIZE)
                {
                    assert_param(0);
                }
                else if(counter != 0)
                {
                    ringbuffer_in(dev->rb_p, dev->rx_dma_buff, counter);
                    if(dev->oflag & AT_DEVICE_FLAG_DMA_RX)
                    {
                        csi_uart_receive_async(uart, &dev->rx_dma_buff[0], AT_DEVICE_RX_DMA_BUFFER_SIZE);
                    }
                    if(dev->rx_indicate)
                    {
                        dev->rx_indicate(dev, counter);
                    }
                }
            }
            #endif
            if(dev->oflag & AT_DEVICE_FLAG_INT_RX)
            {
                rt_uint32_t counter = at_hal_get_uart_counter(dev, uart);
                if(counter != 0)
                {
                    ringbuffer_in(dev->rb_p, &dev->rx_buff[dev->get_index], counter);
                    dev->get_index += counter;
                    if(dev->rx_indicate)
                    {
                        dev->rx_indicate(dev, counter);
                    }
                }
            }
        }break;
        default:
            break;
    }
}
#if(defined AT_DEVICE_TX_USING_DMA && AT_DEVICE_TX_USING_DMA == 1U)
static void csi_tx_dma_callback(csi_dma_event_t event, void *arg)
{
}
#endif
#if(defined AT_DEVICE_RX_USING_DMA && AT_DEVICE_RX_USING_DMA == 1U)
static void csi_rx_dma_callback(csi_dma_event_t event, void *arg)
{
}
#endif
rt_err_t at_device_open(device_t dev, rt_uint16_t oflag)
{
    if(dev == RT_NULL)
    {
        return RT_EINVAL;
    }
    dev->oflag = oflag;
    #if(defined AT_DEVICE_RX_USING_DMA && AT_DEVICE_RX_USING_DMA != 1U)
    if((dev->oflag & AT_DEVICE_FLAG_DMA_RX) != 0)//DMA RX
    {
        return RT_EIO;
    }
    #endif
    if(dev->rb_p == NULL)
    {
        dev->rb_p = ringbuffer_create(AT_DEVICE_RX_BUFFER_SIZE);
    }
    csi_error_t ret;
    /* init uart */
    ret = csi_uart_init(&dev->uart, AT_DEVICE_UART_IDX);
    CHECK_RETURN(ret);
    /* set uart baudrate */
    ret = csi_uart_baud(&dev->uart, AT_DEVICE_BAUDRATE);
    CHECK_RETURN(ret);
    /* set uart format */
    ret = csi_uart_format(&dev->uart, AT_DEVICE_DATA_BITS, AT_DEVICE_PARITY, AT_DEVICE_STOPBITS);
    CHECK_RETURN(ret);
    /* attach callback to uart device */
    ret = csi_uart_attach_callback(&dev->uart, uart_event_cb, dev);
    CHECK_RETURN(ret);

#if((defined AT_DEVICE_RX_USING_DMA && AT_DEVICE_RX_USING_DMA == 1) || \
    (defined AT_DEVICE_TX_USING_DMA && AT_DEVICE_TX_USING_DMA == 1))
    ret = csi_dma_ctrl_init(&dev->dma_ctrl, 0);
    CHECK_RETURN(ret);
    csi_dma_ch_config_t config;
#endif
#if(defined AT_DEVICE_TX_USING_DMA && AT_DEVICE_TX_USING_DMA == 1U)
    ret = csi_dma_ch_alloc(&dev->tx_dma_ch, 0, 0);
    CHECK_RETURN(ret);
    /* TX DMA Config */
    config.src_inc = DMA_ADDR_INC;
    config.dst_inc = DMA_ADDR_CONSTANT;
    config.src_tw = DMA_DATA_WIDTH_8_BITS;
    config.dst_tw = DMA_DATA_WIDTH_8_BITS;
    config.dw.src_hs_if = DMA_HS_MEMORY;
    config.dw.dst_hs_if = AT_DEVICE_DMA_TXHS;
    config.dw.src_reload_en = false;
    config.dw.dst_reload_en = false;
    config.dw.src_burst_len = 0;
    config.dw.dst_burst_len = 0;
    config.dw.trans_dir = DMA_MEM2PERH;
    //作为UART搬运数据时，注册的回调函数会被UART进行替换，所以当前注册回调函数是无效的
    ret = csi_dma_ch_config(&dev->tx_dma_ch, &config, NULL, NULL);
    CHECK_RETURN(ret);
#endif
    
#if(defined AT_DEVICE_RX_USING_DMA && AT_DEVICE_RX_USING_DMA == 1U)
    ret = csi_dma_ch_alloc(&dev->rx_dma_ch, 1, 0);
    CHECK_RETURN(ret);
    /* RX DMA Config */
    config.src_inc = DMA_ADDR_CONSTANT;
    config.dst_inc = DMA_ADDR_INC;
    config.src_tw = DMA_DATA_WIDTH_8_BITS;
    config.dst_tw = DMA_DATA_WIDTH_8_BITS;
    config.dw.src_hs_if = AT_DEVICE_DMA_RXHS;
    config.dw.dst_hs_if = DMA_HS_MEMORY;
    config.dw.src_reload_en = false;
    config.dw.dst_reload_en = false;
    config.dw.src_burst_len = 0;
    config.dw.dst_burst_len = 0;
    config.dw.trans_dir = DMA_PERH2MEM;
    ret = csi_dma_ch_config(&dev->rx_dma_ch, &config, NULL, NULL);
    CHECK_RETURN(ret);    
#endif
#if((defined AT_DEVICE_RX_USING_DMA && AT_DEVICE_RX_USING_DMA == 1) && \
    (defined AT_DEVICE_TX_USING_DMA && AT_DEVICE_TX_USING_DMA == 1))
    csi_uart_link_dma(&dev->uart, &dev->tx_dma_ch, &dev->rx_dma_ch);
#elif((defined AT_DEVICE_RX_USING_DMA && AT_DEVICE_RX_USING_DMA == 1))
    csi_uart_link_dma(&dev->uart, NULL, &dev->rx_dma_ch);
#elif((defined AT_DEVICE_TX_USING_DMA && AT_DEVICE_TX_USING_DMA == 1))
    csi_uart_link_dma(&dev->uart, &dev->tx_dma_ch, NULL);
#endif
    csi_uart_idle_ctrl(&dev->uart, true);
    at_device_restart_receive_async(dev);
    return RT_EOK;
}
rt_err_t at_device_restart_receive_async(device_t dev)
{
    if(dev == RT_NULL)
    {
        return RT_EINVAL;
    }
#if((defined AT_DEVICE_RX_USING_DMA && AT_DEVICE_RX_USING_DMA == 1))
    // dev->rx_dma_buff = (char *)0x02000100;
    if((dev->oflag & AT_DEVICE_FLAG_DMA_RX) != 0)//DMA RX
    {
        csi_uart_receive_async(&dev->uart, &dev->rx_dma_buff[0], AT_DEVICE_RX_DMA_BUFFER_SIZE);
    }
#endif
    if((dev->oflag & AT_DEVICE_FLAG_INT_RX) != 0)//INT RX
    {
        dev->get_index = 0;
        csi_uart_receive_async(&dev->uart, &dev->rx_buff[0], AT_DEVICE_RX_BUFFER_SIZE);
    }
    return RT_EOK;
}
rt_size_t at_device_write(device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    // at_device_restart_receive_async(dev);
#if(defined AT_DEVICE_TX_USING_INT && AT_DEVICE_TX_USING_INT == 1U)
    if(csi_uart_send_async(&dev->uart, buffer, size) == CSI_OK)
    {
        return size;
    }
#else
    if (csi_uart_send(&dev->uart, buffer, size, HAL_MAX_DELAY) == size)
    {
        return size;
    }
#endif
    return 0;
}
rt_size_t at_device_read(device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    return ringbuffer_out(dev->rb_p, buffer, size);
}
rt_err_t at_device_close(device_t dev)
{
    if(dev == NULL)
    {
        return RT_EOK;
    }
    ringbuffer_destroy(dev->rb_p);
    return RT_EOK;
}


rt_err_t at_device_set_rx_indicate(device_t dev, rt_err_t (*rx_ind)(device_t dev, rt_size_t size))
{
    assert_param(dev != NULL);
    dev->rx_indicate = rx_ind;
    return RT_EOK;
}


rt_ringbuffer_t rt_ringbuffer_create(rt_uint16_t size)
{
    return ringbuffer_create(size);
}
rt_size_t rt_ringbuffer_getchar(rt_ringbuffer_t rb, rt_uint8_t *ch)
{
    return ringbuffer_out(rb, ch, 1);
}
void rt_ringbuffer_destroy(struct ringbuffer *rb)
{
    ringbuffer_destroy(rb);
}
rt_size_t rt_ringbuffer_put_force(rt_ringbuffer_t rb, const rt_uint8_t *ptr, rt_uint16_t length)
{
    return ringbuffer_in(rb, ptr, length);
}


device_t at_console_get_device(void)
{
    return NULL;
}
#endif /* CONFIG_AT_CLIENT_ENABLE */
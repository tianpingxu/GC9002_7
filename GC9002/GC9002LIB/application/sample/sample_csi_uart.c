#include "sample_def.h"
#include "csi_uart.h"
#include "csi_irq.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_UART_

static csi_uart_t g_uart;
static uint8_t *recv_buf;
static uint8_t *send_buf;
#define EXAMPLE_UART_IDX        1
#define EXAMPLE_UART_BAUDRATE   115200
#define CHECK_RETURN(ret)                      \
        do {                                        \
            if (ret != CSI_OK) {                    \
                while(1);                          \
            }                                       \
        } while(0);


static csi_dma_ch_t g_dma_ch_tx;
static csi_dma_ch_t g_dma_ch_rx;
static volatile uint8_t rx_async_flag = 0;
static volatile uint8_t tx_async_flag = 0;
static volatile uint8_t idle_flag = 0;
static void uart_event_cb(struct _csi_uart *uart, csi_uart_event_t event, void *arg){
    switch (event) {
        case UART_EVENT_SEND_COMPLETE:
            tx_async_flag = 1;
            break;
        case UART_EVENT_RECEIVE_COMPLETE:
            rx_async_flag = 1;
            break;
        case UART_EVENT_IDLE:
            idle_flag = 1;
            break;
        default:
            break;
    }
}
#define TEST_SYNC   2//0 普通TX  1中断TX  2DMA TX
#if(TEST_SYNC == 2)
static csi_dma_ctrl_t dma_ctrl;
static csi_dma_ch_t tx_dma_ch;
static csi_dma_ch_t rx_dma_ch;
static void csi_tx_dma_callback(csi_dma_event_t event, void *arg)
{
    printf("%s\n", (char *)arg);
}
static void csi_rx_dma_callback(csi_dma_event_t event, void *arg)
{
    printf("%s\n", (char *)arg);
}
#endif

static DMA_HandleTypeDef tx_hdma =
{
    .Instance = DMA,
};
#define TEST_SEND_STRING        "hello world\n"
#define TEST_SEND_LENGTH        strlen(TEST_SEND_STRING)
#define TEST_RECV_LENGTH        4
DMA_HandleTypeDef hdma;
int main(void)
{
    csi_error_t ret;
    uint8_t buff[16];
    memset(buff, 0, sizeof(buff));
    hdma.Instance = DMA;
    send_buf = (uint8_t *)0x02000000;
    recv_buf = (uint8_t *)0x02000100;
    /* init uart */
    ret = csi_uart_init(&g_uart, EXAMPLE_UART_IDX);
    CHECK_RETURN(ret);

    /* set uart baudrate */
    ret = csi_uart_baud(&g_uart, EXAMPLE_UART_BAUDRATE);
    CHECK_RETURN(ret);

    /* set uart format */
    ret = csi_uart_format(&g_uart, UART_DATA_BITS_8, CSI_UART_PARITY_NONE, UART_STOP_BITS_1);
    CHECK_RETURN(ret);
    strcpy((char *)send_buf, TEST_SEND_STRING);
#if(TEST_SYNC == 0)
    while(1)
    {
        if (csi_uart_send(&g_uart, send_buf, TEST_SEND_LENGTH, 0xffffffff) != TEST_SEND_LENGTH)
        {
            while(1);
        }
        if (csi_uart_receive(&g_uart, recv_buf, TEST_RECV_LENGTH, 0xffffffff) != TEST_RECV_LENGTH)
        {
            while(1);
        }
    }

#elif(TEST_SYNC == 1)
    /* attach callback to uart device */
    ret = csi_uart_attach_callback(&g_uart, uart_event_cb, NULL);
    CHECK_RETURN(ret);
    
    ret = csi_uart_send_async(&g_uart, send_buf, TEST_SEND_LENGTH);
    CHECK_RETURN(ret);
    while(1) {
        if (tx_async_flag) {
            tx_async_flag = 0;
            break;
        }
    }
    ret = csi_uart_receive_async(&g_uart, recv_buf, TEST_RECV_LENGTH);
    CHECK_RETURN(ret);

    while(1) {
        if (rx_async_flag) {
            rx_async_flag = 0;
            break;
        }
    }
#elif(TEST_SYNC == 2)
    ret = csi_dma_ctrl_init(&dma_ctrl, 0);
    CHECK_RETURN(ret);
    ret = csi_dma_ch_alloc(&tx_dma_ch, 0, 0);
    CHECK_RETURN(ret);
    ret = csi_dma_ch_alloc(&rx_dma_ch, 1, 0);
    CHECK_RETURN(ret);
    csi_dma_ch_config_t config;
    /* TX DMA Config */
    config.src_inc = DMA_ADDR_INC;
    config.dst_inc = DMA_ADDR_CONSTANT;
    config.src_tw = DMA_DATA_WIDTH_8_BITS;
    config.dst_tw = DMA_DATA_WIDTH_8_BITS;
    config.dw.src_hs_if = DMA_HS_MEMORY;
    config.dw.dst_hs_if = DMA_HS_UART1_TX;
    config.dw.src_reload_en = false;
    config.dw.dst_reload_en = false;
    config.dw.src_burst_len = 0;
    config.dw.dst_burst_len = 0;
    config.dw.trans_dir = DMA_MEM2PERH;
    //作为UART搬运数据时，注册的回调函数会被UART进行替换，所以当前注册回调函数是无效的
    ret = csi_dma_ch_config(&tx_dma_ch, &config, NULL, NULL);
    CHECK_RETURN(ret);
    
    /* RX DMA Config */
    config.src_inc = DMA_ADDR_CONSTANT;
    config.dst_inc = DMA_ADDR_INC;
    config.src_tw = DMA_DATA_WIDTH_8_BITS;
    config.dst_tw = DMA_DATA_WIDTH_8_BITS;
    config.dw.src_hs_if = DMA_HS_UART1_RX;
    config.dw.dst_hs_if = DMA_HS_MEMORY;
    config.dw.src_reload_en = false;
    config.dw.dst_reload_en = false;
    config.dw.src_burst_len = 0;
    config.dw.dst_burst_len = 0;
    config.dw.trans_dir = DMA_PERH2MEM;
    ret = csi_dma_ch_config(&rx_dma_ch, &config, NULL, NULL);
    CHECK_RETURN(ret);
    
    csi_uart_link_dma(&g_uart, &tx_dma_ch, &rx_dma_ch);
    /* attach callback to uart device */
    ret = csi_uart_attach_callback(&g_uart, uart_event_cb, NULL);
    CHECK_RETURN(ret);
    uint32_t counter;
    while(1)
    {
        // for (int i = 0; i < 16; i++)
        {
            ret = csi_uart_send_async(&g_uart, send_buf, TEST_SEND_LENGTH);
            CHECK_RETURN(ret);
            while(1) {
                if (tx_async_flag) {
                    tx_async_flag = 0;
                    break;
                }
            }
        }
        
        ret = csi_uart_receive_async(&g_uart, recv_buf, 4);
        CHECK_RETURN(ret);

        while(1) {
            // counter = csi_dma_ch_get_counter(g_uart.rx_dma);
            if (rx_async_flag) {
                rx_async_flag = 0;
                break;
            }
        }
    }
#endif

    /* Uninit the uart device */
    csi_uart_uninit(&g_uart);
    while(1)
    {
		mdelay(1000);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_UART_ */
#endif /* BSP_DEBUG */
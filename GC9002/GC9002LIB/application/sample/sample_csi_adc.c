#include "sample_def.h"
#include "csi_adc.h"
#include "csi_pin.h"
#include "csi_irq.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_ADC_

static csi_adc_t g_adc;
#define CHECK_RETURN(ret)                      \
        do {                                        \
            if (ret != CSI_OK) {                    \
                while(1);                          \
            }                                       \
        } while(0);


static csi_dma_ch_t g_dma_ch_rx;
static volatile uint8_t rx_async_flag = 0;
static volatile uint8_t idle_flag = 0;
static void adc_event_cb(csi_adc_t *g_adc, csi_adc_event_t event, void *arg){
    switch (event) {
        case ADC_EVENT_CONVERT_COMPLETE:
            rx_async_flag = 1;
            break;
        case ADC_EVENT_CONVERT_HALF_DONE:
            break;
        case ADC_EVENT_ERROR:
            printf("ADC_EVENT_ERROR\n");
            break;
        default:
            break;
    }
}
#define TEST_SYNC               2//0 普通RX  1中断RX  2DMA RX
#define TEST_ADC_CHANNEL        ADC_CHANNEL_TS
#if(TEST_SYNC == 2)
static csi_dma_ctrl_t dma_ctrl;
static csi_dma_ch_t rx_dma_ch;
#endif

#define TEST_ADC_BUFFER_SIZE    6
static uint16_t rx_data[TEST_ADC_BUFFER_SIZE];

int test_func(void);

void task_test(void *p)
{
    while(1)
    {
        test_func();
    }
}
void task_idle(void *p)
{
    while(1)
    {
        printf("task_idle...\n");
        #ifdef CONFIG_KERNEL_NONE
        HAL_Delay(3000);
        #else
        csi_kernel_delay(pdMS_TO_TICKS(3000));
        #endif
    }
}

#define TEST_TASK_STK_SIZE      1024
#define TEST_TASK_PRIOR         10
#define TEST_TIME_QUANTA        100
#ifndef CONFIG_KERNEL_NONE
static k_task_handle_t g_uwTask_test;
static k_task_handle_t g_uwTask_idle;
#endif
int main(void)
{
    printf("start to execute main fun. \n");
    #ifdef CONFIG_KERNEL_NONE
    task_test(NULL);
    #else
    csi_kernel_init();
    
    csi_kernel_task_new((k_task_entry_t)task_test, "task_test", NULL, TEST_TASK_PRIOR, TEST_TIME_QUANTA, NULL, TEST_TASK_STK_SIZE, &g_uwTask_test);
    if (g_uwTask_test == NULL) {
        csi_kernel_sched_resume(0);
        printf("Fail to create task_test! \r\n");
    }
    csi_kernel_task_new((k_task_entry_t)task_idle, "task_idle", NULL, TEST_TASK_PRIOR, TEST_TIME_QUANTA, NULL, 256, &g_uwTask_idle);
    if (g_uwTask_idle == NULL) {
        csi_kernel_sched_resume(0);
        printf("Fail to create task_idle! \r\n");
    }
    
    csi_kernel_start();
    
    while(1)
    {
        printf("Hello FreeRTOS.\n");
        HAL_Delay(100*1000);
    }
    #endif
    return 0;
}
int test_func(void)
{
    int32_t data;
    csi_error_t ret;
    /* GPIO init */
    csi_pin_set_mode(PB19, GPIO_MODE_PULLNONE);
    /* Initialize ADC peripheral */
    ret = csi_adc_init(&g_adc, 0);
    CHECK_RETURN(ret);
    /* Attention:
     * If Channel is 16(ADC_CHANNEL_TS) in dma mode, Please refer to the following parameter Settings
     *  div is range 3~15
     *  div set 15, sample time must >= 10
     *  div set 14, sample time must >= 10
     *  div set 13, sample time must >= 10
     *  div set 12, sample time must >= 11
     *  div set 11, sample time must >= 11
     *  div set 10, sample time must >= 12
     *  div set 9, sample time must >= 12
     *  div set 8, sample time must >= 13
     *  div set 7, sample time must >= 13
     *  div set 6, sample time must >= 13
     *  div set 5, sample time must >= 13
     *  div set 4, sample time must >= 14
     *  div set 3, sample time must >= 14
     */
    /* Configure frequency divsion, this value can be one of(2~15) */
    if(csi_adc_freq_div(&g_adc, 3) == 0){
        while (1);
    }
    /* Configure sampling time, this value can be one of(0~15) */
    ret = csi_adc_sampling_time(&g_adc, 14);
    CHECK_RETURN(ret);
    /* Enable channel 0~16, 16 is temperature sensor channel */
    ret = csi_adc_channel_enable(&g_adc, TEST_ADC_CHANNEL, true);
    CHECK_RETURN(ret);
    /* Disable continue mode */
    ret = csi_adc_continue_mode(&g_adc, false);
    CHECK_RETURN(ret);
#if(TEST_SYNC == 0)
    while(1)
    {
        /* Trigger new conversion */
        ret = csi_adc_start(&g_adc);
        CHECK_RETURN(ret);
        /* Read result */
        data = csi_adc_read(&g_adc);
        #if(TEST_ADC_CHANNEL == 16)
        printf("result: %d, Temperature: %d\n", data, HAL_ADC_ValueToCelsius(data));
        #else
        printf("get g_adc result: %d\n", data);
        #endif
        #ifdef CONFIG_KERNEL_NONE
        HAL_Delay(3000);
        #else
        csi_kernel_delay(pdMS_TO_TICKS(1000));
        #endif
    }
#elif(TEST_SYNC == 1)
    /* Set continue mode */
    csi_adc_continue_mode(&g_adc, true);
    CHECK_RETURN(ret);
    /* Set data buffer */
    ret = csi_adc_set_buffer(&g_adc, (uint32_t *)rx_data, sizeof(rx_data));
    CHECK_RETURN(ret);
    /* attach callback to g_adc device */
    ret = csi_adc_attach_callback(&g_adc, adc_event_cb, NULL);
    CHECK_RETURN(ret);
    while(1) {
        /* Trigger new conversion */
        ret = csi_adc_start_async(&g_adc);
        CHECK_RETURN(ret);
        while (rx_async_flag == 0)
        {
            #ifdef CONFIG_KERNEL_NONE
            HAL_Delay(100);
            #else
            csi_kernel_delay(pdMS_TO_TICKS(100));
            #endif
        }
        rx_async_flag = 0;
        for (int i = 0; i < TEST_ADC_BUFFER_SIZE; i++)
        {
            #if(TEST_ADC_CHANNEL == 16)
            printf("result[%d]: %d, Temperature[%d]: %d\n", i, rx_data[i], i, HAL_ADC_ValueToCelsius(rx_data[i]));
            #else
            printf("get g_adc result[%d]: %d\n", i, rx_data[i]);
            #endif
            rx_data[i] = 0;
        }
        printf("\n");
        #ifdef CONFIG_KERNEL_NONE
        HAL_Delay(2000);
        #else
        csi_kernel_delay(pdMS_TO_TICKS(2000));
        #endif
    }
#elif(TEST_SYNC == 2)
    ret = csi_dma_ctrl_init(&dma_ctrl, 0);
    CHECK_RETURN(ret);
    ret = csi_dma_ch_alloc(&rx_dma_ch, 0, 0);
    CHECK_RETURN(ret);
    csi_dma_ch_config_t config;    
    /* RX DMA Config */
    config.src_inc = DMA_ADDR_CONSTANT;
    config.dst_inc = DMA_ADDR_INC;
    config.src_tw = DMA_DATA_WIDTH_16_BITS;
    config.dst_tw = DMA_DATA_WIDTH_16_BITS;
    config.dw.src_hs_if = DMA_HS_ADC;
    config.dw.dst_hs_if = DMA_HS_MEMORY;
    config.dw.src_reload_en = false;
    config.dw.dst_reload_en = false;
    config.dw.src_burst_len = 0;
    config.dw.dst_burst_len = 0;
    config.dw.trans_dir = DMA_PERH2MEM;
    ret = csi_dma_ch_config(&rx_dma_ch, &config, NULL, NULL);
    CHECK_RETURN(ret);
    
    ret = csi_adc_link_dma(&g_adc, &rx_dma_ch);
    CHECK_RETURN(ret);
    /* attach callback to g_adc device */
    ret = csi_adc_attach_callback(&g_adc, adc_event_cb, NULL);
    CHECK_RETURN(ret);
    /* Set continue mode */
    csi_adc_continue_mode(&g_adc, true);
    CHECK_RETURN(ret);
    /* Set data buffer */
    ret = csi_adc_set_buffer(&g_adc, (uint32_t *)rx_data, sizeof(rx_data));
    CHECK_RETURN(ret);
    while(1) {
        /* Trigger new conversion */
        ret = csi_adc_start_async(&g_adc);
        CHECK_RETURN(ret);
        while (rx_async_flag == 0)
        {
            #ifdef CONFIG_KERNEL_NONE
            HAL_Delay(100);
            #else
            csi_kernel_delay(pdMS_TO_TICKS(100));
            #endif
        }
        rx_async_flag = 0;
        for (int i = 0; i < TEST_ADC_BUFFER_SIZE; i++)
        {
            #if(TEST_ADC_CHANNEL == 16)
            printf("result[%d]: %d, Temperature[%d]: %d\n", i, rx_data[i], i, HAL_ADC_ValueToCelsius(rx_data[i]));
            #else
            printf("get g_adc result[%d]: %d\n", i, rx_data[i]);
            #endif
            rx_data[i] = 0;
        }
        printf("\n");
        #ifdef CONFIG_KERNEL_NONE
        HAL_Delay(2000);
        #else
        csi_kernel_delay(pdMS_TO_TICKS(2000));
        #endif
    }
#endif

    /* Uninit the g_adc device */
    csi_adc_uninit(&g_adc);
    while(1)
    {
		mdelay(1000);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_UART_ */
#endif /* BSP_DEBUG */
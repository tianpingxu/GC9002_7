/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_adc.h
 * @brief    header File for ADC Driver
 * @version  V1.0
 * @date     2023/10/08
 * @model    adc
 ******************************************************************************/
#ifndef __CSI_ADC_H__
#define __CSI_ADC_H__

#include <csi_common.h>
#include <csi_dma.h>
#include <csi_irq.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_ADC_ENABLED

#define  CSI_ADC_CNT        1

typedef enum
{
    ADC_EVENT_CONVERT_COMPLETE, /* 转换完成事件，当指定的数据转换完成时触发 */
    ADC_EVENT_CONVERT_HALF_DONE,/* 转换完成一半，当指定的数据转换完成时触发 */
    ADC_EVENT_ERROR,            /* 数据丢失事件，未及时读取数据时触发 */
}csi_adc_event_t;
typedef struct _csi_adc
{
    csi_dev_t dev;          //csi Device unity handle
    void (*callback)(struct _csi_adc *adc, csi_adc_event_t event, void *arg);//User callback function
    void *arg;              //Callback function argument
    uint32_t *data;         //指向接收数据缓存的地址
    uint32_t size;          //接收数据的大小
    csi_error_t (*start)(struct _csi_adc *adc);  //指向ADC开始转换函数(异步)
    csi_error_t (*stop)(struct _csi_adc *adc);   //指向ADC停止转换函数(异步)
    csi_dma_ch_t *dma;      //指向接收DMA通道
    csi_state_t state;      //ADC设备的当前状态
    void *priv;             //Device private variable
}csi_adc_t;

typedef void (*csi_adc_cb_t)(csi_adc_t *adc, csi_adc_event_t event, void *arg);

csi_error_t csi_adc_init(csi_adc_t *adc, uint32_t idx);
void        csi_adc_uninit(csi_adc_t *adc);
csi_error_t csi_adc_set_buffer(csi_adc_t *adc, uint32_t *data, uint32_t num);
csi_error_t csi_adc_start(csi_adc_t *adc);
csi_error_t csi_adc_start_async(csi_adc_t *adc);
csi_error_t csi_adc_stop(csi_adc_t *adc);
csi_error_t csi_adc_stop_async(csi_adc_t *adc);
csi_error_t csi_adc_channel_enable(csi_adc_t *adc, uint8_t ch_id, bool is_enable);
csi_error_t csi_adc_channel_sampling_time(csi_adc_t *adc, uint8_t ch_id, uint16_t clock_num);
csi_error_t csi_adc_sampling_time(csi_adc_t *adc, uint16_t clock_num);
csi_error_t csi_adc_continue_mode(csi_adc_t *adc, bool is_enable);
uint32_t    csi_adc_freq_div(csi_adc_t *adc, uint32_t div);
uint32_t    csi_adc_get_freq(csi_adc_t *adc);
int32_t     csi_adc_read(csi_adc_t *adc);
csi_error_t csi_adc_get_state(csi_adc_t *adc, csi_state_t *state);
csi_error_t csi_adc_attach_callback(csi_adc_t *adc, void *callback, void *arg);
void        csi_adc_detach_callback(csi_adc_t *adc);
csi_error_t csi_adc_link_dma(csi_adc_t *adc, csi_dma_ch_t *dma);

#endif /* CONFIG_CSI_DRV_ADC_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_ADC_H__ */
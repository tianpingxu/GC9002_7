/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_i2s.h
 * @brief    header File for I2S Driver
 * @version  V1.0
 * @date     2023/03/03
 * @model    i2s
 ******************************************************************************/
#ifndef __CSI_I2S_H__
#define __CSI_I2S_H__

#include <csi_common.h>
#include <csi_dma.h>
#include <csi_irq.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_I2S_ENABLED
#define CSI_IIS_CNT            1    // Peripheral count

typedef enum I2S_Mode csi_i2s_mode_t;

typedef enum
{
    I2S_PROTOCOL_I2S,               //I2S传输协议为I2S
    I2S_PROTOCOL_MSB_JUSTIFIED,     //I2S传输协议为MSB_JUSTIFIED
    I2S_PROTOCOL_LSB_JUSTIFIED,     //I2S传输协议为LSB_JUSTIFIED
    I2S_PROTOCOL_PCM,               //I2S传输协议为PCM
}csi_i2s_protocol_t;

typedef enum
{
    I2S_LEFT_POLARITY_LOW,      //低电平对应左声道
    I2S_LEFT_POLARITY_HIGH,     //高电平对应左声道
}csi_i2s_ws_left_polarity_t;
typedef enum
{
    I2S_SAMPLE_RATE_8000,       //采样比率8K
    I2S_SAMPLE_RATE_11025,      //采样比率11.025K
    I2S_SAMPLE_RATE_12000,      //采样比率12K
    I2S_SAMPLE_RATE_16000,      //采样比率16K
    I2S_SAMPLE_RATE_22050,      //采样比率22.05K
    I2S_SAMPLE_RATE_24000,      //采样比率24K
    I2S_SAMPLE_RATE_32000,      //采样比率32K
    I2S_SAMPLE_RATE_44100,      //采样比率44.1K
    I2S_SAMPLE_RATE_48000,      //采样比率48K
    I2S_SAMPLE_RATE_96000,      //采样比率96K
    I2S_SAMPLE_RATE_192000,     //采样比率192K
    I2S_SAMPLE_RATE_256000,     //采样比率256K
}csi_i2s_sample_rate_t;
typedef enum
{
    I2S_SAMPLE_WIDTH_16BIT,     //采样宽度16bits
    I2S_SAMPLE_WIDTH_24BIT,     //采样宽度24bits
    I2S_SAMPLE_WIDTH_32BIT,     //采样宽度32bits
}csi_i2s_sample_width_t;
typedef enum
{
    I2S_SCLK_16FS,     //SCLK频率为FS的16倍
    I2S_SCLK_32FS,     //SCLK频率为FS的32倍
    I2S_SCLK_48FS,     //SCLK频率为FS的48倍
    I2S_SCLK_64FS,     //SCLK频率为FS的64倍
}csi_i2s_sclk_freq_t;
typedef enum
{
    I2S_MCLK_256FS,     //MCLK频率为256倍
    I2S_MCLK_384FS,     //MCLK频率为384倍
}csi_i2s_mclk_freq_t;
typedef enum
{
    I2S_LEFT_CHANNEL,       //单声道选择左声道
    I2S_RIGHT_CHANNEL,      //单声道选择右声道
    I2S_LEFT_RIGHT_CHANNEL, //选择左右声道
}csi_i2s_sound_channel_t;
typedef enum
{
    I2S_EVENT_TRANSMIT_COMPLETE,    //数据发送完成事件
    I2S_EVENT_RECEIVE_COMPLETE,     //数据接收完成事件
    I2S_EVENT_TX_BUFFER_EMPYT,      //I2S发送FIFO数据为空
    I2S_EVENT_RX_BUFFER_FULL,       //I2S接收FIFO数据满
    I2S_EVENT_ERROR_OVERFLOW,       //I2S总线产生FIFO溢出错误事件
    I2S_EVENT_ERROR_UNDERFLOW,      //I2S总线产生FIFO下溢错误事件
    I2S_EVENT_ERROR,                //I2S总线数据错误
}csi_i2s_event_t ;


typedef struct
{
    csi_i2s_mode_t mode;                    //I2S主从模式设置
    csi_i2s_protocol_t protocol;            //I2S传输协议设置
    csi_i2s_ws_left_polarity_t polarity;    //WSCLK电平极性对应声道设置
    csi_i2s_sample_rate_t rate;             //I2S采样比率设置
    csi_i2s_sample_width_t width;           //I2S采样宽度设置
    csi_i2s_sclk_freq_t sclk_nfs;           //SCLK的频率设置
    csi_i2s_mclk_freq_t mclk_nfs;           //MCLK的频率设置
}csi_i2s_format_t;

typedef struct 
{
    uint8_t *buffer;    //环形缓冲区地址
    uint32_t size;      //环形缓冲区大小
    uint32_t write;     //环形缓冲区当前写指针位置
    uint32_t read;      //环形缓冲区当前读指针位置
    uint32_t data_len;  //环形缓冲区当前可读数据长度
}ringbuffer_t;
typedef struct _csi_i2s
{
    csi_dev_t dev;          //csi Device unity handle
    void (*callback)(struct _csi_i2s *i2s, csi_i2s_event_t event, void *arg);//User callback function
    void *arg;              //Callback function argument
    ringbuffer_t *tx_buf;   //发送音频流用的ringbuffer缓冲区
    ringbuffer_t *rx_buf;   //接收音频流用的ringbuffer缓冲区
    csi_dma_ch_t *tx_dma;   //用于发送的DMA通道句柄
    csi_dma_ch_t *rx_dma;   //用于接收的DMA通道句柄
    uint32_t tx_period;     //设置完成多少数据发送上报周期
    uint32_t rx_period;     //设置完成多少数据接收上报周期
    csi_state_t state;      //I2S设备的当前状态
    void *priv;             //Device private variable
}csi_i2s_t;

csi_error_t csi_i2s_init(csi_i2s_t *i2s, uint32_t idx);
void        csi_i2s_uninit(csi_i2s_t *i2s);
void        csi_i2s_enable(csi_i2s_t *i2s, bool en);
csi_error_t csi_i2s_format(csi_i2s_t *i2s, csi_i2s_format_t *format);
csi_error_t csi_i2s_tx_select_sound_channel(csi_i2s_t *i2s, csi_i2s_sound_channel_t ch);
csi_error_t csi_i2s_rx_select_sound_channel(csi_i2s_t *i2s, csi_i2s_sound_channel_t ch);
csi_error_t csi_i2s_tx_link_dma(csi_i2s_t *i2s, csi_dma_ch_t *tx_dma);
csi_error_t csi_i2s_rx_link_dma(csi_i2s_t *i2s, csi_dma_ch_t *rx_dma);
void        csi_i2s_tx_set_buffer(csi_i2s_t *i2s, ringbuffer_t *buffer);
void        csi_i2s_rx_set_buffer(csi_i2s_t *i2s, ringbuffer_t *buffer);
csi_error_t csi_i2s_tx_set_period(csi_i2s_t *i2s, uint32_t period);
csi_error_t csi_i2s_rx_set_period(csi_i2s_t *i2s, uint32_t period);
uint32_t    csi_i2s_tx_buffer_avail(csi_i2s_t *i2s);
uint32_t    csi_i2s_rx_buffer_avail(csi_i2s_t *i2s);
csi_error_t csi_i2s_rx_buffer_reset(csi_i2s_t *i2s);
uint32_t    csi_i2s_send(csi_i2s_t *i2s, const void *data, uint32_t size);
uint32_t    csi_i2s_send_async(csi_i2s_t *i2s, const void *data, uint32_t size);
uint32_t    csi_i2s_receive(csi_i2s_t *i2s, void *data, uint32_t size);
uint32_t    csi_i2s_receive_async(csi_i2s_t *i2s, void *data, uint32_t size);
csi_error_t csi_i2s_send_start(csi_i2s_t *i2s);
csi_error_t csi_i2s_send_resume(csi_i2s_t *i2s);
csi_error_t csi_i2s_send_pause(csi_i2s_t *i2s);
csi_error_t csi_i2s_send_stop(csi_i2s_t *i2s);
csi_error_t csi_i2s_receive_start(csi_i2s_t *i2s);
csi_error_t csi_i2s_receive_stop(csi_i2s_t *i2s);
csi_error_t csi_i2s_attach_callback(csi_i2s_t *i2s, void(*callback)(struct _csi_i2s *i2s, csi_i2s_event_t event, void *arg), void *arg);
csi_error_t csi_i2s_get_state(csi_i2s_t *i2s, csi_state_t *state);




#endif /* CONFIG_CSI_DRV_I2S_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_I2S_H__ */
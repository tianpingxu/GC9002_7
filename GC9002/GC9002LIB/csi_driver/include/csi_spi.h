/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_spi.h
 * @brief    header File for SPI Driver
 * @version  V1.0
 * @date     2023/02/25
 * @model    spi
 ******************************************************************************/
#ifndef __CSI_SPI_H__
#define __CSI_SPI_H__

#include <csi_common.h>
#include <csi_dma.h>
#include <csi_irq.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_SPI_ENABLED

#define CSI_SPI_CNT            3    // Peripheral count


typedef enum
{
    SPI_EVENT_TRANSMIT_COMPLETE,            //数据发送完成事件
    SPI_EVENT_RECEIVE_COMPLETE,             //数据接收完成事件
    SPI_EVENT_TRANSMIT_RECEIVE_COMPLETE,    //数据发送/接收完成事件
    SPI_EVENT_ERROR_OVERFLOW,               //数据溢出事件
    SPI_EVENT_ERROR_UNDERFLOW,              //数据下溢事件
    SPI_EVENT_ERROR,                        //总线错误事件
}csi_spi_event_t;

typedef enum
{
    SPI_MASTER,            //主机模式
    SPI_SLAVE,             //从机模式
}csi_spi_mode_t;
/*
 * CPOL=0，表示当SCLK=0时处于空闲态，所以有效状态就是SCLK处于高电平时
 * CPOL=1，表示当SCLK=1时处于空闲态，所以有效状态就是SCLK处于低电平时
 * CPHA=0，表示数据采样是在第1个边沿，数据发送在第2个边沿
 * CPHA=1，表示数据采样是在第2个边沿，数据发送在第1个边沿
 * */
typedef enum
{
    SPI_FORMAT_CPOL0_CPHA0,     //CPOL=0 CPHA=0
    SPI_FORMAT_CPOL0_CPHA1,     //CPOL=0 CPHA=1
    SPI_FORMAT_CPOL1_CPHA0,     //CPOL=1 CPHA=0
    SPI_FORMAT_CPOL1_CPHA1,     //CPOL=1 CPHA=1
}csi_spi_cp_format_t;

typedef enum
{
    SPI_FRAME_LEN_4,     //数据宽度：4bit
    SPI_FRAME_LEN_5,     //数据宽度：5bit
    SPI_FRAME_LEN_6,     //数据宽度：6bit
    SPI_FRAME_LEN_7,     //数据宽度：7bit
    SPI_FRAME_LEN_8,     //数据宽度：8bit
    SPI_FRAME_LEN_9,     //数据宽度：9bit
    SPI_FRAME_LEN_10,    //数据宽度：10bit
    SPI_FRAME_LEN_11,    //数据宽度：11bit
    SPI_FRAME_LEN_12,    //数据宽度：12bit
    SPI_FRAME_LEN_13,    //数据宽度：13bit
    SPI_FRAME_LEN_14,    //数据宽度：14bit
    SPI_FRAME_LEN_15,    //数据宽度：15bit
    SPI_FRAME_LEN_16,    //数据宽度：16bit
}csi_spi_frame_len_t;

typedef struct _csi_spi
{
    csi_dev_t dev;      //csi Device unity handle
    void(*callback)(struct _csi_spi *spi, csi_spi_event_t event, void *arg);//User callback function
    void *arg;          //Callback function argument
    void *tx_data;      //指向发送缓存的地址
    uint32_t tx_size;   //发送数据的大小
    void *rx_data;      //指向接收缓存的地址
    uint32_t rx_size;   //接收数据的大小
    csi_error_t(*send)(struct _csi_spi *spi, const void *data, uint32_t size);  //指向发送函数(异步)
    csi_error_t(*receive)(struct _csi_spi *spi, void *data, uint32_t size);     //指向接收函数(异步)
    csi_error_t(*send_receive)(struct _csi_spi *spi, const void *data_out, void *data_in, uint32_t size);
    csi_dma_ch_t *tx_dma;   //指向发送DMA通道
    csi_dma_ch_t *rx_dma;   //指向接收DMA通道
    uint32_t state;
    void *priv;         //Device private variable
}csi_spi_t;

csi_error_t csi_spi_init(csi_spi_t *spi, uint32_t idx);
void        csi_spi_uninit(csi_spi_t *spi);
csi_error_t csi_spi_attach_callback(csi_spi_t *spi, void(*callback)(struct _csi_spi *spi, csi_spi_event_t event, void *arg), void *arg);
void        csi_spi_detach_callback(csi_spi_t *spi);
csi_error_t csi_spi_mode(csi_spi_t *spi, csi_spi_mode_t mode);
csi_error_t csi_spi_cp_format(csi_spi_t *spi, csi_spi_cp_format_t format);
csi_error_t csi_spi_frame_len(csi_spi_t *spi, csi_spi_frame_len_t length);
uint32_t    csi_spi_baud(csi_spi_t *spi, uint32_t baud);
int32_t     csi_spi_send(csi_spi_t *spi, const void *data, uint32_t size, uint32_t timeout);
csi_error_t csi_spi_send_async(csi_spi_t *spi, const void *data, uint32_t size);
uint32_t    csi_spi_receive(csi_spi_t *spi, void *data, uint32_t size, uint32_t timeout);
csi_error_t csi_spi_receive_async(csi_spi_t *spi, void *data, uint32_t size);
uint32_t    csi_spi_send_receive(csi_spi_t *spi, const void *data_out, void *data_in, uint32_t size, uint32_t timeout);
csi_error_t csi_spi_send_receive_async(csi_spi_t *spi, const void *data_out, void *data_in, uint32_t size);
void        csi_spi_select_slave(csi_spi_t *spi, int slave_num);
csi_error_t csi_spi_link_dma(csi_spi_t *spi, csi_dma_ch_t *tx_dma, csi_dma_ch_t *rx_dma);
csi_error_t csi_spi_get_state(csi_spi_t *spi, csi_state_t *state);



#endif /* CONFIG_CSI_DRV_SPI_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_SPI_H__ */
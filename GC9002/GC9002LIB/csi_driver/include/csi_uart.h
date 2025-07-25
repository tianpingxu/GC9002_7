/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_uart.h
 * @brief    header File for UART Driver
 * @version  V1.0
 * @date     2023/02/21
 * @model    gpio
 ******************************************************************************/
#ifndef __CSI_UART_H__
#define __CSI_UART_H__

#include <csi_common.h>
#include <csi_dma.h>
#include <csi_irq.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_UART_ENABLED

#define CSI_UART_CNT    3   // Peripheral count

typedef enum
{
    UART_EVENT_SEND_COMPLETE,           //数据发送完成事件
    UART_EVENT_RECEIVE_COMPLETE,        //数据接收完成事件
    UART_EVENT_IDLE,                    //硬件空闲事件
    UART_EVENT_RECEIVE_FIFO_READABLE,   //FIFO残留数据等待接收事件
    UART_ENENT_BREAK_INTR,              //数据接收中断事件
    UART_EVENT_ERROR_OVERFLOW,          //数据接收溢出事件
    UART_EVENT_ERROR_PARITY,            //数据校验位错误事件
    UART_EVENT_ERROR_FRAMING,           //数据无有效停止位事件
}csi_uart_event_t;
typedef struct _csi_uart
{
    csi_dev_t dev;      //csi Device unity handle
    void(*callback)(struct _csi_uart *uart, csi_uart_event_t event, void *arg);//User callback function
    void *arg;          //Callback function argument
    uint8_t* tx_data;   //发送缓存区地址
    uint32_t tx_size;   //发送数据字节数
    uint8_t* rx_data;   //接收缓存区地址
    uint32_t rx_size;   //接收数据字节数
    
    csi_dma_ch_t *tx_dma;   //用于发送的DMA通道句柄
    csi_dma_ch_t *rx_dma;   //用于接收的DMA通道句柄
    csi_error_t(*send)(struct _csi_uart uart, const void *data, uint32_t size);     //异步发送函数指针
    csi_error_t(*receive)(struct _csi_uart uart, const void *data, uint32_t size);  //异步接收函数指针
    uint32_t state;     //UART设备读写状态
    void *priv;         //Device private variable
}csi_uart_t;

typedef enum
{
    UART_DATA_BITS_5,   //5位数据位宽
    UART_DATA_BITS_6,   //6位数据位宽
    UART_DATA_BITS_7,   //7位数据位宽
    UART_DATA_BITS_8,   //8位数据位宽
    UART_DATA_BITS_9,   //9位数据位宽
}csi_uart_data_bits_t;
typedef enum
{
    CSI_UART_PARITY_NONE,   //无校验
    CSI_UART_PARITY_EVEN,   //偶校验
    CSI_UART_PARITY_ODD,    //奇校验
}csi_uart_parity_t;
typedef enum
{
    UART_STOP_BITS_1,   //1停止位
    UART_STOP_BITS_2,   //2停止位
    UART_STOP_BITS_1_5, //1.5停止位
}csi_uart_stop_bits_t;

typedef enum
{
    UART_FLOWCTRL_NONE,     //无流控
    UART_FLOWCTRL_RTS,      //发送请求（Require ToSend）
    UART_FLOWCTRL_CTS,      //发送允许（Clear ToSend）
    UART_FLOWCTRL_RTS_CTS,  //发送请求与发送允许功能同时打开
}csi_uart_flowctrl_t;

csi_error_t csi_uart_init(csi_uart_t *uart, uint32_t idx);
void        csi_uart_uninit(csi_uart_t *uart);
csi_error_t csi_uart_baud(csi_uart_t *uart, uint32_t baud);
csi_error_t csi_uart_format(csi_uart_t *uart, csi_uart_data_bits_t data_bits, csi_uart_parity_t parity, csi_uart_stop_bits_t stop_bits);
csi_error_t csi_uart_flowctrl(csi_uart_t *uart,  csi_uart_flowctrl_t flowctrl);

csi_error_t csi_uart_idle_ctrl(csi_uart_t *uart, bool enable);
uint32_t    csi_uart_get_counter(csi_uart_t *uart);
csi_error_t csi_uart_attach_callback(csi_uart_t *uart, void(*callback)(struct _csi_uart *uart, csi_uart_event_t event, void *args), void *arg);
void        csi_uart_detach_callback(csi_uart_t *uart);
csi_error_t csi_uart_link_dma(csi_uart_t *uart, csi_dma_ch_t *tx_dma, csi_dma_ch_t *rx_dma);
int32_t     csi_uart_send(csi_uart_t *uart, const void *data, uint32_t size, uint32_t timeout);
csi_error_t csi_uart_send_async(csi_uart_t *uart, const void *data, uint32_t size);
int32_t     csi_uart_receive(csi_uart_t *uart, void *data, uint32_t size, uint32_t timeout);
csi_error_t csi_uart_receive_async(csi_uart_t *uart, void *data, uint32_t size);
uint8_t     csi_uart_getc(csi_uart_t *uart);
void        csi_uart_putc(csi_uart_t *uart, uint8_t ch);
csi_error_t csi_uart_get_state(csi_uart_t *uart, csi_state_t *state);


#endif /* CONFIG_CSI_DRV_UART_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_UART_H__ */
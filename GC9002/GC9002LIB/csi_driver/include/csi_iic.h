/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_iic.h
 * @brief    header File for IIC Driver
 * @version  V1.0
 * @date     2023/02/26
 * @model    iic
 ******************************************************************************/
#ifndef __CSI_IIC_H__
#define __CSI_IIC_H__

#include <csi_common.h>
#include <csi_dma.h>
#include <csi_irq.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_IIC_ENABLED

#define CSI_IIC_CNT            2    // Peripheral count

typedef enum
{
    IIC_EVENT_SEND_COMPLETE,        //数据发送完成事件
    IIC_EVENT_RECEIVE_COMPLETE,     //数据接收完成事件
    IIC_EVENT_ERROR_OVERFLOW,       //IIC总线产生FIFO溢出事件
    IIC_EVENT_ERROR_UNDERFLOW,      //IIC总线产生FIFO下溢事件
    IIC_EVENT_ERROR,                //总线产生错误
}csi_iic_event_t;
typedef enum
{
    IIC_MODE_MASTER,    //IIC主机模式
    IIC_MODE_SLAVE,     //IIC从机模式
}csi_iic_mode_t;
typedef enum
{
    IIC_ADDRESS_7BIT,   //7位地址模式
    IIC_ADDRESS_10BIT,  //10位地址模式
}csi_iic_addr_mode_t;
typedef enum
{
    IIC_BUS_SPEED_STANDARD,     //standard Speed (<=100kHz)
    IIC_BUS_SPEED_FAST,         //fast Speed (<=400kHz)
    IIC_BUS_SPEED_FAST_PLUS,    //fast+ Speed (<= 1MHz)
    IIC_BUS_SPEED_HIGH,         //high Speed (<=3.4MHz)
}csi_iic_speed_t;
typedef enum
{
    IIC_MEM_ADDR_SIZE_8BIT,     //EEPROM的地址模式是8位
    IIC_MEM_ADDR_SIZE_16BIT,    //EEPROM的地址模式是16位
}csi_iic_mem_addr_size_t;

typedef struct _csi_iic
{
    csi_dev_t dev;          //csi Device unity handle
    void(*callback)(struct _csi_iic *iic, csi_iic_event_t event, void *arg);//User callback function
    void *arg;              //Callback function argument
    uint8_t *data;          //用户传入的数据地址
    uint32_t size;          //用户传入的数据长度
    csi_iic_mode_t mode;    //IIC设备工作模式
    csi_dma_ch_t *tx_dma;   //指向发送DMA通道
    csi_dma_ch_t *rx_dma;   //指向接收DMA通道
    csi_error_t(*send)(struct _csi_iic *iic, const void *data, uint32_t size);  //指向发送函数(异步)
    csi_error_t(*receive)(struct _csi_iic *iic, void *data, uint32_t size);     //指向接收函数(异步)
    csi_state_t state;      //IIC设备的当前状态
    void *priv;             //Device private variable
}csi_iic_t;



csi_error_t csi_iic_init(csi_iic_t *iic, uint32_t idx);
void        csi_iic_uninit(csi_iic_t *iic);
csi_error_t csi_iic_mode(csi_iic_t *iic, csi_iic_mode_t mode);
csi_error_t csi_iic_addr_mode(csi_iic_t *iic, csi_iic_addr_mode_t addr_mode);
csi_error_t csi_iic_speed(csi_iic_t *iic, csi_iic_speed_t speed);
csi_error_t csi_iic_own_addr(csi_iic_t *iic, uint32_t own_addr);
csi_error_t csi_iic_attach_callback(csi_iic_t *iic, void(*callback)(csi_iic_t *iic, csi_iic_event_t event, void *arg), void *arg);
void        csi_iic_detach_callback(csi_iic_t *iic);
csi_error_t csi_iic_link_dma(csi_iic_t *iic, csi_dma_ch_t *tx_dma, csi_dma_ch_t *rx_dma);
int32_t     csi_iic_master_send(csi_iic_t *iic, uint32_t devaddr,const void *data, uint32_t size, uint32_t timeout);
csi_error_t csi_iic_master_send_async(csi_iic_t *iic, uint32_t devaddr, void *data, uint32_t size);
int32_t     csi_iic_master_receive(csi_iic_t *iic, uint32_t devaddr, void *data, uint32_t size, uint32_t timeout);
csi_error_t csi_iic_master_receive_async(csi_iic_t *iic, uint32_t devaddr, void *data, uint32_t size);
int32_t     csi_iic_mem_send(csi_iic_t *iic, 
                             uint32_t devaddr,
                             uint16_t memaddr, 
                             csi_iic_mem_addr_size_t memaddr_size,
                             const void *data,
                             uint32_t size,
                             uint32_t timeout);
int32_t     csi_iic_mem_receive(csi_iic_t *iic,
                                uint32_t devaddr,
                                uint16_t memaddr,
                                csi_iic_mem_addr_size_t memaddr_size,
                                void *data,
                                uint32_t size,
                                uint32_t timeout);
int32_t     csi_iic_slave_send(csi_iic_t *iic, const void *data, uint32_t size, uint32_t timeout);
csi_error_t csi_iic_slave_send_async(csi_iic_t *iic, void *data, uint32_t size);
int32_t     csi_iic_slave_receive(csi_iic_t *iic, void *data, uint32_t size, uint32_t timeout);
csi_error_t csi_iic_slave_receive_async(csi_iic_t *iic, void *data, uint32_t size);
csi_error_t csi_iic_xfer_pending(csi_iic_t *iic, bool enable);
csi_error_t csi_iic_get_state(csi_iic_t *iic, csi_state_t *state);




#endif /* CONFIG_CSI_DRV_IIC_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_IIC_H__ */
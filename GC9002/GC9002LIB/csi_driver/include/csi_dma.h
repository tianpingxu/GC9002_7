/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_dma.h
 * @brief    header File for DMA Driver
 * @version  V1.0
 * @date     2023/02/21
 * @model    gpio
 ******************************************************************************/
#ifndef __CSI_DMA_H__
#define __CSI_DMA_H__

#include <csi_common.h>
#include <csi_irq.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_DMA_ENABLED

#define CSI_DMA_CNT         1   // Peripheral count
#define CSI_DMA_CH_NUM      DMA_NUM_CHANNELS    //Channel count


typedef enum
{
    DMA_EVENT_TRANSFER_DONE,        //数据传输完成事件
    DMA_EVENT_TRANSFER_HALF_DONE,   //数据传输完成一半事件
    DMA_EVENT_TRANSFER_ERROR,       //传输异常事件
}csi_dma_event_t;

typedef enum
{
    DMA_ADDR_INC,       //地址递增
    DMA_ADDR_DEC,       //地址递减
    DMA_ADDR_CONSTANT,  //地址固定
}csi_dma_addr_inc_t;
typedef enum
{
    DMA_DATA_WIDTH_8_BITS,  //8位数据宽度
    DMA_DATA_WIDTH_16_BITS, //16位数据宽度
    DMA_DATA_WIDTH_32_BITS, //32位数据宽度
}csi_dma_data_width_t;
typedef enum
{
    DMA_MEM2MEM,    //内存到内存
    DMA_MEM2PERH,   //内存到外设
    DMA_PERH2MEM,   //外设到内存
    DMA_PERH2PERH,  //外设到外设
}csi_dma_trans_dir_t;

typedef enum
{
    DMA_ITEM_1 = 0x0UL,
    DMA_ITEMS_4,
    DMA_ITEMS_8,
    DMA_ITEMS_16,
    DMA_ITEMS_32,
    DMA_ITEMS_64,
    DMA_ITEMS_128,
    DMA_ITEMS_256,
    DMA_ITEMS_512,
    DMA_ITEMS_1024,
}csi_dma_burst_len_t;
typedef struct
{
    uint8_t                 src_hs_if;      //DMA源硬件握手号
    uint8_t                 dst_hs_if;      //DMA目的硬件握手号
    bool                    src_reload_en;  //源地址自动加载使能
    bool                    dst_reload_en;  //目的地址自动加载使能
    csi_dma_burst_len_t     src_burst_len;  //源突发传输长度
    csi_dma_burst_len_t     dst_burst_len;  //目的突发传输长度
    csi_dma_trans_dir_t     trans_dir;      //传输数据方向配置
}dw_dma_ch_config;
typedef struct
{
    csi_dma_addr_inc_t      src_inc;    //DMA源地址增长方式
    csi_dma_addr_inc_t      dst_inc;    //DMA目的地址增长方式
    csi_dma_data_width_t    src_tw;     //DMA源数据宽度
    csi_dma_data_width_t    dst_tw;     //DMA目的数据宽度
    dw_dma_ch_config        dw;         //dw DMA配置结构体
}csi_dma_ch_config_t;

typedef void (*csi_dma_cb_t)(csi_dma_event_t event, void *arg);
typedef struct _csi_dma_ch csi_dma_ch_t;
struct _csi_dma_ch
{
    void         *parent;       //存放使用DMA的外设句柄，如uart，iic等
    int16_t      ctrl_id;       //DMA控制器id
    int16_t      ch_id;         //通道句柄
    csi_dma_cb_t cb;            //通道回调函数
    void         *arg;          //回调函数的参数指针
    csi_dma_ch_t *next;         //链表的下一个节点
    void         *priv;         //Device private variable
};

typedef struct _csi_dma_ctrl
{
    csi_dev_t    dev;           //csi Device unity handle
    csi_dma_ch_t *ch_list;      //通道链表首地址
    uint32_t     alloc_status;  //通道占用状态
    uint32_t     ch_num;        //控制器拥有的通道数量
    void         *priv;         //Device private variable
}csi_dma_ctrl_t;


csi_error_t csi_dma_ctrl_init(csi_dma_ctrl_t *dma, int8_t ctrl_id);
void        csi_dma_ctrl_uninit(csi_dma_ctrl_t *dma);

csi_error_t csi_dma_ch_alloc(csi_dma_ch_t *dma, int8_t ch_id, int8_t ctrl_id);
void        csi_dma_ch_free(csi_dma_ch_t *dma);

csi_error_t csi_dma_ch_config(csi_dma_ch_t *dma, csi_dma_ch_config_t *config, csi_dma_cb_t cb, void *arg);
void        csi_dma_ch_start(csi_dma_ch_t *dma, void *srcaddr, void *dstaddr, uint32_t length);
void        csi_dma_ch_stop(csi_dma_ch_t *dma);

uint32_t    csi_dma_ch_get_counter(csi_dma_ch_t *dma);

#endif /* CONFIG_CSI_DRV_DMA_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_DMA_H__ */
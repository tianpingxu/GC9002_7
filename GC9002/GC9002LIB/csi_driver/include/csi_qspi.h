/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_qspi.h
 * @brief    header File for QSPI Driver
 * @version  V1.0
 * @date     2023/02/28
 * @model    qspi
 ******************************************************************************/
#ifndef __CSI_QSPI_H__
#define __CSI_QSPI_H__

#include <csi_common.h>
#if(defined CHIP_GC9002)
#include <csi_dma.h>
#include <csi_irq.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_QSPI_ENABLED

#define CSI_QSPI_CNT                    1   // Peripheral count

#if(defined CHIP_GC9005)
#define CQSPI_BASE                      QSPI_DRAM_CACHE_BASE
#define CQSPI_DATA_BASE                 QSPI_DATA_BASE
#define CQSPI_IRQn                      QSPI_IRQn
#define __HAL_RCC_CQSPI_SET_PRES        __HAL_RCC_QSPI_SET_PRES
#define __HAL_RCC_CQSPI_CLK_ENABLE      __HAL_RCC_QSPI_CLK_ENABLE
#endif

#if(defined CHIP_GC9002)
#define __CSI_QSPI_DRIVER 		        CONFIG_ATTRIBUTE_FLASH_DRIVER
#define CSI_BSP_QSPI_REMAP_TYPE         QSPI_REMAP_TYPE_DEFAULT
#define CSI_BSP_QSPI_DIV_0              1
#define CSI_BSP_QSPI_DIV_1              2
#define CSI_BSP_QSPI_DIV_2              4
#define CSI_BSP_QSPI_DIV_3              6
#elif(defined CHIP_GC9005)
#define __CSI_QSPI_DRIVER 		        
#define CSI_BSP_QSPI_REMAP_TYPE         QSPI_REMAP_TYPE_SUB
#define CSI_BSP_QSPI_DIV_0              4
#define CSI_BSP_QSPI_DIV_1              6
#define CSI_BSP_QSPI_DIV_2              8
#define CSI_BSP_QSPI_DIV_3              10
#endif

#define CSI_BSP_QSPI_FREQ_MAX           240000000
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
#define CSI_QSPI_FREQ_MIN               250000  //(48000000/6/32)
#define CSI_QSPI_FREQ_MID               1000000  //(48000000/6/8)
#define CSI_QSPI_FREQ_MAX               24000000//(48000000/2)
#else
#define CSI_QSPI_FREQ_MIN               get_qspi_freq_min()
#define CSI_QSPI_FREQ_MID               (get_qspi_freq_max()/2)
#define CSI_QSPI_FREQ_MAX               get_qspi_freq_max()
#endif

#define QSPI_CFG_BAUD_DIV_MIN           2U
#define QSPI_CFG_BAUD_DIV_MAX           32U


typedef enum
{
    QSPI_REMAP_TYPE_DEFAULT,
    QSPI_REMAP_TYPE_SUB,
}csi_qspi_remap_type_t;
typedef enum
{
    QSPI_CLOCK_MODE_0,      //模式0: CPOL = 0, CPHA = 0
    QSPI_CLOCK_MODE_3,      //模式3: CPOL = 1, CPHA = 1
}csi_qspi_clock_mode_t;
typedef enum
{
    QSPI_ALTERNATE_BYTES_8_BITS = 0x1U,     //1字节
    QSPI_ALTERNATE_BYTES_16_BITS,           //2字节
    QSPI_ALTERNATE_BYTES_24_BITS,           //3字节
    QSPI_ALTERNATE_BYTES_32_BITS,           //4字节
}csi_qspi_alt_size_t;
typedef enum
{
    QSPI_ADDRESS_8_BITS = 0x1U,     //8比特
    QSPI_ADDRESS_16_BITS,           //16比特
    QSPI_ADDRESS_24_BITS,           //24比特
    QSPI_ADDRESS_32_BITS,           //32比特
}csi_qspi_address_size_t;

typedef enum
{
    QSPI_CFG_BUS_SINGLE,    //单线
    QSPI_CFG_BUS_DUAL,      //双线
    QSPI_CFG_BUS_QUAD,      //四线
}csi_qspi_bus_width_t;

typedef enum
{
    QSPI_READ,      //read
    QSPI_WRITE,     //write
}csi_qspi_dir_t;

typedef struct
{
    struct
    {
        csi_qspi_bus_width_t read_bus_width;    //指令阶段总线线数
        csi_qspi_bus_width_t write_bus_width;    //指令阶段总线线数
    }instruction;
    struct
    {
        csi_qspi_bus_width_t read_bus_width;    //读地址阶段总线线数
        csi_qspi_bus_width_t write_bus_width;   //写地址阶段总线线数
        csi_qspi_address_size_t size;           //地址字节数
    }address;
    struct
    {
        csi_qspi_bus_width_t bus_width; //data总线数
        uint8_t opcode;                 //指令值
        uint8_t dummy_count;            //dummy数量
    }read_data;
    struct
    {
        csi_qspi_bus_width_t bus_width; //data总线数
        uint8_t opcode;                 //指令值
        uint8_t dummy_count;            //dummy数量
    }write_data;
    struct
    {
        uint8_t value;                  //These are the 8 bits that are sent to the device following the address bytes
        bool enabled;                   //Mode Bit使能/禁能
    }mode_bit;
    struct
    {
        uint32_t page_size;             //size of page for flash
        uint32_t block_size;            //size of block for flash
    }flash;
}csi_qspi_direct_config_t;
typedef struct
{
    struct
    {
        csi_qspi_bus_width_t bus_width; //指令阶段总线线数
        uint8_t value;                  //指令值
        bool disabled;                  //指令使能/禁能
    }instruction;
    struct
    {
        csi_qspi_bus_width_t bus_width; //地址阶段总线线数
        csi_qspi_address_size_t size;   //地址字节数
        uint32_t value;                 //地址值
        bool disabled;                  //地址使能/禁能
    }address;
    struct
    {
        csi_qspi_bus_width_t bus_width; //交替字节总线数
        csi_qspi_alt_size_t size;       //交替字节字节数
        uint32_t value;                 //交替字节数值
        bool disabled;                  //交替字节使能/禁能
    }alt;
    uint8_t dummy_count;                //dummy数量
    struct
    {
        csi_qspi_bus_width_t bus_width;     //data总线数
    }data;
}csi_qspi_command_t;

typedef enum
{
    QSPI_EVENT_TRANSMIT_COMPLETE,            //数据发送完成事件
    QSPI_EVENT_RECEIVE_COMPLETE,             //数据接收完成事件
    QSPI_EVENT_TRANSMIT_RECEIVE_COMPLETE,    //数据发送/接收完成事件
    QSPI_EVENT_ERROR_OVERFLOW,               //数据溢出事件
    QSPI_EVENT_ERROR_UNDERFLOW,              //数据下溢事件
    QSPI_EVENT_ERROR,                        //总线错误事件
}csi_qspi_event_t;
typedef struct _csi_qspi
{
    csi_dev_t dev;      //csi Device unity handle
    uint32_t remap_bus_base;   //重映射总线基地址
    void(*callback)(struct _csi_qspi qspi, csi_qspi_event_t event, void *arg);//User callback function
    void *arg;          //Callback function argument
    void *tx_data;      //指向发送缓存的地址
    uint32_t tx_size;   //发送数据的大小
    void *rx_data;      //指向接收缓存的地址
    uint32_t rx_size;   //接收数据的大小
    csi_state_t state;
    csi_qspi_direct_config_t *direct_config;
    void *priv;         //Device private variable
}csi_qspi_t;

struct qspi_irq_hdr
{
    void (*hdr)(csi_qspi_event_t event, void *args);
    void *arg;
};


__CSI_QSPI_DRIVER csi_error_t csi_qspi_init(csi_qspi_t *qspi, uint32_t idx);
void        csi_qspi_uninit(csi_qspi_t *qspi);
csi_error_t csi_qspi_attach_callback(csi_qspi_t *qspi, void(*callback)(csi_qspi_event_t event, void *arg), void *arg);
void        csi_qspi_detach_callback(csi_qspi_t *qspi);
__CSI_QSPI_DRIVER uint32_t    csi_qspi_frequence(csi_qspi_t *qspi, uint32_t hz);
__CSI_QSPI_DRIVER csi_error_t csi_qspi_mode(csi_qspi_t *qspi, csi_qspi_clock_mode_t mode);
__CSI_QSPI_DRIVER int32_t     csi_qspi_send(csi_qspi_t *qspi, csi_qspi_command_t *cmd, const void *data, uint32_t size, uint32_t timeout);
__CSI_QSPI_DRIVER uint32_t    csi_qspi_receive(csi_qspi_t *qspi, csi_qspi_command_t *cmd, void *data, uint32_t size, uint32_t timeout);
__CSI_QSPI_DRIVER uint32_t    csi_qspi_send_receive(csi_qspi_t *qspi, csi_qspi_command_t *cmd, const void *tx_data, void *rx_data, uint32_t size, uint32_t timeout);
__CSI_QSPI_DRIVER csi_error_t csi_qspi_send_async(csi_qspi_t *qspi, csi_qspi_command_t *cmd, const void *data, uint32_t size);
__CSI_QSPI_DRIVER csi_error_t csi_qspi_receive_async(csi_qspi_t *qspi, csi_qspi_command_t *cmd, void *data, uint32_t size);
__CSI_QSPI_DRIVER csi_error_t csi_qspi_send_receive_async(csi_qspi_t *qspi, csi_qspi_command_t *cmd, const void *tx_data, void *rx_data, uint32_t size);
#if(defined CHIP_GC9002)
csi_error_t csi_qspi_link_dma(csi_qspi_t *qspi, csi_dma_ch_t *tx_dma, csi_dma_ch_t *rx_dma);
#endif
csi_error_t csi_qspi_get_state(csi_qspi_t *qspi, csi_state_t *state);

__CSI_QSPI_DRIVER csi_error_t csi_qspi_direct_mode_config(csi_qspi_t *qspi, csi_qspi_direct_config_t *config, csi_qspi_dir_t dir);
__CSI_QSPI_DRIVER csi_error_t csi_qspi_adapted_capture(csi_qspi_t *qspi);
__CSI_QSPI_DRIVER csi_error_t csi_qspi_direct_send(csi_qspi_t *qspi,
                                   csi_qspi_direct_config_t *config,
                                   uint32_t remap_addr,
                                   uint32_t offset_addr,
                                   const void *data,
                                   uint32_t size);
__CSI_QSPI_DRIVER csi_error_t csi_qspi_direct_receive(csi_qspi_t *qspi,
                                      csi_qspi_direct_config_t *config,
                                      uint32_t remap_addr,
                                      uint32_t offset_addr,
                                      void *data,
                                      uint32_t size);
bool        csi_qspi_is_idle(csi_qspi_t *qspi);


/* qspi flash add interface */
__CSI_QSPI_DRIVER void csi_qspi_write_protection_config(csi_qspi_t *qspi, uint32_t lower_addr, uint32_t upper_addr, bool enable);
__CSI_QSPI_DRIVER void csi_qspi_write_protection_ctrl(csi_qspi_t *qspi, bool enable);

__CSI_QSPI_DRIVER unsigned int get_qspi_freq_max(void);
__CSI_QSPI_DRIVER unsigned int get_qspi_freq_min(void);

#endif /* CONFIG_CSI_DRV_QSPI_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_QSPI_H__ */
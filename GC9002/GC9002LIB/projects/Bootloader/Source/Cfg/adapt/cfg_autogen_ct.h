#ifndef __CFG_AUTOGEN_CT_H
#define __CFG_AUTOGEN_CT_H

#include <stdio.h>
#include <stdint.h>

typedef struct
{
	struct
	{
		uint32_t valid_flag;                                        /* 有效标志 */
		uint16_t version;                                           /* 配置参数版本 */
		uint16_t checksum;                                          /* 除头以外参数数据校验和 */
		uint32_t size;                                              /* 除头以外参数数据大小 */
	}Head;

	struct
	{
		uint8_t cfg_uart;                                           /* 是否配置Uart升级 */
		uint8_t cfg_can;                                            /* 是否配置CAN升级 */
		uint8_t cfg_sd;                                             /* 是否配置SD卡升级 */
		uint8_t cfg_usb;                                            /* 是否配置USB升级 */
		uint16_t can_uart_wait_ms;                                  /* Uart升级等待时长(毫秒) */
		uint8_t res_upg[10];                                        /* 预留 */
	}UpgCfg;

	struct
	{
		uint8_t uart_channel;                                       /* Uart通道号 */
		uint8_t uart_tx_port;                                       /* Uart发送管脚Port口 */
		uint8_t uart_tx_pin;                                        /* Uart发送管脚Pin口 */
		uint8_t uart_rx_port;                                       /* Uart接收管脚Port口 */
		uint8_t uart_rx_pin;                                        /* Uart接收管脚Pin口 */
		uint8_t uart_baud;                                          /* Uart波特率 */
		uint8_t res_uart[10];                                       /* 预留 */
	}UartCfg;

	struct
	{
		uint32_t can_recv_addr;                                     /* CAN接收地址 */
		uint32_t can_send_addr;                                     /* CAN发送地址 */
		uint32_t can_filter_id0;                                    /* CAN过滤ID0 */
		uint32_t can_filter_mask0;                                  /* CAN过滤mask0 */
		uint32_t can_filter_id1;                                    /* CAN过滤ID1 */
		uint32_t can_filter_mask1;                                  /* CAN过滤mask1 */
		uint8_t can_channel;                                        /* CAN通道号 */
		uint8_t canfd_baud_slow;                                    /* CANFD低速波特率 */
		uint8_t canfd_baud_fast;                                    /* CANFD高速波特率 */
		uint8_t can_tx_port;                                        /* CAN发送管脚Port口 */
		uint8_t can_tx_pin;                                         /* CAN发送管脚Pin口 */
		uint8_t can_rx_port;                                        /* CAN接收管脚Port口 */
		uint8_t can_rx_pin;                                         /* CAN接收管脚Pin口 */
		uint8_t can_if_transceiver_ctrl;                            /* CAN收发器stdby脚拉低与否 */
		uint8_t can_transceiver_port;                               /* CAN收发器stdby脚控制Port口 */
		uint8_t can_transceiver_pin;                                /* CAN收发器stdby脚控制Pin口 */
		uint8_t res_can[14];                                        /* 预留 */
	}CanCfg;

	struct
	{
		uint8_t sd_spi_channel;                                     /* SD卡SPI通道号 */
		uint8_t res_sd[19];                                         /* 预留 */
	}SdCfg;

	struct
	{
		uint8_t usb_if_use_enable;                                  /* USB是否使用使能 */
		uint8_t usb_ctrl_enable_port;                               /* USB使能Port口 */
		uint8_t usb_ctrl_enable_pin;                                /* USB使能Pin口 */
		uint8_t usb_if_use_insert_check;                            /* USB是否使用插入检测 */
		uint8_t usb_ctrl_insert_check_port;                         /* USB插入检测Port口 */
		uint8_t usb_ctrl_insert_check_pin;                          /* USB插入检测Pin口 */
		uint8_t res_usb[18];                                        /* 预留 */
	}UsbCfg;

	struct
	{
		uint8_t touch_x_plus_port;                                  /* 触摸X+Port口 */
		uint8_t touch_x_plus_pin;                                   /* 触摸X+Pin口 */
		uint8_t touch_y_plus_port;                                  /* 触摸Y+Port口 */
		uint8_t touch_y_plus_pin;                                   /* 触摸Y+Pin口 */
		uint8_t touch_x_sub_port;                                   /* 触摸X-Port口 */
		uint8_t touch_x_sub_pin;                                    /* 触摸X-Pin口 */
		uint8_t touch_y_sub_port;                                   /* 触摸Y-Port口 */
		uint8_t touch_y_sub_pin;                                    /* 触摸Y-Pin口 */
		uint8_t touch_i2c_scl_port;                                 /* 触摸SCL Port口 */
		uint8_t touch_i2c_scl_pin;                                  /* 触摸SCL Pin口 */
		uint8_t touch_i2c_sda_port;                                 /* 触摸SDA Port口 */
		uint8_t touch_i2c_sda_pin;                                  /* 触摸SDA Pin口 */
		uint8_t touch_i2c_irq_port;                                 /* 触摸IRQ Port口 */
		uint8_t touch_i2c_irq_pin;                                  /* 触摸IRQ Pin口 */
		uint8_t touch_i2c_rst_port;                                 /* 触摸RST Port口 */
		uint8_t touch_i2c_rst_pin;                                  /* 触摸RST Pin口 */
		uint8_t res_touch[8];                                       /* 预留 */
	}TouchCfg;

	struct
	{
		uint8_t sys_if_use_pro_bin;                                 /* 是否从product.bin解析参数 */
		uint8_t sys_if_use_display;                                 /* 是否使用Display */
		uint8_t sys_if_bl_all_on;                                   /* 是否背光一直亮 */
		uint8_t sys_ctrl_bl_port;                                   /* 背光控制管脚Port口 */
		uint8_t sys_ctrl_bl_pin;                                    /* 背光控制管脚Pin口 */
		uint8_t sys_buzzer_port;                                    /* 蜂鸣器Port口 */
		uint8_t sys_buzzer_pin;                                     /* 蜂鸣器Pin口 */
		uint8_t sys_test_port;                                      /* 系统测试脚Port口 */
		uint8_t sys_test_pin;                                       /* 系统测试脚Pin口 */
		uint8_t sys_if_partial_refresh_en;                          /* 是否使能局部刷新 */
		uint8_t res_sys[46];                                        /* 预留 */
	}SysCfg;

}CFG_tstUpgCfg;


#endif

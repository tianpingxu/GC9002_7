#include <stdio.h>
#include <stdint.h>
#include "cfg_autogen_c1.h"


const  __attribute__ ((aligned(4))) uint8_t CFG_au8CfgBin[] =
{
	0x49,0x4d,0x48,0x41,0x02,0x01,0x4d,0x05,0xcc,0x00,0x00,0x00,0x01,0x00,0x01,0x01,
	0xf4,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x01,0x09,0x01,
	0x08,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc7,0x07,0x00,0x00,
	0xcf,0x07,0x00,0x00,0xc7,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0xdf,0x07,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x01,0x05,0x01,0x12,0x01,0x13,0x01,0x01,0x11,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x01,0x01,0x0c,0x00,0x01,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x0f,0x01,0x0e,0x01,0x0d,0x01,0x0c,
	0x01,0x0f,0x01,0x0e,0x01,0x0d,0x01,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x01,0x01,0x01,0x01,0x04,0x01,0x08,0x01,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

CFG_tstUpgCfg * g_pCfg;

void CFG_SetCfgPtr(CFG_tstUpgCfg *pCfg)
{
	g_pCfg = pCfg;
}

CFG_tstUpgCfg* CFG_GetCfgPtrOnRom(void)
{
	return (CFG_tstUpgCfg*)CFG_au8CfgBin;
}

uint32_t CFG_Get_valid_flag(void)
{
	return g_pCfg->Head.valid_flag;
}


uint16_t CFG_Get_version(void)
{
	return g_pCfg->Head.version;
}


uint16_t CFG_Get_checksum(void)
{
	return g_pCfg->Head.checksum;
}


uint32_t CFG_Get_size(void)
{
	return g_pCfg->Head.size;
}


uint8_t CFG_Get_cfg_uart(void)
{
	return g_pCfg->UpgCfg.cfg_uart;
}


uint8_t CFG_Get_cfg_can(void)
{
	return g_pCfg->UpgCfg.cfg_can;
}


uint8_t CFG_Get_cfg_sd(void)
{
	return g_pCfg->UpgCfg.cfg_sd;
}


uint8_t CFG_Get_cfg_usb(void)
{
	return g_pCfg->UpgCfg.cfg_usb;
}


uint16_t CFG_Get_can_uart_wait_ms(void)
{
	return g_pCfg->UpgCfg.can_uart_wait_ms;
}


uint8_t* CFG_Get_res_upg(void)
{
	return g_pCfg->UpgCfg.res_upg;
}


uint8_t CFG_Get_uart_channel(void)
{
	return g_pCfg->UartCfg.uart_channel;
}


uint8_t CFG_Get_uart_tx_port(void)
{
	return g_pCfg->UartCfg.uart_tx_port;
}


uint8_t CFG_Get_uart_tx_pin(void)
{
	return g_pCfg->UartCfg.uart_tx_pin;
}


uint8_t CFG_Get_uart_rx_port(void)
{
	return g_pCfg->UartCfg.uart_rx_port;
}


uint8_t CFG_Get_uart_rx_pin(void)
{
	return g_pCfg->UartCfg.uart_rx_pin;
}


uint8_t CFG_Get_uart_baud(void)
{
	return g_pCfg->UartCfg.uart_baud;
}


uint8_t* CFG_Get_res_uart(void)
{
	return g_pCfg->UartCfg.res_uart;
}


uint32_t CFG_Get_can_recv_addr(void)
{
	return g_pCfg->CanCfg.can_recv_addr;
}


uint32_t CFG_Get_can_send_addr(void)
{
	return g_pCfg->CanCfg.can_send_addr;
}


uint32_t CFG_Get_can_filter_id0(void)
{
	return g_pCfg->CanCfg.can_filter_id0;
}


uint32_t CFG_Get_can_filter_mask0(void)
{
	return g_pCfg->CanCfg.can_filter_mask0;
}


uint32_t CFG_Get_can_filter_id1(void)
{
	return g_pCfg->CanCfg.can_filter_id1;
}


uint32_t CFG_Get_can_filter_mask1(void)
{
	return g_pCfg->CanCfg.can_filter_mask1;
}


uint8_t CFG_Get_can_channel(void)
{
	return g_pCfg->CanCfg.can_channel;
}


uint8_t CFG_Get_canfd_baud_slow(void)
{
	return g_pCfg->CanCfg.canfd_baud_slow;
}


uint8_t CFG_Get_canfd_baud_fast(void)
{
	return g_pCfg->CanCfg.canfd_baud_fast;
}


uint8_t CFG_Get_can_tx_port(void)
{
	return g_pCfg->CanCfg.can_tx_port;
}


uint8_t CFG_Get_can_tx_pin(void)
{
	return g_pCfg->CanCfg.can_tx_pin;
}


uint8_t CFG_Get_can_rx_port(void)
{
	return g_pCfg->CanCfg.can_rx_port;
}


uint8_t CFG_Get_can_rx_pin(void)
{
	return g_pCfg->CanCfg.can_rx_pin;
}


uint8_t CFG_Get_can_if_transceiver_ctrl(void)
{
	return g_pCfg->CanCfg.can_if_transceiver_ctrl;
}


uint8_t CFG_Get_can_transceiver_port(void)
{
	return g_pCfg->CanCfg.can_transceiver_port;
}


uint8_t CFG_Get_can_transceiver_pin(void)
{
	return g_pCfg->CanCfg.can_transceiver_pin;
}


uint8_t* CFG_Get_res_can(void)
{
	return g_pCfg->CanCfg.res_can;
}


uint8_t CFG_Get_sd_spi_channel(void)
{
	return g_pCfg->SdCfg.sd_spi_channel;
}


uint8_t* CFG_Get_res_sd(void)
{
	return g_pCfg->SdCfg.res_sd;
}


uint8_t CFG_Get_usb_if_use_enable(void)
{
	return g_pCfg->UsbCfg.usb_if_use_enable;
}


uint8_t CFG_Get_usb_ctrl_enable_port(void)
{
	return g_pCfg->UsbCfg.usb_ctrl_enable_port;
}


uint8_t CFG_Get_usb_ctrl_enable_pin(void)
{
	return g_pCfg->UsbCfg.usb_ctrl_enable_pin;
}


uint8_t CFG_Get_usb_if_use_insert_check(void)
{
	return g_pCfg->UsbCfg.usb_if_use_insert_check;
}


uint8_t CFG_Get_usb_ctrl_insert_check_port(void)
{
	return g_pCfg->UsbCfg.usb_ctrl_insert_check_port;
}


uint8_t CFG_Get_usb_ctrl_insert_check_pin(void)
{
	return g_pCfg->UsbCfg.usb_ctrl_insert_check_pin;
}


uint8_t* CFG_Get_res_usb(void)
{
	return g_pCfg->UsbCfg.res_usb;
}


uint8_t CFG_Get_touch_x_plus_port(void)
{
	return g_pCfg->TouchCfg.touch_x_plus_port;
}


uint8_t CFG_Get_touch_x_plus_pin(void)
{
	return g_pCfg->TouchCfg.touch_x_plus_pin;
}


uint8_t CFG_Get_touch_y_plus_port(void)
{
	return g_pCfg->TouchCfg.touch_y_plus_port;
}


uint8_t CFG_Get_touch_y_plus_pin(void)
{
	return g_pCfg->TouchCfg.touch_y_plus_pin;
}


uint8_t CFG_Get_touch_x_sub_port(void)
{
	return g_pCfg->TouchCfg.touch_x_sub_port;
}


uint8_t CFG_Get_touch_x_sub_pin(void)
{
	return g_pCfg->TouchCfg.touch_x_sub_pin;
}


uint8_t CFG_Get_touch_y_sub_port(void)
{
	return g_pCfg->TouchCfg.touch_y_sub_port;
}


uint8_t CFG_Get_touch_y_sub_pin(void)
{
	return g_pCfg->TouchCfg.touch_y_sub_pin;
}


uint8_t CFG_Get_touch_i2c_scl_port(void)
{
	return g_pCfg->TouchCfg.touch_i2c_scl_port;
}


uint8_t CFG_Get_touch_i2c_scl_pin(void)
{
	return g_pCfg->TouchCfg.touch_i2c_scl_pin;
}


uint8_t CFG_Get_touch_i2c_sda_port(void)
{
	return g_pCfg->TouchCfg.touch_i2c_sda_port;
}


uint8_t CFG_Get_touch_i2c_sda_pin(void)
{
	return g_pCfg->TouchCfg.touch_i2c_sda_pin;
}


uint8_t CFG_Get_touch_i2c_irq_port(void)
{
	return g_pCfg->TouchCfg.touch_i2c_irq_port;
}


uint8_t CFG_Get_touch_i2c_irq_pin(void)
{
	return g_pCfg->TouchCfg.touch_i2c_irq_pin;
}


uint8_t CFG_Get_touch_i2c_rst_port(void)
{
	return g_pCfg->TouchCfg.touch_i2c_rst_port;
}


uint8_t CFG_Get_touch_i2c_rst_pin(void)
{
	return g_pCfg->TouchCfg.touch_i2c_rst_pin;
}


uint8_t* CFG_Get_res_touch(void)
{
	return g_pCfg->TouchCfg.res_touch;
}


uint8_t CFG_Get_sys_if_use_pro_bin(void)
{
	return g_pCfg->SysCfg.sys_if_use_pro_bin;
}


uint8_t CFG_Get_sys_if_use_display(void)
{
	return g_pCfg->SysCfg.sys_if_use_display;
}


uint8_t CFG_Get_sys_if_bl_all_on(void)
{
	return g_pCfg->SysCfg.sys_if_bl_all_on;
}


uint8_t CFG_Get_sys_ctrl_bl_port(void)
{
	return g_pCfg->SysCfg.sys_ctrl_bl_port;
}


uint8_t CFG_Get_sys_ctrl_bl_pin(void)
{
	return g_pCfg->SysCfg.sys_ctrl_bl_pin;
}


uint8_t CFG_Get_sys_buzzer_port(void)
{
	return g_pCfg->SysCfg.sys_buzzer_port;
}


uint8_t CFG_Get_sys_buzzer_pin(void)
{
	return g_pCfg->SysCfg.sys_buzzer_pin;
}


uint8_t CFG_Get_sys_test_port(void)
{
	return g_pCfg->SysCfg.sys_test_port;
}


uint8_t CFG_Get_sys_test_pin(void)
{
	return g_pCfg->SysCfg.sys_test_pin;
}


uint8_t CFG_Get_sys_if_partial_refresh_en(void)
{
	return g_pCfg->SysCfg.sys_if_partial_refresh_en;
}


uint8_t* CFG_Get_res_sys(void)
{
	return g_pCfg->SysCfg.res_sys;
}


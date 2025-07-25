/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
/******************************************************************************
 * @file     csi_porting.h
 * @brief    header File for Porting Driver
 * @version  V1.0
 * @date     2023/02/28
 * @model    porting
 ******************************************************************************/
#ifndef __CSI_PORTING_H__
#define __CSI_PORTING_H__

#include <csi_common.h>
#include <csi_irq.h>

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_CSI_DRV_PORTING_ENABLED


uint32_t soc_get_apb_freq(uint32_t idx);
uint32_t soc_get_ahb_freq(uint32_t idx);
uint32_t soc_get_cpu_freq(uint32_t idx);

uint32_t soc_get_uart_freq(uint32_t idx);
uint32_t soc_get_spi_freq(uint32_t idx);
uint32_t soc_get_iic_freq(uint32_t idx);
uint32_t soc_get_i2s_freq(uint32_t idx);
uint32_t soc_get_pwm_freq(uint32_t idx);
uint32_t soc_get_adc_freq(uint32_t idx);
uint32_t soc_get_qspi_freq(uint32_t idx);
uint32_t soc_get_usi_freq(uint32_t idx);
uint32_t soc_get_timer_freq(uint32_t idx);
uint32_t soc_get_rtc_freq(uint32_t idx);
uint32_t soc_get_wdt_freq(uint32_t idx);
uint32_t soc_get_sdio_freq(uint32_t idx);
uint32_t soc_get_emmc_freq(uint32_t idx);
uint32_t soc_get_usb_freq(uint32_t idx);
uint32_t soc_get_coretime_freq(void);
uint32_t soc_get_cur_cpu_freq(void);
uint32_t soc_get_ref_clk_freq(uint32_t idx);
void soc_set_sys_freq(uint32_t freq);
void soc_clk_enable(int32_t module);
void soc_clk_disable(int32_t module);
uint32_t soc_get_cpu_id(void);
void soc_dcache_clean_invalid_range(unsigned long addr, uint32_t size);
void soc_dcache_clean_invalid_all(void);
void soc_dcache_invalid_range(unsigned long addr, uint32_t size);
void soc_dcache_clean(void);
void soc_icache_invalid(void);
void soc_dma_address_remap(unsigned long addr);
typedef enum
{
    PM_MODE_RUN,            //运行模式
    PM_MODE_SLEEP_1,        //睡眠等级1模式
    PM_MODE_SLEEP_2,        //睡眠等级2模式
    PM_MODE_DEEP_SLEEP_1,   //深度睡眠等级1模式
    PM_MODE_DEEP_SLEEP_2,   //深度睡眠等级2模式
}csi_pm_mode_t;
csi_error_t soc_pm_enter_sleep(csi_pm_mode_t mode);
csi_error_t soc_pm_config_wakeup_source(uint32_t wakeup_num, bool enable);


#endif /* CONFIG_CSI_DRV_PORTING_ENABLED */
#ifdef __cplusplus
}
#endif
#endif /* __CSI_PORTING_H__ */
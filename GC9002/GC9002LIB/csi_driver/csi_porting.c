#include <csi_porting.h>
#ifdef CONFIG_CSI_DRV_PORTING_ENABLED


uint32_t soc_get_apb_freq(uint32_t idx)
{
    (void)idx;
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
    return CONFIG_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAPBClock();
#endif
}

uint32_t soc_get_ahb_freq(uint32_t idx)
{
    (void)idx;
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
    return CONFIG_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAHBClock();
#endif
}

uint32_t soc_get_cpu_freq(uint32_t idx)
{
    (void)idx;
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
    return CONFIG_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAHBClock();
#endif
}

uint32_t soc_get_uart_freq(uint32_t idx)
{
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
    return CONFIG_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAPBClock();
#endif
}

uint32_t soc_get_spi_freq(uint32_t idx)
{
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
    return CONFIG_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAPBClock();
#endif
}

uint32_t soc_get_iic_freq(uint32_t idx)
{
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
    return CONFIG_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAPBClock();
#endif
}

uint32_t soc_get_i2s_freq(uint32_t idx)
{
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
    return CONFIG_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAPBClock();
#endif
}

uint32_t soc_get_pwm_freq(uint32_t idx)
{
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
    return CONFIG_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAPBClock();
#endif
}

uint32_t soc_get_adc_freq(uint32_t idx)
{
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
    return CONFIG_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAPBClock();
#endif
}

uint32_t soc_get_qspi_freq(uint32_t idx)
{
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
    return CONFIG_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAPBClock();
#endif
}

uint32_t soc_get_usi_freq(uint32_t idx)
{
    return 0;
}
uint32_t soc_get_timer_freq(uint32_t idx)
{
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
    return CONFIG_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAPBClock();
#endif
}

uint32_t soc_get_rtc_freq(uint32_t idx)
{
    return 31250;
}

uint32_t soc_get_wdt_freq(uint32_t idx)
{
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
    return CONFIG_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAPBClock();
#endif
}

uint32_t soc_get_sdio_freq(uint32_t idx)
{
    return 0;
}

uint32_t soc_get_emmc_freq(uint32_t idx)
{
    return 0;
}

uint32_t soc_get_usb_freq(uint32_t idx)
{
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
    return CONFIG_FPGA_SOC_FREQ;
#else
    return 0;
#endif
}

uint32_t soc_get_coretime_freq(void)
{
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
    return CONFIG_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAHBClock();
#endif
}

uint32_t soc_get_cur_cpu_freq(void)
{
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
    return CONFIG_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAHBClock();
#endif
}

uint32_t soc_get_ref_clk_freq(uint32_t idx)
{
    return 0;
}

void soc_set_sys_freq(uint32_t freq)
{
    (void)freq;
	// RCC system clock configuration, AHB clock = pll / AHB pres
    HAL_RCC_SYSCLKConfig(RCC_PLL_432M, RCC_AHB_Pres_3);
}

void soc_clk_enable(int32_t module)
{
    
}

void soc_clk_disable(int32_t module)
{
    
}

uint32_t soc_get_cpu_id(void)
{
    return 0;
}

void soc_dcache_clean_invalid_range(unsigned long addr, uint32_t size)
{
    ccm_FlushInvalidateDCache_by_Addr((uint32_t *)addr, size);
}

void soc_dcache_clean_invalid_all(void)
{
    ccm_FlushInvalidateDCache();
}

void soc_dcache_invalid_range(unsigned long addr, uint32_t size)
{
    (void)addr;
    (void)size;
}

void soc_dcache_clean(void)
{
    ccm_FlushDCache();
}

void soc_icache_invalid(void)
{
    
}

void soc_dma_address_remap(unsigned long addr)
{
    (void)addr;
}

csi_error_t soc_pm_enter_sleep(csi_pm_mode_t mode)
{
    (void)mode;
    return CSI_UNSUPPORTED;
}

csi_error_t soc_pm_config_wakeup_source(uint32_t wakeup_num, bool enable)
{
    (void)wakeup_num;
    (void)enable;
    return CSI_UNSUPPORTED;
}

#endif /* CONFIG_CSI_DRV_PORTING_ENABLED */
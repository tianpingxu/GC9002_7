#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "csi_config.h"
#include <board.h>

#ifdef __cplusplus
extern "C" {
#endif


static inline int32_t drv_get_cpu_freq(int32_t idx)
{
#if(defined USE_FPGA_SOC && USE_FPGA_SOC == 1U)
    return USE_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAHBClock();
#endif
}

static inline int32_t drv_get_sys_freq(void)
{
#if(defined USE_FPGA_SOC && USE_FPGA_SOC == 1U)
    return USE_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAHBClock();
#endif
}

static inline int32_t drv_get_apb_freq(void)
{
#if(defined USE_FPGA_SOC && USE_FPGA_SOC == 1U)
    return USE_FPGA_SOC_FREQ;
#else
    return HAL_RCC_GetAPBClock();
#endif
}

void mdelay(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* _SYSTEM_H_ */
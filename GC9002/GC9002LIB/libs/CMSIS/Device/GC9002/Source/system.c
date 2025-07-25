/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     system.c
 * @brief    CSI Device System Source File
 * @version  V1.0
 * @date     02. Oct 2018
 ******************************************************************************/

#include <csi_config.h>
#include <system.h>
#include <soc.h>
#include <csi_irq.h>
#include <csi_tick.h>

extern void mm_heap_initialize(void);
extern void csi_hal_internal_flash_init(void);

int32_t drv_get_cpu_id(void)
{
    return 0;
}

#ifdef CONFIG_KERNEL_NONE
static void _system_init_for_baremetal(void)
{
    /* enable mexstatus SPUSHEN */
#if ((CONFIG_CPU_GC9002 != 1) && (CONFIG_CPU_E902M != 1))
    uint32_t mexstatus = __get_MEXSTATUS();
    mexstatus |= (1 << 16);
    __set_MEXSTATUS(mexstatus);
#endif

    csi_irq_vectors_init();
    csi_tick_init();
    __enable_excp_irq();

    mm_heap_initialize();
}
#endif

#ifndef CONFIG_KERNEL_NONE
static void _system_init_for_kernel(void)
{
     /* enable mexstatus SPUSHEN and SPSWAPEN */
#if ((CONFIG_CPU_GC9002 != 1) && (CONFIG_CPU_E902M != 1))
    uint32_t mexstatus = __get_MEXSTATUS();
    mexstatus |= (0x3 << 16);
    __set_MEXSTATUS(mexstatus);
#endif

    csi_irq_vectors_init();
    __disable_excp_irq();
    csi_tick_init();

#ifndef CONFIG_KERNEL_RHINO
#ifndef CONFIG_NUTTXMM_NONE
    mm_heap_initialize();
#endif
#endif
}
#endif

/**
  * @brief  initialize system map
  * @param  None
  * @return None
  */
void systemmap_config(void)
{
   csi_sysmap_config_region(0, 0x20000000, SYSMAP_SYSMAPCFG_B_Msk | SYSMAP_SYSMAPCFG_C_Msk);
   csi_sysmap_config_region(1, 0x40000000, SYSMAP_SYSMAPCFG_B_Msk | SYSMAP_SYSMAPCFG_C_Msk);
   csi_sysmap_config_region(2, 0x50000000, SYSMAP_SYSMAPCFG_SO_Msk);
   csi_sysmap_config_region(3, 0x50700000, SYSMAP_SYSMAPCFG_B_Msk | SYSMAP_SYSMAPCFG_C_Msk);
   csi_sysmap_config_region(4, 0x60000000, SYSMAP_SYSMAPCFG_SO_Msk);
   csi_sysmap_config_region(5, 0x80000000, SYSMAP_SYSMAPCFG_B_Msk | SYSMAP_SYSMAPCFG_C_Msk);
   csi_sysmap_config_region(6, 0x90000000, SYSMAP_SYSMAPCFG_B_Msk | SYSMAP_SYSMAPCFG_C_Msk);
   csi_sysmap_config_region(7, 0xf0000000, SYSMAP_SYSMAPCFG_SO_Msk);
}

/**
  * @brief  initialize the system
  *         Initialize the psr and vbr.
  * @param  None
  * @return None
  */
void SystemInit(void)
{
#if(defined CONFIG_RUN_BOOTLOADER && CONFIG_RUN_BOOTLOADER == 1U)
#if(defined CONFIG_USE_HYPER && CONFIG_USE_HYPER == 1U)
    if(HAL_HYPER_CheckStatus() != HAL_OK)
    {
        HAL_RCC_SystemReset();
    }
#endif
#endif

    int i;
#if ((CONFIG_CPU_GC9002 != 1) && (CONFIG_CPU_E902M != 1))
    systemmap_config();
#endif

    /* enable mstatus FS */
#if (__riscv_flen)
    uint32_t mstatus = __get_MSTATUS();
    mstatus |= (1 << 13);
    __set_MSTATUS(mstatus);
#endif

    //csi_relocate_vector_table();
    /* get interrupt level from info */
    CLIC->CFG = (((CLIC->INFO & CLIC_CLICINFO_CTLBIT_Msk) >> CLIC_CLICINFO_CTLBIT_Pos) << CLIC_CLICCFG_NLBIT_Pos);

    for (i = 0; i < 64; i++) {
        CLIC->CTRL[i].IP = 0;
        CLIC->CTRL[i].ATTR = 1; /* use vector interrupt */
    }

    /* tspend use positive interrupt */
    CLIC->CTRL[Machine_Software_IRQn].ATTR = 0x3;

#if(defined CONFIG_RUN_BOOTLOADER && CONFIG_RUN_BOOTLOADER == 1U)

    HAL_PMU_VTR3318_Calibrate(200);
    HAL_PMU_VTR12_Calibrate(200);
#endif

#if ((CONFIG_CPU_GC9002 != 1) && (CONFIG_CPU_E902M != 1))
    csi_dcache_enable();
#endif
#if(defined CONFIG_USE_ICACHE && CONFIG_USE_ICACHE ==1U)
    csi_icache_enable();
#endif /* CONFIG_USE_ICACHE */
    csi_irq_enable(Machine_Software_IRQn);

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    extern void NonVectorHandlers_Init(void);
    NonVectorHandlers_Init();// Non Vector Handler init
#endif /* CONFIG_CSI_NON_VECTOR */
    /* Temporary prevent re-initialize RCCC and Hyper solution: read RCC->SYS_PLL_CTRL value and judge if intialized */
    //if(RCC->SYS_PLL_CTRL.reg_u32val != 0x8108c1e0)
    {
	    // RCC system clock configuration, AHB clock = pll / AHB pres
        HAL_RCC_SYSCLKConfig(CONFIG_PLL_FREQ, CONFIG_AHB_DIV);
    
#if(defined CONFIG_USE_HYPER && CONFIG_USE_HYPER == 1U)
        // Hyper Configation
        HYPER_HandleTypeDef hhyper;
    	hhyper.Instance = HYPER;
		hhyper.Pres = CONFIG_HYPER_DIV;
		hhyper.MapAHBAddrBase = HYPER_DATA_BASE;
        /* Prevent init HYPER RAM repeatlly */
		HAL_HYPER_Init(&hhyper);

#if(defined CONFIG_USE_DCACHE && CONFIG_USE_DCACHE == 1U)
	// DCache Configation
#if(defined CONFIG_BOOT_DCACHE_DISABLE && CONFIG_BOOT_DCACHE_DISABLE == 1U)
#else

	    ccm_EnableDCache();
#endif /* CONFIG_BOOT_DCACHE_DISABLE */
#endif /* CONFIG_USE_DCACHE */
#endif /* CONFIG_USE_HYPER */
    }
    
#if(defined CONFIG_CQSPIFLASH && CONFIG_CQSPIFLASH == 1U)
	csi_hal_internal_flash_init();
#endif
 
#ifdef CONFIG_KERNEL_NONE
    _system_init_for_baremetal();
#else
    _system_init_for_kernel();
#endif /* CONFIG_KERNEL_NONE */
}


static void _mdelay(void)
{
    uint64_t start, cur, delta;
    uint32_t cnt = (drv_get_sys_freq() / 1000);
    start = csi_coret_get_value_u64();

    while (1) {
        cur = csi_coret_get_value_u64();
        delta = cur - start;

        if (delta >= cnt) {
            return;
        }
    }
}

void mdelay(uint32_t ms)
{
    if (ms == 0) {
        return;
    }

    while (ms--) {
        _mdelay();
    }
}






#include <csi_tick.h>
#ifdef CONFIG_CSI_DRV_TICK_ENABLED

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_CORET_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_CORET_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    extern void csi_tick_config(void);
    csi_tick_config();
    g_sys_tick++;
#if defined(CONFIG_KERNEL_RHINO)
    systick_handler();
#elif defined(CONFIG_KERNEL_FREERTOS)
    extern void xPortSysTickHandler(void);
    xPortSysTickHandler();
#elif defined(CONFIG_KERNEL_UCOS)
    OSTimeTick();
#endif

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

void csi_tick_config(void)
{
    csi_coret_config(drv_get_sys_freq() / CONFIG_SYSTICK_HZ, CORET_IRQn);    //10ms
}
csi_error_t csi_tick_init(void)
{
    csi_tick_config();
//    csi_irq_enable(CORET_IRQn);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(CORET_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 2, CSI_CORET_IRQHandler);
#else
    csi_clic_register_irq(CORET_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 2, CSI_CORET_IRQHandler);
#endif
    return CSI_OK;
}
void csi_tick_uninit(void)
{
    csi_irq_disable(CORET_IRQn);
}

uint64_t csi_tick_get(void)
{
    return csi_coret_get_value_u64();
}
uint64_t csi_tick_get_s(void)
{
    return (uint64_t)(csi_tick_get_us()/1000000);
}
uint64_t csi_tick_get_ms(void)
{
    return (uint64_t)(csi_tick_get_us()/1000);
}
uint64_t csi_tick_get_us(void)
{
    uint64_t retVal = csi_coret_get_value_u64();
    uint64_t div = drv_get_sys_freq()/1000000;
    return (uint64_t)(retVal/div);
}
void csi_tick_increase(void)
{
    
}

#endif /* CONFIG_CSI_DRV_TICK_ENABLED */
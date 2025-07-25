#include <csi_wdt.h>
#ifdef CONFIG_CSI_DRV_WDT_ENABLED

static WDT_HandleTypeDef handle_wdg = {.Instance = WDT};

#define CSI_WDT_LEI     48//单位 MHz

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_WDT_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_WDT_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    if(handle_wdg.priv_obj != NULL)
    {
        if(((csi_wdt_t *)(handle_wdg.priv_obj))->callback != NULL)
        {
            ((csi_wdt_t *)(handle_wdg.priv_obj))->callback((csi_wdt_t *)(handle_wdg.priv_obj), ((csi_wdt_t *)(handle_wdg.priv_obj))->arg);
        }
    }
    
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

csi_error_t csi_wdt_init(csi_wdt_t *wdt, uint32_t idx)
{
    if(wdt == NULL || idx >= CSI_WDT_CNT)
    {
        return CSI_ERROR;
    }
    if(idx == 0)
    {
        HANDLE_REG_BASE(wdt) = WDT_BASE;
        HANDLE_IRQ_NUM(wdt) = WDT_IRQn;
        HANDLE_DEV_IDX(wdt) = 0;
        HANDLE_IRQ_HANDLER(wdt) = (void *)CSI_WDT_IRQHandler;
    }
    wdt->priv = (void *)&handle_wdg;
    handle_wdg.priv_obj = wdt;
    ((WDT_HandleTypeDef *)(wdt->priv))->Instance = WDT;
	HAL_WDT_StructInit((WDT_HandleTypeDef *)(wdt->priv));
    return CSI_OK;
}
csi_error_t csi_wdt_set_timeout(csi_wdt_t *wdt, uint32_t ms)
{
    if(wdt == NULL)
    {
        return CSI_ERROR;
    }
    if(wdt->priv == NULL)
    {
        return CSI_ERROR;
    }
    uint64_t timeout = ms;
    timeout = timeout*1000*CSI_WDT_LEI;
    if(timeout < 128000)
    {
        ((WDT_HandleTypeDef *)(wdt->priv))->Init.Counter = WDT_Timeout_64K_CLOCKS;
    }
    else if(timeout >= 128000 && timeout < 256000)
    {
        ((WDT_HandleTypeDef *)(wdt->priv))->Init.Counter = WDT_Timeout_128K_CLOCKS;
    }
    else if(timeout >= 256000 && timeout < 512000)
    {
        ((WDT_HandleTypeDef *)(wdt->priv))->Init.Counter = WDT_Timeout_256K_CLOCKS;
    }
    else if(timeout >= 512000 && timeout < 1000000)
    {
        ((WDT_HandleTypeDef *)(wdt->priv))->Init.Counter = WDT_Timeout_512K_CLOCKS;
    }
    else if(timeout >= 1000000 && timeout < 2000000)
    {
        ((WDT_HandleTypeDef *)(wdt->priv))->Init.Counter = WDT_Timeout_1M_CLOCKS;
    }
    else if(timeout >= 2000000 && timeout < 4000000)
    {
        ((WDT_HandleTypeDef *)(wdt->priv))->Init.Counter = WDT_Timeout_2M_CLOCKS;
    }
    else if(timeout >= 4000000 && timeout < 8000000)
    {
        ((WDT_HandleTypeDef *)(wdt->priv))->Init.Counter = WDT_Timeout_4M_CLOCKS;
    }
    else if(timeout >= 8000000 && timeout < 16000000)
    {
        ((WDT_HandleTypeDef *)(wdt->priv))->Init.Counter = WDT_Timeout_8M_CLOCKS;
    }
    else if(timeout >= 16000000 && timeout < 32000000)
    {
        ((WDT_HandleTypeDef *)(wdt->priv))->Init.Counter = WDT_Timeout_16M_CLOCKS;
    }
    else if(timeout >= 32000000 && timeout < 64000000)
    {
        ((WDT_HandleTypeDef *)(wdt->priv))->Init.Counter = WDT_Timeout_32M_CLOCKS;
    }
    else if(timeout >= 64000000 && timeout < 128000000)
    {
        ((WDT_HandleTypeDef *)(wdt->priv))->Init.Counter = WDT_Timeout_64M_CLOCKS;
    }
    else if(timeout >= 128000000 && timeout < 256000000)
    {
        ((WDT_HandleTypeDef *)(wdt->priv))->Init.Counter = WDT_Timeout_128M_CLOCKS;
    }
    else if(timeout >= 256000000 && timeout < 512000000)
    {
        ((WDT_HandleTypeDef *)(wdt->priv))->Init.Counter = WDT_Timeout_256M_CLOCKS;
    }
    else if(timeout >= 512000000 && timeout < 1000000000)
    {
        ((WDT_HandleTypeDef *)(wdt->priv))->Init.Counter = WDT_Timeout_512M_CLOCKS;
    }
    else if(timeout >= 1000000000 && timeout < 2000000000)
    {
        ((WDT_HandleTypeDef *)(wdt->priv))->Init.Counter = WDT_Timeout_1G_CLOCKS;
    }
    else
    {
        ((WDT_HandleTypeDef *)(wdt->priv))->Init.Counter = WDT_Timeout_2G_CLOCKS;
    }
	((WDT_HandleTypeDef *)(wdt->priv))->Init.ResetMode = WDT_INTERRUPT_MODE;
	((WDT_HandleTypeDef *)(wdt->priv))->Init.ResetPulseLen = WDT_PCLK_CYCLES_32;
    return CSI_OK;
}
csi_error_t csi_wdt_start(csi_wdt_t *wdt)
{
    if(wdt == NULL)
    {
        return CSI_ERROR;
    }
    if(wdt->priv == NULL)
    {
        return CSI_ERROR;
    }
	HAL_WDT_Init((WDT_HandleTypeDef *)(wdt->priv));
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(HANDLE_IRQ_NUM(wdt), CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 4, HANDLE_IRQ_HANDLER(wdt));
#else
    csi_clic_register_irq(HANDLE_IRQ_NUM(wdt), CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 4, HANDLE_IRQ_HANDLER(wdt));
#endif
    HAL_WDT_Enable((WDT_HandleTypeDef *)(wdt->priv));
    return CSI_OK;
}
void csi_wdt_stop(csi_wdt_t *wdt)
{
    if(wdt == NULL)
    {
        return;
    }
    if(wdt->priv == NULL)
    {
        return;
    }
    HAL_WDT_Disable((WDT_HandleTypeDef *)(wdt->priv));
}
csi_error_t csi_wdt_feed(csi_wdt_t *wdt)
{
    if(wdt == NULL)
    {
        return CSI_ERROR;
    }
    if(wdt->priv == NULL)
    {
        return CSI_ERROR;
    }
    HAL_WDT_Refresh((WDT_HandleTypeDef *)(wdt->priv));
    return CSI_OK;
}
uint32_t csi_wdt_get_remaining_time(csi_wdt_t *wdt)
{
    if(wdt == NULL)
    {
        return CSI_ERROR;
    }
    if(wdt->priv == NULL)
    {
        return CSI_ERROR;
    }
    uint32_t timeout = HAL_WDT_GetTimeoutCounterValue((WDT_HandleTypeDef *)(wdt->priv));
    uint32_t cur = HAL_WDT_GetCurrentCounterValue((WDT_HandleTypeDef *)(wdt->priv));
    uint32_t ret_val = (timeout - cur)/CSI_WDT_LEI;
    return ret_val;
}
bool csi_wdt_is_running(csi_wdt_t *wdt)
{
    if(wdt == NULL)
    {
        return false;
    }
    if(wdt->priv == NULL)
    {
        return CSI_ERROR;
    }
    return __HAL_WDT_GET_EN((WDT_HandleTypeDef *)(wdt->priv));
}
csi_error_t csi_wdt_attach_callback(csi_wdt_t *wdt, void(*callback)(struct _csi_wdt *wdt, void *arg), void *arg)
{
    if(wdt == NULL || callback == NULL)
    {
        return CSI_ERROR;
    }
    wdt->callback = callback;
    wdt->arg = arg;
    return CSI_OK;
}
void csi_wdt_detach_callback(csi_wdt_t *wdt)
{
    if(wdt == NULL)
    {
        return;
    }
    wdt->callback = NULL;
    wdt->arg = NULL;
}
#endif /* CONFIG_CSI_DRV_WDT_ENABLED */
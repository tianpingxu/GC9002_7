#include <csi_qspi.h>
#ifdef CONFIG_CSI_DRV_QSPI_ENABLED

/* There is a bug in global variable initialization assignments, sometimes assignments are unsuccessful.
 * Therefore, the assignment operation needs to be implemented in the code.
 */
static struct qspi_dev_cfg_t cqspi_dev_cfg = {.base = CQSPI_BASE, .remap_bus_base = CQSPI_DATA_BASE};
static struct qspi_dev_t cqspi_dev = {.cfg = &cqspi_dev_cfg};
#if(defined CHIP_GC9002)
static struct qspi_dev_cfg_t dqspi_dev_cfg = {.base = DQSPI_BASE, .remap_bus_base = DQSPI_DATA_BASE};
static struct qspi_dev_t dqspi_dev = {.cfg = &dqspi_dev_cfg};
#endif

static struct qspi_irq_hdr qspi_irq_hdr_tab[CSI_QSPI_CNT];

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_CQSPI_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_CQSPI_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_DQSPI_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_DQSPI_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

__CSI_QSPI_DRIVER static void QSPI_MspInit(unsigned int base_addr)
{
#if((defined CHIP_GC9005))
    __HAL_RCC_CQSPI_CLK_ENABLE();
    __HAL_RCC_CQSPI_SET_PRES(3);
#elif((defined CHIP_GC9002))
    if(base_addr == CQSPI_BASE)
    {
        __HAL_RCC_CQSPI_CLK_ENABLE();
        __HAL_RCC_CQSPI_SET_PRES(2);
    }
    else if(base_addr == DQSPI_BASE)
    {
        __HAL_RCC_DQSPI_CLK_ENABLE();
        __HAL_RCC_DQSPI_SET_PRES(2);
    }
#endif
}
__CSI_QSPI_DRIVER csi_error_t csi_qspi_init(csi_qspi_t *qspi, uint32_t idx)
{
    if(qspi == NULL || idx >= CSI_QSPI_CNT)
    {
        return CSI_ERROR;
    }
    if(idx == 0)
    {
        cqspi_dev_cfg.base = CQSPI_BASE;
        cqspi_dev_cfg.remap_bus_base = CQSPI_DATA_BASE;
        cqspi_dev.cfg = &cqspi_dev_cfg;
        HANDLE_REG_BASE(qspi) = CQSPI_BASE;
        HANDLE_IRQ_NUM(qspi) = CQSPI_IRQn;
        HANDLE_DEV_IDX(qspi) = 0;
        HANDLE_IRQ_HANDLER(qspi) = (void *)CSI_CQSPI_IRQHandler;
        qspi->priv = (void *)&cqspi_dev;
    }
#if(defined CHIP_GC9002)
    else if(idx == 1)
    {
        dqspi_dev_cfg.base = DQSPI_BASE;
        dqspi_dev_cfg.remap_bus_base = DQSPI_DATA_BASE;
        dqspi_dev.cfg = &dqspi_dev_cfg;
        HANDLE_REG_BASE(qspi) = DQSPI_BASE;
        HANDLE_IRQ_NUM(qspi) = DQSPI_IRQn;
        HANDLE_DEV_IDX(qspi) = 1;
        HANDLE_IRQ_HANDLER(qspi) = (void *)CSI_DQSPI_IRQHandler;
        qspi->priv = (void *)&dqspi_dev;
    }
#endif
    qspi_reset_regs((struct qspi_dev_t *)qspi->priv);
    qspi_callback_register((struct qspi_dev_t *)qspi->priv, 0, QSPI_MspInit);
    return CSI_OK;
}
void csi_qspi_uninit(csi_qspi_t *qspi)
{
    if(qspi == NULL)
    {
        return;
    }
    HANDLE_REG_BASE(qspi) = 0;
    HANDLE_IRQ_NUM(qspi) = 0;
    HANDLE_DEV_IDX(qspi) = 0;
    HANDLE_IRQ_HANDLER(qspi) = NULL;
    qspi->priv = NULL;
}
csi_error_t csi_qspi_attach_callback(csi_qspi_t *qspi, void(*callback)(csi_qspi_event_t event, void *arg), void *arg)
{
    if(qspi == NULL || callback == NULL)
    {
        return CSI_ERROR;
    }
    if(HANDLE_DEV_IDX(qspi) >= CSI_QSPI_CNT)
    {
        return CSI_ERROR;
    }
    qspi_irq_hdr_tab[HANDLE_DEV_IDX(qspi)].hdr = callback;
    qspi_irq_hdr_tab[HANDLE_DEV_IDX(qspi)].arg = arg;
    return CSI_OK;
}
void csi_qspi_detach_callback(csi_qspi_t *qspi)
{
    if(qspi == NULL)
    {
        return;
    }
    if(HANDLE_DEV_IDX(qspi) >= CSI_QSPI_CNT)
    {
        return;
    }
    qspi_irq_hdr_tab[HANDLE_DEV_IDX(qspi)].hdr = NULL;
    qspi_irq_hdr_tab[HANDLE_DEV_IDX(qspi)].arg = NULL;
}
static __CSI_QSPI_DRIVER uint32_t csi_rcc_get_pll(void)
{
#if(defined USE_FPGA_SOC && USE_FPGA_SOC == 1U)
    return USE_FPGA_SOC_FREQ;
#else
    /*
     * F_CLKOUT = 8MHz / N * F / Q
     * */
    return (uint32_t)(8000000 / \
            RCC->SYS_PLL_CTRL.bit.sys_pll_n * \
            RCC->SYS_PLL_CTRL.bit.sys_pll_f / \
            RCC->SYS_PLL_CTRL.bit.sys_pll_q);
#endif
}
__CSI_QSPI_DRIVER uint32_t csi_qspi_frequence(csi_qspi_t *qspi, uint32_t hz)
{
    if(qspi == NULL || hz < CSI_QSPI_FREQ_MIN || hz > CSI_QSPI_FREQ_MAX)
    {
        return 0;
    }
    if(qspi->priv == NULL)
    {
        return 0;
    }
    uint32_t pll_freq = 0;
    uint32_t qspi_presc = 0;
    uint32_t qspi_freq = 0;
    uint32_t qspi_div = 0;
#if(defined CONFIG_FPGA_SOC && CONFIG_FPGA_SOC == 1U)
    pll_freq = CONFIG_FPGA_SOC_FREQ;
#else
    pll_freq = csi_rcc_get_pll();
#endif

    for(int i = 0; i <= 3; i++)
    {
#if(defined CHIP_GC9005)
        switch (i) {
        case 0: qspi_presc = 4;
            break;
        case 1: qspi_presc = 6;
            break;
        case 2: qspi_presc = 8;
            break;
        case 3: qspi_presc = 10;
            break;
        default:
            break;
        }
#elif(defined CHIP_GC9002)
        switch (i) {
        case 0: qspi_presc = 1;
            break;
        case 1: qspi_presc = 2;
            break;
        case 2: qspi_presc = 4;
            break;
        case 3: qspi_presc = 6;
            break;
        default:
            break;
        }
#endif
        qspi_freq = pll_freq/qspi_presc;
        qspi_div = qspi_freq/hz;
        if(qspi_freq <= CSI_BSP_QSPI_FREQ_MAX)
        {
            if(qspi_div >= QSPI_CFG_BAUD_DIV_MIN && qspi_div <= QSPI_CFG_BAUD_DIV_MAX)
            {
                if(HANDLE_REG_BASE(qspi) == CQSPI_BASE)
                {
                    __HAL_RCC_CQSPI_SET_PRES(i);
                    __HAL_RCC_CQSPI_CLK_ENABLE();
                }
#if(defined CHIP_GC9002)
                else if(HANDLE_REG_BASE(qspi) == DQSPI_BASE)
                {
                    __HAL_RCC_DQSPI_SET_PRES(i);
                    __HAL_RCC_DQSPI_CLK_ENABLE();
                }
#endif
                qspi_set_baud_rate_div((struct qspi_dev_t *)qspi->priv, qspi_div);
                return hz;
            }
        }
    }
    return 0;
}
__CSI_QSPI_DRIVER csi_error_t csi_qspi_mode(csi_qspi_t *qspi, csi_qspi_clock_mode_t mode)
{
    switch(mode)
    {
        case QSPI_CLOCK_MODE_0:
            qspi_set_clock_mode((struct qspi_dev_t *)qspi->priv, 0, 0);
        break;
        case QSPI_CLOCK_MODE_3:
            qspi_set_clock_mode((struct qspi_dev_t *)qspi->priv, 1, 1);
        break;
        default:
        break;
    }
    return CSI_OK;
}
__CSI_QSPI_DRIVER int32_t csi_qspi_send(csi_qspi_t *qspi, csi_qspi_command_t *cmd, const void *data, uint32_t size, uint32_t timeout)
{
    if(qspi == NULL || cmd == NULL)
    {
        return 0;
    }
    if(qspi->priv == NULL)
    {
        return 0;
    }
    qspi->tx_data = (void*)data;
    qspi->tx_size = size;
    qspi_set_spi_mode_base((struct qspi_dev_t *)qspi->priv, 
                           (enum qspi_spi_mode_t)cmd->instruction.bus_width, 
                           (enum qspi_spi_mode_t)cmd->address.bus_width, 
                           (enum qspi_spi_mode_t)cmd->data.bus_width, 
                           (enum qspi_spi_mode_t)cmd->address.bus_width, 
                           (enum qspi_spi_mode_t)cmd->data.bus_width); 
    qspi_enable((struct qspi_dev_t *)qspi->priv);
    qspi_send_cmd((struct qspi_dev_t *)qspi->priv,
                  (cmd->instruction.disabled ? QSPI_ARG_NOT_USED : cmd->instruction.value),
                  QSPI_ARG_PTR_NOT_USED,
                  QSPI_ARG_NOT_USED,
                  data,
                  size,
                  (cmd->address.disabled ? QSPI_ARG_NOT_USED : cmd->address.value),
                  (cmd->address.disabled ? QSPI_ARG_NOT_USED : (uint32_t)cmd->address.size),
                  cmd->dummy_count,
                  timeout);
    return size;
}
__CSI_QSPI_DRIVER uint32_t csi_qspi_receive(csi_qspi_t *qspi, csi_qspi_command_t *cmd, void *data, uint32_t size, uint32_t timeout)
{
    if(qspi == NULL || cmd == NULL)
    {
        return 0;
    }
    if(qspi->priv == NULL)
    {
        return 0;
    }
    qspi->rx_data = (void*)data;
    qspi->rx_size = size;
    qspi_set_spi_mode_base((struct qspi_dev_t *)qspi->priv, 
                           (enum qspi_spi_mode_t)cmd->instruction.bus_width, 
                           (enum qspi_spi_mode_t)cmd->address.bus_width, 
                           (enum qspi_spi_mode_t)cmd->data.bus_width, 
                           (enum qspi_spi_mode_t)cmd->address.bus_width, 
                           (enum qspi_spi_mode_t)cmd->data.bus_width); 
    qspi_enable((struct qspi_dev_t *)qspi->priv);
    if(qspi_send_cmd((struct qspi_dev_t *)qspi->priv,
                  (cmd->instruction.disabled ? QSPI_ARG_NOT_USED : cmd->instruction.value),
                  data,
                  size,
                  QSPI_ARG_PTR_NOT_USED,
                  QSPI_ARG_NOT_USED,
                  (cmd->address.disabled ? QSPI_ARG_NOT_USED : cmd->address.value),
                  (cmd->address.disabled ? QSPI_ARG_NOT_USED : (uint32_t)cmd->address.size),
                  cmd->dummy_count,
                  timeout) != QSPI_ERR_NONE)
    {
        return 0;
    }
    return size;
}
__CSI_QSPI_DRIVER uint32_t csi_qspi_send_receive(csi_qspi_t *qspi, csi_qspi_command_t *cmd, const void *tx_data, void *rx_data, uint32_t size, uint32_t timeout)
{
    if(qspi == NULL || cmd == NULL)
    {
        return 0;
    }
    if(qspi->priv == NULL)
    {
        return 0;
    }
    qspi->tx_data = (void*)tx_data;
    qspi->tx_size = size;
    qspi->rx_data = (void*)rx_data;
    qspi->rx_size = size;
    qspi_set_spi_mode_base((struct qspi_dev_t *)qspi->priv, 
                           (enum qspi_spi_mode_t)cmd->instruction.bus_width, 
                           (enum qspi_spi_mode_t)cmd->address.bus_width, 
                           (enum qspi_spi_mode_t)cmd->data.bus_width, 
                           (enum qspi_spi_mode_t)cmd->address.bus_width, 
                           (enum qspi_spi_mode_t)cmd->data.bus_width); 
    qspi_enable((struct qspi_dev_t *)qspi->priv);
    qspi_send_cmd((struct qspi_dev_t *)qspi->priv,
                  (cmd->instruction.disabled ? QSPI_ARG_NOT_USED : cmd->instruction.value),
                  rx_data,
                  size,
                  tx_data,
                  size,
                  (cmd->address.disabled ? QSPI_ARG_NOT_USED : cmd->address.value),
                  (cmd->address.disabled ? QSPI_ARG_NOT_USED : (uint32_t)cmd->address.size),
                  cmd->dummy_count,
                  timeout);
    return size;
}
__CSI_QSPI_DRIVER csi_error_t csi_qspi_send_async(csi_qspi_t *qspi, csi_qspi_command_t *cmd, const void *data, uint32_t size)
{
    (void)qspi;
    (void)cmd;
    (void)data;
    (void)size;
    return CSI_UNSUPPORTED;
}
__CSI_QSPI_DRIVER csi_error_t csi_qspi_receive_async(csi_qspi_t *qspi, csi_qspi_command_t *cmd, void *data, uint32_t size)
{
    (void)qspi;
    (void)cmd;
    (void)data;
    (void)size;
    return CSI_UNSUPPORTED;
}
__CSI_QSPI_DRIVER csi_error_t csi_qspi_send_receive_async(csi_qspi_t *qspi, csi_qspi_command_t *cmd, const void *tx_data, void *rx_data, uint32_t size)
{
    (void)qspi;
    (void)cmd;
    (void)tx_data;
    (void)rx_data;
    (void)size;
    return CSI_UNSUPPORTED;
}
#if(defined CHIP_GC9002)
__CSI_QSPI_DRIVER csi_error_t csi_qspi_link_dma(csi_qspi_t *qspi, csi_dma_ch_t *tx_dma, csi_dma_ch_t *rx_dma)
{
    (void)qspi;
    (void)tx_dma;
    (void)rx_dma;
    return CSI_UNSUPPORTED;
}
#endif
csi_error_t csi_qspi_get_state(csi_qspi_t *qspi, csi_state_t *state)
{
    if(qspi == NULL || state == NULL)
    {
        return CSI_ERROR;
    }
    *state = qspi->state;
    return CSI_OK;
}
__CSI_QSPI_DRIVER static uint32_t qspi_inverse_pow(uint32_t x, uint32_t power)
{
    uint32_t y = 0;
    uint32_t tempval = power;
    do
    {
        tempval /= x;
        y++;
    }while(tempval > 1);
    return y;
}
__CSI_QSPI_DRIVER csi_error_t csi_qspi_direct_mode_config(csi_qspi_t *qspi, csi_qspi_direct_config_t *config, csi_qspi_dir_t dir)
{
    if(qspi == NULL || config == NULL)
    {
        return CSI_ERROR;
    }
    if(qspi->priv == NULL)
    {
        return CSI_ERROR;
    }
    while(qspi_is_idle((struct qspi_dev_t *)qspi->priv) != true);
    qspi_set_spi_mode_base((struct qspi_dev_t *)qspi->priv, 
                           (enum qspi_spi_mode_t)(dir == QSPI_READ ? config->instruction.read_bus_width : config->instruction.write_bus_width),
                           (enum qspi_spi_mode_t)config->address.read_bus_width,
                           (enum qspi_spi_mode_t)config->read_data.bus_width,
                           (enum qspi_spi_mode_t)config->address.write_bus_width,
                           (enum qspi_spi_mode_t)config->write_data.bus_width);
    qspi_cfg_reads((struct qspi_dev_t *)qspi->priv, config->read_data.opcode, config->read_data.dummy_count);       //config read
    qspi_cfg_writes((struct qspi_dev_t *)qspi->priv, config->write_data.opcode, config->write_data.dummy_count);    //config write
    qspi_set_mode_bit((struct qspi_dev_t *)qspi->priv, config->mode_bit.enabled, config->mode_bit.value);           //config mode bit
    
    qspi_cfg_addr_bytes((struct qspi_dev_t *)qspi->priv, (uint32_t)config->address.size);                       //config Number of address bytes
    qspi_cfg_page_size((struct qspi_dev_t *)qspi->priv, config->flash.page_size);                               //config Number of bytes per device page
    qspi_cfg_block_bytes((struct qspi_dev_t *)qspi->priv, qspi_inverse_pow(2, config->flash.block_size));       //config Number of bytes per block
    
    return CSI_OK;
}
__CSI_QSPI_DRIVER csi_error_t csi_qspi_adapted_capture(csi_qspi_t *qspi)
{
    if(qspi == NULL)
    {
        return CSI_ERROR;
    }
    if(qspi->priv == NULL)
    {
        return CSI_ERROR;
    }
    if(qspi_adapted_capture((struct qspi_dev_t *)qspi->priv) != QSPI_ERR_NONE)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
__CSI_QSPI_DRIVER csi_error_t csi_qspi_direct_send(csi_qspi_t *qspi,
                                   csi_qspi_direct_config_t *config,
                                   uint32_t remap_addr,
                                   uint32_t offset_addr,
                                   const void *data,
                                   uint32_t size)
{
    if(qspi == NULL || data == NULL || size == 0)
    {
        return CSI_ERROR;
    }
    if(qspi->priv == NULL)
    {
        return CSI_ERROR;
    }
    csi_error_t ret;
    ret = csi_qspi_direct_mode_config(qspi, config, QSPI_WRITE);
    if(ret != CSI_OK)
    {
        return ret;
    }
    if(config->address.size == QSPI_ADDRESS_32_BITS)
    {
#if(defined CSI_BSP_QSPI_REMAP_TYPE && CSI_BSP_QSPI_REMAP_TYPE == QSPI_REMAP_TYPE_SUB)
        qspi_direct_access_write((struct qspi_dev_t *)qspi->priv, remap_addr-((struct qspi_dev_t *)qspi->priv)->cfg->remap_bus_base, offset_addr, data, size);
#else
        qspi_direct_access_write((struct qspi_dev_t *)qspi->priv, remap_addr, offset_addr, data, size);
#endif
    }
    else
    {
        qspi_direct_access_write((struct qspi_dev_t *)qspi->priv, remap_addr, offset_addr, data, size);
    }
    return CSI_OK;
}
__CSI_QSPI_DRIVER csi_error_t csi_qspi_direct_receive(csi_qspi_t *qspi,
                                      csi_qspi_direct_config_t *config,
                                      uint32_t remap_addr,
                                      uint32_t offset_addr,
                                      void *data,
                                      uint32_t size)
{
    if(qspi == NULL || data == NULL || size == 0)
    {
        return CSI_ERROR;
    }
    if(qspi->priv == NULL)
    {
        return CSI_ERROR;
    }
    csi_error_t ret;
    ret = csi_qspi_direct_mode_config(qspi, config, QSPI_READ);
    if(ret != CSI_OK)
    {
        return ret;
    }
    if(config->address.size == QSPI_ADDRESS_32_BITS)
    {
#if(defined CSI_BSP_QSPI_REMAP_TYPE && CSI_BSP_QSPI_REMAP_TYPE == QSPI_REMAP_TYPE_SUB)
        qspi_direct_access_read((struct qspi_dev_t *)qspi->priv, remap_addr-((struct qspi_dev_t *)qspi->priv)->cfg->remap_bus_base, offset_addr, data, size);
#else
        qspi_direct_access_read((struct qspi_dev_t *)qspi->priv, remap_addr, offset_addr, data, size);
#endif
    }
    else
    {
        qspi_direct_access_read((struct qspi_dev_t *)qspi->priv, remap_addr, offset_addr, data, size);
    }
    return CSI_OK;
}
__CSI_QSPI_DRIVER bool csi_qspi_is_idle(csi_qspi_t *qspi)
{
    return qspi_is_idle((struct qspi_dev_t *)qspi->priv);
}

__CSI_QSPI_DRIVER void csi_qspi_write_protection_config(csi_qspi_t *qspi, uint32_t lower_addr, uint32_t upper_addr, bool enable)
{
    if(qspi == NULL || enable == false)
    {
        return;
    }
    if(qspi->priv == NULL)
    {
        return;
    }
    qspi_write_protection_config((struct qspi_dev_t *)qspi->priv, lower_addr, upper_addr, enable);
}
__CSI_QSPI_DRIVER void csi_qspi_write_protection_ctrl(csi_qspi_t *qspi, bool enable)
{
    if(qspi == NULL)
    {
        return;
    }
    if(qspi->priv == NULL)
    {
        return;
    }
    qspi_write_protection_ctrl((struct qspi_dev_t *)qspi->priv, enable);
}


__CSI_QSPI_DRIVER unsigned int get_qspi_freq_max(void)
{
    unsigned int freq = 0;
    unsigned int pll = csi_rcc_get_pll();
#if(defined CHIP_GC9005)
    if(pll == 1440000000)
    {
        freq = (pll/6/2);
    }
    else if(pll == 1200000000)
    {
        freq = (pll/6/2);
    }
    else if(pll == 960000000)
    {
        freq = (pll/4/2);
    }
    else if(pll == 720000000)
    {
        freq = (pll/4/2);
    }
#elif(defined CHIP_GC9002)
    if(pll == 480000000)
    {
        freq = (pll/2/2);
    }
    else if(pll == 432000000)
    {
        freq = (pll/2/2);
    }
    else if(pll == 288000000)
    {
        freq = (pll/2/2);
    }
    else if(pll == 240000000)
    {
        freq = (pll/1/2);
    }
#endif
    return freq;
}
__CSI_QSPI_DRIVER unsigned int get_qspi_freq_min(void)
{
    unsigned int freq = 0;
    unsigned int pll = csi_rcc_get_pll();
    freq = (pll/CSI_BSP_QSPI_DIV_3/32);
    return freq;
}
#endif
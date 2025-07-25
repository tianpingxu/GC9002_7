#include <csi_iic.h>
#ifdef CONFIG_CSI_DRV_IIC_ENABLED

static I2C_HandleTypeDef handle_iic[CSI_IIC_CNT];

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_I2C0_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_I2C0_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();
    HAL_I2C_IRQHandler(&handle_iic[0]);
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_I2C1_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_I2C1_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();
    HAL_I2C_IRQHandler(&handle_iic[1]);
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

void CSI_HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c != NULL)
    {
        if(hi2c->priv_obj != NULL)
        {
            if(((csi_iic_t *)(hi2c->priv_obj))->callback != NULL)
            {
                ((csi_iic_t *)(hi2c->priv_obj))->callback((csi_iic_t *)(hi2c->priv_obj), IIC_EVENT_SEND_COMPLETE, ((csi_iic_t *)(hi2c->priv_obj))->arg);
            }
        }
    }
}
void CSI_HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c != NULL)
    {
        if(hi2c->priv_obj != NULL)
        {
            if(((csi_iic_t *)(hi2c->priv_obj))->callback != NULL)
            {
                ((csi_iic_t *)(hi2c->priv_obj))->callback((csi_iic_t *)(hi2c->priv_obj), IIC_EVENT_RECEIVE_COMPLETE, ((csi_iic_t *)(hi2c->priv_obj))->arg);
            }
        }
    }
}
void CSI_HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c != NULL)
    {
        if(hi2c->priv_obj != NULL)
        {
            if(((csi_iic_t *)(hi2c->priv_obj))->callback != NULL)
            {
                ((csi_iic_t *)(hi2c->priv_obj))->callback((csi_iic_t *)(hi2c->priv_obj), IIC_EVENT_SEND_COMPLETE, ((csi_iic_t *)(hi2c->priv_obj))->arg);
            }
        }
    }
}
void CSI_HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c != NULL)
    {
        if(hi2c->priv_obj != NULL)
        {
            if(((csi_iic_t *)(hi2c->priv_obj))->callback != NULL)
            {
                ((csi_iic_t *)(hi2c->priv_obj))->callback((csi_iic_t *)(hi2c->priv_obj), IIC_EVENT_RECEIVE_COMPLETE, ((csi_iic_t *)(hi2c->priv_obj))->arg);
            }
        }
    }
}
void CSI_HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c != NULL)
    {
        if(hi2c->priv_obj != NULL)
        {
            if(((csi_iic_t *)(hi2c->priv_obj))->callback != NULL)
            {
                ((csi_iic_t *)(hi2c->priv_obj))->callback((csi_iic_t *)(hi2c->priv_obj), IIC_EVENT_ERROR, ((csi_iic_t *)(hi2c->priv_obj))->arg);
            }
        }
    }
}
csi_error_t csi_iic_init(csi_iic_t *iic, uint32_t idx)
{
    if(iic == NULL || idx >= CSI_IIC_CNT)
    {
        return CSI_ERROR;
    }
    if(idx == 0)
    {
        HANDLE_REG_BASE(iic) = I2C0_BASE;
        HANDLE_IRQ_NUM(iic) = I2C0_IRQn;
        HANDLE_DEV_IDX(iic) = 0;
        HANDLE_IRQ_HANDLER(iic) = (void *)CSI_I2C0_IRQHandler;
    }
    else if(idx == 1)
    {
        HANDLE_REG_BASE(iic) = I2C1_BASE;
        HANDLE_IRQ_NUM(iic) = I2C1_IRQn;
        HANDLE_DEV_IDX(iic) = 1;
        HANDLE_IRQ_HANDLER(iic) = (void *)CSI_I2C1_IRQHandler;
    }
    handle_iic[idx].priv_obj = iic;
    iic->priv = (void *)&handle_iic[idx];
    ((I2C_HandleTypeDef *)iic->priv)->Instance = (I2C_TypeDef *)HANDLE_REG_BASE(iic);
    HAL_I2C_StructInit((I2C_HandleTypeDef *)iic->priv);
    HAL_I2C_Init((I2C_HandleTypeDef *)iic->priv);
    HAL_I2C_RegisterCallback((I2C_HandleTypeDef *)iic->priv, HAL_I2C_MASTER_TX_COMPLETE_CB_ID, CSI_HAL_I2C_MasterTxCpltCallback);
    HAL_I2C_RegisterCallback((I2C_HandleTypeDef *)iic->priv, HAL_I2C_MASTER_RX_COMPLETE_CB_ID, CSI_HAL_I2C_MasterRxCpltCallback);
    HAL_I2C_RegisterCallback((I2C_HandleTypeDef *)iic->priv, HAL_I2C_SLAVE_TX_COMPLETE_CB_ID, CSI_HAL_I2C_SlaveTxCpltCallback);
    HAL_I2C_RegisterCallback((I2C_HandleTypeDef *)iic->priv, HAL_I2C_SLAVE_RX_COMPLETE_CB_ID, CSI_HAL_I2C_SlaveRxCpltCallback);
    HAL_I2C_RegisterCallback((I2C_HandleTypeDef *)iic->priv, HAL_I2C_ERROR_CB_ID, CSI_HAL_I2C_ErrorCallback);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(HANDLE_IRQ_NUM(iic), CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, HANDLE_IRQ_HANDLER(iic));
#else
    csi_clic_register_irq(HANDLE_IRQ_NUM(iic), CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, HANDLE_IRQ_HANDLER(iic));
#endif
    return CSI_OK;
}
void csi_iic_uninit(csi_iic_t *iic)
{
    if(iic == NULL)
    {
        return;
    }
    HANDLE_REG_BASE(iic) = 0;
    HANDLE_IRQ_NUM(iic) = 0;
    HANDLE_DEV_IDX(iic) = 0;
    HANDLE_IRQ_HANDLER(iic) = NULL;
    iic->priv = NULL;
}
csi_error_t csi_iic_mode(csi_iic_t *iic, csi_iic_mode_t mode)
{
    if(iic == NULL)
    {
        return CSI_ERROR;
    }
    if(iic->priv == NULL)
    {
        return CSI_ERROR;
    }
    iic->mode = mode;
    HAL_I2C_BusMode_Select((I2C_HandleTypeDef *)iic->priv, (iic->mode == IIC_MODE_MASTER ? I2C_Mode_Master : I2C_Mode_Slave));
    return CSI_OK;
}
csi_error_t csi_iic_addr_mode(csi_iic_t *iic, csi_iic_addr_mode_t addr_mode)
{
    if(iic == NULL)
    {
        return CSI_ERROR;
    }
    if(iic->priv == NULL)
    {
        return CSI_ERROR;
    }
    HAL_I2C_AddrMode_Select((I2C_HandleTypeDef *)iic->priv, (addr_mode == IIC_ADDRESS_7BIT ? I2C_AddrMode_7BIT : I2C_AddrMode_10BIT));
    return CSI_OK;
}
csi_error_t csi_iic_speed(csi_iic_t *iic, csi_iic_speed_t speed)
{
    if(iic == NULL)
    {
        return CSI_ERROR;
    }
    if(iic->priv == NULL)
    {
        return CSI_ERROR;
    }
    switch(speed)
    {
        case IIC_BUS_SPEED_STANDARD:
            ((I2C_HandleTypeDef *)iic->priv)->Init.Speed = I2C_Speed_Standard;
        break;
        case IIC_BUS_SPEED_FAST:
            ((I2C_HandleTypeDef *)iic->priv)->Init.Speed = I2C_Speed_Fast;
        break;
        case IIC_BUS_SPEED_FAST_PLUS:
            ((I2C_HandleTypeDef *)iic->priv)->Init.Speed = I2C_Speed_FastPlus;
        break;
        case IIC_BUS_SPEED_HIGH:
            ((I2C_HandleTypeDef *)iic->priv)->Init.Speed = I2C_Speed_High;
        break;
        default: return CSI_ERROR;
    }
    HAL_I2C_Init((I2C_HandleTypeDef *)iic->priv);
    return CSI_OK;
}
csi_error_t csi_iic_own_addr(csi_iic_t *iic, uint32_t own_addr)
{
    if(iic == NULL)
    {
        return CSI_ERROR;
    }
    if(iic->priv == NULL)
    {
        return CSI_ERROR;
    }
    ((I2C_HandleTypeDef *)iic->priv)->Init.OwnAddress1 = own_addr;
    HAL_I2C_Init((I2C_HandleTypeDef *)iic->priv);
    return CSI_OK;
}
csi_error_t csi_iic_attach_callback(csi_iic_t *iic, void(*callback)(csi_iic_t *iic, csi_iic_event_t event, void *arg), void *arg)
{
    if(iic == NULL)
    {
        return CSI_ERROR;
    }
    iic->callback = callback;
    iic->arg = arg;
    return CSI_OK;
}
void csi_iic_detach_callback(csi_iic_t *iic)
{
    if(iic == NULL)
    {
        return;
    }
    iic->callback = NULL;
    iic->arg = NULL;
}
csi_error_t csi_iic_link_dma(csi_iic_t *iic, csi_dma_ch_t *tx_dma, csi_dma_ch_t *rx_dma)
{
    if(iic == NULL)
    {
        return CSI_ERROR;
    }
    if(iic->priv == NULL)
    {
        return CSI_ERROR;
    }
    iic->tx_dma = tx_dma;
    iic->rx_dma = rx_dma;
    return CSI_OK;
}
int32_t csi_iic_master_send(csi_iic_t *iic, uint32_t devaddr,const void *data, uint32_t size, uint32_t timeout)
{
    if(iic == NULL || data == NULL || size == 0)
    {
        return 0;
    }
    if(iic->priv == NULL)
    {
        return 0;
    }
    if(HAL_I2C_Master_Transmit((I2C_HandleTypeDef *)iic->priv, devaddr, (uint8_t *)data, size, timeout) != HAL_OK)
    {
        return 0;
    }
    return size;
}
csi_error_t csi_iic_master_send_async(csi_iic_t *iic, uint32_t devaddr, void *data, uint32_t size)
{
    if(iic == NULL || data == NULL || size == 0)
    {
        return CSI_ERROR;
    }
    if(iic->priv == NULL)
    {
        return CSI_ERROR;
    }
    if(HAL_I2C_Master_Transmit_IT((I2C_HandleTypeDef *)iic->priv, devaddr, (uint8_t *)data, size) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
int32_t csi_iic_master_receive(csi_iic_t *iic, uint32_t devaddr, void *data, uint32_t size, uint32_t timeout)
{
    if(iic == NULL || data == NULL || size == 0)
    {
        return 0;
    }
    if(iic->priv == NULL)
    {
        return 0;
    }
    if(HAL_I2C_Master_Receive((I2C_HandleTypeDef *)iic->priv, devaddr, (uint8_t *)data, size, timeout) != HAL_OK)
    {
        return 0;
    }
    return (int32_t)size;
}
csi_error_t csi_iic_master_receive_async(csi_iic_t *iic, uint32_t devaddr, void *data, uint32_t size)
{
    if(iic == NULL || data == NULL || size == 0)
    {
        return CSI_ERROR;
    }
    if(iic->priv == NULL)
    {
        return CSI_ERROR;
    }
    if(HAL_I2C_Master_Receive_IT((I2C_HandleTypeDef *)iic->priv, devaddr, (uint8_t *)data, size) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
int32_t     csi_iic_mem_send(csi_iic_t *iic, 
                             uint32_t devaddr,
                             uint16_t memaddr, 
                             csi_iic_mem_addr_size_t memaddr_size,
                             const void *data,
                             uint32_t size,
                             uint32_t timeout)
{
    if(iic == NULL || data == NULL || size == 0)
    {
        return CSI_ERROR;
    }
    if(iic->priv == NULL)
    {
        return CSI_ERROR;
    }
    if(HAL_I2C_Mem_Write((I2C_HandleTypeDef *)iic->priv, 
                         devaddr, 
                         memaddr, 
                         (memaddr_size == IIC_MEM_ADDR_SIZE_8BIT ? I2C_MEMADD_SIZE_8BIT : I2C_MEMADD_SIZE_16BIT), 
                         (uint8_t *)data, 
                         size, 
                         timeout) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return 0;
}
int32_t     csi_iic_mem_receive(csi_iic_t *iic,
                                uint32_t devaddr,
                                uint16_t memaddr,
                                csi_iic_mem_addr_size_t memaddr_size,
                                void *data,
                                uint32_t size,
                                uint32_t timeout)
{
    if(iic == NULL || data == NULL || size == 0)
    {
        return CSI_ERROR;
    }
    if(iic->priv == NULL)
    {
        return CSI_ERROR;
    }
    if(HAL_I2C_Mem_Read((I2C_HandleTypeDef *)iic->priv, 
                         devaddr, 
                         memaddr, 
                         (memaddr_size == IIC_MEM_ADDR_SIZE_8BIT ? I2C_MEMADD_SIZE_8BIT : I2C_MEMADD_SIZE_16BIT), 
                         (uint8_t *)data, 
                         size, 
                         timeout) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return 0;
}
int32_t csi_iic_slave_send(csi_iic_t *iic, const void *data, uint32_t size, uint32_t timeout)
{
    if(iic == NULL || data == NULL || size == 0)
    {
        return 0;
    }
    if(iic->priv == NULL)
    {
        return 0;
    }
    if(HAL_I2C_Slave_Transmit((I2C_HandleTypeDef *)iic->priv, (uint8_t *)data, size, timeout) != HAL_OK)
    {
        return 0;
    }
    return size;
}
csi_error_t csi_iic_slave_send_async(csi_iic_t *iic, void *data, uint32_t size)
{
    if(iic == NULL || data == NULL || size == 0)
    {
        return CSI_ERROR;
    }
    if(iic->priv == NULL)
    {
        return CSI_ERROR;
    }
    if(HAL_I2C_Slave_Transmit_IT((I2C_HandleTypeDef *)iic->priv, (uint8_t *)data, size) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
int32_t csi_iic_slave_receive(csi_iic_t *iic, void *data, uint32_t size, uint32_t timeout)
{
    if(iic == NULL || data == NULL || size == 0)
    {
        return 0;
    }
    if(iic->priv == NULL)
    {
        return 0;
    }
    if(HAL_I2C_Slave_Receive((I2C_HandleTypeDef *)iic->priv, (uint8_t *)data, size, timeout) != HAL_OK)
    {
        return 0;
    }
    return (int32_t)size;
}
csi_error_t csi_iic_slave_receive_async(csi_iic_t *iic, void *data, uint32_t size)
{
    if(iic == NULL || data == NULL || size == 0)
    {
        return CSI_ERROR;
    }
    if(iic->priv == NULL)
    {
        return CSI_ERROR;
    }
    if(HAL_I2C_Slave_Receive_IT((I2C_HandleTypeDef *)iic->priv, (uint8_t *)data, size) != HAL_OK)
    {
        return CSI_ERROR;
    }
    return CSI_OK;
}
csi_error_t csi_iic_xfer_pending(csi_iic_t *iic, bool enable)
{
    return CSI_UNSUPPORTED;
}
csi_error_t csi_iic_get_state(csi_iic_t *iic, csi_state_t *state)
{
    return CSI_UNSUPPORTED;
}





#endif /* CONFIG_CSI_DRV_IIC_ENABLED */
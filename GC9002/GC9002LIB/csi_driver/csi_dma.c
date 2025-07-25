#include "csi_dma.h"
#ifdef CONFIG_CSI_DRV_DMA_ENABLED

static DMA_HandleTypeDef hdma_tab[CSI_DMA_CH_NUM];
static csi_dma_ch_t s_ch_list_tab[CSI_DMA_CH_NUM];
static volatile uint32_t dma_ch_alloc_status = 0;
static volatile uint8_t dma_init_status = 0;

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CSI_DMA_IRQHandler(void)
#else
static ATTRIBUTE_ISR void CSI_DMA_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();
    
    for(int i = 0; i < CSI_DMA_CH_NUM; i++)
    {
        if(hdma_tab[i].Instance != NULL)
        {
            HAL_DMA_IRQHandler(&hdma_tab[i]);
        }
    }
    
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
static void CSI_HAL_DMA_XferCpltCallback(DMA_HandleTypeDef *hdma)
{
    if(hdma->ChObj != NULL)
    {
        if(((csi_dma_ch_t *)hdma->ChObj)->cb != NULL)
        {
            ((csi_dma_ch_t *)hdma->ChObj)->cb(DMA_EVENT_TRANSFER_DONE, ((csi_dma_ch_t *)hdma->ChObj)->arg);
        }
    }
}
static void CSI_HAL_DMA_BlockCpltCallback(DMA_HandleTypeDef *hdma)
{
    (void)hdma;
}
static void CSI_HAL_DMA_SourceCpltCallback(DMA_HandleTypeDef *hdma)
{
    (void)hdma;
}
static void CSI_HAL_DMA_DestinationCpltCallback(DMA_HandleTypeDef *hdma)
{
    (void)hdma;
}
static void CSI_HAL_DMA_XferErrorCallback(DMA_HandleTypeDef *hdma)
{
    if(hdma->ChObj != NULL)
    {
        if(((csi_dma_ch_t *)hdma->ChObj)->cb != NULL)
        {
            ((csi_dma_ch_t *)hdma->ChObj)->cb(DMA_EVENT_TRANSFER_ERROR, ((csi_dma_ch_t *)hdma->ChObj)->arg);
        }
    }
}
static void CSI_HAL_DMA_XferAbortCallback(DMA_HandleTypeDef *hdma)
{
    (void)hdma;
}

csi_error_t csi_dma_ctrl_init(csi_dma_ctrl_t *dma, int8_t ctrl_id)
{
    if(dma == NULL || ctrl_id >= CSI_DMA_CNT)
    {
        return CSI_ERROR;
    }
    if((dma_init_status & (0x1 << ctrl_id)) != 0)
    {
        return CSI_OK;
    }
    if(ctrl_id == 0)
    {
        HANDLE_REG_BASE(dma) = DMA_BASE;
        HANDLE_IRQ_NUM(dma) = DMA_IRQn;
        HANDLE_DEV_IDX(dma) = ctrl_id;
        HANDLE_IRQ_HANDLER(dma) = (void *)CSI_DMA_IRQHandler;
    }
    
    memset((void *)hdma_tab, 0, sizeof(DMA_HandleTypeDef)*CSI_DMA_CH_NUM);
    dma_init_status |= (0x1 << ctrl_id);
    dma->ch_list = NULL;
    dma->alloc_status = dma_ch_alloc_status;
    dma->ch_num = CSI_DMA_CH_NUM;
    dma->priv = NULL;
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(HANDLE_IRQ_NUM(dma), CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, HANDLE_IRQ_HANDLER(dma));
#else
    csi_clic_register_irq(HANDLE_IRQ_NUM(dma), CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, HANDLE_IRQ_HANDLER(dma));
#endif
    return CSI_OK;
}
void csi_dma_ctrl_uninit(csi_dma_ctrl_t *dma)
{
    if(dma == NULL)
    {
        return;
    }
    if(HANDLE_DEV_IDX(dma) >= CSI_DMA_CNT)
    {
        return;
    }
    dma_init_status &= ~(0x1 << HANDLE_DEV_IDX(dma));
}
csi_error_t csi_dma_ch_alloc(csi_dma_ch_t *dma, int8_t ch_id, int8_t ctrl_id)
{
    if(dma == NULL || ctrl_id != 0)
    {
        return CSI_ERROR;
    }
    if(ch_id >= 0 || ch_id < CSI_DMA_CH_NUM)
    {
        if(((dma_ch_alloc_status >> ch_id) & 0x1) == 0)
        {
            dma_ch_alloc_status |= (0x1 << ch_id);
            hdma_tab[ch_id].ChObj = (void *)dma;
            hdma_tab[ch_id].Instance = DMA;
            dma->priv = (void *)&hdma_tab[ch_id];
            dma->ctrl_id = 0;
            dma->ch_id = ch_id;
            dma->cb = 0;
            dma->arg = 0;
            return CSI_OK;
        }
    }
    else
    {
        for(int i = 0; i < CSI_DMA_CH_NUM; i++)
        {
            if(((dma_ch_alloc_status >> i) & 0x1) == 0)
            {
                dma_ch_alloc_status |= (0x1 << i);
                hdma_tab[i].ChObj = (void *)dma;
                hdma_tab[i].Instance = DMA;
                dma->priv = (void *)&hdma_tab[i];
                dma->ctrl_id = 0;
                dma->ch_id = i;
                dma->cb = 0;
                dma->arg = 0;
                return CSI_OK;
            }
        }
    }
    return CSI_ERROR;
}
void csi_dma_ch_free(csi_dma_ch_t *dma)
{
    if(dma == NULL)
    {
        return;
    }
    if(dma->ch_id <0 || dma->ch_id >= CSI_DMA_CH_NUM)
    {
        return;
    }
    dma_ch_alloc_status &= ~(0x1 << dma->ch_id);
    hdma_tab[dma->ch_id].Instance = NULL;
    memset((void *)dma, 0, sizeof(csi_dma_ch_t));
}
csi_error_t csi_dma_ch_config(csi_dma_ch_t *dma, csi_dma_ch_config_t *config, csi_dma_cb_t cb, void *arg)
{
    if(dma == NULL || config == NULL)
    {
        return  CSI_ERROR;
    }
    if(dma->priv == NULL || dma->ch_id < 0 || dma->ch_id > CSI_DMA_CH_NUM)
    {
        return  CSI_ERROR;
    }
    switch(config->src_inc)//DMA源地址增长方式
    {
        case DMA_ADDR_INC:
            ((DMA_HandleTypeDef *)dma->priv)->Init.mS_AddrInc = DMA_INCREMENTAL;
        break;
        case DMA_ADDR_DEC:
            ((DMA_HandleTypeDef *)dma->priv)->Init.mS_AddrInc = DMA_DECREMENTS;
        break;
        case DMA_ADDR_CONSTANT:
            ((DMA_HandleTypeDef *)dma->priv)->Init.mS_AddrInc = DMA_FIXED;
        break;
        default: return CSI_ERROR;
    }
    switch(config->dst_inc)//DMA目的地址增长方式
    {
        case DMA_ADDR_INC:
            ((DMA_HandleTypeDef *)dma->priv)->Init.mD_AddrInc = DMA_INCREMENTAL;
        break;
        case DMA_ADDR_DEC:
            ((DMA_HandleTypeDef *)dma->priv)->Init.mD_AddrInc = DMA_DECREMENTS;
        break;
        case DMA_ADDR_CONSTANT:
            ((DMA_HandleTypeDef *)dma->priv)->Init.mD_AddrInc = DMA_FIXED;
        break;
        default: return CSI_ERROR;
    }
    switch(config->src_tw)//DMA源数据宽度
    {
        case DMA_DATA_WIDTH_8_BITS:
            ((DMA_HandleTypeDef *)dma->priv)->Init.mS_TransferWidth = DMA_TR_WIDTH_8bits;
        break;
        case DMA_DATA_WIDTH_16_BITS:
            ((DMA_HandleTypeDef *)dma->priv)->Init.mS_TransferWidth = DMA_TR_WIDTH_16bits;
        break;
        case DMA_DATA_WIDTH_32_BITS:
            ((DMA_HandleTypeDef *)dma->priv)->Init.mS_TransferWidth = DMA_TR_WIDTH_32bits;
        break;
        default: return CSI_ERROR;
    }
    switch(config->dst_tw)//DMA目的数据宽度
    {
        case DMA_DATA_WIDTH_8_BITS:
            ((DMA_HandleTypeDef *)dma->priv)->Init.mD_TransferWidth = DMA_TR_WIDTH_8bits;
        break;
        case DMA_DATA_WIDTH_16_BITS:
            ((DMA_HandleTypeDef *)dma->priv)->Init.mD_TransferWidth = DMA_TR_WIDTH_16bits;
        break;
        case DMA_DATA_WIDTH_32_BITS:
            ((DMA_HandleTypeDef *)dma->priv)->Init.mD_TransferWidth = DMA_TR_WIDTH_32bits;
        break;
        default: return CSI_ERROR;
    }
    switch(config->dw.trans_dir)//传输数据方向配置
    {
        case DMA_MEM2MEM:
            ((DMA_HandleTypeDef *)dma->priv)->Init.mTransferType_FlowControl = DMA_TT_MEM_TO_MEM_DMAC;
        break;
        case DMA_MEM2PERH:
            ((DMA_HandleTypeDef *)dma->priv)->Init.mTransferType_FlowControl = DMA_TT_MEM_TO_PER_DMAC;
        break;
        case DMA_PERH2MEM:
            ((DMA_HandleTypeDef *)dma->priv)->Init.mTransferType_FlowControl = DMA_TT_PER_TO_MEM_DMAC;
        break;
        case DMA_PERH2PERH:
            ((DMA_HandleTypeDef *)dma->priv)->Init.mTransferType_FlowControl = DMA_TT_PER_TO_PER_DMAC;
        break;
        default: return CSI_ERROR;
    }
    dma->cb = cb;
    dma->arg = arg;
    ((DMA_HandleTypeDef *)dma->priv)->Init.mChannel = dma->ch_id;
    ((DMA_HandleTypeDef *)dma->priv)->Init.mS_Peripheral = config->dw.src_hs_if;
    ((DMA_HandleTypeDef *)dma->priv)->Init.mD_Peripheral = config->dw.dst_hs_if;
    ((DMA_HandleTypeDef *)dma->priv)->Init.mS_AutoReload_EN = (config->dw.src_reload_en ? DMA_ENABLE : DMA_DISABLE);
    ((DMA_HandleTypeDef *)dma->priv)->Init.mD_AutoReload_EN = (config->dw.dst_reload_en ? DMA_ENABLE : DMA_DISABLE);
    
    ((DMA_HandleTypeDef *)dma->priv)->Init.mS_BurstTransactionLen = (enum DMA_BURST_MSIZE_Enum)config->dw.src_burst_len;
    ((DMA_HandleTypeDef *)dma->priv)->Init.mD_BurstTransactionLen = (enum DMA_BURST_MSIZE_Enum)config->dw.dst_burst_len;
    ((DMA_HandleTypeDef *)dma->priv)->Init.mChannelPriority = dma->ch_id;
    ((DMA_HandleTypeDef *)dma->priv)->Init.mEnable = DMA_DISABLE;
    HAL_DMA_Init((DMA_HandleTypeDef *)dma->priv);
    HAL_DMA_RegisterCallback((DMA_HandleTypeDef *)dma->priv, HAL_DMA_XFER_CPLT_CB_ID, CSI_HAL_DMA_XferCpltCallback);
    HAL_DMA_RegisterCallback((DMA_HandleTypeDef *)dma->priv, HAL_DMA_XFER_BLOCK_CB_ID, CSI_HAL_DMA_BlockCpltCallback);
    HAL_DMA_RegisterCallback((DMA_HandleTypeDef *)dma->priv, HAL_DMA_XFER_SRC_CB_ID, CSI_HAL_DMA_SourceCpltCallback);
    HAL_DMA_RegisterCallback((DMA_HandleTypeDef *)dma->priv, HAL_DMA_XFER_DST_CB_ID, CSI_HAL_DMA_DestinationCpltCallback);
    HAL_DMA_RegisterCallback((DMA_HandleTypeDef *)dma->priv, HAL_DMA_XFER_ERROR_CB_ID, CSI_HAL_DMA_XferErrorCallback);
    HAL_DMA_RegisterCallback((DMA_HandleTypeDef *)dma->priv, HAL_DMA_XFER_ABORT_CB_ID, CSI_HAL_DMA_XferAbortCallback);
    return CSI_OK;
}

void csi_dma_ch_start(csi_dma_ch_t *dma, void *srcaddr, void *dstaddr, uint32_t length)
{
    if(dma == NULL)
    {
        return;
    }
    if(dma->priv == NULL || dma->ch_id < 0 || dma->ch_id > CSI_DMA_CH_NUM)
    {
        return;
    }
    HAL_DMA_Start_IT((DMA_HandleTypeDef *)dma->priv, (uint32_t)srcaddr, (uint32_t)dstaddr, length);
}
void csi_dma_ch_stop(csi_dma_ch_t *dma)
{
    if(dma == NULL)
    {
        return;
    }
    if(dma->priv == NULL || dma->ch_id < 0 || dma->ch_id > CSI_DMA_CH_NUM)
    {
        return;
    }
    HAL_DMA_Abort_IT((DMA_HandleTypeDef *)dma->priv);
}

uint32_t csi_dma_ch_get_counter(csi_dma_ch_t *dma)
{
    return __HAL_DMA_CHX_GET_COUNTER((DMA_HandleTypeDef *)dma->priv);
}
#endif /* CONFIG_CSI_DRV_DMA_ENABLED */
#include "sample_def.h"
#include "csi_dma.h"
#include "csi_irq.h"

#if(BSP_DEBUG == TEST_SAMPLE_CSI)
#ifdef  _SAMPLE_CSI_DMA_


#define CHECK_RETURN(ret)                           \
        do {                                        \
            if (ret != CSI_OK) {                    \
                while(1);                           \
            }                                       \
        } while(0);

static csi_dma_ctrl_t dma_ctrl;
static csi_dma_ch_t dma_ch;

void csi_dma_callback(csi_dma_event_t event, void *arg)
{
    printf("dma tx success\n");
}
uint8_t *tx_data = NULL;
uint8_t *rx_data = NULL;

int main(void)
{
    csi_error_t ret;
    csi_dma_ch_config_t config;
    
    tx_data = (uint8_t *)0x02000000;
    rx_data = (uint8_t *)0x02001000;
    for(int i = 0; i < 0x0400; i++)
    {
        tx_data[i] = i;
        rx_data[i] = 0;
    }
    
    ret = csi_dma_ctrl_init(&dma_ctrl, 0);
    CHECK_RETURN(ret);
    ret = csi_dma_ch_alloc(&dma_ch, 0, 0);
    CHECK_RETURN(ret);
    
    config.src_inc = DMA_ADDR_INC;
    config.dst_inc = DMA_ADDR_INC;
    config.src_tw = DMA_DATA_WIDTH_32_BITS;
    config.dst_tw = DMA_DATA_WIDTH_32_BITS;
    config.dw.src_hs_if = DMA_HS_MEMORY;
    config.dw.dst_hs_if = DMA_HS_MEMORY;
    config.dw.src_reload_en = false;
    config.dw.dst_reload_en = false;
    config.dw.src_burst_len = 0;
    config.dw.dst_burst_len = 0;
    config.dw.trans_dir = DMA_MEM2MEM;
    
    ret = csi_dma_ch_config(&dma_ch, &config, csi_dma_callback, NULL);
    CHECK_RETURN(ret);
    csi_dma_ch_start(&dma_ch, tx_data, rx_data, DMA_CHANNEL_1_BLOCK_TS_MAX*4);
    while(1)
    {
		mdelay(1000);
    }
    return 0;
}

#endif /* _SAMPLE_CSI_DMA_ */
#endif /* BSP_DEBUG */
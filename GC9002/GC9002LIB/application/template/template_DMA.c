#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx.h"
#include "core_rv32.h"
#include "template_def.h"
#include "csi_irq.h"

#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_DMA_
    
uint32_t get(uint32_t addr) {
    volatile uint32_t * ptr = (volatile uint32_t *)(addr);
    return *ptr;
}

void set(uint32_t addr, uint32_t value) {
    volatile uint32_t * ptr = (volatile uint32_t *)(addr);
    *ptr = value;
}
void dma_send(uint8_t ch, uint32_t src_addr, uint32_t dst_addr, uint32_t len) {
    // CONFIG START
    uint32_t src_dev_type = 1; // 0:hw 1:sw
    uint32_t dst_dev_type = 0; // 0:hw 1:sw
    uint32_t src_addr_incr_mode = 0; // 0:incr, 1:decr, 2:fix
    uint32_t dst_addr_incr_mode = 2;
    uint32_t src_hs = 0;
    uint32_t dst_hs = 0;
    // CONFIG END
    uint32_t src_burst_type = 0;
    uint32_t dst_burst_type = 0;
    uint32_t src_width = 0;
    uint32_t dst_width = 0;
    uint32_t length = 0;
    switch(len & 0x3) {
        case 0: src_width = 2; length = len >> 2; break;
        case 1: src_width = 0; length = len; break;
        case 2: src_width = 1; length = len >> 1; break;
        case 3: src_width = 0; length = len; break;
    }

    set(0x50200398, 1); // enable dma
    set(0x50200000 + 0x58 * ch, src_addr); // src addr
    set(0x50200008 + 0x58 * ch, dst_addr); // dst addr
    set(0x50200040 + 0x58 * ch, (src_dev_type << 11) | (dst_dev_type << 10)); // src - sw, dst - hw
    set(0x50200044 + 0x58 * ch, 0x1c | (dst_hs << 11) | (src_hs << 7) ); // cfg
    set(0x50200018 + 0x58 * ch, 0x00100001 | (src_burst_type << 14) | (dst_burst_type << 11) | (src_addr_incr_mode << 9) | (dst_addr_incr_mode << 7) | (src_width << 4) | (dst_width<<1));
    set(0x5020001c + 0x58 * ch, length);
    uint32_t regvalue;
    regvalue = get(0x502003a0);
    set(0x502003a0, regvalue | (0x101 << ch)); // enable channel
}

void dma_recv(uint8_t ch, uint32_t src_addr, uint32_t dst_addr, uint32_t len) {
    // CONFIG START
    uint32_t src_dev_type = 0; // 0:hw 1:sw
    uint32_t dst_dev_type = 1; // 0:hw 1:sw
    uint32_t src_addr_incr_mode = 2; // 0:incr, 1:decr, 2:fix
    uint32_t dst_addr_incr_mode = 0;
    uint32_t src_hs = 3;
    uint32_t dst_hs = 0;
    // CONFIG END
    uint32_t src_burst_type = 0;
    uint32_t dst_burst_type = 0;
    uint32_t src_width = 0;
    uint32_t dst_width = 0;
    uint32_t length = len;
    switch(len & 0x3) {
        case 0: dst_width = 2; break;
        case 1: dst_width = 0; break;
        case 2: dst_width = 1; break;
        case 3: dst_width = 0; break;
    }

    set(0x50200398, 1); // enable dma
    set(0x50200000 + 0x58 * ch, src_addr); // src addr
    set(0x50200008 + 0x58 * ch, dst_addr); // dst addr
    set(0x50200040 + 0x58 * ch, (src_dev_type << 11) | (dst_dev_type << 10)); // src - sw, dst - hw
    set(0x50200044 + 0x58 * ch, 0x1c | (dst_hs << 11) | (src_hs << 7) ); // cfg
    set(0x50200018 + 0x58 * ch, 0x00200001 | (src_burst_type << 14) | (dst_burst_type << 11) | (src_addr_incr_mode << 9) | (dst_addr_incr_mode << 7) | (src_width << 4) | (dst_width<<1));
    set(0x5020001c + 0x58 * ch, length);
    uint32_t regvalue;
    regvalue = get(0x502003a0);
    set(0x502003a0, regvalue | (0x101 << ch)); // enable channel
}

void dma_memcpy(uint8_t ch, uint32_t src_addr, uint32_t dst_addr, uint32_t len) {
    // CONFIG START
    uint32_t src_dev_type = 1; // 0:hw 1:sw
    uint32_t dst_dev_type = 1; // 0:hw 1:sw
    uint32_t src_addr_incr_mode = 0; // 0:incr, 1:decr, 2:fix
    uint32_t dst_addr_incr_mode = 0;
    uint32_t src_hs = 0;
    uint32_t dst_hs = 0;
    // CONFIG END
    uint32_t src_burst_type = 0;
    uint32_t dst_burst_type = 0;
    uint32_t src_width = 0;
    uint32_t dst_width = 0;
    uint32_t length = 0;
    switch(len & 0x3) {
        case 0: src_width = 2; length = len >> 2; break;
        case 1: src_width = 0; length = len; break;
        case 2: src_width = 1; length = len >> 1; break;
        case 3: src_width = 0; length = len; break;
    }
    dst_width = src_width;

    set(0x50200398, 1); // enable dma
    set(0x50200000 + 0x58 * ch, src_addr); // src addr
    set(0x50200008 + 0x58 * ch, dst_addr); // dst addr
    set(0x50200040 + 0x58 * ch, (src_dev_type << 11) | (dst_dev_type << 10));
    set(0x50200044 + 0x58 * ch, 0x1c | (dst_hs << 11) | (src_hs << 7) ); // cfg
    set(0x50200018 + 0x58 * ch, 0x1 | (src_burst_type << 14) | (dst_burst_type << 11) | (src_addr_incr_mode << 9) | (dst_addr_incr_mode << 7) | (src_width << 4) | (dst_width<<1));
    set(0x5020001c + 0x58 * ch, length);
}

void dma_start(uint8_t ch) {
    uint32_t regvalue;
    regvalue = get(0x502003a0);
    set(0x502003a0, regvalue | (0x101 << ch)); // enable channel
}

void dma_wait_done(uint8_t ch) {
    uint32_t status;
    do {
      status = get(0x502003a0);
    } while((status & (0x1 << ch)) != 0);
}

static DMA_HandleTypeDef tx_hdma =
{
    .Instance = DMA,
};

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_DMA_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_DMA_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

	HAL_DMA_IRQHandler(&tx_hdma);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

static void TEST_HAL_DMA_XferCpltCallback(DMA_HandleTypeDef * hdma)
{
    printf("dma tx success\n");
}
#define TEST_DMA_BUFF_LENGTH    64
__attribute__ ((aligned(64))) uint8_t tx_data[TEST_DMA_BUFF_LENGTH];
__attribute__ ((aligned(64))) uint8_t rx_data[TEST_DMA_BUFF_LENGTH];
extern uint8_t hyper_capture[32];
extern uint32_t ldo_capture[8];
static WDT_HandleTypeDef hwdg = {.Instance = WDT};
#define TEST_DMA_HYPER  1
#define TEST_HYPER_ADDR_START	            0x20000000
#define TEST_HYPER_ADDR_END		            0x20400000

int main(void)
{
    printf("hyper test:");
    // Hyper Configation
    HYPER_HandleTypeDef hhyper;
    hhyper.Instance = HYPER;
    hhyper.Pres = CONFIG_HYPER_DIV;
    hhyper.MapAHBAddrBase = HYPER_DATA_BASE;
    HAL_HYPER_Init(&hhyper);
	ccm_EnableDCache();
    
    for(int i = 0; i < TEST_DMA_BUFF_LENGTH; i++)
    {
        tx_data[i] = i % 0xFF;
        rx_data[i] = 0;
    }
    uint32_t result = 0;
	unsigned int tempreg = 0;
    volatile unsigned int *ptr;
    uint32_t value = 0;
#if(1)
    #if(TEST_DMA_HYPER)
	for(unsigned int addr = TEST_HYPER_ADDR_START; addr < TEST_HYPER_ADDR_END; addr += TEST_DMA_BUFF_LENGTH)
    {
        memset(rx_data, 0, TEST_DMA_BUFF_LENGTH);
        dma_memcpy(0, (uint32_t)tx_data, addr, TEST_DMA_BUFF_LENGTH);
        dma_start(0);
        dma_wait_done(0);
        dma_memcpy(0, addr, (uint32_t)rx_data, TEST_DMA_BUFF_LENGTH);
        dma_start(0);
        dma_wait_done(0);
        result = memcmp(tx_data, rx_data, TEST_DMA_BUFF_LENGTH);
    }
    #else
	for(unsigned int addr = TEST_HYPER_ADDR_START; addr < TEST_HYPER_ADDR_END; addr += 4)
	{
		ptr = (volatile unsigned int *)addr;
		*ptr = addr;
	}
	for(unsigned int addr = TEST_HYPER_ADDR_START; addr < TEST_HYPER_ADDR_END; addr += 4)
	{
		ptr = (volatile unsigned int *)addr;
		tempreg = *ptr;
		if(tempreg != addr)
		{
            result = (uint32_t)ptr;
//            #if(TEST_DEBUG_PRINT)
            printf("hyper ng addr:0x%08X\n", result);
//            #endif
            break;
		}
	}
    #endif
#else
    HAL_DMA_StructInit(&(tx_hdma.Init));
    
    tx_hdma.Init.mChannel = DMA_CHANNEL1;
    tx_hdma.Init.mS_Address = 0x0;/*!< Specifies Source Address.             */
    tx_hdma.Init.mD_Address = 0x0;/*!< Specifies Destination Address.        */
    tx_hdma.Init.mBlockSize = 0;/*!<[1 ~ 1024]                             */
    tx_hdma.Init.mS_AutoReload_EN = DMA_DISABLE;
    tx_hdma.Init.mD_AutoReload_EN = DMA_DISABLE;
    tx_hdma.Init.mS_TransferWidth = DMA_TR_WIDTH_8bits;
    tx_hdma.Init.mD_TransferWidth = DMA_TR_WIDTH_8bits;
    tx_hdma.Init.mS_BurstTransactionLen = DMA_DATA_ITEM_1;
    tx_hdma.Init.mD_BurstTransactionLen = DMA_DATA_ITEM_1;
    tx_hdma.Init.mS_AddrInc = DMA_INCREMENTAL;
    tx_hdma.Init.mD_AddrInc = DMA_INCREMENTAL;
    tx_hdma.Init.mChannelPriority = DMA_MIN_CHANNEL_PRIORITY;
    tx_hdma.Init.mTransferType_FlowControl = DMA_TT_MEM_TO_MEM_DMAC;
    tx_hdma.Init.mS_Peripheral = DMA_HS_MEMORY;
    tx_hdma.Init.mD_Peripheral = DMA_HS_MEMORY;
    tx_hdma.Init.mEnable = DMA_DISABLE;
    HAL_DMA_Init(&tx_hdma);
    HAL_DMA_RegisterCallback(&tx_hdma, HAL_DMA_XFER_CPLT_CB_ID, TEST_HAL_DMA_XferCpltCallback);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(DMA_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEMPLATE_DMA_IRQHandler);
#else
    csi_clic_register_irq(DMA_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEMPLATE_DMA_IRQHandler);
#endif
    HAL_DMA_Start(&tx_hdma, (uint32_t)&tx_data[0], (uint32_t)&rx_data[0], TEST_DMA_BUFF_LENGTH);
    // HAL_DMA_Start_IT(&tx_hdma, (uint32_t)&tx_data[0], (uint32_t)&rx_data[0], TEST_DMA_BUFF_LENGTH);
#endif
    if(result != 0)
    {
        printf("\nhyper_capture:\n");
        for(int i = 0; i < 32; i++)
        {
            printf(" %d,", hyper_capture[i]);
        }
        printf("\n");
        printf("ldo_capture:\n");
        printf("tmpFactor:");
        for(int i = 0; i < 3; i++)
        {
            printf(" %d,", ldo_capture[i]);
        }
        printf("\ndiff:");
        for(int i = 3; i < 6; i++)
        {
            printf(" %d,", ldo_capture[i]);
        }
        printf("\nminDiff:%d", ldo_capture[6]);
        printf("\ntargetFactor:%d", ldo_capture[7]);
        printf("\n");
    }
    else {
        printf(" pass\n");
        
        HAL_WDT_StructInit(&hwdg);
        hwdg.Init.ResetMode = WDT_RESET_MODE;
        hwdg.Init.Counter = WDT_Timeout_64K_CLOCKS;
        HAL_WDT_Init(&hwdg);
        HAL_WDT_Enable(&hwdg);
        HAL_WDT_SetTimeoutPeriod(&hwdg, WDT_Timeout_64K_CLOCKS);
        HAL_WDT_Refresh(&hwdg);
        
        
        
        
        
        
        
        
        
//        printf(" pass\n");
        
//        csi_system_reset();
    }
    while (1)
    {
		HAL_Delay(100);
    }
	return 0;
}


#endif
#endif
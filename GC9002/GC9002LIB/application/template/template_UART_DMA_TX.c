#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx.h"
#include "core_rv32.h"
#include "template_def.h"
#include "csi_irq.h"

#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_UART_DMA_TX_
    

#define TEST_UARTx          UART1
#define UARTx_IRQn          UART1_IRQn
#define UARTx_IRQHandler    TEMPLATE_UART1_IRQHandler
#define TEST_DMA_HS         DMA_HS_UART1_TX

static DMA_HandleTypeDef tx_hdma =
{
    .Instance = DMA,
};
static UART_HandleTypeDef huart =
{
    .Instance = TEST_UARTx,
};
static void UART_Init_Test(void);
static void DMA_Init_Test(void);


int main(void)
{
    UART_Init_Test();
    DMA_Init_Test();
    
	uint8_t pdata_tx[64];
    
	memset(pdata_tx, 'H', 64);
    while (1)
    {
        HAL_UART_Transmit_DMA(&huart, pdata_tx, 64);
		HAL_Delay(1000);
    }
	return 0;
}


#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_UART0_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_UART0_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

	HAL_UART_IRQHandler(&huart);
	__HAL_UART_CLEAR_IDLEFLAG(&huart);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_UART1_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_UART1_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

	HAL_UART_IRQHandler(&huart);
	__HAL_UART_CLEAR_IDLEFLAG(&huart);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_UART2_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_UART2_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

	HAL_UART_IRQHandler(&huart);
	__HAL_UART_CLEAR_IDLEFLAG(&huart);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_UART_DMA_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_UART_DMA_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();

	HAL_DMA_IRQHandler(huart.hdmatx);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

static void UART_Init_Test(void)
{
    /* config the UART */
    HAL_UART_StructInit(&(huart.Init));
    HAL_UART_Init(&huart);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(UARTx_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, UARTx_IRQHandler);
#else
    csi_clic_register_irq(UARTx_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, UARTx_IRQHandler);
#endif
	
}
static void DMA_Init_Test(void)
{
    HAL_DMA_StructInit(&(tx_hdma.Init));
	__HAL_LINKDMA(&huart, hdmatx, tx_hdma);
    tx_hdma.Init.mChannel = DMA_CHANNEL1;
    tx_hdma.Init.mS_Address = (uint32_t)0;/*!< Specifies Source Address.             */
    tx_hdma.Init.mD_Address = ((uint32_t)&(huart.Instance->DR));/*!< Specifies Destination Address.        */
    tx_hdma.Init.mBlockSize = 0;
    tx_hdma.Init.mS_AutoReload_EN = DMA_DISABLE;
    tx_hdma.Init.mD_AutoReload_EN = DMA_DISABLE;
    tx_hdma.Init.mS_TransferWidth = DMA_TR_WIDTH_8bits;
    tx_hdma.Init.mD_TransferWidth = DMA_TR_WIDTH_8bits;
    tx_hdma.Init.mS_BurstTransactionLen = DMA_DATA_ITEM_1;
    tx_hdma.Init.mD_BurstTransactionLen = DMA_DATA_ITEM_1;
    tx_hdma.Init.mS_AddrInc = DMA_INCREMENTAL;
    tx_hdma.Init.mD_AddrInc = DMA_FIXED;
    tx_hdma.Init.mChannelPriority = DMA_MIN_CHANNEL_PRIORITY;
    tx_hdma.Init.mTransferType_FlowControl = DMA_TT_MEM_TO_PER_DMAC;
    tx_hdma.Init.mS_Peripheral = DMA_HS_MEMORY;
    tx_hdma.Init.mD_Peripheral = TEST_DMA_HS;
    tx_hdma.Init.mEnable = DMA_DISABLE;
    HAL_DMA_Init(&tx_hdma);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(DMA_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEMPLATE_UART_DMA_IRQHandler);
#else
    csi_clic_register_irq(DMA_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEMPLATE_UART_DMA_IRQHandler);
#endif

    CLEAR_BIT(huart.Instance->CR3, UART_CR3_EIE);
    __HAL_UART_CLEAR_IDLEFLAG(&(huart));
    __HAL_UART_ENABLE_IT(&(huart), UART_IT_TC);
}


#endif
#endif
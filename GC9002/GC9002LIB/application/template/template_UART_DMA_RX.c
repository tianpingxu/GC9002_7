#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx.h"
#include "core_rv32.h"
#include "template_def.h"
#include "csi_irq.h"

#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_UART_DMA_RX_


#define TEST_UARTx          UART0
#define UARTx_IRQn          UART0_IRQn
#define UARTx_IRQHandler    TEMPLATE_UART0_IRQHandler
#define TEST_DMA_HS         DMA_HS_UART0_RX

static DMA_HandleTypeDef rx_hdma =
{
    .Instance = DMA,
};
static UART_HandleTypeDef handle_uart =
{
    .Instance = TEST_UARTx,
};
static void UART_Init_Test(void);
static void DMA_Init_Test(void);

static uint8_t rx_data[64];
int main(void)
{
    UART_Init_Test();
    DMA_Init_Test();
	memset(rx_data, 0, 64);
    HAL_UART_SendData(TEST_UARTx, 'H');
    HAL_UART_SendData(TEST_UARTx, 'e');
    HAL_UART_SendData(TEST_UARTx, 'l');
    HAL_UART_SendData(TEST_UARTx, 'l');
    HAL_UART_SendData(TEST_UARTx, 'o');
	HAL_UART_Receive_DMA(&handle_uart, rx_data, 2);
    while (1)
    {
		HAL_Delay(100);
    }
	return 0;
}

void TEST_HAL_UART_IDLECallback(UART_HandleTypeDef *huart)
{
	__HAL_UART_CLEAR_IDLEFLAG(huart);
	HAL_UART_Receive_DMA(huart, rx_data, 2);
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

	HAL_UART_IRQHandler(&handle_uart);

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

	HAL_UART_IRQHandler(&handle_uart);

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

	HAL_UART_IRQHandler(&handle_uart);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


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

	HAL_DMA_IRQHandler(handle_uart.hdmarx);

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

static void UART_Init_Test(void)
{
    /* config the UART */
    HAL_UART_StructInit(&(handle_uart.Init));
    HAL_UART_Init(&handle_uart);
    HAL_UART_RegisterCallback(&handle_uart, HAL_UART_IDLE_CB_ID, TEST_HAL_UART_IDLECallback);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
   csi_clic_register_irq(UARTx_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, UARTx_IRQHandler);
#else
    csi_clic_register_irq(UARTx_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, UARTx_IRQHandler);
#endif
	
}
static void DMA_Init_Test(void)
{
    HAL_DMA_StructInit(&(rx_hdma.Init));
	__HAL_LINKDMA(&handle_uart, hdmarx, rx_hdma);
    rx_hdma.Init.mChannel = DMA_CHANNEL1;
    rx_hdma.Init.mS_Address = ((uint32_t)&(handle_uart.Instance->DR));/*!< Specifies Source Address.             */
    rx_hdma.Init.mD_Address = 0;/*!< Specifies Destination Address.        */
    rx_hdma.Init.mBlockSize = 0;
    rx_hdma.Init.mS_AutoReload_EN = DMA_DISABLE;
    rx_hdma.Init.mD_AutoReload_EN = DMA_DISABLE;
    rx_hdma.Init.mS_TransferWidth = DMA_TR_WIDTH_8bits;
    rx_hdma.Init.mD_TransferWidth = DMA_TR_WIDTH_8bits;//16
    rx_hdma.Init.mS_BurstTransactionLen = DMA_DATA_ITEM_1;
    rx_hdma.Init.mD_BurstTransactionLen = DMA_DATA_ITEM_1;
    rx_hdma.Init.mS_AddrInc = DMA_FIXED;
    rx_hdma.Init.mD_AddrInc = DMA_INCREMENTAL;
    rx_hdma.Init.mChannelPriority = DMA_MIN_CHANNEL_PRIORITY;
    rx_hdma.Init.mTransferType_FlowControl = DMA_TT_PER_TO_MEM_DMAC;
    rx_hdma.Init.mS_Peripheral = TEST_DMA_HS;
    rx_hdma.Init.mD_Peripheral = DMA_HS_MEMORY;
    rx_hdma.Init.mEnable = DMA_DISABLE;
    HAL_DMA_Init(&rx_hdma);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(DMA_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEMPLATE_DMA_IRQHandler);
#else
    csi_clic_register_irq(DMA_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEMPLATE_DMA_IRQHandler);
#endif

    CLEAR_BIT(handle_uart.Instance->CR3, UART_CR3_EIE);
    __HAL_UART_CLEAR_IDLEFLAG(&(handle_uart));
    __HAL_UART_ENABLE_IT(&handle_uart, UART_IT_IDLE);
}


#endif
#endif
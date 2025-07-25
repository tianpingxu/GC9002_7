#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx.h"
#include "core_rv32.h"
#include "template_def.h"
#include "csi_irq.h"
#include "board.h"
#include "csi_irq_vectors.h"

#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_UART_IT_

#define TEST_UARTx          UART1
#define UARTx_IRQn          UART1_IRQn
#define UARTx_IRQHandler    TEMPLATE_UART1_IRQHandler

static UART_HandleTypeDef huart = {.Instance = TEST_UARTx, };
/*
 * TEMPLATE_UART0_IRQHandler 和 UART2_IRQHandler的实现相同
 * 在编译等级为-Os下，编译出的固件有bug，UART2_IRQHandler的执行过程中会jal TEMPLATE_UART0_IRQHandler
 * 从而导致异常，异常码mcause = 0x08000001
 * */
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

    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
static volatile uint32_t flag = 0;
static void TEST_HAL_TX_Comple_Callback(UART_HandleTypeDef *huart)
{
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC);
    while(!__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE));
    while(!__HAL_UART_GET_FLAG(huart, UART_FLAG_TC));
    *((volatile unsigned int *)(GPIOA_BASE+4*9)) = 0;
    *((volatile unsigned int *)(GPIOA_BASE+4*9)) = 1;
	flag = 1;
}
static void TEST_HAL_ErrCallback(UART_HandleTypeDef *huart)
{
    __HAL_UART_CLEAR_FEFLAG(huart);
}
int main(void)
{
    /* config the UART */
    huart.Instance = TEST_UARTx;
    HAL_UART_StructInit(&(huart.Init));
    HAL_UART_Init(&huart);
	HAL_UART_RegisterCallback(&huart, HAL_UART_TX_COMPLETE_CB_ID, TEST_HAL_TX_Comple_Callback);
	HAL_UART_RegisterCallback(&huart, HAL_UART_ERROR_CB_ID, TEST_HAL_ErrCallback);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(UARTx_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, UARTx_IRQHandler);
#else
    csi_clic_register_irq(UARTx_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, UARTx_IRQHandler);
#endif
    
	uint8_t pdata_tx[64];
	uint8_t pdata_rx[64];
	memset(pdata_tx, 'H', 64);
    while (1)
    {
		flag = 0;
        HAL_UART_Transmit_IT(&huart, pdata_tx, 5);
		while (flag == 0);
		while(1);
        HAL_Delay(10);
    }
	return 0;
}

#endif
#endif
/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : com_c1.c
*
* Description  : This file contains the implementation of COM interface
*
* Environment  : MDK / PC Simulation
*
* Responsible  : 
*
* Guidelines   : 
*
* Template name: C++
*
* Data         : 2024-02-27
****************************************************************************/


/* Foreign headerfiles */
#include <stdio.h>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "event_groups.h"
#include "semphr.h"

#include "trace.h"


/* Own headerfiles */
#include "com_c1.h"

// #if COM_COMPILE_SWITCH

k_task_handle_t ComTaskHandle;
static EventGroupHandle_t UartRxEvent = NULL;

static DMA_HandleTypeDef rx_hdma =
{
    .Instance = DMA,
};

UART_HandleTypeDef handle_uart =
{
    .Instance = UART1,
};

static __attribute__ ((aligned(4))) uint8_t uart_rx_buf[UART_RX_BUF_SIZE];
static __attribute__ ((aligned(4))) uint8_t uart_tx_buf[UART_TX_BUF_SIZE];
static Fifo_t uart_rx_fifo;
static Fifo_t uart_tx_fifo;

static volatile uint8_t dma_rx_buf_index = 0;
static volatile uint8_t dma_rx_buf_index_last = 0;
/* Make sure UART DMA buffer localled in iRAM address space */
#ifndef CHIP_GC9002_NoHyper
__attribute__((used, section(".iramdata"))) 
#endif
__attribute__ ((aligned(64)))uint8_t dma_rx_buf[2][UART_DMA_SIZE]  ;
/* Define customer parse (once) buffer */
static __attribute__ ((aligned(4))) uint8_t customer_parse_buf[CUSTOM_UART_PARSE_ONCE_SIZE];

static uint8_t COM_Uart_u8TxActive;


static void COM_vTaskInit( void );
static void COM_vTask(void* pvParameters);
static void COM_vUartInit( void );
static void COM_vDmaInit( void );
static void COM_UART_MspInit(UART_HandleTypeDef *huart);
static void COM_UART_ErrorCallback(UART_HandleTypeDef *huart);
static void COM_UART_IDLECallback(UART_HandleTypeDef *huart);
#if (defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void COM_UART_IRQHandler(void);
#else
static ATTRIBUTE_ISR void COM_UART_IRQHandler(void);
#endif

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void COM_DMA_IRQHandler(void);
#else
static ATTRIBUTE_ISR void COM_DMA_IRQHandler(void);
#endif

/**
  * @name       COM_vInit
  * @brief      Initialize for COM module
  * @note       
  * @param      none
  * @retval     none
  * @author     
  * @date       2024-02-29
  */
void COM_vInit(void)
{
    COM_Uart_u8TxActive = 0;
    fifo_init(&uart_rx_fifo, uart_rx_buf, UART_RX_BUF_SIZE, 1);
    fifo_init(&uart_tx_fifo, uart_tx_buf, UART_TX_BUF_SIZE, 1);

    UartRxEvent = xEventGroupCreate();
    if (NULL == UartRxEvent)
    {
        printf("UartRxEvent create failed\n");
    }

    COM_vTaskInit();
    COM_vDmaInit();
    COM_vUartInit();
    
    HAL_UART_Receive_DMA(&handle_uart, &dma_rx_buf[0][0], (UART_DMA_SIZE <= DMA_CHANNEL_2_BLOCK_TS_MAX ? UART_DMA_SIZE : DMA_CHANNEL_2_BLOCK_TS_MAX));
}


static void COM_vTaskInit( void )
{
    portBASE_TYPE xReturn;
    
    xReturn = csi_kernel_task_new(COM_vTask, "ahmi com task", NULL, KPRIO_LOW3, 0, NULL, configMINIMAL_STACK_SIZE*2, &ComTaskHandle);
    if (xReturn != 0)
    {
        ERROR_PRINT("Fail to create SetValueTask Task.\r\n");
    }
}


static void COM_vTask(void* pvParameters)
{
    EventBits_t evt_bits = 0;
    while(1)
    {
        evt_bits = xEventGroupWaitBits(UartRxEvent, UART_RX_EVENT_UART_IDLE,     //等待的event标志位
                                        pdTRUE,     //退出xEventGroupWaitBits之前，event标志位是否清除，若不清除，那么当3个标志位一起到来后，xEventGroupWaitBits持续激活态
                                        pdFALSE,    //event事件之间的关系,pdTRUE: &, pdFALSE：|
                                        portMAX_DELAY);
        if((evt_bits & UART_RX_EVENT_UART_IDLE) == UART_RX_EVENT_UART_IDLE)
        {
            uint16_t len;
            #if COM_UART_TEST_ROLLBACK
            static uint32_t rec_cnt = 0;
            #endif

            while(1)
            {
                len = fifo_readable_item_count(&uart_rx_fifo);
                if (len >= CUSTOM_UART_PARSE_ONCE_SIZE)
                {
                    len = CUSTOM_UART_PARSE_ONCE_SIZE;
                }
                else if (len == 0)
                {
                    break;
                }

                fifo_read_batch(&uart_rx_fifo, customer_parse_buf, len);
                extern void rx_data_hadle(uint8_t* buf, uint16_t len);
                rx_data_hadle(customer_parse_buf, len);

                // #if COM_UART_TEST_ROLLBACK
                // rec_cnt += len;
                // COM_iUartAsynSend(customer_parse_buf, len);
                // #else
                // CUSTOM_UART_PARSE_FUNC(customer_parse_buf, len);
                // #endif
            }
        }
    }
}


static void COM_vUartInit( void )
{
    /* config the UART */
    handle_uart.Instance = CUSTOM_UART;
    HAL_UART_StructInit(&(handle_uart.Init));
    handle_uart.Init.BaudRate = CUSTOM_UART_BAUD_RATE;
    
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(CUSTOM_UART_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 2, COM_UART_IRQHandler);
#else
    csi_clic_register_irq(CUSTOM_UART_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 2, COM_UART_IRQHandler);
#endif
    
    HAL_UART_RegisterCallback(&handle_uart, HAL_UART_MSPINIT_CB_ID, COM_UART_MspInit);
    HAL_UART_Init(&handle_uart);
    HAL_UART_RegisterCallback(&handle_uart, HAL_UART_ERROR_CB_ID, COM_UART_ErrorCallback);
    HAL_UART_RegisterCallback(&handle_uart, HAL_UART_IDLE_CB_ID, COM_UART_IDLECallback);
    
    CLEAR_BIT(handle_uart.Instance->CR3, UART_CR3_EIE);
    __HAL_UART_CLEAR_IDLEFLAG(&(handle_uart));
    __HAL_UART_ENABLE_IT(&handle_uart, UART_IT_IDLE);
}


static void COM_vDmaInit( void )
{
    HAL_DMA_StructInit(&(rx_hdma.Init));
    __HAL_LINKDMA(&handle_uart, hdmarx, rx_hdma);
    rx_hdma.Instance = DMA;
    rx_hdma.Init.mChannel = DMA_CHANNEL2;
    rx_hdma.Init.mS_Address =  ((uint32_t)&(handle_uart.Instance->DR));;/*!< Specifies Source Address.             */
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
    rx_hdma.Init.mChannelPriority = DMA_MAX_CHANNEL_PRIORITY;
    rx_hdma.Init.mTransferType_FlowControl = DMA_TT_PER_TO_MEM_DMAC;
    rx_hdma.Init.mS_Peripheral = CUSTOM_UART_DMA_RX_HS;
    rx_hdma.Init.mD_Peripheral = DMA_HS_MEMORY;
    rx_hdma.Init.mEnable = DMA_DISABLE;
    rx_hdma.Parent = &handle_uart;
    HAL_DMA_Init(&rx_hdma);

}


static void COM_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    uint32_t rxPin, txPin;
    GPIO_TypeDef* rxPort;
    GPIO_TypeDef* txPort;
    bool errExist = FALSE;
    
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    rxPort = CUSTOM_UART_RX_PORT;
    rxPin = CUSTOM_UART_RX_PIN;
    txPort = CUSTOM_UART_TX_PORT;
    txPin = CUSTOM_UART_TX_PIN;

    if(huart->Instance == UART0)
    {
    	if((GPIO_PIN_2 == rxPin) && (GPIO_PIN_3 == txPin) && (GPIOB == rxPort) && (GPIOB == txPort))
    	{
            /**UART0 GPIO Configuration
             * PB2      ------> UART0_RX 
             * PB3      ------> UART0_TX
             * */
    		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF2;
    	}
        else if((GPIO_PIN_0 == rxPin) && (GPIO_PIN_1 == txPin) && (GPIOA == rxPort) && (GPIOA == txPort))
        {
            /**UART0 GPIO Configuration
             * PA0      ------> UART0_RX 
             * PA1      ------> UART0_TX
             * */
    		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF4;
    	}
        else if((GPIO_PIN_15 == rxPin) && (GPIO_PIN_14 == txPin) && (GPIOB == rxPort) && (GPIOB == txPort))
        {
            /**UART0 GPIO Configuration
             * PB15      ------> UART0_RX 
             * PB14      ------> UART0_TX
             * */
    		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF5;
    	}
        else
        {
            errExist = TRUE;
        }
        GPIO_InitStruct.Pin = rxPin | txPin;
    	HAL_GPIO_Init(rxPort, &GPIO_InitStruct);
    }
    else if(huart->Instance == UART1)
    {
        if((GPIO_PIN_6 == rxPin) && (GPIO_PIN_7 == txPin) && (GPIOB == rxPort) && (GPIOB == txPort))
    	{
            /**UART1 GPIO Configuration
             * PB6      ------> UART1_RX
             * PB7      ------> UART1_TX
             * */
    		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF1;
    	}
        else if((GPIO_PIN_0 == rxPin) && (GPIO_PIN_1 == txPin) && (GPIOB == rxPort) && (GPIOB == txPort))
        {
            /**UART1 GPIO Configuration
             * PB0      ------> UART0_RX 
             * PB1      ------> UART0_TX
             * */
    		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF2;
    	}
        else if((GPIO_PIN_2 == rxPin) && (GPIO_PIN_3 == txPin) && (GPIOA == rxPort) && (GPIOA == txPort))
        {
            /**UART1 GPIO Configuration
             * PA2      ------> UART0_RX 
             * PA3      ------> UART0_TX
             * */
    		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF4;
    	}
        else
        {
            errExist = TRUE;
        }
	}
    else if(huart->Instance == UART2)
    {
        
        if((GPIO_PIN_8 == rxPin) && (GPIO_PIN_9 == txPin) && (GPIOB == rxPort) && (GPIOB == txPort))
    	{
            /**UART2 GPIO Configuration
             * PB8      ------> UART2_RX    K24     EX2_IO2
             * PB9      ------> UART2_TX    K25     EX2_IO3
             * */
    		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF1;
    	}
        else if((GPIO_PIN_4 == rxPin) && (GPIO_PIN_5 == txPin) && (GPIOA == rxPort) && (GPIOA == txPort))
        {
            /**UART2 GPIO Configuration
             * PA4      ------> UART2_RX 
             * PA5      ------> UART2_TX
             * */
    		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF4;
    	}
        else if((GPIO_PIN_15 == rxPin) && (GPIO_PIN_16 == txPin) && (GPIOA == rxPort) && (GPIOA == txPort))
        {
            /**UART2 GPIO Configuration
             * PA15      ------> UART2_RX 
             * PA16      ------> UART2_TX
             * */
    		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF2;
    	}
        else
        {
            errExist = TRUE;
        }
	}

    if(FALSE == errExist)
    {
        GPIO_InitStruct.Pin = rxPin | txPin;
        HAL_GPIO_Init(rxPort, &GPIO_InitStruct);
    }
    else
    {
        /* Record error */
    
        /* Depend on FAE request, initialization failing will hold on here */
        while(1);
    }
    
}


static void COM_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    __HAL_UART_CLEAR_OREFLAG(huart);
    __HAL_UART_CLEAR_NEFLAG(huart);
    __HAL_UART_CLEAR_FEFLAG(huart);
    if (huart == &handle_uart)
    {        
        dma_rx_buf_index_last = dma_rx_buf_index;
        dma_rx_buf_index ^= 1;      
        
        CLEAR_BIT(huart->Instance->CR3, UART_CR3_DMAR);
        uint64_t tempreg = 0;
        /* Disable the stream */
        __HAL_DMA_CHX_DISABLE(huart->hdmarx, huart->hdmarx->Init.mChannel);
        /* Check if the DMA Stream is effectively disabled */
        while(__HAL_DMA_CHANNEL_IS_ENABLE(huart->hdmarx));
        
        /* set destination address. */
        huart->hdmarx->Instance->CHx[huart->hdmarx->Init.mChannel].DAR.reg_u64val = (uint64_t)(uint32_t)&dma_rx_buf[dma_rx_buf_index][0];
        
        /* set transfer block size. Block Transfer Size = BLOCK_TS */
        tempreg = huart->hdmarx->Instance->CHx[huart->hdmarx->Init.mChannel].CTL.reg_u64val;
        ((union _DMA_CTL *)&tempreg)->bit.BLOCK_TS = huart->hdmarx->Init.mBlockSize;
        huart->hdmarx->Instance->CHx[huart->hdmarx->Init.mChannel].CTL.reg_u64val = tempreg;
        
        /* Enable the Peripheral */
        __HAL_DMA_CHX_ENABLE(huart->hdmarx, huart->hdmarx->Init.mChannel);
        
        __HAL_UART_CLEAR_FEFLAG(huart);
        SET_BIT(huart->Instance->CR3, UART_CR3_DMAR);
    }
}


#ifndef CHIP_GC9002_NoHyper
__attribute__((used, section(".flashdriver"))) 
#endif
static void COM_UART_IDLECallback(UART_HandleTypeDef *huart)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult = pdFAIL;
    uint32_t temp_index = 0;
    uint32_t length = 0;
    uint32_t count = 0;
    uint32_t fifo_count = 0;
    uint64_t tempreg = 0;

    count = __HAL_DMA_CHX_GET_COUNTER(huart->hdmarx);
    if(count == 0)
    {
        __HAL_UART_CLEAR_IDLEFLAG(huart);
        return;
    }
    dma_rx_buf_index_last = dma_rx_buf_index;
    dma_rx_buf_index ^= 1;

    CLEAR_BIT(huart->Instance->CR3, UART_CR3_DMAR);
    
    __HAL_DMA_CHX_DISABLE(huart->hdmarx, huart->hdmarx->Init.mChannel);
    /* Check if the DMA Stream is effectively disabled */
    while(__HAL_DMA_CHANNEL_IS_ENABLE(huart->hdmarx));
    
    /* set destination address. */
    huart->hdmarx->Instance->CHx[huart->hdmarx->Init.mChannel].DAR.reg_u64val = (uint64_t)(uint32_t)&dma_rx_buf[dma_rx_buf_index][0];
    
    /* set transfer block size. Block Transfer Size = BLOCK_TS */
    tempreg = huart->hdmarx->Instance->CHx[huart->hdmarx->Init.mChannel].CTL.reg_u64val;
    ((union _DMA_CTL *)&tempreg)->bit.BLOCK_TS = huart->hdmarx->Init.mBlockSize;
    huart->hdmarx->Instance->CHx[huart->hdmarx->Init.mChannel].CTL.reg_u64val = tempreg;
    
    /* Enable the Peripheral */
    __HAL_DMA_CHX_ENABLE(huart->hdmarx, huart->hdmarx->Init.mChannel);
    
    __HAL_UART_CLEAR_FEFLAG(huart);
    /* Enable the UART Parity Error Interrupt */
    SET_BIT(huart->Instance->CR3, UART_CR3_DMAR);
    
    fifo_count = fifo_writeable_item_count(&uart_rx_fifo);
    if(fifo_count < count)
    {
        count = fifo_count;
    }
    fifo_write_batch(&uart_rx_fifo, &dma_rx_buf[dma_rx_buf_index_last][0], count);
	
    EventBits_t bit = xEventGroupGetBitsFromISR(UartRxEvent);
	if((bit & UART_RX_EVENT_UART_IDLE) != UART_RX_EVENT_UART_IDLE)
	{
		xResult = xEventGroupSetBitsFromISR(UartRxEvent, UART_RX_EVENT_UART_IDLE, &xHigherPriorityTaskWoken);
		// Was the message posted successfully?
		if(xResult == pdPASS)
		{
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}	
	}
}


/**
  * @name       COM_UART_DMA_RxCpltCallback
  * @brief      UART DMA rx complete callback function
  * @note       
  * @param      huart: UART handler
  * @retval     none
  * @author     
  * @date       2024-02-28
  */
#ifndef CHIP_GC9002_NoHyper
__attribute__((used, section(".flashdriver"))) 
#endif
void COM_UART_DMA_RxCpltCallback(DMA_HandleTypeDef *hdma)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult = pdFAIL;
    uint32_t length = 0;
    uint32_t count = 0;
    uint64_t tempreg = 0;
    uint32_t fifo_count = 0;
    dma_rx_buf_index_last = dma_rx_buf_index;
    dma_rx_buf_index ^= 1;

    UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;
    CLEAR_BIT(huart->Instance->CR3, UART_CR3_DMAR);
    
    __HAL_DMA_CHX_DISABLE(hdma, hdma->Init.mChannel);
    /* Check if the DMA Stream is effectively disabled */
    while(__HAL_DMA_CHANNEL_IS_ENABLE(hdma));
    
    /* set destination address. */
    huart->hdmarx->Instance->CHx[huart->hdmarx->Init.mChannel].DAR.reg_u64val = (uint64_t)(uint32_t)&dma_rx_buf[dma_rx_buf_index][0];

    /* set transfer block size. Block Transfer Size = BLOCK_TS */
    tempreg = huart->hdmarx->Instance->CHx[huart->hdmarx->Init.mChannel].CTL.reg_u64val;
    ((union _DMA_CTL *)&tempreg)->bit.BLOCK_TS = huart->hdmarx->Init.mBlockSize;
    huart->hdmarx->Instance->CHx[huart->hdmarx->Init.mChannel].CTL.reg_u64val = tempreg;
    
    /* Disable all the transfer interrupts */
    __HAL_DMA_CHX_CLEAR_IT(hdma, hdma->Init.mChannel, DMA_INT_ALL);
    /* Enable the Peripheral */
    __HAL_DMA_CHX_ENABLE(hdma, hdma->Init.mChannel);
    
    __HAL_UART_CLEAR_OREFLAG(huart);
    SET_BIT(huart->Instance->CR3, UART_CR3_DMAR);
    
    fifo_count = fifo_writeable_item_count(&uart_rx_fifo);
    count = huart->hdmarx->Init.mBlockSize;
    if(fifo_count < count)
    {
        count = fifo_count;
    }
    fifo_write_batch(&uart_rx_fifo, &dma_rx_buf[dma_rx_buf_index_last][0], count);

    EventBits_t bit = xEventGroupGetBitsFromISR(UartRxEvent);
	if((bit & UART_RX_EVENT_UART_IDLE) != UART_RX_EVENT_UART_IDLE)
	{
		xResult = xEventGroupSetBitsFromISR(UartRxEvent, UART_RX_EVENT_UART_IDLE, &xHigherPriorityTaskWoken);
		// Was the message posted successfully?
		if(xResult == pdPASS)
		{
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
	}
}



#if (defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void COM_UART_IRQHandler(void)
#else
#ifndef CHIP_GC9002_NoHyper
__attribute__((used, section(".flashdriver"))) 
#endif
static ATTRIBUTE_ISR void COM_UART_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    uint32_t isrflags = READ_REG(handle_uart.Instance->SR);
    /* Disable all the transfer interrupts */
    if ((isrflags & 0x0080) && (COM_Uart_u8TxActive == TRUE))
    {
      /* Transmit Data Register Empty */
      if (uart_tx_fifo.idx_read != uart_tx_fifo.idx_write)
      {
        uint8_t *pChar = uart_tx_fifo.fifo_ptr;
        handle_uart.Instance->DR = pChar[uart_tx_fifo.idx_read++];
        
        if(uart_tx_fifo.idx_read >= uart_tx_fifo.item_sum)
        {
            uart_tx_fifo.idx_read = 0;
        }
        
      }
      else
      {
        handle_uart.Instance->CR1 &= ~0x0080;
        COM_Uart_u8TxActive = FALSE;
      }
    }
    
    if(isrflags & (uint32_t)(UART_SR_IDLE))
    {
        if(handle_uart.IDLECallback)
            handle_uart.IDLECallback(&handle_uart);
    }
    else if(isrflags & (uint32_t)(UART_SR_PE | UART_SR_FE | UART_SR_ORE | UART_SR_NE))
    {
        if(handle_uart.ErrorCallback)
            handle_uart.ErrorCallback(&handle_uart);
    }
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void COM_DMA_IRQHandler(void)
#else
static ATTRIBUTE_ISR void COM_DMA_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    /* Disable all the transfer interrupts */
    __HAL_DMA_CHX_CLEAR_IT(handle_uart.hdmarx, handle_uart.hdmarx->Init.mChannel, DMA_INT_ALL);
    COM_UART_DMA_RxCpltCallback(handle_uart.hdmarx);

#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


/**
  * @name       COM_iUartAsynSend
  * @brief      Initialize the Timer, default use Timer7
  * @note       
  * @param      buf: data buffer to send out
  * @param      len: data size
  * @retval     0: send successful, other: send failed
  * @author     
  * @date       2024-02-28
  */
int32_t COM_iUartAsynSend(uint8_t* buf, uint16_t len)
{
    int res;
    
    if(fifo_write_batch(&uart_tx_fifo, buf, (int)len))
    {
        res = 0;
        
        csi_irq_disable(CUSTOM_UART_IRQn);
        __ASM volatile("nop");
        if (COM_Uart_u8TxActive == FALSE)
        {
            /* Send directly to UART. */
            uint8_t *pChar = uart_tx_fifo.fifo_ptr;
            handle_uart.Instance->DR = pChar[uart_tx_fifo.idx_read++];
            
            if(uart_tx_fifo.idx_read >= uart_tx_fifo.item_sum)
            {
                uart_tx_fifo.idx_read = 0;
            }
            handle_uart.Instance->CR1 |= 0x0080;
            COM_Uart_u8TxActive = TRUE;
        }
        else
        {
        
        }
        csi_irq_enable(CUSTOM_UART_IRQn);
        
    }
    else
    {
        res = -1;
    }

    return res;
}


// #define FRAME_SIZE 32  // 每个数据帧最大长度（不包括帧尾）
// #define END_MARKER 0x0D  // 数据帧结束符（0x0D 对应十六进制的 CR）
// #define END_MARKER_NEXT 0x0A // 数据帧结束符（0x0A 对应十六进制的 LF）

// void split_data(const unsigned char *data, size_t data_len) {
//     size_t i = 0;
//     size_t frame_start = 0;
//     size_t frame_len = 0;
//     size_t frame_count = 0;
	
//     unsigned char frames[6][FRAME_SIZE + 2] = {0};  

//     while (i < data_len) 
// 	{
//         if ((i + 1 < data_len && data[i] == END_MARKER) && (data[i + 1] == END_MARKER_NEXT)) 
// 		{  
//             frame_len = i - frame_start;  
			
//             if (frame_len > FRAME_SIZE) 
// 			{
//                 size_t extra_data_len = frame_len - FRAME_SIZE;
//                 size_t split_frame = 0;

//                 for (size_t j = frame_start; j < frame_start + FRAME_SIZE; ++j) {
//                     frames[frame_count][split_frame++] = data[j];
//                 }
				
//                 frame_count++;
//                 frame_start = i + 2;  
//             } 
// 			else 
// 			{ 
//                 size_t split_frame = 0;
				
//                 for (size_t j = frame_start; j < frame_start + frame_len; ++j) {
//                     frames[frame_count][split_frame++] = data[j];
//                 }
				
//                 frame_count++;
//             }
			
//             frame_start = i + 2;  
//             i += 2;  
			
//         } 
// 		else 
// 		{
//             ++i;
//         }
//     }
// 	for(int a = 0; a <= 2; a++){
// 	csi_ahmi_SetRegValue(1010,1);
// 	csi_ahmi_SetStrRegValue(1015, (char *)(&frames[0][0]), FRAME_SIZE);
// 	csi_kernel_delay(20);
// 	csi_ahmi_SetStrRegValue(1016, (char *)(&frames[1][0]), FRAME_SIZE);
// 	csi_kernel_delay(20);
// 	csi_ahmi_SetStrRegValue(1040, (char *)(&frames[2][0]), FRAME_SIZE);
// 	csi_kernel_delay(20);
// 	csi_ahmi_SetStrRegValue(1041, (char *)(&frames[3][0]), FRAME_SIZE);
// 	csi_kernel_delay(20);
// 	csi_ahmi_SetStrRegValue(1042, (char *)(&frames[4][0]), FRAME_SIZE);
// 	csi_kernel_delay(20);
// 	csi_ahmi_SetRegValue(1010,0);
// 	}/*
// 	csi_ahmi_SetRegValue(1010,1);
// 	csi_ahmi_SetStrRegValue(1015, (char *)(&frames[0][0]), FRAME_SIZE);
// 	csi_ahmi_SetStrRegValue(1016, (char *)(&frames[1][0]), FRAME_SIZE);
// 	csi_ahmi_SetStrRegValue(1040, (char *)(&frames[2][0]), FRAME_SIZE);
// 	csi_ahmi_SetStrRegValue(1041, (char *)(&frames[3][0]), FRAME_SIZE);
// 	csi_ahmi_SetStrRegValue(1042, (char *)(&frames[4][0]), FRAME_SIZE);
// 	csi_ahmi_SetRegValue(1010,0);*/
// }

// void split_data_card(const unsigned char *data, size_t data_len) {
//     size_t i = 0;
//     size_t frame_start = 0;
//     size_t frame_len = 0;
//     size_t frame_count = 0;
	
//     unsigned char frames[6][FRAME_SIZE + 2] = {0};  

//     while (i < data_len) 
// 	{
//         if ((i + 1 < data_len && data[i] == END_MARKER) && (data[i + 1] == END_MARKER_NEXT)) 
// 		{  
//             frame_len = i - frame_start;  
			
//             if (frame_len > FRAME_SIZE) 
// 			{
//                 size_t extra_data_len = frame_len - FRAME_SIZE;
//                 size_t split_frame = 0;

//                 for (size_t j = frame_start; j < frame_start + FRAME_SIZE; ++j) {
//                     frames[frame_count][split_frame++] = data[j];
//                 }
				
//                 frame_count++;
//                 frame_start = i + 2;  
//             } 
// 			else 
// 			{ 
//                 size_t split_frame = 0;
				
//                 for (size_t j = frame_start; j < frame_start + frame_len; ++j) {
//                     frames[frame_count][split_frame++] = data[j];
//                 }
				
//                 frame_count++;
//             }
			
//             frame_start = i + 2;  
//             i += 2;  
			
//         } 
// 		else 
// 		{
//             ++i;
//         }
//     }
// 	csi_ahmi_SetStrRegValue(1012, (char *)(&frames[0][0]), FRAME_SIZE);
// 	csi_ahmi_SetStrRegValue(1019, (char *)(&frames[1][0]), FRAME_SIZE);

// }

// void split_data_card_2(const unsigned char *data, size_t data_len) {
//     size_t i = 0;
//     size_t frame_start = 0;
//     size_t frame_len = 0;
//     size_t frame_count = 0;
	
//     unsigned char frames[6][FRAME_SIZE + 2] = {0};  

//     while (i < data_len) 
// 	{
//         if ((i + 1 < data_len && data[i] == END_MARKER) && (data[i + 1] == END_MARKER_NEXT)) 
// 		{  
//             frame_len = i - frame_start;  
			
//             if (frame_len > FRAME_SIZE) 
// 			{
//                 size_t extra_data_len = frame_len - FRAME_SIZE;
//                 size_t split_frame = 0;

//                 for (size_t j = frame_start; j < frame_start + FRAME_SIZE; ++j) {
//                     frames[frame_count][split_frame++] = data[j];
//                 }
				
//                 frame_count++;
//                 frame_start = i + 2;  
//             } 
// 			else 
// 			{ 
//                 size_t split_frame = 0;
				
//                 for (size_t j = frame_start; j < frame_start + frame_len; ++j) {
//                     frames[frame_count][split_frame++] = data[j];
//                 }
				
//                 frame_count++;
//             }
			
//             frame_start = i + 2;  
//             i += 2;  
			
//         } 
// 		else 
// 		{
//             ++i;
//         }
//     }
// 	csi_ahmi_SetStrRegValue(1009, (char *)(&frames[0][0]), FRAME_SIZE);
// 	csi_ahmi_SetStrRegValue(1014, (char *)(&frames[1][0]), FRAME_SIZE);

// }


// int32_t RxDataParser(uint8_t* buf, uint16_t len)
// {
// 	uint16_t page_id=0;
// 	uint16_t page_falg=0;
// 	uint16_t widget_id=0;
// 	char string[512];
// 	const char *p = string;
// 	uint16_t str_len=0;
// 	uint16_t str_len_1=0;
// 	uint16_t str_len_2=0;
// 	uint8_t rebuf[9] = {0xEE,0xB1,0x00,0x00,0x01,0xFF,0xFC,0xFF,0xFF};
// 	uint8_t rebuf_b[14] = {0xEE,0xB1,0x11,0x00, 0x02, 0x00, 0x01, 0x10, 0x01, 0x01, 0xFF, 0xFC, 0xFF, 0xFF};
// 	uint8_t rebuf_c[15] = {0xEE,0xB1,0x10,0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFC, 0xFF, 0xFF};
// 	printf("RxDataParser:%d\r\n",len);
	
// 	//if(len>11)
// 	{
// 		if((buf[0]== 0xEE) && (buf[1]== 0xB1) && (buf[2]== 0x10))//EE B1 10
// 		{
// 			if((buf[len-4] == 0xFF)&&(buf[len-3] == 0xFC)&& (buf[len-2] == 0xFF)&&(buf[len-1] == 0xFF))//FF FC FF FF
// 			{
// 				page_id = (buf[3]<<8)|buf[4];
// 				widget_id = (buf[5]<<8)|buf[6];
// 				str_len = len-3-4-4;
// 				memcpy(string,buf+3+4,str_len);
// 				//printf("str:%s\r\n",string);
// 				printf("str:");
// 				for(int i=0;i<str_len;i++)
// 					printf("%02x ",string[i]);
// 				printf("\r\n");
// 				switch (page_id) {
//     case 0:
//         if (widget_id == 2) {
//             csi_ahmi_SetStrRegValue(1017, string, str_len);
//         }
//         break;
//     case 1:
//         if (widget_id == 6) {
//             csi_ahmi_SetStrRegValue(1051, string, str_len);
//         }
//         if (widget_id == 5) {
//             csi_ahmi_SetRegValue(2, 6); // set page
//             page_falg = 1;
//         }
//         break;
//     case 2:
//         if (widget_id == 6) {
//             split_data_card_2(string, str_len);
//         }
//         if (widget_id == 7) {
//             csi_ahmi_SetStrRegValue(1052, string, str_len);
//         }
//         if (widget_id == 5) {
//             csi_ahmi_SetRegValue(2, 6); // set page
//             page_falg = 1;
//         }
//         break;
//     case 3:
//         if (widget_id == 6) {
//             csi_ahmi_SetStrRegValue(1053, string, str_len);
//         }
//         if (widget_id == 5) {
//             csi_ahmi_SetRegValue(2, 6); // set page
//             page_falg = 1;
//         }
//         break;
//     case 4:
//         if (widget_id == 2) {
//             csi_ahmi_SetStrRegValue(1013, string, str_len);
//         }
//         if (widget_id == 3) {
//             csi_ahmi_SetStrRegValue(1054, string, str_len);
//         }
//         if (widget_id == 4) {
//             csi_ahmi_SetStrRegValue(1011, string, str_len);
//         }
//         if (widget_id == 5) {
//             csi_ahmi_SetRegValue(1061, buf[7]);
//         }
//         break;
//     case 5:
//         if (widget_id == 2) {
//             split_data_card(string, str_len);
//         }
//         if (widget_id == 5) {
//             csi_ahmi_SetStrRegValue(1055, string, str_len);
//         }
//         if (widget_id == 6) {
//             csi_ahmi_SetStrRegValue(1018, string, str_len);
//         }
//         if (widget_id == 3) {
//             split_data(string, str_len);
//         }
//         break;
//     case 6:
//         if (widget_id == 1 || widget_id == 2) {
//             csi_ahmi_SetRegValue(1058, buf[8]);
//             csi_ahmi_SetRegValue(1056, buf[10]);
//             csi_ahmi_SetRegValue(1057, (buf[10] + 1));
//         }
//         break;
//     default:
//         // 可选的默认分支
//         break;
// }
// 			}
// 		}
// 		else if((buf[0]== 0xEE) && (buf[1]== 0xB1) && (buf[2]== 0x01))//EE B1 01 获取页面
// 		{
// 			if((buf[len-4] == 0xFF)&&(buf[len-3] == 0xFC)&& (buf[len-2] == 0xFF)&&(buf[len-1] == 0xFF))
// 			{
// 				rebuf[2] = 0x01;
// 				rebuf[4] = (csi_ahmi_GetRegValue(2) - 1);
// 				COM_iUartAsynSend(rebuf,9);
// 			}
// 		}
// 		else if((buf[0]== 0xEE) && (buf[1]== 0xB1) && (buf[2]== 0x00))//EE B1 00 设置页面
// 		{
// 			if((buf[len-4] == 0xFF)&&(buf[len-3] == 0xFC)&& (buf[len-2] == 0xFF)&&(buf[len-1] == 0xFF))
// 			csi_ahmi_SetRegValue(2, buf[4]+1); //set page
// 		}
// 		else if((buf[0]== 0xEE) && (buf[1]== 0xB1) && (buf[2]== 0x11))//EE B1 11 读取按钮状态
// 		{
// 			if(buf[4] == 0x06)
// 			{
// 				rebuf_c[4] = 0x06;
// 				rebuf_c[6] = 0x01;
// 				rebuf_c[8] = csi_ahmi_GetRegValue(1058);
// 				rebuf_c[10] = csi_ahmi_GetRegValue(1056);
// 				COM_iUartAsynSend(rebuf_c, 15);
// 			}
// 			else{
// 			rebuf_b[4] = 0x05;
// 			rebuf_b[6] = 0x07;
// 			rebuf_b[9] = (csi_ahmi_GetRegValue(1035));
// 			COM_iUartAsynSend(rebuf_b, 14);
			
// 			}
// 		}
// 	}
// }
// #endif


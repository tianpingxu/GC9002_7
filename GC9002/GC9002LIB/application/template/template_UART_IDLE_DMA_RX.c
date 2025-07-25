#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx.h"
#include "core_rv32.h"
#include "template_def.h"
#include "csi_irq.h"

#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_UART_IDLE_DMA_RX_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#include "ringbuffer/ringbuffer.h"

#define TEST_USE_UPG_UART_DMA               (1U)
#define TEST_USE_UPG_UART_RINGBUFF          (1U)
#define TEST_DMA_HS                         DMA_HS_UART0_RX
#define UPG_UART_CHANNEL                    UART0
#define UPG_UARTx_IRQn                      UART0_IRQn
#define TEST_EVENT_UART_IDLE                0x80

#define UPG_RING_BUFF_SIZE                                  512

#define UPG_UART_HEAD_A_SIZE                                (1)
#define UPG_UART_HEAD_H_SIZE                                (1)

#define UPG_UART_HEAD_SIZE                                  (2)
#define UPG_UART_CTRL_SIZE                                  (2)
#define UPG_UART_CMD_SIZE                                   (2)
#define UPG_UART_CONTENT_SIZE                               (2)

#define UPG_UART_TAIL_SIZE                                  (2)

#define UPG_UART_OTHER_SIZE                                 (UPG_UART_HEAD_SIZE + UPG_UART_CTRL_SIZE + UPG_UART_TAIL_SIZE)

#define UPG_UART_HEAD_OFFSET                                (0)
#define UPG_UART_HEAD_A_OFFSET                              (UPG_UART_HEAD_OFFSET)
#define UPG_UART_HEAD_H_OFFSET                              (UPG_UART_HEAD_A_OFFSET + UPG_UART_HEAD_A_SIZE)

#define UPG_UART_CTRL_OFFSET                                (UPG_UART_HEAD_OFFSET + UPG_UART_HEAD_SIZE)
#define UPG_UART_CMD_OFFSET                                 (UPG_UART_CTRL_OFFSET + UPG_UART_CTRL_SIZE)
#define UPG_UART_CONTENT_OFFSET                             (UPG_UART_CMD_OFFSET + UPG_UART_CMD_SIZE)

/* Note: This Tail Offset calculated from CMD Offset, as the size contain CMD */
#define UPG_UART_TAIL_OFFSET(x)                             (UPG_UART_CMD_OFFSET + (x))

#define UPG_UART_DOWNLOAD_PARA_SIZE                         (4)

#define UPG_UART_DOWNLOAD_DATA_OFFSET                       (UPG_UART_DOWNLOAD_PARA_SIZE + UPG_UART_CMD_SIZE)


/***** Important: config the baudrate of UART *****/
#define UPG_UART_BAUD_RATE                                  (115200) /* currently not support high baudrate like 115200 */

/***** Important: config the max pure data length of one frame *****/
#define UPG_UART_DOWNLOAD_MAX_DATA_LEN                      (128)

/***** Important: config the receive buffer size used in ISR, for receiving one frame data from Upper computer *****/
#define UPG_UART_RECV_ISR_BUFFER_SIZE                       (128)

/***** Important: config the send buffer size used in both ISR and Task, for sending one frame data to Upper compute *****/
#define UPG_UART_SEND_BUFFER_SIZE                           (128)

#define UPG_UART_DOWNLOAD_MAX_DATA_LEN_WITH_PARA            (UPG_UART_DOWNLOAD_MAX_DATA_LEN + UPG_UART_DOWNLOAD_PARA_SIZE)

/* Max content length is download CMD: CMD size + Offset + n(data) */
#define UPG_UART_DOWNLOAD_MAX_CONTENT_LEN                   (UPG_UART_CMD_SIZE + UPG_UART_DOWNLOAD_MAX_DATA_LEN_WITH_PARA)

typedef enum
{
    UPG_enUartRecvSM_Idle = 0,
    UPG_enUartRecvSM_Head_A,
    UPG_enUartRecvSM_Head_H,
    UPG_enUartRecvSM_Ctrl,
    UPG_enUartRecvSM_CmdVsContent,
    UPG_enUartRecvSM_Tail,
    UPG_enUartRecvSM_Invalid
	
}UPG_tenUartRecvSM;

typedef union
{
    volatile uint16_t u16Val;
    struct{
        uint16_t len    : 11;
        uint16_t res    : 4;
        uint16_t crcEn  : 1;

    }bits;
}UPG_tunUartCrtl, *UPG_tunUartCrtlPtr;

typedef struct 
{	

    volatile UPG_tenUartRecvSM      enUartRecvSm;                                   /* current Receive State Machine */
    uint16_t                        u16RecvCmdVsContentLen;                         /* Received CMD and Content length */
    uint16_t                        u16CurCmd;                                      /* Current CMD , used for timeout reply */
    uint16_t                        u16LastOffset;                                  /* Saving last Offset */
	uint8_t*                        pu8RecvBufStartAddress;                         /* start address of Receive buffer */
    uint8_t*                        pu8SendBufStartAddress;                         /* start address of Send buffer */
}UPG_tstUartWorkBuf, *UPG_tstUartWorkBufPtr;

typedef struct 
{	
    uint32_t                        u32Addr;                                        /* Flash Address to download */
    uint32_t                        u32FileSize;                                    /* File size */
}UPG_tstUartContentReqDownload;

typedef struct 
{	
    uint32_t                        u32Offset;                                      /* Offset of the start of burn Flash Address */
    uint8_t                         au8Data[UPG_UART_DOWNLOAD_MAX_DATA_LEN];        /* Upgrade data */
}UPG_tstUartContentDownload;

typedef struct
{
    uint16_t                        u16Content;                                        /* Flash Address to download */                              /* File size */
}UPG_tstUartContentu16;

typedef struct
{
    uint8_t                         au8Data[UPG_UART_DOWNLOAD_MAX_DATA_LEN];        /* Upgrade data */                            /* File size */
}UPG_tstUartContentu8arry;

typedef union
{
    //UPG_tstUartContentReqDownload   stContentReqDownload;
    //UPG_tstUartContentDownload      stContentDownload;
    UPG_tstUartContentu16           stContentu16;
    UPG_tstUartContentu8arry        stContentu8arry;
    
}UPG_tunUartContent;

typedef struct 
{	
    UPG_tunUartCrtl                 unCtrl;                                         /* Ctrl word */
    uint16_t                        u16Cmd;                                         /* Cmd */
	UPG_tunUartContent              unUartContent;                                  /* Cmd Content */

}UPG_tstUartCmd, *UPG_tstUartCmdPtr;

typedef union
{
    uint32_t                        u32MaxPackageSize;                              /* CMD 0x0080 reply the Max package(pure data) size */
    uint32_t                        u32Res;                                         /* reply the process result */
}UPG_tunUartRplContent;

typedef struct
{
    UPG_tunUartCrtl                 unCtrl;                                         /* Ctrl word */
    uint16_t                        u16RplCmd;                                      /* Reply Cmd */
    UPG_tunUartRplContent           unRplContent;                                   /* reply Content */

}UPG_tstUartRpl, *UPG_tstUartRplPtr;
static volatile uint8_t upg_r_buff_index = 0;
static volatile uint8_t upg_r_buff_index_last = 0;

#if(defined TEST_USE_UPG_UART_DMA && TEST_USE_UPG_UART_DMA == 1U)
static DMA_HandleTypeDef rx_hdma =
{
    .Instance = DMA,
};
#endif

static xTaskHandle UartRxTaskHandle = NULL;
static QueueHandle_t UartRxQueue = NULL;

static UART_HandleTypeDef huart_upg = {.Instance = UPG_UART_CHANNEL,};
UPG_tstUartWorkBuf UPG_stUartWkbf;
static UPG_tstUartWorkBufPtr UPG_pstUartWkbf;
/* UART Receivd Buffer used in ISR */
static __attribute__ ((aligned(4))) uint8_t UPG_au8UartRecvBuf[UPG_UART_RECV_ISR_BUFFER_SIZE];
/* UART Send Buffer used in ISR & Task */
static __attribute__ ((aligned(4))) uint8_t UPG_au8UartSendBuf[UPG_UART_SEND_BUFFER_SIZE];

#if(defined TEST_USE_UPG_UART_DMA && TEST_USE_UPG_UART_DMA == 1U)
static __attribute__ ((aligned(64))) uint8_t UPG_au8UartDMARecvBuf[2][UPG_UART_RECV_ISR_BUFFER_SIZE];
#endif
struct ringbuffer *rb_upg;
static EventGroupHandle_t UartRxIdleEvent;


#if(defined TEST_USE_UPG_UART_DMA && TEST_USE_UPG_UART_DMA == 1U)
static UPG_tstUartCmd UPG_stUartCmdDma;
#endif

#ifndef CONFIG_KERNEL_NONE
#include <csi_kernel.h>
#endif

void UPG_HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    if(huart->Instance == UART0)
    {
        /**UART0 GPIO Configuration
         * PB14      ------> UART0_RX
         * PB15      ------> UART0_TX
         * */
        GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF5;
        GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
	else if(huart->Instance == UART1)
	{
        /**UART1 GPIO Configuration
         * PA2      ------> UART1_RX
         * PA3      ------> UART1_TX
         * */
		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF4;
		GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//		GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF2;
//		GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
//		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
    else if(huart->Instance == UART2)
    {
        /**UART0 GPIO Configuration
         * PA4      ------> UART2_RX
         * PA5      ------> UART2_TX
         * */
        GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF4;
        GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}
void UPG_HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF0;
    if(huart->Instance == UART0)
    {
        GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
	else if(huart->Instance == UART1)
	{
		GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
    else if(huart->Instance == UART2)
    {
        GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void UPG_UART_IRQHandler(void)
{
    CSI_INTRPT_ENTER();
	HAL_UART_IRQHandler(&huart_upg);
    CSI_INTRPT_EXIT();
}
#else
__attribute__((used, section(".flashdriver"), interrupt ("machine"))) static void UPG_UART_IRQHandler(void)
{
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
    
//	HAL_UART_IRQHandler(&huart_upg);
    
    uint32_t isrflags = READ_REG(huart_upg.Instance->SR);
//    printf("%X#\n", isrflags);
    *((volatile unsigned int *)(GPIOB_BASE+4*4)) = 1;
    if(isrflags & (uint32_t)(UART_SR_IDLE))
    {
        if(huart_upg.IDLECallback)
            huart_upg.IDLECallback(&huart_upg);
    }
    else if(isrflags & (uint32_t)(UART_SR_PE | UART_SR_FE | UART_SR_ORE | UART_SR_NE))
    {
        if(huart_upg.ErrorCallback)
            huart_upg.ErrorCallback(&huart_upg);
    }
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
}
#endif

void TEST_UART_DMA_RxCpltCallback(DMA_HandleTypeDef *hdma);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_DMA_IRQHandler(void)
#else
__attribute__((used, section(".flashdriver"), interrupt ("machine"))) static void TEMPLATE_DMA_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif

//	HAL_DMA_IRQHandler(huart_upg.hdmarx);

    *((volatile unsigned int *)(GPIOB_BASE+4*5)) = 1;
    TEST_UART_DMA_RxCpltCallback(huart_upg.hdmarx);

#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}

static volatile uint32_t rb_index = 0;
static volatile uint16_t rb_length = 0;

__attribute__((used, section(".flashdriver"))) void TEST_UART_DMA_RxCpltCallback(DMA_HandleTypeDef *hdma)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult = pdFAIL;
    uint64_t tempreg = 0;
    upg_r_buff_index_last = upg_r_buff_index;
    upg_r_buff_index ^= 1;
    
    UART_HandleTypeDef *huart = (UART_HandleTypeDef *)((DMA_HandleTypeDef *)hdma)->Parent;
    CLEAR_BIT(huart->Instance->CR3, UART_CR3_DMAR);
    
    __HAL_DMA_CHX_DISABLE(hdma, hdma->Init.mChannel);
    /* Check if the DMA Stream is effectively disabled */
    while(__HAL_DMA_CHANNEL_IS_ENABLE(hdma));
    
    /* set destination address. */
    hdma->Instance->CHx[hdma->Init.mChannel].DAR.reg_u64val = (uint64_t)(uint32_t)&UPG_au8UartDMARecvBuf[upg_r_buff_index][0];

    /* set transfer block size. Block Transfer Size = BLOCK_TS */
    tempreg = hdma->Instance->CHx[hdma->Init.mChannel].CTL.reg_u64val;
    ((union _DMA_CTL *)&tempreg)->bit.BLOCK_TS = hdma->Init.mBlockSize;
    hdma->Instance->CHx[hdma->Init.mChannel].CTL.reg_u64val = tempreg;

    /* Disable all the transfer interrupts */
    __HAL_DMA_CHX_CLEAR_IT(hdma, hdma->Init.mChannel, DMA_INT_ALL);
    /* Enable the Peripheral */
    __HAL_DMA_CHX_ENABLE(hdma, hdma->Init.mChannel);
    
    __HAL_UART_CLEAR_OREFLAG(huart);
    SET_BIT(huart->Instance->CR3, UART_CR3_DMAR);
    *((volatile unsigned int *)(GPIOB_BASE+4*5)) = 0;
    
    ringbuffer_in(rb_upg, &UPG_au8UartDMARecvBuf[upg_r_buff_index_last][0], hdma->Init.mBlockSize);

    EventBits_t bit = xEventGroupGetBitsFromISR(UartRxIdleEvent);
    if((bit & TEST_EVENT_UART_IDLE) != TEST_EVENT_UART_IDLE)
    {
        xResult = xEventGroupSetBitsFromISR(UartRxIdleEvent, TEST_EVENT_UART_IDLE, &xHigherPriorityTaskWoken);
        // Was the message posted successfully?
        if(xResult == pdPASS)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}
__attribute__((used, section(".flashdriver"))) void TEST_HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
//    printf("%d#\n", huart->Instance->SR);
    __HAL_UART_CLEAR_OREFLAG(huart);
    __HAL_UART_CLEAR_NEFLAG(huart);
    __HAL_UART_CLEAR_FEFLAG(huart);
    if(huart == &huart_upg)
    {
        ringbuffer_reset(rb_upg);
        UPG_pstUartWkbf->enUartRecvSm = UPG_enUartRecvSM_Head_A;
        rb_length = 2;
        rb_index = 0;
        
        upg_r_buff_index_last = upg_r_buff_index;
        upg_r_buff_index ^= 1;      
        
        CLEAR_BIT(huart->Instance->CR3, UART_CR3_DMAR);
        uint64_t tempreg = 0;
        /* Disable the stream */
        __HAL_DMA_CHX_DISABLE(huart->hdmarx, huart->hdmarx->Init.mChannel);
        /* Check if the DMA Stream is effectively disabled */
        while(__HAL_DMA_CHANNEL_IS_ENABLE(huart->hdmarx));
        
        /* set destination address. */
        huart->hdmarx->Instance->CHx[huart->hdmarx->Init.mChannel].DAR.reg_u64val = (uint64_t)(uint32_t)&UPG_au8UartDMARecvBuf[upg_r_buff_index][0];
        
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
__attribute__((used, section(".flashdriver"))) void TEST_HAL_UART_IDLECallback(UART_HandleTypeDef *huart)
{
    uint32_t count = 0;
    uint64_t tempreg = 0;
    
    count = __HAL_DMA_CHX_GET_COUNTER(huart->hdmarx);
    if(count == 0)
    {
        __HAL_UART_CLEAR_IDLEFLAG(huart);
        return;
    }
    upg_r_buff_index_last = upg_r_buff_index;
    upg_r_buff_index ^= 1;

    CLEAR_BIT(huart->Instance->CR3, UART_CR3_DMAR);
    
    __HAL_DMA_CHX_DISABLE(huart->hdmarx, huart->hdmarx->Init.mChannel);
    /* Check if the DMA Stream is effectively disabled */
    while(__HAL_DMA_CHANNEL_IS_ENABLE(huart->hdmarx));
    
    /* set destination address. */
    huart->hdmarx->Instance->CHx[huart->hdmarx->Init.mChannel].DAR.reg_u64val = (uint64_t)(uint32_t)&UPG_au8UartDMARecvBuf[upg_r_buff_index][0];
    
    /* set transfer block size. Block Transfer Size = BLOCK_TS */
    tempreg = huart->hdmarx->Instance->CHx[huart->hdmarx->Init.mChannel].CTL.reg_u64val;
    ((union _DMA_CTL *)&tempreg)->bit.BLOCK_TS = huart->hdmarx->Init.mBlockSize;
    huart->hdmarx->Instance->CHx[huart->hdmarx->Init.mChannel].CTL.reg_u64val = tempreg;
    
    /* Enable the Peripheral */
    __HAL_DMA_CHX_ENABLE(huart->hdmarx, huart->hdmarx->Init.mChannel);
    
    __HAL_UART_CLEAR_FEFLAG(huart);
    /* Enable the UART Parity Error Interrupt */
    SET_BIT(huart->Instance->CR3, UART_CR3_DMAR);

    *((volatile unsigned int *)(GPIOB_BASE+4*4)) = 0;
    
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult = pdFAIL;
    uint32_t temp_index = 0;
    uint32_t length = 0;

    if(count <= UPG_UART_RECV_ISR_BUFFER_SIZE)
    {
        length = ringbuffer_in(rb_upg, &UPG_au8UartDMARecvBuf[upg_r_buff_index_last][0], count);
        if(length == count)
        {
            xResult = xEventGroupSetBitsFromISR(UartRxIdleEvent, TEST_EVENT_UART_IDLE, &xHigherPriorityTaskWoken);
            // Was the message posted successfully?
            if(xResult == pdPASS)
            {
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
        }
    }
}

static void DMA_Init_Test(void)
{
    HAL_DMA_StructInit(&(rx_hdma.Init));
	__HAL_LINKDMA(&huart_upg, hdmarx, rx_hdma);
    rx_hdma.Instance = DMA;
    rx_hdma.Init.mChannel = DMA_CHANNEL1;
    rx_hdma.Init.mS_Address = 0;/*!< Specifies Source Address.             */
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
    rx_hdma.Parent = &huart_upg;
    HAL_DMA_Init(&rx_hdma);
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(DMA_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEMPLATE_DMA_IRQHandler);
#else
    csi_clic_register_irq(DMA_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 1, TEMPLATE_DMA_IRQHandler);
#endif

    CLEAR_BIT(huart_upg.Instance->CR3, UART_CR3_EIE);
    __HAL_UART_CLEAR_IDLEFLAG(&(huart_upg));
    __HAL_UART_ENABLE_IT(&huart_upg, UART_IT_IDLE);
}


void UPG_UART_vUartHwInit(void)
{
    /* config the UART */
    huart_upg.Instance = UPG_UART_CHANNEL;
    HAL_UART_StructInit(&(huart_upg.Init));    
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(UPG_UARTx_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 2, UPG_UART_IRQHandler);
#else
    csi_clic_register_irq(UPG_UARTx_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 2, UPG_UART_IRQHandler);
#endif
    HAL_UART_RegisterCallback(&huart_upg, HAL_UART_MSPINIT_CB_ID, UPG_HAL_UART_MspInit);
    HAL_UART_Init(&huart_upg);
    HAL_UART_RegisterCallback(&huart_upg, HAL_UART_ERROR_CB_ID, TEST_HAL_UART_ErrorCallback);
    HAL_UART_RegisterCallback(&huart_upg, HAL_UART_IDLE_CB_ID, TEST_HAL_UART_IDLECallback);
    DMA_Init_Test();
}

void UPG_UART_vUartRxMainProcess(void * pvParameters);
void UPG_UART_vParseCmd(UPG_tstUartCmdPtr pCmd, UART_HandleTypeDef *huart)
{
    printf("R:cmd=[%04X] \n", pCmd->u16Cmd);
}
void UPG_UART_vInit(void)
{
    portBASE_TYPE xReturn = pdTRUE;
    
    UPG_pstUartWkbf = &UPG_stUartWkbf;
    memset(UPG_pstUartWkbf, 0, sizeof(UPG_tstUartWorkBuf));
    UPG_pstUartWkbf->pu8RecvBufStartAddress = UPG_au8UartRecvBuf;
    UPG_pstUartWkbf->pu8SendBufStartAddress = UPG_au8UartSendBuf;
    
    UPG_UART_vUartHwInit();
    rb_upg = ringbuffer_create(UPG_RING_BUFF_SIZE);
    if(rb_upg == NULL)
    {
        printf("rb_upg crate failed\n");
    }
    UartRxIdleEvent = xEventGroupCreate();
    if(UartRxIdleEvent == NULL)
    {
        printf("UartRxIdleEvent crate failed\n");
    }
	HAL_UART_Receive_DMA(&huart_upg, &UPG_au8UartDMARecvBuf[0][0], (UPG_UART_RECV_ISR_BUFFER_SIZE <= DMA_CHANNEL_1_BLOCK_TS_MAX ? UPG_UART_RECV_ISR_BUFFER_SIZE : DMA_CHANNEL_1_BLOCK_TS_MAX));
    
    xReturn = xTaskCreate(UPG_UART_vUartRxMainProcess, "UartRxTask", configMINIMAL_STACK_SIZE*4, NULL, 6, &UartRxTaskHandle);
    if (xReturn != pdTRUE || UartRxTaskHandle == NULL)
    {
        printf("UartRxTask crate failed\n");
        return;
    }
}

void UPG_UART_vUartRxMainProcess(void * pvParameters)
{
    EventBits_t evt_bits = 0;
    volatile uint32_t size = 0;
    uint8_t buffer = 0;
    UPG_tstUartWorkBufPtr tempWkbf = UPG_pstUartWkbf;
    tempWkbf->enUartRecvSm = UPG_enUartRecvSM_Head_A;
    rb_length = 2;
    rb_index = 0;
	BaseType_t xStatus; 	
	uint32_t tmp;
    UPG_tstUartCmd stUartCmd;
	while(1)
	{
        evt_bits = xEventGroupWaitBits(UartRxIdleEvent, TEST_EVENT_UART_IDLE,     //等待的event标志位
                                        pdTRUE,     //退出xEventGroupWaitBits之前，event标志位是否清除，若不清除，那么当3个标志位一起到来后，xEventGroupWaitBits持续激活态
                                        pdFALSE,    //event事件之间的关系,pdTRUE: &, pdFALSE：|
                                        portMAX_DELAY);
        if((evt_bits & TEST_EVENT_UART_IDLE) == TEST_EVENT_UART_IDLE)
        {
            #if(defined TEST_DEBUG_DMA && TEST_DEBUG_DMA == 1U)
            // PA7
            *(volatile unsigned int *)0x4000A01C = 1;
            printf("%d-%d", tempWkbf->enUartRecvSm, ringbuffer_len(rb_upg));
            #endif
            while (ringbuffer_len(rb_upg) > 0)
            {
                switch (tempWkbf->enUartRecvSm)
                {
                    case UPG_enUartRecvSM_Head_A:
                    {
                        size = ringbuffer_out(rb_upg, &tempWkbf->pu8RecvBufStartAddress[rb_index], rb_length);
                        if(size == rb_length) {
                            /* If 'A' has received, next step:prepare to receive 'H' */
                            if(tempWkbf->pu8RecvBufStartAddress[rb_index] == (uint8_t)'A' && tempWkbf->pu8RecvBufStartAddress[rb_index+1] == (uint8_t)'H')
                            {
                                tempWkbf->enUartRecvSm = UPG_enUartRecvSM_Ctrl;
                                rb_length = 2;
                                rb_index += 2;
                            } else {
                                #if(defined TEST_DEBUG_DMA && TEST_DEBUG_DMA == 1U)
                                printf("EA");
                                #endif
                                tempWkbf->enUartRecvSm = UPG_enUartRecvSM_Head_A;
                                rb_length = 2;
                                rb_index = 0;
                            }
                        } else if(size > 0 && size < rb_length){
                            #if(defined TEST_DEBUG_DMA && TEST_DEBUG_DMA == 1U)
                            printf("AES");
                            #endif
                            rb_length -= size;
                            rb_index += size;
                        }
                    }break;
                    case UPG_enUartRecvSM_Ctrl:
                    {
                        size = ringbuffer_out(rb_upg, &tempWkbf->pu8RecvBufStartAddress[rb_index], rb_length);
                        if(size == rb_length) {
                            /*Note: MSB in receive buffer, need to be cast to LSB */
                            rb_length = ReadUint16_BigEndian(&tempWkbf->pu8RecvBufStartAddress[rb_index]);
                            /* Ignore the CRC Enable bit */
                            tempWkbf->u16RecvCmdVsContentLen = ((UPG_tunUartCrtlPtr)(&rb_length))->bits.len;
                            /* if max content length is valid , enter next SM: UPG_enUartRecvSM_CmdVsContent; else restart from head */
                            if(tempWkbf->u16RecvCmdVsContentLen <= UPG_UART_DOWNLOAD_MAX_CONTENT_LEN)
                            {
                                tempWkbf->enUartRecvSm = UPG_enUartRecvSM_CmdVsContent;
                                rb_length = tempWkbf->u16RecvCmdVsContentLen + 2;// 帧尾 "MI"
                                rb_index += 2;
                            } else {
                                #if(defined TEST_DEBUG_DMA && TEST_DEBUG_DMA == 1U)
                                printf("EC");
                                #endif
                                tempWkbf->enUartRecvSm = UPG_enUartRecvSM_Head_A;
                                rb_length = 2;
                                rb_index = 0;
                            }
                        } else if(size > 0 && size < rb_length){
                            #if(defined TEST_DEBUG_DMA && TEST_DEBUG_DMA == 1U)
                            printf("CES");
                            #endif
                            rb_length -= size;
                            rb_index += size;
                        }
                    }break;
                    case UPG_enUartRecvSM_CmdVsContent:
                    {
                        size = ringbuffer_out(rb_upg, &tempWkbf->pu8RecvBufStartAddress[rb_index], rb_length);
                        if(size == rb_length)
                        {
                            rb_index += size;
                            if(tempWkbf->pu8RecvBufStartAddress[rb_index-2] == (uint8_t)'M' 
                            && tempWkbf->pu8RecvBufStartAddress[rb_index-1] == (uint8_t)'I')
                            {
                                /* Send Queen to task from ISR */
                                /*Note: MSB in receive buffer, need to be cast to LSB */
                                UPG_stUartCmdDma.unCtrl.u16Val = ReadUint16_BigEndian(&tempWkbf->pu8RecvBufStartAddress[UPG_UART_CTRL_OFFSET]);
                                UPG_stUartCmdDma.u16Cmd = ReadUint16_BigEndian(&tempWkbf->pu8RecvBufStartAddress[UPG_UART_CMD_OFFSET]);
                                
                                if(tempWkbf->u16RecvCmdVsContentLen - UPG_UART_CMD_SIZE > 0)
                                {
                                    memcpy(&UPG_stUartCmdDma.unUartContent, &tempWkbf->pu8RecvBufStartAddress[UPG_UART_CONTENT_OFFSET], tempWkbf->u16RecvCmdVsContentLen - UPG_UART_CMD_SIZE);
                                }
                                
                                UPG_UART_vParseCmd(&UPG_stUartCmdDma, &huart_upg);
                                
                                tempWkbf->enUartRecvSm = UPG_enUartRecvSM_Head_A;
                                rb_length = 2;
                                rb_index = 0;
                            } else {
                                #if(defined TEST_DEBUG_DMA && TEST_DEBUG_DMA == 1U)
                                printf("EI");
                                #endif
                                tempWkbf->enUartRecvSm = UPG_enUartRecvSM_Head_A;
                                rb_length = 2;
                                rb_index = 0;
                            }
                        } else if(size > 0 && size < rb_length){
                            #if(defined TEST_DEBUG_DMA && TEST_DEBUG_DMA == 1U)
                            printf("DES");
                            #endif
                            rb_length -= size;
                            rb_index += size;
                        }
                    }break;
                    default: 
                        #if(defined TEST_DEBUG_DMA && TEST_DEBUG_DMA == 1U)
                        printf("EQ");
                        #endif
                    break;
                }
            }
        }
	}
}

int main(void) 
{
    printf("hello uart idle dma rx test...\n");
	/* Init Uart. */
	UPG_UART_vInit();
	/* Start scheduler tasks for System. */
    printf("task start...\n");
    vTaskStartScheduler(); 
	while (1);
}
#endif /* _TEMPLATE_UART_IDLE_DMA_RX_ */
#endif
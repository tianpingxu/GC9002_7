#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "upgrade_c1.h"



static xTaskHandle UartRxTaskHandle;
static QueueHandle_t UartRxQueue;

static UART_HandleTypeDef huart;
static UPG_tstUartWorkBuf UPG_stUartWkbf;
static UPG_tstUartWorkBufPtr UPG_pstUartWkbf;

/* UART Receivd Buffer used in ISR */
static __attribute__ ((aligned(4))) uint8_t UPG_au8UartRecvBuf[UPG_UART_RECV_ISR_BUFFER_SIZE];

/* UART Send Buffer used in ISR & Task */
static __attribute__ ((aligned(4))) uint8_t UPG_au8UartSendBuf[UPG_UART_SEND_BUFFER_SIZE];

/* Reduce the ISR stack loader , to define this global variable struct */
static UPG_tstUartCmd UPG_stUartCmdIsr;

#ifndef CONFIG_KERNEL_NONE
#include <csi_kernel.h>
#endif


#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void UPG_UART_IRQHandler(void)
#else
static ATTRIBUTE_ISR void UPG_UART_IRQHandler(void)
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


void UPG_UART_vMspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    uint32_t rxPin, txPin;
    GPIO_TypeDef* rxPort;
    GPIO_TypeDef* txPort;
    bool errExist = FALSE_0;
    
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    rxPort = SYN_GET_UART_RX_PORT;
    rxPin = SYN_GET_UART_RX_PIN;
    txPort = SYN_GET_UART_TX_PORT;
    txPin = SYN_GET_UART_TX_PIN;

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
            errExist = TRUE_1;
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
            errExist = TRUE_1;
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
            errExist = TRUE_1;
        }
	}

    if(FALSE_0 == errExist)
    {
        GPIO_InitStruct.Pin = rxPin | txPin;
        HAL_GPIO_Init(rxPort, &GPIO_InitStruct);
    }
    else
    {
        /* Record error */
        SYN_ERR_LOG(EM_enErrUartInitFail, EM_enErrLevelFatal);
        /* Depend on FAE request, initialization failing will hold on here */
        while(1);
    }
    
}


void UPG_UART_vUartHwInit(void)
{
    IRQn_Type iqr;
    uint32_t sysIqr;
    
    sysIqr = UPG_UART_ENTER_CRITICAL();
    /* config the UART */
    huart.Instance = SYN_GET_UART_CHANEL;
    //HAL_UART_StructInit(&(huart.Init));
    /* UART_InitStruct members default value */
    huart.Init.BaudRate = SYN_GET_UART_BAUD;
    huart.Init.WordLength = UART_WORDLENGTH_8B;
    huart.Init.StopBits = UART_STOPBITS_1;
    huart.Init.Parity = UART_PARITY_NONE ;
    huart.Init.Mode = UART_MODE_TX_RX;
    huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;

    if(UART0 == huart.Instance)
    {
        iqr = UART0_IRQn;
    }
    else if(UART1 == huart.Instance)
    {
        iqr = UART1_IRQn;
    }
    else
    {
        iqr = UART2_IRQn;
    }
    
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(iqr, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 2, UPG_UART_IRQHandler);
#else
    csi_clic_register_irq(iqr, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 2, UPG_UART_IRQHandler);
#endif
    huart.MspInitCallback = UPG_UART_vMspInit;
    HAL_UART_Init(&huart);
    UPG_stUartWkbf.bUartInited = TRUE;
    UPG_UART_EXIT_CRITICAL(sysIqr);
}


void UPG_UART_vUartHwDeInit(void)
{
    if(UPG_stUartWkbf.bUartInited)
    {
        HAL_UART_DeInit(&huart);
        UPG_stUartWkbf.bUartInited = FALSE;
    }
}


void UPG_UART_vInit(void)
{
    portBASE_TYPE xReturn;
    
    UPG_pstUartWkbf = &UPG_stUartWkbf;
    memset(UPG_pstUartWkbf, 0, sizeof(UPG_tstUartWorkBuf));
    UPG_pstUartWkbf->pu8RecvBufStartAddress = UPG_au8UartRecvBuf;
    UPG_pstUartWkbf->pu8SendBufStartAddress = UPG_au8UartSendBuf;
    
    UPG_UART_vUartHwInit();
    UPG_UART_vStartReceive();
    
    UartRxQueue = xQueueCreate(UART_RX_QUEUE_SIZE, sizeof(UPG_tstUartCmd));
    
    
    xReturn = xTaskCreate(UPG_UART_vUartRxMainProcess, "UartRxTask", configMINIMAL_STACK_SIZE * 3, NULL, 4, &UartRxTaskHandle);
    if (xReturn != pdTRUE)
    {
        /* Record error */
        SYN_ERR_LOG(EM_enCreateTaskFail, EM_enErrLevelFatal);
        return;
    }

}


void UPG_UART_vStartReceive(void)
{
    HAL_UART_RxCpltCallback(&huart);
}


void UPG_UART_vUartRxMainProcess(void * pvParameters)
{
	BaseType_t xStatus; 	
	int32_t tmp;
    UPG_tstUartCmd stUartCmd;

    
	for(;;)
	{	
		xStatus = xQueueReceive(UartRxQueue, (void*)&stUartCmd, portMAX_DELAY);
		if(xStatus != pdPASS)
		{
			return;
		}

        UPG_UART_vParseCmd(&stUartCmd);
	}
}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    __HAL_UART_CLEAR_OREFLAG(huart);
    huart->gState = HAL_UART_STATE_READY;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    BaseType_t res;
    uint16_t length;
    
    switch (UPG_pstUartWkbf->enUartRecvSm)
    {
        case UPG_enUartRecvSM_Idle:
            /* Receive Frame Head section */
            HAL_UART_Receive_IT(huart, UPG_pstUartWkbf->pu8RecvBufStartAddress, UPG_UART_HEAD_A_SIZE);
            UPG_pstUartWkbf->enUartRecvSm = UPG_enUartRecvSM_Head_A;
            break;

        case UPG_enUartRecvSM_Head_A:
            /* If 'A' has received, next step:prepare to receive 'H' */
            if(UPG_pstUartWkbf->pu8RecvBufStartAddress[UPG_UART_HEAD_A_OFFSET] == (uint8_t)'A')
            {
                /* prepare to receive 'H' */
                HAL_UART_Receive_IT(huart, UPG_pstUartWkbf->pu8RecvBufStartAddress + UPG_UART_HEAD_H_OFFSET, UPG_UART_HEAD_H_SIZE);
                UPG_pstUartWkbf->enUartRecvSm = UPG_enUartRecvSM_Head_H;
            }
            else
            {
                HAL_UART_Receive_IT(huart, UPG_pstUartWkbf->pu8RecvBufStartAddress, UPG_UART_HEAD_A_SIZE);
            }
            break;
            
        case UPG_enUartRecvSM_Head_H:
            /* According to received 2 bytes , if it is Frame Head, then enter next step, else repeat receive until Head appear */
            if(UPG_pstUartWkbf->pu8RecvBufStartAddress[UPG_UART_HEAD_H_OFFSET] == (uint8_t)'H')
            {
                /* Receive Frame Ctrl section */
                HAL_UART_Receive_IT(huart, UPG_pstUartWkbf->pu8RecvBufStartAddress + UPG_UART_CTRL_OFFSET, UPG_UART_CTRL_SIZE);
                UPG_pstUartWkbf->enUartRecvSm = UPG_enUartRecvSM_Ctrl;
            }
            else
            {
                HAL_UART_Receive_IT(huart, UPG_pstUartWkbf->pu8RecvBufStartAddress, UPG_UART_HEAD_A_SIZE);
                UPG_pstUartWkbf->enUartRecvSm = UPG_enUartRecvSM_Head_A;
            }
            break;

        case UPG_enUartRecvSM_Ctrl:
            /*Note: MSB in receive buffer, need to be cast to LSB */
            length = *(uint16_t*)(&UPG_pstUartWkbf->pu8RecvBufStartAddress[UPG_UART_CTRL_OFFSET]);
            length = SWAP_UINT16(length);
        
            /* Ignore the CRC Enable bit */
            UPG_pstUartWkbf->u16RecvCmdVsContentLen = ((UPG_tunUartCrtlPtr)(&length))->bits.len;
            /* if max content length is valid , enter next SM: UPG_enUartRecvSM_CmdVsContent; else restart from head */
            if(UPG_pstUartWkbf->u16RecvCmdVsContentLen <= UPG_UART_DOWNLOAD_MAX_CONTENT_LEN)
            {
                /* Receive Frame Cmd and Content section */
                HAL_UART_Receive_IT(huart, UPG_pstUartWkbf->pu8RecvBufStartAddress + UPG_UART_CMD_OFFSET, UPG_pstUartWkbf->u16RecvCmdVsContentLen);
                UPG_pstUartWkbf->enUartRecvSm = UPG_enUartRecvSM_CmdVsContent;
            }
            else
            {
                HAL_UART_Receive_IT(huart, UPG_pstUartWkbf->pu8RecvBufStartAddress, UPG_UART_HEAD_A_SIZE);
                UPG_pstUartWkbf->enUartRecvSm = UPG_enUartRecvSM_Head_A;
            }
            break;

        case UPG_enUartRecvSM_CmdVsContent:
            HAL_UART_Receive_IT(huart, UPG_pstUartWkbf->pu8RecvBufStartAddress + UPG_UART_TAIL_OFFSET(UPG_pstUartWkbf->u16RecvCmdVsContentLen), UPG_UART_TAIL_SIZE);
            UPG_pstUartWkbf->enUartRecvSm = UPG_enUartRecvSM_Tail;
            break;

        case UPG_enUartRecvSM_Tail:
            if(UPG_pstUartWkbf->pu8RecvBufStartAddress[UPG_UART_TAIL_OFFSET(UPG_pstUartWkbf->u16RecvCmdVsContentLen)] == (uint8_t)'M' 
                && UPG_pstUartWkbf->pu8RecvBufStartAddress[UPG_UART_TAIL_OFFSET(UPG_pstUartWkbf->u16RecvCmdVsContentLen) + 1] == (uint8_t)'I')
            {
                /* Send Queen to task from ISR */
                /*Note: MSB in receive buffer, need to be cast to LSB */
                UPG_stUartCmdIsr.unCtrl.u16Val = *((uint16_t*)(UPG_pstUartWkbf->pu8RecvBufStartAddress + UPG_UART_CTRL_OFFSET));
                UPG_stUartCmdIsr.unCtrl.u16Val = SWAP_UINT16(UPG_stUartCmdIsr.unCtrl.u16Val);
                
                UPG_stUartCmdIsr.u16Cmd = *((uint16_t*)(UPG_pstUartWkbf->pu8RecvBufStartAddress + UPG_UART_CMD_OFFSET));
                UPG_stUartCmdIsr.u16Cmd = SWAP_UINT16(UPG_stUartCmdIsr.u16Cmd);
                
                if(UPG_pstUartWkbf->u16RecvCmdVsContentLen - UPG_UART_CMD_SIZE > 0)
                {
                    memcpy(&UPG_stUartCmdIsr.unUartContent, UPG_pstUartWkbf->pu8RecvBufStartAddress + UPG_UART_CONTENT_OFFSET, UPG_pstUartWkbf->u16RecvCmdVsContentLen - UPG_UART_CMD_SIZE);
                }

                if(UartRxQueue != NULL)
                {
            		res = xQueueSendToBackFromISR(UartRxQueue, &UPG_stUartCmdIsr, NULL);
                    if(res != pdTRUE)
                    {

                    }
                }
	
            }
            else
            {
            }
            
            /* Restart to receive new frame */
            HAL_UART_Receive_IT(huart, UPG_pstUartWkbf->pu8RecvBufStartAddress, UPG_UART_HEAD_A_SIZE);
            UPG_pstUartWkbf->enUartRecvSm = UPG_enUartRecvSM_Head_A;
            
            break;
            
        default:
            break;
    }
}


int32_t UPG_UART_i32Reply(UPG_tstUartWorkBufPtr pstUartWkbf, uint16_t cmd, void *rplContent, uint16_t rplContentSize)
{
    uint16_t indexTail;
    UPG_tstUartRplPtr pstUartRpl;
    
    if((rplContent == NULL) || (rplContentSize > sizeof(UPG_tunUartRplContent)))
    {
        /* Record error */
    
        return -1;
    }
    pstUartWkbf->pu8SendBufStartAddress[UPG_UART_HEAD_OFFSET] = 'A';
    pstUartWkbf->pu8SendBufStartAddress[UPG_UART_HEAD_OFFSET + 1] = 'H';

    pstUartRpl = (UPG_tstUartRplPtr)&pstUartWkbf->pu8SendBufStartAddress[UPG_UART_CTRL_OFFSET];
    
    pstUartRpl->unCtrl.u16Val = rplContentSize + UPG_UART_CMD_SIZE; /* CRC not used */
    /* Reply to Desktop program, use MSB, so need to case LSB to MSB */
    pstUartRpl->unCtrl.u16Val = SWAP_UINT16(pstUartRpl->unCtrl.u16Val);
    
    pstUartRpl->u16RplCmd = cmd + UPG_REPLY_OFFSET;
    /* Reply to Desktop program, use MSB, so need to case LSB to MSB */
    pstUartRpl->u16RplCmd = SWAP_UINT16(pstUartRpl->u16RplCmd);
    
    memcpy(&pstUartRpl->unRplContent, rplContent, rplContentSize);
    
    indexTail = UPG_UART_HEAD_SIZE + UPG_UART_CTRL_SIZE + UPG_UART_CMD_SIZE + rplContentSize;

    pstUartWkbf->pu8SendBufStartAddress[indexTail] = 'M';
    pstUartWkbf->pu8SendBufStartAddress[indexTail + 1] = 'I';

    return HAL_UART_Transmit_IT(&huart, &pstUartWkbf->pu8SendBufStartAddress[0], indexTail + 2);

    
}


void UPG_UART_vTimerCallback(void)
{
    uint32_t flag = UPG_REPLY_DELAY_FLAG;
    UPG_tstUartWorkBufPtr pstUartWkbf = UPG_pstUartWkbf;
    
    UPG_UART_i32Reply(pstUartWkbf, pstUartWkbf->u16CurCmd, &flag, sizeof(flag));
    /* Set Timer again */
    UPG_UART_SET_TIMER();
}


void UPG_UART_vParseCmd(UPG_tstUartCmdPtr pCmd)
{
    uint32_t maxPackageSize;
    uint32_t address, fileSize;
    uint32_t offset;
    int32_t res;
    UPG_tstUartWorkBufPtr pstUartWkbf = UPG_pstUartWkbf;

    /* Update current Cmd to workbuf */
    pstUartWkbf->u16CurCmd = pCmd->u16Cmd;
    
    switch (pCmd->u16Cmd)
    {
        case UPG_CMD_SHAKE_HAND:
            /* Received Shakehand signal means UART work */
            UPG_SUBSM_PAS_vUartWork();
            /* Shakehand return the Max Data Length to Upper Computer */
            maxPackageSize = UPG_UART_DOWNLOAD_MAX_DATA_LEN;
            maxPackageSize = SWAP_UINT32(maxPackageSize);
            UPG_UART_i32Reply(pstUartWkbf, UPG_CMD_SHAKE_HAND, &maxPackageSize, sizeof(maxPackageSize));
            
            break;
            
        case UPG_CMD_REQ_DOWNLOAD:
            address = pCmd->unUartContent.stContentReqDownload.u32Addr;
            fileSize = pCmd->unUartContent.stContentReqDownload.u32FileSize;
            /* MSB -> LSB to used by MCU */
            address = SWAP_UINT32(address);
            fileSize = SWAP_UINT32(fileSize);
            
            /* Start a Timer for timeout reply */
            UPG_UART_SET_TIMER();
            /* Call Passive interface to notify new request for downloading is coming */
            res = UPG_PAS_i32SetUpgFileAndSize((uint8_t *)address, fileSize);
            /* Stop Timer */
            UPG_UART_STOP_TIMER();
            
            res = SWAP_UINT32(res);
            UPG_UART_i32Reply(pstUartWkbf, UPG_CMD_REQ_DOWNLOAD, &res, sizeof(res));
            break;

        case UPG_CMD_DOWNLOAD:
            offset = pCmd->unUartContent.stContentDownload.u32Offset;
            /* MSB -> LSB to used by MCU */
            offset = SWAP_UINT32(offset); /* Currently no use of offset in Passive interface */
        
            if(pCmd->unCtrl.bits.len > UPG_UART_DOWNLOAD_DATA_OFFSET)
            {
                /* Important: if current Offset equal to last, ignore it */
                if((pstUartWkbf->u16LastOffset != offset) || (0 == offset))
                {
                    /* Start a Timer for timeout reply */
                    UPG_UART_SET_TIMER();
                    /* Call Passive interface to notify new download data is coming */
                    res = UPG_PAS_i32GetData((uint8_t *)&pCmd->unUartContent.stContentDownload.au8Data[0], pCmd->unCtrl.bits.len - UPG_UART_DOWNLOAD_DATA_OFFSET);
                    /* Stop Timer */
                    UPG_UART_STOP_TIMER();
                }
                else
                {
                    res = OK;
                }

                pstUartWkbf->u16LastOffset = offset;
                res = SWAP_UINT32(res);
                UPG_UART_i32Reply(pstUartWkbf, UPG_CMD_DOWNLOAD, &res, sizeof(res));
            }
            else
            {
                /* Record exception */
            }
            
            break;

        case UPG_CMD_DOWNLOAD_END:
            
            res = 0;
            
            UPG_UART_i32Reply(pstUartWkbf, UPG_CMD_DOWNLOAD_END, &res, sizeof(res));
            /* Notify bootload to quit upgrade */
            UPG_SUBSM_PAS_vCanUartRunQuit();
            break;
            
        default:
            break;
        
    }
}



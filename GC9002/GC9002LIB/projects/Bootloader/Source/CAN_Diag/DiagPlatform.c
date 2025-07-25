#include "DiagPlatform.h"
#include "DiagNWL.h"

#include "SID_28.h"
#include "DiagTimer.h"

#include "core_rv32.h"
#include <csi_config.h>
#include "csi_irq.h"
#include "csi_irq_vectors.h"
#include "upgrade_c1.h"


xTaskHandle     CanRxTaskHandle;
QueueHandle_t CanRxQueue;

static int g_i32RxErrCnt = 0;
static bool g_bCanInited = FALSE;



static CANFD_MsgTypeDef rx_msg;
static CANFD_MsgTypeDef tx_msg;
static CANFD_HandleTypeDef hcanfd;

extern CanFrameType Can_Frame;

#ifndef CONFIG_KERNEL_NONE
#include <csi_kernel.h>
#endif
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void CANDIAG_vCanFdIRQHandler(void)
#else
static ATTRIBUTE_ISR void CANDIAG_vCanFdIRQHandler(void)
#endif
{
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif

    CSI_INTRPT_ENTER();
    
    //HAL_CANFD_IRQHandler(&hcanfd);
    HAL_CANFD_RxMsgCallback(&hcanfd);
    
    CSI_INTRPT_EXIT();
    
#if(defined(CHIP_GC9002) && !(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
        CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}


void CANDIAG_vCanFd_MspInit(CANFD_HandleTypeDef *hcanfd)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    uint32_t rxPin, txPin;
    GPIO_TypeDef* rxPort;
    GPIO_TypeDef* txPort;
        
    HAL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    rxPort = SYN_GET_CAN_RX_PORT;
    rxPin = SYN_GET_CAN_RX_PIN;
    txPort = SYN_GET_CAN_TX_PORT;
    txPin = SYN_GET_CAN_TX_PIN;
    
    if(hcanfd->Instance == CANFD)
    {
        #if(0)
        /**CANFD GPIO Configuration
         * PA6      ------> CAN_TX
         * PA7      ------> CAN_RX
         * */
        GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF5;
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
       
        /**CANFD GPIO Configuration
         * PB18     ------> CAN_TX
         * PB19     ------> CAN_RX
         * */
        GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF6;
        GPIO_InitStruct.Pin = GPIO_PIN_18 | GPIO_PIN_19;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        #endif
        if((GPIO_PIN_19 == rxPin) && (GPIO_PIN_18 == txPin) && (GPIOB == rxPort) && (GPIOB == txPort))
        {
            GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF6;
            GPIO_InitStruct.Pin = rxPin | txPin;
            HAL_GPIO_Init(rxPort, &GPIO_InitStruct);
        }
        else if((GPIO_PIN_7 == rxPin) && (GPIO_PIN_6 == txPin) && (GPIOA == rxPort) && (GPIOA == txPort))
        {
            GPIO_InitStruct.Alternate = (uint32_t)GPIO_AF5;
            GPIO_InitStruct.Pin = rxPin | txPin;
            HAL_GPIO_Init(rxPort, &GPIO_InitStruct);
        }
        else
        {
            /* Record error */
            SYN_ERR_LOG(EM_enErrCanInitFail, EM_enErrLevelFatal);
            /* Depend on FAE request, initialization failing will hold on here */
            while(1);
        }
        
    }
    /* Peripheral clock enable */
    __HAL_RCC_CAN_CLK_ENABLE();
    HAL_RCC_CAN_PresSet(hcanfd->Init.Prescaler, ENABLE);
}


void CANDIAG_vCanTransceiverInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    uint32_t ctrlPin;
    GPIO_TypeDef* ctrlPort;

    if(SYN_GET_CAN_IF_TRANSCEIVER_CTRL)
    {
        ctrlPin = SYN_GET_CAN_TRANSCEIVER_PIN;
        ctrlPort = SYN_GET_CAN_TRANSCEIVER_PORT;
            
        HAL_GPIO_StructInit(&GPIO_InitStruct);
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Pin = ctrlPin;
        HAL_GPIO_Init(ctrlPort, &GPIO_InitStruct);

        HAL_GPIO_WritePin(ctrlPort, ctrlPin, GPIO_PIN_RESET);
    }
    
}


void CANDIAG_vCanFdHwInit(void)
{
    CANFD_FilterTypeDef filter;

    hcanfd.Instance = SYN_GET_CAN_CHANEL;
    
    /* Instead of calling HAL_CANFD_StructInit(&hcanfd) */
    hcanfd.Init.Mode = CANFD_Mode_Normal;
    hcanfd.Init.ISO_En = CANFD_DISABLE;
#if(defined CHIP_GC9005)
    hcanfd.Init.Prescaler = BSP_CANFD_PRESCALER_30;
#elif(defined CHIP_GC9002)
    hcanfd.Init.Prescaler = BSP_CANFD_PRESCALER_12;
#endif
    hcanfd.Init.Baudrate_slow = SYN_GET_CAN_BAUD_SLOW;
    hcanfd.Init.Baudrate_fast = SYN_GET_CAN_BAUD_FAST;

    /* Self-defined CanFd_MspInit callback function */
    hcanfd.MspInitCallback = CANDIAG_vCanFd_MspInit;
    HAL_CANFD_Init(&hcanfd);
    
    /* CAN Filter Init */
    filter.FilterId = SYN_GET_CAN_FILTER_ID0;   //ID
    filter.FilterMaskId = SYN_GET_CAN_FILTER_MASK0;
    filter.FilterBank = CANFD_ACRAMR_0;         //Filter Bank0
    filter.FilterMode = CANFD_Filter_STD;   //accept both standard and extend frame
    HAL_CANFD_ConfigFilter(&hcanfd, &filter);

    filter.FilterId = SYN_GET_CAN_FILTER_ID1;   //ID
    filter.FilterMaskId = SYN_GET_CAN_FILTER_MASK1;
    filter.FilterBank = CANFD_ACRAMR_1;         //Filter Bank1
    filter.FilterMode = CANFD_Filter_STD;   //accept both standard and extend frame
    HAL_CANFD_ConfigFilter(&hcanfd, &filter);


#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(CANFD_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, CANDIAG_vCanFdIRQHandler);
#else
    csi_clic_register_irq(CANFD_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, CANDIAG_vCanFdIRQHandler);
#endif
    HAL_CANFD_AutoReTxCtrl(&hcanfd, CANFD_TBSEL_STB, false);
    HAL_CANFD_AutoReTxCtrl(&hcanfd, CANFD_TBSEL_PTB, false);

    HAL_CANFD_IT_Enable(&hcanfd, CANFD_RIE);
    
    /* CAN Transceiver initialization here */
    CANDIAG_vCanTransceiverInit();
    g_bCanInited = TRUE;
}


void CANDIAG_vCanFdHwDeInit(void)
{
    if(g_bCanInited)
    {
        HAL_CANFD_DeInit(&hcanfd);
        g_bCanInited = FALSE;
    }
}


uint32_t CAN_SendDiagMessage( uint32_t	id, uint8_t* pdata, uint8_t dlc, uint32_t channel)
{
    tx_msg.StdId = id;                      //Standand ID
    tx_msg.ExtId = 0;                       //extended ID
    tx_msg.DLC = dlc;                       //data length
    tx_msg.BRS = CANFD_BRS_Disable;         //if need switch Band rate
    tx_msg.FDF = CANFD_Frame_CAN20;         //Frame Type
    tx_msg.RTR = CANFD_RTR_DATA;            //RTR Type
    tx_msg.IDE = CANFD_ID_STD;              //CAN Frame Type
    tx_msg.Data[0] = pdata[0];
    tx_msg.Data[1] = pdata[1];
    tx_msg.Data[2] = pdata[2];
    tx_msg.Data[3] = pdata[3];
    tx_msg.Data[4] = pdata[4];
    tx_msg.Data[5] = pdata[5];
    tx_msg.Data[6] = pdata[6];
    tx_msg.Data[7] = pdata[7];
    memset(&tx_msg.Data[8], 0x3C, 56);
    tx_msg.Timestamp = 0;

    return HAL_CANFD_AddTxMessage(&hcanfd, &tx_msg);
}


uint32_t CAN_SendDiagMessageIsr(uint32_t id, uint8_t* pdata, uint8_t dlc, uint32_t channel)
{
    CANFD_MsgTypeDef canFD_msg;
    
    canFD_msg.StdId = id;                  //Standand ID
    canFD_msg.ExtId = 0;                   //extended ID
    canFD_msg.DLC = dlc;                   //data length
    canFD_msg.BRS = CANFD_BRS_Disable;     //if need switch Band rate
    canFD_msg.FDF = CANFD_Frame_CAN20;     //Frame Type
    canFD_msg.RTR = CANFD_RTR_DATA;        //RTR Type
    canFD_msg.IDE = CANFD_ID_STD;          //CAN Frame Type
    canFD_msg.Data[0] = pdata[0];
    canFD_msg.Data[1] = pdata[1];
    canFD_msg.Data[2] = pdata[2];
    canFD_msg.Data[3] = pdata[3];
    canFD_msg.Data[4] = pdata[4];
    canFD_msg.Data[5] = pdata[5];
    canFD_msg.Data[6] = pdata[6];
    canFD_msg.Data[7] = pdata[7];
    memset(&canFD_msg.Data[8], 0x3C, 56);
    canFD_msg.Timestamp = 0;

    return HAL_CANFD_AddTxMessage(&hcanfd, &canFD_msg);
}


void CANDIAG_vRxMessageRoute(CanFrameType* pRxMessage)
{
    BaseType_t res;
	if((pRxMessage->Id == TESTER_DIAG_CAN_ID) /*|| (pRxMessage->StdId == TESTER_FUNCTION_CAN_ID)*/)
	{
		res = xQueueSendToBackFromISR(CanRxQueue,pRxMessage,NULL);
        if(res != pdTRUE)
        {
            g_i32RxErrCnt++;
        }
	}
	else if(pRxMessage->Id == TESTER_FUNCTION_CAN_ID)
	{
        /* In order to notify system that CAN is working */
        UPG_SUBSM_PAS_vCanWork();
        /* */
		//if( (get_Communication_flag() == 0) || (get_Communication_flag() == 1) )
		//{
			//CanMsgReceive(pRxMessage);   // by Zhu Jiahai, currently no functional frame need to be dealt
		//}
	}
}



void CANDIAG_vInit(void)
{
    portBASE_TYPE xReturn;
    
    CanRxQueue = xQueueCreate(CAN_RX_QUEUE_SIZE, sizeof(CanFrameType));
    
        
    xReturn = xTaskCreate(CANDIAG_vCanRxMainProcess, "CanRxTask", configMINIMAL_STACK_SIZE * 2, NULL, 4, &CanRxTaskHandle);
    if (xReturn != pdTRUE)
    {
        /* Record error */
        SYN_ERR_LOG(EM_enCreateTaskFail, EM_enErrLevelFatal);
        return;
    }

    
#if SWITCH_SEND_MAIN_STATE
    Diag_Timer_update(Diag_AHMITIM7);
#endif

    /* Temporary put hardware initialization here */
    CANDIAG_vCanFdHwInit();
}



void CANDIAG_vCanRxMainProcess( void * pvParameters )
{
	BaseType_t xStatus; 	
	
	for(;;)
	{	
		xStatus = xQueueReceive(CanRxQueue, (void*)&Can_Frame, portMAX_DELAY);
		if(xStatus != pdPASS)
		{
			return;
		}
		
		gf_diag_com_EXEC();									
	}
}


void HAL_CANFD_RxMsgCallback(CANFD_HandleTypeDef *hcanfd)
{
    HAL_StatusTypeDef retStatus;
    CanFrameType canFrame;

    HAL_CANFD_ClearITFlag(hcanfd, CANFD_FLAG_RIF);
    
    while((hcanfd->Instance->RCTRL_TCTRL_TCMD_CFGSTAT.value & CANFD_RSTAT) != RESET)
    {
        retStatus = HAL_CANFD_GetRxMessage_IT(hcanfd, &rx_msg);
        if(HAL_OK == retStatus)
        {
            canFrame.Id = rx_msg.StdId;
            memcpy(canFrame.data, rx_msg.Data, 8);
            CANDIAG_vRxMessageRoute(&canFrame);
        }
        else
        {
            /* Record error */
        }
    }

}





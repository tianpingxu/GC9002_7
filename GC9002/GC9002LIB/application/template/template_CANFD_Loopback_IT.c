/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-13     huyt        the first version
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gc90xx.h"
#include "core_rv32.h"
#include <csi_config.h>
#include "template_def.h"
#include "csi_irq.h"
#include "csi_irq_vectors.h"

#if(BSP_DEBUG == TEST_TEMPLE_BSP)
#ifdef  _TEMPLATE_CANFD_LOOPBACK_IT_

static void CANFD_Test(void);

int main(void)
{
    CANFD_Test();
    while (1)
    {
		HAL_Delay(1000);
    }
    return 0;
}
#define TEST_ID_LENGTH          CANFD_ID_STD
#define TEST_MODE               CANFD_Frame_CANFD
#define TEST_DATA_LENGTH        CANFD_DLC_64
#define TEST_TX_INTERFACE       0//0:HAL_CANFD_AddTxMessage   1:HAL_CANFD_TxPrimaryMessage
static CANFD_MsgTypeDef tx_msg;
static CANFD_MsgTypeDef rx_msg;
static CANFD_HandleTypeDef hcanfd = {.Instance = CANFD};

#ifndef CONFIG_KERNEL_NONE
#include <csi_kernel.h>
#endif
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
static void TEMPLATE_CANFD_IRQHandler(void)
#else
static ATTRIBUTE_ISR void TEMPLATE_CANFD_IRQHandler(void)
#endif
{
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_ENTER_SP_SWITCH();
#endif
    CSI_INTRPT_ENTER();
    
    HAL_CANFD_IRQHandler(&hcanfd);
    
    CSI_INTRPT_EXIT();
#if(!(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U))
    CSI_VECTOR_IRQ_EXIT_SP_SWITCH();
#endif
}
static void CANFD_Test(void)
{
    CANFD_FilterTypeDef filter;

    HAL_CANFD_StructInit(&hcanfd);
    hcanfd.Init.Mode = CANFD_Mode_LoopbackInternal;

    /* CAN 筛选器初始化 */
#if(TEST_ID_LENGTH == CANFD_ID_STD)
    filter.FilterId = 0x88;   //ID
#elif(TEST_ID_LENGTH == CANFD_ID_EXT)
    filter.FilterId = 0x1313;   //ID
#endif
    filter.FilterMaskId = 0xFFFFFFFF;
    filter.FilterBank = CANFD_ACRAMR_0;         //筛选器组1
    filter.FilterMode = CANFD_Filter_STD_EXT;   //accept both standard and extend frame

    HAL_CANFD_Init(&hcanfd);
    HAL_CANFD_ConfigFilter(&hcanfd, &filter);
#if(TEST_ID_LENGTH == CANFD_ID_STD)
    filter.FilterId = 0x89;   //ID
#elif(TEST_ID_LENGTH == CANFD_ID_EXT)
    filter.FilterId = 0x1414;   //ID
#endif
#if(defined CONFIG_CSI_NON_VECTOR && CONFIG_CSI_NON_VECTOR == 1U)
    csi_clic_register_irq(CANFD_IRQn, CLIC_NON_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, TEMPLATE_CANFD_IRQHandler);
#else
    csi_clic_register_irq(CANFD_IRQn, CLIC_VECTOR_INTERRUPT, CLIC_LEVEL_TRIGGER, 2, 3, TEMPLATE_CANFD_IRQHandler);
#endif
    HAL_CANFD_IT_Enable(&hcanfd, CANFD_IT_ALL);
    while(1)
    {
        tx_msg.StdId = 0x88;            //标准帧ID
        tx_msg.ExtId = 0x1414;          //扩展帧ID
        tx_msg.DLC = TEST_DATA_LENGTH;  //数据长度
        tx_msg.BRS = CANFD_BRS_Enable;  //转换速率, data段使用高速率
        tx_msg.FDF = TEST_MODE;         //CAN帧/CANFD帧
        tx_msg.RTR = CANFD_RTR_DATA;    //数据帧
        tx_msg.IDE = TEST_ID_LENGTH;    //帧ID类型 标准帧/扩展帧
        tx_msg.Data[0] = 0x12;
        tx_msg.Data[1] = 0x34;
        tx_msg.Data[2] = 0x56;
        tx_msg.Data[3] = 0x78;
        tx_msg.Data[4] = 0x90;
        tx_msg.Data[5] = 0x12;
        tx_msg.Data[6] = 0x34;
        tx_msg.Data[7] = 0x56;
        memset(&tx_msg.Data[8], 0x3C, 60);
        memset(&rx_msg, 0, sizeof(rx_msg));
        tx_msg.Timestamp = 0;
#if(TEST_TX_INTERFACE)
        if(HAL_CANFD_TxPrimaryMessage(&hcanfd, &tx_msg, HAL_MAX_DELAY) == HAL_OK)
#else
        if(HAL_CANFD_AddTxMessage(&hcanfd, &tx_msg) == HAL_OK)
#endif
        {
            printf("TX:\nId:0x%x,Data:[0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x]\n", tx_msg.StdId,
																							tx_msg.Data[0],
																							tx_msg.Data[1],
																							tx_msg.Data[2],
																							tx_msg.Data[3],
																							tx_msg.Data[4],
																							tx_msg.Data[5],
																							tx_msg.Data[6],
																							tx_msg.Data[7]);
        }
        else {
            printf("TX error\n");
        }

		HAL_Delay(500);
		while(rx_msg.Data[0] == 0)
		{
			HAL_Delay(500);
		}
    }
}


void HAL_CANFD_RxMsgCallback(CANFD_HandleTypeDef *hcanfd)
{
    HAL_CANFD_GetRxMessage_IT(hcanfd, &rx_msg);
}
void HAL_CANFD_RxBuffOverrunCallback(CANFD_HandleTypeDef *hcanfd)
{}
void HAL_CANFD_RxBuffFullCallback(CANFD_HandleTypeDef *hcanfd)
{}
void HAL_CANFD_RxBuffAlmostFullCallback(CANFD_HandleTypeDef *hcanfd)
{}
void HAL_CANFD_TxPriClptCallback(CANFD_HandleTypeDef *hcanfd)
{
}
void HAL_CANFD_TxSecClptCallback(CANFD_HandleTypeDef *hcanfd)
{
}
void HAL_CANFD_ErrorCallback(CANFD_HandleTypeDef *hcanfd)
{}
void HAL_CANFD_AbortCallback(CANFD_HandleTypeDef *hcanfd)
{}
void HAL_CANFD_PassiveErrorCallback(CANFD_HandleTypeDef *hcanfd)
{}
void HAL_CANFD_ArbitrationLostCallback(CANFD_HandleTypeDef *hcanfd)
{}
void HAL_CANFD_BusErrorCallback(CANFD_HandleTypeDef *hcanfd)
{}


#endif/* _TEMPLATE_CANFD_LOOPBACK_IT_ */
#endif/* BSP_DEBUG */

/**
  ******************************************************************************
  * @file    gc90x_sharememory.h
  * @author  GraphichinaSX Team
  * @version V1.0.0
  * @date    12-April-2019
  * @brief   This file contains all the functions prototypes for the ShareMemory 
  *          firmware library.
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GC900x_SHMEMMANAGE_H
#define __GC900x_SHMEMMANAGE_H

//#include "gc90x_shmemtransport.h"

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/
     
/** 
  * @brief  ShareMemory OPCode 
  */
typedef struct ShareMemorySendValue
{
    uint32_t mID;
    uint32_t mValue;
} SendValueType;

//uint32_t ShareMemory_SendDrawDoneFrame(uint32_t response, uint32_t DataNumber, SendValueType* pData, uint32_t wait,  SendFrameCB sendFramecbfunc, ResponseCB responsecbfunc);
uint32_t ShareMemory_SendDataFrame(uint32_t response, uint32_t DataNumber, SendValueType* pData, uint32_t wait, uint32_t sendFramecbfunc, uint32_t responsecbfunc);
uint32_t ShareMemory_SendLogFrame(uint32_t response, uint32_t DataNumber, SendValueType* pData, uint32_t wait, uint32_t sendFramecbfunc, uint32_t responsecbfunc);
//uint32_t ShareMemory_ReadDataFrame(uint32_t DataNumber, uint32_t* pData, uint32_t wait, SendFrameCB sendFramecbfunc, ResponseCB responsecbfunc);
uint32_t ShareMemory_SendStringFrame(uint32_t response, uint32_t ID, uint32_t DataNumber, uint8_t* pData, uint32_t wait, uint32_t sendFramecbfunc, uint32_t responsecbfunc);
//uint32_t ShareMemory_ReadStringFrame( uint32_t ID, uint32_t DataNumber, uint32_t wait, SendFrameCB sendFramecbfunc, ResponseCB responsecbfunc);
//uint32_t ShareMemory_SendTouchInfoFrame( uint32_t posiX, uint32_t posiY, uint32_t touchState, uint32_t wait, SendFrameCB sendFramecbfunc);
//uint32_t ShareMemory_SendTestCpu1Frame(uint32_t wait, SendFrameCB sendFramecbfunc, ResponseCB responsecbfunc, uint32_t* pIndex);
//uint32_t ShareMemory_ReadCPU0StringInfoFrame( uint32_t infoType, uint32_t DataNumber, uint32_t wait, SendFrameCB sendFramecbfunc, ResponseCB responsecbfunc);

//uint32_t WaitForResponse(void);

//void ReceiveDataCBForQueue(uint32_t* buffer, uint32_t size);

#ifdef __cplusplus
 }
#endif

#endif /* __GC900x_SHAREMEMORY_H */

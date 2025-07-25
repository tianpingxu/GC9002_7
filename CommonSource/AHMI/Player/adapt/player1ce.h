/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : player1ce.h
*
* Description  : This file contains the external / export configuration of PLAR
*
* Environment  : MDK / PC Simulation
*
* Responsible  : Zhu Jiahai
*
* Guidelines   : 
*
* Template name: C++
*
* Data         : 2022-09-23
****************************************************************************/
#ifndef PLAR_CE_H
#define PLAR_CE_H

#ifdef __cplusplus 
extern "C" {
#endif

#define PLAR_MODULE_ID                          (77)  /* Temporary defination value */
#define PLAR_MK_ERR_ID(id)                      (((PLAR_MODULE_ID) << 16) | (id))


/* Define size of Player instruction Queue */
#ifdef CHIP_GC9002_NoHyper
#define PLAR_INSTRUCTION_QUEUE_SIZE             (50)
#else
#define PLAR_INSTRUCTION_QUEUE_SIZE             (200)       /* Prevent complicated UI caused Queen overflow */
#endif

/* Config the size of PLAR stack size */
#ifdef CHIP_GC9002_NoHyper
#define PLAR_STACK_SIZE                         (256 * 6)
#else
#define PLAR_STACK_SIZE                         (256 * 8)       /* Prevent complicated UI caused Queen overflow */
#endif


#define PLAR_PARA_SIZE                          (4)


#define PLAR_xQueueSendToBack(pvItemToQueue, xTicksToWait ) csi_kernel_msgq_put(PlayerInstructionQueue, pvItemToQueue, 0, xTicksToWait)

#define PLAR_xQueueSendToFront(pvItemToQueue, xTicksToWait )   csi_kernel_msgq_put( PlayerInstructionQueue, pvItemToQueue, 1, xTicksToWait)

#ifdef __cplusplus 
}
#endif

#endif


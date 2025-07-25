////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:     AHMI
//
// Create Date:   2015/11/17
// File Name:     main.c
// Project Name:  AHMISimulator
//
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Revision 2.02 - add the modbus definition
// Additional Comments:
//    in 2.00, add the canvas initiation
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "AHMIInterface.h"
#include "publicInclude.h"

#if defined(EMBEDDED) && defined(VERSION_2)
#include "gc90x_initial.h"
#include "gc90x_cache.h"
#include "ITM1c1.h"
#include "SBM1c1.h"
#include "ahmiv4.h"
#include "csi_ahmi_hw.h"
#include "csi_hal_core.h"
#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
#include "gc90x_heap.h"
#if defined(CHIP_GC9002)
#include "CustomerAppMain.h"
#include "com_c1.h"

#endif
#endif
#endif
#include "player1c1.h"


#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
extern uint8_t gcHeap[ gcConfigTOTAL_HEAP_SIZE ];
#endif

#ifdef AHMI_CORE
//-----------------------------
// function: AHMITop/main
// @brief: Top entry.
//-----------------------------
#ifdef PC_SIM
int AHMITop()
#endif  /* PC_SIM */
#ifdef EMBEDDED
int main()
#endif  /* EMBEDDED */
{
#ifdef EMBEDDED
	
	/* FlushDcache execute in code flash. */
    csi_hal_DCache_Flush();
	#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
	#ifndef CHIP_GC9002_NoHyper
	gcInit(gcHeap, gcConfigTOTAL_HEAP_SIZE);
	#endif
	#else
    /* Set CPU1 hardware operation state to NO_USE£¬ make sure call SBM_vEnterCpu1HardwareOp() not hold */
    SBM_vSetCpu1HardwareOpState(SBM_HARDWARE_NO_USE);
    /* To avoid confliction on operating Hardware simultaneous when Power On */
    SBM_vEnterCpu1HardwareOp();
	#endif

	/* Initial system hardware. */
    csi_ahmi_hw_int();
    
	#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
	#else
    /* Release occupying Hardware operation */
    SBM_vExitCpu1HardwareOp();
	#endif
	
#if(ITM_COMPILE_SWITCH == ON)
    ITM_vInit();
#endif

    /* Create and initial the queues for drawing part. */
    initQueue(); 

	/* Create and initial the semaphores for set tag synchronization */
	initSemaphore();

	/* PLAR / EVP / SW of GPU initailization */
    PLAR_vInit();
    EVP_vInit();
    AHMI_vInit();  /*Software of AHMI */

    #if defined(EMBEDDED) && defined(CHIP_GC9002)

    /* Uart communication module */
    #if COM_COMPILE_SWITCH
    COM_vInit();
    #endif
    
    /* Customer Init here */
    CUSTOMER_INIT;
    #endif
    
    /* AHMI part entrence. */
    startAHMI(); 
	
#endif  /* EMBEDDED */

#ifdef PC_SIM
    int err;
	
	/* Create and initial the queues for drawing part. */
    initQueue(); 

	/* Create and initial the semaphores for set tag synchronization */
	initSemaphore();

	/* PLAR initailization */
	PLAR_vInit();

    /* AHMI part entrence. */
    err = startAHMI();
    if (err != AHMI_FUNC_SUCCESS)
    {
        return 0;
    }
#endif  /* PC_SIM */
    
    /* Start scheduler tasks for RTOS. */
    vTaskStartScheduler(); 


#if defined(CHIP_GC9002)
    /*If failed to scheduler tasks*/
    ERROR_PRINT("Fail to start Scheduler.\r\n");
    for (;;);
#else
    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
#endif
}
#endif	/* AHMI_CORE */


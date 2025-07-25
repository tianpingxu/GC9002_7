/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : ITM1c1.c
*
* Description  : This file contains the implementation of ITM interface
*
* Environment  : MDK / PC Simulation
*
* Responsible  : Zhu Jiahai
*
* Guidelines   : 
*
* Template name: C++
*
* Data         : 2022-05-19
****************************************************************************/


/* Foreign headerfiles */
#include <stdio.h>
#include "gc90x_initial.h"
#ifdef __cplusplus 
extern "C" {
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "trace.h"

#ifdef __cplusplus 
}
#endif


/* Own headerfiles */
#include "ITM1c1.h"



#if(ITM_COMPILE_SWITCH == ON)

volatile u32 ITM_u32Tick;


xTaskHandle ITMTaskHandle;



/**
  * @name       ITM_vTimerInit
  * @brief      Initialize the Timer, default use Timer7
  * @note       
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-05-19
  */
ITM_STATIC void ITM__vTimerInit(void)
{

    SYN_ITM_TIMER_INIT();
}


/**
  * @name       ITM__i32CreateTask
  * @brief      Create a task for ITM
  * @note       
  * @param      none
  * @retval     return 0 if create task successfully
  * @author     Zhu Jiahai
  * @date       2022-05-19
  */
ITM_STATIC int ITM__i32CreateTask(void)
{
    portBASE_TYPE xReturn;
    xReturn = xTaskCreate(ITM_vMain, "ITMTask", configITM_STACK_SIZE, NULL, 2, &ITMTaskHandle);
    if (xReturn != pdTRUE)
    {
        ERROR_PRINT("Fail to create ITM Task.\r\n");
        return errTASK_CREATE_FAIL;
    }

    return 0;
}


/**
  * @name       ITM_vInit
  * @brief      Initialization interface for ITM
  * @note       
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-05-19
  */
void ITM_vInit(void)
{

    //ITM__vTimerInit(); /* use Timer8 , initialization in other place */
    
    ITM_u32Tick = 0;

    ITM__i32CreateTask();
}


/**
  * @name       ITM_vIsrProc
  * @brief      Called by Timer ISR, maintain the Tick of ITM 
  * @note        
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-05-19
  */
void ITM_vIsrProc(void)
{
    ITM_u32Tick++;
}


/**
  * @name       ITM_vDeinit
  * @brief      De-Initialization interface for ITM
  * @note       
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-05-19
  */
void ITM_vDeinit(void)
{

}

/* for save CPU loader Info */
ITM_STATIC char ITM_u8CpuLoaderInfo[configTMP_ARRAY_SIZE];
/* for save CPU Stack Info */
ITM_STATIC char ITM_u8CpuStackInfo[configTMP_ARRAY_SIZE];

/**
  * @name       ITM_vMain
  * @brief      Main process of ITM
  * @note       
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-05-19
  */
void ITM_vMain(void *pvParameters)
{
    
    while(1)
    {
        if(ITM_u32Tick > 2000)
        {
            memset(ITM_u8CpuLoaderInfo, 0, configTMP_ARRAY_SIZE);
            /* Print CPU Loader info to buffer */
            /* Print Format: TaskName RunTimes OccupiedRate */
            vTaskGetRunTimeStats(ITM_u8CpuLoaderInfo);

            memset(ITM_u8CpuStackInfo, 0, configTMP_ARRAY_SIZE);
            /* Print CPU stack info to buffer */
            /* Print Format: TaskName TaskState Priority LeftStack TaskSequence */
            vTaskList(ITM_u8CpuStackInfo);
            
        }
        //vTaskDelay(configITM_PRINT_PERIOD * configITM_DELAY_TIMES_PER_MINUTE);
        vTaskDelay(200);
    }

}



#endif /* ITM_COMPILE_SWITCH == ON */


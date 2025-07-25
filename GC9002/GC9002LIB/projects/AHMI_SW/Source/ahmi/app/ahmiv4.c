//#include "gc90x_ahmi.h"
#include "ahmiv4.h"
//#include "gc90x_cache.h"
#include "csi_ahmi.h"
#include "trace.h"
#include <stdio.h>
#include <string.h>
#include "csi_kernel.h"

AHMI_tstWorkBuffer AHMI_stWorkbuf;
/**********************************************EVP**************************************************/
k_task_handle_t EVPTaskHandle;
static k_event_handle_t EvtHandler;

EVP_tstWorkBuffer EVP_stWorkBuf;

/**
  * @name       EVT__i32CreateTask
  * @brief      Create a task for EVT
  * @note       
  * @param      none
  * @retval     return 0 if create task successfully
  * @author     Zhu Jiahai
  * @date       2022-09-26
  */
int32_t EVP__i32CreateTask(void)
{
    int32_t res;
    portBASE_TYPE xReturn;

    xReturn = csi_kernel_task_new(EVP_vMainTask, "EVPTask", NULL, KPRIO_LOW3, 0, NULL, EVP_STACK_SIZE, &EVPTaskHandle);
    //xReturn = xTaskCreate(EVP_vMainTask, "EVPTask", EVP_STACK_SIZE, NULL, 4, &EVPTaskHandle);
    if (xReturn != 0)
    {
        res =  EVP_MK_ERR_ID(enEVP_CreateTaskErr);
        ERROR_PRINT("Fail to create EVP Task.\r\n");
    }
    else
    {
        res = EVP_OK;
    }

    return res;
}



/**
  * @name       EVP__i32CreateEvtGroup
  * @brief      Create Event Group for EVP
  * @note       
  * @param      pvParameters
  * @retval     EVP_OK: success; other: fail
  * @author     Zhu Jiahai
  * @date       2022-09-27
  */
int32_t EVP__i32CreateEvtGroup(void)
{   
    int32_t res = EVP_OK;
    
    EvtHandler = csi_kernel_event_new();
    if(NULL == EvtHandler)
    {
        res =  EVP_MK_ERR_ID(enEVP_CreateEventGroupErr);
    }
    else
    {
        
    }
    return res;
}

/**
  * @name       EVP_vInit
  * @brief      Initialization interface for EVP
  * @note       
  * @param      none
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-09-27
  */
void EVP_vInit(void)
{
    int32_t res;
    
    /* variable of module initailization first */

    
    res = EVP__i32CreateEvtGroup();
    res |= EVP__i32CreateTask();

    if(EVP_OK == res)
    {
        EVP_stWorkBuf.u8Init = pdTRUE;
    }
    else
    {
        ERROR_PRINT("Call EVP_vInit fail.\r\n");
    }
    
}




/**
  * @name       EVP_vMainTask
  * @brief      Task for Event Process
  * @note       
  * @param      pvParameters
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-09-27
  */
void EVP_vMainTask(void* pvParameters)
 {

 	EventBits_t r_event;
    while (1)
    {
        #if 0
        csi_kernel_event_wait(EvtHandler, /*Event handler */
                            EVENT_REQ_DRAW | EVENT_ISR_GPU_DONE | EVENT_ISR_TIME_OUT | EVENT_NOT_FINISH,    /* interesting event */
                            KEVENT_OPT_SET_ANY, 
                            pdTRUE,
                            &r_event, 
                            portMAX_DELAY);      /* wait time */
        #else
        r_event = xEventGroupWaitBits(EvtHandler,      /*Event handler */
                            EVENT_REQ_DRAW | EVENT_ISR_GPU_DONE | EVENT_ISR_TIME_OUT | EVENT_NOT_FINISH,    /* interesting event */
                            pdTRUE,             /* if clear event when quit */
                            pdFALSE,            /* if need all event satisfied */
                            portMAX_DELAY);     /* wait time */
        #endif                    
        if(((EVENT_REQ_DRAW | EVENT_ISR_GPU_DONE | EVENT_ISR_TIME_OUT | EVENT_NOT_FINISH) & r_event) != 0)
        {
            AHMI_vMainProcess(r_event);
        }
        else
        {
            
        }
        
    }
 }
 


/**
  * @name       EVP_i32EventGroupSetBitsReqDraw
  * @brief      Call to set bit of request Draw
  * @note       
  * @param      none
  * @retval     
  * @author     Zhu Jiahai
  * @date       2022-09-27
  */
int32_t EVP_i32EventGroupSetBitsReqDraw(void)
{
    int32_t res;
    //uint32_t ret_flags;
    
    if(EVP_stWorkBuf.u8Init)
    {
        res = EVP_xEventGroupSetBits(EVENT_REQ_DRAW);
        //res = csi_kernel_event_set(EvtHandler, EVENT_REQ_DRAW, &ret_flags);
    }  
    else
    {
        ERROR_PRINT("EVP not initialization.\r\n");
        res =  EVP_MK_ERR_ID(enEVP_ModuleUninted);
    }
    return res;
}


/**
  * @name       EVP_i32EventGroupSetBitsIsrGpuDone
  * @brief      Call to set bit of GPU Done
  * @note       
  * @param      pxHigherPriorityTaskWoken
  * @retval     
  * @author     Zhu Jiahai
  * @date       2022-09-27
  */
int32_t EVP_i32EventGroupSetBitsIsrGpuDone(BaseType_t *pxHigherPriorityTaskWoken)
{
    int32_t res;
    //uint32_t ret_flags;
    
    if(EVP_stWorkBuf.u8Init)
    {
        res = EVP_xEventGroupSetBitsFromISR(EVENT_ISR_GPU_DONE, pxHigherPriorityTaskWoken);
        //res = csi_kernel_event_set(EvtHandler, EVENT_ISR_GPU_DONE, &ret_flags);
    }  
    else
    {
        ERROR_PRINT("EVP not initialization.\r\n");
        res =  EVP_MK_ERR_ID(enEVP_ModuleUninted);
    }
    return res;
}


/**
  * @name       EVP_i32EventGroupSetBitsIsrTimeout
  * @brief      Call to set bit of Timeout
  * @note       
  * @param      pxHigherPriorityTaskWoken
  * @retval     
  * @author     Zhu Jiahai
  * @date       2022-09-27
  */
int32_t EVP_i32EventGroupSetBitsIsrTimeout(BaseType_t *pxHigherPriorityTaskWoken)
{
    int32_t res;
    //uint32_t ret_flags;
    
    if(EVP_stWorkBuf.u8Init)
    {
        res = EVP_xEventGroupSetBitsFromISR(EVENT_ISR_TIME_OUT, pxHigherPriorityTaskWoken);
        //res = csi_kernel_event_set(EvtHandler, EVENT_ISR_TIME_OUT, &ret_flags);
    }  
    else
    {
        ERROR_PRINT("EVP not initialization.\r\n");
        res =  EVP_MK_ERR_ID(enEVP_ModuleUninted);
    }
    return res;
}


/**********************************************EVP End**************************************************/


/**
  * @name       AHMI_vInit
  * @brief      Software initailization for AHMI
  * @note       
  * @param      pvParameters
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-09-27
  */
void AHMI_vInit(void)
{
    memset(&AHMI_stWorkbuf, 0, sizeof(AHMI_tstWorkBuffer));
    AHMI_stWorkbuf.u8GpuSts = enGpuStsIdle; 
}


/**
  * @name       AHMI_vMainProcess
  * @brief      main process for AHMI
  * @note       
  * @param      ebEvent
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-09-27
  */
extern funcStatus setDrawControlTrigger(uint32_t irqFlag);
void AHMI_vMainProcess(EventBits_t ebEvent)
{
    uint8_t evReqDraw, evIsrGpuDone, evIsrTimeout, evNotFinish;
    uint32_t* pu32SourceBuffer;

    evReqDraw = ebEvent & EVENT_REQ_DRAW;
    evIsrGpuDone = ebEvent & EVENT_ISR_GPU_DONE;
    evIsrTimeout = ebEvent & EVENT_ISR_TIME_OUT;
    evNotFinish = ebEvent & EVENT_NOT_FINISH;
        
    switch (AHMI_stWorkbuf.u8GpuSts)
    {
        case enGpuStsIdle:
            if(evReqDraw || evNotFinish)
            {
				AHMI_stWorkbuf.u8GpuSts = enGpuStsBusy;
				
                /* Start a timer to check timeout */
                csi_ahmi_StartDrawingTimer();
				
				csi_ahmi_Draw();
				
                /* In Idle status, Not finished job get running */
                if(evNotFinish)
                {
                    AHMI_stWorkbuf.u8NotFinish = pdFALSE;
                }
            }
            break;

        case enGpuStsBusy:
            if(evIsrGpuDone)
            {
                AHMI_stWorkbuf.u8GpuSts = enGpuStsIdle;
                /* Stop timer be done in AHMI ISR */ 
				setDrawControlTrigger(1);
            }
            else if(evReqDraw)
            {
                AHMI_stWorkbuf.u8NotFinish = pdTRUE;
            }
            else if(evIsrTimeout)
            {
                AHMI_stWorkbuf.u8GpuSts = enGpuStsErr;
                AHMI_stWorkbuf.u8NotFinish = pdTRUE;
            }
            break;

        case enGpuStsErr:
            /* Currently clearing error has been done by MidTimer callback function */
            /* void AHMITIMCB4(void) */
            AHMI_stWorkbuf.u8GpuSts = enGpuStsIdle;
            
            break;

        case enGpuStsUninit:
            ERROR_PRINT("Modue not Inited.\r\n");
            
            break;
        default:
            break;
    }

    /* Only AHMI under not busy, set not finish state, otherwise it will occupy CPU in BUSY state */
    if(AHMI_stWorkbuf.u8NotFinish && (enGpuStsBusy != AHMI_stWorkbuf.u8GpuSts))
    {
        /* trigger task to get run chance to finish job */
        EVP_xEventGroupSetBits(EVENT_NOT_FINISH);
    }
}

/**
  * @name       AHMI_vGetGpuStatus
  * @brief      main process for AHMI
  * @note       
  * @param      ebEvent
  * @retval     none
  * @author     Zhu Jiahai
  * @date       2022-09-27
  */
u32 AHMI_vGetGpuStatus(void)
{
    return AHMI_stWorkbuf.u8GpuSts;
    
}


void AHMI_vSetGpuStatusDone(void)
{
    AHMI_stWorkbuf.u8GpuSts = enGpuStsIdle;
}


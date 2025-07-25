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
#ifndef AHMIV4_H
#define AHMIV4_H
#include "publicType.h"

#ifdef __cplusplus 
extern "C" {
#endif

#include "FreeRTOS.h"
#include "semphr.h"
//#include "task.h"
#include "event_groups.h"
    
/*********************************************EVP************************************************/

#define EVP_MODULE_ID                           (79)  /* Temporary defination value */
#define EVP_MK_ERR_ID(id)                       (((EVP_MODULE_ID) << 16) | (id))

#ifndef EVP_OK
#define EVP_OK                                  (0)
#endif


/* Config the size of PLAR stack size */
#define EVP_STACK_SIZE                          (256)





/* Set Event Mask Bit */
#define EVENT_REQ_DRAW                          (0x01 << 0)     
#define EVENT_ISR_GPU_DONE                      (0x01 << 1)
#define EVENT_ISR_TIME_OUT                      (0x01 << 2)
#define EVENT_NOT_FINISH                        (0x01 << 3)

/* Define a micro function to hide interal variable to other module */
#define EVP_xEventGroupSetBits(uxBitsToSet) xEventGroupSetBits(EvtHandler, uxBitsToSet)

#define EVP_xEventGroupSetBitsFromISR(uxBitsToSet, pxHigherPriorityTaskWoken)    xEventGroupSetBitsFromISR(EvtHandler, uxBitsToSet, pxHigherPriorityTaskWoken)





typedef enum{
    enEVP_OK = 0,                           /**/
    enEVP_CreateTaskErr,                    /* Create Task error */
    enEVP_CreateEventGroupErr,              /* Create Event Grp error */
    enEVP_ModuleUninted,                    /* Module not inited */
    
}EVP_tenErrorType;


typedef struct{
    uint8_t u8Init;                         /* EVP Initialization status */
    uint8_t u8Resv[3];

}EVP_tstWorkBuffer, *EVP_tstWorkBufferPtr;

void EVP_vInit(void);
void EVP_vMainTask(void* pvParameters);
int32_t EVP_i32EventGroupSetBitsReqDraw(void);
int32_t EVP_i32EventGroupSetBitsIsrGpuDone(BaseType_t *pxHigherPriorityTaskWoken);
int32_t EVP_i32EventGroupSetBitsIsrTimeout(BaseType_t *pxHigherPriorityTaskWoken);



/********************************************End*************************************************/


typedef enum{
    enGpuStsUninit = 0,
    enGpuStsIdle,
    enGpuStsBusy,
    enGpuStsErr,
    enGpuStsTotal,
    
}AHMI_tenGpuSts;



typedef struct{
    uint8_t u8GpuSts;                       /* Gpu status */
    uint8_t u8NotFinish;                    /* Has Job not finished */
    uint8_t u8Resv[2];

}AHMI_tstWorkBuffer, *AHMI_tstWorkBufferPtr;


void AHMI_vInit(void);
void AHMI_vMainProcess(EventBits_t ebEvent);
u32 AHMI_vGetGpuStatus(void);
void AHMI_vSetGpuStatusDone(void);
    
#ifdef __cplusplus 
}
#endif

#endif



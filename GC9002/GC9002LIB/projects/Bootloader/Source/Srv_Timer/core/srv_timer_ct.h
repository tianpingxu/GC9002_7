/**
  ******************************************************************************
  * @file    srv_timer_ct.h
  * @author  Zhu Jiahai
  * @version V1.0.0
  * @date    08-07-2021
  * @brief   The data structure proto definition of Timer Serve module.
  *          
  ******************************************************************************
	*/
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SRV_TIMER_CT_H
#define __SRV_TIMER_CT_H

#ifdef __cplusplus
 extern "C" {
#endif
	
/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"

typedef void (* CallbackHandler)(void);

typedef enum
{
    SRV_TIMER_enTimerWaitEraseDone = 0,
    SRV_TIMER_enTimerWaitWriteDone,
	SRV_TIMER_enTimerWaitVerifyDone,
    SRV_TIMER_enTimerWaitJumpDone,
    SRV_TIMER_enTimerWaitUartDone,
    SRV_TIMER_enTimerCount
   
}SRV_TIMER_tenTimerType;


typedef struct 
{	
    bool                            bFlagRun;                       /* Flag to indicate if timer runs or not */
    bool                            bFlagCbExecuted;                /* Flag to indicate if callback function has executed */
    uint32_t                        u32TimerCurCount;               /* current timer count */
    uint32_t                        u32TimerToCount;                /* total timer count */
    CallbackHandler                 pfCbHander;                     /* callback function pointer, to notify user that timer is due */

}SRV_TIMER_tsTimerWorkBuf;




	 

#ifdef __cplusplus
}
#endif

#endif /* __SRV_TIMER_CT_H */





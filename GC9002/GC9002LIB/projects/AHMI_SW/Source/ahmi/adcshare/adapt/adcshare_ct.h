/***************************************************************************
*=====================      Copyright by Su Xian Wei      =================
****************************************************************************
* Title        : adcshare_ct.h
*
* Description  : This file contains the common data type declarations of ADCSHARE
*
* Environment  : MDK / PC Simulation
*
* Responsible  : 
*
* Guidelines   : 
*
* Template name: C++
*
* Data         : 2023-12-18
****************************************************************************/
#ifndef ADCSHARE_CT_H
#define ADCSHARE_CT_H


#ifdef __cplusplus 
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"
#include "adcshare_ci.h"
#include "adcshare_ce.h"

typedef enum
{
    ADCSHR_enErrState_OK = 0,
    ADCSHR_enErrState_InvalidPara,
    ADCSHR_enErrState_Timeout,
    ADCSHR_enErrState_SemErr,
    ADCSHR_enErrState_Total
	
}ADCSHR_tenErrState;



typedef enum
{
    ADCSHR_enCycleSM_Idle = 0,
    ADCSHR_enCycleSM_Work,
    ADCSHR_enCycleSM_Suspend,
    ADCSHR_enCycleSM_Total
	
}ADCSHR_tenCycleSM;


typedef enum
{
    ADCSHR_enPrioSM_Idle = 0,
    ADCSHR_enPrioSM_Work,
    ADCSHR_enPrioSM_Total
	
}ADCSHR_tenPrioSM;


typedef enum
{
    ADCSHR_enChannelState_Idle = 0,
    ADCSHR_enChannelState_Busy,
    ADCSHR_enChannelState_Done,
    ADCSHR_enChannelState_Total
	
}ADCSHR_tenChannelState;


typedef struct 
{
    bool                                bRegistered;                        /* Indicate if current channel to be registered */
	volatile ADCSHR_tenChannelState     enState;                            /* Indicate the state of current channel */
    uint8_t                             u8ContinueTimes;                    /* Indicate continuous sample times */
    uint8_t                             u8BusyCount;                        /* Record busy count, if reach special value, will change busy state */
    #ifndef CHIP_GC9002_NoHyper
	__attribute__ ((aligned(64))) 
	#endif
    uint16_t u16Data[ADCSHR_CHANNEL_DATA_SIZE];
    
}ADCSHR_tstChannelControl;


typedef struct 
{	bool                                bInited;
    volatile uint8_t                    u8ChannelIndex;                          

    volatile ADCSHR_tenCycleSM          enCycleCurSM;                       /* Indicate cycle current state machine */
    volatile ADCSHR_tenCycleSM          enCycleLastSM;                      /* Indicate cycle last state machine */
    volatile ADCSHR_tenPrioSM           enPrioSM;                           /* Indicate Prio state machine */
    ADCSHR_tstChannelControl*           pstChannelControl;            

}ADCSHR_tstWorkBuf;





#ifdef __cplusplus 
}
#endif


#endif


////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI
//
// Create Date:   2017/07/06
// File Name:     Trace.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2017/07/12 by Yu Chunying
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef MIDTIMER__H
#define MIDTIMER__H

#include "gc90x_config.h"

#define TIMENABLE 	1
#define TIMDISNABLE 0

#define TIMSINGLE 1
#define TIMCYCLE 	2

#define NUMOFAHMITIM	8

#ifdef __cplusplus
 extern "C" {
#endif

typedef void (*AHMITimerCB)(void);

typedef struct
{
    uint32_t		mStopValue;     //stop value.  
    uint32_t		mCurValue;      //current value.
    uint8_t			mStatus;        //status for AHMITimer.
    uint8_t			mMode;          //mode for AHMITimer.
}AHMITimer;

typedef struct AHMITimerHandleTag
{
    AHMITimer*		mTimerX;        //pointer to a AHMITimer. 
    uint32_t		mStopValue;     //stop value for a AHMITimer. 
    uint8_t			mMode;          //mode for AHMITimer.		
}AHMITimerHandle;

extern AHMITimer  AHMITIM[NUMOFAHMITIM];    //function array.


#define AHMITIM0 (AHMITIM + 0)
#define AHMITIM1 (AHMITIM + 1)
#define AHMITIM2 (AHMITIM + 2)
#define AHMITIM3 (AHMITIM + 3)
#define AHMITIM4 (AHMITIM + 4)
#define AHMITIM5 (AHMITIM + 5)
#define AHMITIM6 (AHMITIM + 6)
#define AHMITIM7 (AHMITIM + 7)


void AHMITIMCB0(void);
void AHMITIMCB1(void);
void AHMITIMCB2(void);
void AHMITIMCB3(void);
void AHMITIMCB4(void);
void AHMITIMCB5(void);
void AHMITIMCB6(void);
void AHMITIMCB7(void);

void StartAHMITimer(AHMITimerHandle* hTimer);
void StopAHMITimer(AHMITimerHandle* hTimer);
void InitAHMITimer(AHMITimerHandle* hTimer);

void AHMITimerHandlerISR(void);


#ifdef __cplusplus
 }
#endif

#endif

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI小组成员
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

//#include "MidWareDef.h"

#define TIMENABLE 	1
#define TIMDISNABLE 0

#define TIMSINGLE 1
#define TIMCYCLE 	2

#define NUMOFAHMITIM	8

#ifdef __cplusplus
 extern "C" {
#endif
	 
typedef struct TIMTag
{
	uint16_t 				mPeriod;
	uint16_t 				mPrescaler;
	uint16_t 				mClockDivision;
	uint16_t 				mCounterMode;

	uint8_t 				mTimerPrePriority;
	uint8_t 				mTimerSubPriority;
}TIMInit;



typedef void (*AHMITimerCB)(void);

typedef struct
{
	uint32_t					mStopValue;
	uint32_t					mCurValue;
	uint8_t						mStatus;
	uint8_t						mMode;
}AHMITimer;

typedef struct AHMITimerHandleTag
{
	AHMITimer*				mTimerX;
	uint32_t					mStopValue;
	uint8_t						mMode;
		
}AHMITimerHandle;

extern AHMITimer  AHMITIM[NUMOFAHMITIM];


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

void InitAndStartAHMIBaseTimer(void);

void StartAHMITimer(AHMITimerHandle* hTimer);
void StopAHMITimer(AHMITimerHandle* hTimer);
void InitAHMITimer(AHMITimerHandle* hTimer);

uint32_t GetAHMITick(void);

void AHMITimerHandlerISR(void);

//void xTimerInit(TIMHandle* hTimer);
//void xTimersEnable(TIMHandle* hTimer);
//void xTimersDisable(TIMHandle* hTimer);


#ifdef __cplusplus
 }
#endif

#endif

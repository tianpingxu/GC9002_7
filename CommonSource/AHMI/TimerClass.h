////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI
//
// Create Date:   2015/11/17
// File Name:     TimerClass.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Revision 2.01 - modify timer function by Cheng zheyu
// Revision 2.04 - C++, 20160321 by Yu Chunying
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef  TIMERCLASS__H
#define  TIMERCLASS__H

#include "publicType.h"
#include "AHMIBasicStructure.h"
#include "publicDefine.h"

#ifndef PC_SIM_Gen
#include "FreeRTOS.h"
#include "timers.h"
#endif


#ifdef AHMI_CORE

#ifdef __cplusplus
 extern "C" {
#endif
     
#define TimerStop                   0x0
#define TimerStart                  0x1
#define TimerSingleRun              0x0
#define TimerRoundRun               0x2
#define TimerIncrement              0x0
#define TimerDecrement              0x4
#define TimerFirstIncSecondDec		0x8
#define TimerFirstDecSecondInc		0xC

#define TimerInitMsk					0x80000000
#define TimerCurStatusMsk			0x40000000

#define TimerMode    mUserVariables[0]
#define startValue   mUserVariables[1]
#define StopValue    mUserVariables[2]
#define StepValue    mUserVariables[3]
#define IntervalTime mUserVariables[4]

#ifdef PC_SIM_Gen
#define TimerMode_variable		0       
#define startValue_variable		1      
#define StopValue_variable		2       
#define StepValue_variable		3       
#define IntervalTime_variable	4    
#endif

class TimerClass
{
public:
	u32 mUserVariables[5];
	u32 mCurValue;
	u16 mTagID;

#ifndef PC_SIM_Gen
	TimerHandle_t hTimer;
	TimerClass();
	~TimerClass();
	void TimerSetValue(u8 ID, u8 VarialbeID, u32 v);
	void initTimer(u16 TimerID);
	void curTimerStop();
#endif
};

typedef  TimerClass* TimerClassPtr;

#ifndef PC_SIM_Gen
void TimerCallbackFunc( TimerHandle_t pxTimer );
void AnimationTimerCallbackFunc( TimerHandle_t pxTimer );

void TimerProcessTask(void* pvParameters);
void AnimationTimerProcessTask(void* pvParameters);
void TimerProcess(TimerHandle_t pxTimer);
#endif

#ifdef __cplusplus
 }
#endif

#endif

#endif


////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI
//
// Create Date:   2015/11/17
// File Name:     TimerClass.c
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Revision 2.04 - C++, 20160321 by Yu Chunying
// Revision 2.04 - C++, 2018.10.4 modify source of the Timerclass by Yu Chunying.
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#include "publicInclude.h"

#ifdef AHMI_CORE

extern TagClassPtr   TagPtr;
extern TimerClassPtr   TimerPtr;
extern  ConfigInfoClass		ConfigData;
extern AnimationClass  gAnimationClass;
extern IntervalManager gIntervalManager;

TimerClass::TimerClass(void)
{}

TimerClass::~TimerClass(void)
{}

//-----------------------------
// function: InitTimer.
// @brief: Init Timer.
// parameters:
//   @param1 u16 TimerID  timer's ID.
//-----------------------------
void TimerClass::initTimer(u16 TimerID)
{
//	hTimer =  xTimerCreate
//		          (  /* Just a text name, not used by the RTOS kernel. */
//                     "Timer",
//                     /* The timer period in ticks. */
//                     10,
//                     /* The timers will auto-reload themselves when they expire. */
//                     pdTRUE,
//                     /* Assign each timer a unique id equal to its array index. */
//                     ( void * ) TimerID,
//                     /* Each timer calls the same callback when it expires. */
//                     TimerCallbackFunc
//                   );
    /* Set the binding tag.*/
	mTagID = NUM_OF_SYSTEM_TAG  /* The 0~(NUM_OF_SYSTEM_TAG - 1) tag are reserved. */ 
		         + TimerID;     /* One tag for one Timer. */
}

//-----------------------------
// function: TimerSetValue.
// @brief: Set Timer's value.
// parameters:
//   @param1 u8  ID                 ID of the parameter for the Timer.
//   @param2 u8  VarialbeID         VarialbeID of the parameter for the Timer.
//   @param3 u16 v                  value
//-----------------------------
void TimerClass::TimerSetValue(u8 ID, u8 VarialbeID, u32 v)
{
	/* config TimerMode */
	if(VarialbeID == 0)
	{
		mUserVariables[VarialbeID] &= 0xFFFFFFF0;
		mUserVariables[VarialbeID] |= v;

        if((mUserVariables[VarialbeID] & TimerStart) == 0)
		{
			/* Stop timer. */
			curTimerStop();
		}
		else
		{
			if(((mUserVariables[VarialbeID] & TimerInitMsk) == 0)
				|| ((TagPtr->getTag(TimerPtr[ID].mTagID).getValue() < TimerPtr[ID].StopValue) && (TimerPtr[ID].StopValue < TimerPtr[ID].startValue)) 
				|| ((TagPtr->getTag(TimerPtr[ID].mTagID).getValue() > TimerPtr[ID].startValue) && (TimerPtr[ID].StopValue < TimerPtr[ID].startValue))
				|| ((TagPtr->getTag(TimerPtr[ID].mTagID).getValue() > TimerPtr[ID].StopValue) && (TimerPtr[ID].StopValue > TimerPtr[ID].startValue))
				|| ((TagPtr->getTag(TimerPtr[ID].mTagID).getValue() < TimerPtr[ID].startValue) && (TimerPtr[ID].StopValue > TimerPtr[ID].startValue)))
			{
				mUserVariables[VarialbeID] |= TimerInitMsk;
				//TagPtr->getTag(TimerPtr[ID].mTagID).setValue(TimerPtr[ID].startValue, TimerPtr[ID].mTagID);
			}

			if((mUserVariables[VarialbeID] & TimerFirstDecSecondInc) == TimerFirstIncSecondDec)
			{
				mUserVariables[VarialbeID] |= TimerCurStatusMsk;
			}
			else if((mUserVariables[VarialbeID] & TimerFirstDecSecondInc) == TimerFirstDecSecondInc)
			{
				mUserVariables[VarialbeID] &= (~TimerCurStatusMsk);
			}
		}
	}
	else
	{
		mUserVariables[VarialbeID] = v;
	}
}

//-----------------------------
// function: curTimerStop
// @brief: stop this Timer.
// Parameters:
//      void.
//-----------------------------
void TimerClass::curTimerStop()
{
	this->TimerMode &= ~TimerStart;
}

#ifdef PC_SIM
//-----------------------------
// function: TimerCallbackFunc
// @brief:timer call back function. 
//-----------------------------
void TimerCallbackFunc( TimerHandle_t pxTimer )
{
	//u32 TimerID;
	//if(NULL == pxTimer)
	//	return;
	//TimerID = (u32 )pvTimerGetTimerID( pxTimer );
	//	xQueueSendToBack(TimerCmdQueue,&pxTimer,0);
}
#endif

extern uint32_t*            TimerIntervalListPtr;
extern uint32_t             NumberOfTimer;
extern TimerClassPtr		TimerPtr;

//-----------------------------
// function: TimerProcessByHWTimer
// @brief: Change a Timer's count value.
// parameters:
//    @param1 TimerID     ID of a Timer.
//-----------------------------
void TimerProcessByHWTimer(u16 TimerID)
{
	u8  TimerAdd;
	u32 tagValue, CurStatus;
	TimerClassPtr pCurTimer;
	TagClassPtr pBindingTag;
    
    if(TimerID >= NumberOfTimer)
        return;

    /* Get current Timer's pointer. */
	pCurTimer = &TimerPtr[TimerID];
    
    /* Get current Timer's binding tag. */
	pBindingTag = &TagPtr->getTag(pCurTimer->mTagID); 

	tagValue = pBindingTag->getValue();

    /* Get current Timer's mode. */
	TimerAdd = TimerPtr[TimerID].TimerMode & TimerFirstDecSecondInc;

	/* only used by TimerFirstIncSecondDec or TimerFirstDecSecondInc (0x80000000 means timer is adding, 0x00000000 means timer is decreasing). */
	CurStatus = TimerPtr[TimerID].TimerMode & TimerCurStatusMsk;

    /* If the mode is decrement. */
	if(TimerAdd == TimerDecrement)
	{
		if( tagValue >=TimerPtr[TimerID].StopValue && tagValue <= TimerPtr[TimerID].startValue && (s32)tagValue >= 0) //in the duration
		{
			TimerPtr[TimerID].mCurValue = tagValue;
		}
		else if(TimerPtr[TimerID].TimerMode & TimerRoundRun && (tagValue < TimerPtr[TimerID].StopValue || (s32)tagValue <= 0) )//round run
		{
			TimerPtr[TimerID].mCurValue = TimerPtr[TimerID].startValue;
			TagPtr->getTag(TimerPtr[TimerID].mTagID).setValue(TimerPtr[TimerID].mCurValue, TimerPtr[TimerID].mTagID);
			return;
		}
		else if(tagValue < TimerPtr[TimerID].StopValue || tagValue > TimerPtr[TimerID].startValue || (s32)tagValue < 0)//stop timer
		{
			TimerPtr[TimerID].mCurValue = tagValue;
			TimerPtr[TimerID].curTimerStop();
			return;
		}
	} /* End of the mode is decrement. */
    /* If the mode is increse. */
	else if(TimerAdd == TimerIncrement)
	{
		if( tagValue >=TimerPtr[TimerID].startValue && tagValue <= TimerPtr[TimerID].StopValue && (s32)tagValue >= 0) //in the duration
		{
			TimerPtr[TimerID].mCurValue = tagValue;
		}
		else if(TimerPtr[TimerID].TimerMode & TimerRoundRun && (tagValue < TimerPtr[TimerID].startValue || tagValue > TimerPtr[TimerID].StopValue) && (s32)tagValue >= 0)//round run
		{
			TimerPtr[TimerID].mCurValue = TimerPtr[TimerID].startValue;
			TagPtr->getTag(TimerPtr[TimerID].mTagID).setValue(TimerPtr[TimerID].mCurValue, TimerPtr[TimerID].mTagID);
			return;
		}
		else if(tagValue < TimerPtr[TimerID].startValue || tagValue > TimerPtr[TimerID].StopValue || (s32)tagValue < 0)//stop timer
		{
			TimerPtr[TimerID].mCurValue = tagValue;
			TimerPtr[TimerID].curTimerStop();
			return;
		}
	}/* End the mode is increse. */
	/* If the mode is FirstIncSecondDec. */
	else if(TimerAdd == TimerFirstIncSecondDec)
	{
		if( tagValue >= TimerPtr[TimerID].startValue && tagValue <= TimerPtr[TimerID].StopValue && (s32)tagValue >= 0)//in the duration
		{
			TimerPtr[TimerID].mCurValue = tagValue;
		}
		else if((TimerPtr[TimerID].TimerMode & TimerRoundRun) && (!(CurStatus & TimerCurStatusMsk)) && ((tagValue <= TimerPtr[TimerID].startValue) || ((s32)tagValue < 0)))
		{
			TimerPtr[TimerID].TimerMode |= TimerCurStatusMsk;
			TimerPtr[TimerID].mCurValue = (TimerPtr[TimerID].startValue + TimerPtr[TimerID].StepValue);
			TagPtr->getTag(TimerPtr[TimerID].mTagID).setValue(TimerPtr[TimerID].mCurValue, TimerPtr[TimerID].mTagID);
			return;
		}
		else if((TimerPtr[TimerID].TimerMode & TimerRoundRun) && (tagValue >= TimerPtr[TimerID].StopValue) && (CurStatus & TimerCurStatusMsk) && ((s32)tagValue >= 0))
		{
			TimerPtr[TimerID].TimerMode &= (~TimerCurStatusMsk);
			TimerPtr[TimerID].mCurValue = (TimerPtr[TimerID].StopValue - TimerPtr[TimerID].StepValue);
			TagPtr->getTag(TimerPtr[TimerID].mTagID).setValue(TimerPtr[TimerID].mCurValue, TimerPtr[TimerID].mTagID);
			return;
		}
		else if(tagValue >= TimerPtr[TimerID].StopValue && (CurStatus & TimerCurStatusMsk) && (s32)tagValue >= 0)
		{
			TimerPtr[TimerID].TimerMode &= (~TimerCurStatusMsk);
			TimerPtr[TimerID].mCurValue = (TimerPtr[TimerID].StopValue - TimerPtr[TimerID].StepValue);
			TagPtr->getTag(TimerPtr[TimerID].mTagID).setValue(TimerPtr[TimerID].mCurValue, TimerPtr[TimerID].mTagID);
			return;
		}
		else if(tagValue < TimerPtr[TimerID].startValue || (s32)tagValue < 0)//stop timer
		{
			TimerPtr[TimerID].mCurValue = tagValue;
			TimerPtr[TimerID].curTimerStop();
			return;
		}
	}/* End the mode is FirstIncSecondDec. */
	/* If the mode is FirstDecSecondInc. */
	else if(TimerAdd == TimerFirstDecSecondInc)
	{
		if( tagValue >= TimerPtr[TimerID].StopValue && tagValue <= TimerPtr[TimerID].startValue && (s32)tagValue >= 0)//in the duration
		{
			TimerPtr[TimerID].mCurValue = tagValue;
		}
		else if((TimerPtr[TimerID].TimerMode & TimerRoundRun) && (!(CurStatus & TimerCurStatusMsk))  && ((tagValue <= TimerPtr[TimerID].StopValue) || ((s32)tagValue < 0)))
		{
			TimerPtr[TimerID].TimerMode |= TimerCurStatusMsk;
			TimerPtr[TimerID].mCurValue = (TimerPtr[TimerID].StopValue + TimerPtr[TimerID].StepValue);
			TagPtr->getTag(TimerPtr[TimerID].mTagID).setValue(TimerPtr[TimerID].mCurValue, TimerPtr[TimerID].mTagID);
			return;
		}
		else if((TimerPtr[TimerID].TimerMode & TimerRoundRun) && (tagValue >= TimerPtr[TimerID].startValue) && (CurStatus & TimerCurStatusMsk) && ((s32)tagValue >= 0))
		{
			TimerPtr[TimerID].TimerMode &= (~TimerCurStatusMsk);
			TimerPtr[TimerID].mCurValue = (TimerPtr[TimerID].startValue - TimerPtr[TimerID].StepValue);
			TagPtr->getTag(TimerPtr[TimerID].mTagID).setValue(TimerPtr[TimerID].mCurValue, TimerPtr[TimerID].mTagID);
			return;
		}
		else if(tagValue <= TimerPtr[TimerID].StopValue && !(CurStatus & TimerCurStatusMsk) && (s32)tagValue >= 0)//stop timer
		{
			TimerPtr[TimerID].TimerMode |= TimerCurStatusMsk;
			TimerPtr[TimerID].mCurValue = (TimerPtr[TimerID].StopValue + TimerPtr[TimerID].StepValue);
			TagPtr->getTag(TimerPtr[TimerID].mTagID).setValue(TimerPtr[TimerID].mCurValue, TimerPtr[TimerID].mTagID);
			return;
		}
		else if(tagValue > TimerPtr[TimerID].startValue || (s32)tagValue < 0)
		{
			TimerPtr[TimerID].mCurValue = tagValue;
			TimerPtr[TimerID].curTimerStop();
			return;
		}
	}/* End the mode is FirstDecSecondInc. */
	
    /* If the mode is decrement. */
	if(TimerAdd == TimerDecrement)
	{
		TimerPtr[TimerID].mCurValue -=  TimerPtr[TimerID].StepValue;
		if( ( (TimerPtr[TimerID].mCurValue < TimerPtr[TimerID].StopValue) || (s32)TimerPtr[TimerID].mCurValue < 0 ) && !(TimerPtr[TimerID].TimerMode & TimerRoundRun)) //stop
		{
			TimerPtr[TimerID].curTimerStop();
			return;
		}
		else if( (TimerPtr[TimerID].mCurValue == TimerPtr[TimerID].StopValue) && !(TimerPtr[TimerID].TimerMode & TimerRoundRun)) //??stop value
		{
			TimerPtr[TimerID].curTimerStop();
		}
		else if(TimerPtr[TimerID].mCurValue < TimerPtr[TimerID].StopValue || (s32)TimerPtr[TimerID].mCurValue < 0)//round run
		{
			TimerPtr[TimerID].mCurValue = TimerPtr[TimerID].startValue;
		}
		TagPtr->getTag(TimerPtr[TimerID].mTagID).setValue(TimerPtr[TimerID].mCurValue, TimerPtr[TimerID].mTagID);
	} /* End of the mode is decrement. */
    /* If the mode is increse. */
	else if(TimerAdd == TimerIncrement)
	{
		TimerPtr[TimerID].mCurValue +=  TimerPtr[TimerID].StepValue;
		if( (TimerPtr[TimerID].mCurValue > TimerPtr[TimerID].StopValue) && !(TimerPtr[TimerID].TimerMode & TimerRoundRun) ) 
		{
			TimerPtr[TimerID].curTimerStop();
			return;
		}
		else if( (TimerPtr[TimerID].mCurValue == TimerPtr[TimerID].StopValue) && !(TimerPtr[TimerID].TimerMode & TimerRoundRun)) //stop
		{
			TimerPtr[TimerID].curTimerStop();
		}
		else if(TimerPtr[TimerID].mCurValue > TimerPtr[TimerID].StopValue)//round run
		{
			TimerPtr[TimerID].mCurValue = TimerPtr[TimerID].startValue;
		}		
		TagPtr->getTag(TimerPtr[TimerID].mTagID).setValue(TimerPtr[TimerID].mCurValue, TimerPtr[TimerID].mTagID);
	} /* End the mode is increse. */
	/* If the mode is FirstIncSecondDec. */
	else if(TimerAdd == TimerFirstIncSecondDec)
	{
		if(CurStatus & TimerCurStatusMsk)
		{
			TimerPtr[TimerID].mCurValue +=  TimerPtr[TimerID].StepValue;
		}
		else
		{
			TimerPtr[TimerID].mCurValue -=  TimerPtr[TimerID].StepValue;
		}

		if( (TimerPtr[TimerID].mCurValue < TimerPtr[TimerID].startValue) && !(TimerPtr[TimerID].TimerMode & TimerRoundRun) ) 
		{
			TimerPtr[TimerID].curTimerStop();
			return;
		}
		else if( (TimerPtr[TimerID].mCurValue == TimerPtr[TimerID].startValue) && !(TimerPtr[TimerID].TimerMode & TimerRoundRun)) //stop
		{
			TimerPtr[TimerID].curTimerStop();
		}
		else if( (TimerPtr[TimerID].mCurValue >= TimerPtr[TimerID].StopValue) && !(TimerPtr[TimerID].TimerMode & TimerRoundRun) ) 
		{
			TimerPtr[TimerID].mCurValue = TimerPtr[TimerID].StopValue;
		}
		else if((s32)TimerPtr[TimerID].mCurValue > (s32)TimerPtr[TimerID].StopValue)//round run
		{
			TimerPtr[TimerID].mCurValue -=  (TimerPtr[TimerID].StepValue * 2);
			TimerPtr[TimerID].TimerMode &= (~TimerCurStatusMsk);
		}	
		else if((s32)TimerPtr[TimerID].mCurValue < (s32)TimerPtr[TimerID].startValue)//round run
		{
			TimerPtr[TimerID].mCurValue +=  (TimerPtr[TimerID].StepValue * 2);
			TimerPtr[TimerID].TimerMode |= (TimerCurStatusMsk);
		}
		TagPtr->getTag(TimerPtr[TimerID].mTagID).setValue(TimerPtr[TimerID].mCurValue, TimerPtr[TimerID].mTagID);
	}/* End the mode is FirstIncSecondDec. */
	/* If the mode is FirstDecSecondInc. */
	else if(TimerAdd == TimerFirstDecSecondInc)
	{
		if(CurStatus & TimerCurStatusMsk)
		{
			TimerPtr[TimerID].mCurValue +=  TimerPtr[TimerID].StepValue;
		}
		else
		{
			TimerPtr[TimerID].mCurValue -=  TimerPtr[TimerID].StepValue;
		}

		if( (TimerPtr[TimerID].mCurValue > TimerPtr[TimerID].startValue) && !(TimerPtr[TimerID].TimerMode & TimerRoundRun) ) 
		{
			TimerPtr[TimerID].curTimerStop();
			return;
		}
		else if( (TimerPtr[TimerID].mCurValue == TimerPtr[TimerID].startValue) && !(TimerPtr[TimerID].TimerMode & TimerRoundRun)) //stop
		{
			TimerPtr[TimerID].curTimerStop();
		}
		else if( (TimerPtr[TimerID].mCurValue <= TimerPtr[TimerID].StopValue) && !(TimerPtr[TimerID].TimerMode & TimerRoundRun) ) 
		{
			TimerPtr[TimerID].mCurValue = TimerPtr[TimerID].StopValue;
		}
		else if((s32)TimerPtr[TimerID].mCurValue < (s32)TimerPtr[TimerID].StopValue)//round run
		{
			TimerPtr[TimerID].mCurValue +=  (TimerPtr[TimerID].StepValue * 2);
			TimerPtr[TimerID].TimerMode |= (TimerCurStatusMsk);
		}	
		else if((s32)TimerPtr[TimerID].mCurValue > (s32)TimerPtr[TimerID].startValue)//round run
		{
			TimerPtr[TimerID].mCurValue -=  (TimerPtr[TimerID].StepValue * 2);
			TimerPtr[TimerID].TimerMode &= (~TimerCurStatusMsk);
		}
		TagPtr->getTag(TimerPtr[TimerID].mTagID).setValue(TimerPtr[TimerID].mCurValue, TimerPtr[TimerID].mTagID);
	}/* End the mode is FirstDecSecondInc. */
}

#define TIMERACTIONBUFFERSIZE   100
uint8_t TimerActionBuffer[TIMERACTIONBUFFERSIZE];
uint16_t WritePositionOfTAB = 0;
uint16_t ReadPositionOfTAB = 0;
uint16_t NumberOfTAB = 0;

//-----------------------------
// function: AHMITimerForTimerClassHandlerISR.
// @brief: Be called 100 times/s by hardware Timer3, checking which software timer should be changed it's count value.
//-----------------------------
void AHMITimerForTimerClassHandlerISR(uint32_t refreshTime)
{
    uint32_t i = 0;
    
    gIntervalManager.startMaintain(refreshTime);
    
    /*Scan all software timers one by one. */
    for(i = 0; i < NumberOfTimer; i++)
    {
        /*If the software timer is runing. */
        if(TimerPtr[i].TimerMode & TimerStart)
        {
            TimerIntervalListPtr[i] += refreshTime;
            
            /*If it's inter value is beyound threadhold. */
            if(TimerIntervalListPtr[i] >= TimerPtr[i].IntervalTime)
            {
                TimerIntervalListPtr[i] = 0;
//                TimerProcessByHWTimer(i);
                if(NumberOfTAB < TIMERACTIONBUFFERSIZE)
                {
                    /* Store the timer's ID. */
                    TimerActionBuffer[WritePositionOfTAB] = i;
                    
                    /* Change buffer control information. */
                    WritePositionOfTAB++;
                    NumberOfTAB++;
                    if(WritePositionOfTAB >= TIMERACTIONBUFFERSIZE)
                    {
                        WritePositionOfTAB = 0;
                    }
                }
            }
        }
    }
}

#ifdef EMBEDDED
//-----------------------------
// function: TimerProcessTask
// @brief: Simulate hardware Timer in PC, execute 1 time/10ms by sleep.
// parameters:
//    @param1 void*  pvParameters
//-----------------------------
void TimerProcessTask(void* pvParameters)
{
// 	BaseType_t xStatus;
//	TimerHandle_t pxTimer;
//    uint8_t i;
 	for(;;)
 	{   
//        gAnimationClass.Animation_maintain(10);
//        
//        gIntervalManager.maintainExe();
        
        /*For software timers. */
		AHMITimerForTimerClassHandlerISR(10);
        
        /* Find used buffer. */
        while(NumberOfTAB != 0)
        {
            /*For software timers. */
            TimerProcessByHWTimer(TimerActionBuffer[ReadPositionOfTAB]);
            
            /* Change buffer control information. */
            ReadPositionOfTAB++;
            NumberOfTAB--;
            if(ReadPositionOfTAB >= TIMERACTIONBUFFERSIZE)
            {
                ReadPositionOfTAB = 0;
            }
        }
        vTaskDelay(10 / portTICK_RATE_MS);
 	}
}
#endif

#ifdef PC_SIM
u32 AHMITick = 0;
//-----------------------------
// function: TimerProcessTask
// @brief: Simulate hardware Timer in PC, execute 1 time/10ms by sleep.
// parameters:
//    @param1 void*  pvParameters
//-----------------------------
void TimerProcessTask(void* pvParameters)
{
 	BaseType_t xStatus;
	TimerHandle_t pxTimer;
 	for(;;)
 	{
		Sleep(10);
        
		AHMITick += 10;
		
        /* For animation process  task. */
//		gAnimationClass.Animation_maintain(10);
        gAnimationClass.AnimationTrigger(AnimationIntervalTime);

		//gIntervalManager.startMaintain(10);
		
        /*For software timers. */
		AHMITimerForTimerClassHandlerISR(10);

		gIntervalManager.maintainExe();
        
        /* Find used buffer. */
        while(NumberOfTAB != 0)
        {
            /*For software timers. */
            TimerProcessByHWTimer(TimerActionBuffer[ReadPositionOfTAB]);
            
            /* Change buffer control information. */
            ReadPositionOfTAB++;
            NumberOfTAB--;
            if(ReadPositionOfTAB >= TIMERACTIONBUFFERSIZE)
            {
                ReadPositionOfTAB = 0;
            }
        }
		vTaskDelay(2 / portTICK_RATE_MS);
 	}
}
#endif

extern uint32_t AnimationFlag;
//-----------------------------
// function: AnimationTimerProcessTask
// @brief: Simulate  hardware Timer in PC.
// parameters:
//    @param1 void*  pvParameters
//-----------------------------
void AnimationTimerProcessTask(void* pvParameters)
{
// 	BaseType_t xStatus;
//	TimerHandle_t pxTimer;
 	for(;;)
 	{
#ifdef EMBEDDED
//        /*Get the Semaphore. */
//        xSemaphoreTake(interuptSemaphore_animation, portMAX_DELAY);
        
//        if(AnimationFlag > 0)
        {
            //AnimationFlag = 0;  //by Zhu Jiahai
            
            /* Refresh  animation information array. */
//            gAnimationClass.Animation_maintain(AnimationIntervalTime);
            gAnimationClass.AnimationTrigger(AnimationIntervalTime);
            
            gIntervalManager.maintainExe();
        }
#endif  
//  		taskYIELD();
		vTaskDelay(AnimationIntervalTime / portTICK_RATE_MS);
 	}
}
#endif


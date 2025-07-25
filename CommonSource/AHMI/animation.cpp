////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI Group
//
// Create Date:   2016/03/21
// File Name:     DynamicPageClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi
// Additional Comments:
//    animation list create and refresh
//    2016/12/21 modify the structure and add the animation timer
// 
////////////////////////////////////////////////////////////////////////////////
#include "string.h"
#include "publicType.h"
#include "publicInclude.h"
#include "animationDefine.h"
#include "trace.h"
#include "AHMIInterface.h"
#include "player1c1.h"


uint32_t Display_done_fps = 33;

extern u8 animationDuration;
extern QueueHandle_t   ActionInstructionQueue;
extern ConfigInfoClass		ConfigData;


extern DynamicPageClassPtr  gPagePtr;
extern u16             WorkingPageID;

#ifdef EMBEDDED   
extern uint32_t AHMITick;
#endif

//extern QueueHandle_t   TimerCmdQueue;

#ifdef AHMI_CORE

//AnimationMsg  AnimationData[AnimationMsg_Maxnum];

extern QueueHandle_t  AnimationTaskQueue;   //animation task queue.
extern QueueHandle_t PlayerInstructionQueue;  
extern ProjectScreenOptions gScreenOptions;

//-----------------------------
// function: initAnimationMsg
// @brief: Malloc memory for animation massage array and init the array.
// parameters:
//-----------------------------
u32 AnimationClass::initAnimationMsg()
{
    /* Malloc memory for animation massage array. */
	this->pAnimationMsgPtr = (AnimationMsgPtr)csi_kernel_malloc(sizeof(AnimationMsg)* AnimationMsg_Maxnum, NULL);
	if(pAnimationMsgPtr == NULL )
	{
		ERROR_PRINT("animation Class Initial failed.\r\n");
		return (EM_enErrRetModularInit | EM_ErrRetInit_InitAnimationMsgErr);
	}
    
	/* Init animation massage array. */
	memset(this->pAnimationMsgPtr, 0 , sizeof(AnimationMsg)* AnimationMsg_Maxnum);

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function: initAnimationTimer
// @brief: Create a sofeware timer for animation, only for PC_SIM.
// parameters:
// @param: TimerID          ID of the sofeware timer.
//-----------------------------
//funcStatus AnimationClass::initAnimationTimer(u16 TimerID)
//{
//#ifdef PC_SIM
//    /* Create a sofeware timer for, 1 tick implies 10 ms. */
//	aTimer =  xTimerCreate
//		          (  /* Just a text name, not used by the RTOS kernel. */
//                     "aTimer",
//                     /* The timer period in ticks. */
//                     10,
//                     /* The timers will auto-reload themselves when they expire. */
//                     pdTRUE,
//                     /* Assign each timer a unique id equal to its array index. */
//                     ( void * ) TimerID,
//                     /* Each timer calls the same callback when it expires. */
//                     TimerCallbackFunc
//                   );
//#endif
//    
////	xTimerStart(aTimer,0);
//	return AHMI_FUNC_SUCCESS;
//}

//-----------------------------
// function: Animation_maintain
// @brief: Maintain the animation message array.
// parameters:
// @param: refreshTime          interval time between pre called.
//-----------------------------
void AnimationClass::AnimationTrigger(uint32_t refreshTime)
{
	animationTaskQueueMsg i;

    for(i=0;i<AnimationMsg_Maxnum;i++)
	{
        /* If the message element is used. */
        if(pAnimationMsgPtr[i].RefreshType != ANIMATION_REFRESH_NULL)
        {
			#ifdef PC_SIM
			sendPlayerInstruction(i, refreshTime, pAnimationMsgPtr[i].Now_time);
			#else
			sendPlayerInstruction(i, refreshTime, pAnimationMsgPtr[i].Now_Duration);
			#endif
        }
    }
}

//-----------------------------
// function: Animation_maintain
// @brief: Maintain the animation message array.
// parameters:
// @param: refreshTime          interval time between pre called.
//-----------------------------
funcStatus AnimationClass::Animation_maintain(uint32_t id, uint32_t refreshTime)
{
	/* Get the type of widget. */
	u16 widgetType;
#ifdef STATIC_BUFFER_EN
//	RefreshMsg refreshMsg;
#endif
    if(id >= AnimationMsg_Maxnum)
		return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_DataOverrun);
    

    /* If the message element is used. */
    if(pAnimationMsgPtr[id].RefreshType != ANIMATION_REFRESH_NULL)
    {
        /* The animation finished, clear the message. */
        if(pAnimationMsgPtr[id].Now_Duration >= pAnimationMsgPtr[id].Duration)
        {
            /* If the refresh type if refresh page. */
            if(pAnimationMsgPtr[id].RefreshType == ANIMATION_REFRESH_PAGE)
            {
                animationDuration = 0;				
            }
            
            animationTaskQueueHandle(id);
            //call animation finish callback

            animationFinishHanlde(id);

			if(pAnimationMsgPtr[id].Now_Duration < pAnimationMsgPtr[id].Duration && pAnimationMsgPtr[id].RefreshType == ANIMATION_REFRESH_PAGE)
			{
				return AHMI_FUNC_SUCCESS;
			}

            widgetType = pAnimationMsgPtr[id].New_ElementPtr.wptr->WidgetType;

            if(widgetType == SELECTOR)
            {
                if((pAnimationMsgPtr[id].New_ElementPtr.wptr->SELECTORSTATUS & 0x2) == 0)
                {
                    /* Clear the message flag. */
                    pAnimationMsgPtr[id].RefreshType=ANIMATION_REFRESH_NULL;
                }
            }
            else if(widgetType != ON_OFF_TEX && widgetType != ALPHA_SLIDE && widgetType != TEXT_INPUT)
            {
                /* Clear the message flag. */
                pAnimationMsgPtr[id].RefreshType=ANIMATION_REFRESH_NULL;
            }
        }
#ifdef EMBEDDED
        /* Refresh the animation element. */
        else
        {
            /* Clear the count for time. */
            pAnimationMsgPtr[id].Now_time=0;
            
            /* Send ID of the message to the animationTaskQueue, animationTask will do the refresh work. */
            if(animationTaskQueueHandle(id) != AHMI_FUNC_SUCCESS)
				return (animationTaskQueueHandle(id));
        }
#endif
#ifdef PC_SIM
        /* Refresh the animation element, only for PC_SIM. */
        else if(pAnimationMsgPtr[id].Now_time>=pAnimationMsgPtr[id].Interval_time)
        {
            /* Clear the count for interval time. */
            pAnimationMsgPtr[id].Now_time=0;
            
            
            /* Send ID of the message to the animationTaskQueue, animationTask will do the refresh work. */
            if(animationTaskQueueHandle(id) != AHMI_FUNC_SUCCESS)
				return (animationTaskQueueHandle(id));
        }
        else 
        {
            /* Change the count for interval time. */
            pAnimationMsgPtr[id].Now_time += refreshTime;
        }
#endif
    }
    
    return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function: Create_AnimatianData
// @brief: Create a new animation by set a animation message element.
// parameters:
// @param1: A_RefreshType               refresh type
// @param2: A_New_ElementPtr            pointer to the new element
// @param3: A_Old_ElementPtr            pointer to the old element
//-----------------------------
u32 AnimationClass::Create_AnimatianData(	
	u8           A_RefreshType,   
	ElementPtr   A_New_ElementPtr,
	ElementPtr   A_Old_ElementPtr
						  ) 
{
	DynamicPageClassPtr pCurrentPage;
	u8 i;
	u16 widgetType;
//	taskENTER_CRITICAL();
//	if(A_RefreshType==ANIMATION_REFRESH_PAGE && animationDuration == 1)
//	{
//		taskEXIT_CRITICAL();
//		return AHMI_FUNC_SUCCESS;
//	}
    
	pCurrentPage = &gPagePtr[WorkingPageID];
    /* If the refresh type is widget. */
    if(A_RefreshType==ANIMATION_REFRESH_WIDGET)
    {
        /* Get the type of widget. */
        widgetType = A_New_ElementPtr.wptr->WidgetType;
        
		/* if this is meter widget, the new animation should cover the old one. */
        if( widgetType == NUMBOX_TEX || widgetType == TEXTURE_NUM)
        {
			if((A_New_ElementPtr.wptr->INITIALIZED & 0x2) == 0)
			{
				/* Scan all the animation message. */
				for(i=0;i<AnimationMsg_Maxnum;i++) 
				{
					//widget
				    /* find the same refresh element. */
				    if(pAnimationMsgPtr[i].RefreshType == A_RefreshType && A_New_ElementPtr.wptr == pAnimationMsgPtr[i].New_ElementPtr.wptr && A_Old_ElementPtr.wptr == pAnimationMsgPtr[i].Old_ElementPtr.wptr) 
				    {
				        //exist
						//skip
				        
//				        taskEXIT_CRITICAL();
#ifdef AHMI_DEBUG
						ERROR_PRINT("create canvas animation successfully");
#endif
				        return AHMI_FUNC_SUCCESS;
				    }

				}
			}
        }

        /* if this is meter widget, the new animation should cover the old one. */
        if( widgetType == CIRCLESLIDER_TEX || widgetType == METER_TEX || widgetType == PROGBAR_TEX || widgetType == BUTTON_SWITCH || widgetType == GALLERY || widgetType == ON_OFF_TEX || widgetType==SELECTOR || widgetType == CombinationSwitch || widgetType== ALPHA_SLIDE || widgetType == TEXT_INPUT)
        {
            /* Scan all the animation message. */
            for(i=0;i<AnimationMsg_Maxnum;i++) 
            {
				//widget
                /* find the same refresh element. */
                if(pAnimationMsgPtr[i].RefreshType == A_RefreshType && A_New_ElementPtr.wptr == pAnimationMsgPtr[i].New_ElementPtr.wptr && A_Old_ElementPtr.wptr == pAnimationMsgPtr[i].Old_ElementPtr.wptr) 
                {
                    /* replace it. */
                    pAnimationMsgPtr[i].RefreshType = ANIMATION_REFRESH_WIDGET;
                    pAnimationMsgPtr[i].New_ElementPtr.wptr = A_New_ElementPtr.wptr;
                    pAnimationMsgPtr[i].Old_ElementPtr.wptr = A_Old_ElementPtr.wptr ;
                    pAnimationMsgPtr[i].Duration = A_New_ElementPtr.wptr->ANIMATION_DURATION;
					
                    pAnimationMsgPtr[i].Now_Duration =  0;
					pAnimationMsgPtr[i].Now_time = 0;
                    pAnimationMsgPtr[i].Interval_time = 100 / Display_done_fps < Animation_Time ? Animation_Time : 100 / Display_done_fps;
                    #ifdef EMBEDDED
                    pAnimationMsgPtr[i].mOldTicks = AHMITick;
                    #endif
                    
                    /*A_New_ElementPtr.wptr->START_TAG_L = A_New_ElementPtr.wptr->ANIMATIONCURVALUE_L;
                    A_New_ElementPtr.wptr->START_TAG_H = A_New_ElementPtr.wptr->ANIMATIONCURVALUE_H;*/
                    
                     /* Send ID of the message to the animationTaskQueue, animationTask will do the refresh work. */
                    animationTaskQueueHandle(i);
//                    taskEXIT_CRITICAL();
#ifdef AHMI_DEBUG
					ERROR_PRINT("create canvas animation successfully");
#endif
                    return AHMI_FUNC_SUCCESS;
                }

            }
        }
    }

	if (A_RefreshType == ANIMATION_REFRESH_SUBCANVAS)
	{
		for (int i = 0; i < AnimationMsg_Maxnum; i++)
		{
			//subcanvas animation with same canvas
			if(pAnimationMsgPtr[i].RefreshType == A_RefreshType  && A_New_ElementPtr.scptr->attachCanvas == pAnimationMsgPtr[i].New_ElementPtr.scptr->attachCanvas ) 
			{
				/* replace it. */
				pAnimationMsgPtr[i].RefreshType = ANIMATION_REFRESH_SUBCANVAS;
				pAnimationMsgPtr[i].New_ElementPtr.scptr = A_New_ElementPtr.scptr;
				pAnimationMsgPtr[i].Old_ElementPtr.scptr = A_Old_ElementPtr.scptr ;
				pAnimationMsgPtr[i].Duration = pCurrentPage->pCanvasList[A_New_ElementPtr.scptr->attachCanvas].subcanvasAnimationDuration;
				pAnimationMsgPtr[i].Now_Duration = 0;
				pAnimationMsgPtr[i].Now_time = 0;
				pAnimationMsgPtr[i].Interval_time = 100 / Display_done_fps < Animation_Time ? Animation_Time : 100 / Display_done_fps;


				#ifdef EMBEDDED
				pAnimationMsgPtr[i].mOldTicks = AHMITick;
				#endif
                    
				/*A_New_ElementPtr.wptr->START_TAG_L = A_New_ElementPtr.wptr->ANIMATIONCURVALUE_L;
				A_New_ElementPtr.wptr->START_TAG_H = A_New_ElementPtr.wptr->ANIMATIONCURVALUE_H;*/
                    
					/* Send ID of the message to the animationTaskQueue, animationTask will do the refresh work. */
				animationTaskQueueHandle(i);
//				taskEXIT_CRITICAL();
		#ifdef AHMI_DEBUG
				ERROR_PRINT("create canvas animation successfully");
		#endif
				return AHMI_FUNC_SUCCESS;
			}
		}
	}

	if (A_RefreshType == ANIMATION_REFRESH_PAGE)
	{
		for (int i = 0; i < AnimationMsg_Maxnum; i++)
		{
			//page animation
			//only one at one time
			if(pAnimationMsgPtr[i].RefreshType == A_RefreshType  ) 
			{
				/* replace it. */
				pAnimationMsgPtr[i].RefreshType = ANIMATION_REFRESH_PAGE;
				pAnimationMsgPtr[i].New_ElementPtr.pageptr = A_New_ElementPtr.pageptr;
				pAnimationMsgPtr[i].Old_ElementPtr.pageptr = A_Old_ElementPtr.pageptr ;
				pAnimationMsgPtr[i].Duration = pAnimationMsgPtr[i].New_ElementPtr.pageptr->pageAnimationDuration;
				pAnimationMsgPtr[i].Now_Duration = 0;
				pAnimationMsgPtr[i].Now_time = 0;
				pAnimationMsgPtr[i].Interval_time = 100 / Display_done_fps < Animation_Time ? Animation_Time : 100 / Display_done_fps;


				#ifdef EMBEDDED
				pAnimationMsgPtr[i].mOldTicks = AHMITick;
				#endif
                    
				/*A_New_ElementPtr.wptr->START_TAG_L = A_New_ElementPtr.wptr->ANIMATIONCURVALUE_L;
				A_New_ElementPtr.wptr->START_TAG_H = A_New_ElementPtr.wptr->ANIMATIONCURVALUE_H;*/
                    
					/* Send ID of the message to the animationTaskQueue, animationTask will do the refresh work. */
				animationTaskQueueHandle(i);
//				taskEXIT_CRITICAL();
		#ifdef AHMI_DEBUG
				ERROR_PRINT("create canvas animation successfully");
		#endif
				return AHMI_FUNC_SUCCESS;
			}
		}
	}
	
    for(i=0;i<AnimationMsg_Maxnum;i++)
    {
        if(pAnimationMsgPtr[i].RefreshType ==ANIMATION_REFRESH_NULL)
        {
            /* If the refresh type is page. */
            if(A_RefreshType==ANIMATION_REFRESH_PAGE)
            {                         
#ifdef AHMI_DEBUG
                ERROR_PRINT("create page animation successfully");
#endif
                /* check the data information null. */
                if((A_New_ElementPtr.pageptr == NULL) || (A_Old_ElementPtr.pageptr == NULL))
                {
//					taskEXIT_CRITICAL();
                    ERROR_PRINT("error when creating page animation: page pointer null");
					return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_NULLPointerErr);
                }
				
				/* check the data information out of boundary. */
                if(((A_New_ElementPtr.pageptr - gPagePtr) >=  ConfigData.NumofPages) || 
				   ((A_Old_ElementPtr.pageptr - gPagePtr) >=  ConfigData.NumofPages))
                {
//					taskEXIT_CRITICAL();
                    ERROR_PRINT("error when creating page animation: page pointer out of boundary");
					return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_DataOverrun);
                }
				
                animationDuration = 1;
                pAnimationMsgPtr[i].RefreshType = ANIMATION_REFRESH_PAGE;
                pAnimationMsgPtr[i].New_ElementPtr.pageptr=A_New_ElementPtr.pageptr ;
                pAnimationMsgPtr[i].Old_ElementPtr.pageptr=A_Old_ElementPtr.pageptr ;
                //pAnimationMsgPtr[i].Total_Frame = pAnimationMsgPtr[i].New_ElementPtr.pageptr->mtotoalFrame;
                pAnimationMsgPtr[i].Duration = pAnimationMsgPtr[i].New_ElementPtr.pageptr->pageAnimationDuration;
            }
            /* If the refresh type is canvas. */
            else if(A_RefreshType==ANIMATION_REFRESH_CANVAS)
            { 
                if((A_New_ElementPtr.cptr == NULL) || (A_Old_ElementPtr.cptr == NULL))
                {
//					taskEXIT_CRITICAL();
                    ERROR_PRINT("error when creating canvas animation: canvas pointer null");
					return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_NULLPointerErr);
                }
				if(((A_New_ElementPtr.cptr - pCurrentPage->pCanvasList) >=  pCurrentPage->mNumOfCanvas) || 
				   ((A_Old_ElementPtr.cptr - pCurrentPage->pCanvasList) >=  pCurrentPage->mNumOfCanvas))
                {
//					taskEXIT_CRITICAL();
                    ERROR_PRINT("error when creating canvas animation: canvas pointer out of boundary");
					return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_DataOverrun);
                }
#ifdef AHMI_DEBUG
                ERROR_PRINT("create canvas animation successfully");
#endif
                pAnimationMsgPtr[i].RefreshType = ANIMATION_REFRESH_CANVAS;
                pAnimationMsgPtr[i].New_ElementPtr = A_New_ElementPtr;
                pAnimationMsgPtr[i].Old_ElementPtr = A_Old_ElementPtr ;
                //pAnimationMsgPtr[i].Total_Frame = gPagePtr[WorkingPageID].pAnimationActionList[ A_New_ElementPtr.cptr->curCustomAnimationPtr].mTotalFrame;
                pAnimationMsgPtr[i].Duration = pCurrentPage->pAnimationKeyFrameList[pCurrentPage->pAnimationActionList[A_New_ElementPtr.cptr->curCustomAnimationPtr].keyFrameStopID].mTime;
            }
            /* If the refresh type is subcancas. */
            else if(A_RefreshType==ANIMATION_REFRESH_SUBCANVAS)
            { 
                if((A_New_ElementPtr.scptr == NULL) || (A_Old_ElementPtr.scptr == NULL))
                {
//					taskEXIT_CRITICAL();
                    ERROR_PRINT("error when creating subcanvas animation: subcanvas pointer null");
					return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_NULLPointerErr);
                }
				if(((A_New_ElementPtr.scptr - pCurrentPage->pSubCanvasList) >=  pCurrentPage->mNumOfSubCanvas) || 
				   ((A_Old_ElementPtr.scptr - pCurrentPage->pSubCanvasList) >=  pCurrentPage->mNumOfSubCanvas))
                {
//					taskEXIT_CRITICAL();
                    ERROR_PRINT("error when creating subcanvas animation: subcanvas pointer out of boundary");
					return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_DataOverrun);
                }
#ifdef AHMI_DEBUG
					ERROR_PRINT("create subcanvas animation successfully");
#endif
                pAnimationMsgPtr[i].RefreshType = ANIMATION_REFRESH_SUBCANVAS;
                pAnimationMsgPtr[i].New_ElementPtr.scptr = A_New_ElementPtr.scptr;
                pAnimationMsgPtr[i].Old_ElementPtr.scptr = A_Old_ElementPtr.scptr ;
                //pAnimationMsgPtr[i].Total_Frame = gPagePtr[WorkingPageID].pCanvasList[A_New_ElementPtr.scptr->attachCanvas].subcanvasAnimationFrame;
                pAnimationMsgPtr[i].Duration = pCurrentPage->pCanvasList[A_New_ElementPtr.scptr->attachCanvas].subcanvasAnimationDuration;
            }
            /* If the refresh type is widget. */
            else if(A_RefreshType == ANIMATION_REFRESH_WIDGET)
            {
				if((A_New_ElementPtr.wptr == NULL) || (A_Old_ElementPtr.wptr == NULL))
                {
//					taskEXIT_CRITICAL();
                    ERROR_PRINT("error when creating widget animation: widget pointer null");
                    return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_NULLPointerErr);
                }
//				if(((A_New_ElementPtr.wptr - pCurrentPage->pWidgetList) >=  pCurrentPage->mNumOfWidget) || 
//				   ((A_Old_ElementPtr.wptr - pCurrentPage->pWidgetList) >=  pCurrentPage->mNumOfWidget))
				if(!gPagePtr[A_New_ElementPtr.wptr->ATTATCH_PAGE].shouldShow)
                {
//					taskEXIT_CRITICAL();
                    ERROR_PRINT("error when creating widget animation: widget pointer out of boundary");
					return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_DataOverrun);
                }
#ifdef AHMI_DEBUG
				ERROR_PRINT("create widget animation successfully");
#endif
                pAnimationMsgPtr[i].RefreshType = ANIMATION_REFRESH_WIDGET;
                pAnimationMsgPtr[i].New_ElementPtr.wptr = A_New_ElementPtr.wptr;
                pAnimationMsgPtr[i].Old_ElementPtr.wptr = A_Old_ElementPtr.wptr ;
                //pAnimationMsgPtr[i].Total_Frame = ( (A_New_ElementPtr.wptr->TOTALFRAME_AND_NOWFRAME) >> 8 );
                pAnimationMsgPtr[i].Duration = A_New_ElementPtr.wptr->ANIMATION_DURATION;
            }
            /* If the refresh type is undefine. */
            else
            {
//                taskEXIT_CRITICAL();
				return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_RefreshTypeErr);
            }

            //pAnimationMsgPtr[i].Now_Frame =0;
            pAnimationMsgPtr[i].Now_Duration = 0;
            pAnimationMsgPtr[i].Now_time = 0;
            
            pAnimationMsgPtr[i].Interval_time =100/Display_done_fps<Animation_Time?Animation_Time:100/Display_done_fps;
            #ifdef EMBEDDED
            pAnimationMsgPtr[i].mOldTicks = AHMITick;
            #endif
            
            /* Send ID of the message to the animationTaskQueue, animationTask will do the refresh work. */
            animationTaskQueueHandle(i);
			
//            taskEXIT_CRITICAL();
            /* success. */
            return AHMI_FUNC_SUCCESS;
        }
	}

#ifdef AHMI_DEBUG
		ERROR_PRINT("create animation failure: no enough place for animation member");
#endif
//	taskEXIT_CRITICAL();
	
    /* space overrun. */
	return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_SpaceOverrun);
}

//-----------------------------
// function: Get_AnimatianData_Index
// @brief: Get an existed animation by set a animation message element.
// parameters:
// @param1: A_RefreshType               refresh type
// @param2: A_New_ElementPtr            pointer to the new element
// @param3: A_Old_ElementPtr            pointer to the old element
//-----------------------------
s32 AnimationClass::Get_AnimatianData_Index(	
	u8           A_RefreshType,   
	ElementPtr   A_New_ElementPtr,
	ElementPtr   A_Old_ElementPtr
						  ) 
{
//	DynamicPageClassPtr pCurrentPage;
	u8 i;
	u16 widgetType;
//	taskENTER_CRITICAL();
	
//	pCurrentPage = &gPagePtr[WorkingPageID];
    /* If the refresh type is widget. */
    if(A_RefreshType==ANIMATION_REFRESH_WIDGET)
    {
        /* Get the type of widget. */
        widgetType = A_New_ElementPtr.wptr->WidgetType;
        
        /* if this is meter widget, the new animation should cover the old one. */
        if( widgetType == METER_TEX || widgetType == PROGBAR_TEX || widgetType == BUTTON_SWITCH || widgetType == GALLERY || widgetType == ON_OFF_TEX || widgetType==SELECTOR || widgetType == CombinationSwitch)
        {
            /* Scan all the animation message. */
            for(i=0;i<AnimationMsg_Maxnum;i++) 
            {
				//widget
                /* find the same refresh element. */
                if(pAnimationMsgPtr[i].RefreshType == A_RefreshType && A_New_ElementPtr.wptr == pAnimationMsgPtr[i].New_ElementPtr.wptr && A_Old_ElementPtr.wptr == pAnimationMsgPtr[i].Old_ElementPtr.wptr) 
                {
//                    taskEXIT_CRITICAL();
					return i;
                }
            }
        }
    }

	if (A_RefreshType == ANIMATION_REFRESH_SUBCANVAS)
	{
		for (int i = 0; i < AnimationMsg_Maxnum; i++)
		{
			//subcanvas animation with same canvas
			if(pAnimationMsgPtr[i].RefreshType == A_RefreshType  && A_New_ElementPtr.scptr->attachCanvas == pAnimationMsgPtr[i].New_ElementPtr.scptr->attachCanvas ) 
			{
//				taskEXIT_CRITICAL();
				return i;
			}
		}
	}

	if (A_RefreshType == ANIMATION_REFRESH_PAGE)
	{
		for (int i = 0; i < AnimationMsg_Maxnum; i++)
		{
			//page animation
			//only one at one time
			if(pAnimationMsgPtr[i].RefreshType == A_RefreshType  ) 
			{
//				taskEXIT_CRITICAL();
				return i;
			}
		}
	}
//	taskEXIT_CRITICAL();
	
	/* failure. */
	return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_RefreshTypeErr);
}

funcStatus AnimationClass::clearAnimationWithIdx(s32 idx)
{
	pAnimationMsgPtr[idx].RefreshType = ANIMATION_REFRESH_NULL;
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name： executeAnimationWithElapsedTime
// 动画队列处理任务
//  
//     @param1 animationTaskQueueMsg i 队列消息
// addtional comments :
//   
//-----------------------------
funcStatus AnimationClass::executeAnimationWithElapsedTime(animationTaskQueueMsg i)
{
#ifdef EMBEDDED    
    uint32_t ticksForAnimation = 0;
    
    int32_t intervalTicks = 0;
#endif
	
	AnimationMsgPtr curAnimationData = &pAnimationMsgPtr[i];
	
#ifdef PC_SIM
	curAnimationData->Now_Duration += (u16)(curAnimationData->Now_time) * 10;
	animationTaskQueueHandle(i);
	curAnimationData->Now_Duration -= (u16)(curAnimationData->Now_time) * 10;
#endif
	
#ifdef EMBEDDED     
    ticksForAnimation = AHMITick;
    
    intervalTicks = ticksForAnimation - curAnimationData->mOldTicks;
    
    if(intervalTicks < 0)
    {
        intervalTicks = 0 - intervalTicks;
    }
	
	curAnimationData->mOldTicks = ticksForAnimation;
#endif
	
	/* Update Now_Duration. */
	#ifdef PC_SIM
		curAnimationData->Now_Duration = (u16)(curAnimationData->Now_Duration + curAnimationData->Interval_time * 10);
	#endif
	#ifdef EMBEDDED
		curAnimationData->Now_Duration = (u16)(curAnimationData->Now_Duration + intervalTicks);
	#endif

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function: animationTaskQueueHandle
// @brief: Refresh  animation work.
// parameters:
// @param1: i               ID of a animation message element.
//-----------------------------
u32 AnimationClass::animationTaskQueueHandle(animationTaskQueueMsg i)
{
	AnimationMsgPtr curAnimationData = &pAnimationMsgPtr[i];

#ifdef EMBEDDED    
    uint32_t ticksForAnimation = 0;
    
    int32_t intervalTicks = 0;
#endif
    
	if(curAnimationData == NULL)
	{
		ERROR_PRINT("error in animationTaskQueueHandler: pointer null");
		return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_NULLPointerErr);
	}
	
	if(i >= AnimationMsg_Maxnum)
	{
		ERROR_PRINT("error in animationTaskQueueHandler: space overrun");
		return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_SpaceOverrun);
	}

#ifdef EMBEDDED     
    ticksForAnimation = AHMITick;
    
    intervalTicks = ticksForAnimation - curAnimationData->mOldTicks;
    
    if(intervalTicks < 0)
    {
        intervalTicks = 0 - intervalTicks;
    }
    
    curAnimationData->mOldTicks = ticksForAnimation;
#endif
	
	if (curAnimationData->Now_Duration > curAnimationData->Duration)
	{
		curAnimationData->Now_Duration = curAnimationData->Duration;
	}
	
	switch(curAnimationData->RefreshType)
	{
        /* Page animation. */
        case(ANIMATION_REFRESH_PAGE):
            //set aTag of page
            //taskENTER_CRITICAL();
            curAnimationData->New_ElementPtr.pageptr->setATag(curAnimationData->Now_Duration); 
            //taskEXIT_CRITICAL();
            break;
        /* Canvas animation. */
        case(ANIMATION_REFRESH_CANVAS):
            //taskENTER_CRITICAL();
            curAnimationData->New_ElementPtr.cptr->setATag(curAnimationData->Now_Duration); 
            //taskEXIT_CRITICAL();
            break;
        /* Subcanvas animation. */
        case(ANIMATION_REFRESH_SUBCANVAS):
            //taskENTER_CRITICAL();
			curAnimationData->New_ElementPtr.scptr->setAtag(curAnimationData->Now_Duration, curAnimationData->Old_ElementPtr.scptr);
            //taskEXIT_CRITICAL();
            break;
        /* Widget animation. */
        case(ANIMATION_REFRESH_WIDGET):
            //taskENTER_CRITICAL();
            curAnimationData->New_ElementPtr.wptr->setATag(curAnimationData->Now_Duration);
            //taskEXIT_CRITICAL();
            break;
        default:
            ERROR_PRINT("error in animationTaskQueueHandler: current type of animation donnot exist");
			return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_RefreshTypeErr);
	}
	
	/* Update Now_Duration. */
	#ifdef PC_SIM
		curAnimationData->Now_Duration = (u16)(curAnimationData->Now_Duration + curAnimationData->Interval_time * 10);
	#endif
	#ifdef EMBEDDED
		curAnimationData->Now_Duration = (u16)(curAnimationData->Now_Duration + intervalTicks);
	#endif

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function: animationFinishHanlde
// @brief: Refresh  animation work.
// parameters:
// @param1: i               ID of a animation message element.
//-----------------------------
u32 AnimationClass::animationFinishHanlde(animationTaskQueueMsg i)
{
	AnimationMsgPtr curAnimationData = &pAnimationMsgPtr[i];

#ifdef EMBEDDED    
    uint32_t ticksForAnimation = 0;
    
    int32_t intervalTicks = 0;
#endif
    
	if(curAnimationData == NULL)
	{
		ERROR_PRINT("error in animationFinishHanlde: pointer null");
		return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_NULLPointerErr);
	}
	
	if(i >= AnimationMsg_Maxnum)
	{
		ERROR_PRINT("error in animationFinishHanlde: space overrun");
		return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_SpaceOverrun);
	}
	
#ifdef EMBEDDED     
    ticksForAnimation = AHMITick;
    
    intervalTicks = ticksForAnimation - curAnimationData->mOldTicks;
    
    if(intervalTicks < 0)
    {
        intervalTicks = 0 - intervalTicks;
    }
    
    curAnimationData->mOldTicks = ticksForAnimation;
#endif
	
	switch(curAnimationData->RefreshType)
	{
        /* Subcanvas animation. */
        case(ANIMATION_REFRESH_SUBCANVAS):
            //taskENTER_CRITICAL();
			curAnimationData->New_ElementPtr.scptr->animationFinishCallback(curAnimationData->Old_ElementPtr.scptr);
            //taskEXIT_CRITICAL();
            break;
        /* Widget animation. */
        default:
            return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_RefreshTypeErr);
	}

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function: animationQueueReset
// @brief: Clear all animation message.
// parameters:
//-----------------------------
funcStatus AnimationClass::animationQueueReset(void)
{
	u8 i;
	for(i=0;i<AnimationMsg_Maxnum;i++)
	{
		if(pAnimationMsgPtr[i].RefreshType != ANIMATION_REFRESH_WIDGET)
		{
			pAnimationMsgPtr[i].RefreshType = ANIMATION_REFRESH_NULL;
		}
	}
	animationDuration = 0;
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function: sendPlayerInstruction
// @brief: send instruction to player command queue.
// parameters:id
//-----------------------------
funcStatus AnimationClass::sendPlayerInstruction(uint32_t id, uint32_t refreshTime, uint32_t nowtime)
{
    PLAR_tstAHMICmd stAHMICmd;
    
    if(id >= AnimationMsg_Maxnum)
		return (EM_enErrRetModularAnimation | EM_ErrRetAnimation_DataOverrun);
    
    memset(&stAHMICmd, 0, sizeof(stAHMICmd));
    stAHMICmd.u32CmdType = enCmdType_TriggerAnimation;
    stAHMICmd.u32PageID = WorkingPageID;
    stAHMICmd.u32Para[0] = id;
    stAHMICmd.u32Para[1] = refreshTime;
	stAHMICmd.u32Para[2] = nowtime;

    if(gScreenOptions.enable)
		{
			stAHMICmd.screenType = RefreshScreenMain;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}

			vTaskDelay(30 / portTICK_RATE_MS);
			stAHMICmd.screenType = RefreshScreenSub;
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
		else
		{
			if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, 0, portMAX_DELAY) != 0)
			{
				return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
			}
		}
    
    return AHMI_FUNC_SUCCESS;
}


u32 AnimationClass::getAnimationRefreshType(u32 id)
{
    if(id >= AnimationMsg_Maxnum)
        return ANIMATION_REFRESH_NULL;
    
    return pAnimationMsgPtr[id].RefreshType;
}

WidgetClassPtr AnimationClass::getAnimationRefreshElementPtr(u32 id)
{
    if(id >= AnimationMsg_Maxnum)
        return ANIMATION_REFRESH_NULL;
    
    return pAnimationMsgPtr[id].New_ElementPtr.wptr;
}
#endif

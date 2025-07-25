////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     drawImmediately_cd.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#include "publicType.h"
#include "refreshQueueHandler_c1.h"
#include "refreshQueueHandler_cd.h"


#ifndef REFRESHQUEUEHANDLER__H
#define REFRESHQUEUEHANDLER__H

#ifdef __cplusplus
 extern "C" {
#endif

#include "publicDefine.h"
#ifdef AHMI_CORE

typedef enum
{
    RefreshScreenSingle = 0,
	RefreshScreenMain,
	RefreshScreenSub
   
}RefreshScreenTypes;
	 
typedef enum
{
    EM_ErrRetRefreshQueue_NULLPointerErr = 1,
	EM_ErrRetRefreshQueue_RefreshTypeErr,
	EM_ErrRetRefreshQueue_DataOverrun,
	EM_ErrRetRefreshQueue_QueueSendErr
   
}EM_ErrRetRefreshQueue;
	 
union ElementPtr //the union of page, canvas, subcanvas,widget,texture pointer
{
    DynamicPageClassPtr pageptr; //page pointer
	WidgetClassPtr      wptr;    //widget pointer
	CanvasClassPtr      cptr;    //canvas pointer
	SubCanvasClassPtr   scptr;   //subcanvas pointer
	TextureClassPtr     tptr;  
};

typedef struct Animation        //structure for animation message
{
	ElementPtr      New_ElementPtr;     //new element pointer
	ElementPtr      Old_ElementPtr;     //old element pointer
    uint32_t        mOldTicks;
	u16			    Now_Duration;       //current duration
	u16			    Duration;           //duration
    u8              RefreshType;        //type of animation
	u8              Now_time  ;         //current time
	u8              Interval_time;      //interval time for refresh
}AnimationMsg;

typedef AnimationMsg* AnimationMsgPtr;

typedef struct Refresh              //structure for refresh
{
	u8           mElementType;      //element type 
	ElementPtr   mElementPtr;	    //element pointer 
}RefreshMsg;

typedef RefreshMsg* RefreshMsgPtr;

typedef u8 animationTaskQueueMsg;

funcStatus computingTileBoxFromAnimationMsg(AnimationMsg* pAnimationMsg, TileBoxClassPtr SourceBox);

u32 drawimmediately(RefreshMsg*);
u32 drawimmediatelyForPlayer(RefreshMsg* refreshMsg, RefreshScreenTypes screenType);

void PageRefreshTask(void* pvParameters);

#ifdef EMBEDDED

u32 DrawTheFirstFrame(void);

#ifdef VERSION_2
void FivePointSwitchBufferAndDraw(uint32_t* pSourceshift);
#endif
/**
  * @brief  AHMI done singal interrupt service function
  * @note
  * @param  None
  * @retval None
  */
void ExtiTrigAhmiDoneHandler( void );

/**
  * @brief  Display done singal interrupt service function
  * @note
  * @param  None
  * @retval None
  */
void ExtiTrigDisplayDoneHandler( void );

/**
  * @brief  Refresh animation interrupt service function
  * @note
  * @param  None
  * @retval None
  */
void ExtiTrigRefreshAnimationHandler( void );

/**
  * @brief  Switch source buffer index and notify FPGA start draw interrupt service function
  * @note
  * @param  None
  * @retval None
  */
void ExtiTrigDrawHandler( void );

/**
  * @brief  Update tag interrupt service function
  * @note
  * @param  None
  * @retval None
  */
void ExtiTrigTagUpdateHandler( void );

void REF_vInit(void);

void REF_vRefreshAnimationHandler( void );
void StartDrawingTimer( void );


#endif

#if (defined WHOLE_TRIBLE_BUFFER) || (defined PARTIAL_TRIBLE_BUFFER)
void swtichDispBufferTask(void* pvParameters);
#endif
#if (defined WHOLE_TRIBLE_BUFFER) || (defined PARTIAL_TRIBLE_BUFFER)
void screenUpdateTask(void* pvParameters);
#endif

#endif

#ifdef __cplusplus
 }
#endif

#endif

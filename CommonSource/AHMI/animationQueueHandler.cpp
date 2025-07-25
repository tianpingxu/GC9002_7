////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     DynamicPageClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    handle the animation queue
//    add comments by zuz 2018/10/29
// 
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "animationDefine.h"

#ifdef AHMI_CORE

extern k_msgq_handle_t  AnimationTaskQueue;///////////////////the animation task queue, storing the next handling animation
extern k_msgq_handle_t   RefreshQueueWithoutDoubleBuffer;
extern AnimationClass  gAnimationClass;


//-----------------------------
// name�� sendToAnimationQueue
// send elements to animation queue
// @param A_RefreshType : the type of animation, it could be ANIMATION_REFRESH_NULL/ ANIMATION_REFRESH_PAGE/ ANIMATION_REFRESH_SUBCANVAS/ ANIMATION_REFRESH_CANVAS/ ANIMATION_REFRESH_WIDGET/ ANIMATION_REFRESH_DOUBLE_BUFFER / ANIMAITON_REFRESH_STATIC_BUFFER
// @param A_New_ElementPtr : the new element that moves in
// @param A_Old_ElementPtr : the old element that moves away
//  
// addtional comments :
//   
//-----------------------------
funcStatus sendToAnimationQueue(
	u8           A_RefreshType,    
	ElementPtr   A_New_ElementPtr, 
	ElementPtr   A_Old_ElementPtr  
	)
{
	AnimationMsg curAnimationMsg;
	curAnimationMsg.RefreshType = A_RefreshType;
	curAnimationMsg.New_ElementPtr = A_New_ElementPtr;
	curAnimationMsg.Old_ElementPtr = A_Old_ElementPtr;
	gAnimationClass.Create_AnimatianData(curAnimationMsg.RefreshType, curAnimationMsg.New_ElementPtr, curAnimationMsg.Old_ElementPtr);
	return AHMI_FUNC_SUCCESS;
}
//-----------------------------
// name�� animationQueueTask
// the task to handle animation queue
//  
// addtional comments :
//   
//-----------------------------
void animationQueueTask(void* pvParameters)
{
	AnimationMsg curAnimationMsg;
	BaseType_t xStatus;
	xStatus=csi_kernel_msgq_get(AnimationTaskQueue,
 								(void*)&curAnimationMsg,
 								portMAX_DELAY);
 	if(xStatus != 0)
 	{
 		return;
 	}
	gAnimationClass.Create_AnimatianData(curAnimationMsg.RefreshType, curAnimationMsg.New_ElementPtr, curAnimationMsg.Old_ElementPtr);
}


#endif

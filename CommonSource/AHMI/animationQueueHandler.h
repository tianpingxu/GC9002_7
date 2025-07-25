////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     DynamicPage.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    task to handle the animation queue
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef ANIMATIONQUEUEHANDLER__H
#define ANIMATIONQUEUEHANDLER__H
#include "publicDefine.h"
#ifdef AHMI_CORE

//-----------------------------
// name£º animationQueueTask
// the task to handle animation queue
//  
// addtional comments :
//   
//-----------------------------
void animationQueueTask(void*);

//-----------------------------
// name£º sendToAnimationQueue
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
	);
#endif

#endif

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     DynamicPageClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/12/27 by Zhou Yuzhi 
// Additional Comments:
//    animation action
// 
////////////////////////////////////////////////////////////////////////////////
#include "publicType.h"
#include "publicInclude.h"
#include "animationDefine.h"
#include "trace.h"


extern AnimationClass  gAnimationClass;

//-----------------------------
// name£º triggerAnimation
// trigger the element's current animation
// now there is only canvas animation
//  
//  @param1 ElementPtr curElementPtr type of element
//  @param2 u8 elememtType           pointer of element
// addtional comments :
// none
//-----------------------------
funcStatus AnimationActionClass::triggerAnimation(ElementPtr curElementPtr, u8 elememtType)
{
	if(elememtType == ANIMATION_REFRESH_CANVAS) //canvas action
	{
		curElementPtr.cptr->curCustomAnimationPtr = mAnimationID;
		return gAnimationClass.Create_AnimatianData(ANIMATION_REFRESH_CANVAS, curElementPtr, curElementPtr);
	}
	else 
		return AHMI_FUNC_FAILURE;
}

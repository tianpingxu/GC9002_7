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
#ifndef ANIMATION_ACTION__H
#define ANIMATION_ACTION__H

#include "publicType.h"
//#include "refreshQueueHandler.h"
#include "animationDefine.h"
#include "trace.h"

union ElementPtr;


struct AnimationKeyFrame
{
	s16 mTranslateX;        //point x 11.4
	s16 mTranslateY;        //point y 11.4
	u16 mScaleX;		    //scale x  6.10
	u16 mScaleY;		    //scale y  6.10
	s16 mScaleCenterX;
	s16 mScaleCenterY;
	u16 mScaleCenterXTag;
	u16 mScaleCenterYTag;
	
	u16 mTranslateXTag;                
	u16 mTranslateYTag;
	u16 mScaleXTag;
	u16 mScaleYTag;
	

	s16 mRotateX;
	s16 mRotateY;
	s16 mRotateRadius;		//1.11.4
	
	s16 mProjectX;			//1.11.4
	s16 mProjectY;			//1.11.4
	

	u16 mRotateXTag;
	u16 mRotateYTag;
	u16 mRotateRadiusTag;
	
	u16 mProjectXTag;
	u16 mProjectYTag;
	
	u16 mTime;              //time of animation
	u8  mTimingFun;			//easing
	u8  Reserve0;			//Reserve for align, can use for other feature but don't delete!!!

};

/**
* class for recording the user define canvas animation
* recoding startPont/stopPoint/startWH/stopWH
* runs until END_CODE action
*/
class AnimationActionClass
{
public:
	

	
	u16 keyFrameStartID;				//start id of keyframe in keyFrameList
	u16 keyFrameStopID;				//stop id of keyframe in keyFrameList
	u8  mAnimationDirection;			//direction, 0 is add and 1 is minus
	u8  mAnimationID;				//animation ID, only when tag and id is correct will the animation be triggered
	u8  Reserve0;					//Reserve for align, can use for other feature but don't delete!!!
	u8  Reserve1;					//Reserve for align, can use for other feature but don't delete!!!

	funcStatus triggerAnimation(ElementPtr curElementPtr, u8 elememtType); //trigger the element's current animation
};

#endif

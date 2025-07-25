////////////////////////////////////////////////////////////////////////////////
// Company:		 Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     DynamicPageClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    definition of subcanvas
// 
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"

#ifdef AHMI_CORE

extern DynamicPageClassPtr  gPagePtr;
extern u16           WorkingPageID;
extern QueueHandle_t		ActionInstructionQueue;
extern AnimationClass  gAnimationClass;
extern Easing			gEasing;

SubCanvasClass::SubCanvasClass()
{}

SubCanvasClass::~SubCanvasClass()
{}

//-----------------------------
// name£º  initSubCanvas
// init SubCanvas matrix & mATag
//  
// @param1 none
// addtional comments :
// ...
//-----------------------------
funcStatus SubCanvasClass::initSubCanvas(void)
{
	this->mATag = 0;
	this->mSubCanvasMatrix.NewMatrix_init();
	this->mSubCanvasMatrix.NewMatrixInv_init();
	this->mSubCanvasMatrix.NewMatrix_init();
	
	return this->mSubCanvasMatrix.NewMatrixInv_init();
}

//-----------------------------
// name£º  initWidgetValueInSubCanvas
// init SubCanvas widget value
//  
// @param1 none
// addtional comments :
// ...
//-----------------------------
funcStatus SubCanvasClass::initWidgetValueInSubCanvas(
	DynamicPageClassPtr pageptr
	)
{
	//init all widget value in current subcanvas.
	for (int WidgetCount = 0; WidgetCount < NumOfWidget; WidgetCount++)
	{
		WidgetClassPtr wptr = &(pageptr->pWidgetList[(this->StartAddrOfWidget + WidgetCount)]);
		wptr->initWidgetValue(pageptr);
	}
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name£º  setAtag
// set animation tag
//  
// @param1 u16 value  this value is between 0 and totalframe
// addtional comments :
// ...
//-----------------------------
u32 SubCanvasClass::setAtag(u16 value, SubCanvasClassPtr lastScptr)
{
	CanvasClassPtr focusedCanvas;
	s16 curSubCanvasOffsetX; //1.11.4 , relative to canvas                                                    
	s16 curSubCanvasOffsetY; //1.11.4
//	u16 scalerX;     //1.6.9
//	u16 scalerY;     //1.6.9
	u16 transitionParamIn; //0.10
	u16 transitionParamOut; //0.10
	s16 scale; // 1.6.9
	u16 lastSubcanvasIdx;
	u16 minSubcanvasIdx;
	u16 maxSubcanvasIdx;
	s32 i;
	s32 totalOffset;
	SubCanvasClassPtr tempScptr;
//	RefreshMsg refreshMsg;

#ifdef AHMI_DEBUG
	char text[100];
	sprintf(text,"set current subcanvas animation tag %d", value);
	ERROR_PRINT(text);
#endif
	
	focusedCanvas = &gPagePtr[WorkingPageID].pCanvasList[attachCanvas];
	if(focusedCanvas == NULL) //no animation
	{
		return (EM_enErrRetModularSubcanvas | EM_ErrRetSubcanvas_NULLPointerErr);
	}
	if((value > focusedCanvas->subcanvasAnimationDuration) || (focusedCanvas->subcanvasAnimationDuration == 0)) //out of baundary
	{
		ERROR_PRINT("ERROR when setting subcanvas animation tag, value out of baundary");
		return (EM_enErrRetModularSubcanvas | EM_ErrRetSubcanvas_DurationOverrun);
	}
	this->mATag = value;
	transitionParamIn = (mATag) * 1024 / focusedCanvas->subcanvasAnimationDuration;//0.10
	//gPagePtr[WorkingPageID].inoutTransitioin(transitionParamIn, transitionParamOut);
	gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, focusedCanvas->timingFun);
	
	switch(focusedCanvas->animationType) 
	{
	case( SUBCANVAS_ANIMATION_SHIFTING_L ): //shift from left

		curSubCanvasOffsetX = (s16)( ( - (s32)focusedCanvas->mwidth ) * 16 + (s32)( (transitionParamOut * focusedCanvas->mwidth ) >> 6 )) ;
		curSubCanvasOffsetY = 0;

		this->mOffsetX = curSubCanvasOffsetX / 16;
		this->mOffsetY = curSubCanvasOffsetY / 16;

		CalculateMatrix(&(this->mSubCanvasMatrix));

		break;
	case( SUBCANVAS_ANIMATION_SHIFTING_R ): //shift from right

		curSubCanvasOffsetX = (s16) (((s32)focusedCanvas->mwidth ) * 16 - (s32)( (transitionParamOut * focusedCanvas->mwidth ) >> 6 )) ;
		curSubCanvasOffsetY = 0;

		this->mOffsetX = curSubCanvasOffsetX / 16;
		this->mOffsetY = curSubCanvasOffsetY / 16;

		CalculateMatrix(&(this->mSubCanvasMatrix));

		break;
	case( SUBCANVAS_ANIMATION_SHIFTING_UP ): //shift from up

		curSubCanvasOffsetX = 0 ;
		curSubCanvasOffsetY = (s16) (( -(s32)focusedCanvas->mheight ) * 16 + (s32)( (transitionParamOut * focusedCanvas->mheight ) >> 6 ));

		this->mOffsetX = curSubCanvasOffsetX / 16;
		this->mOffsetY = curSubCanvasOffsetY / 16;

		CalculateMatrix(&(this->mSubCanvasMatrix));

		break;
	case( SUBCANVAS_ANIMATION_SHIFTING_DOWN ): //shift from down

		curSubCanvasOffsetX = 0 ;
		curSubCanvasOffsetY = (s16) (( (s32)focusedCanvas->mheight ) * 16 - (s32)( (transitionParamOut * focusedCanvas->mheight ) >> 6 ));

		this->mOffsetX = curSubCanvasOffsetX / 16;
		this->mOffsetY = curSubCanvasOffsetY / 16;

		CalculateMatrix(&(this->mSubCanvasMatrix));

		break;
	case( SUBCANVAS_ANIMATION_ZOOM ): //ZOOM from center

		if(transitionParamOut < 16) //in case that A exceeds the boundary
			transitionParamOut = 16;
		//offset = width / 2 -> 0
		//curSubCanvasOffsetX = (s32)((focusedCanvas->mwidth << 4) >> 1); /*- (s32)( (transitionParamOut * (focusedCanvas->mwidth) ) / (1 << 6)*/ /* 0.10 * int / 0.10 * 8 */
		//curSubCanvasOffsetY = (s32)((focusedCanvas->mheight << 4) >> 1); /*- (s32)( (transitionParamOut * (focusedCanvas->mheight) ) / (1 << 6)*/ /* 0.10 * int / 0.10 * 8 */
		//this->mSubCanvasMatrix.A = (s32)((0x80000 / transitionParamOut) << 11); //1.11.20
		//scale = (s16)((0x80000 / transitionParamOut) ); //1.6.9
		scale = (transitionParamOut >> 1);//0.10 ¡ú 1.6.9
		if(scale > (1 << 9) ) //the max of 1.6.9
			scale = (s16)(1 << 9); //the maxium number of the 1.6.9
		if(scale < 4)
			scale = 4;     //the munium number

		this->mZoomX = scale;
		this->mZoomY = scale;

		CalculateMatrix(&(this->mSubCanvasMatrix));

		break;
	case( SUBCANVAS_ANIMATION_SWIPE_H ):
		if (focusedCanvas->pressFlag)
		{
			//pressing, abort animation
			return AHMI_FUNC_SUCCESS;
		}

		if (focusedCanvas->ReleasingAnimationFlag)
		{
			int releasingStep = 2;
			if (focusedCanvas->ReleasingSpeed >= 0)
			{
				if((focusedCanvas->ReleasingSpeed - releasingStep <= 0) || (this->mATag >= focusedCanvas->subcanvasAnimationDuration))
				{
					//end of release
					focusedCanvas->ReleasingSpeed = 0;
					focusedCanvas->ReleasingAnimationFlag = 0;
				}
				else
				{
					//release animation
					focusedCanvas->ReleasingSpeed -= releasingStep;
				}
			}
			else
			{
				if((focusedCanvas->ReleasingSpeed + releasingStep >= 0) || (this->mATag >= focusedCanvas->subcanvasAnimationDuration))
				{
					//end of release
					focusedCanvas->ReleasingSpeed = 0;
					focusedCanvas->ReleasingAnimationFlag = 0;
				}
				else
				{
					//release animation
					focusedCanvas->ReleasingSpeed += releasingStep;
				}
			}
						
			if (focusedCanvas->ReleasingAnimationFlag == 0)
			{
				focusedCanvas->LoadNextSubCanvasAfterReleasingAnimation();
			}
			else
			{
				focusedCanvas->updateSubCanvasListOffset(focusedCanvas->ReleasingSpeed, 0);
			}
		}
		else
		{
//			lastSubcanvasIdx = 0;
//			for (i = 0; i < focusedCanvas->mNumOfSubCanvas; i++)
//			{
//				tempScptr = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + focusedCanvas->mStartAddrOfSubCanvas + i;
//				if (tempScptr == lastScptr)
//				{
//					lastSubcanvasIdx = i;
//					break;
//				}
//			}
//			//totalOffset = 0;
//
//			if (lastSubcanvasIdx > focusedCanvas->mFocusedCanvas)
//			{
//				minSubcanvasIdx = focusedCanvas->mFocusedCanvas;
//				maxSubcanvasIdx = lastSubcanvasIdx;
//			}else
//			{
//				minSubcanvasIdx = lastSubcanvasIdx;
//				maxSubcanvasIdx = focusedCanvas->mFocusedCanvas;
//			}
//
//			if (minSubcanvasIdx == maxSubcanvasIdx)
//			{
//				if (minSubcanvasIdx > 0)
//				{
//					minSubcanvasIdx = minSubcanvasIdx - 1;
//				}
//				maxSubcanvasIdx = maxSubcanvasIdx + 1;
//			}

			totalOffset = (s32)(transitionParamOut * this->animationStartOffset) >> 6;

			for (i = 0; i < focusedCanvas->mNumOfSubCanvas; i++)
			{
				tempScptr = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + focusedCanvas->mStartAddrOfSubCanvas + i;
				tempScptr->mOffsetX = ((i-focusedCanvas->mFocusedCanvas)*focusedCanvas->mwidth*16 + this->animationStartOffset * 16 - totalOffset)/16;
				if (tempScptr->mOffsetX + tempScptr->mWidth < 0 || tempScptr->mOffsetX > focusedCanvas->mwidth || tempScptr->mOffsetY + tempScptr->mHeight < 0 || tempScptr->mOffsetY > focusedCanvas->mheight)
				{
					tempScptr->shouldShow = 0;
				}
				else
				{
					//subcanvas should animate
					tempScptr->shouldShow = 1;
					tempScptr->CalculateMatrix(&(tempScptr->mSubCanvasMatrix));
				}
//				if (i >= minSubcanvasIdx && i <= maxSubcanvasIdx)
//				{
//					//subcanvas should animate
//					tempScptr->shouldShow = 1;
//					tempScptr->mOffsetX = ((i-focusedCanvas->mFocusedCanvas)*focusedCanvas->mwidth*16 + this->animationStartOffset * 16 - totalOffset)/16;
//					tempScptr->CalculateMatrix(&(tempScptr->mSubCanvasMatrix));
//				}else
//				{
//					//subcanvas out of animattion
//					tempScptr->shouldShow = 0;
//				}
			}
		}
		break;
	case( SUBCANVAS_ANIMATION_SWIPE_V ):
		if (focusedCanvas->pressFlag)
		{
			//pressing, abort animation
			return AHMI_FUNC_SUCCESS;
		}

		if (focusedCanvas->ReleasingAnimationFlag)
		{
			int releasingStep = 2;
			if (focusedCanvas->ReleasingSpeed >= 0)
			{
				if((focusedCanvas->ReleasingSpeed - releasingStep <= 0) || (this->mATag >= focusedCanvas->subcanvasAnimationDuration))
				{
					//end of release
					focusedCanvas->ReleasingSpeed = 0;
					focusedCanvas->ReleasingAnimationFlag = 0;
				}
				else
				{
					//release animation
					focusedCanvas->ReleasingSpeed -= releasingStep;
				}
			}
			else
			{
				if((focusedCanvas->ReleasingSpeed + releasingStep >= 0) || (this->mATag >= focusedCanvas->subcanvasAnimationDuration))
				{
					//end of release
					focusedCanvas->ReleasingSpeed = 0;
					focusedCanvas->ReleasingAnimationFlag = 0;
				}
				else
				{
					//release animation
					focusedCanvas->ReleasingSpeed += releasingStep;
				}
			}
						
			if (focusedCanvas->ReleasingAnimationFlag == 0)
			{
				focusedCanvas->LoadNextSubCanvasAfterReleasingAnimation();
			}
			else
			{
				focusedCanvas->updateSubCanvasListOffset(focusedCanvas->ReleasingSpeed, 1);
			}
		}
		else
		{
//			lastSubcanvasIdx = 0;
//			for (i = 0; i < focusedCanvas->mNumOfSubCanvas; i++)
//			{
//				tempScptr = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + focusedCanvas->mStartAddrOfSubCanvas + i;
//				if (tempScptr == lastScptr)
//				{
//					lastSubcanvasIdx = i;
//					break;
//				}
//			}
//			//totalOffset = 0;
//
//			if (lastSubcanvasIdx > focusedCanvas->mFocusedCanvas)
//			{
//				minSubcanvasIdx = focusedCanvas->mFocusedCanvas;
//				maxSubcanvasIdx = lastSubcanvasIdx;
//			}else
//			{
//				minSubcanvasIdx = lastSubcanvasIdx;
//				maxSubcanvasIdx = focusedCanvas->mFocusedCanvas;
//			}
//
//			if (minSubcanvasIdx == maxSubcanvasIdx)
//			{
//				if (minSubcanvasIdx > 0)
//				{
//					minSubcanvasIdx = minSubcanvasIdx - 1;
//				}
//				maxSubcanvasIdx = maxSubcanvasIdx + 1;
//			}

			totalOffset = (s32)(transitionParamOut * this->animationStartOffset) >> 6;

			for (i = 0; i < focusedCanvas->mNumOfSubCanvas; i++)
			{
				tempScptr = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + focusedCanvas->mStartAddrOfSubCanvas + i;
				tempScptr->mOffsetY = ((i-focusedCanvas->mFocusedCanvas)*focusedCanvas->mheight*16 + this->animationStartOffset * 16 - totalOffset)/16;
				if (tempScptr->mOffsetX + tempScptr->mWidth < 0 || tempScptr->mOffsetX > focusedCanvas->mwidth || tempScptr->mOffsetY + tempScptr->mHeight < 0 || tempScptr->mOffsetY > focusedCanvas->mheight)
				{
					tempScptr->shouldShow = 0;
				}
				else
				{
					//subcanvas should animate
					tempScptr->shouldShow = 1;
					tempScptr->CalculateMatrix(&(tempScptr->mSubCanvasMatrix));
				}
			}
		}
		break;
	case( SUBCANVAS_ANIMATION_PULL_DOWN ):
	case( SUBCANVAS_ANIMATION_PULL_UP ):
		//if (focusedCanvas->pressFlag)
		//{
		//	//pressing, abort animation
		//	return AHMI_FUNC_SUCCESS;
		//}

		//lastSubcanvasIdx = 0;
		if ((focusedCanvas->pullState & 2) && (focusedCanvas->pullState & 4) )
		{
			//pulling, back
			if (lastScptr == this)
			{
				//restore
				tempScptr =  (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + focusedCanvas->mStartAddrOfSubCanvas + focusedCanvas->subCanvasWillLoadIdx;
				tempScptr->shouldShow = 1;
				this->mOffsetY = ((s32)(transitionParamOut * (0 - this->animationStartOffset)) >> 10) + this->animationStartOffset;
				this->CalculateMatrix(&(this->mSubCanvasMatrix));
			}
			else
			{
				//next subcanvas
				
				lastScptr->shouldShow = 1;

				lastScptr->mOffsetY = ((s32)(transitionParamOut * ( ((focusedCanvas->pullState & 4) !=0 ? -1 : 1) * (focusedCanvas->animationType == SUBCANVAS_ANIMATION_PULL_DOWN ? 1 : -1) * focusedCanvas->mheight - this->animationStartOffset)) >> 10) + this->animationStartOffset;
				lastScptr->CalculateMatrix(&(lastScptr->mSubCanvasMatrix));
			}
		}
		else
		{
			if (lastScptr == this)
			{
				//restore
				tempScptr =  (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + focusedCanvas->mStartAddrOfSubCanvas + focusedCanvas->subCanvasWillLoadIdx;
				tempScptr->shouldShow = 1;
				tempScptr->mOffsetY = ((s32)(transitionParamOut * ( ((focusedCanvas->pullState & 4) !=0 ? 1 : -1) * (focusedCanvas->animationType == SUBCANVAS_ANIMATION_PULL_DOWN ? 1 : -1) * focusedCanvas->mheight - this->animationStartOffset)) >> 10) + this->animationStartOffset;
				tempScptr->CalculateMatrix(&(tempScptr->mSubCanvasMatrix));
			}
			else
			{
				//next subcanvas
				totalOffset = (s32)(transitionParamOut * this->animationStartOffset) >> 6;
				lastScptr->shouldShow = 1;
				this->mOffsetY = (this->animationStartOffset * 16 - totalOffset)/16;
				this->CalculateMatrix(&(this->mSubCanvasMatrix));
			}
		}
		
		
		break;
	case( SUBCANVAS_ANIMATION_PULL_RIGHT ):
	case( SUBCANVAS_ANIMATION_PULL_LEFT ):
		//if (focusedCanvas->pressFlag)
		//{
		//	//pressing, abort animation
		//	return AHMI_FUNC_SUCCESS;
		//}

		//lastSubcanvasIdx = 0;
		if ((focusedCanvas->pullState & 1) && (focusedCanvas->pullState & 4) )
		{
			//pulling, back
			if (lastScptr == this)
			{
				//restore
				tempScptr =  (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + focusedCanvas->mStartAddrOfSubCanvas + focusedCanvas->subCanvasWillLoadIdx;
				tempScptr->shouldShow = 1;
				this->mOffsetX = ((s32)(transitionParamOut * (0 - this->animationStartOffset)) >> 10) + this->animationStartOffset;
				this->CalculateMatrix(&(this->mSubCanvasMatrix));
			}
			else
			{
				//next subcanvas
				
				lastScptr->shouldShow = 1;

				lastScptr->mOffsetX = ((s32)(transitionParamOut * ( ((focusedCanvas->pullState & 4) !=0 ? -1 : 1) * (focusedCanvas->animationType == SUBCANVAS_ANIMATION_PULL_RIGHT ? 1 : -1) * focusedCanvas->mwidth - this->animationStartOffset)) >> 10) + this->animationStartOffset;
				lastScptr->CalculateMatrix(&(lastScptr->mSubCanvasMatrix));
			}
		}
		else
		{
			if (lastScptr == this)
			{
				//restore
				tempScptr = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + focusedCanvas->mStartAddrOfSubCanvas + focusedCanvas->subCanvasWillLoadIdx;
				tempScptr->shouldShow = 1;
				tempScptr->mOffsetX = ((s32)(transitionParamOut * ( ((focusedCanvas->pullState & 4) !=0 ? 1 : -1) * (focusedCanvas->animationType == SUBCANVAS_ANIMATION_PULL_RIGHT ? 1 : -1) * focusedCanvas->mwidth - this->animationStartOffset)) >> 10) + this->animationStartOffset;
				tempScptr->CalculateMatrix(&(tempScptr->mSubCanvasMatrix));
			}
			else
			{
				//next subcanvas
				totalOffset = (s32)(transitionParamOut * this->animationStartOffset) >> 6;
				lastScptr->shouldShow = 1;
				this->mOffsetX = (this->animationStartOffset * 16 - totalOffset)/16;
				this->CalculateMatrix(&(this->mSubCanvasMatrix));
			}
		
		}
		
		
		break;
	default:
#ifdef AHMI_DEBUG
		ERROR_PRINT("current canvas don't support such kind of animation");
#endif
		return (EM_enErrRetModularSubcanvas | EM_ErrRetSubcanvas_AnimationTypeErr);
	}

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name   animationFinishCallback
// process animation finish
//  
// @param1 SubCanvasClassPtr lastScptr last shown subCanvas
// addtional comments :
// ...
//-----------------------------
u32 SubCanvasClass::animationFinishCallback(SubCanvasClassPtr lastScptr)
{
	CanvasClassPtr focusedCanvas;
//	s16 curSubCanvasOffsetX; //1.11.4 , relative to canvas                                                    
//	s16 curSubCanvasOffsetY; //1.11.4
//	u16 scalerX;     //1.6.9
//	u16 scalerY;     //1.6.9
//	u16 transitionParamIn; //0.10
//	u16 transitionParamOut; //0.10
//	s16 scale; // 1.6.9
//	u16 lastSubcanvasIdx;
//	u16 minSubcanvasIdx;
//	u16 maxSubcanvasIdx;
	s32 i;
//	s32 totalOffset;
	SubCanvasClassPtr tempScptr;
//	RefreshMsg refreshMsg;

#ifdef AHMI_DEBUG
	char text[100];
	sprintf(text,"set current subcanvas animation tag %d", value);
	ERROR_PRINT(text);
#endif
	
	focusedCanvas = &gPagePtr[WorkingPageID].pCanvasList[attachCanvas];
	if(focusedCanvas == NULL) //no animation
	{
		return (EM_enErrRetModularSubcanvas | EM_ErrRetSubcanvas_NULLPointerErr);
	}
	if(focusedCanvas->subcanvasAnimationDuration == 0)
	{
		return (EM_enErrRetModularSubcanvas | EM_ErrRetSubcanvas_DurationOverrun);
	}

	switch(focusedCanvas->animationType) 
	{
		case( SUBCANVAS_ANIMATION_SWIPE_H ):
		case( SUBCANVAS_ANIMATION_SWIPE_V ):
			if (focusedCanvas->pressFlag)
			{
				return AHMI_FUNC_SUCCESS;
			}
			for (i = 0; i < focusedCanvas->mNumOfSubCanvas; i++)
			{
				if(i == focusedCanvas->mFocusedCanvas)
					continue;
				
				tempScptr = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + focusedCanvas->mStartAddrOfSubCanvas + i;
				tempScptr->shouldShow = 0;
			}
		break;
		case( SUBCANVAS_ANIMATION_PULL_DOWN ):
		case( SUBCANVAS_ANIMATION_PULL_UP ):
		case( SUBCANVAS_ANIMATION_PULL_LEFT ):
		case( SUBCANVAS_ANIMATION_PULL_RIGHT ):
			for (i = 0; i < focusedCanvas->mNumOfSubCanvas; i++)
			{
				tempScptr = (SubCanvasClassPtr)( (&gPagePtr[WorkingPageID])->pSubCanvasList) + focusedCanvas->mStartAddrOfSubCanvas + i;
				tempScptr->shouldShow = 0;
			}
			focusedCanvas->pullState = 0;
		break;
		default:
			return (EM_enErrRetModularSubcanvas | EM_ErrRetSubcanvas_AnimationTypeErr);
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name:CalculateMatrix
// Calculate the matrix parameters from coordinate parameters
//  
//     @param1 pCanvasMatrix Canvas Matrix pointer
// addtional comments :
//-----------------------------
funcStatus SubCanvasClass::CalculateMatrix(NewMatrix* pSubCanvasMatrix)
{
	s32 tempRotateCenterX = 0;
	s32 tempRotateCenterY = 0;
	s32 tempOffsetX = 0;
	s32 tempOffsetY = 0;

	pSubCanvasMatrix->NewMatrix_init();
	pSubCanvasMatrix->NewMatrixInv_init();

	pSubCanvasMatrix->translate(-((this->mWidth / 2) << 13), -((this->mHeight / 2) << 13));
	pSubCanvasMatrix->scale(this->mZoomX,this->mZoomY);
	pSubCanvasMatrix->translate(((this->mWidth / 2) << 13), ((this->mHeight / 2) << 13));

	if(this->mZoomX < 0)
	{
		tempOffsetX = this->mZoomX;
		tempOffsetX *= this->mWidth;
		tempOffsetX /= 512; 
		tempOffsetX = -1 - tempOffsetX;
	}
	if((s16)(this->mZoomY) < 0)
	{
		tempOffsetY = this->mZoomY;
		tempOffsetY *= this->mHeight;
		tempOffsetY /= 512; 
		tempOffsetY = -1 - tempOffsetY;
	}

	tempOffsetX += (this->mOffsetX + this->contentOffsetX);
	tempOffsetY += (this->mOffsetY + this->contentOffsetY);


	if(this->mRotateAngle != 0)
	{
		tempRotateCenterX = this->mRotateCenterX;
		tempRotateCenterX *= this->mZoomX;
		tempRotateCenterX /=  (1 << 9);

		tempRotateCenterY = this->mRotateCenterY;
		tempRotateCenterY *= this->mZoomY;
		tempRotateCenterY /= (1 << 9);

		pSubCanvasMatrix->translate( -(tempRotateCenterX << 13), -(tempRotateCenterY << 13));
		pSubCanvasMatrix->rotate(this->mRotateAngle);
		pSubCanvasMatrix->translate( (tempRotateCenterX << 13), (tempRotateCenterY << 13));
	}

	if((this->mProjectionX != 0) || (this->mProjectionY != 0))
	{
		//pSubCanvasMatrix->project(this->mProjectionX, this->mProjectionY, this->mWidth, this->mHeight);
		pSubCanvasMatrix->project(this->mProjectionX, this->mProjectionY, this->mWidth, this->mHeight);
	}

	pSubCanvasMatrix->translate(tempOffsetX << 13, tempOffsetY << 13);

	this->SetSubCanvasMatrixRefreshFlag();

	return AHMI_FUNC_SUCCESS;
}

void SubCanvasClass::SetSubCanvasMatrixRefreshFlag(void)
{
//	taskENTER_CRITICAL();
	for (int WidgetCount = 0; WidgetCount < this->NumOfWidget; WidgetCount++)
	{
		WidgetClassPtr wptr = &(gPagePtr[WorkingPageID].pWidgetList[ (this->StartAddrOfWidget + WidgetCount)]);

		//Value refresh is not performed during animation or transformation for widget CHART
		if((wptr->WidgetAttr  & 0x1f) != CHART)
		{
			wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
		}
		wptr->SetWidgetMatrixRefreshFlag();
	}
//	taskEXIT_CRITICAL();
}

#endif

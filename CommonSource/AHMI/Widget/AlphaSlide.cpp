////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     CanvasClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 3.00 - File Created 2016/04/13 by YuChunYing
// Additional Comments:
//   
// 
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "AlphaSlide.h"
#include "drawImmediately_cd.h"

#ifdef AHMI_CORE
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
//extern u16 WorkingPageID;

extern QueueHandle_t		ActionInstructionQueue;

extern Easing gEasing;

//-----------------------------
// name£º DynamicTexClass
// initialize
//  
//
// addtional comments :
//   none
//-----------------------------
AlphaSlideClass::AlphaSlideClass()
{

}

//-----------------------------
// name£º ~DynamicTexClass
// deconstruct
//  
//
// addtional comments :
//   none
//-----------------------------
AlphaSlideClass::~AlphaSlideClass()
{

}

funcStatus AlphaSlideClass::initWidget(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,   
	u32 *u32p_sourceShift,  
	NewMatrixPtr pWidgetMatrix,
	u8 u8_pageRefresh,       
	u8 RefreshType ,         
	TileBoxClassPtr pTileBox,
	u8 staticTextureEn       
	)
{
	TagClassPtr bindTag;
	ActionTriggerClass tagtrigger;
	WidgetClassInterface myWidgetClassInterface;
	#if (defined VERSION_2) && (defined EMBEDDED)  
	u32 Op_sourceShift = 0;
	funcStatus status;
	#endif

	if((NULL == p_wptr) || (NULL == u32p_sourceShift) || (NULL == pTileBox))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	
	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag))
	{
        bindTag = &TagPtr->getTag(p_wptr->BindTagID);
		// find the bind tag
		tagtrigger.mTagPtr = bindTag;
		tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
		//set the widget value
		if(widgetCtrl(pagePtr, p_wptr,&tagtrigger,u8_pageRefresh) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
        
        p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
	}

	#if (defined PC_SIM) || (defined CHIP_GC9002_NoHyper)
	if(myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn, pagePtr) != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);
		
	return AHMI_FUNC_SUCCESS;
	#endif
		
	#if (defined VERSION_2) && (defined EMBEDDED)
	Op_sourceShift = *u32p_sourceShift;
	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetSourceBufferRefreshFlag) || (RefreshType == ANIMATION_MATRIXREFRESH_PAGE) || (gScreenOptions.enable))
	{
		status = myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn,pagePtr);
		if(status == AHMI_FUNC_MATRIX_ILLEGAL)
		{
			*u32p_sourceShift = Op_sourceShift;
			if(myWidgetClassInterface.drawTexture_withCopySourceBuffer(p_wptr, u32p_sourceShift) != AHMI_FUNC_SUCCESS)
				return (EM_enErrRetModularWidget | EM_ErrRetWidget_CopySourcebufferErr);
		}
		else if(status != AHMI_FUNC_SUCCESS)
		{
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);
		}

		p_wptr->REFRESH_FLAG &= (~WidgetSourceBufferRefreshFlag);
	}
	else
	{
		if(myWidgetClassInterface.drawTexture_withCopySourceBuffer(p_wptr, u32p_sourceShift) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_CopySourcebufferErr);
	}
	#endif
	
	return AHMI_FUNC_SUCCESS;
}

//*****************************
//dynamic texture 
//only one layer of texture to be displayed, but can be multi-slice
//   
//   ...
//   slice 2
//   slice 1 <- focused slice (to be displayed)
//   slice 0
// the tag value means the focused slice
//WidgetAttr£º
//15-8 : reserved
//5-7  : the mode£º0£ºswitch between different texture 1£ºrotate
//4-0  : the type of the widget
//*****************************
funcStatus AlphaSlideClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{
	s32 value,i;
	TextureClassPtr texturePtr;
	s32	maxValue;
	s32	minValue;
	u16 textWidth;
	u16 textHeight;
	u16 widgetWidth = p_wptr->WidgetWidth;
	u16 widgetHeight = p_wptr->WidgetHeight;
	u16 scrollDuration = p_wptr->ScrollDuration;
	u16 scrollDelay = p_wptr->ScrollDelay;
	u8 scrollStatus = (p_wptr->ScrollOptions >> 4) & 3; //0:idle 1:delay 2:scroll
	u8 scrollDirection = p_wptr->ScrollOptions & 3;

	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	if( (s16)(p_wptr->WidgetOffsetX) > MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetY) > MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetX) < -MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetY) < -MAX_WIDTH_AND_HEIGHT ||
		p_wptr->WidgetWidth > MAX_WIDTH_AND_HEIGHT ||
		p_wptr->WidgetHeight > MAX_WIDTH_AND_HEIGHT || 
		p_wptr->WidgetWidth == 0 ||  
		p_wptr->WidgetHeight == 0)
	{
		ERROR_PRINT("ERROR: when drawing alphaslide widget, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	value = (s32)(ActionPtr->mTagPtr->getValue());

	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);
	
	if((value > maxValue) || (value < minValue)) 
	{
		for(i = 0; i < p_wptr->NumOfTex; i++)
		{
			texturePtr = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex+i];
			texturePtr->mTexAttr |= FORCE_UNDISPLAY;
		}
		ERROR_PRINT("ERROR: widget bindTag value exceeds the boundary");
		return AHMI_FUNC_SUCCESS;
	}
	else
	{
		for(i = 0; i < p_wptr->NumOfTex; i++)
		{
			texturePtr = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex+i];
			texturePtr->mTexAttr &= (~FORCE_UNDISPLAY);
		}
	}

	//tag to find the focus slice
	texturePtr = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex+1];
	texturePtr->FocusedSlice = value;

	//text scroll
	if (scrollDuration)
	{
		if(u8_pageRefresh){
			p_wptr->ANIMATIONCURVALUE_L = 0;
			p_wptr->ANIMATIONCURVALUE_H = 0 >> 16;
			if (scrollStatus == 0)//idle
			{
				p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
				p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;
				
				textWidth = texturePtr->TexWidth;
				textHeight = texturePtr->TexHeight;
				widgetWidth = p_wptr->WidgetWidth;
				widgetHeight = p_wptr->WidgetHeight;
				switch(scrollDirection)
				{
					case 0://"<-"
						texturePtr->OffsetX = 0;
						texturePtr->OffsetY = (widgetHeight - textHeight)/2 << 4;
					break;
					
					case 1://"->"
						texturePtr->OffsetX = (widgetWidth - textWidth) << 4;
						texturePtr->OffsetY = (widgetHeight - textHeight)/2 << 4;
					break;
					
					case 2://from down to up
						texturePtr->OffsetX = (widgetWidth - textWidth)/2 << 4;
						texturePtr->OffsetY = 0;
					break;
					
					case 3://from up to down
						texturePtr->OffsetX = (widgetWidth - textWidth)/2 << 4;
						texturePtr->OffsetY = (widgetHeight - textHeight) << 4;
					break;
					
					default:
						
					break;
				}
				
				if (scrollDelay > 0)
				{
					//run delay
					p_wptr->STOP_TAG_L = scrollDelay;
					p_wptr->STOP_TAG_H =  scrollDelay >> 16;
					p_wptr->ANIMATION_DURATION = scrollDelay;
					p_wptr->ScrollOptions = ((p_wptr->ScrollOptions & (~(3<<4))) | (1 << 4));
				}
				else
				{
					//run scroll
					p_wptr->STOP_TAG_L = scrollDuration;
					p_wptr->STOP_TAG_H =  scrollDuration >> 16;
					p_wptr->ANIMATION_DURATION = scrollDuration;
					p_wptr->ScrollOptions = ((p_wptr->ScrollOptions & (~(3<<4))) | (2 << 4));
				}
				p_wptr->loadTextureWithAnimation(pagePtr);
			}
			else if (scrollStatus == 2 || scrollStatus == 1)//scroll or delay
			{
				p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
				p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;

				p_wptr->STOP_TAG_L = scrollDuration;
				p_wptr->STOP_TAG_H =  scrollDuration >> 16;
				p_wptr->ANIMATION_DURATION = scrollDuration;
				p_wptr->ScrollOptions = ((p_wptr->ScrollOptions & (~(3<<4))) | (2 << 4));
				p_wptr->loadTextureWithAnimation(pagePtr);
			}
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Tag.setValue
//widgetCtrl
//15-11: reserved
//11:  whether the button is color button
//10-7: only used in complex button, the number of button. 1-15
//6-5: The mode of button:0 is single button,1 is switch button,2 is complex button
//4-0: type of widget, button is BUTTON_TEX
//**************************************
funcStatus AlphaSlideClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    u8 u8_widgetRefresh      
    )
{
	s32 value, i;
	TextureClassPtr texturePtr;
	s32	maxValue;
	s32	minValue;
	s32 oldValue;
	u16 oldValueinit;

	value = (s32)(ActionPtr->mTagPtr->getValue());
	oldValueinit = (p_wptr->WidgetAttr & 0x8000) ? 1 : 0;

	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);
	
	if(oldValueinit)
	{
		oldValue = (s32)((p_wptr->OldValueH  << 16) + p_wptr->OldValueL);
	}
    else // no old value
    {
		oldValue = 0;
    }

	if((value > maxValue) || (value < minValue)) 
	{
		for(i = 0; i < p_wptr->NumOfTex; i++)
		{
			texturePtr = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex+i];
			texturePtr->mTexAttr |= FORCE_UNDISPLAY;
		}
		ERROR_PRINT("ERROR: AlphaSlide widget bindTag value exceeds the boundary");
		u16 CanvasID, SubcanvasID, WidgetID;
		p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
		EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
		if((oldValue > maxValue) || (oldValue < minValue))
		{
			
		}
		else
		{
			p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
		}
		p_wptr->WidgetAttr |= 0x8000;
		p_wptr->OldValueL = (u16)value;
		p_wptr->OldValueH = (u16)(value >> 16);
		return AHMI_FUNC_SUCCESS;
	}

	p_wptr->WidgetAttr |= 0x8000;
	p_wptr->OldValueL = (u16)value;
	p_wptr->OldValueH = (u16)(value >> 16);

    if(oldValueinit)
	{
		if(oldValue != value)
		{
			p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
			p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
		}
	}
	else
	{
		p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
		p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	}
    
	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Mouse.Touch
//The widget don't support touch action.
//**************************************
funcStatus AlphaSlideClass::MouseTouchCtrl(
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    PIDPoint* pPressPoint,
    PIDPoint* pReleasePoint,
    PIDPoint* pAbsolutePressPoint,
    PIDPoint* pAbsoluteReleasePoint,
    u8 u8_pageRefresh      
    )
{
    if(p_wptr->TouchPropagation & TouchPropagationSet)
	{   
        /* Touch propagation. */
        return WIDGET_TOUCH_CANCEL;
	}
    
    /* Don,t propagation. */
    return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Keyboard.Touch
//
//@param WidgetClassPtr p_wptr Pointer of widget
//@param ActionPtr             Pointer of trigger
//@param u8_pageRefresh        whether refresh the page. if refresh, the value of the widget also refresh.
//**************************************
funcStatus AlphaSlideClass::KeyboardTouchCtrl(
	DynamicPageClassPtr pagePtr,
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    u8 u8_pageRefresh      
    )
{
    p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
    
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name£º setColor
// set the purecolor color
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u8 r             //the value of the red color
//  @param	 u8 g             //the value of the red color
//  @param	 u8 b             //the value of the red color
//  @param	 u8 a             //the value of the red color
// addtional comments :
//   none
//-----------------------------
funcStatus AlphaSlideClass::setColor(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr, 
	u8 r,
	u8 g,
	u8 b,
	u8 a
)
{
	TextureClassPtr colorTexturePtr; 
	u32 color;
	colorTexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + p_wptr->NumOfTex-1]);
	color = (a<<24) + (r<<16) + (g<<8) + b;
	colorTexturePtr->TexAddr = color;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	
	return AHMI_FUNC_SUCCESS;
}

funcStatus AlphaSlideClass::setATag
	(
	WidgetClassPtr p_wptr,  
	u16 ATagValue
	)
{
	s32 ValueStart;
	s32 ValueStop;
	s32 curValue;
//	u8 onOffValue;
//	u8 onOffBit;
	TextureClassPtr texturePtr = &gPagePtr[p_wptr->ATTATCH_PAGE].pBasicTextureList[p_wptr->StartNumofTex] + 1; 
#ifndef WHOLE_TRIBLE_BUFFER
	RefreshMsg refreshMsg;
#endif
	u16 transitionParamIn;
	u16 transitionParamOut;
	u16 scrollDuration = p_wptr->ScrollDuration;
	u16 scrollDelay = p_wptr->ScrollDelay;
	u8 scrollStatus = (p_wptr->ScrollOptions >> 4) & 3;
	u16 textWidth = texturePtr->TexWidth;
	u16 textHeight = texturePtr->TexHeight;
	u16 widgetWidth = p_wptr->WidgetWidth;
	u16 widgetHeight = p_wptr->WidgetHeight;
	s16 offset = 0;
	//funcStatus status;
	
	if(scrollDuration == 0 ) //no animation
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetAnimationDurationErr);

//	taskENTER_CRITICAL();
#ifdef AHMI_DEBUG
	char text[100];
	sprintf(text,"set meter animation tag %d", ATagValue);
	ERROR_PRINT(text);
#endif
	p_wptr->NOW_DURATION |= ATagValue;
	
	//record current animation
	p_wptr->ANIMATIONCURVALUE_L = (u16)ATagValue;
	p_wptr->ANIMATIONCURVALUE_H = (u16)(ATagValue >> 16);

	//calculate offset with variables
	//u16 scrollDuration = p_wptr->ANIMATION_DURATION;
	u8 scrollDirection = p_wptr->ScrollOptions & 3;
	u8 scrollAutoReverse = (p_wptr->ScrollOptions & 4)>>2;
	
	if (scrollStatus == 2)
	{
		//scroll
		ValueStart = (s32)((p_wptr->START_TAG_L) +  ( (p_wptr->START_TAG_H) << 16));
		ValueStop = (s32)((p_wptr->STOP_TAG_L) + ( (p_wptr->STOP_TAG_H) << 16));
		transitionParamIn = (ATagValue) * 1024 / scrollDuration;//0.10
		gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut,p_wptr->TimingFun);
		
		if (scrollDirection == 0 || scrollDirection == 1)
		{
			/*ValueStop = widgetWidth + textWidth;
			curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);*/
			if(scrollAutoReverse)
			{
				if(textWidth < widgetWidth)
				{
					textWidth = widgetWidth;
				}
				ValueStop = 2*(textWidth - widgetWidth);
				curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);
				if(scrollDirection == 1)//"->"
				{
					if(curValue > textWidth-widgetWidth)
					{
						offset = (ValueStop - curValue) + widgetWidth - textWidth;
					}
					else
					{
						offset = curValue + widgetWidth - textWidth;
					}
				}
				else//"<-"
				{
					if(curValue > textWidth-widgetWidth)
					{
						offset = -(ValueStop - curValue);
					}
					else
					{
						offset = -curValue;
					}
				}
			}else{
				ValueStop = textWidth + widgetWidth;
				curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);
				if(scrollDirection == 1)//"->"
				{
					if(curValue > textWidth)
					{
						offset = -widgetWidth + (curValue - textWidth)+widgetWidth-textWidth;
					}
					else
					{
						offset =curValue+widgetWidth-textWidth;
					}
				}
				else//"<-"
				{
					if(curValue > textWidth)
					{
						offset = widgetWidth - (curValue - textWidth);
					}
					else
					{
						offset = -curValue;
					}
				}
			}
			texturePtr->OffsetX = offset << 4;
			//texturePtr->OffsetY = (widgetHeight-textHeight)/2 << 4;
			//texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
			//texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
			/*status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
			if(status == AHMI_FUNC_MATRIX_ILLEGAL)
			{
				return status;
			}*/
		}
		else if(scrollDirection == 2 || scrollDirection == 3)
		{
			if(scrollAutoReverse)
			{
				if(textHeight < widgetHeight)
				{
					textHeight = widgetHeight;
				}
				ValueStop = 2*(textHeight - widgetHeight);
				curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);
				if(scrollDirection == 3)
				{
					if(curValue > textHeight-widgetHeight)
					{
						offset = (ValueStop - curValue) + widgetHeight - textHeight;
					}
					else
					{
						offset = curValue + widgetHeight - textHeight;
					}
				}
				else
				{
					if(curValue > textHeight-widgetHeight)
					{
						offset = -(ValueStop - curValue);
					}
					else
					{
						offset = -curValue;
					}
				}
			}else{
				ValueStop = textHeight + widgetHeight;
				curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);
				if(scrollDirection == 3)
				{
					if(curValue > textHeight)
					{
						offset = -widgetHeight + (curValue - textHeight)+widgetHeight-textHeight;
					}
					else
					{
						offset =curValue+widgetHeight-textHeight;
					}
				}
				else
				{
					if(curValue > textHeight)
					{
						offset = widgetHeight - (curValue - textHeight);
					}
					else
					{
						offset = -curValue;
					}
				}
			}
			//texturePtr->OffsetX = (widgetWidth-textWidth)/2 << 4;
			texturePtr->OffsetY = offset << 4;
			//texturePtr->TexTopTileBox = texturePtr->OffsetY >> 9;
			//texturePtr->TexButtomTileBox = (texturePtr->OffsetY + texturePtr->TexHeight * 16) >> 9;
		}
	}
	
	if (scrollStatus == 1)
	{
		if(ATagValue >= scrollDelay)
		{
			//p_wptr->NOW_DURATION &= 0x0000;
			p_wptr->STOP_TAG_L = scrollDuration;
			p_wptr->STOP_TAG_H =  scrollDuration >> 16;
			p_wptr->ANIMATION_DURATION = scrollDuration;
			p_wptr->ScrollOptions = ((p_wptr->ScrollOptions & (~(3<<4))) | (2 << 4));
			p_wptr->loadTextureWithAnimation(&gPagePtr[p_wptr->ATTATCH_PAGE]);
		}
	}
	else
	{
		if(ATagValue >= scrollDuration)
		{
			//p_wptr->NOW_DURATION &= 0x0000;
			p_wptr->loadTextureWithAnimation(&gPagePtr[p_wptr->ATTATCH_PAGE]);
		}
	}
	
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
//	taskEXIT_CRITICAL();
	
	return AHMI_FUNC_SUCCESS;
}

#endif

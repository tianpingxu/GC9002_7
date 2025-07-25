////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2019/01/08
// File Name:     GalleryClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2019/01/08 by lpj 
// Additional Comments:
//    definition of button widget
// 
////////////////////////////////////////////////////////////////////////////////

#include "GalleryClass.h"
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "drawImmediately_cd.h"
#ifdef AHMI_DEBUG
#include <stdio.h>
#include "trace.h"
#endif

#define Picture_Spacing		2/3 //2/3 used to adjust the interval of the pictures

#ifdef AHMI_CORE
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
//extern u16 WorkingPageID;

extern QueueHandle_t		ActionInstructionQueue;

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;

extern Easing gEasing;

GalleryClass::GalleryClass(void)
{
}


GalleryClass::~GalleryClass(void)
{
}

funcStatus GalleryClass::initWidget(
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
	WidgetClassInterface myWidgetClassInterface;
	TagClassPtr bindTag = &TagPtr->getTag(p_wptr->BindTagID);
	ActionTriggerClass tagtrigger;
	#if (defined VERSION_2) && (defined EMBEDDED)  
	u32 Op_sourceShift = 0;
	funcStatus status;
	#endif

	if((NULL == p_wptr) || (NULL == u32p_sourceShift) || (NULL == pTileBox))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);

	if(u8_pageRefresh || (p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag))
	{
		//find the binding tag
		tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
		tagtrigger.mTagPtr = bindTag;
		//refresh the widget value
		if(widgetCtrl(pagePtr, p_wptr,&tagtrigger,u8_pageRefresh) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);

		p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
	}
	
	#if (defined PC_SIM) || (defined CHIP_GC9002_NoHyper)
	//draw all texture
	if(myWidgetClassInterface.drawGallryTexture(pagePtr, p_wptr,u32p_sourceShift, pWidgetMatrix, RefreshType,pTileBox,staticTextureEn) != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);
		
	return AHMI_FUNC_SUCCESS;
	#endif
	
	#if (defined VERSION_2) && (defined EMBEDDED)   
	Op_sourceShift = *u32p_sourceShift;
	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetSourceBufferRefreshFlag) || (RefreshType == ANIMATION_MATRIXREFRESH_PAGE) || (gScreenOptions.enable))
	{
		status = myWidgetClassInterface.drawGallryTexture(pagePtr, p_wptr,u32p_sourceShift, pWidgetMatrix, RefreshType,pTileBox,staticTextureEn);
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

//**************************************
//config and add widget
//widgetCtrl
//15-11: reserved

//10-7:  the number of picture. 1-15

//4-0: type of widget
//**************************************
funcStatus GalleryClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh      
	)
{
	TextureClassPtr TexturePtr;
	DynamicPageClassPtr pCurrentPage;
	s32 picture_num;
	s32 tag_value = 0;
//	s32 dynamic_offsetx_value = 0;
//	u8 curHighLight; //current highlight button
//	u8 flag = 0;
//	u16 cur_x;
//	u16 cur_y;
//	u8 signleColorButtonEn; //whether the button is single color
//	u8 r,g,b,a;
	static u16 texturewid = 0;
	//TextureClassPtr curTexture;
//	int i;
#ifdef AHMI_DEBUG
	//char text[100];
#endif
	//get texture
	pCurrentPage = pagePtr;
	
	if (u8_pageRefresh)
	{
		//check the parameters
		if((NULL == p_wptr) || (NULL == ActionPtr))
		{
			ERROR_PRINT("ERROR: gallery widget illegal input");
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
			ERROR_PRINT("ERROR: when drawing gallery widght, the offset or width or height exceeds the banduary");
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
		}

		if(pCurrentPage->pBasicTextureList == NULL)
		{
			ERROR_PRINT("ERROR: when drawing gallery widght, the texture list corrupt");
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
		}

		if(p_wptr->StartNumofTex < pCurrentPage->mTotalNumOfTexture ){
			TexturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex]);
			texturewid = TexturePtr->TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512;
		}
		else 
		{
			ERROR_PRINT("ERROR: when drawing gallery widght, the start texture is more then total number of texture");
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
		}

		if((TexturePtr < (void*)startOfDynamicPage) || (TexturePtr > (void*)endOfDynamicPage))
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);

	}
	picture_num = (u8)((u16)(p_wptr->NumOfButtonAndCurHighLight) >> 8);
	if ((s32)ActionPtr->mTagPtr->getValue() > picture_num-1)
	{
		ActionPtr->mTagPtr->updateTagValueToValueSpace(picture_num-1);
	}

	tag_value = ActionPtr->mTagPtr->getValue();
	u16	myvalue = tag_value * texturewid * Picture_Spacing;

	if( p_wptr->ANIMATION_DURATION == 0) //no animation
		renderTexture(pCurrentPage, p_wptr,myvalue,texturewid);
	else if(u8_pageRefresh)
	{
		//when page refresh, use the animation value as the next value, donnot need to refresh the widget by Mr.z
		//p_wptr->ANIMATIONCURVALUE_L = (u16)( (u32)tag_value );
		//p_wptr->ANIMATIONCURVALUE_H = (u16)(( (u32)tag_value ) >> 16);
		p_wptr->START_TAG_L = (u16)( (u32)myvalue );
		p_wptr->START_TAG_H = (u16)(( (u32)myvalue) >> 16);
		p_wptr->STOP_TAG_L = (u16)( (u32)myvalue);
		p_wptr->STOP_TAG_H = (u16)(( (u32)myvalue) >> 16);
		renderTexture(pCurrentPage, p_wptr, myvalue,texturewid);	 //需要产品进一步验证后决定是否删除
		//p_wptr->START_TAG_L =  (u16)( (u32)value );
		//p_wptr->START_TAG_H = (u16)(( (u32)value ) >> 16);
		//p_wptr->STOP_TAG_L =  (u16)( (u32)value );
		//p_wptr->STOP_TAG_H = (u16)(( (u32)value ) >> 16);
	}
	//else 
	//{
	//	p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
	//	p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;
	//	p_wptr->STOP_TAG_L = (u16)( (u32)myvalue );
	//	p_wptr->STOP_TAG_H = (u16)(( (u32)myvalue ) >> 16);
	//}

	//display = (p_wptr->WidgetAttr & PROBARDISPLAY)?1:0;

	//if(display)
	//{
		//if(u8_pageRefresh )
		//{
		//	if( p_wptr->ANIMATION_DURATION != 0 ) //no animation
		//	{
		//		p_wptr->loadTextureWithAnimation();

		//	}
		//}
	//}
	
	return AHMI_FUNC_SUCCESS;
}

funcStatus GalleryClass::setATag(WidgetClassPtr p_wptr,  u16 ATagValue){
	
	s32 ValueStart;
	s32 ValueStop;
	s32 curValue;
	u16 transitionParamIn; //portion of animation progress 0.10
	u16 transitionParamOut; //portion of animation progress after varying function 0.10
#ifndef WHOLE_TRIBLE_BUFFER
	RefreshMsg refreshMsg;
#endif

	u16 totalDuration = p_wptr->ANIMATION_DURATION;
	if(totalDuration == 0 ) //no animation
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetAnimationDurationErr);

//	taskENTER_CRITICAL();
#ifdef AHMI_DEBUG
	char text[100];
	sprintf(text,"set meter animation tag %d", ATagValue);
	ERROR_PRINT(text);
#endif
	p_wptr->NOW_DURATION |= ATagValue;

	//start and stop value of animation
	ValueStart = (s32)((p_wptr->START_TAG_L) +  ( (p_wptr->START_TAG_H) << 16));
	ValueStop = (s32)((p_wptr->STOP_TAG_L) + ( (p_wptr->STOP_TAG_H) << 16));

	//insert current value
	//current = start + (stop - start) * ratio
	if (ATagValue > totalDuration)
	{
		ATagValue = totalDuration;
	}

	transitionParamIn = (ATagValue) * 1024 / totalDuration;//0.10
	
	gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut,p_wptr->TimingFun);

	curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);

	//p_wptr->OldValueL = (u16)curValue;
	//p_wptr->OldValueH = (u16)(curValue >> 16); 
	
	//record current animation by Mr.z
	p_wptr->ANIMATIONCURVALUE_L = (u16)curValue;
	p_wptr->ANIMATIONCURVALUE_H = (u16)(curValue >> 16);
	s16 texturewid = gPagePtr[p_wptr->ATTATCH_PAGE].pBasicTextureList[p_wptr->StartNumofTex].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512;
	//render the texture
	renderTexture(&gPagePtr[p_wptr->ATTATCH_PAGE], p_wptr, curValue,texturewid);

	if(ATagValue >= totalDuration)
		p_wptr->NOW_DURATION &= 0x0000;

	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
//	taskEXIT_CRITICAL();
	
	return AHMI_FUNC_SUCCESS;
}

funcStatus GalleryClass::SetValueCtrl
	(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			
    ActionTriggerClassPtr ActionPtr,
    u8 u8_widgetRefresh	
	)
{ 
	TextureClassPtr TexturePtr;
	DynamicPageClassPtr pCurrentPage;
	u16 texturewid = 0;
	s32 picture_num;
	s32	value = (s32)(ActionPtr->mTagPtr->getValue());
	s32 oldValue;
	u16 oldValueinit;
	if( (s16)(p_wptr->WidgetOffsetX) > MAX_WIDTH_AND_HEIGHT || 
			(s16)(p_wptr->WidgetOffsetY) > MAX_WIDTH_AND_HEIGHT || 
			(s16)(p_wptr->WidgetOffsetX) < -MAX_WIDTH_AND_HEIGHT || 
			(s16)(p_wptr->WidgetOffsetY) < -MAX_WIDTH_AND_HEIGHT ||
			p_wptr->WidgetWidth > MAX_WIDTH_AND_HEIGHT ||
			p_wptr->WidgetHeight > MAX_WIDTH_AND_HEIGHT || 
			p_wptr->WidgetWidth == 0 ||  
			p_wptr->WidgetHeight == 0)
		{
			ERROR_PRINT("ERROR: when drawing gallery widght, the offset or width or height exceeds the banduary");
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
		}
	pCurrentPage = pagePtr;
	if(pCurrentPage->pBasicTextureList == NULL)
	{
		ERROR_PRINT("ERROR: when drawing gallery widght, the texture list corrupt");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	if(p_wptr->StartNumofTex < pCurrentPage->mTotalNumOfTexture ){
		TexturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex]);
		texturewid = TexturePtr->TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512;
	}
	else 
	{
		ERROR_PRINT("ERROR: when drawing gallery widght, the start texture is more then total number of texture");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
	}
	picture_num = (u8)((u16)(p_wptr->NumOfButtonAndCurHighLight) >> 8);
	if (value < 0)
	{
		ERROR_PRINT("ERROR: Gallery widget bindTag value exceeds the boundary");
		u16 CanvasID, SubcanvasID, WidgetID;
		p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
		EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
		value = 0;
	}
	else if (value > (picture_num - 1))
	{
		ERROR_PRINT("ERROR: Gallery widget bindTag value exceeds the boundary");
		u16 CanvasID, SubcanvasID, WidgetID;
		p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
		EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
		value = (picture_num - 1);
	}
	
	oldValueinit = (p_wptr->WidgetAttr & 0x8000) ? 1 : 0;
	
	if(oldValueinit)
	{
		oldValue = (s32)((p_wptr->OldValueH  << 16) + p_wptr->OldValueL);
	}
    else // no old value
    {

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
		else
		{
			return AHMI_FUNC_SUCCESS;
		}
	}
	else
	{
		p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
		p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	}
	
	u16 myvalue = value * texturewid * Picture_Spacing;
	if(u8_widgetRefresh == 0)
    {
        if((p_wptr->ANIMATION_DURATION) != 0) //contain animation
        {
			//calculate real start value
			p_wptr->calculateCurrentAnimation(pagePtr);
			p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
		    p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;
		    p_wptr->STOP_TAG_L = (u16)( (u32)myvalue );
		    p_wptr->STOP_TAG_H = (u16)(( (u32)myvalue ) >> 16);
			p_wptr->loadTextureWithAnimation(pagePtr);
        }
    }
	
	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Mouse.Touch
//The widget don't support touch action.
//**************************************
funcStatus GalleryClass::MouseTouchCtrl(
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

funcStatus GalleryClass::renderTexture
	(
		DynamicPageClassPtr pagePtr,
		WidgetClassPtr p_wptr,  
		s32 value ,
		s16 texwid
		)
{
	TextureClassPtr  DrawingTex;
	int maxangle = 45<<4;
	int middlex = p_wptr->WidgetWidth/2;
	int temp_offsetx = (middlex - texwid/2 - value)*16;
//	int numofpic = p_wptr->NumOfButtonAndCurHighLight >> 8;
	int temp = 0;
	int TextureCount;
	for (TextureCount = 0; TextureCount < p_wptr->NumOfTex - 1; TextureCount++)
	{
		DrawingTex = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex + TextureCount];
		temp = temp_offsetx + TextureCount * (texwid<<4) * Picture_Spacing;
		
		if((temp > (MAX_WIDTH_AND_HEIGHT << 4)) || (temp < (-MAX_WIDTH_AND_HEIGHT << 4)))
		{
			DrawingTex->mTexAttr |= (FORCE_UNDISPLAY); 
			if(temp > (MAX_WIDTH_AND_HEIGHT << 4))
			{
				DrawingTex->ShearAngleZX = maxangle;
			}
			else if(temp < (-MAX_WIDTH_AND_HEIGHT << 4))
			{
				DrawingTex->ShearAngleZX = -maxangle;
			}
			continue;
		}
		
		DrawingTex->OffsetX = temp;

		if((DrawingTex->OffsetX <= (-texwid * Picture_Spacing << 4)) || (DrawingTex->OffsetX >= s16(p_wptr->WidgetWidth << 4)))
		{
			DrawingTex->mTexAttr |= (FORCE_UNDISPLAY);
			if(DrawingTex->OffsetX <= (-texwid * Picture_Spacing << 4))
			{
				DrawingTex->ShearAngleZX = -maxangle;
			}
			else if(DrawingTex->OffsetX >= s16(p_wptr->WidgetWidth << 4))
			{
				DrawingTex->ShearAngleZX = maxangle;
			}
		}
		else
		{
			DrawingTex->mTexAttr &= ~(FORCE_UNDISPLAY); 
			#ifdef VERSION_2
			ComputeShearAngle(pagePtr, DrawingTex,middlex,texwid);
			#endif
		}
	}
	return AHMI_FUNC_SUCCESS;
}
#ifdef VERSION_2
funcStatus GalleryClass::ComputeShearAngle
	(
	DynamicPageClassPtr pagePtr,
	TextureClassPtr textureptr,   
	s32 middlex,                
	s16 texwid                
	)
{
	int maxangle = 45<<4;
	int distance = middlex - textureptr->OffsetX/16 - texwid/2;
	/*if (distance < 10 && distance > -10)
	{
		distance = 0;
	}*/
	if (distance > texwid/2)
	{
		textureptr->ShearAngleZX = -maxangle;
	}
	else if (distance > (-texwid/2))
	{
		textureptr->ShearAngleZX = -maxangle * distance * 2 / texwid;
	}
	else
	{
		textureptr->ShearAngleZX = maxangle;
	}
	return AHMI_FUNC_SUCCESS;
}
#endif

#endif

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     CombinationSwitch.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 3.00 - File Created 2020/09/22 by yn
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "CombinationSwitch.h"
#include "drawImmediately_cd.h"

#ifdef AHMI_CORE
extern ActionTriggerClass		gWidgetTagTrigger;
extern ActionTriggerClass		gMouseTagTrigger;
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
//extern u16 WorkingPageID;

extern QueueHandle_t		ActionInstructionQueue;

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;

//-----------------------------
// name�� DynamicTexClass
// ���캯��
//  
//
// addtional comments :
//   none
//-----------------------------
CombinationSwitchClass::CombinationSwitchClass()
{

}

//-----------------------------
// name�� ~DynamicTexClass
// ��������
//  
//
// addtional comments :
//   none
//-----------------------------
CombinationSwitchClass::~CombinationSwitchClass()
{

}

//-----------------------------
// name�� DynamicTexClass::initWidget
// 
//  @param   WidgetClassPtr p_wptr, 
//  @param	 u32 *u32p_sourceShift,
//  @param   u8 u8_pageRefresh,
//  @param   TileBoxClassPtr pTileBox 
// addtional comments :
//   none
//-----------------------------
funcStatus CombinationSwitchClass::initWidget(
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
		tagtrigger.mTagPtr = bindTag;
		tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
		if(widgetCtrl(pagePtr, p_wptr,&tagtrigger,u8_pageRefresh) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
        
        p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
	}

	#if (defined PC_SIM) || (defined CHIP_GC9002_NoHyper)
	if(myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift,pWidgetMatrix,RefreshType,pTileBox,staticTextureEn, pagePtr) != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);
		
	return AHMI_FUNC_SUCCESS;
	#endif
	
	#if (defined VERSION_2) && (defined EMBEDDED)
	Op_sourceShift = *u32p_sourceShift;
	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetSourceBufferRefreshFlag) || (RefreshType == ANIMATION_MATRIXREFRESH_PAGE) || (gScreenOptions.enable))
	{
		status = myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn, pagePtr);
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
//on or off texture
//only one texture that can be determined to display or not
//tag indicates whether to display
//WidgetAttr��
//5-9  : binding to which bit of number
//4-0  : widget type 13
//*****************************
funcStatus CombinationSwitchClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			    //pointer to widget
	ActionTriggerClassPtr ActionPtr,    //pointer to action trigger
	u8 u8_pageRefresh				    //refresh whole page flag
	)
{
	u32 value, i;
	TextureClassPtr texturePtr;
//	u16 oldValueinit;
//	u32 oldValue;
	u32 texture_num;
	u32 flashmode;
//	u8 align;

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
		ERROR_PRINT("ERROR: when drawing onofftexture widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	value = ActionPtr->mTagPtr->getValue();
//	oldValue = (p_wptr->OldValueH << 16) | (p_wptr->OldValueL);
//	align = p_wptr->AlignMode;
	flashmode = (p_wptr->CS_FLASHMODEH << 16) | (p_wptr->CS_FLASHMODEL);

    //changeable texture.
	texturePtr = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex]; 

#ifdef EMBEDDED
	if((texturePtr < (void*)startOfDynamicPage) || (texturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
#endif

    //switch texture
	texture_num = p_wptr->NumOfTex;
	//set display or not
	for(i = 0; i < texture_num; i++)
	{
		if((flashmode >> i) & 0x1)
		{
			//flash mode
			if(p_wptr->CS_FLASHDUR < 100 || p_wptr->CS_FLASHDUR > 100000){
				ERROR_PRINT("ERROR: switch flash duration exceeds the limit");
				return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
			}

			texturePtr[i].mTexAttr |= (FORCE_UNDISPLAY);
			if((value >> i) & 0x1)
			{
				//add timeout to on and off
				if(u8_pageRefresh){
					p_wptr->ANIMATIONCURVALUE_L = 0;
					p_wptr->ANIMATIONCURVALUE_H = 0 >> 16;	
				}else
				{
					//calculate real start value
					p_wptr->calculateCurrentAnimation(pagePtr);
				}

				p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
				p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;
				p_wptr->STOP_TAG_L = p_wptr->CS_FLASHDUR;
				p_wptr->STOP_TAG_H =  p_wptr->CS_FLASHDUR >> 16;
				//start timeout
				p_wptr->ANIMATION_DURATION = p_wptr->CS_FLASHDUR;

				p_wptr->loadTextureWithAnimation(pagePtr);
			}
			else
			{
				//keep off
				texturePtr[i].mTexAttr |= (FORCE_UNDISPLAY);
			}
		}
		else
		{
			if((value >> i) & 0x1){
				texturePtr[i].mTexAttr &= (~FORCE_UNDISPLAY);
			}else{
				//do not switch texture
				texturePtr[i].mTexAttr |= (FORCE_UNDISPLAY);
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
//4-0: type of widget
//**************************************
u8 oldCombinationSwitchSequence[32] = {0};
u8 oldCombinationSwitchNum = 0;
funcStatus CombinationSwitchClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    u8 u8_widgetRefresh      
    )
{
	TextureClassPtr texturePtr;
	u32 texture_num, i, j = 0;
    s32 value;
	s32 oldValue;
	u16 oldValueinit;
	u8 align;

	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	value = (s32)ActionPtr->mTagPtr->getValue();
	oldValueinit = (p_wptr->WidgetAttr & 0x8000) ? 1 : 0;
	align = p_wptr->AlignMode;

	//set display position
	texture_num = p_wptr->NumOfTex;

	if(oldValueinit)
	{
		oldValue = (s32)((p_wptr->OldValueH  << 16) + p_wptr->OldValueL);
	}
    else // no old value
    {
		oldValue = 0;
    }
    
	for(i = 0; i < oldCombinationSwitchNum; i++)
	{
		if((value >> oldCombinationSwitchSequence[i]) & 0x1)
		{
			oldCombinationSwitchSequence[j++] = oldCombinationSwitchSequence[i];
		}
	}
	oldCombinationSwitchNum = j;

	for(i = 0; i < texture_num; i++)
	{
		if(((value >> i) & 0x1) && (((oldValue >> i) & 0x1) == 0))
		{
			oldCombinationSwitchSequence[oldCombinationSwitchNum++] = i;
		}
	}

	//changeable texture.
	texturePtr = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex]; 
	for(i = 0; i < oldCombinationSwitchNum; i++)
	{
		if(align == CombinationSwitch_LEFTALIGN)
		{
			texturePtr[oldCombinationSwitchSequence[i]].OffsetX = ((p_wptr->WidgetOffsetX + i * texturePtr->TexWidth) << 4);
		}
		else if(align == CombinationSwitch_RIGHTALIGN)
		{
			texturePtr[oldCombinationSwitchSequence[i]].OffsetX = ((p_wptr->WidgetOffsetX + p_wptr->WidgetWidth - (i+1) * texturePtr->TexWidth) << 4);
		}
		else if(align == CombinationSwitch_UPALIGN)
		{
			texturePtr[oldCombinationSwitchSequence[i]].OffsetY = ((p_wptr->WidgetOffsetY + i * texturePtr->TexHeight) << 4);
		}
		else if(align == CombinationSwitch_DOWNALIGN)
		{
			texturePtr[oldCombinationSwitchSequence[i]].OffsetY = ((p_wptr->WidgetOffsetY + p_wptr->WidgetHeight - (i+1) * texturePtr->TexHeight) << 4);
		}
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
funcStatus CombinationSwitchClass::MouseTouchCtrl(
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
funcStatus CombinationSwitchClass::KeyboardTouchCtrl(
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
// name�� setATag
// set the texture's animation tag
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the animation tag
// addtional comments :
//   none
//-----------------------------
funcStatus CombinationSwitchClass::setATag
	(
	WidgetClassPtr p_wptr,  
	u16 ATagValue
	)
{
	s32 oldValue;
	u32 curValue, i;
	u32 texture_num;
	u32 flashmode;
	TextureClassPtr texturePtr = &gPagePtr[p_wptr->ATTATCH_PAGE].pBasicTextureList[p_wptr->StartNumofTex];
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

	curValue = ATagValue;
	
	oldValue = (s32)((p_wptr->OldValueH  << 16) + p_wptr->OldValueL);

	//record current animation
	p_wptr->ANIMATIONCURVALUE_L = (u16)curValue;
	p_wptr->ANIMATIONCURVALUE_H = (u16)(curValue >> 16);

	//render the texture
	texture_num = p_wptr->NumOfTex;
	texturePtr = &gPagePtr[p_wptr->ATTATCH_PAGE].pBasicTextureList[p_wptr->StartNumofTex]; 
	flashmode = (p_wptr->CS_FLASHMODEH << 16) | (p_wptr->CS_FLASHMODEL);

	//set display or not
	for(i = 0; i < texture_num; i++)
	{
		if(((flashmode >> i) & 0x1) && ((oldValue >> i) & 0x1))
		{
			if(curValue > totalDuration/2){//50% on 50% off
			//on
			texturePtr[i].mTexAttr &= (~FORCE_UNDISPLAY);
			}else{
			//off
			texturePtr[i].mTexAttr |= (FORCE_UNDISPLAY);
			}
		}
	}	

	if(ATagValue >= totalDuration)
		//p_wptr->NOW_DURATION &= 0x0000;
		p_wptr->loadTextureWithAnimation(&gPagePtr[p_wptr->ATTATCH_PAGE]);

	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
//	taskEXIT_CRITICAL();
	
	return AHMI_FUNC_SUCCESS;
}
#endif

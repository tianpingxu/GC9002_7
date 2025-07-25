//
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "ButtonSwitch.h"

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
extern Easing gEasing;


//-----------------------------
// name£º MeterClass
// construct
//  
//
// addtional comments :
//   none
//-----------------------------
ButtonSwitchClass::ButtonSwitchClass()
{

}

//-----------------------------
// name£º ~MeterClass
// deconstruct
//  
//
// addtional comments :
//   none
//-----------------------------
ButtonSwitchClass::~ButtonSwitchClass()
{

}

//**************************************
//initialize the widget
//
//@param WidgetClassPtr p_wptr Pointer of widget
//@param u32 *u32p_sourceShift,   //pointer of sourcebuffer
//@param u8_pageRefresh          //whether refresh the page. if refresh, the value of the widget also refresh.
//@param u8 RefreshType ,        //the type of animation
//@param TileBoxClassPtr pTileBox, //tile box @see TileBoxClass
//@param u8 staticTextureEn          //whether to use the static buffer, not used now
//**************************************
funcStatus ButtonSwitchClass::initWidget(
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
		//find the bind tag
		bindTag = &TagPtr->getTag(p_wptr->BindTagID);
		//set the value of widget according to tag
		tagtrigger.mTagPtr = bindTag;
		tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
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

//-----------------------------
// compute the texture using the tag value
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the tag
// addtional comments :
//   none
//-----------------------------
funcStatus ButtonSwitchClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			//¿Ø¼þÖ¸Õë
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				//Ò³ÃæË¢ÐÂ
	)
{	
	s32 ValueStart;
	s32 ValueStop;
	s32 value;

	//param check
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
		ERROR_PRINT("ERROR: when drawing buttonswitch widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	//params
	value = (s32)(ActionPtr->mTagPtr->getValue());

	if(value > 0)
		value = 1;
	else if(value < 0)
		value = 0;
				
	//if(ActionPtr->mInputType == ACTION_TAG_SET_VALUE)
	//{
	value = value * 1000; // scale value for animation
	//render the texture 
	if( p_wptr->ANIMATION_DURATION == 0) //no animation
	{
		renderTexture(pagePtr, p_wptr, value);	//set the value of texture from current value
	}
	else if(u8_pageRefresh)	   //animation of page refresh
	{
		//when page refresh,use the value of animation as next value, not according to current value by Mr.z
		//p_wptr->ANIMATIONCURVALUE_L = (u16)( (u32)value );
		//p_wptr->ANIMATIONCURVALUE_H = (u16)(( (u32)value ) >> 16);
		p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
		p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;
		p_wptr->STOP_TAG_L = (u16)( (u32)value );
		p_wptr->STOP_TAG_H = (u16)(( (u32)value ) >> 16);
		renderTexture(pagePtr, p_wptr, value);	   
	}
	else // first frame
	{
		p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
		p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;
		p_wptr->STOP_TAG_L = (u16)( (u32)value );
		p_wptr->STOP_TAG_H = (u16)(( (u32)value ) >> 16);
	}
	//}

	ValueStart = (s32)((p_wptr->START_TAG_L) +  ( (p_wptr->START_TAG_H) << 16));
	ValueStop = (s32)((p_wptr->STOP_TAG_L) + ( (p_wptr->STOP_TAG_H) << 16));

	if( (p_wptr->ANIMATION_DURATION != 0) && (ValueStart != ValueStop) ) //create animation
	{
		p_wptr->loadTextureWithAnimation(pagePtr);
	}

	//if((u8_pageRefresh == 0) && (p_wptr->ANIMATION_DURATION != 0))
	//{
	//	//send refresh message
	//	//calculate real start value
	//	p_wptr->calculateCurrentAnimation(pagePtr);
	//	p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
	//	p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;

	//	p_wptr->loadTextureWithAnimation(pagePtr);
	//}

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
funcStatus ButtonSwitchClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			  
	ActionTriggerClassPtr ActionPtr,  
	u8 u8_widgetRefresh				  
	)
{
	s32 value;
	s32 oldValue;
	u16 oldValueinit;
	
	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	
	value = ActionPtr->mTagPtr->getValue();
	if(value > 0)
		value = 1;
	else if(value < 0)
		value = 0;
	
	oldValueinit = (p_wptr->WidgetAttr & 0x8000) ? 1 : 0;

	if(oldValueinit)
	{
		oldValue = (s32)((p_wptr->OldValueH  << 16) + p_wptr->OldValueL);
		if(oldValue > 0)
			oldValue = 1;
		else if(oldValue < 0)
			oldValue = 0;
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
//widgetCtrl
//15-11: reserved
//11:  whether the button is color button
//10-7: only used in complex button, the number of button. 1-15
//6-5: The mode of button:0 is single button,1 is switch button,2 is complex button
//4-0: type of widget, button is BUTTON_TEX
//**************************************
funcStatus ButtonSwitchClass::MouseTouchCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	ActionTriggerClassPtr ActionPtr,
    PIDPoint* pPressPoint,
    PIDPoint* pReleasePoint,
	PIDPoint* pAbsolutePressPoint,
    PIDPoint* pAbsoluteReleasePoint,
	u8 u8_widgetRefresh      
	)
{
	s32 tag_value= 0;
	s32 value;
	//s32 cur_x;
	//s32 cur_y;
	DynamicPageClassPtr pCurrentPage;
	u32 actionAddr;
//	s32 diffX = 0;
//	s32 diffY = 0;
//	u8 actionType;

	//param check
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
		ERROR_PRINT("ERROR: when drawing buttonswitch widget, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	//get texture
	pCurrentPage = pagePtr;
	if(pCurrentPage->pBasicTextureList == NULL)
	{
		ERROR_PRINT("ERROR: when drawing buttonswitch widght, the texture list corrupt");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

//	actionType = ActionPtr->mInputType;
	
	//params
	value = (s32)(ActionPtr->mTagPtr->getValue());

	if(value > 0)
		value = 1;
	else if(value < 0)
		value = 0;
				
	if( ActionPtr->mInputType == ACTION_MOUSE_PRESS )
	{
		tag_value = value == 1?0:1;

		TagPtr->getTag(p_wptr->BindTagID).setRawValue(tag_value);
	}
	else if( ActionPtr->mInputType == ACTION_MOUSE_RELEASE )
	{
		//send release action
		if(p_wptr->OnRealeaseAction <= pCurrentPage->mActionInstructionsSize)
		{
			actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnRealeaseAction);
			csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
		}
	}

	p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	
	if(p_wptr->TouchPropagation & TouchPropagationSet)
	{
		return WIDGET_TOUCH_CANCEL;
	}
	else
	{
		return AHMI_FUNC_SUCCESS;
	}
}

funcStatus ButtonSwitchClass::renderTexture
	(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	s32 value
	)
{
	//start address of texture
	TextureClassPtr texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);

#ifdef EMBEDDED
	if((texturePtr < (void*)startOfDynamicPage) || (texturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
#endif

	texturePtr[1].OffsetX = texturePtr[0].OffsetX + value * (texturePtr[0].TexWidth << 4) /2 /1000; // down sacle 1000

	return AHMI_FUNC_SUCCESS;
}

funcStatus ButtonSwitchClass::setATag(
	WidgetClassPtr p_wptr,    //widget pointer
	u16 value                 //value of animation tag
	)
{
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
	sprintf(text,"set meter animation tag %d", value);
	ERROR_PRINT(text);
#endif
	p_wptr->NOW_DURATION |= value;
	ValueStart = (s32)((p_wptr->START_TAG_L) +  ( (p_wptr->START_TAG_H) << 16));
	ValueStop = (s32)((p_wptr->STOP_TAG_L) + ( (p_wptr->STOP_TAG_H) << 16));

	//char text[100];
	//sprintf(text,"loop: start: %d - stop: %d", ValueStart, ValueStop);
	//ERROR_PRINT(text);

	transitionParamIn = (value) * 1024 / totalDuration;//0.10
	
	gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut,p_wptr->TimingFun);

	curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);

	//Angle = 16 * (value) * ( (s16)(p_wptr->MeterMaxAngle) - (s16)(p_wptr->MeterMinAngle) ) * (ValueStop - ValueStart) /(totalDuration * (maxValue - minValue)) + 16 * (ValueStart) * ( (s16)(p_wptr->MeterMaxAngle) - (s16)(p_wptr->MeterMinAngle) ) / (maxValue - minValue);//12.4

	p_wptr->OldValueL = (u16)curValue;
	p_wptr->OldValueH = (u16)(curValue >> 16); 
	
	//record current animation by Mr.z
	p_wptr->ANIMATIONCURVALUE_L = (u16)curValue;
	p_wptr->ANIMATIONCURVALUE_H = (u16)(curValue >> 16);

	/*sprintf(text,"loop: animation start: %d - stop: %d ..", p_wptr->ANIMATIONCURVALUE_L, p_wptr->ANIMATIONCURVALUE_H);
	ERROR_PRINT(text);*/

	renderTexture(&gPagePtr[p_wptr->ATTATCH_PAGE], p_wptr,curValue);

	if(value >= totalDuration)
		p_wptr->NOW_DURATION &= 0x0000; // set now frame to 0

	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
//	taskEXIT_CRITICAL();
	
	return AHMI_FUNC_SUCCESS;
}

#endif

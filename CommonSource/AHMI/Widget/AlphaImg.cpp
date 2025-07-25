////////////////////////////////////////////////////////////////////////////////
//alpha img
//created by Zzen1sS
//2019/1/8
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "AlphaImg.h"
#include "drawImmediately_cd.h"

#ifdef AHMI_CORE
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
//extern u16 WorkingPageID;

extern QueueHandle_t		ActionInstructionQueue;

//-----------------------------
// name£º DynamicTexClass
// initialize
//  
//
// addtional comments :
//   none
//-----------------------------
AlphaImgClass::AlphaImgClass()
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
AlphaImgClass::~AlphaImgClass()
{

}

funcStatus AlphaImgClass::initWidget(
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
		// find the bind tag
		bindTag = &TagPtr->getTag(p_wptr->BindTagID);
		tagtrigger.mTagPtr = bindTag;
		tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
		//set the widget value
		if(widgetCtrl(pagePtr, p_wptr,&tagtrigger,1) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);

		p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
	}

	#if (defined PC_SIM) || (defined CHIP_GC9002_NoHyper)
	if(myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn,pagePtr) != AHMI_FUNC_SUCCESS)
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
funcStatus AlphaImgClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{
	s32 value;
    s32	maxValue;
	s32	minValue;
    u32 textAddr;
    TextureClassPtr texturePtr;

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
		ERROR_PRINT("ERROR: when drawing alphaimg widget, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}
	
	value = (s32)(ActionPtr->mTagPtr->getValue());

//	curValue = (p_wptr->CurValueH << 16) + p_wptr->CurValueL;
	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);

	value = 255 * value / (maxValue-minValue);
	if (value < 0)
	{
		value = 0;
	}else if (value > 255)
	{
		value = 255;
	}

	//alpha
	texturePtr = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex];
	textAddr =  texturePtr->TexAddr;
	texturePtr->TexAddr = (textAddr & ((1<<24)-1)) + (((u32)value) << 24);
	//tag to find the focus slice
	texturePtr = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex+1];
	texturePtr->FocusedSlice = 0;

	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Tag.setValue
//widgetCtrl
//15-11: reserved
//11:  whether the button is color button
//10-7: only used in complex button, the number of button. 1-15
//6-5: The mode of button:0 is single button,1 is switch button,2 is complex button
//4-0: type of widget, button is ALPHA_IMG
//**************************************
funcStatus AlphaImgClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    u8 u8_widgetRefresh      
    )
{
	s32 value;
    s32	maxValue;
	s32	minValue;
	s32 oldValue;
	u16 oldValueinit;

	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

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

	if((value < minValue) || (value > maxValue)){
		ERROR_PRINT("ERROR: AlphaImg widget bindTag value exceeds the boundary");
		u16 CanvasID, SubcanvasID, WidgetID;
		p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
		EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_SetValueErr);
	}

	if(maxValue == minValue){
		ERROR_PRINT("ERROR: divided by zero");
		u16 CanvasID, SubcanvasID, WidgetID;
		p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
		EM_vLog(EM_enErrDividedByZero,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, 0, 0);
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_SetValueErr);
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
funcStatus AlphaImgClass::MouseTouchCtrl(
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

#endif

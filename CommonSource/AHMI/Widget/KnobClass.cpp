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
#include "KnobClass.h"
#include "myMathClass.h"

#ifdef AHMI_CORE
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;

extern QueueHandle_t		ActionInstructionQueue;

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;

//-----------------------------
// name： KnobClass
// 构造函数
//  
//
// addtional comments :
//   none
//-----------------------------
KnobClass::KnobClass()
{

}

//-----------------------------
// name： ~KnobClass
// 析构函数
//  
//
// addtional comments :
//   none
//-----------------------------
KnobClass::~KnobClass()
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
funcStatus KnobClass::initWidget(
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
	if(myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn, pagePtr) != AHMI_FUNC_SUCCESS)
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

//**************************************
//reponse for 
//widgetCtrl
//15-11: reserved
//11:  whether the button is color button
//10-7: only used in complex button, the number of button. 1-15
//6-5: The mode of button:0 is single button,1 is switch button,2 is complex button
//4-0: type of widget, button is KNOB_TEX
//**************************************
funcStatus KnobClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh			
	)
{
	u16  angleTemp;
	u16* angle = &angleTemp;
	TextureClassPtr texturePtr;
	u32	maxValue;
	u32	minValue;

	if((NULL == p_wptr) || (NULL == ActionPtr))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);

	maxValue = (p_wptr->MaxValueH << 16) + p_wptr->MaxValueL;
	minValue = (p_wptr->MinValueH << 16) + p_wptr->MinValueL;

	texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);

#ifdef EMBEDDED
	if((texturePtr < (void*)startOfDynamicPage) || (texturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
#endif

    if(ActionPtr->mInputType == ACTION_TAG_SET_VALUE)
	{
		angle =(u16*) (16 * (ActionPtr->mTagPtr->getValue() - minValue)* 360 /(maxValue - minValue));
		texturePtr[1].RotateAngle  = *(s16 *)(angle);
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
funcStatus KnobClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    u8 u8_widgetRefresh      
    )
{
    p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
    
    return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Mouse.Touch
//The widget don't support touch action.
//@param WidgetClassPtr p_wptr Pointer of widget
//@param ActionPtr             Pointer of trigger
//@param u8_pageRefresh        whether refresh the page. if refresh, the value of the widget also refresh.
//**************************************
funcStatus KnobClass::MouseTouchCtrl(
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

//**************************************
//reponse for Keyboard.Touch
//
//@param WidgetClassPtr p_wptr Pointer of widget
//@param ActionPtr             Pointer of trigger
//@param u8_pageRefresh        whether refresh the page. if refresh, the value of the widget also refresh.
//**************************************
funcStatus KnobClass::KeyboardTouchCtrl(
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

#endif 


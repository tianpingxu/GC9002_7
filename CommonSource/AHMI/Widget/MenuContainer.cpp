////////////////////////////////////////////////////////////////////////////////
//keyboard container
//created by Zzen1sS
//2020/3/10
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "MenuContainer.h"
#include "drawImmediately_cd.h"
#include "stdlib.h"

#ifdef AHMI_CORE
extern ConfigInfoClass		ConfigData;
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
extern MenuState*			gMenuState;

//-----------------------------
// name£º DynamicTexClass
// initialize
//  
//
// addtional comments :
//   none
//-----------------------------
MenuContainerClass::MenuContainerClass()
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
MenuContainerClass::~MenuContainerClass()
{

}

funcStatus MenuContainerClass::initWidget(
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
	int i;
	#if (defined VERSION_2) && (defined EMBEDDED)  
	u32 Op_sourceShift = 0;
	funcStatus status;
	#endif
	
	if((NULL == p_wptr) || (NULL == u32p_sourceShift) || (NULL == pTileBox))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);

	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag))
	{
		if(u8_pageRefresh)
		{
			if(p_wptr->MENUSTATEID < ConfigData.NumOfMenuStates)
			{
				gMenuState[p_wptr->MENUSTATEID].currentValue = 0;
				gMenuState[p_wptr->MENUSTATEID].tagID = p_wptr->BindTagID;
				gMenuState[p_wptr->MENUSTATEID].mode = p_wptr->MENUMODE;
				gMenuState[p_wptr->MENUSTATEID].state = 0;
				gMenuState[p_wptr->MENUSTATEID].statebyItem = 1;
				for(i = 0; i < 32; i++)
				{
					gMenuState[p_wptr->MENUSTATEID].curMenuItemSequence[i] = 0xFFFF;
				}
			}
		}
		// find the bind tag
		bindTag = &TagPtr->getTag(p_wptr->BindTagID);
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
funcStatus MenuContainerClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{
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

	texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);

	if(u8_pageRefresh)
	{
		texturePtr[1].OffsetY = (texturePtr[0].TexHeight << 4);
		texturePtr[0].mTexAttr &= ~(FORCE_UNDISPLAY);
		texturePtr[1].mTexAttr |= (FORCE_UNDISPLAY);
	}

	if(p_wptr->MENUSTATEID < ConfigData.NumOfMenuStates)
	{
		if(gMenuState[p_wptr->MENUSTATEID].state == 0)
		{
			texturePtr[0].mTexAttr &= ~(FORCE_UNDISPLAY);
			texturePtr[1].mTexAttr |= (FORCE_UNDISPLAY);	
		}
		else if(gMenuState[p_wptr->MENUSTATEID].state == 1)
		{
			texturePtr[0].mTexAttr &= ~(FORCE_UNDISPLAY);
			texturePtr[1].mTexAttr &= ~(FORCE_UNDISPLAY);
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
//4-0: type of widget, button is ALPHA_IMG
//**************************************
funcStatus MenuContainerClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    u8 u8_pageRefresh      
    )
{
	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	
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
funcStatus MenuContainerClass::MouseTouchCtrl(
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
//	DynamicPageClassPtr pCurrentPage;
//	CanvasClassPtr curCanvas = &pagePtr->pCanvasList[p_wptr->ATTATCH_CANVAS];
//	SubCanvasClassPtr curSubCan = &(pagePtr->pSubCanvasList[curCanvas->mStartAddrOfSubCanvas + curCanvas->mFocusedCanvas]);
	TextureClassPtr texturePtr;
#ifdef AHMI_DEBUG
	//char text[100];
#endif

	//check the parameters
	if((NULL == p_wptr) || (NULL == ActionPtr))
	{
		ERROR_PRINT("ERROR: button widget illegal input");
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
		ERROR_PRINT("ERROR: when drawing swipe responder widght, the offset or width or height exceeds the banduary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

//	pCurrentPage =  pagePtr;
	texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);

	//mouse
	if( ActionPtr->mInputType == ACTION_MOUSE_PRESS ) //mouse press
	{
		if(((s16)(pPressPoint->y) <= p_wptr->WidgetHeight) && ((s16)(pPressPoint->y) >= texturePtr[0].TexHeight))
		{
			if(gMenuState[p_wptr->MENUSTATEID].state == 0)
			{
				return WIDGET_TOUCH_CANCEL;
			}
			else if(gMenuState[p_wptr->MENUSTATEID].state == 1)
			{
				if(gMenuState[p_wptr->MENUSTATEID].statebyItem == 0)
				{
					gMenuState[p_wptr->MENUSTATEID].statebyItem = 1;
					gMenuState[p_wptr->MENUSTATEID].state = 0;
				}

				if(p_wptr->TouchPropagation & TouchPropagationSet)
				{
					return WIDGET_TOUCH_CANCEL;
				}
				else
				{
					return AHMI_FUNC_SUCCESS;
				}
			}
		}

		if(p_wptr->MENUSTATEID < ConfigData.NumOfMenuStates)
		{
			if(gMenuState[p_wptr->MENUSTATEID].state == 0)
			{
				gMenuState[p_wptr->MENUSTATEID].state = 1;
			}
			else if(gMenuState[p_wptr->MENUSTATEID].state == 1)
			{
				gMenuState[p_wptr->MENUSTATEID].state = 0;
			}
		}
	}
	else if(ActionPtr->mInputType == ACTION_MOUSE_RELEASE || ActionPtr->mInputType == ACTION_MOUSE_CANCLE || ActionPtr->mInputType == ACTION_MOUSE_HOLDING) //mouse up
	{
		if(p_wptr->MENUSTATEID < ConfigData.NumOfMenuStates)
		{
			if(gMenuState[p_wptr->MENUSTATEID].state == 0)
			{
				texturePtr[0].mTexAttr &= ~(FORCE_UNDISPLAY);
				texturePtr[1].mTexAttr |= (FORCE_UNDISPLAY);	
			}
			else if(gMenuState[p_wptr->MENUSTATEID].state == 1)
			{
				texturePtr[0].mTexAttr &= ~(FORCE_UNDISPLAY);
				texturePtr[1].mTexAttr &= ~(FORCE_UNDISPLAY);
			}
		}
	}
	//end of MOUSE_TOUCH
	else 
	{
		ERROR_PRINT("ERROR: illegal action trigger button widget");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_ActionTypeErr);
	}

	p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;

	if(gMenuState[p_wptr->MENUSTATEID].state == 0)
	{
		return WIDGET_TOUCH_CANCEL;
	}
	else
	{
		if(p_wptr->TouchPropagation & TouchPropagationSet)
		{
			return WIDGET_TOUCH_CANCEL;
		}
		else
		{
			return AHMI_FUNC_SUCCESS;
		}
	}
}
#endif

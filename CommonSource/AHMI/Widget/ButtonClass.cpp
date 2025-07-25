////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     CanvasClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Revision 1.5.1 - File Modified 2018/11/19 by Yu Chunying 
// Additional Comments:
//    definition of button widget
// 
////////////////////////////////////////////////////////////////////////////////

#include "string.h"
#include "ButtonClass.h"
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "drawImmediately_cd.h"
#ifdef AHMI_DEBUG
#include <stdio.h>
#include "trace.h"
#endif

#ifdef AHMI_CORE
#define Press2DraggingLength    (5)

extern ConfigInfoClass		ConfigData;
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;

extern QueueHandle_t		ActionInstructionQueue;
extern MenuState*			gMenuState;
extern u8*					gStringPtr;
extern StringClassPtr		StringListPtr;

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;

extern uint32_t AHMITick;


ButtonClass::ButtonClass(void)
{
}


ButtonClass::~ButtonClass(void)
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
funcStatus ButtonClass::initWidget(
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
    ActionTriggerClass tagtrigger;
	TagClassPtr bindTag;
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
				gMenuState[p_wptr->MENUSTATEID].state = 0;
			}
		}
		//find the binding tag
        bindTag = &TagPtr->getTag(p_wptr->BindTagID);
		tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
		tagtrigger.mTagPtr = bindTag;
		//refresh the widget value
		if(widgetCtrl(pagePtr, p_wptr,&tagtrigger,u8_pageRefresh) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
        
        p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
	}
	
	#if (defined PC_SIM) || (defined CHIP_GC9002_NoHyper)
	if(p_wptr->MENUSTATEID < ConfigData.NumOfMenuStates)
	{
		if(gMenuState[p_wptr->MENUSTATEID].state != 0)
		{
			//draw all texture
			if(myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn, pagePtr) != AHMI_FUNC_SUCCESS)
				return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);
		}
	}
	else
	{
		//draw all texture
		if(myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn, pagePtr) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);
	}
	
	return AHMI_FUNC_SUCCESS;
	#endif
	
	#if (defined VERSION_2) && (defined EMBEDDED)
	Op_sourceShift = *u32p_sourceShift;
	if(p_wptr->MENUSTATEID < ConfigData.NumOfMenuStates)
	{
		if(gMenuState[p_wptr->MENUSTATEID].state != 0)
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
		}
	}
	else
	{
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
	}
	#endif
	
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
funcStatus ButtonClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh      
	)
{
	u8 button_type;
	u8 button_num;
	TextureClassPtr TexturePtr;
	DynamicPageClassPtr pCurrentPage;
	s32 tag_value = 0;
	u8 flag = 0;
	//TextureClassPtr curTexture;
	int i;
	u8 noTexMode;
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
		ERROR_PRINT("ERROR: when drawing button widght, the offset or width or height exceeds the banduary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	noTexMode = p_wptr->NumOfTex == 0?1:0;

	//get texture
	pCurrentPage = pagePtr;
	if(pCurrentPage->pBasicTextureList == NULL)
	{
		ERROR_PRINT("ERROR: when drawing button widght, the texture list corrupt");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	//validate texture
	if (!noTexMode)
	{
		if(p_wptr->StartNumofTex < pCurrentPage->mTotalNumOfTexture )
			TexturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex]);
		else 
		{
			ERROR_PRINT("ERROR: when drawing button widght, the start texture is more then total number of texture");
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
		}

		if((TexturePtr < (void*)startOfDynamicPage) || (TexturePtr > (void*)endOfDynamicPage))
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
	}
	
	button_type = (p_wptr->WidgetAttr & 0x60) >> 5;

	if(button_type == SINGLEBUTTON)// one button
	{
		//tag set value
		if(ActionPtr->mInputType == ACTION_TAG_SET_VALUE)
		{
			if(ActionPtr->mTagPtr == NULL)//no tag
			{
				ERROR_PRINT("ERROR: the set tag value trigger runs failure in button widget, no tag pointer valid");
				return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
			}
			tag_value = ActionPtr->mTagPtr->getValue();
			TagPtr->getTag(p_wptr->BindTagID).updateTagValueToValueSpace(tag_value);
			if(u8_pageRefresh || (p_wptr->REFRESH_FLAG & WidgetBindCanvasRefreshFlag))
            {
				if (!noTexMode)
				{
					pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].FocusedSlice = 0;
				}
                
				tag_value = 0;

				if(p_wptr->REFRESH_FLAG & WidgetBindCanvasRefreshFlag)
				{
					p_wptr->REFRESH_FLAG &= (~WidgetBindCanvasRefreshFlag);
				}
            }
		}//end of TAG_SET_VALUE
	}
	else if(button_type == SWITCHBUTTON)
	{
		if(ActionPtr->mInputType == ACTION_TAG_SET_VALUE) //set tag value
		{
			if (!noTexMode)
			{
				tag_value = (s32)ActionPtr->mTagPtr->getValue();
				if(tag_value > 0){//press
					TexturePtr->FocusedSlice = 1;
				}else{//release
					TexturePtr->FocusedSlice = 0;
				}
			}
		}
		else 
		{
			ERROR_PRINT("ERROR: illegal action trigger button widget");
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_ActionTypeErr);
		}
	}
	// complex button, serverl button
	else if(button_type == COMPLEXBUTTON)
	{
		button_num = (u8)((u16)(p_wptr->NumOfButtonAndCurHighLight) >> 8);
        
		if(ActionPtr->mInputType == ACTION_TAG_SET_VALUE) //set tag value
		{
			tag_value = (s32)ActionPtr->mTagPtr->getValue();
			//change the tag_value button to show press image
			for(i = 0;i < button_num;i++)
				TexturePtr[i].FocusedSlice = 0;
            
			if(tag_value > 0 && tag_value <= button_num )
			{
                flag = tag_value;
                TexturePtr[flag-1].FocusedSlice = 1;
			}
		}
		else 
		{
			ERROR_PRINT("ERROR: illegal action trigger button widget");
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_ActionTypeErr);
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
funcStatus ButtonClass::SetValueCtrl(
	WidgetClassPtr p_wptr,  
	ActionTriggerClassPtr ActionPtr,
	u8 u8_widgetRefresh      
	)
{
	u8 button_type;
	s32 tag_value = 0;
	
#ifdef AHMI_DEBUG
	//char text[100];
#endif

	//check the parameters
	if((NULL == p_wptr) || (NULL == ActionPtr))
	{
		ERROR_PRINT("ERROR: button widget illegal input");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	
	button_type = (p_wptr->WidgetAttr & 0x60) >> 5;

	if(button_type == SINGLEBUTTON)// one button
	{
		//tag set value
		if(ActionPtr->mInputType == ACTION_TAG_SET_VALUE)
		{
			if(ActionPtr->mTagPtr == NULL)//no tag
			{
				ERROR_PRINT("ERROR: the set tag value trigger runs failure in button widget, no tag pointer valid");
				return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
			}
			tag_value = ActionPtr->mTagPtr->getValue();
			TagPtr->getTag(p_wptr->BindTagID).updateTagValueToValueSpace( tag_value);
			//return AHMI_FUNC_SUCCESS;
		}//end of TAG_SET_VALUE	
	}
	else if(button_type == SWITCHBUTTON)
	{
		if(ActionPtr->mInputType == ACTION_TAG_SET_VALUE) //set tag value
		{
			tag_value = (s32)ActionPtr->mTagPtr->getValue();
            TagPtr->getTag(p_wptr->BindTagID).updateTagValueToValueSpace(tag_value);
		}
		else 
		{
			ERROR_PRINT("ERROR: illegal action trigger button widget");
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_ActionTypeErr);
		}
	}
	// complex button, serverl button
	else if(button_type == COMPLEXBUTTON)
	{
		if(ActionPtr->mInputType == ACTION_TAG_SET_VALUE) //set tag value
		{
			tag_value = (s32)ActionPtr->mTagPtr->getValue();
            TagPtr->getTag(p_wptr->BindTagID).updateTagValueToValueSpace(tag_value);
		}
		else 
		{
			ERROR_PRINT("ERROR: illegal action trigger button widget");
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_ActionTypeErr);
		}
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
funcStatus ButtonClass::MouseTouchCtrl(
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
	u8 button_type;
	u8 button_num;
	u16 button_width;
	u16 button_height;
	s16 button_offsetX;
	s16 button_offsetY;	
	TextureClassPtr TexturePtr;
	DynamicPageClassPtr pCurrentPage;
	u32 actionAddr;
	s32 tag_value = 0;
	s32 duration = 0;
    u32 OldTickForButtonHold = 0;
	int32_t diffX = 0;
	int32_t diffY = 0;
	u8 flag = 0;
	u16 cur_x;
	u16 cur_y;
	u8 signleColorButtonEn; //whether the button is single color
	u8 r,g,b,a;
	u8 noTexMode;
    u8 actionType;
//	u8 Propagationtype = 0;
	//TextureClassPtr curTexture;
	int i,j;
//	CanvasClassPtr curCanvas = &pagePtr->pCanvasList[p_wptr->ATTATCH_CANVAS];
//	SubCanvasClassPtr curSubCan = &(pagePtr->pSubCanvasList[curCanvas->mStartAddrOfSubCanvas + curCanvas->mFocusedCanvas]);
#ifdef AHMI_DEBUG
	//char text[100];
#endif
	noTexMode = p_wptr->NumOfTex == 0?1:0;
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
		ERROR_PRINT("ERROR: when drawing button widght, the offset or width or height exceeds the banduary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}
	
	//get texture
	pCurrentPage = pagePtr;
	if(pCurrentPage->pBasicTextureList == NULL)
	{
		ERROR_PRINT("ERROR: when drawing button widght, the texture list corrupt");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	if (!noTexMode)
	{
		if(p_wptr->StartNumofTex < pCurrentPage->mTotalNumOfTexture )
			TexturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex]);
		else 
		{
			ERROR_PRINT("ERROR: when drawing button widght, the start texture is more then total number of texture");
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
		}

		if((TexturePtr < (void*)startOfDynamicPage) || (TexturePtr > (void*)endOfDynamicPage))
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
	}

	button_type = (p_wptr->WidgetAttr & 0x60) >> 5;
    actionType = ActionPtr->mInputType;
    
    if(actionType == ACTION_MOUSE_PRESS)
    {
        p_wptr->PressFlag = 1;
        
        p_wptr->LongPressFlag = 0;
        p_wptr->HoldFirstFlag = 0;
        OldTickForButtonHold = AHMITick;
        p_wptr->OldTickForHoldL = (u16)OldTickForButtonHold;
        p_wptr->OldTickForHoldH = (u16)(OldTickForButtonHold >> 16);

		p_wptr->TouchPressPosX = pAbsolutePressPoint->x;
		p_wptr->TouchPressPosY = pAbsolutePressPoint->y;
    }
    else if(actionType == ACTION_MOUSE_HOLDING)
    {
		if(p_wptr->TouchPropagation & TouchPropagationSet)
		{
			if(p_wptr->TouchPropagation & TouchStatePropagationWithCancle)
			{

			}
			else
			{
				diffX = pAbsoluteReleasePoint->x - p_wptr->TouchPressPosX;
				diffY = pAbsoluteReleasePoint->y - p_wptr->TouchPressPosY;
		
				if(diffX < 0)
					diffX = 0 - diffX;
		
				if(diffY < 0)
					diffY = 0 - diffY;
		
				if((diffX > WIDGET_TOUCH_DIFF) || (diffY > WIDGET_TOUCH_DIFF))
				{
					p_wptr->TouchPropagation &= TouchPropagationSet;
					p_wptr->TouchPropagation |= TouchStatePropagationWithCancle;
				}
				else
				{
					p_wptr->TouchPropagation &= TouchPropagationSet;
				}
			}
		}
		else
		{
			if(p_wptr->PressFlag)
			{
				if(((pReleasePoint->x) <= 0) || ((pReleasePoint->x) >= (p_wptr->WidgetWidth)) ||
					((pReleasePoint->y) <= 0) || ((pReleasePoint->y) >= (p_wptr->WidgetHeight)))
				{
					diffX = pReleasePoint->x - pPressPoint->x;
					diffY = pReleasePoint->y - pPressPoint->y;
			
					if(diffX < 0)
						diffX = 0 - diffX;
			
					if(diffY < 0)
						diffY = 0 - diffY;
					
					if((diffX > WIDGET_TOUCH_DIFF) || (diffY > WIDGET_TOUCH_DIFF))
					{
						actionType = ACTION_MOUSE_CANCLE;
					}
				}
			}
		}

		if((p_wptr->TouchPropagation & TouchPropagationSet) && (p_wptr->TouchPropagation & TouchStatePropagationWithCancle))
		{

		}
		else if(actionType == ACTION_MOUSE_CANCLE)
		{

		}
		else
		{
			if(p_wptr->PressFlag)
			{
			    /* Timing 500ms */
			    if(p_wptr->LongPressFlag == 0)
			    {
			        OldTickForButtonHold = AHMITick;
			        p_wptr->OldTickForHoldL = (u16)OldTickForButtonHold;
			        p_wptr->OldTickForHoldH = (u16)(OldTickForButtonHold >> 16);
			        p_wptr->LongPressFlag = 1;
			    }
			    
			    OldTickForButtonHold = (u32)((p_wptr->OldTickForHoldH  << 16) + p_wptr->OldTickForHoldL);
			    
			    duration = AHMITick - OldTickForButtonHold;
    
			    if(duration < 0)
			        duration  = 0 - duration;
			    
			    if(p_wptr->HoldFirstFlag == 0)
			    {
			        /* The timing didn't reach 500ms */
					#ifdef PC_SIM
					if(duration <= 10)
					#endif
					#if (defined VERSION_2) && (defined EMBEDDED)  
			        if(duration <= 1000)
					#endif
			        {
			            actionType = ACTION_MOUSE_HOLDING;
			        }
			        /* Reach 1000ms */
			        else
			        {
			            /* clear time flag */
			            p_wptr->LongPressFlag = 0;
			            
			            {/* Less position offset */
    
			                actionType = ACTION_MOUSE_LONGPRESS;
			                p_wptr->HoldFirstFlag = 1;
			            }
			        }
			    }
			    else
			    {
			        /* The timing didn't reach 80ms */
			        #ifdef PC_SIM
					if(duration <= 2)
					#endif
					#if (defined VERSION_2) && (defined EMBEDDED)  
			        if(duration <= 200)
					#endif
			        {
			            actionType = ACTION_MOUSE_HOLDING;;
			        }
			        /* Reach 200ms */
			        else
			        {
			            /* clear time flag */
			            p_wptr->LongPressFlag = 0;
			            
			            actionType = ACTION_MOUSE_LONGPRESS;
    
			        }
			    }
			}
		}
    }
    else if((actionType == ACTION_MOUSE_CANCLE) || (actionType == ACTION_MOUSE_RELEASE))
    {
        //p_wptr->PressFlag = 0;
        p_wptr->HoldFirstFlag = 0;
        p_wptr->LongPressFlag = 0;

		p_wptr->TouchPressPosX = 0;
		p_wptr->TouchPressPosY = 0;
    }
    
	if(button_type == SINGLEBUTTON)// one button
	{
		signleColorButtonEn = (p_wptr->WidgetAttr & SINGLE_COLOR_BUTTON_BIT) ? 1 : 0; //whether it is color button

		//mouse
		if( actionType == ACTION_MOUSE_PRESS ) //mouse press
		{
			tag_value = 1;
			if (!noTexMode)
			{
				if(signleColorButtonEn)
				{
					//change into the press color
					a = 0xff;
					r = (p_wptr-> PressColorValueRG & 0xff00) >> 8;
					g = (p_wptr-> PressColorValueRG & 0x00ff) ;
					b = (p_wptr -> PressValueBAndReleaseColorValueR & 0xff00) >> 8;
					pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].TexAddr = (a << 24) + (r << 16) + (g << 8) + b;
				}
				else
				{
					if(p_wptr->MENUSTATEID < ConfigData.NumOfMenuStates)
					{
						if(gMenuState[p_wptr->MENUSTATEID].mode != 0) //for Multiple choice
						{
							if(p_wptr->MENUITEMVALUE == 32) //for confirm
							{
								if(p_wptr->TouchPropagation & TouchPropagationSet)
								{
									//change menu container state
									gMenuState[p_wptr->MENUSTATEID].statebyItem = 0;
								}
								else
								{
									//change menu container state
									gMenuState[p_wptr->MENUSTATEID].state = 0;
								}

								//set number tag
								if((gMenuState[p_wptr->MENUSTATEID].tagID > 0) && (gMenuState[p_wptr->MENUSTATEID].tagID < ConfigData.NumofTags))
								{
									TagPtr->getTag(gMenuState[p_wptr->MENUSTATEID].tagID).setRawValue(gMenuState[p_wptr->MENUSTATEID].currentValue);
								}
							}
							else //for other menuitem
							{
								if(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].FocusedSlice == 0)
								{
									pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].FocusedSlice = 1;

									if(p_wptr->MENUITEMVALUE < 32)
									{
										gMenuState[p_wptr->MENUSTATEID].currentValue |= (1 << p_wptr->MENUITEMVALUE);

										for(i = 0; i < 32; i++)
										{
											if(gMenuState[p_wptr->MENUSTATEID].curMenuItemSequence[i] == 0xFFFF)
											{
												gMenuState[p_wptr->MENUSTATEID].curMenuItemSequence[i] = p_wptr->MENUITEMSTRINGID;
												break;
											}
											else
											{
												continue;
											}
										}
									}
								}
								else if(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].FocusedSlice == 1)
								{
									pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].FocusedSlice = 0;

									if(p_wptr->MENUITEMVALUE < 32)
									{
										gMenuState[p_wptr->MENUSTATEID].currentValue &= ~(1 << p_wptr->MENUITEMVALUE);

										for(i = 0; i < 32; i++)
										{
											if(gMenuState[p_wptr->MENUSTATEID].curMenuItemSequence[i] == p_wptr->MENUITEMSTRINGID)
											{
												for(j = i; j < 31; j++)
												{
													gMenuState[p_wptr->MENUSTATEID].curMenuItemSequence[j] = gMenuState[p_wptr->MENUSTATEID].curMenuItemSequence[j + 1];
												}
												gMenuState[p_wptr->MENUSTATEID].curMenuItemSequence[31] = 0xFFFF;
												break;
											}
											else
											{
												continue;
											}
										}
									}
								}

								if(gMenuState[p_wptr->MENUSTATEID].curMenuItemSequence[0] != 0xFFFF)
								{
									if(gMenuState[p_wptr->MENUSTATEID].curMenuItemSequence[1] != 0xFFFF)
									{
										u16 strlength = StringListPtr[gMenuState[p_wptr->MENUSTATEID].curMenuItemSequence[0]].length;
										u8* curStr = gStringPtr + StringListPtr[gMenuState[p_wptr->MENUSTATEID].curMenuItemSequence[0]].addr;
										u8 tempStr[32] = {0};
										for(i = 0; i < strlength; i++)
										{
											tempStr[i] = *curStr++;
										}
										strcat((char*)tempStr,"...");
										TagPtr->getTag(p_wptr->BindTagID).setString((char*)tempStr, 1);
									}
									else
									{
//										u16 strlength = StringListPtr[gMenuState[p_wptr->MENUSTATEID].curMenuItemSequence[0]].length;
										u8* curStr = gStringPtr + StringListPtr[gMenuState[p_wptr->MENUSTATEID].curMenuItemSequence[0]].addr;
										TagPtr->getTag(p_wptr->BindTagID).setString((char*)curStr, 1);
									}
								}
								else
								{
									char showStr[32] = {0};
									TagPtr->getTag(p_wptr->BindTagID).setString(showStr, 1);
								}
							}
						}
						else //for Single choice
						{
							if(gMenuState[p_wptr->MENUSTATEID].state == 1)
							{
								if(p_wptr->TouchPropagation & TouchPropagationSet)
								{
									//change menu container state
									gMenuState[p_wptr->MENUSTATEID].statebyItem = 0;
								}
								else
								{
									//change menu container state
									gMenuState[p_wptr->MENUSTATEID].state = 0;
								}

								//set number tag
								if((gMenuState[p_wptr->MENUSTATEID].tagID > 0) && (gMenuState[p_wptr->MENUSTATEID].tagID < ConfigData.NumofTags))
								{
									TagPtr->getTag(gMenuState[p_wptr->MENUSTATEID].tagID).setRawValue(p_wptr->MENUITEMVALUE);
								}

								//set string tag
								if((p_wptr->BindTagID > 0) && (p_wptr->BindTagID < ConfigData.NumofTags))
								{
									if(p_wptr->MENUITEMSTRINGID < ConfigData.NumofStrings)
									{
//										u16 strlength = StringListPtr[p_wptr->MENUITEMSTRINGID].length;
										u8* curStr = gStringPtr + StringListPtr[p_wptr->MENUITEMSTRINGID].addr;
										TagPtr->getTag(p_wptr->BindTagID).setString((char*)curStr, 1);
									}
								}
							}
						}
					}
					else
					{
						pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].FocusedSlice = 1; //change into press image
					}
				}
			}	

			//send press action
			if(p_wptr->OnPressAction <= pCurrentPage->mActionInstructionsSize)
			{
				actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnPressAction);
				csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
			}
		}
		else if(actionType == ACTION_MOUSE_RELEASE) //mouse up
		{
			if(p_wptr->PressFlag == 1)
			{
				p_wptr->PressFlag = 0;
				tag_value = 0;
				if (!noTexMode)
				{
					if(signleColorButtonEn)
					{
						//change into the release color
						a = 0xff;
						r = (p_wptr->PressValueBAndReleaseColorValueR & 0x00ff) ;
						g = (p_wptr->ReleaseColorValueGB & 0xff00) >> 8;
						b = (p_wptr->ReleaseColorValueGB & 0x00ff) ;
						pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].TexAddr = (a << 24) + (r << 16) + (g << 8) + b;
					}
					else 
					{
						if(p_wptr->MENUSTATEID < ConfigData.NumOfMenuStates)
						{
							
						}
						else
						{
							pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].FocusedSlice = 0;//change into release image
						}
					}
				}
				
				if(p_wptr->TouchPropagation & TouchStatePropagationWithCancle)
				{
					p_wptr->TouchPropagation &= (~TouchStatePropagationWithCancle);
				}
				else
				{
					if (((pReleasePoint->x - pPressPoint->x) < -Press2DraggingLength) || ((pReleasePoint->x - pPressPoint->x) > Press2DraggingLength) 
						|| ((pReleasePoint->y - pPressPoint->y) < -Press2DraggingLength) || ((pReleasePoint->y - pPressPoint->y) > Press2DraggingLength))
					{
					}else{
						//release without move mouse
						//send clickRelease action
						if(p_wptr->OnClickReleaseAction <= pCurrentPage->mActionInstructionsSize)
						{
							actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnClickReleaseAction);
							csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
						}
					}
					//send release action
					if(p_wptr->OnRealeaseAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnRealeaseAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}
					
				}
			}
		}
		else if(actionType == ACTION_MOUSE_CANCLE) //cancle, just release, donnot send action
		{
			if(p_wptr->PressFlag)
			{
				p_wptr->PressFlag = 0;
				tag_value = 0;
				if (!noTexMode)
				{
					if(signleColorButtonEn)
					{
						//change into the release color
						a = 0xff;
						r = (p_wptr->PressValueBAndReleaseColorValueR & 0x00ff) ;
						g = (p_wptr->ReleaseColorValueGB & 0xff00) >> 8;
						b = (p_wptr->ReleaseColorValueGB & 0x00ff) ;
						pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].TexAddr = (a << 24) + (r << 16) + (g << 8) + b;
					}
					else 
					{
						if(p_wptr->MENUSTATEID < ConfigData.NumOfMenuStates)
						{
							
						}
						else
						{
							pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].FocusedSlice = 0;//change into release image
						}
					}
				}
				
				if(p_wptr->TouchPropagation & TouchStatePropagationWithCancle)
				{
					p_wptr->TouchPropagation &= (~TouchStatePropagationWithCancle);
				}
				//send swipeOut action
				if(p_wptr->OnSwipeOutAction <= pCurrentPage->mActionInstructionsSize)
				{
					actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnSwipeOutAction);
					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
				}
			}
		}
		else if(actionType == ACTION_MOUSE_HOLDING)
		{
			if(p_wptr->TouchPropagation & TouchPropagationSet)
			{
				if(p_wptr->TouchPropagation & TouchStatePropagationWithCancle)
				{
					tag_value = 0;
					if (!noTexMode)
					{
						if(signleColorButtonEn)
						{
							//change into the release color
							a = 0xff;
							r = (p_wptr->PressValueBAndReleaseColorValueR & 0x00ff) ;
							g = (p_wptr->ReleaseColorValueGB & 0xff00) >> 8;
							b = (p_wptr->ReleaseColorValueGB & 0x00ff) ;
							pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].TexAddr = (a << 24) + (r << 16) + (g << 8) + b;
						}
						else
							pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].FocusedSlice = 0; //change into press image
					}
				}
			}
		}
		else if(actionType == ACTION_MOUSE_LONGPRESS)
		{
			if(p_wptr->TouchPropagation & TouchStatePropagationWithCancle)
			{

			}
			else
			{
				if(p_wptr->PressFlag == 1)
				{
					//send longpress action
					if(p_wptr->OnLongPressAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnLongPressAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}
				}
			}
		}
		//end of MOUSE_TOUCH
		else 
		{
			ERROR_PRINT("ERROR: illegal action trigger button widget");
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_ActionTypeErr);
		}
	}
	// switch button
	else if(button_type == SWITCHBUTTON)
	{
		if(actionType == ACTION_MOUSE_PRESS) //mouse down
		{
			tag_value = TagPtr->getTag(p_wptr->BindTagID).getValue();
			if(tag_value > 0){
				TagPtr->getTag(p_wptr->BindTagID).setRawValue(0);
			}else{
				TagPtr->getTag(p_wptr->BindTagID).setRawValue(1);
			}
			//send press action
			if(p_wptr->OnPressAction <= pCurrentPage->mActionInstructionsSize)
			{
				actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnPressAction);
				csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
			}
		}
		else if(actionType == ACTION_MOUSE_RELEASE) //mouse up
		{
			if(p_wptr->PressFlag == 1)
			{
				p_wptr->PressFlag = 0;
				//send release action
				if(p_wptr->OnRealeaseAction <= pCurrentPage->mActionInstructionsSize)
				{
					actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnRealeaseAction);
					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
				}
			}
		}
		else if(actionType == ACTION_MOUSE_LONGPRESS)
		{
			if(p_wptr->PressFlag == 1)
			{
				//send longpress action
				if(p_wptr->OnLongPressAction <= pCurrentPage->mActionInstructionsSize)
				{
					actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnLongPressAction);
					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
				}
			}
		}
		else if (actionType == ACTION_MOUSE_HOLDING || actionType == ACTION_MOUSE_CANCLE)
		{

		}
		else 
		{
			ERROR_PRINT("ERROR: illegal action trigger button widget");
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_ActionTypeErr);
		}
	}
	// complex button, serverl button
	else if(button_type == COMPLEXBUTTON)
	{
		button_num = (u8)((u16)(p_wptr->NumOfButtonAndCurHighLight) >> 8);
		if(actionType == ACTION_MOUSE_PRESS) //mouse down
		{
			cur_x = ((s16)(pPressPoint->x)) << 4; //mouse position
			cur_y = ((s16)(pPressPoint->y)) << 4;
			for(i = 0;i < button_num;i++)
			{
				button_width = TexturePtr[i].TexWidth;
				button_height = TexturePtr[i].TexHeight;
				button_offsetX = TexturePtr[i].OffsetX;
				button_offsetY = TexturePtr[i].OffsetY;	
				//find which button is on
				if(!(cur_x < button_offsetX || cur_x > button_offsetX + (button_width << 4) || cur_y < button_offsetY || cur_y > button_offsetY + (button_height << 4)) && !flag)
				{
					flag = i + 1;
					tag_value = i + 1;
				}
			}
			if(flag)
			{
				//call tag_set_value
				TagPtr->getTag(p_wptr->BindTagID).setRawValue(tag_value);
			}
			//send press action
			if(p_wptr->OnPressAction <= pCurrentPage->mActionInstructionsSize)
			{
				actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnPressAction);
				csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
			}
		}//end of mouse press
		else if (actionType == ACTION_MOUSE_RELEASE)
		{
			//send release action
			if(p_wptr->OnRealeaseAction <= pCurrentPage->mActionInstructionsSize)
			{
				actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnRealeaseAction);
				csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
			}
		}
		else if (actionType == ACTION_MOUSE_LONGPRESS || actionType == ACTION_MOUSE_HOLDING || actionType == ACTION_MOUSE_CANCLE)
		{

		}
		else 
		{
			ERROR_PRINT("ERROR: illegal action trigger button widget");
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_ActionTypeErr);
		}
	}
    
    p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;

	if((p_wptr->TouchPropagation & TouchPropagationSet) || (ActionPtr->mInputType == ACTION_MOUSE_CANCLE))
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
//widgetCtrl
//15-11: reserved
//11:  whether the button is color button
//10-7: only used in complex button, the number of button. 1-15
//6-5: The mode of button:0 is single button,1 is switch button,2 is complex button
//4-0: type of widget, button is BUTTON_TEX
//**************************************
funcStatus ButtonClass::KeyboardTouchCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh      
	)
{
	u8 button_type;
	u8 button_num;
	TextureClassPtr TexturePtr;
	DynamicPageClassPtr pCurrentPage;
	u32 actionAddr;
	s32 tag_value = 0;
	u8 curHighLight; //current highlight button
	u8 flag = 0;

	//TextureClassPtr curTexture;
    
	int i;
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
		ERROR_PRINT("ERROR: when drawing button widght, the offset or width or height exceeds the banduary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	//get texture
	pCurrentPage = pagePtr;
	if(pCurrentPage->pBasicTextureList == NULL)
	{
		ERROR_PRINT("ERROR: when drawing button widght, the texture list corrupt");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	if(p_wptr->StartNumofTex < pCurrentPage->mTotalNumOfTexture )
		TexturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex]);
	else 
	{
		ERROR_PRINT("ERROR: when drawing button widght, the start texture is more then total number of texture");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
	}

	if((TexturePtr < (void*)startOfDynamicPage) || (TexturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);

	button_type = (p_wptr->WidgetAttr & 0x60) >> 5;

	if(button_type == SINGLEBUTTON)// one button
	{
		if(ActionPtr->mInputType == ACTION_KEYBOARD_PRE || ActionPtr->mInputType == ACTION_KEYBOARD_NEXT || ActionPtr->mInputType == ACTION_KEYBOARD_OK)//keyboard
		{
			//tag_value = ActionPtr->mTagPtr->mValue;
			curHighLight = (u8)(p_wptr->NumOfButtonAndCurHighLight);
			if(ActionPtr->mInputType == ACTION_KEYBOARD_PRE ||ActionPtr->mInputType == ACTION_KEYBOARD_NEXT) //left, the highlight goes away
			{
				if(curHighLight == 0) //LIGHT ON
				{
					curHighLight = 1;
					pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 1].mTexAttr &= ~(FORCE_UNDISPLAY); //highlight color on
					p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighLight;
				}
				else if(curHighLight == 1)//LIGHT OFF
				{
					curHighLight = 0;
					pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 1].mTexAttr |= (FORCE_UNDISPLAY); //highlight color off
					p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighLight;
				}
			}
			else if(ActionPtr->mInputType == ACTION_KEYBOARD_OK) //enter
			{
				if(p_wptr->OnEnteredAction <= pCurrentPage->mActionInstructionsSize)
				{
					actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnEnteredAction);
					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
				}
				return AHMI_FUNC_SUCCESS;
			}
		}
		else if(ActionPtr->mInputType == ACTION_KEYBOARD_CLEAR)// clear the highlight 
		{
			curHighLight = 0;
			pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 1].mTexAttr |= (FORCE_UNDISPLAY); //highlight color off
			p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighLight;
		}
		else 
		{
			ERROR_PRINT("ERROR: illegal action trigger button widget");
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_ActionTypeErr);
		}
	
	}
	// complex button, serverl button
	else if(button_type == COMPLEXBUTTON)
	{
		button_num = (u8)((u16)(p_wptr->NumOfButtonAndCurHighLight) >> 8);
	
		if(ActionPtr->mInputType == ACTION_KEYBOARD_NEXT || ActionPtr->mInputType == ACTION_KEYBOARD_OK || ActionPtr->mInputType == ACTION_KEYBOARD_PRE) //keyboard
		{
			curHighLight = (u8)(p_wptr->NumOfButtonAndCurHighLight);
			//highlight go away
			if( (curHighLight == 1 && ActionPtr->mInputType == ACTION_KEYBOARD_PRE) || (curHighLight == button_num && ActionPtr->mInputType == ACTION_KEYBOARD_NEXT))   		
            {
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].mTexAttr |= (FORCE_UNDISPLAY); //turn off the highlight
				curHighLight = 0;
				p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighLight;
			}
			//highlight moves from right
			else if(curHighLight == 0 && ActionPtr->mInputType == ACTION_KEYBOARD_PRE)
			{
				curHighLight = (u8)button_num;
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].mTexAttr &= ~(FORCE_UNDISPLAY); //turn on the highlight
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].OffsetX = pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + curHighLight - 1].OffsetX;
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].OffsetY = pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + curHighLight - 1].OffsetY;
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].TexWidth = pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + curHighLight - 1].TexWidth;
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].TexHeight = pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + curHighLight - 1].TexHeight;
				//pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].renewTextureSourceBox(NULL,NULL,NULL);
				p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighLight;

			}
			//highlight moves from left
			else if(curHighLight == 0 && ActionPtr->mInputType == ACTION_KEYBOARD_NEXT)
			{
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].mTexAttr &= ~(FORCE_UNDISPLAY); //highlight turn on
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].OffsetX = pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].OffsetX;
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].OffsetY = pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].OffsetY;
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].TexWidth = pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].TexWidth;
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].TexHeight = pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].TexHeight;
				//pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].renewTextureSourceBox(NULL,NULL,NULL);
				curHighLight = 1;
				p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighLight;
			}
			else if(ActionPtr->mInputType == ACTION_KEYBOARD_PRE || ActionPtr->mInputType == ACTION_KEYBOARD_NEXT)//highlight change to another button
			{
				if(ActionPtr->mInputType == ACTION_KEYBOARD_PRE)
					curHighLight --;
				else if(ActionPtr->mInputType == ACTION_KEYBOARD_NEXT)
					curHighLight ++;
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].OffsetX = pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + curHighLight - 1].OffsetX;
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].OffsetY = pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + curHighLight - 1].OffsetY;
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].TexWidth = pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + curHighLight - 1].TexWidth;
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].TexHeight = pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + curHighLight - 1].TexHeight;
				//pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].renewTextureSourceBox(NULL,NULL,NULL);
				p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighLight;
			}
			else if(ActionPtr->mInputType == ACTION_KEYBOARD_OK)//OK
			{
				tag_value = curHighLight;
				for(i = 0;i < button_num;i++)
					TexturePtr[i].FocusedSlice = 0;
				if(tag_value > 0 && tag_value <= button_num )
				{
					flag = tag_value;
					TexturePtr[flag-1].FocusedSlice = 1;
				}
				if(ActionPtr->mTagPtr == NULL)//no tag
				{
					ERROR_PRINT("ERROR: the set tag value trigger runs failure in button widget, no tag pointer valid");
					return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
				}
				ActionPtr->mTagPtr->updateTagValueToValueSpace(tag_value);
				if(p_wptr->OnEnteredAction <= pCurrentPage->mActionInstructionsSize)
				{
					actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnEnteredAction);
					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
				}
			}
		}
		else if(ActionPtr->mInputType == ACTION_KEYBOARD_CLEAR)// clear the highlight 
		{
			pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + button_num].mTexAttr |= (FORCE_UNDISPLAY); 
			curHighLight = 0;
			p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighLight;
		}//end of clear highlight
		else 
		{
			ERROR_PRINT("ERROR: illegal action trigger button widget");
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_ActionTypeErr);
		}
	}

    p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
    
	return AHMI_FUNC_SUCCESS;
}

#endif

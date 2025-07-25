////////////////////////////////////////////////////////////////////////////////
// Created by Zzen1sS
// 2020/3/18
////////////////////////////////////////////////////////////////////////////////

#include "SwipeResponderClass.h"
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "drawImmediately_cd.h"
#ifdef AHMI_DEBUG
#include <stdio.h>
#include "trace.h"
#endif

#ifdef AHMI_CORE
#define Press2DraggingLength    (5)

extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;

extern QueueHandle_t		ActionInstructionQueue;

extern uint32_t AHMITick;


SwipeResponderClass::SwipeResponderClass(void)
{
}


SwipeResponderClass::~SwipeResponderClass(void)
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
funcStatus SwipeResponderClass::initWidget(
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
	//WidgetClassInterface myWidgetClassInterface;
 //   ActionTriggerClass tagtrigger;
	//if((NULL == p_wptr) || (NULL == u32p_sourceShift) || (NULL == pTileBox))
	//	return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	//
 //   
	//if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag))
	//{
	//	
	//	tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
	//	//refresh the widget value
	//	if(widgetCtrl(p_wptr,&tagtrigger,u8_pageRefresh) != AHMI_FUNC_SUCCESS)
	//		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
 //       
 //       p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
	//}
	//draw all texture
	/*if(myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift,RefreshType,pTileBox,staticTextureEn) != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);*/
	
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
funcStatus SwipeResponderClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh      
	)
{
	
    
	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Tag.setValue
//SetValueCtrl
//15-11: reserved
//11:  whether the button is color button
//10-7: only used in complex button, the number of button. 1-15
//6-5: The mode of button:0 is single button,1 is switch button,2 is complex button
//4-0: type of widget, button is BUTTON_TEX
//**************************************
funcStatus SwipeResponderClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh      
	)
{
	
    
	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Mouse.Touch
//Do not support touch propagation.
//15-11: reserved
//11:  whether the button is color button
//10-7: only used in complex button, the number of button. 1-15
//6-5: The mode of button:0 is single button,1 is switch button,2 is complex button
//4-0: type of widget, button is BUTTON_TEX
//**************************************
funcStatus SwipeResponderClass::MouseTouchCtrl(
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
	DynamicPageClassPtr pCurrentPage;
	u32 actionAddr;
//	s32 tag_value = 0;
//	s32 duration = 0;
//	u8 flag = 0;
	s16 cur_x;
	s16 cur_y;
	s16 distanceH;
	s16 distanceV;
	u8 directionH;
	u8 directionV;
//	int i;
	s32 duration = 0;
	u32 oldTick;
	u8 actionType;
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

	pCurrentPage =  pagePtr;

	//check long press
	actionType = ActionPtr->mInputType;
	if (actionType == ACTION_MOUSE_HOLDING)
	{
		if(p_wptr->PressFlag)
		{
			uint8_t checklongpressFlag = 0;
			directionH = p_wptr->SwipeDirection & 0x01;
			directionV = (p_wptr->SwipeDirection & 0x02)>>1;
			cur_x  = (s16)(pReleasePoint->x);
			cur_y  = (s16)(pReleasePoint->y);
			
			if (directionH && directionV)
			{
				//both horizontal and vertical
				if (cur_x - (s16)p_wptr->PressedPosX < Press2DraggingLength && cur_x - (s16)p_wptr->PressedPosX > -Press2DraggingLength && 
					cur_y - (s16)p_wptr->PressedPosY < Press2DraggingLength && cur_y - (s16)p_wptr->PressedPosY > -Press2DraggingLength)
				{
					checklongpressFlag = 1;
				}

			}else if (directionH && !directionV)
			{
				//only horizontal
				if (cur_x - (s16)p_wptr->PressedPosX < Press2DraggingLength && cur_x - (s16)p_wptr->PressedPosX > -Press2DraggingLength)
				{
					checklongpressFlag = 1;
				}
			}else if (!directionH && directionV)
			{
				//only vertical
				if (cur_y - (s16)p_wptr->PressedPosY < Press2DraggingLength && cur_y - (s16)p_wptr->PressedPosY > -Press2DraggingLength)
				{
					checklongpressFlag = 1;
				}
			}else{
				//neither horizontal nor vertical
				checklongpressFlag = 1;
			}
			
			if(checklongpressFlag)
			{
				oldTick = (u32)((p_wptr->SwipeTickH  << 16) + p_wptr->SwipeTickL);
				duration = AHMITick - oldTick;

				if(duration < 0)
					duration  = 0 - duration;

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
					p_wptr->SwipeTickL = (u16)AHMITick;
					p_wptr->SwipeTickH = (u16)(AHMITick >> 16);
					actionType = ACTION_MOUSE_LONGPRESS;
				}
			}
		}
	}
	
	if( actionType == ACTION_MOUSE_PRESS ) //mouse press
	{
		cur_x  = (s16)(pPressPoint->x);
		cur_y  = (s16)(pPressPoint->y);
		p_wptr->PressedPosX = cur_x;
		p_wptr->PressedPosY = cur_y;
        p_wptr->PressFlag = 1;

		//pressed
		p_wptr->SwipeStatus = 1;

        oldTick = AHMITick;
        p_wptr->SwipeTickL = (u16)oldTick;
        p_wptr->SwipeTickH = (u16)(oldTick >> 16);

		//send press action
		if(p_wptr->OnPressAction <= pCurrentPage->mActionInstructionsSize)
        {
            actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnPressAction);
			csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
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
	else if(actionType == ACTION_MOUSE_HOLDING) //mouse holding
	{
        if(p_wptr->PressFlag)
        {
			if((p_wptr->SwipeStatus & 0x2) == 0)
			{
				if(cur_x - (s16)p_wptr->PressedPosX < -Press2DraggingLength || cur_x - (s16)p_wptr->PressedPosX > Press2DraggingLength ||
				   cur_y - (s16)p_wptr->PressedPosY < -Press2DraggingLength || cur_y - (s16)p_wptr->PressedPosY > Press2DraggingLength)
				{
					//dragging
					p_wptr->SwipeStatus |= 0x2;
				}
			}
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
	else if(actionType == ACTION_MOUSE_RELEASE || actionType == ACTION_MOUSE_CANCLE) //mouse release or cancel
	{
        if(p_wptr->PressFlag)
        {
            directionH = p_wptr->SwipeDirection & 0x01;
            directionV = (p_wptr->SwipeDirection & 0x02)>>1;
            distanceH = p_wptr->SwipeDistanceH;
            distanceV = p_wptr->SwipeDistanceV;
            cur_x  = (s16)(pReleasePoint->x);
            cur_y  = (s16)(pReleasePoint->y);

			if(p_wptr->SwipeStatus & 0x2)
			{
				//dragging, Determine whether the four directions of top, bottom, left, and right have been triggered
				if (directionH)
				{
					//horizontal
					if (cur_x - (s16)p_wptr->PressedPosX > distanceH)
					{
						//swipe right
						if(p_wptr->SwipeRightAction <= pCurrentPage->mActionInstructionsSize)
						{
							actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->SwipeRightAction);
							csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
						}
					}
					else if (cur_x - (s16)p_wptr->PressedPosX < -distanceH)
					{
						//swipe left
						if(p_wptr->SwipeLeftAction <= pCurrentPage->mActionInstructionsSize)
						{
							actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->SwipeLeftAction);
							csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
						}
					}
				}
				if (directionV)
				{
					//vertical
					if (cur_y - (s16)p_wptr->PressedPosY > distanceV)
					{
						//swipe bottom
						if(p_wptr->SwipeBottomAction <= pCurrentPage->mActionInstructionsSize)
						{
							actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->SwipeBottomAction);
							csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
						}
					}
					else if (cur_y - (s16)p_wptr->PressedPosY < -distanceV)
					{
						//swipe top
						if(p_wptr->SwipeTopAction <= pCurrentPage->mActionInstructionsSize)
						{
							actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->SwipeTopAction);
							csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
						}
					}
				}
			}
			else
			{
				if(actionType == ACTION_MOUSE_RELEASE)
				{
					//clickrelease, send clickrelease action if needed
					if(p_wptr->OnRealeaseAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnRealeaseAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}
				}
            }

			p_wptr->PressFlag = 0;
			p_wptr->SwipeStatus = 0;
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
	else if(actionType == ACTION_MOUSE_LONGPRESS) //mouse long press
	{
		if(p_wptr->PressFlag)
		{
            //send longpress action
			if(p_wptr->OnLongPressAction <= pCurrentPage->mActionInstructionsSize)
			{
				actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnLongPressAction);
				csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
			}
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
	else 
	{
		ERROR_PRINT("ERROR: illegal action trigger SwipeResponder widget");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_ActionTypeErr);
	}

}
#endif

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:    2023/9/5
// File Name:     ListClass.cpp
// Project Name:  AHMISimulator
// 
// 
////////////////////////////////////////////////////////////////////////////////

#include "ListClass.h"
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "drawImmediately_cd.h"
#ifdef AHMI_DEBUG
#include <stdio.h>
#include "trace.h"
#endif

#ifdef AHMI_CORE
#define PressToDraggingThreshold      (0xA)

extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
//extern u16 WorkingPageID;
extern uint32_t AHMITick;
extern QueueHandle_t		ActionInstructionQueue;

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;

extern Easing gEasing;
extern u8 force_cancel_subcanvas_page_mousetouch;

ListClass::ListClass(void)
{
}


ListClass::~ListClass(void)
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
funcStatus ListClass::initWidget(
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
	//draw all texture
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
//reponse for Tag.setValue

//**************************************
funcStatus ListClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh      
	)
{
//	u8 button_type;
//	u8 button_num;
	TextureClassPtr TexturePtr;
	DynamicPageClassPtr pCurrentPage;
	s32 tag_value = 0;
//	u8 flag = 0;
	u16 singleLength;//length of each selector item
	//TextureClassPtr curTexture;
	s32 baseOffset;
	
	u16 count;
	u16 showCount;
	u8 pressed;
//	u8 animating;
	
	u8 arrange;
	
//	u8 cycle;
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
		ERROR_PRINT("ERROR: when drawing list widght, the offset or width or height exceeds the banduary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	//get texture
	pCurrentPage = pagePtr;
	if(pCurrentPage->pBasicTextureList == NULL)
	{
		ERROR_PRINT("ERROR: when drawing list widght, the texture list corrupt");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	if(p_wptr->StartNumofTex < pCurrentPage->mTotalNumOfTexture )
		TexturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex]);
	else 
	{
		ERROR_PRINT("ERROR: when drawing list widght, the start texture is more then total number of texture");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
	}

	if((TexturePtr < (void*)startOfDynamicPage) || (TexturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
	
	count = p_wptr->ListItemCount;
	showCount = p_wptr->ListShowNum;
	arrange = p_wptr->ListArrange;
	
//	cycle = (p_wptr->SELECTOROPTIONS & 0x02)>>1;

	if(ActionPtr->mInputType == ACTION_TAG_SET_VALUE) //set tag value
	{
		renderTexture(pagePtr, p_wptr, ActionPtr->mTagPtr);
		
	}
		
	else 
	{
		ERROR_PRINT("ERROR: illegal action trigger list widget");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_ActionTypeErr);
	}
    
	return AHMI_FUNC_SUCCESS;
}


funcStatus ListClass::renderTexture
		 (
		 DynamicPageClassPtr pagePtr,
		 WidgetClassPtr p_wptr,  
		 TagClassPtr tagPtr
		 ){
	s32 curValue;

	TextureClassPtr TexturePtr;
	
	TextureClassPtr curTexturePtr;
	DynamicPageClassPtr pCurrentPage;
	u16 count;
	u16 showCount;
	u16 singleLength;//length of each selector item
	//TextureClassPtr curTexture;
	s32 baseOffset;
	s32 offset;
	u16 padding;
	
	u8 arrange;
	u8 curHighLight;
	int i;
	int baseIdx;
	
	u8 modifyEn = p_wptr->ListHighlightOptions & 0x01;
	//get texture
	pCurrentPage = pagePtr;
	TexturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex+2]);

	padding = p_wptr->ListPadding;
	
	count = p_wptr->ListItemCount;
	
	showCount = p_wptr->ListShowNum;
	
	u16 valueCount = tagPtr->getCurArryElemNum();

	u16 highlightElemID = p_wptr->ListHighlightElemID;
	
	arrange = p_wptr->ListArrange;
	//2 ~ count+1 selector item tex
	if(arrange)
	{
		singleLength = (TexturePtr->TexWidth);
	}
	else
	{
		singleLength = (TexturePtr->TexHeight);
	}

	baseOffset = (s16)p_wptr->ListOffset;

	for (i = 0; i < valueCount && i < count; i++)
	{
		curValue = tagPtr->getArrayS32Value(i);
		curTexturePtr = TexturePtr + i;
		curTexturePtr->FocusedSlice = curValue;

		offset = baseOffset + i*(padding + singleLength);
		if (arrange)
		{
			if (offset > p_wptr->WidgetWidth || (offset + singleLength) < 0)
			{
				curTexturePtr->mTexAttr |= FORCE_UNDISPLAY;
			}
			else
			{
				curTexturePtr->OffsetX = offset<<4;
				curTexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
			}
		}
		else
		{
			if (offset > p_wptr->WidgetHeight || (offset + singleLength) < 0)
			{
				curTexturePtr->mTexAttr |= FORCE_UNDISPLAY;
			}
			else
			{
				curTexturePtr->OffsetY = offset << 4;
				curTexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
			}
		}

		
	}
	for (i = valueCount; i < count; i++)
	{
		curTexturePtr = TexturePtr + i;
		curTexturePtr->mTexAttr |= FORCE_UNDISPLAY;
	}


	//widget highlight
	curHighLight = (u8)(p_wptr->NumOfButtonAndCurHighLight);
	if(curHighLight == 1) //LIGHT ON
	{
		if (modifyEn)
		{
			pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 2 + count].mTexAttr |= (FORCE_UNDISPLAY); //widget highlight off

			//calc slice offset
			if (highlightElemID >= 0 && highlightElemID < count && highlightElemID < valueCount)
			{
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 2 + count + 1].mTexAttr &= ~(FORCE_UNDISPLAY); //slice highlight on
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 2 + count + 1].OffsetX = (TexturePtr + highlightElemID)->OffsetX;
				pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 2 + count + 1].OffsetY = (TexturePtr + highlightElemID)->OffsetY;
			}
			
		}
		else
		{
			pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 2 + count].mTexAttr &= ~(FORCE_UNDISPLAY); //widget highlight on
			pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 2 + count + 1].mTexAttr |= (FORCE_UNDISPLAY); //slice highlight off
		}
		
		
	}
	else if(curHighLight == 0)//LIGHT OFF
	{
		pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 2 + count].mTexAttr |= (FORCE_UNDISPLAY); //widget highlight off
		pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 2 + count + 1].mTexAttr |= (FORCE_UNDISPLAY); //slice highlight off
	}

	
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	return AHMI_FUNC_SUCCESS;
 }



//**************************************
//reponse for Tag.setValue

//**************************************
funcStatus ListClass::SetValueCtrl(
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
//Do not support touch propagation.
//**************************************
funcStatus ListClass::MouseTouchCtrl(
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
	
	s32 cur_x;
	s32 cur_y;
	
	u16 count;
	u16 showCount;
	u16 singleLength;//length of each selector item
	//TextureClassPtr curTexture;
//	s32 baseOffsetY;
	TextureClassPtr TexturePtr;

	s32 nextOffset;
	s32 dragOffsetLimit;
	u8 pressed;
	u16 padding;
	u8 arrange;
	u8 actionType;
	u16 valueCount;
	u8 flag = 0;
	u32 actionAddr;
	s32 idx;
	TagClassPtr selectedTag;
	u32 oldTick;
	s32 duration;
	s16 dragMovement;
	
//	int i;
//	CanvasClassPtr curCanvas = &pagePtr->pCanvasList[p_wptr->ATTATCH_CANVAS];
//	SubCanvasClassPtr curSubCan = &(pagePtr->pSubCanvasList[curCanvas->mStartAddrOfSubCanvas + curCanvas->mFocusedCanvas]);

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
		ERROR_PRINT("ERROR: when drawing List widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	actionType = ActionPtr->mInputType;

	//start address of texture
	TexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex+2]);

	#ifdef EMBEDDED
	if((TexturePtr < (void*)startOfDynamicPage) || (TexturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
#endif

	
	count = p_wptr->ListItemCount;
	showCount = p_wptr->ListShowNum;
	padding = p_wptr->ListPadding;
	arrange = p_wptr->ListArrange;
	valueCount = ActionPtr->mTagPtr->getCurArryElemNum();
	
	selectedTag = &TagPtr->getTag(p_wptr->ListSelectedTagID);

	if(arrange)
	{
		singleLength = (TexturePtr->TexWidth);
	}
	else
	{
		singleLength = (TexturePtr->TexHeight);
	}



	if(actionType == ACTION_MOUSE_HOLDING)
	{
		if(((pReleasePoint->x) <= 0) || ((pReleasePoint->x) >= (p_wptr->WidgetWidth)) ||
		   ((pReleasePoint->y) <= 0) || ((pReleasePoint->y) >= (p_wptr->WidgetHeight)))
		{
			actionType = ACTION_MOUSE_CANCLE;
		}
	}

	////check long press
	//if (actionType == ACTION_MOUSE_HOLDING)
	//{
	//	if(((p_wptr->ListStatus & 1) == 1 && (p_wptr->ListStatus & 2) != 2) && (p_wptr->PressFlag))
	//	{
	//		//pressed, but not dragging
	//		oldTick = (u32)((p_wptr->ListTickH  << 16) + p_wptr->ListTickL);
	//		    
	//		duration = AHMITick - oldTick;
	//		#ifdef PC_SIM
	//		if(duration >= 1000)
	//		#endif
	//		#if (defined VERSION_2) && (defined EMBEDDED)  
	//		if(duration >= 1000)
	//		#endif
	//		{
	//			actionType = ACTION_MOUSE_LONGPRESS;
	//		}
	//	}
	//}

	if( actionType == ACTION_MOUSE_PRESS )
	{
		p_wptr->PressFlag = 1;
		p_wptr->ListStatus |= 1;
		p_wptr->ListTickL = (u16)AHMITick;
		p_wptr->ListTickH = (u16)(AHMITick >> 16);
			       
		//ERROR_PRINT("PRESS");
		cur_x  = ((s16)(pPressPoint->x));
		cur_y  = ((s16)(pPressPoint->y));

		//offset between mouse press and center selector item
		if(arrange)
		{
			p_wptr->ListPressOffset = (u16)(cur_x);
		}
		else
		{
			p_wptr->ListPressOffset = (u16)(cur_y);
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
	else if (actionType == ACTION_MOUSE_HOLDING)
	{
		if(((p_wptr->ListStatus & 1) == 1) && (p_wptr->PressFlag))
		{
			//judge whether longpress
			if((p_wptr->ListStatus & 2) != 2)
			{
				oldTick = (u32)((p_wptr->ListTickH  << 16) + p_wptr->ListTickL);
			    
				duration = AHMITick - oldTick;
				#ifdef PC_SIM
				if(duration >= 10)
				#endif
				#if (defined VERSION_2) && (defined EMBEDDED)  
				if(duration >= 1000)
				#endif
				{
					p_wptr->ListTickL = (u16)AHMITick;
					p_wptr->ListTickH = (u16)(AHMITick >> 16);
					//send longpress action
					if(p_wptr->OnLongPressAction <= pagePtr->mActionInstructionsSize)
					{
						actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->OnLongPressAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}
				}
			}

			cur_x  = ((s16)(pReleasePoint->x)) ;
			cur_y  = ((s16)(pReleasePoint->y)) ;
			if(arrange)
			{
				dragOffsetLimit = p_wptr->WidgetWidth - (singleLength + padding) * count;
				dragMovement = (cur_x - (s16)(p_wptr->ListPressOffset));
				if ((p_wptr->ListStatus & 2) != 2)
				{
					if (dragMovement > -PressToDraggingThreshold && dragMovement < PressToDraggingThreshold)
					{
						//no drag
						return AHMI_FUNC_SUCCESS;
					}
					else
					{
						selectedTag->setRawValue(0xFFFFFFFF);
						p_wptr->ListStatus |= 2;
					}
				}
				nextOffset =  (s16)(p_wptr->ListOffset) + dragMovement;
				if (nextOffset >= dragOffsetLimit && nextOffset <= 0)
				{
					p_wptr->ListOffset = (u16)nextOffset;
				}
				else
				{
					if(nextOffset > 0)
						p_wptr->ListOffset = 0;
					else
						p_wptr->ListOffset = dragOffsetLimit;
				}
				p_wptr->ListPressOffset = (u16)cur_x;
			}
			else
			{
				dragOffsetLimit = p_wptr->WidgetHeight - (singleLength + padding) * count;
				dragMovement = (cur_y - (s16)(p_wptr->ListPressOffset));
				if ((p_wptr->ListStatus & 2) != 2)
				{
					if (dragMovement > -PressToDraggingThreshold && dragMovement < PressToDraggingThreshold)
					{
						//no drag
						return AHMI_FUNC_SUCCESS;
					}
					else
					{
						selectedTag->setRawValue(0xFFFFFFFF);
						p_wptr->ListStatus |= 2;
					}
				}
				nextOffset =  (s16)(p_wptr->ListOffset) + dragMovement;
				if (nextOffset >= dragOffsetLimit && nextOffset <= 0)
				{
					p_wptr->ListOffset = (u16)nextOffset;
				}
				else
				{
					if(nextOffset > 0)
						p_wptr->ListOffset = 0;
					else
						p_wptr->ListOffset = dragOffsetLimit;
				}
				p_wptr->ListPressOffset = (u16)cur_y;
			}
			renderTexture(pagePtr, p_wptr, ActionPtr->mTagPtr);
			force_cancel_subcanvas_page_mousetouch = 1;
			return AHMI_FUNC_SUCCESS;
		}
		else
		{
			return AHMI_FUNC_SUCCESS;
		}
		
	}
	//else if (actionType == ACTION_MOUSE_LONGPRESS)
	//{
	//	if(p_wptr->PressFlag)
	//	{
	//		cur_x  = ((s16)(pPressPoint->x) );
	//		cur_y  = ((s16)(pPressPoint->y));
	//		p_wptr->ListTickL = (u16)AHMITick;
	//		p_wptr->ListTickH = (u16)(AHMITick >> 16);
	//	
	//		if (arrange)
	//		{
	//			idx = (cur_x - (s16)p_wptr->ListOffset)  / (singleLength + padding);
	//			if ( idx >= 0 && (cur_x - (s16)p_wptr->ListOffset) <= idx * (singleLength + padding) + singleLength)
	//			{
	//				//hit
	//				selectedTag->setRawValue(idx);
	//				//send release action
	//				if(p_wptr->OnLongPressAction <= pagePtr->mActionInstructionsSize)
	//				{
	//					actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->OnLongPressAction);
	//					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
	//				}
	//			}
	//		}
	//		else
	//		{
	//			idx = (cur_y - (s16)p_wptr->ListOffset) / (singleLength + padding);
	//			if ( idx >= 0 && (cur_y - (s16)p_wptr->ListOffset) <= idx * (singleLength + padding) + singleLength)
	//			{
	//				//hit
	//				selectedTag->setRawValue(idx);
	//				//send release action
	//				if(p_wptr->OnLongPressAction <= pagePtr->mActionInstructionsSize)
	//				{
	//					actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->OnLongPressAction);
	//					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
	//				}
	//			}
	//		}
	//		
	//		if(p_wptr->TouchPropagation & TouchPropagationSet)
	//		{
	//			return WIDGET_TOUCH_CANCEL;
	//		}
	//		else
	//		{
	//			return AHMI_FUNC_SUCCESS;
	//		}
	//	}
	//}
	else if (actionType == ACTION_MOUSE_RELEASE || actionType == ACTION_MOUSE_CANCLE)
	{
		if(((p_wptr->ListStatus & 1) == 1) && (p_wptr->PressFlag))
		{
			if ((p_wptr->ListStatus & 2) == 2)
			{
				//dragging to release, do nothing
			}
			else
			{
				if(actionType == ACTION_MOUSE_RELEASE)
				{
					//send release action
					if(p_wptr->OnRealeaseAction <= pagePtr->mActionInstructionsSize)
					{
						actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->OnRealeaseAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}

					cur_x  = ((s16)(pReleasePoint->x));
					cur_y  = ((s16)(pReleasePoint->y));

					if (arrange)
					{
						idx = (cur_x - (s16)p_wptr->ListOffset)  / (singleLength + padding);
						if ( idx >= 0 && (cur_x - (s16)p_wptr->ListOffset) <= idx * (singleLength + padding) + singleLength)
						{
							//hit
							selectedTag->setRawValue(idx);
							////send release action
							//if(p_wptr->OnRealeaseAction <= pagePtr->mActionInstructionsSize)
							//{
							//	actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->OnRealeaseAction);
							//	csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
							//}
						}
					}
					else
					{
						idx = (cur_y - (s16)p_wptr->ListOffset) / (singleLength + padding);
						if ( idx >= 0 && (cur_y - (s16)p_wptr->ListOffset) <= idx * (singleLength + padding) + singleLength)
						{
							//hit
							selectedTag->setRawValue(idx);
							////send release action
							//if(p_wptr->OnRealeaseAction <= pagePtr->mActionInstructionsSize)
							//{
							//	actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->OnRealeaseAction);
							//	csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
							//}
						}
					}
				}
			}
			p_wptr->ListStatus  = 0;
			p_wptr->PressFlag = 0;
			
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

	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Keyboard.Touch
//**************************************
funcStatus ListClass::KeyboardTouchCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh      
	)
{
	u32 value;
	u32 divValue;
	u32 actionAddr;
	u16 code = 0;
	u8 displayMode;
	u8 modifyEn; //1 indicates is being modified
	u8 i;
	u16 highlightWidth, highlightHeight;
	u8 month, day;
	TextureClassPtr texturePtr;
	DynamicPageClassPtr pCurrentPage;
	u8 curHighLight;
	u16 highlightElemIdx;
	u16 count;
	u16 valueCount;
	u16 showCount;
	u16 padding;
	u8 arrange;
	s16 offset;
	s16 relativeOffset;
	u16 singleLength;
	TagClassPtr selectedTag;
	u32 oldTick;
	s32 duration;
	
	if(NULL == p_wptr || NULL == ActionPtr || NULL == ActionPtr->mTagPtr){
		ERROR_PRINT("ERROR: widgetPtr or ActionPtr ActionPtr->mTagPtris NULL"); 
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	
	if( NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: when drawing list widght, the texture list corrupt");
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
		ERROR_PRINT("ERROR: when drawing list widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	pCurrentPage = pagePtr;

	count = p_wptr->ListItemCount;
	
	valueCount = ActionPtr->mTagPtr->getCurArryElemNum();

	showCount = p_wptr->ListShowNum;
	padding = p_wptr->ListPadding;
	arrange = p_wptr->ListArrange;
	offset = p_wptr->ListOffset;

	if (count > valueCount)
	{
		count = valueCount;
	}
	
	modifyEn = p_wptr->ListHighlightOptions & 0x01;

	
	
	//check params
	if((NULL == p_wptr) || (NULL == ActionPtr))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);

	if(ActionPtr->mInputType == ACTION_KEYBOARD_OK_PRESS)
	{
		if ((p_wptr->ListStatus & 4) == 0)
		{
			//not pressed
			p_wptr->ListStatus |= 4;
			p_wptr->ListStatus &= ~8;
			oldTick = AHMITick;
			p_wptr->ListTickL = (u16)oldTick;
			p_wptr->ListTickH = (u16)(oldTick >> 16);
		}
		else if ((p_wptr->ListStatus & 8) == 0)
		{
			//pressed, not long press, check long press
			oldTick = (u32)((p_wptr->ListTickH  << 16) + p_wptr->ListTickL);
			    
			duration = AHMITick - oldTick;
			#ifdef PC_SIM
			if(duration >= 2000)
			#endif
			#if (defined VERSION_2) && (defined EMBEDDED)  
			if(duration >= 1000)
			#endif
			{
				//is long press
				p_wptr->ListStatus |= 8;
				
				//exit modify
				modifyEn = 0;
				p_wptr-> ListHighlightOptions &= ~(0x1);
				//get highlighted slice
				highlightElemIdx = p_wptr->ListHighlightElemID;
				//enter
				if (highlightElemIdx >= 0 && highlightElemIdx < count)
				{
					//hit
					selectedTag = &TagPtr->getTag(p_wptr->ListSelectedTagID);
					if (selectedTag != NULL)
					{
						selectedTag->setRawValue(highlightElemIdx);
						//send release action
						if(p_wptr->OnLongPressAction <= pagePtr->mActionInstructionsSize)
						{
							actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->OnLongPressAction);
							csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
						}
					}
				}
			}
		}
		else
		{
			return AHMI_FUNC_SUCCESS;
		}
		
	}
    else if(ActionPtr->mInputType == ACTION_KEYBOARD_OK)
	{
		if ((p_wptr->ListStatus & 8) == 8)
		{
			//long pressed, clear status
			p_wptr->ListStatus = 0;

		}
		else
		{
			p_wptr->ListStatus = 0;
			if(modifyEn == 0 && count > 0)//enter change mode
			{
				modifyEn = 1;
				p_wptr-> ListHighlightOptions |= 0x1;
				p_wptr->ListHighlightElemID = 0;
			}
			else if(modifyEn == 1) //send the changed value and change it
			{
				modifyEn = 0;
				p_wptr-> ListHighlightOptions &= ~(0x1);
				//get highlighted slice
				highlightElemIdx = p_wptr->ListHighlightElemID;
				//enter
				if (highlightElemIdx >= 0 && highlightElemIdx < count)
				{
					//hit
					selectedTag = &TagPtr->getTag(p_wptr->ListSelectedTagID);
					if (selectedTag != NULL)
					{
						selectedTag->setRawValue(highlightElemIdx);
						//send release action
						if(p_wptr->OnRealeaseAction <= pagePtr->mActionInstructionsSize)
						{
							actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->OnRealeaseAction);
							csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
						}
					}
				
				}
			}
		}
		
	}
	else if(ActionPtr->mInputType == ACTION_KEYBOARD_PRE || ActionPtr->mInputType == ACTION_KEYBOARD_NEXT) //left
	{
		if(modifyEn == 0) //unchanged mode
		{
			curHighLight = (u8)(p_wptr->NumOfButtonAndCurHighLight);
			if(curHighLight == 0) //LIGHT ON
			{
				curHighLight = 1;
				//pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 2 + count].mTexAttr &= ~(FORCE_UNDISPLAY); //highlight color on
				//pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 2 + count + 1].mTexAttr &= ~(FORCE_UNDISPLAY); //highlight color on
				p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighLight;
			}
			else if(curHighLight == 1)//LIGHT OFF
			{
				curHighLight = 0;
				//pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 2 + count].mTexAttr |= (FORCE_UNDISPLAY); //highlight color off
				//pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 2 + count + 1].mTexAttr |= (FORCE_UNDISPLAY); //highlight color off
				p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighLight;
			}
		}
		else if(modifyEn) //modify mode
		{
			if (count > 0)
			{
				highlightElemIdx = p_wptr->ListHighlightElemID;
				if (ActionPtr->mInputType == ACTION_KEYBOARD_NEXT)
				{
					if (highlightElemIdx < count - 1)
					{
						highlightElemIdx += 1;
					}
					else
					{
						highlightElemIdx = count - 1;
					}
				}
				else
				{
					if (highlightElemIdx > 0)
					{
						highlightElemIdx -= 1;
					}
					else
					{
						highlightElemIdx = 0;
					}
				}
				p_wptr->ListHighlightElemID = highlightElemIdx;
				
				//calc offset
				if(arrange)
				{
					singleLength =  pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex+2].TexWidth;
					relativeOffset = highlightElemIdx * (padding + singleLength);
					if (offset + (s16)relativeOffset < 0)
					{
						offset = -relativeOffset;
					}
					else if (offset + (s16)relativeOffset + (s16)singleLength > (s16)p_wptr->WidgetWidth)
					{
						offset =  (s16)p_wptr->WidgetWidth - (s16)singleLength - (s16)relativeOffset;
					}
				}
				else
				{
					singleLength =  pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex+2].TexHeight;
					relativeOffset = highlightElemIdx * (padding + singleLength);
					if (offset + (s16)relativeOffset < 0)
					{
						offset = -relativeOffset;
					}
					else if (offset + (s16)relativeOffset + (s16)singleLength > (s16)p_wptr->WidgetHeight)
					{
						offset =  (s16)p_wptr->WidgetHeight - (s16)singleLength - (s16)relativeOffset;
					}
				}
				p_wptr->ListOffset = offset;
			}
			
		}
	}
	
	else if(ActionPtr->mInputType == ACTION_KEYBOARD_CLEAR)
	{
		p_wptr-> ListHighlightOptions &= ~(0x1);
		curHighLight = 0;
		//pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 2 + count].mTexAttr |= (FORCE_UNDISPLAY); //highlight color off
		//pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 2 + count + 1].mTexAttr |= (FORCE_UNDISPLAY); //highlight color off
		p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighLight;
	}
    
    p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;

	return AHMI_FUNC_SUCCESS;
}


#endif

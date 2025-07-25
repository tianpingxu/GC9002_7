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
#include "DynamicTexClass.h"
#include "drawImmediately_cd.h"

#ifdef AHMI_CORE
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
//extern u16  WorkingPageID;

extern QueueHandle_t		ActionInstructionQueue;

//-----------------------------
// name�� DynamicTexClass
// initialize
//  
//
// addtional comments :
//   none
//-----------------------------
DynamicTexClass::DynamicTexClass()
{

}

//-----------------------------
// name�� ~DynamicTexClass
// deconstruct
//  
//
// addtional comments :
//   none
//-----------------------------
DynamicTexClass::~DynamicTexClass()
{

}

funcStatus DynamicTexClass::initWidget(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,   
	u32 *u32p_sourceShift,  
	DynamicPageClassPtr pageptr,
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
		if(widgetCtrl(pageptr,p_wptr,&tagtrigger,u8_pageRefresh) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
        
        p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
	}

	#if (defined PC_SIM) || (defined CHIP_GC9002_NoHyper)
	if(p_wptr->TextureSource)
	{
		#ifndef CHIP_GC9002_NoHyper
		if(p_wptr->TextureSource == 1)
		{
			AddTextureFromCopyPasteAction(u32p_sourceShift, p_wptr->BindTagID);
		}
		else if(p_wptr->TextureSource == 2)
		{
			AddTextureFromSnapshot(u32p_sourceShift, p_wptr);
		}
		#endif //endof #ifndef CHIP_GC9002_NoHyper
	}
	else
	{
		if(myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn, pagePtr) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);
	}
	
	return AHMI_FUNC_SUCCESS;
	#endif
	
	#if (defined VERSION_2) && (defined EMBEDDED)
	Op_sourceShift = *u32p_sourceShift;
	if(p_wptr->TextureSource)
	{
		#ifndef CHIP_GC9002_NoHyper
		if(p_wptr->TextureSource == 1)
		{
			AddTextureFromCopyPasteAction(u32p_sourceShift, p_wptr->BindTagID);
		}
		else if(p_wptr->TextureSource == 2)
		{
			AddTextureFromSnapshot(u32p_sourceShift, p_wptr);
		}
		#endif //endof #ifndef CHIP_GC9002_NoHyper
	}
	else
	{
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
//WidgetAttr��
//15-8 : reserved
//5-7  : the mode��0��switch between different texture 1��rotate
//4-0  : the type of the widget
//*****************************
funcStatus DynamicTexClass::widgetCtrl(
	DynamicPageClassPtr pageptr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{
	s32 value;
	s32 rotateInitValue;
	s32 value_temp = 0;
	TextureClassPtr texturePtr, preTexturePtr;
	DynamicPageClassPtr pCurrentPage;
	u8 movingDir = 0;
	u8 dynamicType = 0;
	u16 angle = 0;

	s32 sValue;
	s32	maxValue;
	s32	minValue;
	u32 curValue;
	s32 displayValue;


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
		ERROR_PRINT("ERROR: when drawing dynamicTex widget, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	dynamicType = (p_wptr->WidgetAttr & DYNAMIC_TYPE_BIT) >> 5;

	value = (s32)(ActionPtr->mTagPtr->getValue());

	curValue = (p_wptr->CurValueH << 16) + p_wptr->CurValueL;
	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);

	rotateInitValue = (s32)((p_wptr->RotateInitValueH << 16) + p_wptr->RotateInitValueL);

	pCurrentPage = pageptr;

	if(dynamicType == SWITCH_TEXTURE)  //switch between several images
	{
		if((u32)value > (u32)maxValue) 
		{
			value_temp = value;
			value = (u32)maxValue;
		}
		else if((u32)value < (u32)minValue)
			value = (u32)minValue;
		

		if(p_wptr->NumOfLine == 1)
		{
			texturePtr = &pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex];
			if((u32)value_temp > (u32)value)
				texturePtr->mTexAttr |= (FORCE_UNDISPLAY);
			else
				texturePtr->mTexAttr &= (~FORCE_UNDISPLAY);
			preTexturePtr = texturePtr;
		}
		else if(p_wptr->NumOfLine == 2)
		{
			if(u8_pageRefresh)
			{
				p_wptr->PreviousTexturePtrFlag = 0;
				texturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex]);
				preTexturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex+1]);
			}
			else if((p_wptr->PreviousTexturePtrFlag == 0) || (p_wptr->PreviousTexturePtrFlag == 0xCCCC))
			{
				p_wptr->PreviousTexturePtrFlag = 1;
				texturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 1]);
				preTexturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex]);
			}
			else if(p_wptr->PreviousTexturePtrFlag == 1)
			{
				p_wptr->PreviousTexturePtrFlag = 0;
				texturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex]);
				preTexturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 1]);
			}
		}
		else 
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);


		//moving direction
		if(p_wptr->NumOfLine != 1)
		{
			if((u32)value > curValue)
			{
				//up
				movingDir = 0;
			}
			else if((u32)value < curValue)
			{
				//down
				movingDir = 1;
			}
			else 
			{
				//not moving
				if(u8_pageRefresh == 0)
					return AHMI_FUNC_SUCCESS;
			}
		}

		//operate value
		texturePtr->FocusedSlice = (u16)value;
		if(u8_pageRefresh)
			preTexturePtr->FocusedSlice = (u16)value;

		//set the position
		if(p_wptr->NumOfLine == 2 && !u8_pageRefresh)
		{
			if(movingDir == 0) //up
			{
				//texturePtr->OffsetY = (p_wptr->WidgetOffsetY + p_wptr->WidgetHeight) << 4;
				//preTexturePtr->OffsetY =  (p_wptr->WidgetOffsetY) << 4;
				texturePtr->OffsetY = (p_wptr->WidgetHeight) << 4;
				preTexturePtr->OffsetY =  (0) << 4;
			}
			else if(movingDir == 1)//down
			{
				//texturePtr->OffsetY = (p_wptr->WidgetOffsetY - p_wptr->WidgetHeight) << 4;
				//preTexturePtr->OffsetY =  (p_wptr->WidgetOffsetY) << 4;
				texturePtr->OffsetY = (0 - p_wptr->WidgetHeight) << 4;
				preTexturePtr->OffsetY =  (0) << 4;
			}
		}
	}
	else if(dynamicType == CENTRAL_ROTATE) //rotation
	{
		texturePtr = &pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex];
		maxValue = maxValue - minValue;
		minValue = 0;
		sValue = (s32)value;
		value %= maxValue;
		if(sValue != 0 && value == 0 ){
			value = maxValue;
		}

		if (value > maxValue) {
            value = maxValue;
        } else if (value < minValue) {
            value = minValue;
        }
		displayValue = value + rotateInitValue;

		//if(sValue < 0)
		//	value = -sValue;
		////value to compute angle
		//if(value > 360)
		//	value %= 360;

		//if(value > maxValue) 
		//	value = maxValue;
		//if(value < minValue)
		//	value = minValue;

		angle = displayValue;

		texturePtr[0].RotateAngle = angle * 16;
		texturePtr[0].mTexAttr |= TEXTURE_CENTRAL_ROTATE;
		//texturePtr[0].mTexAttr |= TEXTURE_USING_WIDGET_BOX;
		texturePtr[0].mTexAttr |= USING_PIXEL_RATIO;  //needs to be fixed for ellipse, by zuz 20180802
	}
	else if(dynamicType == TRANSLATION) //one texture translation
	{

	}
	else if(dynamicType == DIM) //dim, not used now
	{
		if(value > maxValue) 
			value = maxValue;
		if(value < minValue)
			value = minValue;
		//tag to find the focus slice
		texturePtr = &pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex];
		texturePtr->FocusedSlice = value;

	}
	else
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
    
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
funcStatus DynamicTexClass::SetValueCtrl(
	DynamicPageClassPtr pageptr,
	WidgetClassPtr p_wptr,  
	ActionTriggerClassPtr ActionPtr,
	u8 u8_widgetRefresh      
	)
{
	DynamicPageClassPtr pCurrentPage;
    s32 value;
	u8 dynamicType = 0;
	u32 actionAddr;
	s32 sValue;
	s32	maxValue;
	s32	minValue;
	//s32	rotateInitValue;
	s32 lowAlarmValue;
	s32 highAlarmValue;
	u32 oldValue = 0;
	s32 sOldValue = 0;
	u16 oldValueinit = 0;

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
		ERROR_PRINT("ERROR: when drawing dynamicTex widget, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	dynamicType = (p_wptr->WidgetAttr & DYNAMIC_TYPE_BIT) >> 5;

	value = (s32)(ActionPtr->mTagPtr->getValue());

	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);
	//rotateInitValue = (s32)((p_wptr->RotateInitValueH << 16) + p_wptr->RotateInitValueL);
	oldValueinit = (p_wptr->WidgetAttr & 0x8000);
	
	if(dynamicType == CENTRAL_ROTATE){
		maxValue = maxValue - minValue;
		minValue = 0;
	}
	if((value < minValue) || (value > maxValue)){
		ERROR_PRINT("ERROR: DynamicTex widget bindTag value exceeds the boundary");
		u16 CanvasID, SubcanvasID, WidgetID;
		p_wptr->getWidgetIndex(pageptr, &CanvasID, &SubcanvasID, &WidgetID);
		EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
	}

	lowAlarmValue  = (s32)((p_wptr->LowAlarmValueH  << 16) + p_wptr->LowAlarmValueL);
	highAlarmValue = (s32)((p_wptr->HighAlarmValueH << 16) + p_wptr->HighAlarmValueL);
       
	sValue = (s32)value;

	pCurrentPage = pageptr;

	if(u8_widgetRefresh == 0)
	{
		//refresh this widget

		if(oldValueinit)
		{
			oldValue = (p_wptr->OldValueH  << 16) + p_wptr->OldValueL;
               
			if(dynamicType == SWITCH_TEXTURE)
			{
				if(p_wptr->EnterLowAlarmAction && oldValue > (u32)lowAlarmValue && (u32)value <= (u32)lowAlarmValue)
				{
					//enter low
					if(p_wptr->EnterLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterLowAlarmAction);
                        csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}		
				}
				else if(p_wptr->LeaveLowAlarmAction && oldValue <= (u32)lowAlarmValue && (u32)value > (u32)lowAlarmValue)
				{
					//leave low
					if(p_wptr->LeaveLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->LeaveLowAlarmAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}		
				}
				if(p_wptr->EnterHighAlarmAction && oldValue < (u32)highAlarmValue && (u32)value >= (u32)highAlarmValue)
				{
					//enter high
					if(p_wptr->EnterHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterHighAlarmAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}
				}
				else if(p_wptr->LeaveHighAlarmAction && oldValue >= (u32)highAlarmValue && (u32)value < (u32)highAlarmValue)
				{
					//leave high
					if(p_wptr->LeaveHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->LeaveHighAlarmAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}	
				}
			}
               
			else if(dynamicType == CENTRAL_ROTATE)
			{
				maxValue = maxValue - minValue;
				minValue = 0;
				sValue = (s32)value;
				value %= maxValue;

				if(sValue != 0 && value == 0 ){
					value = maxValue;
				}


				if (value > maxValue) {
					value = maxValue;
				} else if (value < minValue) {
					value = minValue;
				}
				//value += rotateInitValue;
				
				sOldValue = (u32)oldValue;
                
				if(p_wptr->EnterLowAlarmAction && sOldValue > (s32)lowAlarmValue && (s32)value <= (s32)lowAlarmValue)
				{
					//enter low
					if(p_wptr->EnterLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterLowAlarmAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}		
				}
				else if(p_wptr->LeaveLowAlarmAction && sOldValue <= (s32)lowAlarmValue && (s32)value > (s32)lowAlarmValue)
				{
					//leave low
					if(p_wptr->LeaveLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->LeaveLowAlarmAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}		
				}
				if(p_wptr->EnterHighAlarmAction && sOldValue < (s32)highAlarmValue && (s32)value >= (s32)highAlarmValue)
				{
					//enter high
					if(p_wptr->EnterHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterHighAlarmAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}
				}
				else if(p_wptr->LeaveHighAlarmAction && sOldValue >= (s32)highAlarmValue && (s32)value < (s32)highAlarmValue)
				{
					//leave high
					if(p_wptr->LeaveHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->LeaveHighAlarmAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}	
				}
			}
		}
        else
        {
            //if(p_wptr->EnterLowAlarmAction && value < lowAlarmValue)
            //{
            //    //enter low
            //    if(p_wptr->EnterLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
            //    {
            //        actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterLowAlarmAction);
            //        xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
            //    }		
            //}
            //
            //if(p_wptr->EnterHighAlarmAction && value > highAlarmValue)
            //{
            //    //enter high
            //    if(p_wptr->EnterHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
            //    {
            //        actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterHighAlarmAction);
            //        xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);		
            //    }
            //}
        }
           
		p_wptr->WidgetAttr |= 0x8000;
		p_wptr->OldValueL = (u16)value;
		p_wptr->OldValueH = (u16)(value >> 16); 
	}
    
    if(oldValueinit)
	{
		if(oldValue != (u32)value)
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
funcStatus DynamicTexClass::MouseTouchCtrl(
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
funcStatus DynamicTexClass::KeyboardTouchCtrl(
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

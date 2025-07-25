////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     CanvasClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 1.00 - File Created 2016/04/13 by YuChunYing
// Revision 1.5.1 - File Modified 2018/11/19 by Yu Chunying 
// Additional Comments:
//   add comments by zuz 2018/10/29
// 
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "SliderClass.h"

#ifdef AHMI_CORE
extern ActionTriggerClass		gWidgetTagTrigger;
extern ActionTriggerClass		gMouseTagTrigger;
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
//extern u16  WorkingPageID;

extern QueueHandle_t		ActionInstructionQueue;

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;

extern u8 force_cancel_subcanvas_page_mousetouch;

//#if defined(EMBEDDED) && defined(VERSION_2)
//extern void EnterSafeSectionForTag(void);
//extern void ExitSafeSectionForTag(void);
//#endif


//-----------------------------
// name£º MeterClass
// construct
//  
//
// addtional comments :
//   none
//-----------------------------
SliderClass::SliderClass()
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
SliderClass::~SliderClass()
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
funcStatus SliderClass::initWidget(
	DynamicPageClassPtr pageptr,
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
		if(widgetCtrl(pageptr,p_wptr,&tagtrigger,u8_pageRefresh) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
        
        p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
	}

	#if (defined PC_SIM) || (defined CHIP_GC9002_NoHyper)
	if(myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn,pageptr) != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);
		
	return AHMI_FUNC_SUCCESS;
	#endif
	
	#if (defined VERSION_2) && (defined EMBEDDED)
	Op_sourceShift = *u32p_sourceShift;
	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetSourceBufferRefreshFlag) || (RefreshType == ANIMATION_MATRIXREFRESH_PAGE) || (gScreenOptions.enable))
	{
		status = myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn, pageptr);
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
// reponse for Tag.setValue
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the tag
// addtional comments :
//   none
//-----------------------------
funcStatus SliderClass::widgetCtrl(
	DynamicPageClassPtr pageptr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{	
	s32 value;
	s32	maxValue;
	s32	minValue;
	u16 HalfWidth1;
	u16 Width0;
	u16 type;
	u8 slideBlockModeId;
	TextureClassPtr texturePtr;
	CanvasClassPtr curCanvas = &pageptr->pCanvasList[p_wptr->ATTATCH_CANVAS];
	SubCanvasClassPtr curSubCan;

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
		ERROR_PRINT("ERROR: when drawing slider widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}
	
	if(p_wptr->WidgetAttr & 0x40) // for updata slider offset by subcanvas contentoffset
	{
		curSubCan = &(pageptr->pSubCanvasList[curCanvas->mStartAddrOfSubCanvas + curCanvas->mFocusedCanvas]);

		//start address of texture
		texturePtr = &(pageptr->pBasicTextureList[p_wptr->StartNumofTex]);
		type  = (p_wptr->WidgetAttr & 0x20)  ; //type, 0 herizontal, 1 vertical

		if(type==0)
		{
			HalfWidth1 = ((texturePtr[1].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)/2) <<4;
			Width0 = (texturePtr[0].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512) << 4;

			if(curCanvas->mwidth != curSubCan->contentWidth)
			{
				texturePtr[1].OffsetX = curSubCan->contentOffsetX * (Width0 - HalfWidth1*2) / (curCanvas->mwidth - curSubCan->contentWidth);
			}
		}
		else
		{
			HalfWidth1 = ((texturePtr[1].TexHeight * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)/2) <<4;
			Width0 = (texturePtr[0].TexHeight * p_wptr->ADAPTIVE_SCALE_LEVEL / 512) << 4;

			if(curCanvas->mheight != curSubCan->contentHeight)
			{
				texturePtr[1].OffsetY = curSubCan->contentOffsetY * (Width0 - HalfWidth1*2) / (curCanvas->mheight - curSubCan->contentHeight);
			}
		}
		return AHMI_FUNC_SUCCESS;
	}
	
	//start address of texture
	texturePtr = &(pageptr->pBasicTextureList[p_wptr->StartNumofTex]);

	//params
	type  = (p_wptr->WidgetAttr & 0x20)  ; //type, 0 herizontal, 1 vertical
	value = (s32)(ActionPtr->mTagPtr->getValue());

	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);

	if(maxValue == minValue){
		ERROR_PRINT("ERROR: can't be divided by zero");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
	}

#ifdef EMBEDDED
	if((texturePtr < (void*)startOfDynamicPage) || (texturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
#endif
	//mode
	if (p_wptr->NumOfTex==5 || p_wptr->NumOfTex==4)
	{
		slideBlockModeId = 1;
	}else{
		slideBlockModeId = 0;
	}

    if(ActionPtr->mInputType == ACTION_TAG_SET_VALUE)
	{
		if(value > maxValue)
			value = maxValue;
		else if(value < minValue)
			value = minValue;

		

		if(type==0)///herizontal
		{
           
			if (slideBlockModeId == 0)
			{
				HalfWidth1 = ((texturePtr[1].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)/2) <<4;
				Width0 = (texturePtr[0].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512) << 4;
				texturePtr[1].OffsetX = texturePtr[0].OffsetX + (value - minValue) * ((float)(Width0 - HalfWidth1 *2)  / (float)(maxValue - minValue));
			}else{
				HalfWidth1 = ((texturePtr[4].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)/2) <<4;
				Width0 = (texturePtr[0].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512) << 4;
				//0:bg tex 1:mask for clear alpha buffer 2:mask for slide progress 3:slide progress tex 4:bar
				texturePtr[4].OffsetX = texturePtr[0].OffsetX + (value - minValue) * ((float)(Width0 - HalfWidth1 *2)  / (float)(maxValue - minValue));
				//slide progress mask
				
				texturePtr[2].TexWidth = (texturePtr[4].OffsetX -  texturePtr[0].OffsetX + HalfWidth1) >> 4;
			}
			
			

		}
		else  //vertical
		{
            
			if (slideBlockModeId == 0)
			{
				HalfWidth1 = (texturePtr[1].TexHeight/2) <<4;
				Width0 = texturePtr[0].TexHeight << 4;
				texturePtr[1].OffsetY = texturePtr[0].OffsetY + ( Width0 - HalfWidth1*2) - (value - minValue) * ((float)(Width0 - HalfWidth1 *2) / (float)(maxValue - minValue));
			}else{
				HalfWidth1 = (texturePtr[4].TexHeight/2) <<4;
				Width0 = texturePtr[0].TexHeight << 4;
				//0:bg tex 1:mask for clear alpha buffer 2:mask for slide progress 3:slide progress tex 4:bar
				texturePtr[4].OffsetY = texturePtr[0].OffsetY + ( Width0 - HalfWidth1*2) - (value - minValue) * ((float)(Width0 - HalfWidth1 *2) / (float)(maxValue - minValue));
				//slide progress mask
				texturePtr[2].OffsetY = texturePtr[4].OffsetY + HalfWidth1;
				texturePtr[2].TexHeight = ( Width0 - (texturePtr[4].OffsetY - texturePtr[0].OffsetY) - HalfWidth1) >> 4;
			}
		}
	}

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// reponse for Tag.setValue
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the tag
// addtional comments :
//   none
//-----------------------------
funcStatus SliderClass::SetValueCtrl(
	DynamicPageClassPtr pageptr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_widgetRefresh				
	)
{	
	DynamicPageClassPtr pCurrentPage;
	u32 actionAddr;
	s32 oldValue=0;
	s32 value;
	s32	maxValue;
	s32	minValue;
	s32 lowAlarmValue ;
	s32 highAlarmValue;
	u16 oldValueinit=0;

	//param check
	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}


	//params
	oldValueinit = (p_wptr->WidgetAttr & 0x8000);
	value = (s32)(ActionPtr->mTagPtr->getValue());

	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);
	lowAlarmValue  = (s32)((p_wptr->LowAlarmValueH  << 16) + p_wptr->LowAlarmValueL );
	highAlarmValue = (s32)((p_wptr->HighAlarmValueH << 16) + p_wptr->HighAlarmValueL);

	if(maxValue == minValue){
		ERROR_PRINT("ERROR: can't be divided by zero");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_SetValueErr);
	}

	if((value < minValue) || (value > maxValue)){
		ERROR_PRINT("ERROR: Slider widget bindTag value exceeds the boundary");
		u16 CanvasID, SubcanvasID, WidgetID;
		p_wptr->getWidgetIndex(pageptr, &CanvasID, &SubcanvasID, &WidgetID);
		EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
	}

    if(ActionPtr->mInputType == ACTION_TAG_SET_VALUE)
	{
		if(value > maxValue)
			value = maxValue;
		else if(value < minValue)
			value = minValue;
	}

	pCurrentPage = pageptr;
	if(u8_widgetRefresh == 0 && ActionPtr->mInputType == ACTION_TAG_SET_VALUE)
	{

		if(oldValueinit)
		{
			oldValue = (s32)((p_wptr->OldValueH  << 16) + p_wptr->OldValueL);
			//tagchange  by Mr.z
			if(oldValue != value){
				if(p_wptr->TagChangeAction && p_wptr->TagChangeAction<= pCurrentPage->mActionInstructionsSize)
				{
					actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->TagChangeAction);
					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
				}
			}
			if(p_wptr->EnterLowAlarmAction && oldValue > lowAlarmValue && value <= lowAlarmValue)
			{
				//enter low
				if(p_wptr->EnterLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
				{
					actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterLowAlarmAction);
					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
				}		
			}
			else if(p_wptr->LeaveLowAlarmAction && oldValue <= lowAlarmValue && value > lowAlarmValue)
			{
				//leave low
				if(p_wptr->LeaveLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
				{
					actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->LeaveLowAlarmAction);
					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
				}		
			}
			if(p_wptr->EnterHighAlarmAction && oldValue < highAlarmValue && value >= highAlarmValue)
			{
				//enter high
				if(p_wptr->EnterHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
				{
					actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterHighAlarmAction);
					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
				}
			}
			else if(p_wptr->LeaveHighAlarmAction && oldValue >= highAlarmValue && value < highAlarmValue)
			{
				//leave high
				if(p_wptr->LeaveHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
				{
					actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->LeaveHighAlarmAction);
					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
				}	
			}
		}
        else
        {
   //         if(p_wptr->TagChangeAction && (p_wptr->TagChangeAction <= pCurrentPage->mActionInstructionsSize))
			//{
			//	actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->TagChangeAction);
			//	xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
			//}
            
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
//Do not support touch propagation.
//@param WidgetClassPtr p_wptr Pointer of widget
//@param ActionPtr             Pointer of trigger
//@param u8_pageRefresh        whether refresh the page. if refresh, the value of the widget also refresh.
//**************************************
funcStatus SliderClass::MouseTouchCtrl(
	DynamicPageClassPtr pageptr,
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
	s32 cur_x;
	s32 cur_y;
	s32	maxValue;
	s32	minValue;
	u16 HalfWidth1;
	u16 Width0;
	u16 type;
	u8 scrolltype = 0;
	u8 slideBlockModeId;
	u8 actionType;
	TextureClassPtr texturePtr;
	CanvasClassPtr curCanvas = &pageptr->pCanvasList[p_wptr->ATTATCH_CANVAS];
	SubCanvasClassPtr curSubCan = &(pageptr->pSubCanvasList[curCanvas->mStartAddrOfSubCanvas + curCanvas->mFocusedCanvas]);
	u32 actionAddr;

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
		ERROR_PRINT("ERROR: when drawing slider widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	actionType = ActionPtr->mInputType;

	//start address of texture
	texturePtr = &(pageptr->pBasicTextureList[p_wptr->StartNumofTex]);

	//params
	type  = (p_wptr->WidgetAttr & 0x20)  ; //type, 0 herizontal, 1 vertical
	scrolltype  = (p_wptr->WidgetAttr & 0x40)  ; //scrolltype, 0 normall, 1 scroll

	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);
	//mode
	if (p_wptr->NumOfTex==5 || p_wptr->NumOfTex==4)
	{
		slideBlockModeId = 1;
	}else{
		slideBlockModeId = 0;
	}

	if(maxValue == minValue){
		ERROR_PRINT("ERROR: can't be divided by zero");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetActionTriggerErr);
	}

#ifdef EMBEDDED
	if((texturePtr < (void*)startOfDynamicPage) || (texturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
#endif

//	if(scrolltype) //scroll slider don't change into ACTION_MOUSE_CANCLE
//	{
//
//	}
//	else //normal slider judge whether change into ACTION_MOUSE_CANCLE
//	{
//		if(actionType == ACTION_MOUSE_HOLDING)
//		{
//			if(((pReleasePoint->x) <= 0) || ((pReleasePoint->x) >= (p_wptr->WidgetWidth)) ||
//			   ((pReleasePoint->y) <= 0) || ((pReleasePoint->y) >= (p_wptr->WidgetHeight)))
//			{
//				actionType = ACTION_MOUSE_CANCLE;
//			}
//		}
//	}

	if( actionType == ACTION_MOUSE_PRESS )
	{
		p_wptr->PressFlag = 1;
		cur_x  = ((s16)(pPressPoint->x) << 4);
		cur_y  = ((s16)(pPressPoint->y) << 4);
		if(type==0)//herizontal
		{
			if (slideBlockModeId == 0)
			{
				HalfWidth1 = ((texturePtr[1].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)/2) <<4;
				Width0 = (texturePtr[0].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512) << 4;
	
				if((cur_x - texturePtr[0].OffsetX) > HalfWidth1)
					{
					if(( texturePtr[0].OffsetX + Width0 - cur_x) < HalfWidth1)
						texturePtr[1].OffsetX =   texturePtr[0].OffsetX + Width0 - HalfWidth1*2;		/////////right side limit
					else 
						texturePtr[1].OffsetX =   cur_x - HalfWidth1;
					}
				else 	/////////left side limit
					texturePtr[1].OffsetX =   texturePtr[0].OffsetX;

				tag_value = (float)(texturePtr[1].OffsetX - texturePtr[0].OffsetX) / (float)( Width0 - HalfWidth1*2) * (maxValue - minValue) + minValue ; //caculate value
			}else {//0:bg tex 1:mask for clear alpha buffer 2:mask for slide progress 3:slide progress tex 4:bar
				HalfWidth1 = ((texturePtr[4].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)/2) <<4;
				Width0 = (texturePtr[0].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512) << 4;
	
				if((cur_x - texturePtr[0].OffsetX) > HalfWidth1)
					{
					if(( texturePtr[0].OffsetX + Width0 - cur_x) < HalfWidth1)
						texturePtr[4].OffsetX =   texturePtr[0].OffsetX + Width0 - HalfWidth1*2;		/////////right side limit
					else 
						texturePtr[4].OffsetX =   cur_x - HalfWidth1;
					}
				else 	/////////left side limit
					texturePtr[4].OffsetX =   texturePtr[0].OffsetX;

				texturePtr[2].TexWidth = (texturePtr[4].OffsetX -  texturePtr[0].OffsetX + HalfWidth1) >> 4;

				tag_value = (float)(texturePtr[4].OffsetX - texturePtr[0].OffsetX) / (float)( Width0 - HalfWidth1*2) * (maxValue - minValue) + minValue ; //caculate value
			}
			
		}
		else  //vertical
		{
			if (slideBlockModeId == 0)
			{
				HalfWidth1 = (texturePtr[1].TexHeight/2) <<4;
				Width0 = texturePtr[0].TexHeight << 4;
	
				if((cur_y - texturePtr[0].OffsetY) > HalfWidth1)
				{
					if(( texturePtr[0].OffsetY + Width0 - cur_y) < HalfWidth1)
						texturePtr[1].OffsetY =   texturePtr[0].OffsetY + Width0 - HalfWidth1*2;		/////////down side limit
					else 
						texturePtr[1].OffsetY =   cur_y - HalfWidth1;
					}
				else 	/////////up side limit
					texturePtr[1].OffsetY =   texturePtr[0].OffsetY;

				//tag_value = (float)(texturePtr[1].OffsetY - texturePtr[0].OffsetY) / (float)( Width0 - HalfWidth1*2) * (maxValue - minValue) + minValue ; //caculate value
				tag_value = (float)(Width0 - HalfWidth1*2 - texturePtr[1].OffsetY + texturePtr[0].OffsetY) / (float)( Width0 - HalfWidth1*2) * (maxValue - minValue) + minValue ; //caculate value
			}else {//0:bg tex 1:mask for clear alpha buffer 2:mask for slide progress 3:slide progress tex 4:bar
				HalfWidth1 = (texturePtr[4].TexHeight/2) <<4;
				Width0 = texturePtr[0].TexHeight << 4;
	
				if((cur_y - texturePtr[0].OffsetY) > HalfWidth1)
				{
					if(( texturePtr[0].OffsetY + Width0 - cur_y) < HalfWidth1)
						texturePtr[4].OffsetY =   texturePtr[0].OffsetY + Width0 - HalfWidth1*2;		/////////down side limit
					else 
						texturePtr[4].OffsetY =   cur_y - HalfWidth1;
					}
				else 	/////////up side limit
					texturePtr[4].OffsetY =   texturePtr[0].OffsetY;

				texturePtr[2].OffsetY = texturePtr[4].OffsetY + HalfWidth1;
				texturePtr[2].TexHeight = ( Width0 - (texturePtr[4].OffsetY - texturePtr[0].OffsetY) - HalfWidth1) >> 4;

				tag_value = (float)(Width0 - HalfWidth1*2 - texturePtr[4].OffsetY + texturePtr[0].OffsetY) / (float)( Width0 - HalfWidth1*2) * (maxValue - minValue) + minValue ; //caculate value
				//tag_value = (float)(texturePtr[4].OffsetY - texturePtr[0].OffsetY) / (float)( Width0 - HalfWidth1*2) * (maxValue - minValue) + minValue ; //caculate value
			}
		}

		if(scrolltype)
		{
			curSubCan = &(pageptr->pSubCanvasList[curCanvas->mStartAddrOfSubCanvas + curCanvas->mFocusedCanvas]);

			if((curSubCan->contentWidth != curCanvas->mwidth) || (curSubCan->contentHeight != curCanvas->mheight))
			{
				if(type==0)
				{
					curSubCan->contentOffsetX = tag_value * (curCanvas->mwidth - curSubCan->contentWidth) / (maxValue - minValue);

					//#if defined(EMBEDDED) && defined(VERSION_2)
					//EnterSafeSectionForTag();
					//#endif
					curSubCan->CalculateMatrix(&(curSubCan->mSubCanvasMatrix));
					//#if defined(EMBEDDED) && defined(VERSION_2)
					//ExitSafeSectionForTag();
					//#endif
				}
				else
				{
					curSubCan->contentOffsetY = (maxValue - tag_value + minValue) * (curCanvas->mheight - curSubCan->contentHeight) / (maxValue - minValue);

					//#if defined(EMBEDDED) && defined(VERSION_2)
					//EnterSafeSectionForTag();
					//#endif
					curSubCan->CalculateMatrix(&(curSubCan->mSubCanvasMatrix));
					//#if defined(EMBEDDED) && defined(VERSION_2)
					//ExitSafeSectionForTag();
					//#endif
				}
			}
		}
		else
		{
			if(p_wptr->BindTagID != 0)
			{
				TagPtr->getTag(p_wptr->BindTagID).setRawValue(tag_value);
			}
		}

		if(p_wptr->BindTagID != 0)
		{
			p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
		}
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

	else if(actionType == ACTION_MOUSE_HOLDING)
	{
		if(p_wptr->PressFlag == 1)
		{
			cur_x  = ((s16)(pReleasePoint->x)) << 4;
			cur_y  = ((s16)(pReleasePoint->y)) << 4;
			if(type==0)///herizontal
			{
				if (slideBlockModeId == 0)
				{
					HalfWidth1 = ((texturePtr[1].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)/2) <<4;
					Width0 = (texturePtr[0].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512) << 4;
	
					if((cur_x - texturePtr[0].OffsetX) > HalfWidth1)
						{
						if(( texturePtr[0].OffsetX + Width0 - cur_x) < HalfWidth1)
							texturePtr[1].OffsetX =   texturePtr[0].OffsetX + Width0 - HalfWidth1*2;		/////////right side limit
						else 
							texturePtr[1].OffsetX =   cur_x - HalfWidth1;
						}
					else 	/////////left side limit
						texturePtr[1].OffsetX =   texturePtr[0].OffsetX;

					tag_value = (float)(texturePtr[1].OffsetX - texturePtr[0].OffsetX) / (float)( Width0 - HalfWidth1*2) * (maxValue-minValue) + minValue ; //caculate value
				}else {//0:bg tex 1:mask for clear alpha buffer 2:mask for slide progress 3:slide progress tex 4:bar
					HalfWidth1 = ((texturePtr[4].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)/2) <<4;
					Width0 = (texturePtr[0].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512) << 4;
	
					if((cur_x - texturePtr[0].OffsetX) > HalfWidth1)
						{
						if(( texturePtr[0].OffsetX + Width0 - cur_x) < HalfWidth1)
							texturePtr[4].OffsetX =   texturePtr[0].OffsetX + Width0 - HalfWidth1*2;		/////////right side limit
						else 
							texturePtr[4].OffsetX =   cur_x - HalfWidth1;
						}
					else 	/////////left side limit
						texturePtr[4].OffsetX =   texturePtr[0].OffsetX;

					texturePtr[2].TexWidth = (texturePtr[4].OffsetX -  texturePtr[0].OffsetX + HalfWidth1) >> 4;

					tag_value = (float)(texturePtr[4].OffsetX - texturePtr[0].OffsetX) / (float)( Width0 - HalfWidth1*2) * (maxValue-minValue) + minValue ; //caculate value
				}
			}
			else  //vertical
			{
				if (slideBlockModeId == 0)
				{
					HalfWidth1 = (texturePtr[1].TexHeight/2) <<4;
					Width0 = texturePtr[0].TexHeight << 4;
	
					if((cur_y - texturePtr[0].OffsetY) > HalfWidth1)
					{
						if(( texturePtr[0].OffsetY + Width0 - cur_y) < HalfWidth1)
							texturePtr[1].OffsetY =   texturePtr[0].OffsetY + Width0 - HalfWidth1*2;		/////////down side limit
						else 
							texturePtr[1].OffsetY =   cur_y - HalfWidth1;
						}
					else 	/////////up side limit
						texturePtr[1].OffsetY =   texturePtr[0].OffsetY;

					tag_value = (float)(Width0 - HalfWidth1*2 - texturePtr[1].OffsetY + texturePtr[0].OffsetY) / (float)( Width0 - HalfWidth1*2) * (maxValue - minValue) + minValue ; //caculate value
					//tag_value = (float)(texturePtr[1].OffsetY - texturePtr[0].OffsetY) / (float)( Width0 - HalfWidth1*2) * (maxValue - minValue) + minValue ; //caculate value
				}else {//0:bg tex 1:mask for clear alpha buffer 2:mask for slide progress 3:slide progress tex 4:bar
					HalfWidth1 = (texturePtr[4].TexHeight/2) <<4;
					Width0 = texturePtr[0].TexHeight << 4;
	
					if((cur_y - texturePtr[0].OffsetY) > HalfWidth1)
					{
						if(( texturePtr[0].OffsetY + Width0 - cur_y) < HalfWidth1)
							texturePtr[4].OffsetY =   texturePtr[0].OffsetY + Width0 - HalfWidth1*2;		/////////down side limit
						else 
							texturePtr[4].OffsetY =   cur_y - HalfWidth1;
						}
					else 	/////////up side limit
						texturePtr[4].OffsetY =   texturePtr[0].OffsetY;
					
					texturePtr[2].OffsetY = texturePtr[4].OffsetY + HalfWidth1;
					texturePtr[2].TexHeight = ( Width0 - (texturePtr[4].OffsetY - texturePtr[0].OffsetY) - HalfWidth1) >> 4;

					tag_value = (float)(Width0 - HalfWidth1*2 - texturePtr[4].OffsetY + texturePtr[0].OffsetY) / (float)( Width0 - HalfWidth1*2) * (maxValue - minValue) + minValue ; //caculate value
					//tag_value = (float)(texturePtr[4].OffsetY - texturePtr[0].OffsetY) / (float)( Width0 - HalfWidth1*2) * (maxValue - minValue) + minValue ; //caculate value
				}
			}

			if(scrolltype)
			{
				curSubCan = &(pageptr->pSubCanvasList[curCanvas->mStartAddrOfSubCanvas + curCanvas->mFocusedCanvas]);

				if((curSubCan->contentWidth != curCanvas->mwidth) || (curSubCan->contentHeight != curCanvas->mheight))
				{
					if(type==0)
					{
						curSubCan->contentOffsetX = tag_value * (curCanvas->mwidth - curSubCan->contentWidth) / (maxValue - minValue);

						//#if defined(EMBEDDED) && defined(VERSION_2)
						//EnterSafeSectionForTag();
						//#endif
						curSubCan->CalculateMatrix(&(curSubCan->mSubCanvasMatrix));
						//#if defined(EMBEDDED) && defined(VERSION_2)
						//ExitSafeSectionForTag();
						//#endif
					}
					else
					{
						curSubCan->contentOffsetY = (maxValue - tag_value + minValue) * (curCanvas->mheight - curSubCan->contentHeight) / (maxValue - minValue);

						//#if defined(EMBEDDED) && defined(VERSION_2)
						//EnterSafeSectionForTag();
						//#endif
						curSubCan->CalculateMatrix(&(curSubCan->mSubCanvasMatrix));
						//#if defined(EMBEDDED) && defined(VERSION_2)
						//ExitSafeSectionForTag();
						//#endif
					}
				}
			}
			else
			{
				if(p_wptr->BindTagID != 0)
				{
					TagPtr->getTag(p_wptr->BindTagID).setRawValue(tag_value);
				}
			}

			if(p_wptr->BindTagID != 0)
			{
				p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
			}
			p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;

			force_cancel_subcanvas_page_mousetouch = 1;
		}
		return AHMI_FUNC_SUCCESS;

	}else if(actionType == ACTION_MOUSE_RELEASE || actionType == ACTION_MOUSE_CANCLE) //mouse up
	{
		if(p_wptr->PressFlag == 1)
		{
			p_wptr->PressFlag = 0;

			//send release action
			if(p_wptr->OnRealeaseAction <= pageptr->mActionInstructionsSize)
			{
				actionAddr = (u32)(pageptr->pActionInstructions + p_wptr->OnRealeaseAction);
                csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
			}
		}
	}
    
	if(p_wptr->BindTagID != 0)
	{
		p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	}
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
funcStatus SliderClass::KeyboardTouchCtrl(
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{	
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
		ERROR_PRINT("ERROR: when drawing slider widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}
    
    p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	
	return AHMI_FUNC_SUCCESS;
}

#endif

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
//   add comments by zuz 2018/10/29
// 
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "drawImmediately_cd.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "TouchTrack.h"
#include "WriteTexture.h"

#ifdef AHMI_CORE
extern ActionTriggerClass		gWidgetTagTrigger;
extern ActionTriggerClass		gMouseTagTrigger;
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;

extern QueueHandle_t		ActionInstructionQueue;

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;

extern u8 force_cancel_subcanvas_page_mousetouch;

//-----------------------------
// name£º TouchTrackClass
// construct
//  
//
// addtional comments :
//   none
//-----------------------------
TouchTrackClass::TouchTrackClass()
{

}

//-----------------------------
// name£º ~TouchTrackClass
// deconstruct
//  
//
// addtional comments :
//   none
//-----------------------------
TouchTrackClass::~TouchTrackClass()
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
funcStatus TouchTrackClass::initWidget(
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

//-----------------------------
// compute the texture using the tag value
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 ActionPtr                //the value of the tag
//  @param	 u8_pageRefresh           //refresh type
// addtional comments :
//   none
//-----------------------------
#if defined(EMBEDDED) && defined(VERSION_2)
extern uint8_t* p_Oscilloscopetexture[Chart_Num_MAX];
extern uint32_t  START_ADDR_OFFSET_FROM_GCHEAP;
#endif
funcStatus TouchTrackClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,		
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh			
	)
{	
	s32 cur_x, old_x, lastold_x;
	s32 cur_y, old_y, lastold_y;
	s32 value, oldValue, lastoldValue;
	u16 Width0;
	u16 Height0;
	u16 oldValueinit=0;
	u8 showTrajectory;
	u8 linewidth;
	TextureClassPtr texturePtr;

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
		ERROR_PRINT("ERROR: when drawing touch track widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	showTrajectory = p_wptr->SHOWMODE & 0x3;
	linewidth = p_wptr->SHOWMODE >> 2;

	#ifndef CHIP_GC9002_NoHyper
	if(showTrajectory)
	{
		if(u8_pageRefresh)
		{
			initOscilloscopeTextureRAM(p_wptr->IDnumber, p_wptr->WidgetWidth, p_wptr->WidgetHeight);

			//start address of trajectory texture
			texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 2]);
			#if defined(VERSION_2) && defined(EMBEDDED)
			texturePtr->TexAddr = ((uint32_t)(p_Oscilloscopetexture[p_wptr->IDnumber]) + START_ADDR_OFFSET_FROM_GCHEAP);
			#else
			texturePtr->TexAddr = (START_ADDR_OF_Oscilloscope + p_wptr->IDnumber * SIZE_OF_SingleOscilloscope_BUF + START_ADDR_OFFSET);
			#endif
		}
	}
	#endif //endof #ifndef CHIP_GC9002_NoHyper
	
	//start address of texture
	texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);

	//params
	oldValueinit = (p_wptr->WidgetAttr & 0x8000);
	value = (s32)(ActionPtr->mTagPtr->getValue());

#ifdef EMBEDDED
	if((texturePtr < (void*)startOfDynamicPage) || (texturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
#endif

	Width0 = (texturePtr[0].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512) << 4;
	Height0 = (texturePtr[0].TexHeight * p_wptr->ADAPTIVE_SCALE_LEVEL / 512) << 4;

    if(ActionPtr->mInputType == ACTION_TAG_SET_VALUE && !u8_pageRefresh)
	{
		cur_x = (value/10000)<<4;
		cur_y = (value%10000)<<4;
		if (cur_x<0)
		{
			cur_x = 0;
		}else if (cur_x>Width0)
		{
			cur_x = Width0;
		}

		if (cur_y<0)
		{
			cur_y = 0;
		}else if (cur_y>Height0)
		{
			cur_y = Height0;
		}
		//modify tex offset
		if(showTrajectory)
		{
			//if (showTrajectory == 2)
			//{
			//	if (value == 0xFFFFFFFF)
			//	{
			//		//stop
			//		p_wptr->WidgetAttr |= 0x8000;
			//		p_wptr->OldValueL = (u16)value;
			//		p_wptr->OldValueH = (u16)(value >> 16); 
			//	}
			//	else
			//	{
			//		texturePtr[4].OffsetX = cur_x+texturePtr[0].OffsetX - ((texturePtr[4].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)<<4)/2;
			//		texturePtr[4].OffsetY = cur_y+texturePtr[0].OffsetY - ((texturePtr[4].TexHeight * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)<<4)/2;

			//		if(oldValueinit)
			//		{
			//			oldValue = (u32)((p_wptr->OldValueH  << 16) + p_wptr->OldValueL );
			//			if (oldValue == 0xFFFFFFFF)
			//			{
			//				//skip
			//			}
			//			else
			//			{
			//				old_x = (oldValue/10000)<<4;
			//				old_y = (oldValue%10000)<<4;

			//				#ifndef CHIP_GC9002_NoHyper
			//				//drawPointTexture_alpha1(p_wptr->IDnumber,cur_x>>4,cur_y>>4,p_wptr->WidgetWidth,p_wptr->WidgetHeight,linewidth);
			//				Bresenham(p_wptr->IDnumber,old_x>>4,old_y>>4,cur_x>>4,cur_y>>4,p_wptr->WidgetWidth,p_wptr->WidgetHeight,linewidth);
			//				#endif //endof #ifndef CHIP_GC9002_NoHyper
			//			}
			//			
			//	
			//			p_wptr->WidgetAttr |= 0x8000;
			//			p_wptr->OldValueL = (u16)value;
			//			p_wptr->OldValueH = (u16)(value >> 16); 
			//		}
			//		else
			//		{
			//			p_wptr->WidgetAttr |= 0x8000;
			//			p_wptr->OldValueL = (u16)value;
			//			p_wptr->OldValueH = (u16)(value >> 16); 
			//		}
			//	}
			//}
			//else
			{
				texturePtr[4].OffsetX = cur_x+texturePtr[0].OffsetX - ((texturePtr[4].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)<<4)/2;
				texturePtr[4].OffsetY = cur_y+texturePtr[0].OffsetY - ((texturePtr[4].TexHeight * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)<<4)/2;

				if(oldValueinit)
				{
				    lastoldValue = (u32)((p_wptr->ANIMATIONCURVALUE_H  << 16) + p_wptr->ANIMATIONCURVALUE_L );
				    lastold_x = (lastoldValue/10000)<<4;
				    lastold_y = (lastoldValue%10000)<<4;

					oldValue = (u32)((p_wptr->OldValueH  << 16) + p_wptr->OldValueL );
					old_x = (oldValue/10000)<<4;
					old_y = (oldValue%10000)<<4;

					#ifndef CHIP_GC9002_NoHyper
					if((lastoldValue == oldValue) || (oldValue == value))
					{

					}
					else
					{
					    Bezier(p_wptr->IDnumber,lastold_x>>4,lastold_y>>4,old_x>>4,old_y>>4,cur_x>>4,cur_y>>4,p_wptr->WidgetWidth,p_wptr->WidgetHeight,linewidth);
                    }
					//drawPointTexture_alpha1(p_wptr->IDnumber,cur_x>>4,cur_y>>4,p_wptr->WidgetWidth,p_wptr->WidgetHeight,9);
					//Bresenham(p_wptr->IDnumber,old_x>>4,old_y>>4,cur_x>>4,cur_y>>4,p_wptr->WidgetWidth,p_wptr->WidgetHeight,linewidth);
					//Bezier(p_wptr->IDnumber,lastold_x>>4,lastold_y>>4,old_x>>4,old_y>>4,cur_x>>4,cur_y>>4,p_wptr->WidgetWidth,p_wptr->WidgetHeight,linewidth);
					#endif //endof #ifndef CHIP_GC9002_NoHyper
				
					p_wptr->WidgetAttr |= 0x8000;

					p_wptr->ANIMATIONCURVALUE_L = p_wptr->OldValueL;
					p_wptr->ANIMATIONCURVALUE_H = p_wptr->OldValueH;

					p_wptr->OldValueL = (u16)value;
					p_wptr->OldValueH = (u16)(value >> 16); 
				}
				else
				{
					p_wptr->WidgetAttr |= 0x8000;
					p_wptr->OldValueL = (u16)value;
					p_wptr->OldValueH = (u16)(value >> 16);
					p_wptr->ANIMATIONCURVALUE_L = (u16)value;
					p_wptr->ANIMATIONCURVALUE_H = (u16)(value >> 16);
				}
			}
			
		}
		else
		{
			texturePtr[1].OffsetX = cur_x+texturePtr[0].OffsetX - ((texturePtr[1].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)<<4)/2;
			texturePtr[1].OffsetY = cur_y+texturePtr[0].OffsetY - ((texturePtr[1].TexHeight * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)<<4)/2;
			
			p_wptr->WidgetAttr |= 0x8000;
			p_wptr->OldValueL = (u16)value;
			p_wptr->OldValueH = (u16)(value >> 16); 
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
funcStatus TouchTrackClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    u8 u8_widgetRefresh      
    )
{
	DynamicPageClassPtr pCurrentPage;
	u32 actionAddr;
	s32 oldValue=0;
	s32 value;
    s32 lowAlarmValue ;
	s32 highAlarmValue;
	u16 oldValueinit=0;

	//param check
	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	oldValueinit = (p_wptr->WidgetAttr & 0x8000);
	value = (s32)(ActionPtr->mTagPtr->getValue());

    pCurrentPage = pagePtr;
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
//don'r support touch propagation
//@param WidgetClassPtr p_wptr Pointer of widget
//@param ActionPtr             Pointer of trigger
//@param u8_pageRefresh        whether refresh the page. if refresh, the value of the widget also refresh.
//**************************************
funcStatus TouchTrackClass::MouseTouchCtrl(
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
   	s32 tag_value= 0, oldValue;
	s32 cur_x;
	s32 cur_y;
	u16 Width0;
	u16 Height0;
	TextureClassPtr texturePtr;
	//u8 showTrajectory;

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
		ERROR_PRINT("ERROR: when drawing touch track widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	//start address of texture
	texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);

	//showTrajectory = p_wptr->SHOWMODE & 0x3;
	
#ifdef EMBEDDED
	if((texturePtr < (void*)startOfDynamicPage) || (texturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
#endif

	Width0 = (texturePtr[0].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512) << 4;
	Height0 = (texturePtr[0].TexHeight * p_wptr->ADAPTIVE_SCALE_LEVEL / 512) << 4;

	if( ActionPtr->mInputType == ACTION_MOUSE_PRESS )
	{
		p_wptr->PressFlag = 1;

		cur_x  = ((s16)(pPressPoint->x) << 4);
		cur_y  = ((s16)(pPressPoint->y) << 4);


		cur_x = cur_x - texturePtr[0].OffsetX;
		cur_y = cur_y - texturePtr[0].OffsetY;

		if (cur_x<0)
		{
			cur_x = 0;
		}else if (cur_x>Width0)
		{
			cur_x = Width0;
		}

		if (cur_y<0)
		{
			cur_y = 0;
		}else if (cur_y>Height0)
		{
			cur_y = Height0;
		}

		tag_value = (cur_x>>4) * 10000 + (cur_y>>4) ; //caculate value

		p_wptr->OldValueL = (u16)tag_value;
		p_wptr->OldValueH = (u16)(tag_value >> 16);
		p_wptr->WidgetAttr &= 0x7FFF;
		//modify tex offset
		/*texturePtr[1].OffsetX = cur_x+texturePtr[0].OffsetX - (texturePtr[1].TexWidth<<4)/2;
		texturePtr[1].OffsetY = cur_y+texturePtr[0].OffsetY - (texturePtr[1].TexHeight<<4)/2;*/

		TagPtr->getTag(p_wptr->BindTagID).setRawValue(tag_value);
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
	else if(ActionPtr->mInputType == ACTION_MOUSE_HOLDING)
	{
		if(p_wptr->PressFlag)
		{
			cur_x  = ((s16)(pReleasePoint->x)) << 4;
			cur_y  = ((s16)(pReleasePoint->y)) << 4;

			cur_x = cur_x - texturePtr[0].OffsetX;
			cur_y = cur_y - texturePtr[0].OffsetY;

			if (cur_x<0)
			{
				cur_x = 0;
			}else if (cur_x>Width0)
			{
				cur_x = Width0;
			}

			if (cur_y<0)
			{
				cur_y = 0;
			}else if (cur_y>Height0)
			{
				cur_y = Height0;
			}

			tag_value = (cur_x>>4) * 10000 + (cur_y>>4) ; //caculate value
			oldValue = (u32)((p_wptr->OldValueH  << 16) + p_wptr->OldValueL );
			//modify tex offset
			/*texturePtr[1].OffsetX = cur_x+texturePtr[0].OffsetX - (texturePtr[1].TexWidth<<4)/2;
			texturePtr[1].OffsetY = cur_y+texturePtr[0].OffsetY - (texturePtr[1].TexHeight<<4)/2;*/

            if(tag_value != oldValue)
            {
                TagPtr->getTag(p_wptr->BindTagID).setRawValue(tag_value);
                force_cancel_subcanvas_page_mousetouch = 1;
                p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
                p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
            }
			return AHMI_FUNC_SUCCESS;
		}
	}
    else if(ActionPtr->mInputType == ACTION_MOUSE_RELEASE)
	{
		if(p_wptr->PressFlag)
		{
			p_wptr->PressFlag = 0;

			//if (showTrajectory == 2)
			//{
			//	tag_value = 0xFFFFFFFF ; //caculate value
			//	TagPtr->getTag(p_wptr->BindTagID).setRawValue(tag_value);
			//}

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
	}
	
	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Keyboard.Touch
//
//@param WidgetClassPtr p_wptr Pointer of widget
//@param ActionPtr             Pointer of trigger
//@param u8_pageRefresh        whether refresh the page. if refresh, the value of the widget also refresh.
//**************************************
funcStatus TouchTrackClass::KeyboardTouchCtrl(
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

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
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "ProgBarClass.h"
#include "drawImmediately_cd.h"

#ifdef AHMI_CORE
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
//extern u16 WorkingPageID;

extern QueueHandle_t		ActionInstructionQueue;

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;

extern Easing gEasing;

//-----------------------------
// name�?ProgBarClass
// construct
//  
//
// addtional comments :
//   none
//-----------------------------
ProgBarClass::ProgBarClass()
{

}

//-----------------------------
// name�?~ProgBarClass
// deconstruct
//  
//
// addtional comments :
//   none
//-----------------------------
ProgBarClass::~ProgBarClass()
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
funcStatus ProgBarClass::initWidget(
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
        //find the binding tag
        bindTag = &TagPtr->getTag(p_wptr->BindTagID);
		//set the value of widget
		tagtrigger.mTagPtr = bindTag;
		tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
		if(u8_pageRefresh)
		{
			SetValueCtrl(pagePtr, p_wptr, &tagtrigger,u8_pageRefresh );
		}
		if(widgetCtrl(pagePtr,p_wptr,&tagtrigger,u8_pageRefresh) != AHMI_FUNC_SUCCESS)
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

//******************************************
//progbar
//needs 3/4/5 texture
//0：background
//1：puremask for bar to decide how many to display
//2: bar texture
//3-4: (optianal) pointer texture and pointer mask
//WidgetAttr�?
//15:whether the old value has been initialized
//14-10:reserved
//9:whether to display?
//8:whether the pointer needs to change color, 1 change, 0 not change
//7:progbar mode ,0 oridinal,1 changing color
//6:pointer, 1 has, 0 no
//5:direction of the bar,1 vertical,0 herizontal
//4-0:widget type 0x4
//******************************************
funcStatus ProgBarClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{
	s32 ValueStart;
	s32 ValueStop;
	s32 value;
//	u8 display;

	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	if( (s16)(p_wptr->WidgetOffsetX) > MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetY) > MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetX) < -MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetY) < -MAX_WIDTH_AND_HEIGHT ||
		p_wptr->WidgetWidth > MAX_WIDTH_AND_HEIGHT ||
		p_wptr->WidgetHeight > MAX_WIDTH_AND_HEIGHT )
	{
		ERROR_PRINT("ERROR: when drawing progressbar widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	value = (s32)ActionPtr->mTagPtr->getValue();
	renderTexture(pagePtr, p_wptr,value);

	if( p_wptr->ANIMATION_DURATION != 0) //animation
	{
		p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
		p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;
		p_wptr->STOP_TAG_L = (u16)( (u32)value );
		p_wptr->STOP_TAG_H = (u16)(( (u32)value ) >> 16);

		ValueStart = (s32)((p_wptr->START_TAG_L) +  ( (p_wptr->START_TAG_H) << 16));
		ValueStop = (s32)((p_wptr->STOP_TAG_L) + ( (p_wptr->STOP_TAG_H) << 16));

		if(ValueStart != ValueStop)//create animation
		{
			p_wptr->loadTextureWithAnimation(pagePtr);
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
//4-0: type of widget
//**************************************
funcStatus ProgBarClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    u8 u8_widgetRefresh,
	u8 u8_actionEnable
    )
{
	DynamicPageClassPtr pCurrentPage;
	s32 maxValue;
	s32 minValue;
    s32 value;
	s32 lowAlarmValue ;
	s32 highAlarmValue;
	u32 actionAddr;
	s32 oldValue;
	u16 oldValueinit;

	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	value = (s32)ActionPtr->mTagPtr->getValue();

	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);

	if((value < minValue) || (value > maxValue)){
		ERROR_PRINT("ERROR: ProgBar widget bindTag value exceeds the boundary");
		u16 CanvasID, SubcanvasID, WidgetID;
		p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
		EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
	}

	if(u8_widgetRefresh == 0)
    {
        if((p_wptr->ANIMATION_DURATION) != 0) //contain animation
        {
			//calculate real start value
			p_wptr->calculateCurrentAnimation(pagePtr);
			p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
		    p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;
		    p_wptr->STOP_TAG_L = (u16)( (u32)value );
		    p_wptr->STOP_TAG_H = (u16)(( (u32)value ) >> 16);
			//p_wptr->loadTextureWithAnimation(pagePtr);
        }
    }

	oldValueinit = (p_wptr->WidgetAttr & 0x8000) ? 1 : 0;

	lowAlarmValue  = (s32)((p_wptr->LowAlarmValueH  << 16) + p_wptr->LowAlarmValueL ) ;
	highAlarmValue = (s32)((p_wptr->HighAlarmValueH << 16) + p_wptr->HighAlarmValueL) ;
	
	pCurrentPage = pagePtr;
	if(oldValueinit)
	{
		oldValue = (s32)((p_wptr->OldValueH  << 16) + p_wptr->OldValueL);
		if((u8_actionEnable && p_wptr->GlobalTriggerFlag && (p_wptr->NumOfTex == 0)) || ((p_wptr->GlobalTriggerFlag == 0) && (p_wptr->NumOfTex == 0)) || (p_wptr->NumOfTex != 0))
		{
			//tagchange by Mr.z
			if(oldValue != value){
				if(p_wptr->TagChangeAction && (p_wptr->TagChangeAction <= pCurrentPage->mActionInstructionsSize))
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
	}
    else // no old value
    {
  //      if(p_wptr->TagChangeAction && (p_wptr->TagChangeAction <= pCurrentPage->mActionInstructionsSize))
		//{
		//	actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->TagChangeAction);
		//	xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
		//}
        
  //      if(p_wptr->EnterLowAlarmAction && value < lowAlarmValue)
		//{
		//	//enter low
		//	if(p_wptr->EnterLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
		//	{
		//		actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterLowAlarmAction);
		//		xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
		//	}		
		//}
  //      
  //      if(p_wptr->EnterHighAlarmAction && value > highAlarmValue)
		//{
		//	//enter high
		//	if(p_wptr->EnterHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
		//	{
		//		actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterHighAlarmAction);
		//		xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);		
		//	}
		//}
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
funcStatus ProgBarClass::MouseTouchCtrl(
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
funcStatus ProgBarClass::KeyboardTouchCtrl(
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

//-----------------------------
// compute the texture using the tag value
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the tag
// addtional comments :
//   none
//-----------------------------
funcStatus ProgBarClass::renderTexture
	(
		DynamicPageClassPtr pagePtr,
		WidgetClassPtr p_wptr,  
		s32 value               
	)
{
    TextureClassPtr texturePtr;
	s32	maxValue;
	s32	minValue;
	s32 threshold0;
	s32 threshold1;
	u16 width;
	u16 height;
	u16 newWidth;
	u16 newHeight;
	u16 offsetX;
	u16 offsetY;
//	u16 rightBox;
//	u16 topBox;
	u16 changeColorMode; //change color mode�? is gradually,1sudden
    u16 barColor;
	u16 barPtr;
	s16 differenceOfR;
	s16 differenceOfG;
	s16 differenceOfB;
	u16 thresholdNum;
	u16 display; //thether to display the program bar
	u16 differenceOfValue;
	u8 dir;
	u8 lineFileEn;       //whether has line
	u8 lineChangeColor;  //whether change color
	u8 changeColor;
	u8 startR;
	u8 startG;
	u8 startB;
	u8 endR;
	u8 endG;
	u8 endB;
	u8 curR;
	u8 curG;
	u8 curB;
//	u8 linePtrOffsetToBar;
	

	if((NULL == p_wptr) )
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);

	dir = (p_wptr->WidgetAttr & PROGBAR_DIR  ) >> 13;
	changeColor = (p_wptr->WidgetAttr & CHANGECOLOR  )? 1 : 0;
	lineChangeColor = (p_wptr->WidgetAttr & LINE_CHANGE_COLOR  )? 1 : 0;
	lineChangeColor = 0;
	width = p_wptr->WidgetWidth;
	height = p_wptr->WidgetHeight;
	//offsetX = p_wptr->WidgetOffsetX;
	//offsetY = p_wptr->WidgetOffsetY;
	offsetX = 0;
	offsetY = 0;
	lineFileEn = (p_wptr->WidgetAttr & LINE_FILE_EN  )? 1 : 0; //whether has line
	display = (p_wptr->WidgetAttr & PROBARDISPLAY)?1:0;//thether to display the program bar
	barColor = (p_wptr->WidgetAttr & BAR_IS_COLOR)?1:0;//whether the bar is color
	changeColorMode = (p_wptr->WidgetAttr & CHANGECOLOR_MODE)?1:0; //whether change gradually
	thresholdNum = (p_wptr->WidgetAttr & THRESHOLD_NUM)?1:0; //the number of threshold, 0 is one, 1 is two

	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);
	if(maxValue == minValue){
		ERROR_PRINT("ERRROR: can't divided by zero");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
	}
	if(value > maxValue) 
		value = maxValue;
	if(value < minValue) 
		value = minValue;

	//not the trigger
	if(display)
	{
		texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);

		if(barColor) //color mode, the bar needs only one texture. change the width or hight
			barPtr = 1; 
		else         //image mode, the bar needs two texture, change the width or height of mask texture
		{
			barPtr = 3;
			//texturePtr[barPtr - 1].mTexAttr |= TEXTURE_USING_WIDGET_BOX;
		}
		//change the size of mask texture
		if(dir ==1)//up
		{

			newHeight = (u16)(((value - minValue) * height) /((maxValue - minValue))); //the changed height
			offsetY = offsetY + height - newHeight;
			if(!barColor)//the progbar is not color
			{
				//barPtr - 1 is the puremask texture for showing
				//0-background, 1-PUREMASK for clear alpha buffer, 2-PUREMASK for showing the display part, 3 is the bar(image)
				texturePtr[barPtr-1].OffsetY = (offsetY)<<4;
				texturePtr[barPtr-1].TexHeight = newHeight;
			}
			else
			{
				//texture[0] is background, not change
				texturePtr[barPtr].OffsetY = (offsetY)<<4;
				texturePtr[barPtr].TexHeight = newHeight;
			}

			//bar needs to change color
			if(changeColor)
			{
				if(changeColorMode) //sudden change
				{
				
					//threshold0 = p_wptr->THRESHOLD0_L + (p_wptr->THRESHOLD0_H << 8);
					//threshold1 = p_wptr->THRESHOLD1_L + (p_wptr->THRESHOLD1_H << 8);
					threshold0 = int((s16)p_wptr->THRESHOLD0);
					threshold1 = int((s16)p_wptr->THRESHOLD1);

					//mask
					texturePtr[barPtr-1].OffsetY = (offsetY)<<4;
					texturePtr[barPtr-1].TexHeight = newHeight;
					//texturePtr[barPtr-1].TexTopTileBox = (u8)topBox;
					//texturePtr[barPtr-1].mTexAttr |= (TEXTURE_USING_WIDGET_BOX);

					if(thresholdNum) //3 threshold
					{
						//t1 - barPtr
						//t2 - barPtr + 1
						//t3 - barPtr + 2
						if(value < threshold0){
							//t1
							texturePtr[barPtr].mTexAttr &= ~(FORCE_UNDISPLAY);
							texturePtr[barPtr+1].mTexAttr |= (FORCE_UNDISPLAY);
							texturePtr[barPtr+2].mTexAttr |= (FORCE_UNDISPLAY);
						}else if(value >= threshold1){
							//t3
							texturePtr[barPtr+2].mTexAttr &= ~(FORCE_UNDISPLAY);
							texturePtr[barPtr].mTexAttr |= (FORCE_UNDISPLAY);
							texturePtr[barPtr+1].mTexAttr |= (FORCE_UNDISPLAY);
						}else{
							//t2
							texturePtr[barPtr+1].mTexAttr &= ~(FORCE_UNDISPLAY);
							texturePtr[barPtr].mTexAttr |= (FORCE_UNDISPLAY);
							texturePtr[barPtr+2].mTexAttr |= (FORCE_UNDISPLAY);
						}

						//used for line file
						barPtr = 5;
					}
					else //2 threshold
					{
						if(value < threshold0){
							//tex1 show
							texturePtr[barPtr].mTexAttr &= ~(FORCE_UNDISPLAY);
							texturePtr[barPtr+1].mTexAttr |= (FORCE_UNDISPLAY);
						}else{
							texturePtr[barPtr].mTexAttr |= (FORCE_UNDISPLAY);
							texturePtr[barPtr+1].mTexAttr &= ~(FORCE_UNDISPLAY);
						}

						//used for line file
						barPtr = 4;
						
					}
				}
				else //gradual change
				{
					startB = (u8)p_wptr->StartColorValueGB;
					startG = (u8)(p_wptr->StartColorValueGB >> 8);
					startR = (u8)p_wptr->EndColorValueBAndStartColorValueR;
					endB = (u8)(p_wptr->EndColorValueBAndStartColorValueR >> 8);
					endG = (u8)p_wptr->EndColorValueRG;
					endR = (u8)(p_wptr->EndColorValueRG >> 8);
					differenceOfB = endB - startB;
					differenceOfG = endG - startG;
					differenceOfR = endR - startR;
					differenceOfValue = maxValue - minValue;

					//insert the color
					curB = startB;
					curB += ((s16)(value - minValue) * differenceOfB) / differenceOfValue;
					curG = startG;
					curG += ((s16)(value - minValue) * differenceOfG) / differenceOfValue;
					curR = startR;
					curR += ((s16)(value - minValue) * differenceOfR) / differenceOfValue;
					//0-background, 1-color 2-line
					texturePtr[barPtr].TexAddr &= 0xFF000000;
					texturePtr[barPtr].TexAddr |= curB;
					texturePtr[barPtr].TexAddr |= (curG << 8);
					texturePtr[barPtr].TexAddr |= (curR << 16);
				}

			}

			//has line
			if(lineFileEn)
			{
				if(!(p_wptr->NumOfTex >= 5 && p_wptr->NumOfTex <=  11) )
					return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
				//texturePtr[barPtr + 3].mTexAttr |= TEXTURE_USING_WIDGET_BOX;
				texturePtr[barPtr + 3].OffsetY = (offsetY - (texturePtr[barPtr + 3].TexHeight/2 * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)) << 4;
				texturePtr[barPtr + 3].OffsetX = (offsetX + (width - (texturePtr[barPtr + 3].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512))/2) << 4;
				if(lineChangeColor) 
				{ 
					//0-background, 1-bar color 2-clear puremask, 3 alpha 4-line color
					//we need to change line color, so it is barPtr+3
					texturePtr[barPtr+4].TexAddr &= 0xFF000000;
					texturePtr[barPtr+4].TexAddr |= curB;
					texturePtr[barPtr+4].TexAddr |= (curG << 8);
					texturePtr[barPtr+4].TexAddr |= (curR << 16);
				}
			}
		}
		else if(dir == 0)//right
		{
			newWidth = (u16)( ((value - minValue) * width) /((maxValue - minValue)) );
			//texturePtr[1].TexWidth  = newWidth;
			//rightBox = (offsetX + newWidth)/TILESIZE;
			//texturePtr[1].TexRightTileBox = (u8)rightBox;
			//if(!barColor)
			//	texturePtr[2].TexRightTileBox = (u8)rightBox;


			if(!barColor)//进度条的bar不是颜色
			{
				//barPtr - 1 is the puremask texture for showing
				//0-background, 1-PUREMASK for clear alpha buffer, 2-PUREMASK for showing the display part, 3 is the bar(image)
				texturePtr[barPtr-1].OffsetX = (offsetX)<<4;
				texturePtr[barPtr-1].TexWidth = newWidth;
			}
			else
			{
				//0号纹理是背景不变�?号是纯颜�?
				texturePtr[barPtr].OffsetX = (offsetX)<<4;
				texturePtr[barPtr].TexWidth = newWidth;

			}

			if(changeColor)
			{
				if(changeColorMode) //sudden change
				{
					//threshold0 = p_wptr->THRESHOLD0_L + (p_wptr->THRESHOLD0_H << 8);
					//threshold1 = p_wptr->THRESHOLD1_L + (p_wptr->THRESHOLD1_H << 8);
					threshold0 = int((s16)p_wptr->THRESHOLD0);
					threshold1 = int((s16)p_wptr->THRESHOLD1);

					//mask
					texturePtr[barPtr-1].OffsetX = (offsetX)<<4;
					texturePtr[barPtr-1].TexWidth  = newWidth;
					//texturePtr[barPtr-1].TexRightTileBox = (u8)rightBox;
					//texturePtr[barPtr-1].mTexAttr |= (TEXTURE_USING_WIDGET_BOX);
					if(thresholdNum) //3 threshold
					{
						if(value < threshold0){
							//1
							texturePtr[barPtr].mTexAttr &= ~(FORCE_UNDISPLAY);
							texturePtr[barPtr+1].mTexAttr |= (FORCE_UNDISPLAY);
							texturePtr[barPtr+2].mTexAttr |= (FORCE_UNDISPLAY);
						}else if(value >= threshold1){
							//3
							texturePtr[barPtr+2].mTexAttr &= ~(FORCE_UNDISPLAY);
							texturePtr[barPtr].mTexAttr |= (FORCE_UNDISPLAY);
							texturePtr[barPtr+1].mTexAttr |= (FORCE_UNDISPLAY);
						}else{
							//2
							texturePtr[barPtr+1].mTexAttr &= ~(FORCE_UNDISPLAY);
							texturePtr[barPtr].mTexAttr |= (FORCE_UNDISPLAY);
							texturePtr[barPtr+2].mTexAttr |= (FORCE_UNDISPLAY);
						}
						barPtr = 5;
					}
					else //2 threshold
					{
						if(value < threshold0){
							//tex1 show
							texturePtr[barPtr].mTexAttr &= ~(FORCE_UNDISPLAY);
							texturePtr[barPtr+1].mTexAttr |= (FORCE_UNDISPLAY);
						}else{
							texturePtr[barPtr].mTexAttr |= (FORCE_UNDISPLAY);
							texturePtr[barPtr+1].mTexAttr &= ~(FORCE_UNDISPLAY);
						}

						barPtr = 4;
					}
				}
				else //gradual
				{
					startB = (u8)p_wptr->StartColorValueGB;
					startG = (u8)(p_wptr->StartColorValueGB >> 8);
					startR = (u8)p_wptr->EndColorValueBAndStartColorValueR;
					endB = (u8)(p_wptr->EndColorValueBAndStartColorValueR >> 8);
					endG = (u8)p_wptr->EndColorValueRG;
					endR = (u8)(p_wptr->EndColorValueRG >> 8);
					differenceOfB = endB - startB;
					differenceOfG = endG - startG;
					differenceOfR = endR - startR;
					differenceOfValue = maxValue - minValue;

					//insert the color
					curB = startB;
					curB += ((s16)(value - minValue) * differenceOfB) / differenceOfValue;
					curG = startG;
					curG += ((s16)(value - minValue) * differenceOfG) / differenceOfValue;
					curR = startR;
					curR += ((s16)(value - minValue) * differenceOfR) / differenceOfValue;

					texturePtr[1].TexAddr &= 0xFF000000;
					texturePtr[1].TexAddr |= curB;
					texturePtr[1].TexAddr |= (curG << 8);
					texturePtr[1].TexAddr |= (curR << 16);
				}
			}

			//has line
			if(lineFileEn)
			{
				if(!(p_wptr->NumOfTex >= 5 && p_wptr->NumOfTex <=  11) )
					return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
				//texturePtr[barPtr + 3].mTexAttr |= TEXTURE_USING_WIDGET_BOX;
				texturePtr[barPtr + 3].OffsetX = (offsetX + newWidth - (texturePtr[barPtr + 3].TexWidth/2 * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)) << 4;
				texturePtr[barPtr + 3].OffsetY =  (offsetY + (height - (texturePtr[barPtr + 3].TexHeight * p_wptr->ADAPTIVE_SCALE_LEVEL / 512))/2)<< 4;

				if(lineChangeColor) 
				{
					texturePtr[barPtr + 4].TexAddr &= 0xFF000000;
					texturePtr[barPtr + 4].TexAddr |= curB;
					texturePtr[barPtr + 4].TexAddr |= (curG << 8);
					texturePtr[barPtr + 4].TexAddr |= (curR << 16);
				}
			}
		}
		else if(dir == 2)//left
		{
			newWidth = (u16)( ((value - minValue) * width) /((maxValue - minValue)) );

			if(!barColor)//进度条的bar不是颜色
			{
				//barPtr - 1 is the puremask texture for showing
				//0-background, 1-PUREMASK for clear alpha buffer, 2-PUREMASK for showing the display part, 3 is the bar(image)
				texturePtr[barPtr-1].OffsetX = (offsetX + width - newWidth)<<4;
				texturePtr[barPtr-1].TexWidth = newWidth;
			}
			else
			{
				//0号纹理是背景不变�?号是纯颜�?
				texturePtr[barPtr].OffsetX = (offsetX + width - newWidth)<<4;
				texturePtr[barPtr].TexWidth = newWidth;

			}

			if(changeColor)
			{
				if(changeColorMode) //sudden change
				{
					//threshold0 = p_wptr->THRESHOLD0_L + (p_wptr->THRESHOLD0_H << 8);
					//threshold1 = p_wptr->THRESHOLD1_L + (p_wptr->THRESHOLD1_H << 8);
					threshold0 = int((s16)p_wptr->THRESHOLD0);
					threshold1 = int((s16)p_wptr->THRESHOLD1);

					//mask
					texturePtr[barPtr-1].OffsetX = (offsetX + width - newWidth)<<4;
					texturePtr[barPtr-1].TexWidth  = newWidth;
					//texturePtr[barPtr-1].TexRightTileBox = (u8)rightBox;
					//texturePtr[barPtr-1].mTexAttr |= (TEXTURE_USING_WIDGET_BOX);
					if(thresholdNum) //3 threshold
					{
						if(value < threshold0){
							//1
							texturePtr[barPtr].mTexAttr &= ~(FORCE_UNDISPLAY);
							texturePtr[barPtr+1].mTexAttr |= (FORCE_UNDISPLAY);
							texturePtr[barPtr+2].mTexAttr |= (FORCE_UNDISPLAY);
						}else if(value >= threshold1){
							//3
							texturePtr[barPtr+2].mTexAttr &= ~(FORCE_UNDISPLAY);
							texturePtr[barPtr].mTexAttr |= (FORCE_UNDISPLAY);
							texturePtr[barPtr+1].mTexAttr |= (FORCE_UNDISPLAY);
						}else{
							//2
							texturePtr[barPtr+1].mTexAttr &= ~(FORCE_UNDISPLAY);
							texturePtr[barPtr].mTexAttr |= (FORCE_UNDISPLAY);
							texturePtr[barPtr+2].mTexAttr |= (FORCE_UNDISPLAY);
						}
						barPtr = 5;
					}
					else //2 threshold
					{
						if(value < threshold0){
							//tex1 show
							texturePtr[barPtr].mTexAttr &= ~(FORCE_UNDISPLAY);
							texturePtr[barPtr+1].mTexAttr |= (FORCE_UNDISPLAY);
						}else{
							texturePtr[barPtr].mTexAttr |= (FORCE_UNDISPLAY);
							texturePtr[barPtr+1].mTexAttr &= ~(FORCE_UNDISPLAY);
						}

						barPtr = 4;
					}
				}
				else //gradual
				{
					startB = (u8)p_wptr->StartColorValueGB;
					startG = (u8)(p_wptr->StartColorValueGB >> 8);
					startR = (u8)p_wptr->EndColorValueBAndStartColorValueR;
					endB = (u8)(p_wptr->EndColorValueBAndStartColorValueR >> 8);
					endG = (u8)p_wptr->EndColorValueRG;
					endR = (u8)(p_wptr->EndColorValueRG >> 8);
					differenceOfB = endB - startB;
					differenceOfG = endG - startG;
					differenceOfR = endR - startR;
					differenceOfValue = maxValue - minValue;

					//insert the color
					curB = startB;
					curB += ((s16)(value - minValue) * differenceOfB) / differenceOfValue;
					curG = startG;
					curG += ((s16)(value - minValue) * differenceOfG) / differenceOfValue;
					curR = startR;
					curR += ((s16)(value - minValue) * differenceOfR) / differenceOfValue;

					texturePtr[1].TexAddr &= 0xFF000000;
					texturePtr[1].TexAddr |= curB;
					texturePtr[1].TexAddr |= (curG << 8);
					texturePtr[1].TexAddr |= (curR << 16);
				}
			}

			//has line
			if(lineFileEn)
			{
				if(!(p_wptr->NumOfTex >= 5 && p_wptr->NumOfTex <=  11) )
					return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
				//texturePtr[barPtr + 3].mTexAttr |= TEXTURE_USING_WIDGET_BOX;
				texturePtr[barPtr + 3].OffsetX = (offsetX + width - newWidth - (texturePtr[barPtr + 3].TexWidth/2 * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)) << 4;
				texturePtr[barPtr + 3].OffsetY = (offsetY + (height - texturePtr[barPtr + 3].TexHeight * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)/2)<< 4;
				//rightBox = (offsetX + p_wptr->WidgetWidth)/TILESIZE;
				//texturePtr[barPtr + 3].TexRightTileBox = (u8)rightBox;
				if(lineChangeColor) 
				{
					texturePtr[barPtr + 4].TexAddr &= 0xFF000000;
					texturePtr[barPtr + 4].TexAddr |= curB;
					texturePtr[barPtr + 4].TexAddr |= (curG << 8);
					texturePtr[barPtr + 4].TexAddr |= (curR << 16);
				}
			}
		}
		if(dir ==3)//down
		{

			newHeight = (u16)(((value - minValue) * height) /((maxValue - minValue))); //the changed height
			if(!barColor)//the progbar is not color
			{
				//barPtr - 1 is the puremask texture for showing
				//0-background, 1-PUREMASK for clear alpha buffer, 2-PUREMASK for showing the display part, 3 is the bar(image)
				texturePtr[barPtr-1].OffsetY = (offsetY)<<4;
				texturePtr[barPtr-1].TexHeight = newHeight;
			}
			else
			{
				//texture[0] is background, not change
				texturePtr[barPtr].OffsetY = (offsetY)<<4;
				texturePtr[barPtr].TexHeight = newHeight;
			}

			//bar needs to change color
			if(changeColor)
			{
				if(changeColorMode) //sudden change
				{
				
					//threshold0 = p_wptr->THRESHOLD0_L + (p_wptr->THRESHOLD0_H << 8);
					//threshold1 = p_wptr->THRESHOLD1_L + (p_wptr->THRESHOLD1_H << 8);
					threshold0 = int((s16)p_wptr->THRESHOLD0);
					threshold1 = int((s16)p_wptr->THRESHOLD1);

					//mask
					texturePtr[barPtr-1].OffsetY = (offsetY)<<4;
					texturePtr[barPtr-1].TexHeight = newHeight;
					//texturePtr[barPtr-1].TexTopTileBox = (u8)topBox;
					//texturePtr[barPtr-1].mTexAttr |= (TEXTURE_USING_WIDGET_BOX);

					if(thresholdNum) //3 threshold
					{
						//t1 - barPtr
						//t2 - barPtr + 1
						//t3 - barPtr + 2
						if(value < threshold0){
							//t1
							texturePtr[barPtr].mTexAttr &= ~(FORCE_UNDISPLAY);
							texturePtr[barPtr+1].mTexAttr |= (FORCE_UNDISPLAY);
							texturePtr[barPtr+2].mTexAttr |= (FORCE_UNDISPLAY);
						}else if(value >= threshold1){
							//t3
							texturePtr[barPtr+2].mTexAttr &= ~(FORCE_UNDISPLAY);
							texturePtr[barPtr].mTexAttr |= (FORCE_UNDISPLAY);
							texturePtr[barPtr+1].mTexAttr |= (FORCE_UNDISPLAY);
						}else{
							//t2
							texturePtr[barPtr+1].mTexAttr &= ~(FORCE_UNDISPLAY);
							texturePtr[barPtr].mTexAttr |= (FORCE_UNDISPLAY);
							texturePtr[barPtr+2].mTexAttr |= (FORCE_UNDISPLAY);
						}

						//used for line file
						barPtr = 5;
					}
					else //2 threshold
					{
						if(value < threshold0){
							//tex1 show
							texturePtr[barPtr].mTexAttr &= ~(FORCE_UNDISPLAY);
							texturePtr[barPtr+1].mTexAttr |= (FORCE_UNDISPLAY);
						}else{
							texturePtr[barPtr].mTexAttr |= (FORCE_UNDISPLAY);
							texturePtr[barPtr+1].mTexAttr &= ~(FORCE_UNDISPLAY);
						}

						//used for line file
						barPtr = 4;
						
					}
				}
				else //gradual change
				{
					startB = (u8)p_wptr->StartColorValueGB;
					startG = (u8)(p_wptr->StartColorValueGB >> 8);
					startR = (u8)p_wptr->EndColorValueBAndStartColorValueR;
					endB = (u8)(p_wptr->EndColorValueBAndStartColorValueR >> 8);
					endG = (u8)p_wptr->EndColorValueRG;
					endR = (u8)(p_wptr->EndColorValueRG >> 8);
					differenceOfB = endB - startB;
					differenceOfG = endG - startG;
					differenceOfR = endR - startR;
					differenceOfValue = maxValue - minValue;

					//insert the color
					curB = startB;
					curB += ((s16)(value - minValue) * differenceOfB) / differenceOfValue;
					curG = startG;
					curG += ((s16)(value - minValue) * differenceOfG) / differenceOfValue;
					curR = startR;
					curR += ((s16)(value - minValue) * differenceOfR) / differenceOfValue;
					//0-background, 1-color 2-line
					texturePtr[barPtr].TexAddr &= 0xFF000000;
					texturePtr[barPtr].TexAddr |= curB;
					texturePtr[barPtr].TexAddr |= (curG << 8);
					texturePtr[barPtr].TexAddr |= (curR << 16);
				}

			}

			//has line
			if(lineFileEn)
			{
				if(!(p_wptr->NumOfTex >= 5 && p_wptr->NumOfTex <=  11) )
					return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
				//texturePtr[barPtr + 3].mTexAttr |= TEXTURE_USING_WIDGET_BOX;
				texturePtr[barPtr + 3].OffsetY = (offsetY + newHeight - (texturePtr[barPtr + 3].TexHeight/2 * p_wptr->ADAPTIVE_SCALE_LEVEL / 512)) << 4;
				texturePtr[barPtr + 3].OffsetX = (offsetX + (width - (texturePtr[barPtr + 3].TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512))/2) << 4;
				if(lineChangeColor) 
				{ 
					//0-background, 1-bar color 2-clear puremask, 3 alpha 4-line color
					//we need to change line color, so it is barPtr+3
					texturePtr[barPtr+4].TexAddr &= 0xFF000000;
					texturePtr[barPtr+4].TexAddr |= curB;
					texturePtr[barPtr+4].TexAddr |= (curG << 8);
					texturePtr[barPtr+4].TexAddr |= (curR << 16);
				}
			}
		}
	}
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name�?setATag
// set the texture's animation tag
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the animation tag
// addtional comments :
//   none
//-----------------------------
funcStatus ProgBarClass::setATag
	(
	WidgetClassPtr p_wptr,  
	u16 ATagValue
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
	sprintf(text,"set meter animation tag %d", ATagValue);
	ERROR_PRINT(text);
#endif
	p_wptr->NOW_DURATION |= ATagValue;

	//start and stop value of animation
	ValueStart = (s32)((p_wptr->START_TAG_L) +  ( (p_wptr->START_TAG_H) << 16));
	ValueStop = (s32)((p_wptr->STOP_TAG_L) + ( (p_wptr->STOP_TAG_H) << 16));

	//insert current value
	//current = start + (stop - start) * ratio

	transitionParamIn = (ATagValue) * 1024 / totalDuration;//0.10
	
	gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, p_wptr->TimingFun);

	curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);

	p_wptr->OldValueL = (u16)curValue;
	p_wptr->OldValueH = (u16)(curValue >> 16); 
	
	//record current animation by Mr.z
	p_wptr->ANIMATIONCURVALUE_L = (u16)curValue;
	p_wptr->ANIMATIONCURVALUE_H = (u16)(curValue >> 16);

	//render the texture
	renderTexture(&gPagePtr[p_wptr->ATTATCH_PAGE], p_wptr, curValue);

	if(ATagValue >= totalDuration)
		p_wptr->NOW_DURATION &= 0x0000;

	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
//	taskEXIT_CRITICAL();
	
	return AHMI_FUNC_SUCCESS;
}
#endif

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     CanvasClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 3.00 - File Created 2016/04/13 by yuchunying
// Additional Comments:
//   2018/10/25 add comments by zuz
// 
////////////////////////////////////////////////////////////////////////////////
#include "MeterClass.h"
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "drawImmediately_cd.h"
#include "myMathClass.h"

#define MAX_ANGLE_METER 360

#ifdef AHMI_CORE
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
//extern u16  WorkingPageID;

extern QueueHandle_t		ActionInstructionQueue;

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;

extern Easing gEasing;


//-----------------------------
// name�?MeterClass
// construc
//  
//
// addtional comments :
//   none
//-----------------------------
MeterClass::MeterClass()
{

}

//-----------------------------
// name�?~MeterClass
// deconstruct
//  
//
// addtional comments :
//   none
//-----------------------------
MeterClass::~MeterClass()
{

}

//**************************************
// the meter widget
// simple mode or complex mode
// simple mode display two image:
// 0. background
// 1. pointer
// if the pointer contains mask, then 5 additional mask is needed for pointer
// 0. background
// 1. 0 PUREMASK
// 2. PUREMASK for quadrant 0
// 3. PUREMASK for quadrant 1
// 4. PUREMASK for quadrant 2
// 5. PUREMASK for quadrant 3
// 6. pointer
//
//         |
//     2   |   3
// ________|_________
//         |
//     1   |   0
//         |
// complex mode display three images:
// 0. background
// 1. pointer
// 2. lightband
// light band needs 5 puremask, just like pointer mask above
// if pointer contains mask, 5 additional mask is needed for pointer
//
//WidgetAttr�?
//15:  to judge the old value
//14-9:reserved
//8:whether has the mask for pointer
//6:rotation direction�? clockwise�? anti-clockwise
//5:mode�? simple mode�? complex mode
//4-0:type of widget
//**************************************
funcStatus MeterClass::initWidget(
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
		tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
		tagtrigger.mTagPtr = bindTag;
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

funcStatus MeterClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			  
	ActionTriggerClassPtr ActionPtr,  
	u8 u8_pageRefresh				  
	)
{
	s32 ValueStart;
	s32 ValueStop;
	s32 value;
	s32	maxValue;
	s32	minValue;

	if( NULL == p_wptr || NULL == ActionPtr || NULL == ActionPtr->mTagPtr){
		ERROR_PRINT("ERROR: widgetPtr or ActionPtr is NULL");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	if( NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: when drawing DashBoard widght, the texture list corrupt");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	//set the attr value
	value = (s32)(ActionPtr->mTagPtr->getValue());

	//judge parameters
	if( (s16)(p_wptr->WidgetOffsetX) > MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetY) > MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetX) < -MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetY) < -MAX_WIDTH_AND_HEIGHT ||
		p_wptr->WidgetWidth > MAX_WIDTH_AND_HEIGHT ||
		p_wptr->WidgetHeight > MAX_WIDTH_AND_HEIGHT || 
		p_wptr->WidgetWidth == 0 ||  
		p_wptr->WidgetHeight == 0)
	{
		ERROR_PRINT("ERROR: when drawing DashBoard widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}
	if (    p_wptr->MeterMinAngle > MAX_ANGLE_METER ||
		 (s16)p_wptr->MeterMinAngle < -MAX_ANGLE_METER ||
		      p_wptr->MeterMaxAngle > MAX_ANGLE_METER ||
		 (s16)p_wptr->MeterMaxAngle < -MAX_ANGLE_METER ||
		(p_wptr->MeterMaxAngle < p_wptr->MeterMinAngle))
	{		
		ERROR_PRINT("ERROR: when drawing DashBoard widght, the Angle exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	//render the texture 
	if( p_wptr->ANIMATION_DURATION == 0) //no animation
	{
		renderTexture(pagePtr, p_wptr, value);	//set the value of texture from current value
	}
	else if(u8_pageRefresh)	   //animation of page refresh
	{
		//when page refresh,use the value of animation as next value, not according to current value by Mr.z
		//p_wptr->ANIMATIONCURVALUE_L = (u16)( (u32)value );
		//p_wptr->ANIMATIONCURVALUE_H = (u16)(( (u32)value ) >> 16);
		p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
		p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;
		p_wptr->STOP_TAG_L = (u16)( (u32)value );
		p_wptr->STOP_TAG_H = (u16)(( (u32)value ) >> 16);
		renderTexture(pagePtr, p_wptr, value);	   
	}
	else // first frame
	{
		/*if (p_wptr->NOW_DURATION > 0){
			p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
			p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;
		}else{
			p_wptr->START_TAG_L = p_wptr->STOP_TAG_L;
			p_wptr->START_TAG_H = p_wptr->STOP_TAG_H;
		}*/

		maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
		minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);

		if(value < minValue)
			value = minValue;

		if(value > maxValue)
			value = maxValue;

		p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
		p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;

		
		p_wptr->STOP_TAG_L = (u16)( (u32)value );
		p_wptr->STOP_TAG_H = (u16)(( (u32)value ) >> 16);

		/*char text[20];
		sprintf_s(text,"start %d ,stop %d",p_wptr->START_TAG_L,p_wptr->STOP_TAG_L);
		ERROR_PRINT(text);
*/
	}

	ValueStart = (s32)((p_wptr->START_TAG_L) +  ( (p_wptr->START_TAG_H) << 16));
	ValueStop = (s32)((p_wptr->STOP_TAG_L) + ( (p_wptr->STOP_TAG_H) << 16));

	if( (p_wptr->ANIMATION_DURATION != 0) && (ValueStart != ValueStop) ) //create animation
	{
		p_wptr->loadTextureWithAnimation(pagePtr);
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
funcStatus MeterClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			  
	ActionTriggerClassPtr ActionPtr,  
	u8 u8_widgetRefresh				  
	)
{
	DynamicPageClassPtr pCurrentPage;
	s32 value;
	u32 actionAddr;
	u16 oldValueinit = 0;
	s32 oldValue = 0;
	s32 lowAlarmValue ;
	s32 highAlarmValue;
	s32	maxValue;
	s32	minValue;

	if( NULL == p_wptr || NULL == ActionPtr || NULL == ActionPtr->mTagPtr){
		ERROR_PRINT("ERROR: widgetPtr or ActionPtr is NULL");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	if( NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: when drawing DashBoard widght, the texture list corrupt");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	//set the attr value
	oldValueinit = (p_wptr->WidgetAttr & 0x8000);
	lowAlarmValue  = (s32)((p_wptr->LowAlarmValueH  << 16) + p_wptr->LowAlarmValueL );
	highAlarmValue = (s32)((p_wptr->HighAlarmValueH << 16) + p_wptr->HighAlarmValueL);
	value = (s32)(ActionPtr->mTagPtr->getValue());

	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);

	if((value < minValue) || (value > maxValue)){
		ERROR_PRINT("ERROR: Meter widget bindTag value exceeds the boundary");
		u16 CanvasID, SubcanvasID, WidgetID;
		p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
		EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
	}

	//judge parameters
	if( (s16)(p_wptr->WidgetOffsetX) > MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetY) > MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetX) < -MAX_WIDTH_AND_HEIGHT || 
		(s16)(p_wptr->WidgetOffsetY) < -MAX_WIDTH_AND_HEIGHT ||
		p_wptr->WidgetWidth > MAX_WIDTH_AND_HEIGHT ||
		p_wptr->WidgetHeight > MAX_WIDTH_AND_HEIGHT || 
		p_wptr->WidgetWidth == 0 ||  
		p_wptr->WidgetHeight == 0)
	{
		ERROR_PRINT("ERROR: when drawing DashBoard widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}
	if (     p_wptr->MeterMinAngle > MAX_ANGLE_METER ||
		 (s16)p_wptr->MeterMinAngle < -MAX_ANGLE_METER ||
		      p_wptr->MeterMaxAngle > MAX_ANGLE_METER ||
		 (s16)p_wptr->MeterMaxAngle < -MAX_ANGLE_METER ||
		(p_wptr->MeterMaxAngle < p_wptr->MeterMinAngle))
	{		
		ERROR_PRINT("ERROR: when drawing DashBoard widght, the Angle exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
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

	pCurrentPage = pagePtr;
	if(u8_widgetRefresh == 0)
	{

		if(oldValueinit)
		{
			oldValue = (s32)((p_wptr->OldValueH  << 16) + p_wptr->OldValueL);
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
			else if(p_wptr->LeaveLowAlarmAction && oldValue <= lowAlarmValue && value >lowAlarmValue)
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

		//old value refresh
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
//The widget don't support touch action.
//**************************************
funcStatus MeterClass::MouseTouchCtrl(
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

//-----------------------------
// name�?MeterClass::renderTexture
// compute the texture using the tag value
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the tag
// addtional comments :
//   none
//-----------------------------
funcStatus MeterClass::renderTexture
	(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	s32 value
	)
{
	s32	maxValue;
	s32	minValue;
	s16 Angle;
	u8 rotatingDir = (( p_wptr->WidgetAttr & 0x600) >> 9);

	//compute angle
	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);

	if(value > maxValue) 
		value = maxValue;
	if(value < minValue) 
		value = minValue;
	if(maxValue == minValue){
		ERROR_PRINT("ERROR: draw dashboard error, divided by zero");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
	}

	if(rotatingDir == 2)//double direct
	{
		Angle = 16* (value) * ( (s16)(p_wptr->MeterMaxAngle) - (s16)(p_wptr->MeterMinAngle) ) /(maxValue - minValue);//12.4
	}
	else
	{
		Angle = 16* (value - minValue) * ( (s16)(p_wptr->MeterMaxAngle) - (s16)(p_wptr->MeterMinAngle) ) /(maxValue - minValue);//12.4
	}

	//render texture by angle
	renderTextureAngle(
		pagePtr,
		 p_wptr,  
		 Angle
	);
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// name�?MeterClass::renderTextureAngle
// set the texture's angle
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the tag
// addtional comments :
//   none
//-----------------------------
funcStatus MeterClass::renderTextureAngle
		 (
		 DynamicPageClassPtr pagePtr,
		 WidgetClassPtr p_wptr,   
		 s16 Angle
		 )
{
	
	//attr value
	u8 MeterMode = (u8)(( p_wptr->WidgetAttr & 0xC0 ) >> 6);
	u8 rotatingDir = (( p_wptr->WidgetAttr & 0x600) >> 9);
	u8 pointerMask = ( p_wptr->WidgetAttr & 0x100 ) ?1:0;
	u8 PointerPtr;
	u8 PointerAlphaPtr;
	s16 initAngleForPointerAlpha = 45 * 16;
	s16 initAngleForPointer = 45 * 16;
	s16 startAngleForPointer;
	s16 startAngleForPointerAlpha;
	s16 ShearAngleX;				//12.4
	s16 ShearAngleY;
	s16 pureMaskAdjustAngle;     //puremask adjust angle

	s16 RotateAngle;			//12.4
	u16 ScaleX,ScaleY;
	u8 flag; //if flag == 0, the address need to be 0 in order not to display the meter light

	//u16 centralLenght = ((p_wptr->WidgetWidth) * 3)/8;

	myMathClass mymath;
	TextureClassPtr TexturePtr;
	TileBoxClass meterBox;
	u8 index = 0;

	Angle += ((s16)(p_wptr->MeterMinAngle)) << 4;//12.4 relative to min angle

	TexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);
	
	//0 simple mode 1 complex mode 2 extreme simple mode   3 two direction mode
	if(MeterMode == 2)
		PointerPtr = 6;
	else if(MeterMode == 1|| MeterMode == 3)//complex mode
		if(p_wptr->NumOfTex == 8 || p_wptr->NumOfTex == 13){  //computing the pointer ptr by Mr.z
			PointerAlphaPtr = 5;
			PointerPtr = 7;	 
			initAngleForPointerAlpha = mymath.GetAngleFromTan((long long)(((long long)TexturePtr[PointerAlphaPtr].TexHeight << 20) / TexturePtr[PointerAlphaPtr].TexWidth));
		}else if(p_wptr->NumOfTex == 9 || p_wptr->NumOfTex == 14){  //computing the pointer ptr by Mr.z
			PointerAlphaPtr = 6;
			PointerPtr = 8;	 
			initAngleForPointerAlpha = mymath.GetAngleFromTan((long long)(((long long)TexturePtr[PointerAlphaPtr].TexHeight << 20) / TexturePtr[PointerAlphaPtr].TexWidth));
		}else{
			PointerPtr = 8;
		}
	else if(MeterMode == 0){
		if(p_wptr->NumOfTex == 1 || p_wptr->NumOfTex == 6){	  //computing the pointer ptr by Mr.z
			PointerPtr = 0;	 
		}else{
			PointerPtr = 1;
		}
	}
	if(pointerMask)
		PointerPtr += 5;

	//modify initAngleForPointer when PointerTex width != height
	if(TexturePtr[PointerPtr].TexWidth != TexturePtr[PointerPtr].TexHeight)
	{
		initAngleForPointer = mymath.GetAngleFromTan((long long)(((long long)TexturePtr[PointerPtr].TexWidth << 20) / TexturePtr[PointerPtr].TexHeight));
		initAngleForPointer += 0x10;
	}

	//relative to vertical
	if(MeterMode == 2)
	{
		if(rotatingDir == 1)
		{
			startAngleForPointer = (s16)(p_wptr->StartAngle);
		}
		else if(rotatingDir == 0)
		{
			startAngleForPointer = -(s16)(p_wptr->StartAngle);
		}
		else if(rotatingDir == 2)
		{
			if(Angle >= 0)
			{
				startAngleForPointer = (s16)(p_wptr->StartAngle);
			}
			else
			{
				startAngleForPointer = -(s16)(p_wptr->StartAngle);
			}
		}
	}

	else
	{
		if(rotatingDir)
		{
			startAngleForPointer = (s16)(p_wptr->StartAngle) + (initAngleForPointer); //start angle		// 让offsetValue=42，即startAngle=42，这里startAngleForPointer = 42*16 + 45*16 = 1392
			startAngleForPointerAlpha = (s16)(p_wptr->StartAngle) + (initAngleForPointerAlpha);   // 如果PointerAlpha纹理长宽相等，那么startAngleForPointerAlpha = 42*16 + 45*16 = 1392
		}
		else
		{
			startAngleForPointer = -(s16)(p_wptr->StartAngle) + (initAngleForPointer);
			startAngleForPointerAlpha = -(s16)(p_wptr->StartAngle) + (initAngleForPointerAlpha);
		}
	}

	pureMaskAdjustAngle = (s16)(p_wptr->StartAngle) + (((s16)(p_wptr->MeterMinAngle)) << 4); //12.4

	if(rotatingDir)//clockwise
	{
		if(MeterMode == 1|| MeterMode == 3)//complex mode
		{
			if(p_wptr->NumOfTex == 8 || p_wptr->NumOfTex == 13 || p_wptr->NumOfTex == 9 || p_wptr->NumOfTex == 14){
				TexturePtr[PointerAlphaPtr].RotateAngle = Angle + startAngleForPointerAlpha;//add the start angle,//12.4
				TexturePtr[PointerAlphaPtr].mTexAttr |= ABCDEFMATRIX;
				TexturePtr[PointerAlphaPtr].mTexAttr |= USING_PIXEL_RATIO;//pointer needs to be fixed, by zuz20180802
			}
		}

		TexturePtr[PointerPtr].RotateAngle = Angle + startAngleForPointer;//add the start angle,//12.4
		TexturePtr[PointerPtr].mTexAttr |= ABCDEFMATRIX;
		TexturePtr[PointerPtr].mTexAttr |= USING_PIXEL_RATIO;//pointer needs to be fixed, by zuz20180802

		if(rotatingDir == 1)
		{
			Angle = TexturePtr[PointerPtr].RotateAngle - ((s16)(p_wptr->MeterMinAngle) << 4) - startAngleForPointer;// + (45*16);//////point angle relative to start angle
		}
		else if(rotatingDir == 2)
		{
			if(MeterMode == 1|| MeterMode == 3)//complex mode
			{
				if(p_wptr->NumOfTex == 8 || p_wptr->NumOfTex == 13 || p_wptr->NumOfTex == 9 || p_wptr->NumOfTex == 14)
				{
					TexturePtr[PointerAlphaPtr].mTexAttr |= FORCE_UNDISPLAY;
				}
			}
			Angle =   TexturePtr[PointerPtr].RotateAngle + ((s16)(p_wptr->MeterMinAngle)<<4) - startAngleForPointer;// - 45*16;  //assign to rotate anlge
		}
	}
	else if(rotatingDir == 0)//anti-clockwise
	{
		if(MeterMode == 1|| MeterMode == 3)//complex mode
		{
			if(p_wptr->NumOfTex == 8 || p_wptr->NumOfTex == 13 || p_wptr->NumOfTex == 9 || p_wptr->NumOfTex == 14){
				TexturePtr[PointerAlphaPtr].RotateAngle = -Angle + startAngleForPointerAlpha;
				TexturePtr[PointerAlphaPtr].mTexAttr |= ABCDEFMATRIX;
				TexturePtr[PointerAlphaPtr].mTexAttr |= USING_PIXEL_RATIO;//pointer needs to be fixed, by zuz20180802
			}
		}

		TexturePtr[PointerPtr].RotateAngle = -Angle + startAngleForPointer;
		TexturePtr[PointerPtr].mTexAttr |= ABCDEFMATRIX;
		TexturePtr[PointerPtr].mTexAttr |= USING_PIXEL_RATIO;//pointer needs to be fixed, by zuz20180802
		Angle =   TexturePtr[PointerPtr].RotateAngle + ((s16)(p_wptr->MeterMinAngle)<<4) - startAngleForPointer;// - 45*16;  //assign to rotate anlge
	}

	u8 noBackMode = 0;  //none background  edit by lx
	if(p_wptr->NumOfTex == 8 || p_wptr->NumOfTex == 13){
		// none background
		noBackMode = 1;
	}

	if(MeterMode == 1)//complex mode
	{
		if (noBackMode==1){
			//TexturePtr[0].mTexAttr |= TEXTURE_USING_WIDGET_BOX;
		}else {
			//TexturePtr[1].mTexAttr |= TEXTURE_USING_WIDGET_BOX;
		}
		
		if(rotatingDir == 1)//clock wise
		{
			//     |
			//   2 | 3
			//  ---------
			//   1 | 4
			//     |
			for(PointerPtr = 1  ;PointerPtr<5;PointerPtr++)
			{
				if(Angle>= 16 * 90 * (PointerPtr)) //when pointer exceeds the quadrant,this quadrant shows triangle
				{
					ShearAngleX = 0; //donnot shear
					ShearAngleY = 0;
					RotateAngle = 16 * 90 * (PointerPtr );//rotate to current quadrant
					ScaleX = 512;
					ScaleY = 512;
					flag = 1;
				}
				else if(Angle<= 16 * 90 * ((PointerPtr ) - 1 ))//when pointer donnot go to the quadrant, this quadrant donnot show
				{
					ShearAngleX = 0 ;//no shear
					ShearAngleY = 0;
					RotateAngle = 16 * 90 * (PointerPtr);//rotate to current quadrant
					ScaleX = 0xf;//zoom small so that donnot show
					ScaleY = 0xf;
					flag = 0;
				}
				else //in the quadrant
				{
					ShearAngleX = 90*16*(PointerPtr ) - Angle;
					if(ShearAngleX >= 89*16)
						ShearAngleX = 89*16;
					if(ShearAngleX <= -89*16)
						ShearAngleX = -89*16;
					ShearAngleY = 0;
					RotateAngle = 16 * 90 * ((PointerPtr ));//rotate to current quadrant
					ScaleX = 512;
					ScaleY = 512;
					flag = 1;
				}
				
				if(noBackMode==1){
					index = (PointerPtr - 1)%4 + 1;
				}else{
					index = (PointerPtr - 1)%4 + 2;
				}
				TexturePtr[index].RotateAngle = RotateAngle + pureMaskAdjustAngle;
				TexturePtr[index].ShearAngleX = ShearAngleX;
				TexturePtr[index].ShearAngleY = ShearAngleY;
				TexturePtr[index].ScalerX = ScaleX;
				TexturePtr[index].ScalerY = ScaleY;
				if(flag == 0) TexturePtr[index].TexAddr = 0;
				else TexturePtr[index].TexAddr = 0xff000000;
				//TexturePtr[index].renewTextureSourceBox(NULL,NULL,NULL);
			}
		}
		else if(rotatingDir == 0)//anti-clockwise
		{
			//     |
			//   3 | 2
			//  ---------
			//   4 | 1
			//     |
			for(PointerPtr = (1 );PointerPtr<(5 );PointerPtr++)
			{
				if( (s16)Angle <= (s16)(- 16 * 90 * (PointerPtr )) ) //when pointer exceeds the quadrant,this quadrant shows triangle
				{
					ShearAngleX = 0;//donnot shear
					ShearAngleY = 0;//donnot shear
					RotateAngle = - 16 * 90 * (PointerPtr - 1);//rotate to current quadrant
					ScaleX = 512;
					ScaleY = 512;
					flag = 1;
				}
				else if( (s16)Angle>= (s16)(- 16 * 90 * (PointerPtr - 1 ) ))//when pointer donnot go to the quadrant, this quadrant donnot show
				{
					ShearAngleX = 0;//donnot shear
					ShearAngleY = 0;
					RotateAngle = -16 * 90 * (PointerPtr - 1 );//rotate to current quadrant
					ScaleX = 0xf;//zoom small so that donnot show
					ScaleY = 0xf;
					flag = 0;
				}
				else //in the quadrant
				{
					ShearAngleX = 0;
					ShearAngleY = Angle + PointerPtr * 90 * 16;
					if(ShearAngleY >= 89*16)
						ShearAngleY = 89*16;
					if(ShearAngleY <= -89*16)
						ShearAngleY = -89*16;
					RotateAngle = -16 * 90 * (PointerPtr - 1) + ShearAngleX;//rotate to current quadrant
					ScaleX = 512;
					ScaleY = 512;
					flag = 1;
				}

				u8 index = 0;
				if(noBackMode==1){
					index = (PointerPtr - 1)%4 + 1;
				}else{
					index = (PointerPtr - 1)%4 + 2;
				}

				TexturePtr[index].RotateAngle = RotateAngle - pureMaskAdjustAngle;
				TexturePtr[index].ShearAngleX = ShearAngleX;
				TexturePtr[index].ShearAngleY = ShearAngleY;
				TexturePtr[index].ScalerX = ScaleX;
				TexturePtr[index].ScalerY = ScaleY;
				if(flag == 0) TexturePtr[index].TexAddr = 0;
				else TexturePtr[index].TexAddr = 0xff000000;
				//TexturePtr[index].renewTextureSourceBox(NULL,NULL,NULL);
			}
		}
		if(rotatingDir == 2)//double direct
		{
			//     |	                           |
			//   2 | 3						 3 | 2
			//  ---------	for Angle>0;   ---------    for Angle<0
			//   1 | 4						 4 | 1
			//     |						       |
			if(Angle >= 0)
			{
				for(PointerPtr = 1  ;PointerPtr<5;PointerPtr++)
				{
					if(Angle>= 16 * 90 * (PointerPtr)) //when pointer exceeds the quadrant,this quadrant shows triangle
					{
						ShearAngleX = 0; //donnot shear
						ShearAngleY = 0;
						RotateAngle = 16 * 90 * (PointerPtr );//rotate to current quadrant
						ScaleX = 512;
						ScaleY = 512;
						flag = 1;
					}
					else if(Angle<= 16 * 90 * ((PointerPtr ) - 1 ))//when pointer donnot go to the quadrant, this quadrant donnot show
					{
						ShearAngleX = 0 ;//no shear
						ShearAngleY = 0;
						RotateAngle = 16 * 90 * (PointerPtr);//rotate to current quadrant
						ScaleX = 0xf;//zoom small so that donnot show
						ScaleY = 0xf;
						flag = 0;
					}
					else //in the quadrant
					{
						ShearAngleX = 90*16*(PointerPtr ) - Angle;
						if(ShearAngleX >= 89*16)
							ShearAngleX = 89*16;
						if(ShearAngleX <= -89*16)
							ShearAngleX = -89*16;
						ShearAngleY = 0;
						RotateAngle = 16 * 90 * ((PointerPtr ));//rotate to current quadrant
						ScaleX = 512;
						ScaleY = 512;
						flag = 1;
					}
				
					if(noBackMode==1){
						index = (PointerPtr - 1)%4 + 1;
					}else{
						index = (PointerPtr - 1)%4 + 2;
					}
					TexturePtr[index ].RotateAngle = RotateAngle + pureMaskAdjustAngle;
					TexturePtr[index].ShearAngleX = ShearAngleX;
					TexturePtr[index].ShearAngleY = ShearAngleY;
					TexturePtr[index].ScalerX = ScaleX;
					TexturePtr[index].ScalerY = ScaleY;
					if(flag == 0) TexturePtr[index].TexAddr = 0;
					else TexturePtr[index].TexAddr = 0xff000000;
					//TexturePtr[index].renewTextureSourceBox(NULL,NULL,NULL);
				}
			}
			else if(Angle < 0)
			{
				pureMaskAdjustAngle = -(s16)(p_wptr->StartAngle) + (((s16)(p_wptr->MeterMinAngle)) << 4); //12.4
				for(PointerPtr = (1 );PointerPtr<(5 );PointerPtr++)
				{
					if( (s16)Angle <= (s16)(- 16 * 90 * (PointerPtr )) ) //when pointer exceeds the quadrant,this quadrant shows triangle
					{
						ShearAngleX = 0;//donnot shear
						ShearAngleY = 0;//donnot shear
						RotateAngle = - 16 * 90 * (PointerPtr - 1);//rotate to current quadrant
						ScaleX = 512;
						ScaleY = 512;
						flag = 1;
					}
					else if( (s16)Angle>= (s16)(- 16 * 90 * (PointerPtr - 1 ) ))//when pointer donnot go to the quadrant, this quadrant donnot show
					{
						ShearAngleX = 0;//donnot shear
						ShearAngleY = 0;
						RotateAngle = -16 * 90 * (PointerPtr - 1 );//rotate to current quadrant
						ScaleX = 0xf;//zoom small so that donnot show
						ScaleY = 0xf;
						flag = 0;
					}
					else //in the quadrant
					{
						ShearAngleX = 0;
						ShearAngleY = Angle + PointerPtr * 90 * 16;
						if(ShearAngleY >= 89*16)
							ShearAngleY = 89*16;
						if(ShearAngleY <= -89*16)
							ShearAngleY = -89*16;
						RotateAngle = -16 * 90 * (PointerPtr - 1) + ShearAngleX;//rotate to current quadrant
						ScaleX = 512;
						ScaleY = 512;
						flag = 1;
					}

					u8 index = 0;
					if(noBackMode==1){
						index = (PointerPtr - 1)%4 + 1;
					}else{
						index = (PointerPtr - 1)%4 + 2;
					}

					TexturePtr[index].RotateAngle = RotateAngle - pureMaskAdjustAngle;
					TexturePtr[index].ShearAngleX = ShearAngleX;
					TexturePtr[index].ShearAngleY = ShearAngleY;
					TexturePtr[index].ScalerX = ScaleX;
					TexturePtr[index].ScalerY = ScaleY;
					if(flag == 0) TexturePtr[index].TexAddr = 0;
					else TexturePtr[index].TexAddr = 0xff000000;
					//TexturePtr[index].renewTextureSourceBox(NULL,NULL,NULL);
				}
			}
		}
	}
	else if(MeterMode == 2)//extreme simple mode
	{
		//TexturePtr[0].mTexAttr |= TEXTURE_USING_WIDGET_BOX;
		if(rotatingDir)//clock wise
		{
			for(PointerPtr = 1  ;PointerPtr<5;PointerPtr++)
			{
				if(Angle>= 16 * 90 * (PointerPtr)) 
				{
					ShearAngleX = 0;
					ShearAngleY = 0;
					RotateAngle = 16 * 90 * (PointerPtr );
					ScaleX = 512;
					ScaleY = 512;
					flag = 1;
				}
				else if(Angle<= 16 * 90 * ((PointerPtr ) - 1 ))
				{
					ShearAngleX = 0;
					ShearAngleY = 0;
					RotateAngle = 16 * 90 * (PointerPtr);
					ScaleX = 0xf;
					ScaleY = 0xf;
					flag = 0;
				}
				else 
				{
					ShearAngleX = 90*16*(PointerPtr ) - Angle;
					if(ShearAngleX >= 89*16)
						ShearAngleX = 89*16;
					if(ShearAngleX <= -89*16)
						ShearAngleX = -89*16;
					ShearAngleY = 0;
					RotateAngle = 16 * 90 * ((PointerPtr ));
					ScaleX = 512;
					ScaleY = 512;
					flag = 1;
				}
				TexturePtr[(PointerPtr - 1)%4 + 1 ].RotateAngle = RotateAngle + pureMaskAdjustAngle;
				TexturePtr[(PointerPtr - 1)%4 + 1 ].ShearAngleX = ShearAngleX;
				TexturePtr[(PointerPtr - 1)%4 + 1 ].ShearAngleY = ShearAngleY;
				TexturePtr[(PointerPtr - 1)%4 + 1 ].ScalerX = ScaleX;
				TexturePtr[(PointerPtr - 1)%4 + 1 ].ScalerY = ScaleY;
				if(flag == 0) TexturePtr[index].TexAddr = 0;
				else TexturePtr[index].TexAddr = 0xff000000;
				//TexturePtr[(PointerPtr - 1)%4 + 1 ].renewTextureSourceBox(NULL,NULL,NULL);
			}
		}
		else //ani-clockwise
		{
			
			for(PointerPtr = (1 );PointerPtr<(5 );PointerPtr++)
			{
				if( (s16)Angle <= (s16)(- 16 * 90 * (PointerPtr )) ) 
				{
					ShearAngleX = 0;
					ShearAngleY = 0;
					RotateAngle = - 16 * 90 * (PointerPtr - 1);
					ScaleX = 512;
					ScaleY = 512;
					flag = 1;
				}
				else if( (s16)Angle>= (s16)(- 16 * 90 * (PointerPtr - 1 ) ))
				{
					ShearAngleX = 0;
					ShearAngleY = 0;
					RotateAngle = -16 * 90 * (PointerPtr - 1 );
					ScaleX = 0xf;
					ScaleY = 0xf;
					flag = 0;
				}
				else 
				{
					ShearAngleX = Angle + PointerPtr * 90 * 16;
					if(ShearAngleX >= 89*16)
						ShearAngleX = 89*16;
					RotateAngle = -16 * 90 * (PointerPtr - 1) + ShearAngleX;
					if(ShearAngleX >= 89*16)
						ShearAngleX = 89*16;
					ScaleX = 512;
					ScaleY = 512;
					flag = 1;
				}
				TexturePtr[PointerPtr].RotateAngle = RotateAngle - pureMaskAdjustAngle;
				TexturePtr[PointerPtr].ShearAngleX = ShearAngleX;
				TexturePtr[PointerPtr].ScalerX = ScaleX;
				TexturePtr[PointerPtr].ScalerY = ScaleY;
				if(flag == 0) TexturePtr[index].TexAddr = 0;
				else TexturePtr[index].TexAddr = 0xff000000;
				//TexturePtr[PointerPtr].renewTextureSourceBox(NULL,NULL,NULL);
			}
		}
	}

	return AHMI_FUNC_SUCCESS;
}

funcStatus MeterClass::setATag(
	WidgetClassPtr p_wptr,    //widget pointer
	u16 value                 //value of animation tag
	)
{
	s32 ValueStart;
	s32 ValueStop;
	s32 maxValue;
	s32 minValue;
	s32 curValue;
	s16 Angle;
//	s16 startAngle;
//	s16 stopAngle;
	u16 transitionParamIn; //portion of animation progress 0.10
	u16 transitionParamOut; //portion of animation progress after varying function 0.10
#ifndef WHOLE_TRIBLE_BUFFER
	RefreshMsg refreshMsg;
#endif

	u16 totalDuration = p_wptr->ANIMATION_DURATION;
	u8 rotatingDir = (( p_wptr->WidgetAttr & 0x600) >> 9);

	if(totalDuration == 0 ) //no animation
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetAnimationDurationErr);

//	taskENTER_CRITICAL();
#ifdef AHMI_DEBUG
	char text[100];
	sprintf(text,"set meter animation tag %d", value);
	ERROR_PRINT(text);
#endif
	p_wptr->NOW_DURATION |= value;
	ValueStart = (s32)((p_wptr->START_TAG_L) +  ( (p_wptr->START_TAG_H) << 16));
	ValueStop = (s32)((p_wptr->STOP_TAG_L) + ( (p_wptr->STOP_TAG_H) << 16));
	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);

	//char text[100];
	//sprintf(text,"loop: start: %d - stop: %d", ValueStart, ValueStop);
	//ERROR_PRINT(text);

	transitionParamIn = (value) * 1024 / totalDuration;//0.10
	
	gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut,p_wptr->TimingFun);

	curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);

	if(rotatingDir == 2)//double direct
	{
		Angle = 16* (curValue) * ( (s16)(p_wptr->MeterMaxAngle) - (s16)(p_wptr->MeterMinAngle) ) /(maxValue - minValue);//12.4

	}
	else
	{
		Angle = 16* (curValue - minValue) * ( (s16)(p_wptr->MeterMaxAngle) - (s16)(p_wptr->MeterMinAngle) ) /(maxValue - minValue);//12.4
	}
	//Angle = 16 *  (curValue - minValue) * ((s16)(p_wptr->MeterMaxAngle) - (s16)(p_wptr->MeterMinAngle)) / (maxValue - minValue); 

	//Angle = 16 * (value) * ( (s16)(p_wptr->MeterMaxAngle) - (s16)(p_wptr->MeterMinAngle) ) * (ValueStop - ValueStart) /(totalDuration * (maxValue - minValue)) + 16 * (ValueStart) * ( (s16)(p_wptr->MeterMaxAngle) - (s16)(p_wptr->MeterMinAngle) ) / (maxValue - minValue);//12.4

	p_wptr->OldValueL = (u16)curValue;
	p_wptr->OldValueH = (u16)(curValue >> 16); 
	
	//record current animation by Mr.z
	p_wptr->ANIMATIONCURVALUE_L = (u16)curValue;
	p_wptr->ANIMATIONCURVALUE_H = (u16)(curValue >> 16);

	/*sprintf(text,"loop: animation start: %d - stop: %d ..", p_wptr->ANIMATIONCURVALUE_L, p_wptr->ANIMATIONCURVALUE_H);
	ERROR_PRINT(text);*/

	renderTextureAngle(&gPagePtr[p_wptr->ATTATCH_PAGE], p_wptr, Angle);

	if(value >= totalDuration)
		p_wptr->NOW_DURATION &= 0x0000; // set now frame to 0

	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
//	taskEXIT_CRITICAL();
	
	return AHMI_FUNC_SUCCESS;
}
#endif

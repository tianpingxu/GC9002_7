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
// Revision:
// Revision 1.6.2 - Modify 2019/07/19 by YuChunYing
// Additional Comments:
//      ä¿®å¤æ¶éæ§ä»¶ä¸æ´æ°çbug
//   
// 
////////////////////////////////////////////////////////////////////////////////

#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "ClockClass.h"
#include "drawImmediately_cd.h"

#ifdef AHMI_DEBUG
#include "trace.h"
#include "stdio.h"
#endif

#ifdef AHMI_CORE
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;

extern QueueHandle_t		ActionInstructionQueue;

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;

//-----------------------------
// name:ClockClass
// @brief: constructed function
//  
//
// addtional comments :
//   none
//-----------------------------
ClockClass::ClockClass()
{

}

//-----------------------------
// name: ~ClockClass
// 
//  
//
// addtional comments :
//   none
//-----------------------------
ClockClass::~ClockClass()
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
funcStatus ClockClass::initWidget(
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
//class for time and data widget
//the number is transfered by tag.
//the display mode for CLOCK_HOUR_MINUTE_SECOND_MODE is hour:minite:second,specific by tag[23:16]:tag[15:8]:tag[7:0]
//the diplay mode for CLOCK_HOUR_MINUTE_MODE is hour:minite, specific by tag[15:8]:tag[7:0]
//the display mode for CLOCK_GANG_YEAR_MODE is year/month/day,specified by tag[31:8]/tag[15:8]/tag[7:0]
//the display mode for CLOCK_MINUS_YEAR_MODE is year-month-day,specified by tag[31:8]-tag[15:8]-tag[7:0]
//WidgetAttr
//15-7:reserved
//6-5 :mode,could be CLOCK_HOUR_MINUTE_SECOND_MODE / CLOCK_HOUR_MINUTE_MODE/CLOCK_GANG_YEAR_MODE/ CLOCK_MINUS_YEAR_MODE @see definition of the param
//4-0 :the type of widget
//**************************************
funcStatus ClockClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{
	u32 value;
	u32 divValue;
	u16 code = 0;
	u8 displayMode;
	u8 i;
	TextureClassPtr texturePtr;
	DynamicPageClassPtr pCurrentPage;
    
	if(NULL == p_wptr || NULL == ActionPtr || NULL == ActionPtr->mTagPtr){
		ERROR_PRINT("ERROR: widgetPtr or ActionPtr ActionPtr->mTagPtris NULL"); 
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	
	if( NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: when drawing clock widght, the texture list corrupt");
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
		ERROR_PRINT("ERROR: when drawing clock widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	displayMode = (p_wptr-> WidgetAttr >> 5);

	pCurrentPage = pagePtr;
	texturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex+1]);
	//pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].mTexAttr |= TEXTURE_USING_WIDGET_BOX;
	
	//check params
	if((NULL == p_wptr) || (NULL == ActionPtr))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);

	if((texturePtr < (void*)startOfDynamicPage) || (texturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);

	//tag set value
	//the texture's code is ASIC code - 0x20
	//for example, if you want to display ':', you need to set the focusslice as ':' - 0x20
	if(ActionPtr->mInputType == ACTION_TAG_SET_VALUE || u8_pageRefresh == 1)
	{
		value = ActionPtr->mTagPtr->getValue();

		if(displayMode == CLOCK_HOUR_MINUTE_MODE) //hour minite
		{
			value = (value >> 8);
			for(i = 0; i < 4; i++)
			{
				if(i == 2) //display ':'
				{
					code = ':' - 0x20/*offset*/; 
					texturePtr->FocusedSlice = code;
					texturePtr++;
				}
				divValue = value >> (( (3 - i) << 2) ); 
				code = (divValue & 0xf) + 0x30 - 0x20/*basic offset*/;
				texturePtr->FocusedSlice = code;
				texturePtr++;
			}
		}
		else if(displayMode == CLOCK_HOUR_MINUTE_SECOND_MODE)//display hour minite second
		{
			for(i = 0; i < 6; i++)
			{
				//6 number total
				if(i == 2 || i == 4 )//display ':'
				{
					code = ':' - 0x20/*offset*/; 
					texturePtr->FocusedSlice = code;
					texturePtr++;
				}
				divValue = value >> ( (5 - i) << 2); 
				code = (divValue & 0xf) + 0x30 - 0x20/*basic offset*/;
				texturePtr->FocusedSlice = code;
				texturePtr++;
			}
		}
		else if(displayMode == CLOCK_MINUS_YEAR_MODE || displayMode == CLOCK_GANG_YEAR_MODE)//date mode
		{
			for(i = 0; i < 8; i++)//8 numbers in total
			{
				if(i == 4 || i == 6 || i == 8)
				{
					if(displayMode == CLOCK_GANG_YEAR_MODE) // '/'mode
						code = '/' - 0x20/*offset*/;
					else if(displayMode == CLOCK_MINUS_YEAR_MODE)
						code = '-' - 0x20/*offset*/;
					texturePtr->FocusedSlice = code;
					texturePtr++;
				}
				divValue = value >> ( (7 - i) << 2); 
				code = (divValue & 0xf) + 0x30 - 0x20/*basic offset*/;
				texturePtr->FocusedSlice = code;
				texturePtr++;
			}
		}
		else if(displayMode == CLOCK_USA_YEAR_MODE)//date mode
		{
			/* Separator */
			texturePtr[2].FocusedSlice = '-' - 0x20;
			texturePtr[5].FocusedSlice = '-' - 0x20;

			/* Month */
			code = ((value >> 12) & 0xf) + 0x30 - 0x20;
			texturePtr[0].FocusedSlice = code;

			code = ((value >> 8) & 0xf) + 0x30 - 0x20;
			texturePtr[1].FocusedSlice = code;

			/* Day */
			code = ((value >> 4) & 0xf) + 0x30 - 0x20;
			texturePtr[3].FocusedSlice = code;

			code = ((value) & 0xf) + 0x30 - 0x20;
			texturePtr[4].FocusedSlice = code;

			/* Year */
			code = ((value >> 28) & 0xf) + 0x30 - 0x20;
			texturePtr[6].FocusedSlice = code;

			code = ((value >> 24) & 0xf) + 0x30 - 0x20;
			texturePtr[7].FocusedSlice = code;

			code = ((value >> 20) & 0xf) + 0x30 - 0x20;
			texturePtr[8].FocusedSlice = code;

			code = ((value >> 16) & 0xf) + 0x30 - 0x20;
			texturePtr[9].FocusedSlice = code;
		}
		else if(displayMode == CLOCK_EUROPE_YEAR_MODE)//date mode
		{
			/* Separator */
			texturePtr[2].FocusedSlice = '-' - 0x20;
			texturePtr[5].FocusedSlice = '-' - 0x20;

			/* Day */
			code = ((value >> 4) & 0xf) + 0x30 - 0x20;
			texturePtr[0].FocusedSlice = code;

			code = ((value) & 0xf) + 0x30 - 0x20;
			texturePtr[1].FocusedSlice = code;

			/* Month */
			code = ((value >> 12) & 0xf) + 0x30 - 0x20;
			texturePtr[3].FocusedSlice = code;

			code = ((value >> 8) & 0xf) + 0x30 - 0x20;
			texturePtr[4].FocusedSlice = code;

			/* Year */
			code = ((value >> 28) & 0xf) + 0x30 - 0x20;
			texturePtr[6].FocusedSlice = code;

			code = ((value >> 24) & 0xf) + 0x30 - 0x20;
			texturePtr[7].FocusedSlice = code;

			code = ((value >> 20) & 0xf) + 0x30 - 0x20;
			texturePtr[8].FocusedSlice = code;

			code = ((value >> 16) & 0xf) + 0x30 - 0x20;
			texturePtr[9].FocusedSlice = code;
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
//4-0: type of widget, closk is CLOCK_TEX
//**************************************
funcStatus ClockClass::SetValueCtrl(
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
//**************************************
funcStatus ClockClass::MouseTouchCtrl(
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
//widgetCtrl
//15-11: reserved
//11:  whether the button is color button
//10-7: only used in complex button, the number of button. 1-15
//6-5: The mode of button:0 is single button,1 is switch button,2 is complex button
//4-0: type of widget, button is BUTTON_TEX
//**************************************
funcStatus ClockClass::KeyboardTouchCtrl(
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
	u8 numOfChar;
	u8 hour, minute, second;
	u16 year;
	u16 highlightWidth, highlightHeight;
	u8 month, day;
	TextureClassPtr texturePtr;
	DynamicPageClassPtr pCurrentPage;
	u8 curHighlightNum = (u8)(p_wptr->NumOfButtonAndCurHighLight); 
	
	if(NULL == p_wptr || NULL == ActionPtr || NULL == ActionPtr->mTagPtr){
		ERROR_PRINT("ERROR: widgetPtr or ActionPtr ActionPtr->mTagPtris NULL"); 
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	
	if( NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: when drawing clock widght, the texture list corrupt");
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
		ERROR_PRINT("ERROR: when drawing clock widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	pCurrentPage = pagePtr;
	displayMode = (p_wptr-> WidgetAttr >> 5) & 0x3;
	modifyEn = ( p_wptr-> WidgetAttr >> 7) & 0x01;
	texturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex+1]);
	//pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].mTexAttr |= TEXTURE_USING_WIDGET_BOX;

	if(displayMode == CLOCK_HOUR_MINUTE_MODE) //hour minite
	{
		numOfChar = 5;
	}
	else if(displayMode == CLOCK_HOUR_MINUTE_SECOND_MODE) //hour minite second
	{
		numOfChar = 8;
	}
	else //year month day
	{
		numOfChar = 10;
	}

	//check params
	if((NULL == p_wptr) || (NULL == ActionPtr))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);

	if((texturePtr < (void*)startOfDynamicPage) || (texturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);

	//tag set value
	//the texture's code is ASIC code - 0x20
	//for example, if you want to display ':', you need to set the focusslice as ':' - 0x20
    if(ActionPtr->mInputType == ACTION_KEYBOARD_OK)
	{
		if(modifyEn == 0)//enter change mode
		{
			modifyEn = 1;
			p_wptr-> WidgetAttr |= 0x80;
			return AHMI_FUNC_SUCCESS;
		}
		else if(modifyEn == 1) //send the changed value and change it
		{
			modifyEn = 0;
			p_wptr-> WidgetAttr &= ~(0x80);
			if( (p_wptr->OnEnteredAction) < 65535)
			{
				actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->OnEnteredAction);
                csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
			}
		}
	}
	else if(ActionPtr->mInputType == ACTION_KEYBOARD_PRE) //left
	{
		if(modifyEn == 0) //unchanged mode
		{
			if(displayMode == CLOCK_HOUR_MINUTE_MODE) //hour minite
			{
				if(curHighlightNum ==0)
					curHighlightNum = 2;
				else
					curHighlightNum -= 1;
				if(curHighlightNum != 0) //the highlight is on this widget
				{
					texturePtr[numOfChar + 1].mTexAttr &= ~(FORCE_UNDISPLAY); //show highlight
					texturePtr[numOfChar + 1].OffsetX = texturePtr[(curHighlightNum - 1) * 3].OffsetX;
					texturePtr[numOfChar + 1].OffsetY = texturePtr[(curHighlightNum - 1) * 3].OffsetY;
					highlightWidth = (texturePtr[(curHighlightNum - 1) * 3 ].TexWidth) + ((texturePtr[(curHighlightNum - 1) * 3  + 1].OffsetX - texturePtr[(curHighlightNum - 1) * 3 ].OffsetX)>>4);
					highlightHeight = (texturePtr[(curHighlightNum - 1) * 3].TexHeight);
					texturePtr[numOfChar + 1].ScalerX = highlightWidth * 512 / texturePtr[numOfChar + 1 ].TexWidth;
					texturePtr[numOfChar + 1].ScalerY = highlightHeight * 512 / texturePtr[numOfChar + 1 ].TexHeight;
					//texturePtr[numOfChar + 1].renewTextureSourceBox(NULL,NULL,NULL);
					p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
				}
				else //highlight moves away
				{
					texturePtr[numOfChar + 1].mTexAttr |= (FORCE_UNDISPLAY); //trun off the highlight
					curHighlightNum = 0; //current highlight count becomes 0
					(p_wptr->NumOfButtonAndCurHighLight) = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
				}
			}
			else if(displayMode == CLOCK_HOUR_MINUTE_SECOND_MODE)//hour minute second mode
			{
				if(curHighlightNum == 0)
					curHighlightNum = 3;
				else 
					curHighlightNum -= 1;
				if(curHighlightNum != 0) //highlight is still on
				{
					texturePtr[numOfChar + 1].mTexAttr &= ~(FORCE_UNDISPLAY); //turn on highlight
					texturePtr[numOfChar + 1].OffsetX =    texturePtr[(curHighlightNum - 1) * 3 ].OffsetX;
					texturePtr[numOfChar + 1].OffsetY =    texturePtr[(curHighlightNum - 1) * 3 ].OffsetY;
					highlightWidth = (texturePtr[(curHighlightNum - 1) * 3 ].TexWidth) + ((texturePtr[(curHighlightNum - 1) * 3  + 1].OffsetX - texturePtr[(curHighlightNum - 1) * 3 ].OffsetX)>>4);
					highlightHeight = (texturePtr[(curHighlightNum - 1) * 3 ].TexHeight);
					texturePtr[numOfChar + 1].ScalerX = highlightWidth * 512 / texturePtr[numOfChar + 1 ].TexWidth;
					texturePtr[numOfChar + 1].ScalerY = highlightHeight * 512 / texturePtr[numOfChar + 1 ].TexHeight;
					//texturePtr[numOfChar + 1].renewTextureSourceBox(NULL,NULL,NULL);
					p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
				}
				else //highlight moves away
				{
					texturePtr[numOfChar + 1].mTexAttr |= (FORCE_UNDISPLAY); //highlight turn off
					curHighlightNum = 0; //current highlight count becomes 0
					(p_wptr->NumOfButtonAndCurHighLight) = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
				}
			}
			else //date mode
			{
				if(curHighlightNum == 0)
					curHighlightNum = 3;
				else 
					curHighlightNum -= 1;
				if(curHighlightNum == 1) //highlight on year
				{
					texturePtr[numOfChar + 1].mTexAttr &= ~(FORCE_UNDISPLAY); //turn on highlight
					texturePtr[numOfChar + 1].OffsetX =    texturePtr[0].OffsetX;
					texturePtr[numOfChar + 1].OffsetY =    texturePtr[0].OffsetY;
					highlightWidth = (texturePtr[0].TexWidth) +  ((texturePtr[3].OffsetX -  texturePtr[0].OffsetX)>>4);
					highlightHeight = (texturePtr[0].TexHeight);
					texturePtr[numOfChar + 1].ScalerX = highlightWidth * 512 / texturePtr[numOfChar + 1 ].TexWidth;
					texturePtr[numOfChar + 1].ScalerY = highlightHeight * 512 / texturePtr[ numOfChar + 1 ].TexHeight;
					//texturePtr[numOfChar + 1].renewTextureSourceBox(NULL,NULL,NULL);
					p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
				}
				else if(curHighlightNum == 2)//highlight on month
				{
					texturePtr[numOfChar + 1].mTexAttr &= ~(FORCE_UNDISPLAY); //turn on highlight
					texturePtr[numOfChar + 1].OffsetX =    texturePtr[5].OffsetX;
					texturePtr[numOfChar + 1].OffsetY =    texturePtr[5].OffsetY;
					highlightWidth = (texturePtr[5].TexWidth) +  ((texturePtr[6].OffsetX -  texturePtr[5].OffsetX)>>4);
					highlightHeight = (texturePtr[5].TexHeight);
					texturePtr[numOfChar + 1].ScalerX = highlightWidth * 512 / texturePtr[numOfChar + 1].TexWidth;
					texturePtr[numOfChar + 1].ScalerY = highlightHeight * 512 / texturePtr[numOfChar + 1].TexHeight;
					//texturePtr[numOfChar + 1].renewTextureSourceBox(NULL,NULL,NULL);
					p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
				}
				else if(curHighlightNum == 3)//highlight on day
				{
					texturePtr[numOfChar + 1].mTexAttr &= ~(FORCE_UNDISPLAY); //turn on highlight
					texturePtr[numOfChar + 1].OffsetX =    texturePtr[8].OffsetX;
					texturePtr[numOfChar + 1].OffsetY =    texturePtr[8].OffsetY;
					highlightWidth = (texturePtr[5].TexWidth) +  ((texturePtr[6].OffsetX -  texturePtr[5].OffsetX)>>4);
					highlightHeight = (texturePtr[5].TexHeight);
					texturePtr[numOfChar + 1].ScalerX = highlightWidth * 512 / texturePtr[numOfChar + 1].TexWidth;
					texturePtr[numOfChar + 1].ScalerY = highlightHeight * 512 / texturePtr[numOfChar + 1].TexHeight;
					//texturePtr[numOfChar + 1].renewTextureSourceBox(NULL,NULL,NULL);
					p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
				}
				else //highlight goes away
				{
					texturePtr[numOfChar + 1].mTexAttr |= (FORCE_UNDISPLAY); //turn off highlight
					curHighlightNum = 0; //current highlight count becomes 0
					(p_wptr->NumOfButtonAndCurHighLight) = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
				}
			}
		}
		else if(modifyEn) //modify mode
		{
			if(displayMode == CLOCK_HOUR_MINUTE_MODE || displayMode == CLOCK_HOUR_MINUTE_SECOND_MODE) //hour minute mode
			{
				//get hour minite second value
				value = TagPtr->getTag(p_wptr->BindTagID).getValue();
				hour = (u8)(value >> 16);
				minute = (u8)(value >> 8);
				second = (u8)(value);
				hour = (hour >> 4) * 10 + (hour & 0xf);
				minute = (minute >> 4) * 10 + (minute & 0xf);
				second = (second >> 4) * 10 + (second & 0xf);
				//modify hour minute second
				if(curHighlightNum == 1)//decrese hour
				{
					if(hour > 0)
					{
						hour--;
					}
					else if(hour == 0)
					{
						hour = 23;
					}
				}
				else if(curHighlightNum == 2) //decrese minute
				{
					if(minute > 0)
						minute --;
					else if(minute == 0)
					{
						minute = 59;
						if(hour > 0)
							hour--;
						else if(hour == 0)
							hour = 23;
					}
				}
				else if(curHighlightNum == 3) //decrese second
				{
					if(second > 0)
						second --;
					else if(second == 0)
					{
						second = 59;
						if(minute == 0 && hour == 0)
						{
							minute = 59;
							hour = 23;
						}
						else if(minute == 0)
						{
							minute = 59;
							hour --;
						}
						else
							minute --;
					}
				}
				//recompute tag
				hour = ((hour/10) << 4)  + (hour % 10);
				minute = ((minute/10) << 4)  + (minute % 10);
				second = ((second/10) << 4)  + (second % 10);
				value = (hour << 16) + (minute << 8) + second;
				if(displayMode == CLOCK_HOUR_MINUTE_MODE)
				{
					for(i = 0; i < 4; i++)
					{
						if(i == 2) //display ':'
						{
							code = ':' - 0x20/*offset*/; 
							texturePtr->FocusedSlice = code;
							texturePtr++;
						}
						divValue = value >> (( (5 - i) << 2)); 
						code = (divValue & 0xf) + 0x30 - 0x20/*basic offset*/;
						texturePtr->FocusedSlice = code;
						texturePtr++;
					}
				}
				else if(displayMode == CLOCK_HOUR_MINUTE_SECOND_MODE)
				{
					for(i = 0; i < 6; i++)
					{
						//6 number in total
						if(i == 2 || i == 4 )//display ':'
						{
							code = ':' - 0x20/*offset*/; 
							texturePtr->FocusedSlice = code;
							texturePtr++;
						}
						divValue = value >> ( (5 - i) << 2); 
						code = (divValue & 0xf) + 0x30 - 0x20/*basic offset*/;
						texturePtr->FocusedSlice = code;
						texturePtr++;
					}
				}
			}
			else //hour month day mode
			{
				//get the date value
				value = TagPtr->getTag(p_wptr->BindTagID).getValue();
				year = (u16)(value >> 16);
				month = (u8)(value >> 8);
				day = (u8)(value);
				year = (year >> 12) * 1000 + ( (year >> 8) & 0xf) * 100 + (((year >> 4) & 0xf) * 10) + (year & 0xf);
				month = (month >> 4) * 10 + (month & 0xf);
				day = (day >> 4) * 10 + (day & 0xf);
				//modify
				if(curHighlightNum == 1)//decrese year
				{
					if(year > 0)
					{
						//leap year
						if( (year % 4 == 0 && year % 100 != 0) || ( year % 400 == 0) )
						{
							if(month == 2 && day == 29)
							{
								month = 3;
								day = 1;
							}
						}
						year--;
					}
					else if(year == 0)
					{
						year = 9999;
					}
				}
				else if(curHighlightNum == 2) //decrese month
				{
					if(month > 1)
					{
						switch (month)
						{
						case(5):
						case(7):
						case(8):
						case(10):
							if(day == 31)
							{
								//month += 2;
								day = 1;
							}
							else 
								month--;
							break;
						case(3): //from March to February
							if( (year % 4 == 0 && year % 100 != 0) || ( year % 400 == 0) )
							{
								hour = 29;
							}
							else 
								hour = 28;
							if(day > hour)
							{
								month = 3;
								day = day - hour;
							}
							else
								month--;
							break;
						default:
							month --;
							break;
						}
					}
					else if(month == 1)
					{
						month = 12;
						if(year > 0)
						{
							year--;
						}
						else if(year == 0)
						{
							year = 9999;
						}
					}
				}
				else if(curHighlightNum == 3) //decrese day
				{
					if(day > 1)
						day --;
					else if(day == 1)
					{
						if(month == 1 && year == 0)
						{
							year = 9999;
							month = 12;
							day = 31;
						}
						else if(month == 1)
						{
							year --;
							month = 12;
							day = 31;
						}
						else 
						{
							month --;
							switch( month )
							{
							case(1):
							case(3):
							case(5):
							case(7):
							case(8):
							case(10):
							case(12):
								hour = 31; //limit of day
								break;
							case(2):
								//leap year judge
								if( (year % 4 == 0 && year % 100 != 0) || (year % 100 == 0 && year % 400 == 0) )
								{
									hour = 29;
								}
								else 
									hour = 28;
								break;
							default:
								hour = 30;
							}
							day = hour;
						}
					}
				}

				//recompute tag
				year = ((year/1000) << 12) + ( ((year/100) % 10) << 8) + ( ((year/10) % 10) << 4) + (year % 10);
				month = ((month/10) << 4)  + (month % 10);
				day = ((day/10) << 4)  + (day % 10);
				value = (year << 16) + (month << 8) + day;

				//recompute texture
				for(i = 0; i < 8; i++)//8 number in total
				{
					if(i == 4 || i == 6 || i == 8)
					{
						if(displayMode == CLOCK_GANG_YEAR_MODE) // '/'mode
							code = '/' - 0x20/*offset*/;
						else if(displayMode == CLOCK_MINUS_YEAR_MODE)
							code = '-' - 0x20/*offset*/;
						texturePtr->FocusedSlice = code;
						texturePtr++;
					}
					divValue = value >> ( (7 - i) << 2); 
					code = (divValue & 0xf) + 0x30 - 0x20/*basic offset*/;
					texturePtr->FocusedSlice = code;
					texturePtr++;
				}
			}
#ifdef AHMI_DEBUG
			char text[100];
			sprintf(text,"setting clock to value %x", value);
			ERROR_PRINT(text);
#endif
			ActionPtr->mTagPtr->updateTagValueToValueSpace(value);
		}
	}
	else if(ActionPtr->mInputType == ACTION_KEYBOARD_NEXT) //right
	{
		if(modifyEn == 0) //unmodify
		{
			if(displayMode == CLOCK_HOUR_MINUTE_MODE)
			{
				if(curHighlightNum == 2)
					curHighlightNum = 0;
				else 
					curHighlightNum += 1;
				if(curHighlightNum != 0) //highlight not move away
				{
					texturePtr[numOfChar + 1].mTexAttr &= ~(FORCE_UNDISPLAY); //highlight on
					texturePtr[numOfChar + 1].OffsetX = texturePtr[(curHighlightNum - 1) * 3].OffsetX;
					texturePtr[numOfChar + 1].OffsetY = texturePtr[(curHighlightNum - 1) * 3].OffsetY;
					highlightWidth = (texturePtr[(curHighlightNum - 1) * 3 ].TexWidth) + ((texturePtr[(curHighlightNum - 1) * 3  + 1].OffsetX - texturePtr[(curHighlightNum - 1) * 3 ].OffsetX)>>4);
					highlightHeight = (texturePtr[(curHighlightNum - 1) * 3].TexHeight);
					texturePtr[numOfChar + 1].ScalerX = highlightWidth * 512 / texturePtr[numOfChar + 1].TexWidth;
					texturePtr[numOfChar + 1].ScalerY = highlightHeight * 512 / texturePtr[numOfChar + 1].TexHeight;
					//texturePtr[numOfChar + 1].renewTextureSourceBox(NULL,NULL,NULL);
					p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
				}
				else //highlight move away
				{
					texturePtr[numOfChar + 1].mTexAttr |= (FORCE_UNDISPLAY); //highlight off
					curHighlightNum = 0; 
					(p_wptr->NumOfButtonAndCurHighLight) = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
				}
			}
			else if(displayMode == CLOCK_HOUR_MINUTE_SECOND_MODE)//hour minute second
			{
				if(curHighlightNum == 3)
					curHighlightNum = 0;
				else 
					curHighlightNum += 1;
				if(curHighlightNum != 0) //highlight not move away
				{
					texturePtr[numOfChar + 1].mTexAttr &= ~(FORCE_UNDISPLAY); //highlight on
					texturePtr[numOfChar + 1].OffsetX =    texturePtr[(curHighlightNum - 1) * 3 ].OffsetX;
					texturePtr[numOfChar + 1].OffsetY =    texturePtr[(curHighlightNum - 1) * 3 ].OffsetY;
					highlightWidth = (texturePtr[(curHighlightNum - 1) * 3 ].TexWidth) + ((texturePtr[(curHighlightNum - 1) * 3  + 1].OffsetX - texturePtr[(curHighlightNum - 1) * 3 ].OffsetX)>>4);	
					highlightHeight = (texturePtr[(curHighlightNum - 1) * 3 ].TexHeight) ;
					texturePtr[numOfChar + 1].ScalerX = highlightWidth * 512 / texturePtr[numOfChar + 1].TexWidth;
					texturePtr[numOfChar + 1].ScalerY = highlightHeight * 512 / texturePtr[numOfChar + 1].TexHeight;
					//texturePtr[numOfChar + 1].renewTextureSourceBox(NULL,NULL,NULL);
					p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
				}
				else //highlight move away
				{
					texturePtr[numOfChar + 1].mTexAttr |= (FORCE_UNDISPLAY); //highlight off
					curHighlightNum = 0; 
					(p_wptr->NumOfButtonAndCurHighLight) = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
				}
			}
			else //data mode
			{
				if(curHighlightNum == 3)
					curHighlightNum = 0;
				else 
					curHighlightNum += 1;
				if(curHighlightNum == 1)  //highlight on year
				{
					texturePtr[numOfChar + 1].mTexAttr &= ~(FORCE_UNDISPLAY); //highlight on
					texturePtr[numOfChar + 1].OffsetX =    texturePtr[0].OffsetX;
					texturePtr[numOfChar + 1].OffsetY =    texturePtr[0].OffsetY;
					highlightWidth = (texturePtr[0].TexWidth) +  ((texturePtr[3].OffsetX -  texturePtr[0].OffsetX)>>4);	
					highlightHeight = (texturePtr[0].TexHeight) ;
					texturePtr[numOfChar + 1].ScalerX = highlightWidth * 512 / texturePtr[numOfChar + 1].TexWidth;
					texturePtr[numOfChar + 1].ScalerY = highlightHeight * 512 / texturePtr[numOfChar + 1].TexHeight;
					//texturePtr[numOfChar + 1].renewTextureSourceBox(NULL,NULL,NULL);
					p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
				}
				else if(curHighlightNum == 2)//highlight on month
				{
					texturePtr[numOfChar + 1].mTexAttr &= ~(FORCE_UNDISPLAY); //highlight on
					texturePtr[numOfChar + 1].OffsetX =    texturePtr[5].OffsetX;
					texturePtr[numOfChar + 1].OffsetY =    texturePtr[5].OffsetY;
					highlightWidth = (texturePtr[5].TexWidth) +  ((texturePtr[6].OffsetX -  texturePtr[5].OffsetX)>>4);
					highlightHeight = (texturePtr[5].TexHeight) ;
					texturePtr[numOfChar + 1].ScalerX = highlightWidth * 512 / texturePtr[numOfChar + 1].TexWidth;
					texturePtr[numOfChar + 1].ScalerY = highlightHeight * 512 / texturePtr[numOfChar + 1 ].TexHeight;
					//texturePtr[numOfChar + 1].renewTextureSourceBox(NULL,NULL,NULL);
					p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
				}
				else if(curHighlightNum == 3) //highlight on day
				{
					texturePtr[numOfChar + 1].mTexAttr &= ~(FORCE_UNDISPLAY); //highlight on
					texturePtr[numOfChar + 1].OffsetX =    texturePtr[8].OffsetX;
					texturePtr[numOfChar + 1].OffsetY =    texturePtr[8].OffsetY;
					highlightWidth = (texturePtr[8].TexWidth) +  ((texturePtr[9].OffsetX -  texturePtr[8].OffsetX)>>4);
					highlightHeight = (texturePtr[8].TexHeight) ;
					texturePtr[numOfChar + 1].ScalerX = highlightWidth * 512 / texturePtr[numOfChar + 1].TexWidth;
					texturePtr[numOfChar + 1].ScalerY = highlightHeight * 512 / texturePtr[numOfChar + 1].TexHeight;
					//texturePtr[numOfChar + 1].renewTextureSourceBox(NULL,NULL,NULL);
					p_wptr->NumOfButtonAndCurHighLight = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
				}
				else //highlight moves away
				{
					texturePtr[numOfChar + 1].mTexAttr |= (FORCE_UNDISPLAY); //highlight off
					curHighlightNum = 0; 
					(p_wptr->NumOfButtonAndCurHighLight) = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
				}
			}
		}
		else if(modifyEn) //modify mode
		{
			//change value of tag
			if(displayMode == CLOCK_HOUR_MINUTE_MODE || displayMode == CLOCK_HOUR_MINUTE_SECOND_MODE) //time mode
			{
				//get value
				value = TagPtr->getTag(p_wptr->BindTagID).getValue();
				hour = (u8)(value >> 16);
				minute = (u8)(value >> 8);
				second = (u8)(value);
				hour = (hour >> 4) * 10 + (hour & 0xf);
				minute = (minute >> 4) * 10 + (minute & 0xf);
				second = (second >> 4) * 10 + (second & 0xf);
				//modify hour minite
				if(curHighlightNum == 1)//add hour
				{
					if(hour < 23)
					{
						hour++;
					}
					else if(hour == 23)
					{
						hour = 0;
					}
				}
				else if(curHighlightNum == 2) //add minute 
				{
					if(minute < 59)
						minute ++;
					else if(minute == 59)
					{
						minute = 0;
						if(hour < 23)
						{
							hour++;
						}
						else if(hour == 23)
						{
							hour = 0;
						}
					}
				}
				else if(curHighlightNum == 3) //add second
				{
					if(second < 59)
						second ++;
					else if(second == 59)
					{
						second = 0;
						if(minute == 59 && hour == 23)
						{
							minute = 0;
							hour = 0;
						}
						else if(minute == 59)
						{
							hour++;
							minute = 0;
						}
						else
							minute ++;
					}
				}
				//recompute tag
				hour = ((hour/10) << 4)  + (hour % 10);
				minute = ((minute/10) << 4)  + (minute % 10);
				second = ((second/10) << 4)  + (second % 10);
				value = (hour << 16) + (minute << 8) + second;
				if(displayMode == CLOCK_HOUR_MINUTE_MODE)
				{
					for(i = 0; i < 4; i++)
					{
						if(i == 2) //display ':'
						{
							code = ':' - 0x20/*offset*/; 
							texturePtr->FocusedSlice = code;
							texturePtr++;
						}
#if 0
						divValue = value >> (( (3 - i) << 2) + 2); 
#else
						divValue = value >> ( (5 - i) << 2); 
#endif
						code = (divValue & 0xf) + 0x30 - 0x20/*basic offset*/;
						texturePtr->FocusedSlice = code;
						texturePtr++;
					}
				}
				else if(displayMode == CLOCK_HOUR_MINUTE_SECOND_MODE)
				{
					for(i = 0; i < 6; i++)
					{
						//6 number
						if(i == 2 || i == 4 )//display ':'
						{
							code = ':' - 0x20/*offset*/; 
							texturePtr->FocusedSlice = code;
							texturePtr++;
						}
						divValue = value >> ( (5 - i) << 2); 
						code = (divValue & 0xf) + 0x30 - 0x20/*offset*/;
						texturePtr->FocusedSlice = code;
						texturePtr++;
					}
				}
			}
			else //date mode
			{
				//get date
				value = TagPtr->getTag(p_wptr->BindTagID).getValue();
				year = (u16)(value >> 16);
				month = (u8)(value >> 8);
				day = (u8)(value);
				year = (year >> 12) * 1000 + ((year >> 8) & 0xf) * 100 + (((year >> 4) & 0xf) * 10) + (year & 0xf);
				month = (month >> 4) * 10 + (month & 0xf);
				day = (day >> 4) * 10 + (day & 0xf);
				//modify date
				if(curHighlightNum == 1)//add year
				{
					if(year < 9999)
					{
						//leap year
						if( (year % 4 == 0 && year % 100 != 0) || ( year % 400 == 0) )
						{
							if(month == 2 && day == 29)
							{
								month = 3;
								day = 1;
							}
						}
						year++;
					}
					else if(year == 9999)
					{
						year = 0;
					}
				}
				else if(curHighlightNum == 2) //add month
				{
					if(month < 12)
					{
						switch (month)
						{
						case(3):
						case(5):
						case(7):
						case(8):
						case(10):
							if(day == 31)
							{
								month += 2;
								day = 1;
							}
							else 
								month++;
							break;
						case(1): //from Junary to Bebury
							if( (year % 4 == 0 && year % 100 != 0) || ( year % 400 == 0) )
							{
								hour = 29;
							}
							else 
								hour = 28;
							if(day > hour)
							{
								month = 3;
								day = day - hour;
							}
							else 
								month ++;
							break;
						default:
							month ++;
							break;
						}
						
					}
					else if(month == 12)
					{
						month = 1;
						if(year < 9999)
						{
							year++;
						}
						else if(year == 9999)
						{
							year = 0;
						}
					}
				}
				else if(curHighlightNum == 3) //add day
				{
					switch(month)
					{
					case(1):
					case(3):
					case(5):
					case(7):
					case(8):
					case(10):
					case(12):
						hour = 31; //limit of day
						break;
					case(2):
						//leap year
						if( (year % 4 == 0 && year % 100 != 0) || (year % 100 == 0 && year % 400 == 0) )
						{
							hour = 29;
						}
						else 
							hour = 28;
						break;
					default:
						hour = 30;
					}
					if(day < hour)
						day ++;
					else if(day == hour)
					{
						day = 1;
						if(month == 12)
						{
							month = 1;
							if(year == 9999)
								year = 0;
							else 
								year ++;
						}
						else
							month ++;
					}
				}

				//recompute tag
				year = ((year/1000) << 12) + ( ((year/100) % 10) << 8) + ( ((year/10) % 10) << 4) + (year % 10);
				month = ((month/10) << 4)  + (month % 10);
				day = ((day/10) << 4)  + (day % 10);
				value = (year << 16) + (month << 8) + day;

				//recompute texture value
				for(i = 0; i < 8; i++)//total 8 numbers
				{
					if(i == 4 || i == 6 || i == 8)
					{
						if(displayMode == CLOCK_GANG_YEAR_MODE) //'/' mode
							code = '/' - 0x20/*offset*/;
						else if(displayMode == CLOCK_MINUS_YEAR_MODE)
							code = '-' - 0x20/*offset*/;
						texturePtr->FocusedSlice = code;
						texturePtr++;
					}
					divValue = value >> ( (7 - i) << 2); 
					code = (divValue & 0xf) + 0x30 - 0x20/*offset*/;
					texturePtr->FocusedSlice = code;
					texturePtr++;
				}
			}
#ifdef AHMI_DEBUG
			char text[100];
			sprintf(text,"setting clock to value %x", value);
			ERROR_PRINT(text);
#endif
			ActionPtr->mTagPtr->updateTagValueToValueSpace(value);
		}
	}//end of keyboard next
	else if(ActionPtr->mInputType == ACTION_KEYBOARD_CLEAR)
	{
		texturePtr[numOfChar + 1].mTexAttr |= (FORCE_UNDISPLAY); //turn off highlight
		curHighlightNum = 0; 
		p_wptr-> WidgetAttr &= 0xff7f; //exit the modify mode
		(p_wptr->NumOfButtonAndCurHighLight) = ((p_wptr->NumOfButtonAndCurHighLight) & 0xff00) + curHighlightNum;
	}
    
    p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// nameï¼? setColor
// set the purecolor color
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u8 r             //the value of the red color
//  @param	 u8 g             //the value of the red color
//  @param	 u8 b             //the value of the red color
//  @param	 u8 a             //the value of the red color
// addtional comments :
//   none
//-----------------------------
funcStatus ClockClass::setColor(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr, 
	u8 r,
	u8 g,
	u8 b,
	u8 a
)
{
	TextureClassPtr colorTexturePtr; 
	u32 color;
	u8 displayMode;
	u8 numOfChar;
	
	color = (a<<24) + (r<<16) + (g<<8) + b;

	//get color texture;
	displayMode = (p_wptr-> WidgetAttr >> 5) & 0x3;

	if(displayMode == CLOCK_HOUR_MINUTE_MODE) //hour minite
	{
		numOfChar = 5;
	}
	else if(displayMode == CLOCK_HOUR_MINUTE_SECOND_MODE) //hour minite second
	{
		numOfChar = 8;
	}
	else //year month day
	{
		numOfChar = 10;
	}
	colorTexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + numOfChar + 1]); 

	colorTexturePtr->TexAddr = color;
	
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	
	return AHMI_FUNC_SUCCESS;
}
#endif

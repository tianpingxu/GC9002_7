////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2022/11/8
// File Name:     DataImage.cpp
// Project Name:  AHMISimulator
// 
// 
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "DataImage.h"
#include "drawImmediately_cd.h"
#include "myMathClass.h"
#include "WriteTexture.h"

#ifdef AHMI_CORE
//extern ActionTriggerClass		gWidgetTagTrigger;
//extern ActionTriggerClass		gMouseTagTrigger;
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
//extern u16  WorkingPageID;



extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;


//-----------------------------
// name DataImageClass
// construct
//  
//
// addtional comments :
//   none
//-----------------------------
DataImageClass::DataImageClass()
{

}

//-----------------------------
// name ~DataImageClass
// deconstruct
//  
//
// addtional comments :
//   none
//-----------------------------
DataImageClass::~DataImageClass()
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
funcStatus DataImageClass::initWidget(
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
	#ifndef CHIP_GC9002_NoHyper
	TagClassPtr bindTag;
	ActionTriggerClass tagtrigger;
	WidgetClassInterface myWidgetClassInterface;
	#if defined(EMBEDDED) && defined(VERSION_2)
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
	#ifdef VERSION_2
	if(myWidgetClassInterface.drawDataImageTexture(pagePtr, p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn) != AHMI_FUNC_SUCCESS)
	#endif	
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);
		
	return AHMI_FUNC_SUCCESS;
	#endif
	
	#if defined(EMBEDDED) && defined(VERSION_2)
	Op_sourceShift = *u32p_sourceShift;
	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetSourceBufferRefreshFlag) || (RefreshType == ANIMATION_MATRIXREFRESH_PAGE) || (gScreenOptions.enable))
	{
		status = myWidgetClassInterface.drawDataImageTexture(pagePtr, p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn);
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
	#endif //endof #ifndef CHIP_GC9002_NoHyper
	
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
funcStatus DataImageClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,		
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh			
	)
{	
#ifdef VERSION_2
	#ifndef CHIP_GC9002_NoHyper
	u8* lineData;
	u16 mode, refreshMode, dataAlign, id, lineID, lineDataBytes, dataByteLen;
	TagClassPtr curTag;
	
	id = p_wptr->DATAIMG_ID;
		
	if(u8_pageRefresh)
	{
		initDataImageTextureRAM(id,p_wptr->WidgetWidth, p_wptr->WidgetHeight);
	}

	if(p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag)
	{
		//get new data
		curTag = ActionPtr->mTagPtr;
		lineData = curTag->getArrayValue();
		dataByteLen = curTag->getElemByteLength();
		lineDataBytes = curTag->getCurArryElemNum() * dataByteLen;
		
		mode = p_wptr->DATAIMG_MODE; //0:rgba8888 1: rgb565
		if(mode)
			dataByteLen = 2;
		else
			dataByteLen = 4;
		refreshMode = p_wptr->DATAIMG_REFRESH_MODE; //0:push 1:cycle
		dataAlign = p_wptr->DATAIMG_DATA_ALIGN;//0:column 1:row
		
		//if (refreshMode == 1)
		if (1)
		{
			//cycle
			lineID = p_wptr->DATAIMG_LINE_IDX;
			if (dataAlign == 1)
			{
				if (lineID >= p_wptr->WidgetHeight)
				{
					lineID = 0;
				}
			}
			else
			{
				if (lineID >= p_wptr->WidgetWidth)
				{
					lineID = 0;
				}
			}
		}
		else
		{
			//push
			if (dataAlign == 1)
			{
				lineID = p_wptr->WidgetHeight-1;
			}
			else
			{
				lineID = p_wptr->WidgetWidth-1;
			}
		}
		//lineID = p_wptr->DATAIMG_LINE_IDX;
		//update line data
		updataDataImageTexture(id,lineData, lineDataBytes, dataByteLen, mode,lineID, p_wptr->WidgetWidth, p_wptr->WidgetHeight, dataAlign, refreshMode==0?1:0);
		
		//if (refreshMode == 1)
		if (1)
		{
			lineID += 1;
			p_wptr->DATAIMG_LINE_IDX = lineID;
		}
	}
	#endif //endof #ifndef CHIP_GC9002_NoHyper
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
funcStatus DataImageClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    u8 u8_widgetRefresh      
    )
{
	#ifndef CHIP_GC9002_NoHyper
	//param check
	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	
	p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	#endif //endof #ifndef CHIP_GC9002_NoHyper
	
	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Mouse.Touch
//The widget don't support touch action.
//**************************************
funcStatus DataImageClass::MouseTouchCtrl(
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
	return AHMI_FUNC_SUCCESS;
}
#endif

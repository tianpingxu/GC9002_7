////////////////////////////////////////////////////////////////////////////////
//bar chart
//created by Zzen1sS
//2023/5/9
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "BarChart.h"
#include "drawImmediately_cd.h"

#ifdef AHMI_CORE
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
extern u16 WorkingPageID;

extern QueueHandle_t		ActionInstructionQueue;

//-----------------------------
// name£º DynamicTexClass
// initialize
//  
//
// addtional comments :
//   none
//-----------------------------
BarChartClass::BarChartClass()
{

}

//-----------------------------
// name£º ~DynamicTexClass
// deconstruct
//  
//
// addtional comments :
//   none
//-----------------------------
BarChartClass::~BarChartClass()
{

}

funcStatus BarChartClass::initWidget(
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
		// find the bind tag
		bindTag = &TagPtr->getTag(p_wptr->BindTagID);
		tagtrigger.mTagPtr = bindTag;
		tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
		//set the widget value
		if(widgetCtrl(pagePtr, p_wptr,&tagtrigger,1) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);

		p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
	}

	#if (defined PC_SIM) || (defined CHIP_GC9002_NoHyper)
	if(myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn,pagePtr) != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);
		
	return AHMI_FUNC_SUCCESS;
	#endif
	
	#if (defined VERSION_2) && (defined EMBEDDED)
	Op_sourceShift = *u32p_sourceShift;
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
//WidgetAttr£º
//15-8 : reserved
//5-7  : the mode£º0£ºswitch between different texture 1£ºrotate
//4-0  : the type of the widget
//*****************************
funcStatus BarChartClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{
	s32 value;
    s16	yMin;
	s16	yBase;
    s16 yMax;
	u16 count;
	u16 padding;
	u16 barWidth;
	u16 barHeight;
	s32 scaleX;
	s32 scaleY;
	s32 curWidth;
	s32 curHeight;
	u32 i;
    TextureClassPtr texturePtr;
	u16 arrayElemNum;

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
		ERROR_PRINT("ERROR: when drawing bar chart widget, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}
	
	value = (s32)(ActionPtr->mTagPtr->getValue());

	yMin = p_wptr->BarchartYMin;
	yBase = p_wptr->BarchartYBase;
	yMax = p_wptr->BarchartYMax;
	count = p_wptr->BarchartCount;
	padding = p_wptr->BarchartPadding;
	barWidth = p_wptr->BarchartBarWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512;
	barHeight = p_wptr->BarchartBarHeight * p_wptr->ADAPTIVE_SCALE_LEVEL / 512;

	arrayElemNum = ActionPtr->mTagPtr->getCurArryElemNum();

	curWidth = (p_wptr->WidgetWidth - padding * (count+1))/count;
	scaleX = (curWidth<<9) / barWidth;

	for (i = 0; i < count; i++)
	{
		texturePtr = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex+i+1];
		if (i >= arrayElemNum)
		{
			//no data
			texturePtr->mTexAttr |= (FORCE_UNDISPLAY);
		}
		else
		{
			texturePtr->mTexAttr &= (~FORCE_UNDISPLAY);

			value = ActionPtr->mTagPtr->getArrayS32Value(i);
			
			curHeight = ((s32)p_wptr->WidgetHeight) * (value - yBase) / (yMax - yMin) ;
			scaleY = (curHeight<<9) / barHeight;
			if (scaleY == 0)
			{
				texturePtr->mTexAttr |= (FORCE_UNDISPLAY);
			}
			else
			{
				texturePtr->OffsetY = (((s32)p_wptr->WidgetHeight)  * (yMax - yBase) / (yMax - yMin) - curHeight) << 4;
				texturePtr->OffsetX = (i * (padding + curWidth) + padding)<<4;
				texturePtr->ScalerX = scaleX;
				texturePtr->ScalerY = scaleY;
			}

			

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
//4-0: type of widget, button is ALPHA_IMG
//**************************************
funcStatus BarChartClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    u8 u8_widgetRefresh      
    )
{
	s32 value;
    s32	maxValue;
	s32	minValue;
	s32 oldValue;
	u16 oldValueinit;

	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}




	p_wptr->WidgetAttr |= 0x8000;


	p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	
	return AHMI_FUNC_SUCCESS;
}



#endif

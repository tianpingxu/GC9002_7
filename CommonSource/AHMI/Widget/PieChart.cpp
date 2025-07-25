////////////////////////////////////////////////////////////////////////////////
//pie chart
//created by Zzen1sS
//2023/5/11
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "PieChart.h"
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
PieChartClass::PieChartClass()
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
PieChartClass::~PieChartClass()
{

}

funcStatus PieChartClass::initWidget(
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

//clip arc with mask tex
void clipArc(TextureClassPtr texturePtr, u32 startOffset, u32 stopOffset)
{
	//0:clear
	//1/2/3/4 mask
	//5 img
	u8 i;
	u8 numOfQuad;
	u8 lastArc;
	u32 offset;
	s32 ShearAngleX;
	offset = startOffset;
	
	if(startOffset ==  stopOffset)
	{
		for(i = 0; i < 6; i++)
		{
			texturePtr[i].mTexAttr |= FORCE_UNDISPLAY;
		}
		return;
	}
	else
	{
		for(i = 0; i < 6; i++)
		{
			texturePtr[i].mTexAttr &= (~FORCE_UNDISPLAY);
		}
	}

	stopOffset = stopOffset - startOffset;
	startOffset = 0;
	numOfQuad = stopOffset / 90;
	lastArc = stopOffset % 90;

	//for(i=0;i<4;i++)
	//{
	//	//show
	//	texturePtr[i+1].RotateAngle = -90*16 + i*90*16 + 0;
	//	texturePtr[i+1].ShearAngleX = 0;
	//	texturePtr[i+1].ShearAngleY = 0;
	//	texturePtr[i+1].ScalerX = 512;
	//	texturePtr[i+1].ScalerY = 512;
	//	texturePtr[i+1].TexAddr = 0xff000000;
	//		
	//}
	//return;

	for(i=0;i<numOfQuad;i++)
	{
		//show
		texturePtr[i+1].RotateAngle = -90*16 + i*90*16 + offset*16;
		texturePtr[i+1].ShearAngleX = 0;
		texturePtr[i+1].ShearAngleY = 0;
		texturePtr[i+1].ScalerX = 512;
		texturePtr[i+1].ScalerY = 512;
		texturePtr[i+1].TexAddr = 0xff000000;
	}

	if (numOfQuad < 4)
	{
		//last arc
		ShearAngleX = (90-lastArc)*16;
		if(ShearAngleX >= 89*16)
			ShearAngleX = 89*16;
		if(ShearAngleX <= -89*16)
			ShearAngleX = -89*16;
					
		if (lastArc>0)
		{
			//show
			texturePtr[i+1].RotateAngle = -90*16 + i*90*16 + offset*16;
			texturePtr[i+1].ShearAngleX = ShearAngleX;
			texturePtr[i+1].ShearAngleY = 0;
			texturePtr[i+1].ScalerX = 512;
			texturePtr[i+1].ScalerY = 512;
			texturePtr[i+1].TexAddr = 0xff000000;
		}
		else
		{
			//not show
			texturePtr[i+1].mTexAttr |= FORCE_UNDISPLAY;
			//texturePtr[i+1].RotateAngle = -90*16 + i*90*16 + offset*16;
			//texturePtr[i+1].ShearAngleX = 0;
			//texturePtr[i+1].ShearAngleY = 0;
			//texturePtr[i+1].ScalerX = 0xF;
			//texturePtr[i+1].ScalerY = 0xF;
			//texturePtr[i+1].TexAddr = 0;
		}
	}

	for (i = numOfQuad+1; i < 4; i++)
	{
		//not show
		texturePtr[i+1].mTexAttr |= FORCE_UNDISPLAY;
		//texturePtr[i+1].RotateAngle = -90*16 + i*90*16 + offset*16;
		//texturePtr[i+1].ShearAngleX = 0;
		//texturePtr[i+1].ShearAngleY = 0;
		//texturePtr[i+1].ScalerX = 0xF;
		//texturePtr[i+1].ScalerY = 0xF;
		//texturePtr[i+1].TexAddr = 0;
	}
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
funcStatus PieChartClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{
	u32 value;
	u16 count;
	u16 baseOffset;
	u32 curOffset;
	u32 curArc;
	u32 totalValue;
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
		ERROR_PRINT("ERROR: when drawing pie chart widget, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}
	
	///value = ActionPtr->mTagPtr->getValue();

	count = p_wptr->PiechartCount;
	baseOffset = p_wptr->PiechartOffset;
	

	arrayElemNum = ActionPtr->mTagPtr->getCurArryElemNum();

	totalValue = 0;
	for (i = 0; i < count; i++)
	{
		if (i >= arrayElemNum)
		{
			totalValue += 0;
		}
		else
		{
			value = ActionPtr->mTagPtr->getArrayS32Value(i);
			totalValue += value;

		}
		
	}

	curOffset = baseOffset;
	for (i = 0; i < count; i++)
	{
		texturePtr = &pagePtr->pBasicTextureList[p_wptr->StartNumofTex+i*6+1];
		if (i >= arrayElemNum)
		{
			value = 0;
		}
		else
		{
			value = ActionPtr->mTagPtr->getArrayS32Value(i);
			
		}
		if(totalValue == 0)
		{
			curArc = 0;
		}
		else
		{
			curArc = value * 360 / totalValue;
		}
		clipArc(texturePtr, curOffset, curOffset+curArc);
		curOffset += curArc;
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
funcStatus PieChartClass::SetValueCtrl(
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

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:    2019/7/26
// File Name:     CanvasClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 1.0 - File Created  2019/7/26 by Zzen1sS
// Additional Comments:
//    definition of button widget
// 
////////////////////////////////////////////////////////////////////////////////

#include "SelectorClass.h"
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "drawImmediately_cd.h"
#ifdef AHMI_DEBUG
#include <stdio.h>
#include "trace.h"
#endif

#ifdef AHMI_CORE
#define Press2DraggingLength    (5)

extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
//extern u16 WorkingPageID;

extern QueueHandle_t		ActionInstructionQueue;

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;

extern uint32_t AHMITick;

extern Easing gEasing;
extern u8 force_cancel_subcanvas_page_mousetouch;

SelectorClass::SelectorClass(void)
{
}


SelectorClass::~SelectorClass(void)
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
funcStatus SelectorClass::initWidget(
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
funcStatus SelectorClass::widgetCtrl(
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
	u8 ogAlpha;
	u16 count;
	u16 showCount;
	u8 pressed;
//	u8 animating;
	u8 autoHide;
	u8 arrange;
	u8 selectorMode;
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
		ERROR_PRINT("ERROR: when drawing selector widght, the offset or width or height exceeds the banduary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	//get texture
	pCurrentPage = pagePtr;
	if(pCurrentPage->pBasicTextureList == NULL)
	{
		ERROR_PRINT("ERROR: when drawing selector widght, the texture list corrupt");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	if(p_wptr->StartNumofTex < pCurrentPage->mTotalNumOfTexture )
		TexturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex]);
	else 
	{
		ERROR_PRINT("ERROR: when drawing selector widght, the start texture is more then total number of texture");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
	}

	if((TexturePtr < (void*)startOfDynamicPage) || (TexturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
	
	p_wptr->NumOfTex = p_wptr->SELECTORCOUNT + 4;
	
	ogAlpha = p_wptr->SELECTORALPHA;
	autoHide = p_wptr->SELECTOROPTIONS & 0x01;
	count = p_wptr->SELECTORCOUNT;
	showCount = p_wptr->SELECTORSHOWCOUNT;
	arrange = (p_wptr->SELECTOROPTIONS & 0x04)>>2;
	selectorMode = (p_wptr->SELECTOROPTIONS & 0x16)>>4;
//	cycle = (p_wptr->SELECTOROPTIONS & 0x02)>>1;

	if(ActionPtr->mInputType == ACTION_TAG_SET_VALUE) //set tag value
	{
		tag_value = (s32)ActionPtr->mTagPtr->getValue();
		if ((p_wptr->INITIALIZED & 0x1) == 0)
		{
			if(autoHide)
			{
				renderAlpha(pagePtr, p_wptr,0);
			}
			else
			{
				if(showCount != 1)
				{
					renderAlpha(pagePtr, p_wptr,ogAlpha);
				}
			}
			p_wptr->INITIALIZED |= 0x1;
		}
		
		if( p_wptr->ANIMATION_DURATION == 0) //no animation
		{
			pressed = p_wptr->SELECTORSTATUS & 1;
			
			if(selectorMode==0){
				updateIndex(pagePtr,p_wptr,ActionPtr);
				
			}
			if (pressed)
			{
				if(showCount != 1)
				{
					renderAlpha(pagePtr, p_wptr,ogAlpha);
				}
			}else{
				if(autoHide)
				{
					renderAlpha(pagePtr, p_wptr,0);
				}else
				{
					if(showCount != 1)
					{
						renderAlpha(pagePtr, p_wptr,ogAlpha);
					}
				}
				
			}
			renderTexture(pagePtr, p_wptr, tag_value);	//set the value of texture from current value
		}
		else if(u8_pageRefresh)	   //page refresh
		{
			//when page refresh, use the animation value as the next value, donnot need to refresh the widget by Mr.z
			p_wptr->ANIMATIONCURVALUE_L = p_wptr->SELECTOROFFSET;
			
			p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
			
			p_wptr->STOP_TAG_L = 0;
			
			renderTexture(pagePtr, p_wptr, tag_value);	 //��Ҫ��Ʒ��һ����֤������Ƿ�ɾ��
		}
		else
		{
			//get texture
			TexturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex])+4;
			if(arrange)
			{
				singleLength = ((p_wptr->WidgetWidth / showCount) <<4);
			}
			else
			{
				singleLength = ((p_wptr->WidgetHeight / showCount) <<4);
			}
			
			//tag value should be 0 ~ count-1
			if (tag_value < 0)
			{
				tag_value = 0;
			}else if (tag_value > count-1)
			{
				tag_value = count - 1;
			}
			
			if(selectorMode==1){
				baseOffset = singleLength * (showCount-1)/2; //tag_value selector item's target offsetY

				TexturePtr+=tag_value;//move to the texture that should be in center
				if(arrange)
				{
					p_wptr->START_TAG_L = (u16)(TexturePtr->OffsetX - baseOffset + (TexturePtr->ScalerX*singleLength/512 - singleLength)/2);//current offset relative to real position if tag is set to tag_value
				}
				else
				{
					p_wptr->START_TAG_L = (u16)(TexturePtr->OffsetY - baseOffset + (TexturePtr->ScalerY*singleLength/512 - singleLength)/2);//current offset relative to real position if tag is set to tag_value
				}
			}else{
				updateIndex(pagePtr,p_wptr,ActionPtr);
			}
			p_wptr->STOP_TAG_L = 0;
		}
		pressed = p_wptr->SELECTORSTATUS & 1;
//		animating = (p_wptr->SELECTORSTATUS >> 1) & 1;
		if(u8_pageRefresh == 0 && !pressed)//release mouse
		{
			if( (p_wptr->ANIMATION_DURATION != 0) && (p_wptr->START_TAG_L != p_wptr->STOP_TAG_L) )
			{
				//reset alpha
				if(showCount != 1)
				{
					renderAlpha(pagePtr, p_wptr,ogAlpha);
				}
				////calculate real start value
				//p_wptr->calculateCurrentAnimation(pagePtr);

				//p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
				//p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
				p_wptr->SELECTORSTATUS &= (~0x4);
				p_wptr->SELECTORSTATUS |= 0x2;//animating
				p_wptr->ANIMATION_DURATION = p_wptr->STATIC_ANIMATION_DURATION;
				p_wptr->loadTextureWithAnimation(pagePtr);

			}else if(!selectorMode)
			{
				renderTexture(&gPagePtr[p_wptr->ATTATCH_PAGE], p_wptr, (s32)TagPtr->getTag(p_wptr->BindTagID).getValue());
			}			
		}
	}
		
	else 
	{
		ERROR_PRINT("ERROR: illegal action trigger button widget");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_ActionTypeErr);
	}
    
	return AHMI_FUNC_SUCCESS;
}

funcStatus SelectorClass::updateIndex
		 (
		 DynamicPageClassPtr pagePtr,
		 WidgetClassPtr p_wptr,  
		 ActionTriggerClassPtr ActionPtr
		 ){
	u8 cycle;
	s32 tag_value = 0;
	s16 indexOfTop = (s16)p_wptr->INDEXOFTOP;
//	s16 indexOfSelectedInShow = (s16)p_wptr->INDEXOFSELECTEDINSHOW;
	s32 tempValue;
	s32 d_indexOfTop=0;
	u16 count;
	u16 showCount;
	TextureClassPtr TexturePtr;
	DynamicPageClassPtr pCurrentPage;
	u16 singleLength;//length of each selector item
	u8 arrange = (p_wptr->SELECTOROPTIONS & 0x04)>>2;
//	u8 actionType = ActionPtr->mInputType;
	cycle = (p_wptr->SELECTOROPTIONS & 0x02)>>1;
	tag_value = (s32)ActionPtr->mTagPtr->getValue();
	
	pCurrentPage = pagePtr;
	TexturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex])+4;
	if (arrange)
	{
		singleLength = (TexturePtr->TexWidth<<4);
	}else
	{
		singleLength = (TexturePtr->TexHeight<<4);
	}
	
	count = p_wptr->SELECTORCOUNT;
	showCount = p_wptr->SELECTORSHOWCOUNT;
	//tag value should be 0 ~ count-1
	if (tag_value < 0)
	{
		tag_value = 0;
	}else if (tag_value > count-1)
	{
		tag_value = count - 1;
	}
	
	if(p_wptr->OldValueL == tag_value){ //drag
		TexturePtr += indexOfTop;
		if(p_wptr->ANIMATION_DURATION != 0){
			if (arrange)
			{
				p_wptr->START_TAG_L = (s16)(TexturePtr->OffsetX);
			}else
			{
				p_wptr->START_TAG_L = (s16)(TexturePtr->OffsetY);
			}
			
		}
	}else{
		if(tag_value>=count){
			while (tag_value >= count) {
				tag_value -= count;
			}
		}else if(tag_value<0){
			while (tag_value < 0) {
				tag_value += count;
			}
		}
		if(cycle){
			
			if(indexOfTop+showCount-1>=count){ // 当index为0和index为count-1的item同时出现时
				if(tag_value<indexOfTop && tag_value+count>indexOfTop+showCount-1 && tag_value+count<indexOfTop+showCount/2+count/2){ //选择器往上/左
					p_wptr->INDEXOFSELECTEDINSHOW = showCount-1;
					d_indexOfTop = tag_value+count-(indexOfTop+showCount-1);
					p_wptr->INDEXOFTOP = indexOfTop+d_indexOfTop;
					if(p_wptr->INDEXOFTOP>=count){
						p_wptr->INDEXOFTOP -= count;
					}
					if(p_wptr->ANIMATION_DURATION != 0){
						p_wptr->D_INDEXOFTOP = d_indexOfTop;
						p_wptr->START_TAG_L = d_indexOfTop*singleLength;
					}
				}else if(tag_value<indexOfTop && tag_value+count>indexOfTop+showCount-1 && tag_value+count>=indexOfTop+showCount/2+count/2){ //选择器往下/右
					p_wptr->INDEXOFSELECTEDINSHOW = 0;
					d_indexOfTop = tag_value - indexOfTop;
					p_wptr->INDEXOFTOP = tag_value;

					if(p_wptr->ANIMATION_DURATION != 0){
						p_wptr->D_INDEXOFTOP = d_indexOfTop;
						p_wptr->START_TAG_L = d_indexOfTop*singleLength;
					}
				}else{
					if(tag_value - p_wptr->INDEXOFTOP<0){
						p_wptr->INDEXOFSELECTEDINSHOW = tag_value - p_wptr->INDEXOFTOP + count;
					}else{
						p_wptr->INDEXOFSELECTEDINSHOW = tag_value - p_wptr->INDEXOFTOP;
					}
				}
			}else{
				if(tag_value>indexOfTop && tag_value>indexOfTop+showCount-1 && tag_value<indexOfTop+showCount/2+count/2){ //选择器往上/左
					p_wptr->INDEXOFSELECTEDINSHOW = showCount-1;
					d_indexOfTop = tag_value-(indexOfTop+showCount-1);
					p_wptr->INDEXOFTOP = indexOfTop+d_indexOfTop;
					if(p_wptr->INDEXOFTOP>=count){
						p_wptr->INDEXOFTOP -= count;
					}

					if(p_wptr->ANIMATION_DURATION != 0){
						p_wptr->D_INDEXOFTOP = d_indexOfTop;
						p_wptr->START_TAG_L = d_indexOfTop*singleLength;
					}
				}else if(tag_value>indexOfTop && tag_value>indexOfTop+showCount-1 && tag_value>=indexOfTop+showCount/2+count/2){ //选择器往下/右
					p_wptr->INDEXOFSELECTEDINSHOW = 0;
					d_indexOfTop = tag_value-(indexOfTop+count);
					p_wptr->INDEXOFTOP = tag_value;

					if(p_wptr->ANIMATION_DURATION != 0){
						p_wptr->D_INDEXOFTOP = d_indexOfTop;
						p_wptr->START_TAG_L = d_indexOfTop*singleLength;
					}
				}else if(tag_value<indexOfTop){
					if (tag_value+count<indexOfTop+showCount/2+count/2) { //选择器往上/左
                        d_indexOfTop = (tag_value+count) - (indexOfTop+showCount-1);
                        p_wptr->INDEXOFTOP = indexOfTop+d_indexOfTop;
						if(p_wptr->INDEXOFTOP>=count){
							p_wptr->INDEXOFTOP -= count;
						}
						p_wptr->INDEXOFSELECTEDINSHOW = showCount-1;
						if(p_wptr->ANIMATION_DURATION != 0){
							p_wptr->D_INDEXOFTOP = d_indexOfTop;
							p_wptr->START_TAG_L = d_indexOfTop*singleLength;
						}
						
                    } else { //选择器往下/右
                        p_wptr->INDEXOFSELECTEDINSHOW = 0;
						d_indexOfTop = tag_value-indexOfTop;
						p_wptr->INDEXOFTOP = tag_value;
						if(p_wptr->ANIMATION_DURATION != 0){
							p_wptr->D_INDEXOFTOP = d_indexOfTop;
							p_wptr->START_TAG_L = d_indexOfTop*singleLength;
						}
                    }
					
				}else{
					p_wptr->INDEXOFSELECTEDINSHOW = tag_value - p_wptr->INDEXOFTOP;
				}
			}
		}else{
			if (tag_value >= indexOfTop + showCount) {  
				//输入值大于显示的选项中最大的index时，选择器向上滑，tag_value位于最下方，更新indexOfTop为最上方的index
				tempValue = tag_value - showCount + 1;
				d_indexOfTop = tempValue - indexOfTop;
				if (tempValue>=count) {
					tempValue -= count;
				}
				indexOfTop = tempValue;
				p_wptr->INDEXOFTOP = tempValue;
				p_wptr->INDEXOFSELECTEDINSHOW = showCount-1;

				if(p_wptr->ANIMATION_DURATION != 0){
					p_wptr->D_INDEXOFTOP = d_indexOfTop;
					TexturePtr += indexOfTop;
					if (arrange)
					{
						p_wptr->START_TAG_L = (s16)(TexturePtr->OffsetX);
					}else
					{
						p_wptr->START_TAG_L = (s16)(TexturePtr->OffsetY);
					}
				}

			}else if(tag_value < indexOfTop){
				//输入值小于显示的选项中最小的index（即indexOfTop）时，选择器向下滑，tag_value位于最上方，更新indexOfTop为最上方的index（即indexOfTop）
				tempValue = tag_value;
				d_indexOfTop = tempValue - indexOfTop;
				if (tempValue>=count) {
					tempValue -= count;
				}
				indexOfTop = tempValue;
				p_wptr->INDEXOFTOP = tempValue;
				p_wptr->INDEXOFSELECTEDINSHOW = 0;

				if(p_wptr->ANIMATION_DURATION != 0){
					p_wptr->D_INDEXOFTOP = d_indexOfTop;
					TexturePtr += indexOfTop;
					if (arrange)
					{
						p_wptr->START_TAG_L = (s16)(TexturePtr->OffsetX);
					}else
					{
						p_wptr->START_TAG_L = (s16)(TexturePtr->OffsetY);
					}
				}
			}else{
				p_wptr->INDEXOFSELECTEDINSHOW = tag_value - p_wptr->INDEXOFTOP;

				if(p_wptr->ANIMATION_DURATION != 0){
					TexturePtr += indexOfTop;
					if (arrange)
					{
						p_wptr->START_TAG_L = (s16)(TexturePtr->OffsetX);
					}else
					{
						p_wptr->START_TAG_L = (s16)(TexturePtr->OffsetY);
					}
				}
			}
		}
	}
	return AHMI_FUNC_SUCCESS;
}

u8 calcScaleWithOffset(u8 scale, u16 singleLength, int curOffset, int centerOffset)
{
	int offset = curOffset - centerOffset;
	if (offset < 0)
	{
		offset = -offset;
	}
	if (scale > 16)
	{
		
		if (offset > singleLength)
		{
			return 1*16 * 16 / scale;
		}
		else
		{
			return offset * (1*16 * 16 / scale - 1*16) / singleLength + 1*16;
		}
	}
	else
	{
		if (offset > singleLength)
		{
			return 1*16;
		}
		else
		{
			return offset * (1*16 - scale) / singleLength + scale;
		}
	}
}

int getClosestID(int start, int stop, u16 singleLength, int baseOffset, int centerOffset)
{
	int i;
	int curOffset;
	int id = start-1;
	int minOffset;
	for (i = start; i <= stop ; i++)
	{
		curOffset = baseOffset + (0-i) * singleLength - centerOffset;
		if (curOffset < 0)
		{
			curOffset = -curOffset;
		}
		if(id == start - 1)
		{
			id = i;
			minOffset = curOffset;
		}
		else if(curOffset < minOffset)
		{
			id = i;
			minOffset = curOffset;
		}
	}
	return id;
}

funcStatus SelectorClass::renderTexture
		 (
		 DynamicPageClassPtr pagePtr,
		 WidgetClassPtr p_wptr,  
		 s32 value
		 ){
	s32 tag_value;
//	s32 tempValue;
	TextureClassPtr TexturePtr;
	TextureClassPtr TexturePtrAlpha1;
	TextureClassPtr TexturePtrAlpha2;
	TextureClassPtr TexturePtrAlpha3;
	TextureClassPtr curTexturePtr;
	DynamicPageClassPtr pCurrentPage;
	u16 count;
	u16 showCount;
	u16 singleLength;//length of each selector item
	u16 singleLengthForAlphaMask;
	//TextureClassPtr curTexture;
	s32 baseOffset;
	s32 tempTopOffset;
	u8 cycle;
	u8 arrange;
	u8 focusflag;
	int i;
	int baseIdx;
	u8 curTexScale;
	u8 scaleBackground;
	u8 realScaleBackground;
	int closetID;
	s16 indexOfTop = (s16)p_wptr->INDEXOFTOP;
	s16 d_indexOfTop = (s16)p_wptr->D_INDEXOFTOP;
	s16 indexOfSelectedInShow = (s16)p_wptr->INDEXOFSELECTEDINSHOW;
	int selectorMode;
	selectorMode = (p_wptr->SELECTOROPTIONS & 0x16)>>4;
	
	//get texture
	pCurrentPage = pagePtr;
	TexturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex]);

	tag_value = value;
	
	count = p_wptr->SELECTORCOUNT;
	
	showCount = p_wptr->SELECTORSHOWCOUNT;

	scaleBackground = p_wptr->SELECTORSCALE;
	if(scaleBackground == 0)
	{
		scaleBackground = 16;
	}
	else if(scaleBackground>16)
	{
		realScaleBackground = 16;
	}
	else
	{
		realScaleBackground = scaleBackground;
	}

	//tag value should be 0 ~ count-1
	if (tag_value < 0)
	{
		tag_value = 0;
	}else if (tag_value > count-1)
	{
		tag_value = count - 1;
	}
	TexturePtrAlpha1=TexturePtr + 1;//upper
	TexturePtrAlpha2=TexturePtr + 2;//middle
	TexturePtrAlpha3=TexturePtr + 3;//bottom

	

	TexturePtr=TexturePtr + 4;//0-3 is mask
	arrange = (p_wptr->SELECTOROPTIONS & 0x04)>>2;
	//2 ~ count+1 selector item tex
	if(arrange)
	{
		singleLength = ((p_wptr->WidgetWidth / showCount) <<4);
		singleLengthForAlphaMask = singleLength;
		if(p_wptr->WidgetWidth % showCount)
			singleLengthForAlphaMask += 0x10;
	}
	else
	{
		singleLength = ((p_wptr->WidgetHeight / showCount) <<4);
		singleLengthForAlphaMask = singleLength;
		if(p_wptr->WidgetHeight % showCount)
			singleLengthForAlphaMask += 0x10;
	}

	if (scaleBackground!=16)
	{
		if(arrange)
		{
			TexturePtrAlpha1->TexWidth = ((p_wptr->WidgetWidth) - singleLengthForAlphaMask*realScaleBackground/16/16)/2;
			TexturePtrAlpha2->TexWidth = singleLengthForAlphaMask*realScaleBackground/16/16;
			TexturePtrAlpha2->OffsetX = TexturePtrAlpha1->TexWidth << 4;
			TexturePtrAlpha3->TexWidth = TexturePtrAlpha1->TexWidth;
			TexturePtrAlpha3->OffsetX = (p_wptr->WidgetWidth - TexturePtrAlpha3->TexWidth) << 4;
		}
		else
		{
			TexturePtrAlpha1->TexHeight = ((p_wptr->WidgetHeight) - singleLengthForAlphaMask*realScaleBackground/16/16)/2;
			TexturePtrAlpha2->TexHeight = singleLengthForAlphaMask*realScaleBackground/16/16;
			TexturePtrAlpha2->OffsetY = TexturePtrAlpha1->TexHeight << 4;
			TexturePtrAlpha3->TexHeight = TexturePtrAlpha1->TexHeight;
			TexturePtrAlpha3->OffsetY = (p_wptr->WidgetHeight - TexturePtrAlpha3->TexHeight) << 4;
		}
		
	}

	cycle = (p_wptr->SELECTOROPTIONS & 0x02)>>1;
	focusflag = (p_wptr->SELECTOROPTIONS & 0x08)>>3;
	if(selectorMode==1){
		baseOffset = singleLength * (showCount-1)/2;
		baseOffset += (s16)p_wptr->SELECTOROFFSET;
		
		if (cycle)
		{
			baseIdx = 0;
			tempTopOffset = baseOffset + (baseIdx-tag_value) * singleLength;
			if (tempTopOffset > 0)
			{
				while (tempTopOffset > 0)
				{
					baseIdx = (baseIdx-1)%count;
					if (baseIdx < 0)
					{
						baseIdx += count;
					}
					tempTopOffset -= singleLength;
				}
			}

			if (tempTopOffset + singleLength <= 0)
			{
				while (tempTopOffset + singleLength <= 0)
				{
					baseIdx = (baseIdx+1)%count;
				
					tempTopOffset += singleLength;
				}
			}

			for (i = 0; i < count ; i++)
			{
				curTexturePtr = TexturePtr + baseIdx;

				if(focusflag && ((baseIdx) == tag_value))
					curTexturePtr->FocusedSlice = 1;
				else
					curTexturePtr->FocusedSlice = 0;

				if(arrange)
				{
					curTexScale = calcScaleWithOffset(scaleBackground, singleLength, tempTopOffset + i * singleLength, singleLength * (showCount-1)/2);

					curTexturePtr->OffsetY = 0 - (curTexScale*p_wptr->WidgetHeight - p_wptr->WidgetHeight*16)/2;
					curTexturePtr->OffsetX = tempTopOffset + i * singleLength - (curTexScale*singleLength/16 - singleLength)/2;
					
					curTexturePtr->ScalerX = curTexScale << 5;
					curTexturePtr->ScalerY = curTexScale << 5;

					//curTexturePtr->OffsetX = tempTopOffset + i * singleLength;
					if(tempTopOffset + i * singleLength > 0x7FFF)
					{
						//overrun
						curTexturePtr->mTexAttr |= FORCE_UNDISPLAY;
					}
					else if (curTexturePtr->OffsetX + singleLength < 0)
					{
						//right upper than widget left
						curTexturePtr->mTexAttr |= FORCE_UNDISPLAY;
					}/*else if (TexturePtr->OffsetY  < (p_wptr->WidgetOffsetY<<4))*/
					else if (curTexturePtr->OffsetX  < 0)
					{
						//left upper than widget left
						curTexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
					}/*else if (TexturePtr->OffsetY + singleHeight <= ((p_wptr->WidgetOffsetY<<4)+(p_wptr->WidgetHeight<<4)))*/
					else if (curTexturePtr->OffsetX + singleLength <= (p_wptr->WidgetWidth<<4))
					{
						//right upper than widget right
						curTexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
					}/*else if (TexturePtr->OffsetY < ((p_wptr->WidgetOffsetY<<4)+(p_wptr->WidgetHeight<<4)))*/
					else if (curTexturePtr->OffsetX < (p_wptr->WidgetWidth<<4))
					{
						//left upper than widget right
						curTexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
					}
					else{
						//left lower than widget right
						curTexturePtr->mTexAttr |= FORCE_UNDISPLAY;
					}
				}
				else
				{
					curTexScale = calcScaleWithOffset(scaleBackground, singleLength, tempTopOffset + i * singleLength, singleLength * (showCount-1)/2);

					curTexturePtr->OffsetY = tempTopOffset + i * singleLength - (curTexScale*singleLength/16 - singleLength)/2;
					curTexturePtr->OffsetX = 0 - (curTexScale*p_wptr->WidgetWidth - p_wptr->WidgetWidth*16)/2;
					
					curTexturePtr->ScalerX = curTexScale << 5;
					curTexturePtr->ScalerY = curTexScale << 5;

					//curTexturePtr->OffsetY = tempTopOffset + i * singleLength;
					if(tempTopOffset + i * singleLength > 0x7FFF)
					{
						//overrun
						curTexturePtr->mTexAttr |= FORCE_UNDISPLAY;
					}
					else
					{
						forceTextureUndisplay(p_wptr,curTexturePtr,singleLength);
					}
					
				}
				baseIdx = (baseIdx+1)%count;
			}
		
		}else
		{
			for (i = 0; i < count; i++)
			{
 				for(; i < (tag_value - showCount/2 - 10); i++)
				{
					TexturePtr->mTexAttr |= FORCE_UNDISPLAY;
					TexturePtr++;
				}

				if(focusflag && (i == tag_value))
					TexturePtr->FocusedSlice = 1;
				else
					TexturePtr->FocusedSlice = 0;

				if(arrange)
				{
					int offset_tmp = 0;
					if(scaleBackground == 0x10)
					{
						TexturePtr->OffsetY = 0;
						offset_tmp = baseOffset + (i-tag_value) * singleLength;
						TexturePtr->OffsetX = offset_tmp;
						
						TexturePtr->ScalerX = 0x200;
						TexturePtr->ScalerY = 0x200;
					}
					else
					{
						curTexScale = calcScaleWithOffset(scaleBackground, singleLength, baseOffset + (i-tag_value) * singleLength, singleLength * (showCount-1)/2);

						TexturePtr->OffsetY = 0 - (curTexScale*p_wptr->WidgetHeight - p_wptr->WidgetHeight*16)/2;
						offset_tmp = baseOffset + (i-tag_value) * singleLength - (curTexScale*singleLength/16 - singleLength)/2;
						TexturePtr->OffsetX = offset_tmp;
						
						TexturePtr->ScalerX = curTexScale << 5;
						TexturePtr->ScalerY = curTexScale << 5;
					}

					if(offset_tmp > singleLength * showCount)
					{
						for(; i < count; i++)
						{
							TexturePtr->mTexAttr |= FORCE_UNDISPLAY;
							if(i == 0)
								p_wptr->SELECTORCURSPEED = p_wptr->SwipeFactor;
								
							TexturePtr++;
						}
						break;
					}

					if (offset_tmp + singleLength < 0)
					{
						//right upper than widget left
						TexturePtr->mTexAttr |= FORCE_UNDISPLAY;
						if(i == (count - 1))
							p_wptr->SELECTORCURSPEED = -p_wptr->SwipeFactor;
					}
					else if (offset_tmp < 0)
					{
						//left upper than widget left
						TexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
					}
					else if (offset_tmp + singleLength <= (p_wptr->WidgetWidth<<4))
					{
						//right upper than widget right
						TexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
					}
					else if (offset_tmp < (p_wptr->WidgetWidth<<4))
					{
						//left upper than widget right
						TexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
					}
					else{
						//left lower than widget right
						TexturePtr->mTexAttr |= FORCE_UNDISPLAY;
					}
					TexturePtr++;
				}
				else
				{
					int offset_tmp = 0;
					if(scaleBackground == 0x10)
					{
						offset_tmp = baseOffset + (i-tag_value) * singleLength;
						TexturePtr->OffsetY = offset_tmp;
						TexturePtr->OffsetX = 0;
						
						TexturePtr->ScalerX = 0x200;
						TexturePtr->ScalerY = 0x200;
					}
					else
					{
						curTexScale = calcScaleWithOffset(scaleBackground, singleLength, baseOffset + (i-tag_value) * singleLength, singleLength * (showCount-1)/2);

						offset_tmp = baseOffset + (i-tag_value) * singleLength - (curTexScale*singleLength/16 - singleLength)/2;
						TexturePtr->OffsetY = offset_tmp;
						TexturePtr->OffsetX = 0 - (curTexScale*p_wptr->WidgetWidth - p_wptr->WidgetWidth*16)/2;
						
						TexturePtr->ScalerX = curTexScale << 5;
						TexturePtr->ScalerY = curTexScale << 5;
					}
					
					if(offset_tmp > singleLength * showCount)
					{
						for(; i < count; i++)
						{
							TexturePtr->mTexAttr |= FORCE_UNDISPLAY;
							if(i == 0)
								p_wptr->SELECTORCURSPEED = p_wptr->SwipeFactor;
								
							TexturePtr++;
						}
						break;
					}
					
					if (offset_tmp + singleLength < 0)
					{
						//bottom upper than widget top
						TexturePtr->mTexAttr |= FORCE_UNDISPLAY;
						if(i == (count - 1))
							p_wptr->SELECTORCURSPEED = -p_wptr->SwipeFactor;
					}
					else if (offset_tmp < 0)
					{
						//top upper than widget top
						TexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
					}
					else if (offset_tmp + singleLength <= (p_wptr->WidgetHeight<<4))
					{
						//bottom upper than widget bottom
						TexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
					}
					else if (offset_tmp < (p_wptr->WidgetHeight<<4))
					{
						//top upper than widget bottom
						TexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
					}
					else{
						//top lower than widget bottom
						TexturePtr->mTexAttr |= FORCE_UNDISPLAY;
					}
					TexturePtr++;
				}
			}
		}
	}else {
		if (arrange)
		{
			if( indexOfSelectedInShow< 0 || indexOfSelectedInShow>=showCount){//showed items will be Semitransparent(with alpha)
				TexturePtrAlpha1->TexWidth = 0;
				TexturePtrAlpha2->TexWidth = 0;
				TexturePtrAlpha3->TexWidth = showCount * singleLength/16;
				TexturePtrAlpha3->OffsetX = 0;
			
			}else{
				indexOfSelectedInShow = (s16)p_wptr->INDEXOFSELECTEDINSHOW;
				TexturePtrAlpha1->TexWidth = indexOfSelectedInShow * singleLength/16;
				TexturePtrAlpha2->TexWidth = p_wptr->WidgetWidth/showCount;
				TexturePtrAlpha3->TexWidth = (showCount - indexOfSelectedInShow - 1) * singleLength/16;
				TexturePtrAlpha2->OffsetX = indexOfSelectedInShow * singleLength;
				TexturePtrAlpha3->OffsetX = indexOfSelectedInShow * singleLength + singleLength;
		
			}
		}else
		{
			if( indexOfSelectedInShow< 0 || indexOfSelectedInShow>=showCount){//showed items will be Semitransparent(with alpha)
				TexturePtrAlpha1->TexHeight = 0;
				TexturePtrAlpha2->TexHeight = 0;
				TexturePtrAlpha3->TexHeight = showCount * singleLength/16;
				TexturePtrAlpha3->OffsetY = 0;
			
			}else{
				indexOfSelectedInShow = (s16)p_wptr->INDEXOFSELECTEDINSHOW;
				TexturePtrAlpha1->TexHeight = indexOfSelectedInShow * singleLength/16;
				TexturePtrAlpha2->TexHeight = p_wptr->WidgetHeight/showCount;
				TexturePtrAlpha3->TexHeight = (showCount - indexOfSelectedInShow - 1) * singleLength/16;
				TexturePtrAlpha2->OffsetY = indexOfSelectedInShow * singleLength;
				TexturePtrAlpha3->OffsetY = indexOfSelectedInShow * singleLength + singleLength;
		
			}
		}
		

		baseOffset = 0;
		baseOffset += (s16)p_wptr->SELECTOROFFSET;
		
		if(cycle){
			baseIdx = 0;
			tempTopOffset = baseOffset;

			if (d_indexOfTop > 0) //输入tag值后往上移
			{
				baseIdx = indexOfTop-(s16)p_wptr->D_INDEXOFTOP;
				tempTopOffset = baseOffset - d_indexOfTop*singleLength;
				
			}else if (d_indexOfTop < 0){ //输入tag值后往下移
				baseIdx = indexOfTop;
				tempTopOffset = baseOffset;
			}else{  //输入tag值后不移动
				baseIdx = indexOfTop;
				if(baseOffset>0){  //拖拽后往下拉
					int distance = baseOffset/singleLength+1;
					baseIdx = indexOfTop-distance;
					if(baseIdx<0){
						baseIdx += count;
					}
					tempTopOffset = baseOffset-singleLength*distance;
				}
			}

			for (i = 0; i < count; i++)
			{
				curTexturePtr = TexturePtr + baseIdx;

				if(focusflag && ((baseIdx) == tag_value))
					curTexturePtr->FocusedSlice = 1;
				else
					curTexturePtr->FocusedSlice = 0;

				if (arrange)
				{
					curTexturePtr->OffsetX = tempTopOffset + i * singleLength;

				}
				else
				{
					curTexturePtr->OffsetY = tempTopOffset + i * singleLength;					
				}	
				forceTextureUndisplay(p_wptr,curTexturePtr,singleLength);

				baseIdx = (baseIdx+1)%count;

				
			}
		}else{
			for (i = 0; i < count; i++)
			{
				for(; i < (tag_value - showCount/2 - 10); i++)
				{
					TexturePtr->mTexAttr |= FORCE_UNDISPLAY;
					TexturePtr++;
				}
				if(focusflag && (i == tag_value))
					TexturePtr->FocusedSlice = 1;
				else
					TexturePtr->FocusedSlice = 0;

				int offset_tmp = 0;
				int length_Max = 0;
				offset_tmp = baseOffset + (i-indexOfTop) * singleLength;
				if (arrange)
				{
					TexturePtr->OffsetX = offset_tmp;
					length_Max = (p_wptr->WidgetWidth<<4);
				}
				else
				{
					TexturePtr->OffsetY = offset_tmp;
					length_Max = (p_wptr->WidgetHeight<<4);
				}
				
				if(offset_tmp > singleLength * showCount)
				{
					for(; i < count; i++)
					{
						TexturePtr->mTexAttr |= FORCE_UNDISPLAY;
						if(i == 0)
							p_wptr->SELECTORCURSPEED = p_wptr->SwipeFactor;

						TexturePtr++;
					}
					break;
				}

				if (offset_tmp + singleLength < 0)
				{
					//bottom upper than widget top
					TexturePtr->mTexAttr |= FORCE_UNDISPLAY;
					if(i == (count - 1))
						p_wptr->SELECTORCURSPEED = -p_wptr->SwipeFactor;
				}
				else if (offset_tmp < 0)
				{
					//top upper than widget top
					TexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
				}
				else if (offset_tmp + singleLength <= length_Max)
				{
					//bottom upper than widget bottom
					TexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
				}
				else if (offset_tmp < length_Max)
				{
					//top upper than widget bottom
					TexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
				}
				else{
					//top lower than widget bottom
					TexturePtr->mTexAttr |= FORCE_UNDISPLAY;
				}

				//forceTextureUndisplay(p_wptr,TexturePtr,singleLength);
				TexturePtr++;
			}
		}
	}
	
	cycle = (p_wptr->SELECTOROPTIONS & 0x02)>>1;
	focusflag = (p_wptr->SELECTOROPTIONS & 0x08)>>3;
	p_wptr->OldValueL = (u16)value;
	p_wptr->OldValueH = (u16)(value >> 16);
	
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	return AHMI_FUNC_SUCCESS;
 }

funcStatus SelectorClass::forceTextureUndisplay(
	WidgetClassPtr p_wptr,
	TextureClassPtr TexturePtr,
	u16 singleLength
){
	u8 arrange = (p_wptr->SELECTOROPTIONS & 0x04)>>2;
	if (arrange)
	{
		if (TexturePtr->OffsetX + singleLength < 0)
		{
			//bottom upper than widget top
			TexturePtr->mTexAttr |= FORCE_UNDISPLAY;
		}
		else if (TexturePtr->OffsetX  < 0)
		{
			//top upper than widget top
			TexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
		}
		else if (TexturePtr->OffsetX + singleLength <= (p_wptr->WidgetWidth<<4))
		{
			//bottom upper than widget bottom
			TexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
		}
		else if (TexturePtr->OffsetX < (p_wptr->WidgetWidth<<4))
		{
			//top upper than widget bottom
			TexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
		}
		else{
			//top lower than widget bottom
			TexturePtr->mTexAttr |= FORCE_UNDISPLAY;
		}
	}
	else
	{
		//if (TexturePtr->OffsetY + singleHeight < (p_wptr->WidgetOffsetY<<4))
		if (TexturePtr->OffsetY + singleLength < 0)
		{
			//bottom upper than widget top
			TexturePtr->mTexAttr |= FORCE_UNDISPLAY;
		}/*else if (TexturePtr->OffsetY  < (p_wptr->WidgetOffsetY<<4))*/
		else if (TexturePtr->OffsetY  < 0)
		{
			//top upper than widget top
			TexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
		}/*else if (TexturePtr->OffsetY + singleHeight <= ((p_wptr->WidgetOffsetY<<4)+(p_wptr->WidgetHeight<<4)))*/
		else if (TexturePtr->OffsetY + singleLength <= (p_wptr->WidgetHeight<<4))
		{
			//bottom upper than widget bottom
			TexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
		}/*else if (TexturePtr->OffsetY < ((p_wptr->WidgetOffsetY<<4)+(p_wptr->WidgetHeight<<4)))*/
		else if (TexturePtr->OffsetY < (p_wptr->WidgetHeight<<4))
		{
			//top upper than widget bottom
			TexturePtr->mTexAttr &= ~FORCE_UNDISPLAY;
		}
		else{
			//top lower than widget bottom
			TexturePtr->mTexAttr |= FORCE_UNDISPLAY;
		}
	}
	

	return AHMI_FUNC_SUCCESS;
}

funcStatus SelectorClass::renderAlpha(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	u8 value
){
	TextureClassPtr TexturePtr;
	TexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex])+1;//upper alpha mask
	TexturePtr->TexAddr = (TexturePtr->TexAddr & ((1<<24)-1)) + (((u32)value) << 24);
	TexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex])+3;//bottom alpha mask
	TexturePtr->TexAddr = (TexturePtr->TexAddr & ((1<<24)-1)) + (((u32)value) << 24);
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;

	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Tag.setValue

//**************************************
funcStatus SelectorClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,  
	ActionTriggerClassPtr ActionPtr,
	u8 u8_widgetRefresh      
	)
{
//	s32 oldValue;
	u16 oldValueinit;
	s32 value;
	s32 oldValue;
	u16 count;
	u32 actionAddr;
	int selectorMode;
	selectorMode = (p_wptr->SELECTOROPTIONS & 0x16)>>4;

	count = p_wptr->SELECTORCOUNT;
	value = ActionPtr->mTagPtr->getValue();

	//tag value should be 0 ~ count-1
	if (value < 0)
	{
		ERROR_PRINT("ERROR: Selector widget bindTag value exceeds the boundary");
		u16 CanvasID, SubcanvasID, WidgetID;
		p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
		EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
		value = 0;
	}else if (value > count-1)
	{
		ERROR_PRINT("ERROR: Selector widget bindTag value exceeds the boundary");
		u16 CanvasID, SubcanvasID, WidgetID;
		p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
		EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
		value = count - 1;
	}

	oldValueinit = (p_wptr->WidgetAttr & 0x8000) ? 1 : 0;
	
	if(oldValueinit)
	{
		oldValue = (s32)((p_wptr->OldValueH  << 16) + p_wptr->OldValueL);
		if(oldValue != value){
			//tagchange
			if(p_wptr->TagChangeAction && (p_wptr->TagChangeAction <= pagePtr->mActionInstructionsSize))
			{
				actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->TagChangeAction);
				csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
			}
		}else{
			if((p_wptr->SELECTORSTATUS & 1) == 1)
			{
				//click release
				if(p_wptr->OnRealeaseAction <= pagePtr->mActionInstructionsSize)
				{
					actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->OnRealeaseAction);
					csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
				}
			}
		}
	}
    else // no old value
    {
		
    }
    
	p_wptr->WidgetAttr |= 0x8000;

	if(selectorMode==1){
		p_wptr->OldValueL = (u16)value;
		p_wptr->OldValueH = (u16)(value >> 16);
	}
	
	
	p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;

	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Mouse.Touch
//Do not support touch propagation.
//**************************************
funcStatus SelectorClass::MouseTouchCtrl(
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
	s32 tag_value= 0;
	s32 cur_x;
	s32 cur_y;
	u8 ogAlpha;//original alpha
	u16 count;
	u16 showCount;
	u16 singleLength;//length of each selector item
	//TextureClassPtr curTexture;
//	s32 baseOffsetY;
	TextureClassPtr TexturePtr;
	u16 indexOfTop = p_wptr->INDEXOFTOP;
	s16 mousemoveOffset;
	s16 mousemoveOffsetValue; //for computing index of top
	s16 indexOfSelectedInShow = (s16)p_wptr->INDEXOFSELECTEDINSHOW;
	u8 pressed;
	u8 cycle;
	u8 arrange;
	u8 actionType;
	u16 selector_width;
	u16 selector_height;
	s16 selector_offsetX;
	s16 selector_offsetY;
	u8 flag = 0;
	int selectorMode;
	u32 actionAddr;
	u32 oldTick;
	s32 duration;
	selectorMode = (p_wptr->SELECTOROPTIONS & 0x16)>>4;
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
		ERROR_PRINT("ERROR: when drawing selectorClass widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	actionType = ActionPtr->mInputType;

	//start address of texture
	TexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);

	#ifdef EMBEDDED
	if((TexturePtr < (void*)startOfDynamicPage) || (TexturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
#endif

	ogAlpha = p_wptr->SELECTORALPHA;
	count = p_wptr->SELECTORCOUNT;
	showCount = p_wptr->SELECTORSHOWCOUNT;
	cycle = (p_wptr->SELECTOROPTIONS & 0x2) >> 1;
	arrange = (p_wptr->SELECTOROPTIONS & 0x04)>>2;

	//tag value should be 0 ~ count-1
	
	TexturePtr=TexturePtr + 4;//0-3 is mask
	//2 ~ count+1 selector item tex
	if(arrange)
	{
		singleLength = ((p_wptr->WidgetWidth / showCount) <<4);
	}
	else
	{
		singleLength = ((p_wptr->WidgetHeight / showCount) <<4);
	}
	/*baseOffsetY = (p_wptr->WidgetOffsetY<<4) + singleHeight * (showCount-1)/2;*/
//	baseOffsetY = singleHeight * (showCount-1)/2;
	tag_value = (s32)ActionPtr->mTagPtr->getValue();
		
	//tag value should be 0 ~ count-1
	if (tag_value < 0)
	{
		tag_value = 0;
	}else if (tag_value > count-1)
	{
		tag_value = count - 1;
	}

//	if(actionType == ACTION_MOUSE_HOLDING)
//	{
//		if(((pReleasePoint->x) <= 0) || ((pReleasePoint->x) >= (p_wptr->WidgetWidth)) ||
//		   ((pReleasePoint->y) <= 0) || ((pReleasePoint->y) >= (p_wptr->WidgetHeight)))
//		{
//			actionType = ACTION_MOUSE_CANCLE;
//		}
//	}

	//check long press
	if (actionType == ACTION_MOUSE_HOLDING)
	{
		if ((( p_wptr->SELECTORSTATUS & 1) == 1) && (( p_wptr->SELECTORSTATUS & (1<<3)) != (1<<3)))
		{
			//pressed, but not dragging
			oldTick = (u32)((p_wptr->SELECTORTickH  << 16) + p_wptr->SELECTORTickL);
			duration = AHMITick - oldTick;
			#ifdef PC_SIM
			if(duration >= 50)
			#endif
			#if (defined VERSION_2) && (defined EMBEDDED)  
			if(duration >= 500)
			#endif
			{
				actionType = ACTION_MOUSE_LONGPRESS;

				//trigger once
				p_wptr->SELECTORTickL = (u16)AHMITick;
				p_wptr->SELECTORTickH = (u16)(AHMITick >> 16);
			}
		}
	}

	if( actionType == ACTION_MOUSE_PRESS )
	{
		p_wptr->PressFlag = 1;

		p_wptr->SELECTORTickL = (u16)AHMITick;
		p_wptr->SELECTORTickH = (u16)(AHMITick >> 16);

		p_wptr->SELECTOROldTickForSpeedL = (u16)AHMITick;
		p_wptr->SELECTOROldTickForSpeedH = (u16)(AHMITick >> 16);

		//ERROR_PRINT("PRESS");
		cur_x  = ((s16)(pPressPoint->x) << 4);
		cur_y  = ((s16)(pPressPoint->y) << 4);
		//offset between mouse press and center selector item
		if(arrange)
		{
			p_wptr->SELECTORPRESSOFFSET = (u16)(cur_x);
		}
		else
		{
			p_wptr->SELECTORPRESSOFFSET = (u16)(cur_y);
		}
	    p_wptr->SELECTORSTATUS |= 1;//pressed

		p_wptr->SELECTORSTATUS &= ~(1<<3);//not dragging

		//reset alpha
		if(showCount != 1)
		{
			renderAlpha(pagePtr, p_wptr,ogAlpha);
		}
		
		p_wptr->clearAnimation();
		
		//clear animation
		p_wptr->NOW_DURATION &= 0x0000;
		p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
		//renderTexture(p_wptr, tag_value);
		if(p_wptr->TouchPropagation & TouchPropagationSet)
		{
			return WIDGET_TOUCH_CANCEL;
		}
		else
		{
			return AHMI_FUNC_SUCCESS;
		}
	}else if (actionType == ACTION_MOUSE_HOLDING)
	{
		if(p_wptr->PressFlag == 1)
		{
			//moving
			pressed = p_wptr->SELECTORSTATUS & 1;
			if (!pressed)
			{
				//no press
				//ERROR_PRINT("MOVING");
				return AHMI_FUNC_SUCCESS;
			}
			//ERROR_PRINT("HOLDING");
			//dragging
			cur_x  = ((s16)(pReleasePoint->x)) << 4;
			cur_y  = ((s16)(pReleasePoint->y)) << 4;
			/*char text[100];
			sprintf_s(text,100, "offsetY %d \r\n",cur_y);
			ERROR_PRINT(text);*/

			oldTick = (u32)((p_wptr->SELECTOROldTickForSpeedH  << 16) + p_wptr->SELECTOROldTickForSpeedL);
			duration = AHMITick - oldTick;
			p_wptr->SELECTOROldTickForSpeedL = (u16)AHMITick;
			p_wptr->SELECTOROldTickForSpeedH = (u16)(AHMITick >> 16);

			if(arrange)
			{
				p_wptr->SELECTOROFFSET += (u16)(cur_x - (s16)(p_wptr->SELECTORPRESSOFFSET));
				if(cur_x != p_wptr->SELECTORPRESSOFFSET && duration != 0)
				{
					p_wptr->SELECTORCURSPEED = (s16)((cur_x - (s16)(p_wptr->SELECTORPRESSOFFSET)) >> 4) * 1000 / duration / 4;//"/ 4" maybe should update
				}
				p_wptr->SELECTORPRESSOFFSET = (u16)cur_x;

				if (((pReleasePoint->x - pPressPoint->x) < -Press2DraggingLength) || ((pReleasePoint->x - pPressPoint->x) > Press2DraggingLength))
				{
					//dragging
					p_wptr->SELECTORSTATUS |= 1 << 3;
				}
			}
			else
			{
				p_wptr->SELECTOROFFSET += (u16)(cur_y - (s16)(p_wptr->SELECTORPRESSOFFSET));
				if(cur_y != p_wptr->SELECTORPRESSOFFSET && duration != 0)
				{
					p_wptr->SELECTORCURSPEED = (s16)((cur_y - (s16)(p_wptr->SELECTORPRESSOFFSET)) >> 4) * 1000 / duration / 4;//"/ 4" maybe should update
				}
				p_wptr->SELECTORPRESSOFFSET = (u16)cur_y;

				if (((pReleasePoint->y - pPressPoint->y) < -Press2DraggingLength) || ((pReleasePoint->y - pPressPoint->y) > Press2DraggingLength))
				{
					//dragging
					p_wptr->SELECTORSTATUS |= 1 << 3;
				}
			}
			if(selectorMode==0){
				p_wptr->D_INDEXOFTOP = 0;
			}
			renderTexture(pagePtr, p_wptr, tag_value);
			force_cancel_subcanvas_page_mousetouch = 1;
			return AHMI_FUNC_SUCCESS;
		}
		
	}
	else if (actionType == ACTION_MOUSE_LONGPRESS)
	{
		//send release action
		if(p_wptr->OnLongPressAction <= pagePtr->mActionInstructionsSize)
		{
			actionAddr = (u32)(pagePtr->pActionInstructions + p_wptr->OnLongPressAction);
			csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
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
	else if (actionType == ACTION_MOUSE_RELEASE || actionType == ACTION_MOUSE_CANCLE)
	{
		if(p_wptr->PressFlag == 1)
		{
			p_wptr->PressFlag = 0;
			cur_x  = ((s16)(pReleasePoint->x)) << 4;
			cur_y  = ((s16)(pReleasePoint->y)) << 4;
			
			if((s16(p_wptr->SELECTORCURSPEED) < -s16(p_wptr->SwipeMultiSpeed) || s16(p_wptr->SELECTORCURSPEED) > s16(p_wptr->SwipeMultiSpeed)) && (p_wptr->ANIMATION_DURATION != 0))
			{
				//p_wptr->SELECTORSTATUS &= (~0x4);
				//p_wptr->SELECTORSTATUS |= 0x2;//animating
				p_wptr->SELECTORSTATUS = 0x10;//multiSpeed is true

				s16 movePerFrame = 0;
				if(s16(p_wptr->SELECTORCURSPEED) < 0)
					movePerFrame = -s16(p_wptr->SELECTORCURSPEED);
				else
					movePerFrame = s16(p_wptr->SELECTORCURSPEED);
				p_wptr->ANIMATION_DURATION = ((movePerFrame + s16(p_wptr->SwipeFactor) - 1) / s16(p_wptr->SwipeFactor) + 1) * 1000 / 4;//"/ 4" maybe should update
				p_wptr->loadTextureWithAnimation(pagePtr);
			}
			else
			{
				mousemoveOffset = (s16)p_wptr->SELECTOROFFSET;
				//ERROR_PRINT("release");
				//set new tag with current offset
				if(selectorMode==1){
					if ( mousemoveOffset < 0)
					{
						if(-mousemoveOffset > (p_wptr->SwipeChangeDistance << 4))
						{
							tag_value = tag_value + ((-mousemoveOffset) + singleLength/2)/singleLength;
						}
						if((s16(p_wptr->SELECTORCURSPEED) < -s16(p_wptr->QuickSwipeSpeed) || s16(p_wptr->SELECTORCURSPEED) > s16(p_wptr->QuickSwipeSpeed)) && (p_wptr->ANIMATION_DURATION != 0))
						{
							tag_value++;
						}
					}else
					{
						if(mousemoveOffset > (p_wptr->SwipeChangeDistance << 4))
						{
							tag_value = tag_value - (mousemoveOffset + singleLength/2)/singleLength;
						}
						if((s16(p_wptr->SELECTORCURSPEED) < -s16(p_wptr->QuickSwipeSpeed) || s16(p_wptr->SELECTORCURSPEED) > s16(p_wptr->QuickSwipeSpeed)) && (p_wptr->ANIMATION_DURATION != 0))
						{
							tag_value--;
						}
					}
					//set tag value
					if (cycle)
					{
						tag_value = tag_value % count;
						if (tag_value < 0)
						{
							tag_value += count;
						}
					}else
					{
						if (tag_value < 0)
						{  
							tag_value = 0;
						}else if (tag_value > count-1)
						{
							tag_value = count - 1;
						}
					}

				}else{
				
					if(mousemoveOffset){ // drag then release
						if ( mousemoveOffset < 0)
						{ //向上
							mousemoveOffsetValue = ((-mousemoveOffset) + singleLength/2)/singleLength;
							p_wptr->INDEXOFTOP += mousemoveOffsetValue;
					
							if(cycle){
								if(p_wptr->INDEXOFTOP>=count){
									p_wptr->INDEXOFTOP -= count;
								}
						
							}else{
								if(p_wptr->INDEXOFTOP+showCount>=count){
									p_wptr->INDEXOFTOP = count-showCount;
								}
						
							}

							if(cycle){
								indexOfSelectedInShow -= mousemoveOffsetValue;
								if(indexOfSelectedInShow<-count+showCount){
									indexOfSelectedInShow += count;
								}
								p_wptr->INDEXOFSELECTEDINSHOW = indexOfSelectedInShow;

							}else{
								if(indexOfTop+showCount+mousemoveOffsetValue>count){
									if(count>indexOfTop+showCount){
										indexOfSelectedInShow -= count - indexOfTop-showCount;
									}
								
								}else{
									indexOfSelectedInShow -= mousemoveOffsetValue;
								}
							
								if(indexOfSelectedInShow<-count+showCount){
									indexOfSelectedInShow=-count+showCount;
								}
								p_wptr->INDEXOFSELECTEDINSHOW = indexOfSelectedInShow;
							}
						
						}else
						{ //向下
							mousemoveOffsetValue = (mousemoveOffset + singleLength/2)/singleLength;
					
							if(p_wptr->INDEXOFTOP<mousemoveOffsetValue){
								if(cycle){
									p_wptr->INDEXOFTOP = count-mousemoveOffsetValue+p_wptr->INDEXOFTOP;
								}else{
									p_wptr->INDEXOFTOP = 0;
								}
						
							}else{
								p_wptr->INDEXOFTOP -= mousemoveOffsetValue;
							}
						
							if(cycle){
								indexOfSelectedInShow += mousemoveOffsetValue;
								if(indexOfSelectedInShow>count-1){
									indexOfSelectedInShow=indexOfSelectedInShow-count;
								}
								p_wptr->INDEXOFSELECTEDINSHOW = indexOfSelectedInShow;

							}else{
								if(indexOfTop<mousemoveOffsetValue){
									indexOfSelectedInShow += indexOfTop;
								}else{
									indexOfSelectedInShow += mousemoveOffsetValue;
								
								}
								if(indexOfSelectedInShow>count-1){
									indexOfSelectedInShow=count-1;
								}
								p_wptr->INDEXOFSELECTEDINSHOW = indexOfSelectedInShow;
								
							}
							
						}

					}else{ // press then release
				
						if((cur_x>=p_wptr->WidgetOffsetX) && (cur_x<=p_wptr->WidgetOffsetX+(p_wptr->WidgetWidth<<4)) && (cur_y>=p_wptr->WidgetOffsetY) && (cur_y<=p_wptr->WidgetOffsetY+(p_wptr->WidgetHeight<<4))){
							if (arrange)
							{
								p_wptr->INDEXOFSELECTEDINSHOW = (cur_x - p_wptr->WidgetOffsetX)/16/(singleLength/16);
							}
							else
							{
								p_wptr->INDEXOFSELECTEDINSHOW = (cur_y - p_wptr->WidgetOffsetY)/16/(singleLength/16);
							}							
							tag_value = p_wptr->INDEXOFTOP + p_wptr->INDEXOFSELECTEDINSHOW;
							if(tag_value>=count){
								tag_value -= count;
							}
							TagPtr->getTag(p_wptr->BindTagID).setRawValue(tag_value);
						}
					}
				}
				if(p_wptr->BindTagID != 0)
				{
					TagPtr->getTag(p_wptr->BindTagID).setRawValue(tag_value);
				}
				else
				{
					SetValueCtrl(pagePtr, p_wptr, ActionPtr, 0);
				}
				
				p_wptr->SELECTOROFFSET = (u16)0;
				p_wptr->SELECTORSTATUS &= 0x10;//released
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
	}

	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Keyboard.Touch
//**************************************
funcStatus SelectorClass::KeyboardTouchCtrl(
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
// name�� setATag
// set the texture's animation tag
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the animation tag
// addtional comments :
//   none
//-----------------------------
funcStatus SelectorClass::setATag
	(
	WidgetClassPtr p_wptr,  
	u16 ATagValue
	)
{
	u8 ogAlpha;
	s16 ValueStart;
	s16 ValueStop;
	s16 curValue;
	u16 showCount;
	u16 transitionParamIn; //portion of animation progress 0.10
	u16 transitionParamOut; //portion of animation progress after varying function 0.10
	u8 pressed;
	u8 autoHide;
	
	s32 tag_value = 0;
	u16 singleLength;//length of each selector item
	u16 count;
	u8 cycle;
	u8 arrange;
	int selectorMode;
	TextureClassPtr TexturePtr;
	s8 moveDir;
	
	count = p_wptr->SELECTORCOUNT;
	cycle = (p_wptr->SELECTOROPTIONS & 0x2) >> 1;
	arrange = (p_wptr->SELECTOROPTIONS & 0x04)>>2;
	selectorMode = (p_wptr->SELECTOROPTIONS & 0x16)>>4;
	
#ifndef WHOLE_TRIBLE_BUFFER
	RefreshMsg refreshMsg;
#endif

	u16 totalDuration = p_wptr->ANIMATION_DURATION;
	ogAlpha = p_wptr->SELECTORALPHA;
	autoHide = p_wptr->SELECTOROPTIONS & 0x01;
	showCount = p_wptr->SELECTORSHOWCOUNT;
	if(totalDuration == 0 ) //no animation
		return AHMI_FUNC_SUCCESS;
	pressed = p_wptr->SELECTORSTATUS & 1;
	if (pressed)
	{
		//press
		return AHMI_FUNC_SUCCESS;
	}

//	taskENTER_CRITICAL();
#ifdef AHMI_DEBUG
	char text[100];
	sprintf(text,"set meter animation tag %d", ATagValue);
	ERROR_PRINT(text);
#endif
	p_wptr->NOW_DURATION |= ATagValue;
	//type
	if ((p_wptr->SELECTORSTATUS & 0x4) == 0)//animate offsetY
	{
		if(p_wptr->SELECTORSTATUS & 0x10)
		{
			if (s16(p_wptr->SELECTORCURSPEED) >= 0)
			{
				moveDir = 0;
				if(s16(p_wptr->SELECTORCURSPEED) - s16(p_wptr->SwipeFactor) <= 0)
				{
					p_wptr->SELECTORCURSPEED = 0;
				}
				else
				{
					p_wptr->SELECTORCURSPEED = s16(p_wptr->SELECTORCURSPEED) - s16(p_wptr->SwipeFactor);
				}
			}
			else
			{
				moveDir = 1;
				if(s16(p_wptr->SELECTORCURSPEED) + s16(p_wptr->SwipeFactor) >= 0)
				{
					p_wptr->SELECTORCURSPEED = 0;
				}
				else
				{
					p_wptr->SELECTORCURSPEED = s16(p_wptr->SELECTORCURSPEED) + s16(p_wptr->SwipeFactor);
				}
			}

			if(p_wptr->SELECTORCURSPEED == 0)
			{
				TexturePtr = &(gPagePtr[p_wptr->ATTATCH_PAGE].pBasicTextureList[p_wptr->StartNumofTex])+4;
				if(arrange)
				{
					singleLength = ((p_wptr->WidgetWidth / showCount) <<4);
				}
				else
				{
					singleLength = ((p_wptr->WidgetHeight / showCount) <<4);
				}
				
				if(moveDir)
				{
					tag_value = count - 1;
				}
				else
				{
					tag_value = 0;
				}
				for(int k = 0; k < count; k++)
				{
					if((TexturePtr[k].mTexAttr & FORCE_UNDISPLAY) == FORCE_UNDISPLAY)
						continue;
					
					if(arrange)
					{
						if((!cycle) && (((k == 0) && ((TexturePtr[k].OffsetX) > (p_wptr->WidgetWidth << 3))) || ((k == (count - 1)) && ((TexturePtr[k].OffsetX + singleLength) < (p_wptr->WidgetWidth << 3)))))
						{
							tag_value = k;
							break;
						}
						
						if((TexturePtr[k].OffsetX < (p_wptr->WidgetWidth << 3)) && ((TexturePtr[k].OffsetX + singleLength) > (p_wptr->WidgetWidth << 3)))
						{
							tag_value = k;
							break;
						}
					}
					else
					{
						if((!cycle) && (((k == 0) && ((TexturePtr[k].OffsetY) > (p_wptr->WidgetHeight << 3))) || ((k == (count - 1)) && ((TexturePtr[k].OffsetY + singleLength) < (p_wptr->WidgetHeight << 3)))))
						{
							tag_value = k;
							break;
						}
						
						if((TexturePtr[k].OffsetY < (p_wptr->WidgetHeight << 3)) && ((TexturePtr[k].OffsetY + singleLength) > (p_wptr->WidgetHeight << 3)))
						{
							tag_value = k;
							break;
						}
					}
				}
				
				if(selectorMode==1)
				{
					if(p_wptr->BindTagID != 0)
					{
						TagPtr->getTag(p_wptr->BindTagID).setRawValue(tag_value);
					}
					p_wptr->SELECTOROFFSET = 0;
					p_wptr->SELECTORSTATUS = 0;
				}
				else
				{
					if(!cycle)
					{
						if(tag_value == (count - 1))
						{
							tag_value = (tag_value - 2);
						}
						else if(tag_value == 0)
						{
							tag_value = 0;
						}
						else
						{
							tag_value = tag_value - 1;
						}
					}
					else
					{
						if(tag_value == 0)
						{
							tag_value = (count - 1);
						}
						else
						{
							tag_value = tag_value - 1;
						}
					}
					p_wptr->INDEXOFTOP = tag_value;
					TexturePtr += tag_value;
					if (arrange)
					{
						p_wptr->START_TAG_L = (u16)(TexturePtr->OffsetX);
					}else
					{
						p_wptr->START_TAG_L = (u16)(TexturePtr->OffsetY);
					}
					p_wptr->SELECTORSTATUS = 0;
					p_wptr->SELECTORSTATUS |= 0x2;//animating
					p_wptr->ANIMATION_DURATION = p_wptr->STATIC_ANIMATION_DURATION;
					p_wptr->loadTextureWithAnimation(&gPagePtr[p_wptr->ATTATCH_PAGE]);
				}
			}
			else
			{
				s8 flag = 0;
				if(s16(p_wptr->SELECTOROFFSET) > 0)
					flag = 0;
				else
					flag = 1;
				p_wptr->ANIMATIONCURVALUE_L = (u16)p_wptr->SELECTORCURSPEED;
				p_wptr->SELECTOROFFSET += (u16)p_wptr->SELECTORCURSPEED;
				if(cycle)
				{
					if(arrange)
					{
						singleLength = ((p_wptr->WidgetWidth / showCount) <<4);
					}
					else
					{
						singleLength = ((p_wptr->WidgetHeight / showCount) <<4);
					}
					
					if(s16(p_wptr->SELECTOROFFSET) < (singleLength * (showCount - count)))
					{
						p_wptr->SELECTOROFFSET += (singleLength * count);
					}
					if(s16(p_wptr->SELECTOROFFSET) > (singleLength * (count - showCount)))
					{
						p_wptr->SELECTOROFFSET -= (singleLength * count);
					}
				}
				else
				{
					if((s16(p_wptr->SELECTOROFFSET) < 0) && (flag == 0))
					{
						p_wptr->SELECTOROFFSET = 0x7FFF;
						p_wptr->SELECTORCURSPEED = 0;
					}
					else if((s16(p_wptr->SELECTOROFFSET) > 0) && (flag == 1))
					{
						p_wptr->SELECTOROFFSET = 0x8000;
						p_wptr->SELECTORCURSPEED = 0;
					}
				}
				renderTexture(&gPagePtr[p_wptr->ATTATCH_PAGE], p_wptr, (s32)TagPtr->getTag(p_wptr->BindTagID).getValue());
			}
			return AHMI_FUNC_SUCCESS;
		}
		else
		{
			//start and stop value of animation
			ValueStart = (s16)p_wptr->START_TAG_L;
			ValueStop = (s16)p_wptr->STOP_TAG_L;

			if(ValueStart == ValueStop)
			{
				return AHMI_FUNC_SUCCESS;
			}

			transitionParamIn = (ATagValue) * 1024 / totalDuration;//0.10
		
			gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut,p_wptr->TimingFun);

			curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);
			//curValue = ValueStart + (ValueStop - ValueStart) * ATagValue / totalDuration;

			//p_wptr->OldValueL = (u16)curValue;
	
			//record current animation by Mr.z
			p_wptr->ANIMATIONCURVALUE_L = (u16)curValue;
	
			//p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
			//render the texture
			p_wptr->SELECTOROFFSET = (u16)curValue;
			renderTexture(&gPagePtr[p_wptr->ATTATCH_PAGE], p_wptr, (s32)TagPtr->getTag(p_wptr->BindTagID).getValue());

			//finish animating offset
			if(ATagValue >= totalDuration){
				p_wptr->NOW_DURATION &= 0x0000;
				p_wptr->START_TAG_L = 0;

				if (autoHide && (showCount != 1))
				{
					//start alpha animation
					p_wptr->ANIMATIONCURVALUE_H = ogAlpha;
			
					p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;
			
					p_wptr->STOP_TAG_H = 0;
				
					p_wptr->SELECTORSTATUS |= (0x4);
					p_wptr->ANIMATION_DURATION = p_wptr->STATIC_ANIMATION_DURATION;
					p_wptr->loadTextureWithAnimation(&gPagePtr[p_wptr->ATTATCH_PAGE]);
				}else
				{
					if(showCount != 1)
					{
						renderAlpha(&gPagePtr[p_wptr->ATTATCH_PAGE], p_wptr,ogAlpha);
					}
					p_wptr->SELECTORSTATUS &= (~0x2);
				}
			
			}
		
			p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	//		taskEXIT_CRITICAL();
			return AHMI_FUNC_SUCCESS;
		}
	}else{//animate alpha
		//start and stop value of animation
		ValueStart = (s16)p_wptr->START_TAG_H;
		ValueStop = (s16)p_wptr->STOP_TAG_H;

		if(ValueStart == ValueStop)
		{
			return AHMI_FUNC_SUCCESS;
		}

		transitionParamIn = (ATagValue) * 1024 / totalDuration;//0.10
		
		gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut,p_wptr->TimingFun);

		curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);
		//curValue = ValueStart + (ValueStop - ValueStart) * ATagValue / totalDuration;

		//p_wptr->OldValueH = (u16)curValue;
	
		//record current animation by Mr.z
		p_wptr->ANIMATIONCURVALUE_H = (u16)curValue;
	
		//p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
		//render the alpha
		
		renderAlpha(&gPagePtr[p_wptr->ATTATCH_PAGE], p_wptr, curValue);

		if(ATagValue >= totalDuration){
			p_wptr->NOW_DURATION &= 0x0000;
			p_wptr->SELECTORSTATUS &= (~0x4);
			p_wptr->SELECTORSTATUS &= (~0x2);
		}
		
		p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
//		taskEXIT_CRITICAL();
		return AHMI_FUNC_SUCCESS;
	}
}
#endif

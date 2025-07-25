#include <string.h>
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "TextInput.h"
#include "drawImmediately_cd.h"
#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
#include "SBBatchDraw.h"
#endif

#ifdef AHMI_CORE
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;

//extern u16  WorkingPageID;

extern Easing gEasing;

extern QueueHandle_t		ActionInstructionQueue;
extern EncodingClass*		gEncoder;
extern StringClassPtr       StringListPtr;
extern ConfigInfoClass  ConfigData;

#if defined(VERSION_2) && defined(EMBEDDED)
extern uint32_t gNewSoureBufferSize;
#endif

//-----------------------------
// name�� DynamicTexClass
// initialize
//  
//
// addtional comments :
//   none
//-----------------------------
TextInputClass::TextInputClass()
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
TextInputClass::~TextInputClass()
{

}

funcStatus TextInputClass::initWidget(
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
	
	bindTag = &TagPtr->getTag(p_wptr->BindTagID);
	tagtrigger.mTagPtr = bindTag;
	tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag))
	{
		if(widgetCtrl(pagePtr, p_wptr,&tagtrigger,u8_pageRefresh) != AHMI_FUNC_SUCCESS)
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
        
		p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
	}
	
	#if (defined PC_SIM) || (defined CHIP_GC9002_NoHyper)
	updateTextInputDisplay(pagePtr,p_wptr,u32p_sourceShift, pWidgetMatrix,&tagtrigger,u8_pageRefresh,NULL,0, NULL);
	/*if(myWidgetClassInterface.drawTexture(p_wptr,u32p_sourceShift, pWidgetMatrix,RefreshType,pTileBox,staticTextureEn, pagePtr) != AHMI_FUNC_SUCCESS)
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_DrawTextureErr);*/
		
	return AHMI_FUNC_SUCCESS;
	#endif

	#if (defined VERSION_2) && (defined EMBEDDED)   
	Op_sourceShift = *u32p_sourceShift;
	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag) || (p_wptr->REFRESH_FLAG & WidgetSourceBufferRefreshFlag) || (RefreshType == ANIMATION_MATRIXREFRESH_PAGE) || (gScreenOptions.enable))
	{
		bindTag = &TagPtr->getTag(p_wptr->BindTagID);
		// find the bind tag
		tagtrigger.mTagPtr = bindTag;
		tagtrigger.mInputType = ACTION_TAG_SET_VALUE;

		//set the widget value
		status = updateTextInputDisplay(pagePtr,p_wptr,u32p_sourceShift, pWidgetMatrix,&tagtrigger,u8_pageRefresh,NULL,0, NULL);
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
		
		p_wptr->REFRESH_FLAG &= (~WidgetValueRefreshFlag);
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
//WidgetAttr��
//15-8 : reserved
//5-7  : the mode��0��switch between different texture 1��rotate
//4-0  : the type of the widget
//*****************************
FontlayoutEngine gEngine;
Textinputbox gTextinputbox;
LineTrcak gLineTrcak;


funcStatus TextInputClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			  
	ActionTriggerClassPtr ActionPtr,  
	u8 u8_pageRefresh				  
	)
{
	u16 scrollDuration = p_wptr->ScrollDuration;
	u16 scrollDelay = p_wptr->ScrollDelay;
	u8 scrollStatus = (p_wptr->ScrollOptions >> 4) & 3; //0:idle 1:delay 2:scroll

//	u8 scrollMode = (p_wptr->ScrollOptions >> 3) & 1;

//	int mode = p_wptr->NumOfLine & 0xff; 

	u8 shouldScroll = 1;

	//parameter judge
	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: NULL pointer");
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
		ERROR_PRINT("ERROR: when drawing number widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}
	
	//text scroll
	if (scrollDuration)
	{
		p_wptr->START_TAG_L = 0; // anmation offset
		updateTextInputDisplay(pagePtr, p_wptr, NULL, NULL, ActionPtr, u8_pageRefresh, &(p_wptr->START_TAG_H), 1, &shouldScroll); // text total width or height
		
		if (shouldScroll)
		{
			if (scrollStatus == 0)//idle
			{
				if (scrollDelay > 0)
				{
					//run delay
					p_wptr->ANIMATION_DURATION = scrollDelay;
					p_wptr->ScrollOptions = ((p_wptr->ScrollOptions & (~(3<<4))) | (1 << 4));
				}
				else
				{
					//run scroll
					p_wptr->ANIMATION_DURATION = scrollDuration;
					p_wptr->ScrollOptions = ((p_wptr->ScrollOptions & (~(3<<4))) | (2 << 4));
				}
				p_wptr->loadTextureWithAnimation(pagePtr);
			}
			else if (scrollStatus == 2 || scrollStatus == 1)//scroll or delay
			{
				//run scroll
				p_wptr->ANIMATION_DURATION = scrollDuration;
				p_wptr->ScrollOptions = ((p_wptr->ScrollOptions & (~(3<<4))) | (2 << 4));
				p_wptr->loadTextureWithAnimation(pagePtr);
			}
		}
		else
		{
			p_wptr->clearAnimation();
			p_wptr->ANIMATION_DURATION = 0;
			p_wptr->ScrollOptions = ((p_wptr->ScrollOptions & (~(3<<4))) | (0 << 4));
			//p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
		}
		
	}
	
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
//*****************************
funcStatus TextInputClass::updateTextInputDisplay(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			
	u32 *u32p_sourceShift,
	NewMatrixPtr pWidgetMatrix,
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh,
	u16* totalLength,
	u8 noDraw,
	u8* shouldScroll
	)
{
	funcStatus status;
	#if (defined VERSION_2) && (defined EMBEDDED)  
	u32 mSourceBufferStartPos;
	u32 mSourceBufferLength = 0;
	
	if(!noDraw)
	{
		mSourceBufferStartPos = *u32p_sourceShift;
		p_wptr->SourceBufferStartPos = mSourceBufferStartPos;
	}
	#endif

	s16 animationOffset = p_wptr->START_TAG_L;
//	u16 scrollDuration = p_wptr->ScrollDuration;
//	u8 scrollDirection = p_wptr->ScrollOptions & 3;
	s16 fontRangeID = p_wptr->TextInputFontRangeID;

	int mode = p_wptr->NumOfLine & 0xff;            //          single line or more lines
	int direction = (p_wptr->NumOfLine >> 10) & 0x1;

	u8 scrollMode = (p_wptr->ScrollOptions >> 3) & 1;

	u8 _shouldScroll = 1;

	if (mode == 0)
	{
		TextureClassPtr texturePtr; //texture pointer
		TagClassPtr curTag;
		u32 codePoints[480];
		int spacing;   //spacing
		unsigned char spacing1;
		signed char spacing2;
		int halfSpacing;
		unsigned char halfSpacing1;
		signed char halfSpacing2;
		int curSpacing;
		int fontWidth;
//		int fontHeight;
		int fontPadding;
		u8 * curString;
//		u32 value;
		u32 code;         //ASIC-0x20			
		u16 numofNumber;	
		s16 leftPoint;
		u16	i;
		u8 align;        //align mode
		u8 lastLetterType=0, curLetterType=0;
		s32 totalCharLength = -1;
		s16 leftOffset;
		#if defined(CHIP_GC9002)
		u8 TexNumHasDraw = 0;
		#endif
		

		curTag = ActionPtr->mTagPtr;
//		value = curTag->getValue();
		align = (p_wptr->NumOfLine >> 8) & 0x3;
	
		//draw puremask texture
		texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);
		if (!noDraw)
		{
			status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
			if(status == AHMI_FUNC_MATRIX_ILLEGAL)
			{
				return status;
			}
		}
		

		//draw background texture
		texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 1]);
		if (!noDraw)
		{
			status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
			if(status == AHMI_FUNC_MATRIX_ILLEGAL)
			{
				return status;
			}
		}
		

		//get alpha texture
		texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 2]);

		curString = curTag->getStringValue();
		//numofNumber = StringListPtr[curTag->getValue()].length;
		numofNumber = curTag->mValueLen;
		if (curString == NULL)
		{
			//no string
			return AHMI_FUNC_SUCCESS;
		}

		fontWidth = p_wptr->TextInputFontWidth;
//		fontHeight = p_wptr->TextInputFontHeight;
		fontPadding = (texturePtr->TexWidth - fontWidth)/2;

		spacing1 = ((unsigned char)(p_wptr->SPACINGX & 0xff));
		spacing2 = (signed char)spacing1;
		spacing = spacing2;

		halfSpacing1 = ((unsigned char)(p_wptr->SPACINGX >> 8));
		halfSpacing2 = (signed char)halfSpacing1;
		halfSpacing = halfSpacing2;

		//leftPoint = (s16)p_wptr->WidgetOffsetX;
		leftPoint = 0;
		memset(codePoints, 0, 480 * sizeof(u32));
		gEncoder->trasnU8ArrayToEncodableArray(ConfigData.encoding,curString,codePoints,numofNumber,direction);

		for(i=0;i<numofNumber;i++)
		{
			if(codePoints[i] == 0)
				break;
			
			code = gEncoder->getCharacterIndexByCodePoint(ConfigData.encoding, codePoints[i],fontRangeID);
			if (code < ' ')
			{
				code = ' ';
				if((totalCharLength==-1)){
					totalCharLength = leftPoint;
				}
			}

			//layout
			//get curLetterType
			if (codePoints[i]<128)
			{
				curLetterType = 0;
			}else{
				curLetterType = 1;
			}
			if (i == 0)
			{
				curSpacing = 0;
				curSpacing -= fontPadding;
			}else{
			
				if (lastLetterType == 0)
				{
					curSpacing = curLetterType ? (spacing + halfSpacing)/2 : halfSpacing;
					curSpacing += fontWidth;// - fontPadding;
				}else{
					curSpacing = curLetterType ? spacing : (spacing + halfSpacing)/2;
					curSpacing += fontWidth;// - fontPadding;
				}
			}
			leftPoint += curSpacing;
			lastLetterType = curLetterType;
		}
		if(totalCharLength==-1){
			totalCharLength = leftPoint;
		}

		totalCharLength = totalCharLength + fontWidth + fontPadding;

		if (totalLength != NULL)
		{
			*totalLength = totalCharLength;
		}

		if (scrollMode && p_wptr->START_TAG_H <= p_wptr->WidgetWidth )
		{
			_shouldScroll = 0;
			
		}
		if (shouldScroll != NULL)
		{
			*shouldScroll = _shouldScroll;
		}

		if(direction)
		{
			switch (align)
			{
				case CENTERALIGN:
					leftOffset = (p_wptr->WidgetWidth - totalCharLength)/2;
					break;
				case LEFTALIGN:
					leftOffset = p_wptr->WidgetWidth - totalCharLength;
					break;
				case RIGHTALIGN:
				default:
					leftOffset = 0;
					break;
			}
		}
		else
		{
			switch (align)
			{
				case CENTERALIGN:
					leftOffset = (p_wptr->WidgetWidth - totalCharLength)/2;
					break;
				case RIGHTALIGN:
					leftOffset = p_wptr->WidgetWidth - totalCharLength;
					break;
				case LEFTALIGN:
				default:
					leftOffset = 0;
					break;
			}
		}

		leftOffset += animationOffset;

		leftPoint = 0;
		for(i=0;i<numofNumber;i++)
		{
			if(codePoints[i] == 0)
				break;
			
			code = gEncoder->getCharacterIndexByCodePoint(ConfigData.encoding, codePoints[i], fontRangeID);
			if (code < ' ')
			{
				if((codePoints[i] != 0) && (codePoints[i] != 0x0A))
					code = '?';
				else
					code = ' ';
			}
			code = code - ' ';

			texturePtr->FocusedSlice = code;
			//layout
			//get curLetterType
			if (codePoints[i]<128)
			{
				curLetterType = 0;
			}else{
				curLetterType = 1;
			}
			if (i == 0)
			{
				curSpacing = 0;
				curSpacing -= fontPadding;
			}else{
			
				if (lastLetterType == 0)
				{
					curSpacing = curLetterType ? (spacing + halfSpacing)/2 : halfSpacing;
					curSpacing += fontWidth;// - fontPadding;
				}else{
					curSpacing = curLetterType ? spacing : (spacing + halfSpacing)/2;
					curSpacing += fontWidth;// - fontPadding;
				}
			}
			leftPoint += curSpacing;
			lastLetterType = curLetterType;

			if (code == 14)
			{
				//point
				texturePtr->OffsetX = (leftPoint + (texturePtr->TexWidth)/5) << 4; //dot is left by 20% of texWidth
				/*texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
				texturePtr->TexRightTileBox = (texturePtr->OffsetX - fontWidth/2 + texturePtr->TexWidth * 16) >> 9;*/
			}
			else
			{
				texturePtr->OffsetX = leftPoint << 4;
				/*texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
				texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;*/
			}

			if((texturePtr->OffsetX >> 4) + leftOffset > p_wptr->WidgetWidth)
			{
				for(; i < numofNumber; i++)
				{
					continue;
				}
				break;
			}
			texturePtr->OffsetX += leftOffset << 4;
			texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
			texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;

			if (code != 0)
			{
				if (!noDraw)
				{
					status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
					if(status == AHMI_FUNC_MATRIX_ILLEGAL)
					{
						return status;
					}
					#if defined(CHIP_GC9002)
					TexNumHasDraw++;
					//SBBatchDraw background is one longsourcebuffer(64bytes), so 28 = (2048 - 64) / 64 - 3, 3 means puremask + textinputBackgroundPic + purecolor
					if(TexNumHasDraw > 28)
						break;
					#endif
				}
				
			}
		}

		//draw purecolor texture
		texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 3]);
		if (!noDraw)
		{
			status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
			if(status == AHMI_FUNC_MATRIX_ILLEGAL)
			{
				return status;
			}
		}
		
	}
	else if (mode == 1)
	{
		LineTrcak *linetrack;
		Textinputbox *box;
		TextureClassPtr texturePtr; //texture pointer
		TagClassPtr curTag;
		u32 codePoints[480];
		unsigned char spacing;
		unsigned char halfSpacing;
		int fontWidth;
//		int fontHeight;
		u8 * curString;
//		u32 code;         //ASIC-0x20	
		Span spans[1];
		ParagraphAttrs paragraphAttrs;
		u8 TexNumHasDraw = 0;
		int totalTextLength = 0;
		
		FontlayoutEngine *Engine = &gEngine;
		memset((void *)Engine, 0, sizeof(FontlayoutEngine));

		memset(codePoints, 0, 480 * sizeof(u32));


		//Engine->align = (p_wptr->NumOfLine >> 8) & 0xf;
	
		//draw puremask texture
		texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);
		if (!noDraw)
		{
			status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
			if(status == AHMI_FUNC_MATRIX_ILLEGAL)
			{
				return status;
			}
		}
		

		//draw background texture
		texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 1]);
		if (!noDraw)
		{
			status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
			if(status == AHMI_FUNC_MATRIX_ILLEGAL)
			{
				return status;
			}
		}
		

		//get alpha texture
		texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 2]);

		curTag = ActionPtr->mTagPtr;
		curString = curTag->getStringValue();
		Engine->numofNumber = curTag->mValueLen;
		//Engine->numofNumber = StringListPtr[curTag->getValue()].length;

		fontWidth = p_wptr->TextInputFontWidth;
//		fontHeight = p_wptr->TextInputFontHeight;
		//Engine->fontsize = fontWidth;

		/*Engine->indentationLeft = 0;
		Engine->indentationRight = 0;
		Engine->firstLineIndentation = 0;
		Engine->spacingBetweenLines = p_wptr->TextInputLineSpacing;*/

		paragraphAttrs.indentationLeft = 0;
		paragraphAttrs.indentationRight = 0;
		paragraphAttrs.firstLineIndentation = 0;
		paragraphAttrs.spacingBetweenLines = p_wptr->TextInputLineSpacing;
		paragraphAttrs.direction = (p_wptr->NumOfLine >> 10) & 0x1;
		paragraphAttrs.align = (p_wptr->NumOfLine >> 8) & 0x3;
		paragraphAttrs.verticalAlign = (p_wptr->NumOfLine >> 12) & 0x3;

		spacing = ((unsigned char)(p_wptr->SPACINGX & 0xff));
		//Engine->fullspacing = (signed char)spacing;

		halfSpacing = ((unsigned char)(p_wptr->SPACINGX >> 8));
		//Engine->halfspacing = (signed char)halfSpacing;

		box = &gTextinputbox;
		memset((void *)box, 0, sizeof(Textinputbox));
		box->initialBox(0,animationOffset,p_wptr->WidgetWidth,p_wptr->WidgetHeight);

		linetrack = &gLineTrcak;
		memset((void *)linetrack, 0, sizeof(LineTrcak));
		linetrack->initialLineTrack(box->x,box->y,0,0,0,0);

		gEncoder->trasnU8ArrayToEncodableArray(ConfigData.encoding,curString,codePoints,Engine->numofNumber);

		spans[0].codepoints = codePoints;
		spans[0].codepointsNum = 480;
		spans[0].fontsize = fontWidth;
		spans[0].fullspacing = (signed char)spacing;
		spans[0].halfspacing = (signed char)halfSpacing;
		spans[0].textureptr = texturePtr;
		Engine->preCalculateBoxY(box,paragraphAttrs,spans,1,fontRangeID,linetrack,&totalTextLength);
		status = Engine->layoutlinesByOneTexture(box, paragraphAttrs, spans, 1 , fontRangeID, linetrack,p_wptr,u32p_sourceShift,pWidgetMatrix, noDraw, &TexNumHasDraw);
		if(status == AHMI_FUNC_MATRIX_ILLEGAL)
		{
			return status;
		}
		if (totalLength != NULL)
		{
			*totalLength = totalTextLength;
		}
		
		if (scrollMode && p_wptr->START_TAG_H <= p_wptr->WidgetHeight )
		{
			_shouldScroll = 0;
		}

		if (shouldScroll != NULL)
		{
			*shouldScroll = _shouldScroll;
		}

		//draw purecolor texture
		texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 3]);
		if (!noDraw)
		{
			status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
			if(status == AHMI_FUNC_MATRIX_ILLEGAL)
			{
				return status;
			}
		}
		
	}
	
	#if (defined VERSION_2) && (defined EMBEDDED)
	if(!noDraw)
	{
		#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
		if(BatchDraw_u8GetBatchDrawType() == BatchDraw_SB)
		{
			mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
			p_wptr->SourceBufferLength = mSourceBufferLength;
			if((*u32p_sourceShift - SBBatchDraw_u32GetSBBatchDrawLastSBPos()) > SBBatchDraw_u32GetSBBatchDrawMaxSize())
			{
				SBBatchDraw_vStartSBBatchDraw(mSourceBufferStartPos);
			}
		}
		else if(BatchDraw_u8GetBatchDrawType() == BatchDraw_FB)
		{
			mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
			p_wptr->SourceBufferLength &= 0xF000;
			p_wptr->SourceBufferLength |= mSourceBufferLength;
			PointClass p0(0, 0);
			PointClass p1((p_wptr->WidgetWidth) << 4, 0);
			PointClass p2((p_wptr->WidgetWidth) << 4, (p_wptr->WidgetHeight) << 4);
			PointClass p3(0, (p_wptr->WidgetHeight) << 4);
			p0.rightMulMatrix(pWidgetMatrix);
			p1.rightMulMatrix(pWidgetMatrix);
			p2.rightMulMatrix(pWidgetMatrix);
			p3.rightMulMatrix(pWidgetMatrix);
			FBBatchDraw_vCopyWidgetSB(((p0.mPointX << 12) & 0xFFFF0000) | (p0.mPointY >> 4), 
									  ((p1.mPointX << 12) & 0xFFFF0000) | (p1.mPointY >> 4),
									  ((p2.mPointX << 12) & 0xFFFF0000) | (p2.mPointY >> 4), 
									  ((p3.mPointX << 12) & 0xFFFF0000) | (p3.mPointY >> 4), 
									  (p_wptr->SourceBufferStartPos & 0x7FFF), &(p_wptr->SourceBufferLength));
		}
		#else
		mSourceBufferLength = *u32p_sourceShift - mSourceBufferStartPos;
		p_wptr->SourceBufferLength = mSourceBufferLength;
		#endif
		if(gNewSoureBufferSize == 64)
		{
			p_wptr->SourceBufferStartPos &= (~0x8000);
		}
		else if(gNewSoureBufferSize == 24)
		{
			p_wptr->SourceBufferStartPos |= 0x8000;
		}
		else
		{
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_CopySourcebufferErr);
		}
	}
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
//4-0: type of widget, button is TEXT_INPUT
//**************************************
funcStatus TextInputClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    u8 u8_widgetRefresh      
    )
{
	u16 scrollDelay = p_wptr->ScrollDelay;
	
	if(scrollDelay != 0)
		p_wptr->ScrollOptions &= 0xFFCF;//(p_wptr->ScrollOptions >> 3) & 3; //0:idle 1:delay 2:scroll
	
    p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
    
	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Mouse.Touch
//The widget don't support touch action.
//**************************************
funcStatus TextInputClass::MouseTouchCtrl(
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
// name�� setColor
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
funcStatus TextInputClass::setColor(
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
	colorTexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + p_wptr->NumOfTex-1]);
	color = (a<<24) + (r<<16) + (g<<8) + b;
	colorTexturePtr->TexAddr = color;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	return AHMI_FUNC_SUCCESS;
}



funcStatus TextInputClass::setATag
	(
	WidgetClassPtr p_wptr,  
	u16 ATagValue
	)
{
	s32 ValueStart;
	s32 ValueStop;
	s32 curValue;
//	u8 onOffValue;
//	u8 onOffBit;
	
#ifndef WHOLE_TRIBLE_BUFFER
	RefreshMsg refreshMsg;
#endif
	u16 transitionParamIn;
	u16 transitionParamOut;
	u16 scrollDuration = p_wptr->ScrollDuration;
	u16 scrollDelay = p_wptr->ScrollDelay;
	u8 scrollStatus = (p_wptr->ScrollOptions >> 4) & 3;
	u16 textWidth ;
	u16 textHeight ;
	u16 widgetWidth = p_wptr->WidgetWidth;
	u16 widgetHeight = p_wptr->WidgetHeight;
	s16 offset = 0;
//	funcStatus status;
	
	if(scrollDuration == 0 ) //no animation
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetAnimationDurationErr);

	if (p_wptr->ANIMATION_DURATION == 0)
	{
		//cancel animation
		return AHMI_FUNC_SUCCESS;
	}

//	taskENTER_CRITICAL();
#ifdef AHMI_DEBUG
	char text[100];
	sprintf(text,"set meter animation tag %d", ATagValue);
	ERROR_PRINT(text);
#endif
	p_wptr->NOW_DURATION |= ATagValue;

	//int direction = (p_wptr->NumOfLine >> 10) & 0x1;
	//u8 align = (p_wptr->NumOfLine >> 8) & 0x3;
	s16 leftOffset;
	
	//record current animation
	//p_wptr->ANIMATIONCURVALUE_L = (u16)ATagValue;
	//p_wptr->ANIMATIONCURVALUE_H = (u16)(ATagValue >> 16);

	//calculate offset with variables
	//u16 scrollDuration = p_wptr->ANIMATION_DURATION;
	u8 scrollDirection = p_wptr->ScrollOptions & 3;
	u8 scrollAutoReverse = (p_wptr->ScrollOptions & 4)>>2;
	
	if (scrollStatus == 2)
	{
		//scroll
		ValueStart = 0;
		ValueStop = 0;
		transitionParamIn = (ATagValue) * 1024 / scrollDuration;//0.10
		gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut,p_wptr->TimingFun);
		
		if (scrollDirection == 0 || scrollDirection == 1)
		{
			/*ValueStop = widgetWidth + textWidth;
			curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);*/
			textWidth = p_wptr->START_TAG_H;
			textHeight = p_wptr->WidgetHeight;
			switch (scrollDirection)
			{
				case 0://"<-"
					leftOffset = 0;
					break;
				case 1://"->"
					leftOffset = p_wptr->WidgetWidth - textWidth;
					break;
				default:
					leftOffset = 0;
					break;
			}
//			if(direction)
//			{
//				switch (align)
//				{
//					case CENTERALIGN:
//						leftOffset = (p_wptr->WidgetWidth - textWidth)/2;
//						break;
//					case LEFTALIGN:
//						leftOffset = p_wptr->WidgetWidth - textWidth;
//						break;
//					case RIGHTALIGN:
//					default:
//						leftOffset = 0;
//						break;
//				}
//			}
//			else
//			{
//				switch (align)
//				{
//					case CENTERALIGN:
//						leftOffset = (p_wptr->WidgetWidth - textWidth)/2;
//						break;
//					case RIGHTALIGN:
//						leftOffset = p_wptr->WidgetWidth - textWidth;
//						break;
//					case LEFTALIGN:
//					default:
//						leftOffset = 0;
//						break;
//				}
//			}
			if(scrollAutoReverse)
			{
				if(textWidth < widgetWidth)
				{
					textWidth = widgetWidth;
				}
				ValueStop = 2*(textWidth - widgetWidth);
				curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);
				if(scrollDirection == 1)
				{
					if(curValue > textWidth-widgetWidth)
					{
						offset = (ValueStop - curValue) + widgetWidth - textWidth;
					}
					else
					{
						offset = curValue + widgetWidth - textWidth;
					}
				}
				else
				{
					if(curValue > textWidth-widgetWidth)
					{
						offset = -(ValueStop - curValue);
					}
					else
					{
						offset = -curValue;
					}
				}
			}else{
				ValueStop = textWidth + widgetWidth;
				curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);
				if(scrollDirection == 1)
				{
					if(curValue > textWidth)
					{
						offset = -widgetWidth + (curValue - textWidth)+widgetWidth-textWidth;
					}
					else
					{
						offset =curValue+widgetWidth-textWidth;
					}
				}
				else
				{
					if(curValue > textWidth)
					{
						offset = widgetWidth - (curValue - textWidth);
					}
					else
					{
						offset = -curValue;
					}
				}
			}
			p_wptr->START_TAG_L = offset - leftOffset;
			/*texturePtr->OffsetX = offset << 4;
			texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
			texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;*/
			/*status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
			if(status == AHMI_FUNC_MATRIX_ILLEGAL)
			{
				return status;
			}*/
		}
		else if(scrollDirection == 2 || scrollDirection == 3)
		{
			textWidth = p_wptr->WidgetWidth;
			textHeight = p_wptr->START_TAG_H;
			if(scrollAutoReverse)
			{
				if(textHeight < widgetHeight)
				{
					textHeight = widgetHeight;
				}
				ValueStop = 2*(textHeight - widgetHeight);
				curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);
				if(scrollDirection == 3)
				{
					if(curValue > textHeight-widgetHeight)
					{
						offset = (ValueStop - curValue);
					}
					else
					{
						offset = curValue;
					}
				}
				else
				{
					if(curValue > textHeight-widgetHeight)
					{
						offset = -(ValueStop - curValue);
					}
					else
					{
						offset = -curValue;
					}
				}
			}else{
				ValueStop = textHeight + widgetHeight;
				curValue = ValueStart + ((ValueStop - ValueStart) * transitionParamOut >> 10);
				if(scrollDirection == 3)
				{
					if(curValue > textHeight)
					{
						offset = -widgetHeight + (curValue - textHeight)+widgetHeight-textHeight;
					}
					else
					{
						offset =curValue+widgetHeight-textHeight;
					}
				}
				else
				{
					if(curValue > textHeight)
					{
						offset = widgetHeight - (curValue - textHeight);
					}
					else
					{
						offset = -curValue;
					}
				}
			}
			p_wptr->START_TAG_L = offset;
			/*texturePtr->OffsetY = offset << 4;
			texturePtr->TexTopTileBox = texturePtr->OffsetY >> 9;
			texturePtr->TexButtomTileBox = (texturePtr->OffsetY + texturePtr->TexHeight * 16) >> 9;*/
		}
	}
	
	if (scrollStatus == 1)
	{
		if(ATagValue >= scrollDelay)
		{
			//p_wptr->NOW_DURATION &= 0x0000;
			/*p_wptr->STOP_TAG_L = scrollDuration;
			p_wptr->STOP_TAG_H =  scrollDuration >> 16;*/
			p_wptr->ANIMATION_DURATION = scrollDuration;
			p_wptr->ScrollOptions = ((p_wptr->ScrollOptions & (~(3<<4))) | (2 << 4));
			p_wptr->loadTextureWithAnimation(&gPagePtr[p_wptr->ATTATCH_PAGE]);
		}
	}
	else
	{
		if(ATagValue >= scrollDuration)
		{
			//p_wptr->NOW_DURATION &= 0x0000;
			p_wptr->loadTextureWithAnimation(&gPagePtr[p_wptr->ATTATCH_PAGE]);
		}
	}
	
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
//	taskEXIT_CRITICAL();
	
	return AHMI_FUNC_SUCCESS;
}

#endif

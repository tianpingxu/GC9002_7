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
//   2018/10/25 add comments by ZhouYuzhi
// 
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "NumberClass.h"
#include "publicType.h"
#include "drawImmediately_cd.h"
#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
#include "SBBatchDraw.h"
#endif

#ifdef AHMI_CORE
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
//extern u16  WorkingPageID;

extern QueueHandle_t		ActionInstructionQueue;

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;
extern u8 animationDuration;
extern Easing gEasing;

#if defined(VERSION_2) && defined(EMBEDDED)
extern uint32_t gNewSoureBufferSize;
#endif

//-----------------------------
// name�� NumberClass
// construct
//  
//
// addtional comments :
//   none
//-----------------------------
NumberClass::NumberClass()
{

}

//-----------------------------
// name�� ~NumberClass
// deconstruct
//  
//
// addtional comments :
//   none
//-----------------------------
NumberClass::~NumberClass()
{

}


funcStatus NumberClass::initWidget(
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

	#if (defined PC_SIM) || (defined CHIP_GC9002_NoHyper)
	bindTag = &TagPtr->getTag(p_wptr->BindTagID);
	tagtrigger.mTagPtr = bindTag;
	tagtrigger.mInputType = ACTION_TAG_SET_VALUE;
	updateNumberDisplay(pagePtr,p_wptr,u32p_sourceShift, pWidgetMatrix,&tagtrigger,u8_pageRefresh);
	
	return AHMI_FUNC_SUCCESS;
	#endif
	
	#if (defined VERSION_2) && (defined EMBEDDED)
	Op_sourceShift = *u32p_sourceShift;
	if((u8_pageRefresh) || (p_wptr->REFRESH_FLAG & WidgetValueRefreshFlag) || (p_wptr->REFRESH_FLAG & WidgetSourceBufferRefreshFlag) || (RefreshType == ANIMATION_MATRIXREFRESH_PAGE) || (gScreenOptions.enable))
	{
        bindTag = &TagPtr->getTag(p_wptr->BindTagID);
		tagtrigger.mTagPtr = bindTag;
		tagtrigger.mInputType = ACTION_TAG_SET_VALUE;

		//set the widget value
		status = updateNumberDisplay(pagePtr,p_wptr,u32p_sourceShift, pWidgetMatrix,&tagtrigger,u8_pageRefresh);
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

#if 0
//***********************************
// widget for display number
// display 10 number most. 
// support display pointer. 
// support display minus
// support display leading zeros
// support animation: two lines of number, moving up or down
// the type of alpha is ALPHA4
// 0. 0 PUREMASK to clear alpha buffer
// 1-N. alpha4 for number
// N+1. PURECOLOR for display
//WidgetAttr:
//14:sign mode��0 for unsigned number��range 4294967295~0; 1 for signed number��range 2147483647~-2147483648
//13:leading zeros mode, 0 not display leading, 1 for display
//12-9:how many fraction number to display, 
//8-5:total number for display
//4-0:widget type, 0x8
//**********************************
funcStatus NumberClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{
    u64 tens;
	u32 value;
	u32 uValue[5] = {0};
	s32 sValue[5] = {0};
	s32	maxValue;
	s32	minValue;
	s32 curValue;
	s32 j;
    TextureClassPtr nextTexturePtr, texturePtr; //texture pointer
	DynamicPageClassPtr pCurrentPage;//currentpage pointer
    u16 widthOfFont;   //width of dont
	u16 showNumWidth;
	u16 shiftSize;     //shifting size, by align mode 12.4
	u16 code;         //ASIC-0x20
	s16 temp;
    u8  leadZero;
	u8  sign;
	u8  pointPosRight;
	u8	pointPosLeft;
	u8  numofNumber;
	u8  numofUnitString;
	u32 UnitString = 0;
	u8	i;
	u8	showingZero; //incates the rest of number is to show zero. for example, 1.000, after point it the rest of number is all 0
	u8	signFlag;     //whether the sign has been shown
	                  // 0 not show��1 shown
	u8	dataMinus;    // positive or negtive, 1 is negtive,0 is positive or the sign has been shown
	u8  numOfDisTexture; //the number of texture needed to be display
	u8  nextNumOfDisTexture; //the number of texture needed to be display in the second line
	u8  difOfNumber;   //the total number - the number to be display	
	u8 overflow;     //exceeds max
	u8 align;        //align mode
	u8 overflowStyle;    //the mode after exceeds 0��display max value  1��donnot display
	u8 numRadix;    // radix 0-ten radix  1-hex radix
	u8 markingMode; // whether to display 0x for hex   0-display  1-no display
	u8 transformMode; // whether to display upper case  0-low  1-up
	u8 markingPos; // the position of sign in texture
	u8 nextMarkingPos;
	u8 radixBase = 10;
	u8 upperLetterOffset = 7;   // upper case relative to number in ASIC 
	u8 lowerLetterOffset = 39;  // lower case relative to number in ASIC
	u8 movingDir;  //0 down, 1 up
//	u8 offsetpoint=0;
	s16 SpacingX = (s8)(p_wptr->SPACINGX & 0xFF);
	u8 fontWidth = p_wptr->SPACINGX >> 8;
	u8 paddingX = (fontWidth + 9) / 10;
	s16 xCoordinate;
	u8 fontPadding = 0;
	u8 dotLayout = 0;
	u8 unitflag = 0;
	u8 enableWaiting; // whether enabling waiting value
	s32 waitingValue; // waiting value
	u8 hitWaitingValue = 0; //if enable waiting value, whether current value hit waiting value
	u8 nextHitWaitingValue = 0;//animaton mode, hit waiting value

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

	numOfDisTexture = 0;
	nextNumOfDisTexture = 0;
	difOfNumber = 0;
	widthOfFont = 0;
	shiftSize = 0;
	overflow = 0;

	value = ActionPtr->mTagPtr->getValue();

	leadZero = (p_wptr->WidgetAttr & 0x2000 )?1:0;
	sign = (p_wptr->WidgetAttr & 0x4000 )?1:0;
	pointPosRight = (p_wptr->WidgetAttr >>9) & 0xf;
	dotLayout = p_wptr->DOTOPTIONS & 0x01;
	numofNumber = (p_wptr->WidgetAttr >>5) & 0xf;
	numofUnitString = p_wptr->UnitStringLength;

	align = (p_wptr->NumOfLine >> 8) & 0xf;
	overflowStyle = (p_wptr->NumOfLine >> 12) & 0xf;

	if(numofUnitString != 0)
	{
		unitflag = 1;
		UnitString = (p_wptr->UnitStringH << 16) | p_wptr->UnitStringL;
	}

	////the num of number only supports 1-10
	//if(numofNumber > 10 || numofNumber < 1){
	//	ERROR_PRINT("ERROR: don't support such length of number");
	//	return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
	//}
	//the number of fraction number must be 0-(num-1), connot be negtive
	if(/*pointPosRight < 0 || */ pointPosRight > numofNumber - 1){
		ERROR_PRINT("ERROR: the numbers of decimal is wrong");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
	}
	
	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);

	//sign mode
	if(sign)
	{
		if((s32)value > (s32)maxValue)
		{
			value = (s32)maxValue;
			overflow = 1;
		}
		if((s32)value < (s32)minValue)
		{
			value = (s32)minValue;
			overflow = 1;
		}
	}
	else
	{
		// none sign mode, if the value is negtive, transform it into positive
		if((s32)value > (s32)maxValue)
		{
			value = (s32)maxValue;
			overflow = 1;
		}
		if((s32)value < (s32)minValue)
		{
			value = (s32)minValue;
			overflow = 1;
		}
		if((s32)value < 0){
			value = -value;
		}
	}

	numRadix =  (u8)(p_wptr->HexControl& 0x000f);
	markingMode = (u8)((p_wptr->HexControl & 0x00f0)>>4);
	transformMode = (u8)((p_wptr->HexControl & 0x0f00)>>8);

	enableWaiting = p_wptr->NumberWaitingValue & 0x1;
	temp = (s16)(p_wptr->NumberWaitingValueH);
	waitingValue = (temp << 15) | (s16)(p_wptr->NumberWaitingValue>>1);

	if(u8_pageRefresh)
	{
		p_wptr->CurValueL = (u16)value;
		p_wptr->CurValueH = (u16)(value >> 16);
	}

	if((p_wptr->NumOfLine & 0xff) == 0 || (p_wptr->NumOfLine & 0xff) == 1) //no animation
		curValue = value;
	else
		curValue = (s32)((p_wptr->CurValueH << 16) + p_wptr->CurValueL);

	//hex
	if(numRadix){
		radixBase = 16;
		if(markingMode==1){
			numOfDisTexture +=2;
			// cacualte the position of 0x
			u32 tempValue = curValue;
			u8 count = 0;
			if(tempValue == 0)
			{
				count++;
			}
			else
			{
				while (tempValue>0)
				{
					tempValue /= radixBase;
					count++;
				}
			}

			if (leadZero)
			{
				markingPos = 1;
			}else
			{
				markingPos = numofNumber - 1 - count;
			}
			
			

			//next line
			nextNumOfDisTexture += 2;
			tempValue = value;
			count = 0;
			if(tempValue == 0)
			{
				count++;
			}
			else
			{
				while (tempValue>0)
				{
					tempValue /= radixBase;
					count++;
				}
			}
			if (leadZero)
			{
				nextMarkingPos = 1;
			}else
			{
				nextMarkingPos = numofNumber - 1 - count;
			}
			
		}
	}

	//sign mode
	if(sign)
	{
		if((s32)value > (s32)maxValue)
		{
			value = (s32)maxValue;
			overflow = 1;
		}
		if((s32)value < (s32)minValue)
		{
			value = (s32)minValue;
			overflow = 1;
		}
		if (enableWaiting && waitingValue == value)
		{
			hitWaitingValue = 1;
		}
	}
	else
	{
		// none sign mode, if the value is negtive, transform it into positive
		if((s32)value > (s32)maxValue)
		{
			value = (s32)maxValue;
			overflow = 1;
		}
		if((s32)value < (s32)minValue)
		{
			value = (s32)minValue;
			overflow = 1;
		}
		if (enableWaiting && waitingValue == value)
		{
			hitWaitingValue = 1;
		}
		if((s32)value < 0){
			value = -(s32)value;
		}
	}

	if (enableWaiting && curValue == waitingValue)
	{
		nextHitWaitingValue = 1; //curValue is old value for animation
	}

	if(leadZero == 0 && !nextHitWaitingValue)
	{
		if(curValue == 0)
			numOfDisTexture += 1;
		else
		{
			numOfDisTexture += 0;
			for(j = (s32)curValue > 0 ? curValue : -curValue; j >0; j /= radixBase)
			{
				numOfDisTexture++;
			}
		}
		if(pointPosRight != 0) //at least display (number of fraction + 1)
			if(numOfDisTexture < pointPosRight+1)
				numOfDisTexture = pointPosRight+1;
	}
	else 
		numOfDisTexture = numofNumber;

	if(leadZero == 0 && !hitWaitingValue)
	{
		if(value == 0)
			nextNumOfDisTexture += 1;
		else
		{
			nextNumOfDisTexture += 0;
			for(j = (s32)value > 0 ? (s32)value : (-(s32)value); j >0; j /= radixBase)
			{
				nextNumOfDisTexture++;
			}
		}
		if(pointPosRight != 0) //at least display (number of fraction + 1)
			if(nextNumOfDisTexture < pointPosRight+1)
				nextNumOfDisTexture = pointPosRight+1;
	}
	else 
		nextNumOfDisTexture = numofNumber;

	//overflow style 0:display max 1:not display
	if(overflowStyle == OVERFLOW_DISPLAY)
	{
		overflow = 0;
	}

	tens=1;
	showingZero=0;
	signFlag = 1 - sign; 
	dataMinus=0;


	for(i=0;i<numofNumber;i++) 
		//tens *=10;
		tens *= radixBase;

	pointPosLeft = numofNumber - pointPosRight;//the num of number left to point

	if(pointPosLeft <1 ) pointPosLeft = 1;

	if(sign)
	{
		numofNumber++; //the number of texture, not num
		pointPosLeft++;//the number of char left to point
		if((s32)curValue < 0) //sign of '-'
		{
			numOfDisTexture++;
		}
		if((s32)value < 0)
			nextNumOfDisTexture++;
	}

	if(pointPosRight != 0 )
	{
		numofNumber++;
#if 0
		//no need to judge if the data is zero
		if(curValue != 0)
			numOfDisTexture++; //sign of '.'
		if(value != 0)
			nextNumOfDisTexture++;
#endif	
		numOfDisTexture++; //sign of '.'
		nextNumOfDisTexture++;
	}

	pCurrentPage = pagePtr;
	//pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex].mTexAttr |= TEXTURE_USING_WIDGET_BOX; //the first puremask must use the box of widget
	if((p_wptr->NumOfLine & 0xff) == 0 || (p_wptr->NumOfLine & 0xff) == 1)   //no animation
	{
		texturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex+1]);
		#ifdef EMBEDDED
		if((texturePtr < (void*)startOfDynamicPage) || (texturePtr > (void*)endOfDynamicPage))
			return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
		#endif

	}
	else if((p_wptr->NumOfLine & 0xff) == 2) //animation
	{
		texturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 1]);
		nextTexturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + numofNumber + numofUnitString + 1]);
	}
	else 
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);

	//cal tex padding, for example fontsize is 64px, tex is 78px
	fontPadding = (texturePtr->TexWidth - fontWidth)/2;
	

	//judge the dirction of moving(animation)
	uValue[0] = (u32)value;
	sValue[0] = (s32)value;
	uValue[1] = (u32)curValue;
	sValue[1] = (s32)curValue;

	if((p_wptr->NumOfLine & 0xff) == 2) {
		if(sign)
		{
			if(sValue[0] > (s32)curValue)
			{
				//down
				movingDir = 0;
			}
			else if(sValue[0] < (s32)curValue)
			{
				 //up
				movingDir = 1;
			}
			else
			{
				//not moving
				//if(u8_pageRefresh == 0) 
					//return AHMI_FUNC_SUCCESS;
			}
		}
		else
		{
			if(uValue[0] >= (u32)curValue)
			{
				//down
				movingDir = 0;
			}
			else if(uValue[0] < (u32)curValue)
			{
				 //up
				movingDir = 1;
			}
			else
			{
				//not moving
				//if(u8_pageRefresh == 0)
					//return AHMI_FUNC_SUCCESS;
			}
		}
	}
	else{
		movingDir = 0;
	}
	if(movingDir == 0) //up
	{
		p_wptr->PreviousTexturePtrFlag = 0;
	}
	else if(movingDir == 1)//down
	{
		p_wptr->PreviousTexturePtrFlag = 1;
	}
	else if(u8_pageRefresh == 0 && (overflowStyle == 0))
	{
		//ERROR_PRINT("error in parsing number");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
	}

	if(sValue[1] < 0)
	{
		dataMinus = 1;
		sValue[1] = - sValue[1];
		
	}

	// cut the out of range number
	uValue[1] = uValue[1] % tens;
	sValue[1] = sValue[1] % tens;
	// tens /= 10;
	tens /= radixBase;
	
	if(nextHitWaitingValue)
	{
		for(i=0;i<numofNumber;i++)
		{
			if(i >= (numofNumber - 2))
			{
				texturePtr->FocusedSlice = '-' - 0x20;
			}
			else
			{
				texturePtr->FocusedSlice = 0;
			}
			texturePtr++;
		}
		numOfDisTexture = 2;
	}
	else
	{
		//operate the value of texture
		//pre
		for(i=0;i<numofNumber;i++)
		{

			if(sign)
			{
				code = (u16)(sValue[1] / tens);		
				sValue[1] %= tens;
			}
			else
			{
				code = (u16)(uValue[1] / tens);
				uValue[1] %= tens;
			}

			//sign mode
			if( sign         && dataMinus && (leadZero              || ( !leadZero && code != 0 )               || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
			//  display sign    negtive        display leading zero       not display leading zero and not zero                              arriving 2 number before point        did not display '-'
			{
				if(overflow == 1)
					texturePtr->FocusedSlice = ' ' - 0x20;
				else
					texturePtr->FocusedSlice = '-' - 0x20;
				texturePtr++;

				dataMinus = 0;//'-' is displayed, assume it as postive
				signFlag = 1; //already display sign
				i++;
			}
			//display ' ' before the positive number
			else if( sign && !dataMinus && (leadZero || ( !leadZero && code != 0 ) || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
			//       sign      postive      display '0'    no display '0' and not 0     no display '0' and arriving 2 number before point                 didnot display sign yet
			{
				if(overflow == 1)
					texturePtr->FocusedSlice = ' ' - 0x20;
				else
					texturePtr->FocusedSlice = 0;
				texturePtr++;

				signFlag = 1;//already display sign
				i++;
			}
			//positve number display ' '
			else if( sign && !dataMinus && (leadZero || ( !leadZero && code != 0 ) ||     (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
				//  sign      positive      display '0'    not display '0' and not zero               not display zero and arriving the left 2 to point
			{
				texturePtr->FocusedSlice = ' ' - 0x20;
				texturePtr++;
				signFlag = 1;
				i++;
			}
		
			// display '0x'
			if(numRadix==1&&markingMode==1&&i==markingPos){

				if (overflow == 1)
				{
					texturePtr->FocusedSlice = ' ' - 0x20;
				}else
				{
					texturePtr->FocusedSlice = 'x' - 0x20;
				}
				tens /= radixBase;
				texturePtr++;
				continue;
			}else if(numRadix==1&&markingMode==1&&i==(markingPos-1)){
				if (overflow == 1)
				{
					texturePtr->FocusedSlice = ' ' - 0x20;
				}else
				{
					texturePtr->FocusedSlice = '0' - 0x20;
				}
				tens /= radixBase;
				texturePtr++;
				continue;
			}

			//display point
			if(i == pointPosLeft)
			{
				if(overflow == 1)
					texturePtr->FocusedSlice = ' ' - 0x20;
				else
					texturePtr->FocusedSlice = '.' - 0x20;//��ʾС����
				texturePtr++;
//				offsetpoint = i;
				i++;
			}

				

			//display leading zeros
			if (!showingZero          && code == 0     && !leadZero        && (i != pointPosLeft - 1))
			//   not display rest '0'    zero       not display leading '0'     didn't arrive the point
			{
				code=' ' - 0x20;//show ' '
			}
			//display the '0' before point
			else if(!showingZero && code == 0 && !leadZero && i == pointPosLeft - 1)
			{
				code= '0' - 0x20;//display '0'
				showingZero = 1;
			}
			else
			{
				if(code>9){
					// hex mode
					if(numRadix==1){
						if(transformMode==0){
							code += lowerLetterOffset;
						}else if(transformMode ==1){
							code += upperLetterOffset;
						}
					}
				}
				code = code + 0x30/*the start code of ASIC*/ - 0x20/*the start code of our texture's code*/;

				showingZero = 1;
			}
			// tens =tens /10;
			tens /= radixBase;
			if(overflow == 1)
				texturePtr->FocusedSlice = ' ' - 0x20; 
			else
				if (nextHitWaitingValue)
				{
					texturePtr->FocusedSlice = '-' - 0x20; 
				}else{
					texturePtr->FocusedSlice = code;
				}

			texturePtr++;
		}
	}

	//next, if contains animation, the next line do the same thing with first line except the y position
	if((p_wptr->NumOfLine & 0xff) == 2) //with animation
	{
		tens=1;
		signFlag = 1 - sign; 
		showingZero = 0;
		numofNumber = (p_wptr->WidgetAttr >>5) & 0xf;


		for(i=0;i<numofNumber;i++) 
			tens *=radixBase;

		if(sign)
		{
			numofNumber++; //the number of texture, not the number of char
		}

		if(pointPosRight != 0 )
		{
			numofNumber++;
		}

		dataMinus = 0;
		if(sValue[0] < 0)
		{
			dataMinus = 1;
			sValue[0] = - sValue[0];
		}
		// cut the number out of range
		uValue[0] = uValue[0] % tens;
		sValue[0] = sValue[0] % tens;
		tens /= radixBase;
		if(hitWaitingValue)
		{
			for(i=0;i<numofNumber;i++)
			{
				if(i >= (numofNumber - 2))
				{
					nextTexturePtr->FocusedSlice = '-' - 0x20;
				}
				else
				{
					nextTexturePtr->FocusedSlice = 0;
				}
				nextTexturePtr++;
			}
			nextNumOfDisTexture = 2;
		}
		else
		{
			for(i=0;i<numofNumber;i++)
			{

				if(sign)
				{
					code = (u16)(sValue[0] / tens);		
					sValue[0] %= tens;
				}
				else
				{
					code = (u16)(uValue[0] / tens);
					uValue[0] %= tens;
				}

				//display '-'
				if( sign &&   dataMinus &&     (leadZero ||               ( !leadZero && code != 0 )                 || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
				//  display sign    negtive        display leading zero       not display leading zero and not zero                   arriving 2 number before point                    did not display '-'
				{
					if(overflow == 1)
						nextTexturePtr->FocusedSlice = ' ' - 0x20;
					else
						nextTexturePtr->FocusedSlice = '-' - 0x20;
					nextTexturePtr++;

					dataMinus = 0;//'-' has been displayed, assume it to be positive
					signFlag = 1;
					i++;
				}
				//not showing the sign of positive number
				else if( sign && !dataMinus && (leadZero || ( !leadZero && code != 0 ) || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
				//       sign      postive      display '0'    no display '0' and not 0     no display '0' and arriving 2 number before point                 didnot display sign yet
				{
					if(overflow == 1)
						nextTexturePtr->FocusedSlice = ' ' - 0x20;
					else
						nextTexturePtr->FocusedSlice = 0;
					nextTexturePtr++;

					signFlag = 1;
					i++;
				}
				//display ' ' for the first char of positive number
				else if( sign && !dataMinus && (leadZero || ( !leadZero && code != 0 ) || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
				//       sign      postive      display '0'    no display '0' and not 0     no display '0' and arriving 2 number before point                 didnot display sign yet
				{
					nextTexturePtr->FocusedSlice = ' ' - 0x20;
					nextTexturePtr++;
					signFlag = 1;
					i++;
				}

				// display '0x'
			if(numRadix==1&&markingMode==1&&i==nextMarkingPos){

				if (overflow == 1)
				{
					nextTexturePtr->FocusedSlice = ' ' - 0x20;
				}else
				{
					nextTexturePtr->FocusedSlice = 'x' - 0x20;
				}
				tens /= radixBase;
				nextTexturePtr++;
				continue;
			}else if(numRadix==1&&markingMode==1&&i==(nextMarkingPos-1)){
				if (overflow == 1)
				{
					nextTexturePtr->FocusedSlice = ' ' - 0x20;
				}else
				{
					nextTexturePtr->FocusedSlice = '0' - 0x20;
				}
				tens /= radixBase;
				nextTexturePtr++;
				continue;
			}

				//show '.'
				if(i == pointPosLeft)
				{
					if(overflow == 1)
						nextTexturePtr->FocusedSlice = ' ' - 0x20;
					else
						nextTexturePtr->FocusedSlice = '.' - 0x20;//��ʾС����
					nextTexturePtr++;
				//offsetpoint = i;
					i++;
				}

				//not show leading zeros
				if (!showingZero && code == 0 && !leadZero && (i != pointPosLeft - 1))
	
				{
					code=' ' - 0x20;
				}
				//showing the leading zeros
				else if(!showingZero && code == 0 && !leadZero && i == pointPosLeft - 1)
				{
					code= '0' - 0x20;
					showingZero = 1;
				}
				else
				{
					if(code>9){
						// hex mode
						if(numRadix==1){
							if(transformMode==0){
								code += lowerLetterOffset;
							}else if(transformMode ==1){
								code += upperLetterOffset;
							}
						}
					}
					code = code + 0x30/*the start code of number '0' in ASIC*/ - 0x20/*delete the control code*/;
					showingZero = 1;
				}
				tens =tens /radixBase;
				if(overflow == 1)
					nextTexturePtr->FocusedSlice = ' ' - 0x20;
				else
					if (hitWaitingValue)
					{
						nextTexturePtr->FocusedSlice = '-' - 0x20;
					}else{
						nextTexturePtr->FocusedSlice = code;
					}
				nextTexturePtr++;
			}
		}
	}

	//set unit code
	texturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + 1]);
	//next, if contains animation, the next line do the same thing with first line except the y position
	if((p_wptr->NumOfLine & 0xff) == 2) //with animation
	{
		nextTexturePtr = &(pCurrentPage->pBasicTextureList[p_wptr->StartNumofTex + numofNumber + numofUnitString + 1]);
	}

	for(i = 0; i < numofUnitString; i++)
	{
		texturePtr[numofNumber + i].FocusedSlice = ((UnitString >> (8 * i)) & 0xFF) - 0x20;
		//next, if contains animation, the next line do the same thing with first line except the y position
		if((p_wptr->NumOfLine & 0xff) == 2) //with animation
		{
			nextTexturePtr[numofNumber + i].FocusedSlice = ((UnitString >> (8 * i)) & 0xFF) - 0x20;
		}
	}

	//set unit texture display
	if((p_wptr->NumOfLine & 0xff) == 2) //with animation
	{
		for(i=0;i<(numofNumber + numofUnitString);i++)
		{
			texturePtr[i].mTexAttr &= ~(FORCE_UNDISPLAY);
			nextTexturePtr[i].mTexAttr &= ~(FORCE_UNDISPLAY);
		}
	}
	else //no animation
	{
		for(i=0;i<(numofNumber + numofUnitString);i++)
		{
			texturePtr[i].mTexAttr &= ~(FORCE_UNDISPLAY);
		}
	}

	//judge the assign mode
	if (dotLayout)
	{//dot 1 char
		if(unitflag)// if unit display need add num of unit string texture
		{
			numOfDisTexture += numofUnitString;
		}

		showNumWidth = (fontWidth + SpacingX) * (numOfDisTexture - 1) + fontWidth;
	}else
	{//dot 0.5 char
		if(unitflag)// if unit display need add num of unit string texture
		{
			numOfDisTexture += numofUnitString;
		}

		if(numofNumber - 1 <= pointPosLeft)
		{
			showNumWidth = (fontWidth + SpacingX) * (numOfDisTexture - 1) + fontWidth;
		}
		else
		{
			showNumWidth = (fontWidth + SpacingX) * (numOfDisTexture - 1) - fontWidth/2 + fontWidth;
		}
	}
	
	if(align == RIGHTALIGN)   //default right align
	{
		xCoordinate = (showNumWidth >  (s16)p_wptr->WidgetWidth - 2 * paddingX ) ? 0 : ((s16)p_wptr->WidgetWidth - 2 * paddingX - showNumWidth);
	}
	else if(align == CENTERALIGN)  //middle align
	{
		xCoordinate = (showNumWidth >  (s16)p_wptr->WidgetWidth - 2 * paddingX ) ? 0 : ((s16)p_wptr->WidgetWidth - 2 * paddingX - showNumWidth)/2;
	}
	else if(align == LEFTALIGN) //left align mode
	{
		xCoordinate = 0;

	}
	//xCoordinate += (s16)p_wptr->WidgetOffsetX;//absolute offsetX
	xCoordinate += paddingX;//always has a padding;
	

	//operate the position of alpha texture
	for(i=0;i<(numofNumber + numofUnitString);i++)
	{
		
		if(texturePtr[i].FocusedSlice == 0) //if initial not display, need to display after by Mr.z
		{
			texturePtr[i].mTexAttr |= (FORCE_UNDISPLAY);
		}
		else 
		{
			texturePtr[i].OffsetX = (xCoordinate-fontPadding) << 4;
			texturePtr[i].TexLeftTileBox = texturePtr[i].OffsetX >> 9;
			texturePtr[i].TexRightTileBox = (texturePtr[i].OffsetX + texturePtr[i].TexWidth * 16) >> 9;

			if(nextHitWaitingValue != 1)
			{
				if (dotLayout)
				{
					if(pointPosRight != 0)
					{
						//dot 1 char
						if (i == pointPosLeft-1)
						{
							xCoordinate += fontWidth + (texturePtr[i].TexWidth)*0.2 ;// dot is manually moved left by 20%
						}else if (i == pointPosLeft)
						{
							xCoordinate += fontWidth - (texturePtr[i].TexWidth)*0.2;
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}
					else
					{
						xCoordinate += fontWidth;
					}
				}else
				{
					if(pointPosRight != 0)
					{
						//dot 0.5 char
						if (i == pointPosLeft)
						{
							xCoordinate += fontWidth/2;
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}
					else
					{
						xCoordinate += fontWidth;
					}
				}
			}
			else
			{
				xCoordinate += fontWidth;
			}
			xCoordinate += SpacingX;
		}
	}

	//next, if contains animation, needs to do the same thing for the second line
	if((p_wptr->NumOfLine & 0xff) == 2) //with animation
	{	
		if (dotLayout)
		{//dot 1 char
			if(unitflag)// if unit display need add num of unit string texture
			{
				nextNumOfDisTexture += numofUnitString;
			}

			showNumWidth = (fontWidth + SpacingX) * (nextNumOfDisTexture - 1) + fontWidth;
		}else
		{//dot 0.5 char
			if(unitflag)// if unit display need add num of unit string texture
			{
				nextNumOfDisTexture += numofUnitString;
			}

			if(numofNumber - 1 <= pointPosLeft)
			{
				showNumWidth = (fontWidth + SpacingX) * (nextNumOfDisTexture - 1) + fontWidth;
			}
			else
			{
				showNumWidth = (fontWidth + SpacingX) * (nextNumOfDisTexture - 1) - fontWidth/2 + fontWidth;
			}
		}
	
		if(align == RIGHTALIGN)   //default right align
		{
			xCoordinate = (showNumWidth >  (s16)p_wptr->WidgetWidth - 2 * paddingX ) ? 0 : ((s16)p_wptr->WidgetWidth - 2 * paddingX - showNumWidth);
		}
		else if(align == CENTERALIGN)  //middle align
		{
			xCoordinate = (showNumWidth >  (s16)p_wptr->WidgetWidth - 2 * paddingX ) ? 0 : ((s16)p_wptr->WidgetWidth - 2 * paddingX - showNumWidth)/2;
		}
		else if(align == LEFTALIGN) //left align mode
		{
			xCoordinate = 0;

		}
		//xCoordinate += (s16)p_wptr->WidgetOffsetX;//absolute offsetX
		xCoordinate += paddingX;//always has a padding;
	

		//operate the position of alpha texture
		for(i=0;i<(numofNumber + numofUnitString);i++)
		{
		
			if(nextTexturePtr[i].FocusedSlice == 0) //if initial not display, need to display after by Mr.z
			{
				nextTexturePtr[i].mTexAttr |= (FORCE_UNDISPLAY);
			}
			else 
			{
				nextTexturePtr[i].OffsetX = (xCoordinate-fontPadding) << 4;
				nextTexturePtr[i].TexLeftTileBox = nextTexturePtr[i].OffsetX >> 9;
				nextTexturePtr[i].TexRightTileBox = (nextTexturePtr[i].OffsetX + nextTexturePtr[i].TexWidth * 16) >> 9;

				if(hitWaitingValue != 1)
				{
					if (dotLayout)
					{
						if(pointPosRight != 0)
						{
							if (i == pointPosLeft-1)
							{
								xCoordinate += fontWidth + (texturePtr[i].TexWidth)*0.2;// dot is manually moved left by 20%
							}else if (i == pointPosLeft)
							{
								xCoordinate += fontWidth - (texturePtr[i].TexWidth)*0.2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}else
					{
						if(pointPosRight != 0)
						{
							if (i == pointPosLeft)
							{
								xCoordinate += fontWidth / 2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}
				}
				else
				{
					xCoordinate += fontWidth;
				}
				xCoordinate += SpacingX;
			}
		}

		//modify the positionY
		for(i=0;i<(numofNumber + numofUnitString);i++)
		{
			texturePtr[i].OffsetY = 0;
			nextTexturePtr[i].OffsetY = (p_wptr->WidgetHeight << 4);
		}
	}

	return AHMI_FUNC_SUCCESS;
}
#endif

//----------------------------
// reponse for Tag.setValue
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the tag
// addtional comments :
//   none
//-----------------------------
funcStatus NumberClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_widgetRefresh				
	)
{
	DynamicPageClassPtr pCurrentPage;
	u32 actionAddr;
	u32 value, last_value;
	s32	maxValue;
	s32	minValue;
	s32 lowAlarmValue;
	s32 highAlarmValue;
    //u32 oldValue=0;   //old value
	u16 oldValueinit=0;
	u8  sign;
//	u8  numofNumber;
	u8  overflowStyle = 0;
	u8  overflag = 0;
	u8  animationflag = 0;
	
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

	value = ActionPtr->mTagPtr->getValue();

	sign = (p_wptr->WidgetAttr & 0x4000 )?1:0;
//	numofNumber = (p_wptr->WidgetAttr >>5) & 0xf;
	oldValueinit = (p_wptr->WidgetAttr & 0x8000);
	overflowStyle = (p_wptr->NumOfLine >> 12) & 0xf;

	////the num of number only supports 1-10
	//if(numofNumber > 10 || numofNumber < 1){
	//	ERROR_PRINT("ERROR: don't support such length of number");
	//	return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
	//}
	
	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);
	lowAlarmValue  = (s32)((p_wptr->LowAlarmValueH  << 16) + p_wptr->LowAlarmValueL );
	highAlarmValue = (s32)((p_wptr->HighAlarmValueH << 16) + p_wptr->HighAlarmValueL);

	//sign mode
	if(sign)
	{
		if((s32)value > (s32)maxValue)
		{
			ERROR_PRINT("ERROR: Number widget bindTag value exceeds the boundary");
			u16 CanvasID, SubcanvasID, WidgetID;
			p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
			EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
			//value = (s32)maxValue;
			overflag = 1;
		}
		if((s32)value < (s32)minValue)
		{
			ERROR_PRINT("ERROR: Number widget bindTag value exceeds the boundary");
			u16 CanvasID, SubcanvasID, WidgetID;
			p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
			EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
			//value = (s32)minValue;
			overflag = 2;
		}
	}
	else
	{
		// none sign mode, if the value is negtive, transform it into positive
		if((s32)value > (s32)maxValue)
		{
			ERROR_PRINT("ERROR: Number widget bindTag value exceeds the boundary");
			u16 CanvasID, SubcanvasID, WidgetID;
			p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
			EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
			//value = (s32)maxValue;
			overflag = 1;
		}
		if((s32)value < (s32)minValue) 
		{
			ERROR_PRINT("ERROR: Number widget bindTag value exceeds the boundary");
			u16 CanvasID, SubcanvasID, WidgetID;
			p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
			EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
			//value = (s32)minValue;
			overflag = 2;
		}
		//if((s32)value < 0){
		//	value = -value;
		//}
	}
    
	//modify currnet value
	//if(animationDuration) //page animation
	//{
	//	p_wptr->CurValueL = (u16)value;
	//	p_wptr->CurValueH = (u16)(value >> 16);
	//}

    if(u8_widgetRefresh == 0)
    {
		if(oldValueinit)
		{
			last_value = (s32)((p_wptr->STOP_TAG_H << 16) + p_wptr->STOP_TAG_L);
			if((p_wptr->NumOfLine & 0xff) == 2) //contain animation
			{
				if(p_wptr->NOW_DURATION == 0)
				{
					p_wptr->START_TAG_L = p_wptr->ANIMATIONCURVALUE_L;
					p_wptr->START_TAG_H = p_wptr->ANIMATIONCURVALUE_H;
				}
				else
				{
					p_wptr->START_TAG_L = p_wptr->STOP_TAG_L;
					p_wptr->START_TAG_H = p_wptr->STOP_TAG_H;
					animationflag = 1;
					//return AHMI_FUNC_SUCCESS;
				}
				
				if(overflowStyle == OVERFLOW_DISPLAY && !animationflag)
				{
					if(((s32)last_value < (s32)maxValue && (s32)last_value > (s32)minValue) || ((s32)value < (s32)maxValue && (s32)value > (s32)minValue)
					 ||((s32)last_value <= (s32)maxValue && (s32)last_value >= (s32)minValue && ((s32)value == (s32)maxValue || (s32)value == (s32)minValue))
					 ||((s32)value <= (s32)maxValue && (s32)value >= (s32)minValue && ((s32)last_value == (s32)maxValue || (s32)last_value == (s32)minValue)))
					{
						if(!overflag)
						{
							p_wptr->STOP_TAG_L = (u16)( (u32)value );
							p_wptr->STOP_TAG_H = (u16)(( (u32)value ) >> 16);
							if((p_wptr->STOP_TAG_L != p_wptr->CurValueL) || (p_wptr->STOP_TAG_H != p_wptr->CurValueH))
							{
								p_wptr->loadTextureWithAnimation(pagePtr);
							}
						}
						else
						{
							if(overflag == 1)
							{
								p_wptr->STOP_TAG_L = (u16)( (u32)maxValue );
								p_wptr->STOP_TAG_H = (u16)(( (u32)maxValue ) >> 16);
							}
							else if(overflag == 2)
							{
								p_wptr->STOP_TAG_L = (u16)( (u32)minValue );
								p_wptr->STOP_TAG_H = (u16)(( (u32)minValue ) >> 16);
							}
							p_wptr->loadTextureWithAnimation(pagePtr);
						}
					}
					else
					{
						if((s32)value > (s32)maxValue)
						{
							p_wptr->CurValueL = (u16)maxValue;
							p_wptr->CurValueH = (u16)(maxValue >> 16);
						}
						else if((s32)value < (s32)minValue)
						{
							p_wptr->CurValueL = (u16)minValue;
							p_wptr->CurValueH = (u16)(minValue >> 16);
						}
						else
						{
							p_wptr->CurValueL = (u16)( (u32)value );
							p_wptr->CurValueH = (u16)(( (u32)value ) >> 16);
							p_wptr->STOP_TAG_L = (u16)( (u32)value );
							p_wptr->STOP_TAG_H = (u16)(( (u32)value ) >> 16);
						}
					}
				}
				else if(overflowStyle == OVERFLOW_NON_DISPLAY && !animationflag)
				{
					if(((s32)last_value <= (s32)maxValue && (s32)last_value >= (s32)minValue) || ((s32)value <= (s32)maxValue && (s32)value >= (s32)minValue))
					{
						p_wptr->STOP_TAG_L = (u16)( (u32)value );
						p_wptr->STOP_TAG_H = (u16)(( (u32)value ) >> 16);
						if((p_wptr->STOP_TAG_L != p_wptr->CurValueL) || (p_wptr->STOP_TAG_H != p_wptr->CurValueH))
						{
							p_wptr->loadTextureWithAnimation(pagePtr);
						}
					}
					else
					{
						p_wptr->STOP_TAG_L = (u16)( (u32)value );
						p_wptr->STOP_TAG_H = (u16)(( (u32)value ) >> 16);
					}
				}
			}
			else
			{
				p_wptr->STOP_TAG_L = (u16)value;
				p_wptr->STOP_TAG_H = (u16)(value >> 16);
			}
		}
		else
		{
			if(p_wptr->NOW_DURATION == 0)
			{
				TextureClassPtr pTexturePtr;
				//initial position
				pTexturePtr = &(gPagePtr[p_wptr->ATTATCH_PAGE].pBasicTextureList[p_wptr->StartNumofTex + 1]);
				pTexturePtr->OffsetY = ((((p_wptr->WidgetHeight - pTexturePtr->TexHeight) << 4) )/2);
			}
			p_wptr->CurValueL = (u16)value;
			p_wptr->CurValueH = (u16)(value >> 16);
			p_wptr->STOP_TAG_L = (u16)value;
			p_wptr->STOP_TAG_H = (u16)(value >> 16);
			p_wptr->WidgetAttr |= 0x8000;
		}
    }
	
    pCurrentPage = pagePtr;
	if(u8_widgetRefresh == 0)
	{
		if(oldValueinit)
		{
			//oldValue = (p_wptr->OldValueH  << 16) + p_wptr->OldValueL;
			//p_wptr->OldValueL = (u16)value;
			//p_wptr->OldValueH = (u16)(value >> 16);
			//tagchange alarm
			if(p_wptr->TagChangeAction && last_value != value && (p_wptr->TagChangeAction <= pCurrentPage->mActionInstructionsSize))
            {
				actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->TagChangeAction);
                csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
			}
			//if(sign)
			{
				if(p_wptr->EnterLowAlarmAction && (s32)last_value > (s32)lowAlarmValue && (s32)value <= (s32)lowAlarmValue)
				{
					//enter low alarm
					if(p_wptr->EnterLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterLowAlarmAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}		
				}
				else if(p_wptr->LeaveLowAlarmAction && (s32)last_value <= (s32)lowAlarmValue && (s32)value > (s32)lowAlarmValue)
				{
					//leave low alarm
					if(p_wptr->LeaveLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->LeaveLowAlarmAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}		
				}
				if(p_wptr->EnterHighAlarmAction && (s32)last_value < (s32)highAlarmValue && (s32)value >= (s32)highAlarmValue)
				{
					//enter high alarm
					if(p_wptr->EnterHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterHighAlarmAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}
				}
				else if(p_wptr->LeaveHighAlarmAction && (s32)last_value >= (s32)highAlarmValue && (s32)value < (s32)highAlarmValue)
				{
					//leave high alarm
					if(p_wptr->LeaveHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->LeaveHighAlarmAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}	
				}
			}
//			else
//			{
//				if(p_wptr->EnterLowAlarmAction && last_value > (u32)lowAlarmValue && value <= (u32)lowAlarmValue)
//				{
//					//enter low alarm
//					if(p_wptr->EnterLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
//					{
//						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterLowAlarmAction);
//						xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
//					}		
//				}
//				else if(p_wptr->LeaveLowAlarmAction && last_value <= (u32)lowAlarmValue && value > (u32)lowAlarmValue)
//				{
//					//leave low alarm
//					if(p_wptr->LeaveLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
//					{
//						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->LeaveLowAlarmAction);
//						xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
//					}		
//				}
//				if(p_wptr->EnterHighAlarmAction && last_value < (u32)highAlarmValue && value >= (u32)highAlarmValue)
//				{
//					//enter high alarm
//					if(p_wptr->EnterHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
//					{
//						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterHighAlarmAction);
//						xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);		
//					}
//				}
//				else if(p_wptr->LeaveHighAlarmAction && last_value >= (u32)highAlarmValue && value < (u32)highAlarmValue)
//				{
//					//leave high alarm
//					if(p_wptr->LeaveHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
//					{
//						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->LeaveHighAlarmAction);
//						xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
//					}	
//				}
//			}
			
		}
        else
        {
            ////tagchange alarm, temporary delete
            //if(p_wptr->TagChangeAction && (p_wptr->TagChangeAction <= pCurrentPage->mActionInstructionsSize))
            //{
            //    actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->TagChangeAction);
            //    xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
            //}
            
   //         if(sign)
			//{
			//	if(p_wptr->EnterLowAlarmAction && (s32)value < (s32)lowAlarmValue)
			//	{
			//		//enter low
			//		if(p_wptr->EnterLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
			//		{
			//			actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterLowAlarmAction);
			//			xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
			//		}		
			//	}
			//	if(p_wptr->EnterHighAlarmAction && (s32)value > (s32)highAlarmValue)
			//	{
			//		//enter high
			//		if(p_wptr->EnterHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
			//		{
			//			actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterHighAlarmAction);
			//			xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);		
			//		}
			//	}
			//}
			//else
			//{
			//	if(p_wptr->EnterLowAlarmAction && value < (u32)lowAlarmValue)
			//	{
			//		//enter low
			//		if(p_wptr->EnterLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
			//		{
			//			actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterLowAlarmAction);
			//			xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
			//		}		
			//	}
			//	if(p_wptr->EnterHighAlarmAction && value > (u32)highAlarmValue)
			//	{
			//		//enter high
			//		if(p_wptr->EnterHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
			//		{
			//			actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterHighAlarmAction);
			//			xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);		
			//		}
			//	}
			//}
        }
	}
    
	if(oldValueinit)
	{
		if(last_value != value)
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
funcStatus NumberClass::MouseTouchCtrl(
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
// name�� MeterClass::setATag
// set the texture's animation tag
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the animation tag
// addtional comments :
//   none
//-----------------------------
funcStatus NumberClass::setATag(
	WidgetClassPtr p_wptr,   //widget pointer
	u16 value
	)
{
	s16 preTextureOffsetY; //1.11.4
	u16 transitionParamIn; //portion of animation progress 0.10
	u16 transitionParamOut; //portion of animation progress after varying function 0.10
	u16 widgetHeight;
	TextureClassPtr preTexturePtr; //pre is the first previous number that needs to go away, and next is to be displayed
	TagClassPtr  bindTag;
	u32 bindTagValue;
	u16 totalDuration = p_wptr->ANIMATION_DURATION;

	widgetHeight = p_wptr->WidgetHeight;
	bindTag = &TagPtr->getTag(p_wptr->BindTagID);
	bindTagValue = (u32)(bindTag->getValue());

	if(totalDuration == 0 || ( (p_wptr->NumOfLine & 0xff) == 1) ) //no animation
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetAnimationDurationErr);

//	taskENTER_CRITICAL();
#ifdef AHMI_DEBUG
	char text[100];
	sprintf(text,"set widget tag %d", value);
	ERROR_PRINT(text);
#endif
	p_wptr->NOW_DURATION |= value;

	//caculating the moving parameter
	transitionParamIn = (value) * 1024 / totalDuration;//0.10
	gEasing.calculateEasingFunc(transitionParamIn, transitionParamOut, (p_wptr->TimingFun & 0xFF));

	if(p_wptr -> PreviousTexturePtrFlag == 0) //animation moving down
	{
		preTextureOffsetY = ((widgetHeight) * transitionParamOut) >> 6;   //0->h
	}
	else //animation moving up
	{
		preTextureOffsetY =  - ( ((widgetHeight) * transitionParamOut) >> 6); //0->-h
	}

	//modify the position
	preTexturePtr = &(gPagePtr[p_wptr->ATTATCH_PAGE].pBasicTextureList[p_wptr->StartNumofTex + 1]);
	preTexturePtr->OffsetY = preTextureOffsetY;

	//modify the value
	if( value >= p_wptr->ANIMATION_DURATION) //the last frame
	{
		p_wptr->NOW_DURATION = 0;

		p_wptr->CurValueL = p_wptr->STOP_TAG_L;
		p_wptr->CurValueH = p_wptr->STOP_TAG_H;

		p_wptr->INITIALIZED &= (~0x2);

		if((p_wptr->CurValueL != (u16)bindTagValue) || (p_wptr->CurValueH != (u16)(bindTagValue >> 16)))
		{
			p_wptr->STOP_TAG_L = (u16)( (u32)bindTagValue );
		    p_wptr->STOP_TAG_H = (u16)(( (u32)bindTagValue ) >> 16);
			p_wptr->INITIALIZED |= (0x2);
			TagPtr->getTag(p_wptr->BindTagID).setRawValue(bindTagValue);
		}
	}

	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
//	taskEXIT_CRITICAL();

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
funcStatus NumberClass::setColor(
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
	colorTexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 2]);
	color = (a<<24) + (r<<16) + (g<<8) + b;
	colorTexturePtr->TexAddr = color;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	
	return AHMI_FUNC_SUCCESS;
}

//***********************************
// number to texture for display
//**********************************
funcStatus NumberClass::updateNumberDisplay(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,	
	u32 *u32p_sourceShift,
	NewMatrixPtr pWidgetMatrix,
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{
	funcStatus status;
	#if (defined VERSION_2) && (defined EMBEDDED)  
	u32 mSourceBufferStartPos;
	u32 mSourceBufferLength = 0;
	#endif
    u64 tens;
	u32 value;
	u32 uValue[5] = {0};
	s32 sValue[5] = {0};
	s32	maxValue;
	s32	minValue;
	s32 curValue;
	s32 j;
    TextureClassPtr texturePtr; //texture pointer
//    u16 widthOfFont;   //width of dont
	u16 showNumWidth;
//	u16 shiftSize;     //shifting size, by align mode 12.4
	u16 code;         //ASIC-0x20
	s16 temp;
	s16 TextureOffsetY; //1.11.4
    u8  leadZero;
	u8  sign;
	u8  pointPosRight;
	u8	pointPosLeft;
	u8  numofNumber;
	u8  numofUnitString;
	u32 UnitString = 0;
	u8  numofWaitString = 0;
	u16 Number1PixelUnitChar;
	u32 WaitString = 0;
	u8	i;
	u8	showingZero; //incates the rest of number is to show zero. for example, 1.000, after point it the rest of number is all 0
	u8	signFlag;     //whether the sign has been shown
	                  // 0 not show��1 shown
	u8	dataMinus;    // positive or negtive, 1 is negtive,0 is positive or the sign has been shown
	u8  numOfDisTexture; //the number of texture needed to be display
	u8  nextNumOfDisTexture; //the number of texture needed to be display in the second line
//	u8  difOfNumber;   //the total number - the number to be display	
	u8 overflow, overflow_next;     //exceeds max
	u8 align;        //align mode
	u8 overflowStyle;    //the mode after exceeds 0��display max value  1��donnot display
	u8 numRadix;    // radix 0-ten radix  1-hex radix
	u8 markingMode; // whether to display 0x for hex   0-display  1-no display
	u8 transformMode; // whether to display upper case  0-low  1-up
	u8 markingPos; // the position of sign in texture
	u8 nextMarkingPos;
	u8 radixBase = 10;
	u8 upperLetterOffset = 7;   // upper case relative to number in ASIC 
	u8 lowerLetterOffset = 39;  // lower case relative to number in ASIC
	u8 movingDir;  //0 down, 1 up
//	u8 offsetpoint=0;
	s16 SpacingX = (s8)(p_wptr->SPACINGX & 0xFF);
	u8 fontWidth = p_wptr->SPACINGX >> 8;
	u8 paddingX = (fontWidth + 9) / 10;
	s16 xCoordinate;
	u8 fontPadding = 0;
	u8 dotLayout = 0;
	u8 unitflag = 0;
	u8 enableWaiting; // whether enabling waiting value
	s32 waitingValue; // waiting value
	u8 hitWaitingValue = 0; //if enable waiting value, whether current value hit waiting value
	u8 nextHitWaitingValue = 0;//animaton mode, hit waiting value
	u8 movedirection = (p_wptr->TimingFun >> 8);//0 for "Increase downwards and decrease upwards", 1 for up, 2 for down, 3 for "Decrease downwards and increase upwards"(default)

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

	numOfDisTexture = 0;
	nextNumOfDisTexture = 0;
//	difOfNumber = 0;
//	widthOfFont = 0;
//	shiftSize = 0;
	overflow = 0;
	overflow_next = 0;

	if(u8_pageRefresh)
	{
		p_wptr->STOP_TAG_L = (u16)( (u32)ActionPtr->mTagPtr->getValue() );
		p_wptr->STOP_TAG_H = (u16)(( (u32)ActionPtr->mTagPtr->getValue() ) >> 16);
	}

	value = (s32)((p_wptr->STOP_TAG_H << 16) + p_wptr->STOP_TAG_L);

	leadZero = (p_wptr->WidgetAttr & 0x2000 )?1:0;
	sign = (p_wptr->WidgetAttr & 0x4000 )?1:0;
	pointPosRight = (p_wptr->WidgetAttr >>9) & 0xf;
	dotLayout = p_wptr->DOTOPTIONS & 0x01;
	numofNumber = (p_wptr->WidgetAttr >>5) & 0xf;
	numofUnitString = (p_wptr->UnitStringLength & 0xFF);
	Number1PixelUnitChar = (p_wptr->UnitStringLength >> 8);
	align = (p_wptr->NumOfLine >> 8) & 0xf;
	overflowStyle = (p_wptr->NumOfLine >> 12) & 0xf;

	if(numofUnitString != 0)
	{
		unitflag = 1;
		UnitString = (p_wptr->UnitStringH << 16) | p_wptr->UnitStringL;
	}

	////the num of number only supports 1-10
	//if(numofNumber > 10 || numofNumber < 1){
	//	ERROR_PRINT("ERROR: don't support such length of number");
	//	return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
	//}
	//the number of fraction number must be 0-(num-1), connot be negtive
	if(/*pointPosRight < 0 || */pointPosRight > numofNumber - 1){
		ERROR_PRINT("ERROR: the numbers of decimal is wrong");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
	}
	
	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);

	////sign mode
	//if(sign)
	//{
	//	if((s32)value > (s32)maxValue)
	//	{
	//		value = (s32)maxValue;
	//		overflow = 1;
	//	}
	//	if((s32)value < (s32)minValue)
	//	{
	//		value = (s32)minValue;
	//		overflow = 1;
	//	}
	//}
	//else
	//{
	//	// none sign mode, if the value is negtive, transform it into positive
	//	if((s32)value > (s32)maxValue)
	//	{
	//		value = (s32)maxValue;
	//		overflow = 1;
	//	}
	//	if((s32)value < (s32)minValue)
	//	{
	//		value = (s32)minValue;
	//		overflow = 1;
	//	}
	//	if((s32)value < 0){
	//		value = -value;
	//	}
	//}

	numRadix =  (u8)(p_wptr->HexControl& 0x000f);
	markingMode = (u8)((p_wptr->HexControl & 0x00f0)>>4);
	transformMode = (u8)((p_wptr->HexControl & 0x0f00)>>8);

	enableWaiting = p_wptr->NumberWaitingValue & 0x1;
	temp = (s16)(p_wptr->NumberWaitingValueH);
	waitingValue = (temp << 15) | (s16)(p_wptr->NumberWaitingValue>>1);

	if(enableWaiting)
	{
		numofWaitString = p_wptr->WaitStringLength;
		WaitString = (p_wptr->WaitStringH << 16) | p_wptr->WaitStringL;
	}

	//get current value
	if((p_wptr->NumOfLine & 0xff) == 0 || (p_wptr->NumOfLine & 0xff) == 1) //no animation
	{
		if(u8_pageRefresh)
		{
			p_wptr->CurValueL = (u16)value;
			p_wptr->CurValueH = (u16)(value >> 16);
		}
		
		curValue = value;
		if((s32)curValue > (s32)maxValue)
		{
			if(overflowStyle == OVERFLOW_DISPLAY)
				curValue = (s32)maxValue;
			overflow = 1;
		}
		if((s32)curValue < (s32)minValue)
		{
			if(overflowStyle == OVERFLOW_DISPLAY)
				curValue = (s32)minValue;
			overflow = 1;
		}
		if (enableWaiting && waitingValue == (s32)value)
		{
			hitWaitingValue = 1;
		}
	}
	else
	{
		if(u8_pageRefresh && p_wptr->NOW_DURATION == 0 && !animationDuration)
		{
			if(overflowStyle == OVERFLOW_DISPLAY)
			{
				if((s32)value > (s32)maxValue)
				{
					p_wptr->CurValueL = (u16)maxValue;
					p_wptr->CurValueH = (u16)(maxValue >> 16);
				}
				else if((s32)value < (s32)minValue)
				{
					p_wptr->CurValueL = (u16)minValue;
					p_wptr->CurValueH = (u16)(minValue >> 16);
				}
				else
				{
					//p_wptr->CurValueL = (u16)value;
					//p_wptr->CurValueH = (u16)(value >> 16);
				}
			}
			else
			{
				//p_wptr->CurValueL = (u16)value;
				//p_wptr->CurValueH = (u16)(value >> 16);
			}
		}
		
		curValue = (s32)((p_wptr->CurValueH << 16) + p_wptr->CurValueL);
		if((s32)curValue > (s32)maxValue)
		{
			overflow = 1;
		}
		if((s32)value > (s32)maxValue)
		{
			if(overflowStyle == OVERFLOW_DISPLAY)
				value = (s32)maxValue;
			overflow_next = 1;
		}
		if((s32)curValue < (s32)minValue)
		{
			overflow = 1;
		}
		if((s32)value < (s32)minValue)
		{
			if(overflowStyle == OVERFLOW_DISPLAY)
				value = (s32)minValue;
			overflow_next = 1;
		}
		if (enableWaiting && waitingValue == (s32)value)
		{
			hitWaitingValue = 1;
		}
	}

	//hex
	if(numRadix){
		radixBase = 16;
		if(markingMode==1){
			numOfDisTexture +=2;
			// cacualte the position of 0x
			u32 tempValue = curValue;
			u8 count = 0;
			if(tempValue == 0)
			{
				count++;
			}
			else
			{
				while (tempValue>0)
				{
					tempValue /= radixBase;
					count++;
				}
			}
			if (leadZero)
			{
				markingPos = 1;
			}else
			{
				markingPos = numofNumber - 1 - count;
			}
			
			

			//next line
			nextNumOfDisTexture += 2;
			tempValue = value;
			count = 0;
			if(tempValue == 0)
			{
				count++;
			}
			else
			{
				while (tempValue>0)
				{
					tempValue /= radixBase;
					count++;
				}
			}
			if (leadZero)
			{
				nextMarkingPos = 1;
			}else
			{
				nextMarkingPos = numofNumber - 1 - count;
			}
			
		}
	}

	//if(sign)
	//{
	//	if((s32)curValue > (s32)maxValue)
	//	{
	//		//value = (s32)maxValue;
	//		overflow = 1;
	//	}
	//	if((s32)value > (s32)maxValue)
	//	{
	//		//value = (s32)maxValue;
	//		overflow_next = 1;
	//	}
	//	if((s32)curValue < (s32)minValue)
	//	{
	//		//value = (s32)minValue;
	//		overflow = 1;
	//	}
	//	if((s32)value < (s32)minValue)
	//	{
	//		//value = (s32)minValue;
	//		overflow_next = 1;
	//	}
	//	if (enableWaiting && waitingValue == value)
	//	{
	//		hitWaitingValue = 1;
	//	}
	//}
	//else
	//{
	//	if((s32)curValue > (s32)maxValue)
	//	{
	//		//value = (s32)maxValue;
	//		overflow = 1;
	//	}
	//	if((s32)value > (s32)maxValue)
	//	{
	//		//value = (s32)maxValue;
	//		overflow_next = 1;
	//	}
	//	if((s32)curValue < (s32)minValue)
	//	{
	//		//value = (s32)minValue;
	//		overflow = 1;
	//	}
	//	if((s32)value < (s32)minValue)
	//	{
	//		//value = (s32)minValue;
	//		overflow_next = 1;
	//	}
	//	if (enableWaiting && waitingValue == value)
	//	{
	//		hitWaitingValue = 1;
	//	}
	//	//if((s32)value < 0){
	//	//	value = -(s32)value;
	//	//}
	//}

	if (enableWaiting && curValue == waitingValue)
	{
		nextHitWaitingValue = 1; //curValue is old value for animation
	}

	if(leadZero == 0 && !nextHitWaitingValue)
	{
		if(curValue == 0)
			numOfDisTexture += 1;
		else
		{
			numOfDisTexture += 0;
			for(j = (s32)curValue > 0 ? curValue : -curValue; j >0; j /= radixBase)
			{
				numOfDisTexture++;
			}
		}
		if(pointPosRight != 0) //at least display (number of fraction + 1)
			if(numOfDisTexture < pointPosRight+1)
				numOfDisTexture = pointPosRight+1;
	}
	else 
		numOfDisTexture = numofNumber;

	if(leadZero == 0 && !hitWaitingValue)
	{
		if(value == 0)
			nextNumOfDisTexture += 1;
		else
		{
			nextNumOfDisTexture += 0;
			for(j = (s32)value > 0 ? (s32)value : (-(s32)value); j >0; j /= radixBase)
			{
				nextNumOfDisTexture++;
			}
		}
		if(pointPosRight != 0) //at least display (number of fraction + 1)
			if(nextNumOfDisTexture < pointPosRight+1)
				nextNumOfDisTexture = pointPosRight+1;
	}
	else 
		nextNumOfDisTexture = numofNumber;

	//overflow style 0:display max 1:not display
	if(overflowStyle == OVERFLOW_DISPLAY)
	{
		overflow = 0;
		overflow_next = 0;
	}

	tens=1;
	showingZero=0;
	signFlag = 1 - sign; 
	dataMinus=0;


	for(i=0;i<numofNumber;i++) 
		//tens *=10;
		tens *= radixBase;

	pointPosLeft = numofNumber - pointPosRight;//the num of number left to point

	if(pointPosLeft <1 ) pointPosLeft = 1;

	if(sign)
	{
		numofNumber++; //the number of texture, not num
		pointPosLeft++;//the number of char left to point
		if((s32)curValue < 0) //sign of '-'
		{
			numOfDisTexture++;
		}
		if((s32)value < 0)
			nextNumOfDisTexture++;
	}

	if(pointPosRight != 0 )
	{
		numofNumber++;
#if 0
		//no need to judge if the data is zero
		if(curValue != 0)
			numOfDisTexture++; //sign of '.'
		if(value != 0)
			nextNumOfDisTexture++;
#endif	
		numOfDisTexture++; //sign of '.'
		nextNumOfDisTexture++;
	}

	#if (defined VERSION_2) && (defined EMBEDDED)  
	mSourceBufferStartPos = *u32p_sourceShift;
	p_wptr->SourceBufferStartPos = mSourceBufferStartPos;
	#endif

	//draw puremask texture
	texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);
	status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
	if(status == AHMI_FUNC_MATRIX_ILLEGAL)
	{
		return status;
	}

	//get first alpha texture
	texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex+1]);
	texturePtr->mTexAttr |= MASKTYPE_OR;
	#ifdef EMBEDDED
	if((texturePtr < (void*)startOfDynamicPage) || (texturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
	#endif

	//cal tex padding, for example fontsize is 64px, tex is 78px
	fontPadding = (texturePtr->TexWidth - fontWidth)/2;
	

	//judge the dirction of moving(animation)
	uValue[0] = (u32)value;
	sValue[0] = (s32)value;
	uValue[1] = (u32)curValue;
	sValue[1] = (s32)curValue;

	if((p_wptr->NumOfLine & 0xff) == 2) 
	{
		if(movedirection == 0)//Increase downwards and decrease upwards
		{
			if(sign)
			{
				if(sValue[0] >= (s32)curValue)
				{
					//down
					movingDir = 0;
				}
				else if(sValue[0] < (s32)curValue)
				{
					 //up
					movingDir = 1;
				}
				else
				{
					//not moving
					//if(u8_pageRefresh == 0) 
					//return AHMI_FUNC_SUCCESS;
				}
			}
			else
			{
				if(uValue[0] >= (u32)curValue)
				{
					//down
					movingDir = 0;
				}
				else if(uValue[0] < (u32)curValue)
				{
					 //up
					movingDir = 1;
				}
				else
				{
					//not moving
					//if(u8_pageRefresh == 0)
					//return AHMI_FUNC_SUCCESS;
				}
			}
		}
		else if(movedirection == 1)//up
		{
			movingDir = 1;
		}
		else if(movedirection == 2)//down
		{
			movingDir = 0;
		}
		else if(movedirection == 3)//Decrease downwards and increase upwards, default
		{
			if(sign)
			{
				if(sValue[0] >= (s32)curValue)
				{
					 //up
					movingDir = 1;					
				}
				else if(sValue[0] < (s32)curValue)
				{
					//down
					movingDir = 0;
				}
				else
				{
					//not moving
					//if(u8_pageRefresh == 0) 
					//return AHMI_FUNC_SUCCESS;
				}
			}
			else
			{
				if(uValue[0] >= (u32)curValue)
				{
					//up
					movingDir = 1;					
				}
				else if(uValue[0] < (u32)curValue)
				{
					//down
					movingDir = 0; 
				}
				else
				{
					//not moving
					//if(u8_pageRefresh == 0)
					//return AHMI_FUNC_SUCCESS;
				}
			}
			
		}
	}
	else{
		movingDir = 0;
	}
	if(movingDir == 0) //down
	{
		p_wptr->PreviousTexturePtrFlag = 0;
	}
	else if(movingDir == 1)//up
	{
		p_wptr->PreviousTexturePtrFlag = 1;
	}
	else if(u8_pageRefresh == 0 && (overflowStyle == 0))
	{
		//ERROR_PRINT("error in parsing number");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
	}

	if((overflowStyle == OVERFLOW_NON_DISPLAY) && (overflow))
	{
		numOfDisTexture = 0;
	}

	if(nextHitWaitingValue)
	{
		numOfDisTexture = numofWaitString;
	}

	//judge the assign mode
	if (dotLayout)
	{//dot 1 char
		if(unitflag)// if unit display need add num of unit string texture
		{
			numOfDisTexture += (numofUnitString - Number1PixelUnitChar);
		}

		showNumWidth = (fontWidth + SpacingX) * (numOfDisTexture - 1) + fontWidth + Number1PixelUnitChar;
	}else
	{//dot 0.5 char
		if(unitflag)// if unit display need add num of unit string texture
		{
			numOfDisTexture += (numofUnitString - Number1PixelUnitChar);
		}

		if((numofNumber - 1 <= pointPosLeft) || (nextHitWaitingValue) || ((overflowStyle == OVERFLOW_NON_DISPLAY) && (overflow)))
		{
			showNumWidth = (fontWidth + SpacingX) * (numOfDisTexture - 1) + fontWidth + Number1PixelUnitChar;
		}
		else
		{
			showNumWidth = (fontWidth + SpacingX) * (numOfDisTexture - 1) - fontWidth/2 + fontWidth + Number1PixelUnitChar;
		}
	}
	
	if(align == RIGHTALIGN)   //default right align
	{
		xCoordinate = (showNumWidth >  (s16)p_wptr->WidgetWidth - 2 * paddingX ) ? 0 : ((s16)p_wptr->WidgetWidth - 2 * paddingX - showNumWidth);
	}
	else if(align == CENTERALIGN)  //middle align
	{
		xCoordinate = (showNumWidth >  (s16)p_wptr->WidgetWidth - 2 * paddingX ) ? 0 : ((s16)p_wptr->WidgetWidth - 2 * paddingX - showNumWidth)/2;
	}
	else if(align == LEFTALIGN) //left align mode
	{
		xCoordinate = 0;

	}
	xCoordinate += paddingX;//always has a padding;

	if(sValue[1] < 0)
	{
		dataMinus = 1;
		sValue[1] = - sValue[1];
		
	}

	// cut the out of range number
	uValue[1] = uValue[1] % tens;
	sValue[1] = sValue[1] % tens;
	// tens /= 10;
	tens /= radixBase;
	
	if(nextHitWaitingValue)
	{
		for(i=0;i<numofWaitString;i++)
		{
			texturePtr->FocusedSlice = ((WaitString >> (8 * i)) & 0xFF) - 0x20;
			texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
			texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
			texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
			status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
			if(status == AHMI_FUNC_MATRIX_ILLEGAL)
			{
				return status;
			}
			xCoordinate += fontWidth;
			xCoordinate += SpacingX;
		}
		//for(i=0;i<numofNumber;i++)
		//{
		//	if(i >= (numofNumber - numofWaitString))
		//	{
		//		texturePtr->FocusedSlice = ((WaitString >> (8 * (i - (numofNumber - numofWaitString)))) & 0xFF) - 0x20;
		//		texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
		//		texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
		//		texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
		//		status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
		//		if(status == AHMI_FUNC_MATRIX_ILLEGAL)
		//		{
		//			return status;
		//		}
		//		xCoordinate += fontWidth;
		//		xCoordinate += SpacingX;
		//	}
		//	else
		//	{
		//		texturePtr->FocusedSlice = 0;
		//	}
		//}
	}
	else
	{
		//operate the value of texture
		//pre
		for(i=0;i<numofNumber;i++)
		{

			if(sign)
			{
				code = (u16)(sValue[1] / tens);		
				sValue[1] %= tens;
			}
			else
			{
				code = (u16)(uValue[1] / tens);
				uValue[1] %= tens;
			}

			//sign mode
			if( sign         && dataMinus && (leadZero              || ( !leadZero && code != 0 )               || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
			//  display sign    negtive        display leading zero       not display leading zero and not zero                              arriving 2 number before point        did not display '-'
			{
				if(overflow == 1)
				{
					texturePtr->FocusedSlice = ' ' - 0x20;
				}
				else
				{
					texturePtr->FocusedSlice = '-' - 0x20;
				}
				if(texturePtr->FocusedSlice == 0)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
				texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
				texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(nextHitWaitingValue != 1)
				{
					if (dotLayout)
					{
						if(pointPosRight != 0)
						{
							//dot 1 char
							if (i == pointPosLeft-1)
							{
								xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
							}else if (i == pointPosLeft)
							{
								xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}else
					{
						if(pointPosRight != 0)
						{
							//dot 0.5 char
							if (i == pointPosLeft)
							{
								xCoordinate += fontWidth/2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}
				}
				else
				{
					xCoordinate += fontWidth;
				}
				xCoordinate += SpacingX;
				//texturePtr++;

				dataMinus = 0;//'-' is displayed, assume it as postive
				signFlag = 1; //already display sign
				i++;
			}
			//display ' ' before the positive number
			else if( sign && !dataMinus && (leadZero || ( !leadZero && code != 0 ) || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
			//       sign      postive      display '0'    no display '0' and not 0     no display '0' and arriving 2 number before point                 didnot display sign yet
			{
				if(overflow == 1)
				{
					texturePtr->FocusedSlice = ' ' - 0x20;
					if(texturePtr->FocusedSlice == 0)
					{
						continue;
					}
					texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
					texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
					texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
					status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
					if(status == AHMI_FUNC_MATRIX_ILLEGAL)
					{
						return status;
					}

					if(nextHitWaitingValue != 1)
					{
						if (dotLayout)
						{
							if(pointPosRight != 0)
							{
								//dot 1 char
								if (i == pointPosLeft-1)
								{
									xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
								}else if (i == pointPosLeft)
								{
									xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
								}
								else
								{
									xCoordinate += fontWidth;
								}
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}else
						{
							if(pointPosRight != 0)
							{
								//dot 0.5 char
								if (i == pointPosLeft)
								{
									xCoordinate += fontWidth/2;
								}
								else
								{
									xCoordinate += fontWidth;
								}
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
					}
					else
					{
						xCoordinate += fontWidth;
					}
					xCoordinate += SpacingX;
				}
				else
				{
					texturePtr->FocusedSlice = 0;
				}
				//texturePtr++;

				signFlag = 1;//already display sign
				i++;
			}
			//positve number display ' '
			else if( sign && !dataMinus && (leadZero || ( !leadZero && code != 0 ) ||     (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
				//  sign      positive      display '0'    not display '0' and not zero               not display zero and arriving the left 2 to point
			{
				texturePtr->FocusedSlice = ' ' - 0x20;
				if(texturePtr->FocusedSlice == 0)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
				texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
				texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(nextHitWaitingValue != 1)
				{
					if (dotLayout)
					{
						if(pointPosRight != 0)
						{
							//dot 1 char
							if (i == pointPosLeft-1)
							{
								xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
							}else if (i == pointPosLeft)
							{
								xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}else
					{
						if(pointPosRight != 0)
						{
							//dot 0.5 char
							if (i == pointPosLeft)
							{
								xCoordinate += fontWidth/2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}
				}
				else
				{
					xCoordinate += fontWidth;
				}
				xCoordinate += SpacingX;
				//texturePtr++;
				signFlag = 1;
				i++;
			}
		
			// display '0x'
			if(numRadix==1&&markingMode==1&&i==markingPos){

				if (overflow == 1)
				{
					texturePtr->FocusedSlice = ' ' - 0x20;
				}else
				{
					texturePtr->FocusedSlice = 'x' - 0x20;
				}
				if(texturePtr->FocusedSlice == 0)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
				texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
				texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(nextHitWaitingValue != 1)
				{
					if (dotLayout)
					{
						if(pointPosRight != 0)
						{
							//dot 1 char
							if (i == pointPosLeft-1)
							{
								xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
							}else if (i == pointPosLeft)
							{
								xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}else
					{
						if(pointPosRight != 0)
						{
							//dot 0.5 char
							if (i == pointPosLeft)
							{
								xCoordinate += fontWidth/2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}
				}
				else
				{
					xCoordinate += fontWidth;
				}
				xCoordinate += SpacingX;
				tens /= radixBase;
				//texturePtr++;
				continue;
			}else if(numRadix==1&&markingMode==1&&i==(markingPos-1)){
				if (overflow == 1)
				{
					texturePtr->FocusedSlice = ' ' - 0x20;
				}else
				{
					texturePtr->FocusedSlice = '0' - 0x20;
				}
				tens /= radixBase;
				if(texturePtr->FocusedSlice == 0)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
				texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
				texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(nextHitWaitingValue != 1)
				{
					if (dotLayout)
					{
						if(pointPosRight != 0)
						{
							//dot 1 char
							if (i == pointPosLeft-1)
							{
								xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
							}else if (i == pointPosLeft)
							{
								xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}else
					{
						if(pointPosRight != 0)
						{
							//dot 0.5 char
							if (i == pointPosLeft)
							{
								xCoordinate += fontWidth/2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}
				}
				else
				{
					xCoordinate += fontWidth;
				}
				xCoordinate += SpacingX;
				//texturePtr++;
				continue;
			}

			//display point
			if(i == pointPosLeft)
			{
				if(overflow == 1)
					texturePtr->FocusedSlice = ' ' - 0x20;
				else
					texturePtr->FocusedSlice = '.' - 0x20;//��ʾС����

				if(texturePtr->FocusedSlice == 0)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
				texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
				texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(nextHitWaitingValue != 1)
				{
					if (dotLayout)
					{
						if(pointPosRight != 0)
						{
							//dot 1 char
							if (i == pointPosLeft-1)
							{
								xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
							}else if (i == pointPosLeft)
							{
								xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}else
					{
						if(pointPosRight != 0)
						{
							//dot 0.5 char
							if (i == pointPosLeft)
							{
								xCoordinate += fontWidth/2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}
				}
				else
				{
					xCoordinate += fontWidth;
				}
				xCoordinate += SpacingX;
				//texturePtr++;
//				offsetpoint = i;
				i++;
			}

				

			//display leading zeros
			if (!showingZero          && code == 0     && !leadZero        && (i != pointPosLeft - 1))
			//   not display rest '0'    zero       not display leading '0'     didn't arrive the point
			{
				code=' ' - 0x20;//show ' '
			}
			//display the '0' before point
			else if(!showingZero && code == 0 && !leadZero && i == pointPosLeft - 1)
			{
				code= '0' - 0x20;//display '0'
				showingZero = 1;
			}
			else
			{
				if(code>9){
					// hex mode
					if(numRadix==1){
						if(transformMode==0){
							code += lowerLetterOffset;
						}else if(transformMode ==1){
							code += upperLetterOffset;
						}
					}
				}
				code = code + 0x30/*the start code of ASIC*/ - 0x20/*the start code of our texture's code*/;

				showingZero = 1;
			}
			// tens =tens /10;
			tens /= radixBase;
			if(overflow == 1)
			{
				texturePtr->FocusedSlice = ' ' - 0x20;
				if(texturePtr->FocusedSlice == 0)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
				texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
				texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(nextHitWaitingValue != 1)
				{
					if (dotLayout)
					{
						if(pointPosRight != 0)
						{
							//dot 1 char
							if (i == pointPosLeft-1)
							{
								xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
							}else if (i == pointPosLeft)
							{
								xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}else
					{
						if(pointPosRight != 0)
						{
							//dot 0.5 char
							if (i == pointPosLeft)
							{
								xCoordinate += fontWidth/2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}
				}
				else
				{
					xCoordinate += fontWidth;
				}
				xCoordinate += SpacingX;
			}
			else
			{
				if (nextHitWaitingValue)
				{
					texturePtr->FocusedSlice = '-' - 0x20; 
				}else{
					texturePtr->FocusedSlice = code;
				}
				if(texturePtr->FocusedSlice == 0)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
				texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
				texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(nextHitWaitingValue != 1)
				{
					if (dotLayout)
					{
						if(pointPosRight != 0)
						{
							//dot 1 char
							if (i == pointPosLeft-1)
							{
								xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
							}else if (i == pointPosLeft)
							{
								xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}else
					{
						if(pointPosRight != 0)
						{
							//dot 0.5 char
							if (i == pointPosLeft)
							{
								xCoordinate += fontWidth/2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}
				}
				else
				{
					xCoordinate += fontWidth;
				}
				xCoordinate += SpacingX;
			}
			//texturePtr++;
		}
	}

	//set unit code
	for(i = 0; i < numofUnitString; i++)
	{
		texturePtr->FocusedSlice = ((UnitString >> (8 * i)) & 0xFF) - 0x20;
		if(texturePtr->FocusedSlice == 0)
		{
			if(i < Number1PixelUnitChar)
				xCoordinate += 1;
			else
				xCoordinate += (fontWidth + SpacingX);

			continue;
		}
		texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
		texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
		texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
		status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
		if(status == AHMI_FUNC_MATRIX_ILLEGAL)
		{
			return status;
		}

		if(nextHitWaitingValue != 1)
		{
			if (dotLayout)
			{
				if(pointPosRight != 0)
				{
					//dot 1 char
					if (i == pointPosLeft-1)
					{
						xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
					}else if (i == pointPosLeft)
					{
						xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
					}
					else
					{
						xCoordinate += fontWidth;
					}
				}
				else
				{
					xCoordinate += fontWidth;
				}
			}else
			{
				if(pointPosRight != 0)
				{
					//dot 0.5 char
					if (i == pointPosLeft)
					{
						xCoordinate += fontWidth/2;
					}
					else
					{
						xCoordinate += fontWidth;
					}
				}
				else
				{
					xCoordinate += fontWidth;
				}
			}
		}
		else
		{
			xCoordinate += fontWidth;
		}
		xCoordinate += SpacingX;
	}

	//next, if contains animation, the next line do the same thing with first line except the y position
	if((p_wptr->NumOfLine & 0xff) == 2) //with animation
	{
		if(movingDir == 0) //down
		{
			if(texturePtr->OffsetY != -(p_wptr->WidgetHeight << 4))
			{
				TextureOffsetY = texturePtr->OffsetY - (p_wptr->WidgetHeight << 4);
				texturePtr->OffsetY = TextureOffsetY;
			}
			else
			{
				texturePtr->OffsetY = 0;
			}
		}
		else if(movingDir == 1)//up
		{
			if(texturePtr->OffsetY != (p_wptr->WidgetHeight << 4))
			{
				TextureOffsetY = texturePtr->OffsetY + (p_wptr->WidgetHeight << 4);
				texturePtr->OffsetY = TextureOffsetY;
			}
			else
			{
				texturePtr->OffsetY = 0;
			}
		}

		if((overflowStyle == OVERFLOW_NON_DISPLAY) && (overflow_next))
		{
			nextNumOfDisTexture = 0;
		}

		if(hitWaitingValue)
		{
			nextNumOfDisTexture = numofWaitString;
		}

		if (dotLayout)
		{//dot 1 char
			if(unitflag)// if unit display need add num of unit string texture
			{
				nextNumOfDisTexture += (numofUnitString - Number1PixelUnitChar);
			}

			showNumWidth = (fontWidth + SpacingX) * (nextNumOfDisTexture - 1) + fontWidth + Number1PixelUnitChar;
		}else
		{//dot 0.5 char
			if(unitflag)// if unit display need add num of unit string texture
			{
				nextNumOfDisTexture += (numofUnitString - Number1PixelUnitChar);
			}

			if((numofNumber - 1 <= pointPosLeft) || (hitWaitingValue) || ((overflowStyle == OVERFLOW_NON_DISPLAY) && (overflow_next)))
			{
				showNumWidth = (fontWidth + SpacingX) * (nextNumOfDisTexture - 1) + fontWidth + Number1PixelUnitChar;
			}
			else
			{
				showNumWidth = (fontWidth + SpacingX) * (nextNumOfDisTexture - 1) - fontWidth/2 + fontWidth + Number1PixelUnitChar;
			}
		}
	
		if(align == RIGHTALIGN)   //default right align
		{
			xCoordinate = (showNumWidth >  (s16)p_wptr->WidgetWidth - 2 * paddingX ) ? 0 : ((s16)p_wptr->WidgetWidth - 2 * paddingX - showNumWidth);
		}
		else if(align == CENTERALIGN)  //middle align
		{
			xCoordinate = (showNumWidth >  (s16)p_wptr->WidgetWidth - 2 * paddingX ) ? 0 : ((s16)p_wptr->WidgetWidth - 2 * paddingX - showNumWidth)/2;
		}
		else if(align == LEFTALIGN) //left align mode
		{
			xCoordinate = 0;

		}
		//xCoordinate += (s16)p_wptr->WidgetOffsetX;//absolute offsetX
		xCoordinate += paddingX;//always has a padding;

		tens=1;
		signFlag = 1 - sign; 
		showingZero = 0;
		numofNumber = (p_wptr->WidgetAttr >>5) & 0xf;


		for(i=0;i<numofNumber;i++) 
			tens *=radixBase;

		if(sign)
		{
			numofNumber++; //the number of texture, not the number of char
		}

		if(pointPosRight != 0 )
		{
			numofNumber++;
		}

		dataMinus = 0;
		if(sValue[0] < 0)
		{
			dataMinus = 1;
			sValue[0] = - sValue[0];
		}
		// cut the number out of range
		uValue[0] = uValue[0] % tens;
		sValue[0] = sValue[0] % tens;
		tens /= radixBase;
		if(hitWaitingValue)
		{
			for(i=0;i<numofWaitString;i++)
			{
				texturePtr->FocusedSlice = ((WaitString >> (8 * i)) & 0xFF) - 0x20;
				texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
				texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
				texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}
				xCoordinate += fontWidth;
				xCoordinate += SpacingX;
			}
			//for(i=0;i<numofNumber;i++)
			//{
			//	if(i >= (numofNumber - numofWaitString))
			//	{
			//		texturePtr->FocusedSlice = ((WaitString >> (8 * (i - (numofNumber - numofWaitString)))) & 0xFF) - 0x20;
			//		texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
			//		texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
			//		texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
			//		status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
			//		if(status == AHMI_FUNC_MATRIX_ILLEGAL)
			//		{
			//			return status;
			//		}
			//		
			//		xCoordinate += fontWidth;
			//		xCoordinate += SpacingX;
			//	}
			//	else
			//	{
			//		texturePtr->FocusedSlice = 0;
			//	}
			//}
		}
		else
		{
			for(i=0;i<numofNumber;i++)
			{

				if(sign)
				{
					code = (u16)(sValue[0] / tens);		
					sValue[0] %= tens;
				}
				else
				{
					code = (u16)(uValue[0] / tens);
					uValue[0] %= tens;
				}

				//display '-'
				if( sign &&   dataMinus &&     (leadZero ||               ( !leadZero && code != 0 )                 || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
				//  display sign    negtive        display leading zero       not display leading zero and not zero                   arriving 2 number before point                    did not display '-'
				{
					if(overflow_next == 1)
						texturePtr->FocusedSlice = ' ' - 0x20;
					else
						texturePtr->FocusedSlice = '-' - 0x20;
					if(texturePtr->FocusedSlice == 0)
					{
						continue;
					}
					texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
					texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
					texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
					status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
					if(status == AHMI_FUNC_MATRIX_ILLEGAL)
					{
						return status;
					}

					if(hitWaitingValue != 1)
					{
						if (dotLayout)
						{
							if(pointPosRight != 0)
							{
								//dot 1 char
								if (i == pointPosLeft-1)
								{
									xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
								}else if (i == pointPosLeft)
								{
									xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
								}
								else
								{
									xCoordinate += fontWidth;
								}
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}else
						{
							if(pointPosRight != 0)
							{
								//dot 0.5 char
								if (i == pointPosLeft)
								{
									xCoordinate += fontWidth/2;
								}
								else
								{
									xCoordinate += fontWidth;
								}
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
					}
					else
					{
						xCoordinate += fontWidth;
					}
					xCoordinate += SpacingX;
					//nextTexturePtr++;

					dataMinus = 0;//'-' has been displayed, assume it to be positive
					signFlag = 1;
					i++;
				}
				//not showing the sign of positive number
				else if( sign && !dataMinus && (leadZero || ( !leadZero && code != 0 ) || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
				//       sign      postive      display '0'    no display '0' and not 0     no display '0' and arriving 2 number before point                 didnot display sign yet
				{
					if(overflow_next == 1)
					{
						texturePtr->FocusedSlice = ' ' - 0x20;
						if(texturePtr->FocusedSlice == 0)
						{
							continue;
						}
						texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
						texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
						texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
						status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
						if(status == AHMI_FUNC_MATRIX_ILLEGAL)
						{
							return status;
						}

						if(hitWaitingValue != 1)
						{
							if (dotLayout)
							{
								if(pointPosRight != 0)
								{
									//dot 1 char
									if (i == pointPosLeft-1)
									{
										xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
									}else if (i == pointPosLeft)
									{
										xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
									}
									else
									{
										xCoordinate += fontWidth;
									}
								}
								else
								{
									xCoordinate += fontWidth;
								}
							}else
							{
								if(pointPosRight != 0)
								{
									//dot 0.5 char
									if (i == pointPosLeft)
									{
										xCoordinate += fontWidth/2;
									}
									else
									{
										xCoordinate += fontWidth;
									}
								}
								else
								{
									xCoordinate += fontWidth;
								}
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
						xCoordinate += SpacingX;
					}
					else
					{
						texturePtr->FocusedSlice = 0;
					}
					//nextTexturePtr++;

					signFlag = 1;
					i++;
				}
				//display ' ' for the first char of positive number
				else if( sign && !dataMinus && (leadZero || ( !leadZero && code != 0 ) || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
				//       sign      postive      display '0'    no display '0' and not 0     no display '0' and arriving 2 number before point                 didnot display sign yet
				{
					texturePtr->FocusedSlice = ' ' - 0x20;
					if(texturePtr->FocusedSlice == 0)
					{
						continue;
					}
					texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
					texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
					texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
					status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
					if(status == AHMI_FUNC_MATRIX_ILLEGAL)
					{
						return status;
					}

					if(hitWaitingValue != 1)
					{
						if (dotLayout)
						{
							if(pointPosRight != 0)
							{
								//dot 1 char
								if (i == pointPosLeft-1)
								{
									xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
								}else if (i == pointPosLeft)
								{
									xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
								}
								else
								{
									xCoordinate += fontWidth;
								}
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}else
						{
							if(pointPosRight != 0)
							{
								//dot 0.5 char
								if (i == pointPosLeft)
								{
									xCoordinate += fontWidth/2;
								}
								else
								{
									xCoordinate += fontWidth;
								}
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
					}
					else
					{
						xCoordinate += fontWidth;
					}
					xCoordinate += SpacingX;
					//nextTexturePtr++;
					signFlag = 1;
					i++;
				}

				// display '0x'
			if(numRadix==1&&markingMode==1&&i==nextMarkingPos){

				if (overflow_next == 1)
				{
					texturePtr->FocusedSlice = ' ' - 0x20;
				}else
				{
					texturePtr->FocusedSlice = 'x' - 0x20;
				}
				if(texturePtr->FocusedSlice == 0)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
				texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
				texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(hitWaitingValue != 1)
				{
					if (dotLayout)
					{
						if(pointPosRight != 0)
						{
							//dot 1 char
							if (i == pointPosLeft-1)
							{
								xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
							}else if (i == pointPosLeft)
							{
								xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}else
					{
						if(pointPosRight != 0)
						{
							//dot 0.5 char
							if (i == pointPosLeft)
							{
								xCoordinate += fontWidth/2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}
				}
				else
				{
					xCoordinate += fontWidth;
				}
				xCoordinate += SpacingX;
				tens /= radixBase;
				//nextTexturePtr++;
				continue;
			}else if(numRadix==1&&markingMode==1&&i==(nextMarkingPos-1)){
				if (overflow_next == 1)
				{
					texturePtr->FocusedSlice = ' ' - 0x20;
				}else
				{
					texturePtr->FocusedSlice = '0' - 0x20;
				}
				tens /= radixBase;
				if(texturePtr->FocusedSlice == 0)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
				texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
				texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(hitWaitingValue != 1)
				{
					if (dotLayout)
					{
						if(pointPosRight != 0)
						{
							//dot 1 char
							if (i == pointPosLeft-1)
							{
								xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
							}else if (i == pointPosLeft)
							{
								xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}else
					{
						if(pointPosRight != 0)
						{
							//dot 0.5 char
							if (i == pointPosLeft)
							{
								xCoordinate += fontWidth/2;
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}
				}
				else
				{
					xCoordinate += fontWidth;
				}
				xCoordinate += SpacingX;
				//nextTexturePtr++;
				continue;
			}

				//show '.'
				if(i == pointPosLeft)
				{
					if(overflow_next == 1)
						texturePtr->FocusedSlice = ' ' - 0x20;
					else
						texturePtr->FocusedSlice = '.' - 0x20;//��ʾС����

					if(texturePtr->FocusedSlice == 0)
					{
						continue;
					}
					texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
					texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
					texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
					status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
					if(status == AHMI_FUNC_MATRIX_ILLEGAL)
					{
						return status;
					}

					if(hitWaitingValue != 1)
					{
						if (dotLayout)
						{
							if(pointPosRight != 0)
							{
								//dot 1 char
								if (i == pointPosLeft-1)
								{
									xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
								}else if (i == pointPosLeft)
								{
									xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
								}
								else
								{
									xCoordinate += fontWidth;
								}
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}else
						{
							if(pointPosRight != 0)
							{
								//dot 0.5 char
								if (i == pointPosLeft)
								{
									xCoordinate += fontWidth/2;
								}
								else
								{
									xCoordinate += fontWidth;
								}
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
					}
					else
					{
						xCoordinate += fontWidth;
					}
					xCoordinate += SpacingX;
					//nextTexturePtr++;
				//offsetpoint = i;
					i++;
				}

				//not show leading zeros
				if (!showingZero && code == 0 && !leadZero && (i != pointPosLeft - 1))
				{
					code=' ' - 0x20;
				}
				//showing the leading zeros
				else if(!showingZero && code == 0 && !leadZero && i == pointPosLeft - 1)
				{
					code= '0' - 0x20;
					showingZero = 1;
				}
				else
				{
					if(code>9){
						// hex mode
						if(numRadix==1){
							if(transformMode==0){
								code += lowerLetterOffset;
							}else if(transformMode ==1){
								code += upperLetterOffset;
							}
						}
					}
					code = code + 0x30/*the start code of number '0' in ASIC*/ - 0x20/*delete the control code*/;
					showingZero = 1;
				}
				tens =tens /radixBase;
				if(overflow_next == 1)
				{
					texturePtr->FocusedSlice = ' ' - 0x20;
					if(texturePtr->FocusedSlice == 0)
					{
						continue;
					}
					texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
					texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
					texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
					status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
					if(status == AHMI_FUNC_MATRIX_ILLEGAL)
					{
						return status;
					}

					if(hitWaitingValue != 1)
					{
						if (dotLayout)
						{
							if(pointPosRight != 0)
							{
								//dot 1 char
								if (i == pointPosLeft-1)
								{
									xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
								}else if (i == pointPosLeft)
								{
									xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
								}
								else
								{
									xCoordinate += fontWidth;
								}
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}else
						{
							if(pointPosRight != 0)
							{
								//dot 0.5 char
								if (i == pointPosLeft)
								{
									xCoordinate += fontWidth/2;
								}
								else
								{
									xCoordinate += fontWidth;
								}
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
					}
					else
					{
						xCoordinate += fontWidth;
					}
					xCoordinate += SpacingX;
				}
				else
				{
					if (hitWaitingValue)
					{
						texturePtr->FocusedSlice = '-' - 0x20;
					}else{
						texturePtr->FocusedSlice = code;
					}
					if(texturePtr->FocusedSlice == 0)
					{
						continue;
					}
					texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
					texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
					texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
					status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
					if(status == AHMI_FUNC_MATRIX_ILLEGAL)
					{
						return status;
					}

					if(hitWaitingValue != 1)
					{
						if (dotLayout)
						{
							if(pointPosRight != 0)
							{
								//dot 1 char
								if (i == pointPosLeft-1)
								{
									xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
								}else if (i == pointPosLeft)
								{
									xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
								}
								else
								{
									xCoordinate += fontWidth;
								}
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}else
						{
							if(pointPosRight != 0)
							{
								//dot 0.5 char
								if (i == pointPosLeft)
								{
									xCoordinate += fontWidth/2;
								}
								else
								{
									xCoordinate += fontWidth;
								}
							}
							else
							{
								xCoordinate += fontWidth;
							}
						}
					}
					else
					{
						xCoordinate += fontWidth;
					}
					xCoordinate += SpacingX;
				//nextTexturePtr++;
				}
			}
		}

		//set unit code
		for(i = 0; i < numofUnitString; i++)
		{
			texturePtr->FocusedSlice = ((UnitString >> (8 * i)) & 0xFF) - 0x20;
			if(texturePtr->FocusedSlice == 0)
			{
				if(i < Number1PixelUnitChar)
					xCoordinate += 1;
				else
					xCoordinate += (fontWidth + SpacingX);

				continue;
			}
			texturePtr->OffsetX = (xCoordinate-fontPadding) << 4;
			texturePtr->TexLeftTileBox = texturePtr->OffsetX >> 9;
			texturePtr->TexRightTileBox = (texturePtr->OffsetX + texturePtr->TexWidth * 16) >> 9;
			status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
			if(status == AHMI_FUNC_MATRIX_ILLEGAL)
			{
				return status;
			}

			if(hitWaitingValue != 1)
			{
				if (dotLayout)
				{
					if(pointPosRight != 0)
					{
						//dot 1 char
						if (i == pointPosLeft-1)
						{
							xCoordinate += fontWidth + (texturePtr->TexWidth)*0.2 ;// dot is manually moved left by 20%
						}else if (i == pointPosLeft)
						{
							xCoordinate += fontWidth - (texturePtr->TexWidth)*0.2;
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}
					else
					{
						xCoordinate += fontWidth;
					}
				}else
				{
					if(pointPosRight != 0)
					{
						//dot 0.5 char
						if (i == pointPosLeft)
						{
							xCoordinate += fontWidth/2;
						}
						else
						{
							xCoordinate += fontWidth;
						}
					}
					else
					{
						xCoordinate += fontWidth;
					}
				}
			}
			else
			{
				xCoordinate += fontWidth;
			}
			xCoordinate += SpacingX;
		}

		if(movingDir == 0) //down
		{
			if(texturePtr->OffsetY != -(p_wptr->WidgetHeight << 4))
			{
				TextureOffsetY = texturePtr->OffsetY + (p_wptr->WidgetHeight << 4);
				texturePtr->OffsetY = TextureOffsetY;
			}
			else
			{
				texturePtr->OffsetY = 0;
			}
		}
		else if(movingDir == 1)//up
		{
			TextureOffsetY = texturePtr->OffsetY - (p_wptr->WidgetHeight << 4);
			texturePtr->OffsetY = TextureOffsetY;
		}
	}

	//write background
	texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 2]);
	status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
	if(status == AHMI_FUNC_MATRIX_ILLEGAL)
	{
		return status;
	}

	#if (defined VERSION_2) && (defined EMBEDDED)
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
	#endif

	return AHMI_FUNC_SUCCESS;
}
#endif

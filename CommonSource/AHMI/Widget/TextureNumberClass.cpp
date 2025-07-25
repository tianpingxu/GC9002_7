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
// Revision 1.6.1 - add puremask for animation mode
// Additional Comments:
//    add comments by zuz 2018/10/29
//    all comments, count on Number class 
//    
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "TextureNumberClass.h"
#include "publicType.h"
#include "drawImmediately_cd.h"
#include "Easing.h"
#if defined(CHIP_GC9002) || defined(CHIP_GC9005)
#include "SBBatchDraw.h"
#endif

#ifdef AHMI_CORE
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;
//extern u16 WorkingPageID;

extern QueueHandle_t		ActionInstructionQueue;

extern u32 startOfDynamicPage;
extern u32 endOfDynamicPage;
extern u8 animationDuration;
extern Easing gEasing;

#if defined(VERSION_2) && defined(EMBEDDED)
extern uint32_t gNewSoureBufferSize;
#endif

//-----------------------------
// name： NumberClass
// construct
//  
//
// addtional comments :
//   none
//-----------------------------
TextureNumberClass::TextureNumberClass()
{

}

//-----------------------------
// name： ~NumberClass
// deconstruct
//  
//
// addtional comments :
//   none
//-----------------------------
TextureNumberClass::~TextureNumberClass()
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
funcStatus TextureNumberClass::initWidget(
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
//layer number widget
//@see NumberClass
//the difference is that textureNumberClass reads the number from image, not ALPHA4
//WidgetAttr：
//14:sign mode：0 for unsigned number，range 4294967295~0; 1 for signed number，range 2147483647~-2147483648
//13:leading zeros mode, 0 not display leading, 1 for display
//12-9:how many fraction number to display, 
//8-5:total number for display
//4-0:widget type, 0x8
//------------------------------------------------------------
//|image  |0	1	2	3	4	5	6	7	8	9	.	+	-
//|slice  |0	1	2	3	4	5	6	7	8	9	10	11	12
//------------------------------------------------------------
//**********************************
funcStatus TextureNumberClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{
    TextureClassPtr nextTexturePtr, texturePtr;
	TextureClassPtr pureMaskTexturePtr; // the texture of puremask, only used for animation mode. zuz 20190109
    u64 tens;
	u32 value;
	u32 uValue[5] = {0};
	s32 sValue[5] = {0};
	s32	maxValue;
	s32	minValue;
	s32 curValue;
    s32 j;
	s16 temp;
    u16 widthOfFont;
	u16 shiftSize;
	u16 code;
	u8  leadZero;
	u8  sign;
	u8  pointPosRight;
	u8	pointPosLeft;
	u8  numofNumber;
	u8	i;
	u8	showingZero; 
	u8	signFlag;                     
	u8	dataMinus;   
	u8  numOfDisTexture;
	u8  nextNumOfDisTexture;
	u8  difOfNumber;
	u8 overflow, overflow_next;
	u8 align;
	u8 unitflag;
	u8 overflowStyle;
	u8 movingDir;  //0 moving down, 1 moving up
//	u8 offsetpoint=0;
	u8 numRadix;    // radix 0-ten radix  1-hex radix
	u8 markingMode; // whether to display 0x for hex   0-display  1-no display
	u8 transformMode; // whether to display upper case  0-low  1-up
	u8 markingPos; // the position of sign in texture
	u8 nextMarkingPos;
	u8 radixBase = 10;
	u8 upperLetterOffset = 10;   // upper case relative to number in ASIC 
	u8 lowerLetterOffset = 4;  // lower case relative to number in ASIC
	u8 enableWaiting; // whether enabling waiting value
	s32 waitingValue; // waiting value
	u8 hitWaitingValue = 0; //if enable waiting value, whether current value hit waiting value
	u8 nextHitWaitingValue = 0;//animaton mode, hit waiting value
	DynamicPageClassPtr curPagePtr;

	curPagePtr = pagePtr;
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
		ERROR_PRINT("ERROR: when drawing textureNumber widght, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}

	numOfDisTexture = 0;
	nextNumOfDisTexture = 0;
	difOfNumber = 0;
	widthOfFont = 0;
	shiftSize = 0;
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
	numofNumber = (p_wptr->WidgetAttr >>5) & 0xf;

	align = (p_wptr->NumOfLine >> 8) & 0xf;
	overflowStyle = (p_wptr->NumOfLine >> 12) & 0xf;

	unitflag = p_wptr->DOTOPTIONS;

	if(numofNumber > 10 || numofNumber < 1){
		ERROR_PRINT("ERROR: don't support such length of number");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
	}

	if(/*pointPosRight < 0 || */ pointPosRight > numofNumber - 1){
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
	//		//value = (s32)maxValue;
	//		overflow = 1;
	//	}
	//	if((s32)value < (s32)minValue)
	//	{
	//		//value = (s32)minValue;
	//		overflow = 1;
	//	}
	//}
	//else
	//{
	//	// none sign mode, if the value is negtive, transform it into positive
	//	if((s32)value > (s32)maxValue)
	//	{
	//		//value = (s32)maxValue;
	//		overflow = 1;
	//	}
	//	if((s32)value < (s32)minValue)
	//	{
	//		//value = (s32)minValue;
	//		overflow = 1;
	//	}
	//	//if((s32)value < 0){
	//	//	value = -value;
	//	//}
	//}

	numRadix =  (u8)(p_wptr->HexControl& 0x000f);
	markingMode = (u8)((p_wptr->HexControl & 0x00f0)>>4);
	transformMode = (u8)((p_wptr->HexControl & 0x0f00)>>8);

	enableWaiting = p_wptr->TextureNumberWaitingValue & 0x1;
	temp = (s16)(p_wptr->TextureNumberWaitingValueH);
	waitingValue = (temp << 15) | (s16)(p_wptr->TextureNumberWaitingValue>>1);
	
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
			curValue = (s32)maxValue;
			overflow = 1;
		}
		if((s32)curValue < (s32)minValue)
		{
			curValue = (s32)minValue;
			overflow = 1;
		}
		if (enableWaiting && waitingValue == value)
		{
			hitWaitingValue = 1;
		}
	}
	else
	{
		if(u8_pageRefresh)
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
					p_wptr->CurValueL = (u16)value;
					p_wptr->CurValueH = (u16)(value >> 16);
				}
			}
			else
			{
				p_wptr->CurValueL = (u16)value;
				p_wptr->CurValueH = (u16)(value >> 16);
			}
		}
		
		curValue = (s32)((p_wptr->CurValueH << 16) + p_wptr->CurValueL);
		if((s32)curValue > (s32)maxValue)
		{
			overflow = 1;
		}
		if((s32)value > (s32)maxValue)
		{
			value = maxValue;
			overflow_next = 1;
		}
		if((s32)curValue < (s32)minValue)
		{
			overflow = 1;
		}
		if((s32)value < (s32)minValue)
		{
			value = minValue;
			overflow_next = 1;
		}
		if (enableWaiting && waitingValue == value)
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
			u32 tempValue = value;
			u8 count = 0;
			while (tempValue>0)
			{
				tempValue /= radixBase;
				count++;
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
			tempValue = curValue;
			count = 0;
			while (tempValue>0)
			{
				tempValue /= radixBase;
				count++;
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
	//no leading zeros
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
		if(pointPosRight != 0) 
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
		if(pointPosRight != 0) 
			if(nextNumOfDisTexture < pointPosRight+1)
				nextNumOfDisTexture = pointPosRight+1;
	}
	else 
		nextNumOfDisTexture = numofNumber;

	//overflowStyle 0 display max, 1 not display
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
		tens *=radixBase;

	pointPosLeft = numofNumber - pointPosRight;

	if(pointPosLeft <1 ) pointPosLeft = 1;

	//count the sign texture
	if(sign)
	{
		numofNumber++;
		pointPosLeft++;
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

	if((p_wptr->NumOfLine & 0xff) == 0 || (p_wptr->NumOfLine & 0xff) == 1)   //no animation
	{
		if(unitflag)
		{
			texturePtr = &(curPagePtr->pBasicTextureList[p_wptr->StartNumofTex + numofNumber]);
			texturePtr->FocusedSlice = UNIT;
		}
		texturePtr = &(curPagePtr->pBasicTextureList[p_wptr->StartNumofTex]);
	}
	else if((p_wptr->NumOfLine & 0xff) == 2) //animation
	{
		if(unitflag)
		{
			texturePtr = &(curPagePtr->pBasicTextureList[p_wptr->StartNumofTex + numofNumber + 2]);
			texturePtr->FocusedSlice = UNIT;

			texturePtr = &(curPagePtr->pBasicTextureList[p_wptr->StartNumofTex + 2*numofNumber + 1 + 2]);
			texturePtr->FocusedSlice = UNIT;

			pureMaskTexturePtr = &(curPagePtr->pBasicTextureList[p_wptr->StartNumofTex+1]); //the first texture is puremask to show only the fix part, zuz 20190109
			texturePtr = &(curPagePtr->pBasicTextureList[p_wptr->StartNumofTex+2]);
			nextTexturePtr = &(curPagePtr->pBasicTextureList[p_wptr->StartNumofTex + numofNumber + 1 + 2]);
		}
		else
		{
			pureMaskTexturePtr = &(curPagePtr->pBasicTextureList[p_wptr->StartNumofTex+1]); //the first texture is puremask to show only the fix part, zuz 20190109
			texturePtr = &(curPagePtr->pBasicTextureList[p_wptr->StartNumofTex+2]);
			nextTexturePtr = &(curPagePtr->pBasicTextureList[p_wptr->StartNumofTex + numofNumber + 2]);
		}
	}
	else 
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);

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
				//no moving
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
				//no moving
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
		return AHMI_FUNC_SUCCESS;
	}

	if(sValue[1] < 0)
	{
		dataMinus = 1;
		sValue[1] = - sValue[1];
		
	}

	// cut the exceeding part
	uValue[1] = uValue[1] % tens;
	sValue[1] = sValue[1] % tens;
	tens /= radixBase;

	if(nextHitWaitingValue)
	{
		for(i=0;i<numofNumber;i++)
		{
			if(i >= (numofNumber - 2))
			{
				texturePtr->FocusedSlice = UNDERLINE;
			}
			else
			{
				texturePtr->FocusedSlice = SPACE;
			}
			texturePtr++;
		}
		numOfDisTexture = 2;
	}
	else
	{
		//operate the texture, @see NumberClass.cpp
		//pre
		for(i=0;i<numofNumber;i++)
		{
			if(sign){
				code = (u16)(sValue[1] / tens);		
				sValue[1] %= tens;
			}else
			{
				code = (u16)(uValue[1] / tens);
				uValue[1] %= tens;
			}

			//show '-'
			if( sign && dataMinus && (leadZero || ( !leadZero && code != 0 ) || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
			{
				if(overflow == 1)
					texturePtr->FocusedSlice = SPACE;
				else
					texturePtr->FocusedSlice = UNDERLINE;
				texturePtr++;

				dataMinus = 0;
				signFlag = 1;
				i++;
			}
			else if( sign && !dataMinus && (leadZero || ( !leadZero && code != 0 ) || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
			{
				if(overflow == 1)
					texturePtr->FocusedSlice = SPACE;
				else
					texturePtr->FocusedSlice = SPACE;
				texturePtr++;

				signFlag = 1;
				i++;
			}
			else if( sign && !dataMinus && (leadZero || ( !leadZero && code != 0 ) || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
			{
				texturePtr->FocusedSlice = SPACE;
				texturePtr++;
				signFlag = 1;
				i++;
			}

			// display '0x'
			if(numRadix==1&&markingMode==1&&i==markingPos){

				if(overflow == 1)
					texturePtr->FocusedSlice = SPACE;
				else{
					texturePtr->FocusedSlice = X;
				}
				tens /= radixBase;
				texturePtr++;
				continue;
			}else if(numRadix==1&&markingMode==1&&i==(markingPos-1)){
				if(overflow == 1)
					texturePtr->FocusedSlice = SPACE;
				else{
					texturePtr->FocusedSlice = ZERO;
				}
				tens /= radixBase;
				texturePtr++;
				continue;
			}

			//show '.'
			if(i == pointPosLeft)
			{
				if(overflow == 1)
					texturePtr->FocusedSlice = SPACE;
				else{
					texturePtr->FocusedSlice = MPOINT;
				}
				texturePtr++;
				//offsetpoint = i;
				i++;
			}

				

			//show leading zeros
			if (!showingZero && code == 0 && !leadZero && (i != pointPosLeft - 1))
			{
				code = SPACE;
			}
			else if(!showingZero && code == 0 && !leadZero && i == pointPosLeft - 1)
			{
				code= ZERO;
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
				

				showingZero = 1;
			}


			tens =tens /radixBase;
			if(overflow == 1)
				texturePtr->FocusedSlice = SPACE;    
			else{
				if (nextHitWaitingValue)
				{
					texturePtr->FocusedSlice = UNDERLINE;
				}else {
					texturePtr->FocusedSlice = code;
				}
				
			}
				
			texturePtr++;
		}
	}

	//next
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
			numofNumber++; 
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

		uValue[0] = uValue[0] % tens;
		sValue[0] = sValue[0] % tens;
		tens /= radixBase;

		if(hitWaitingValue)
		{
			for(i=0;i<numofNumber;i++)
			{
				if(i >= (numofNumber - 2))
				{
					nextTexturePtr->FocusedSlice = UNDERLINE;
				}
				else
				{
					nextTexturePtr->FocusedSlice = SPACE;
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

				//show '-'
				if( sign && dataMinus && (leadZero || ( !leadZero && code != 0 ) || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
				{
					if(overflow_next == 1)
						nextTexturePtr->FocusedSlice = SPACE;
					else
						nextTexturePtr->FocusedSlice = UNDERLINE;
					nextTexturePtr++;

					dataMinus = 0;
					signFlag = 1;
					i++;
				}
				//not show for positive number 
				else if( sign && !dataMinus && (leadZero || ( !leadZero && code != 0 ) || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
				{
					if(overflow_next == 1)
						nextTexturePtr->FocusedSlice = SPACE;
					else
						nextTexturePtr->FocusedSlice = SPACE;
					nextTexturePtr++;

					signFlag = 1;
					i++;
				}
				else if( sign && !dataMinus && (leadZero || ( !leadZero && code != 0 ) || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
				{
					nextTexturePtr->FocusedSlice = SPACE;
					nextTexturePtr++;
					signFlag = 1;
					i++;
				}

				// display '0x'
				if(numRadix==1&&markingMode==1&&i==nextMarkingPos){

					if(overflow_next == 1)
						nextTexturePtr->FocusedSlice = SPACE;
					else
						nextTexturePtr->FocusedSlice = SPACE;
					tens /= radixBase;
					nextTexturePtr++;
					continue;
				}else if(numRadix==1&&markingMode==1&&i==(nextMarkingPos-1)){
					if(overflow_next == 1)
						nextTexturePtr->FocusedSlice = SPACE;
					else
						nextTexturePtr->FocusedSlice = ZERO;
					tens /= radixBase;
					nextTexturePtr++;
					continue;
				}

				if(i == pointPosLeft)
				{
					if(overflow_next == 1)
						nextTexturePtr->FocusedSlice = SPACE;
					else
						nextTexturePtr->FocusedSlice = MPOINT;
					nextTexturePtr++;
					//offsetpoint = i;
					i++;
				}

				//show leading zeros
				if (!showingZero && code == 0 && !leadZero && (i != pointPosLeft - 1))
				{
					code= SPACE;
				}
				else if(!showingZero && code == 0 && !leadZero && i == pointPosLeft - 1)
				{
					code= ZERO;
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
					
					showingZero = 1;
				}
				tens =tens /radixBase;
				if(overflow_next == 1)
					nextTexturePtr->FocusedSlice = SPACE;
				else
					if (hitWaitingValue)
					{
						nextTexturePtr->FocusedSlice = UNDERLINE;
					}else{
						nextTexturePtr->FocusedSlice = code;
					}
					
				nextTexturePtr++;
			}
		}
	}

	texturePtr = texturePtr - numofNumber;
	if((p_wptr->NumOfLine & 0xff) == 2) //with animation
		nextTexturePtr = nextTexturePtr - numofNumber;

	for(i=0;i<numofNumber;i++)
	{
		texturePtr[i].mTexAttr &= ~(FORCE_UNDISPLAY);
		if((p_wptr->NumOfLine & 0xff) == 2) //with animation
			nextTexturePtr[i].mTexAttr &= ~(FORCE_UNDISPLAY);
	}

	if(unitflag) //unit display
	{
		//align mode, @see NumberClass.cpp
		if(align == RIGHTALIGN)   
		{
			difOfNumber = 0;
			if(numofNumber == 1) //only one texture
				widthOfFont = 0;
			else
				widthOfFont = texturePtr->TexWidth << 4;
			shiftSize = 0;
		}
		else if(align == CENTERALIGN)  
		{
			difOfNumber = numofNumber - numOfDisTexture;
			if(numofNumber == 1) //only one texture
				widthOfFont = 0;
			else
				widthOfFont = texturePtr->TexWidth << 4;
			shiftSize = widthOfFont * difOfNumber;
			shiftSize  = (shiftSize >> 1);			
			if( (shiftSize & 0x08) != 0)          
				shiftSize += 0x08;
		}
		else if(align == LEFTALIGN) 
		{
			difOfNumber = numofNumber - numOfDisTexture;
			if(numofNumber == 1) //only one texture
				widthOfFont = 0;
			else
				widthOfFont = texturePtr->TexWidth << 4;
			shiftSize = widthOfFont * difOfNumber;
		}
		//operation on position, @see NumberClass.cpp
		for(i=0;i<numofNumber + 1;i++)
		{
			
			texturePtr[i].OffsetX = ((s16)p_wptr->WidgetOffsetX) << 4;
			if((i <= pointPosLeft) || (nextHitWaitingValue == 1))
				texturePtr[i].OffsetX += widthOfFont * i;
			else{
				texturePtr[i].OffsetX += widthOfFont * i - (widthOfFont)/2;
			}
			texturePtr[i].OffsetX -= shiftSize;
			if(texturePtr[i].FocusedSlice == SPACE)
			{
				texturePtr[i].mTexAttr |= (FORCE_UNDISPLAY);
			}
			else 
			{
				texturePtr[i].TexLeftTileBox = texturePtr[i].OffsetX >> 9;
				texturePtr[i].TexRightTileBox = (texturePtr[i].OffsetX + texturePtr[i].TexWidth * 16) >> 9;
			}
		}

		//next
		if((p_wptr->NumOfLine & 0xff) == 2) //with animation
		{
			if(align == RIGHTALIGN)  
			{
				difOfNumber = 0;
				shiftSize = widthOfFont * difOfNumber;
				shiftSize  = shiftSize;  
			}
			else if(align == CENTERALIGN)  
			{
				difOfNumber = numofNumber - nextNumOfDisTexture;
				shiftSize = widthOfFont * difOfNumber;
				shiftSize  = (shiftSize >> 1);		
				if( (shiftSize & 0x08) != 0)          
				shiftSize += 0x08;

			}
			else if(align ==LEFTALIGN) //左对齐
			{
				difOfNumber = numofNumber - nextNumOfDisTexture;
				shiftSize = widthOfFont * difOfNumber;

			}
			for(i=0;i<numofNumber + 1;i++)
			{
				//pureMaskTexturePtr->OffsetX = ((s16)p_wptr->WidgetOffsetX) << 4;
				//nextTexturePtr[i].OffsetX = ((s16)p_wptr->WidgetOffsetX) << 4;
				pureMaskTexturePtr->OffsetX = (0) << 4;
				nextTexturePtr[i].OffsetX = (0) << 4;
				if((i <= pointPosLeft) || (hitWaitingValue == 1))
					nextTexturePtr[i].OffsetX += widthOfFont * i;
				else  //the right number of '.' needs only to move half the width of font
					nextTexturePtr[i].OffsetX += widthOfFont * i - (widthOfFont + 1)/2;
				nextTexturePtr[i].OffsetX -= shiftSize;

				//change the pureamsk's offset, zuz 20190109
				//pureMaskTexturePtr->OffsetX -= shiftSize;

				if(nextTexturePtr[i].FocusedSlice == SPACE)
				{
					nextTexturePtr[i].mTexAttr |= (FORCE_UNDISPLAY);
				}
				else 
				{
					nextTexturePtr[i].TexLeftTileBox = nextTexturePtr[i].OffsetX >> 9;
					nextTexturePtr[i].TexRightTileBox = (nextTexturePtr[i].OffsetX + nextTexturePtr[i].TexWidth * 16) >> 9;
				}
			}
		}
	}
	else //no unit display
	{
		//align mode, @see NumberClass.cpp
		if(align == RIGHTALIGN)   
		{
			difOfNumber = 0;
			if(numofNumber == 1) //only one texture
				widthOfFont = 0;
			else
				widthOfFont = texturePtr->TexWidth << 4;
			shiftSize = 0;
		}
		else if(align == CENTERALIGN)  
		{
			difOfNumber = numofNumber - numOfDisTexture;
			if(numofNumber == 1) //only one texture
				widthOfFont = 0;
			else
				widthOfFont = texturePtr->TexWidth << 4;
			shiftSize = widthOfFont * difOfNumber;
			shiftSize  = (shiftSize >> 1);			
			if( (shiftSize & 0x08) != 0)          
				shiftSize += 0x08;
		}
		else if(align == LEFTALIGN) 
		{
			difOfNumber = numofNumber - numOfDisTexture;
			if(numofNumber == 1) //only one texture
				widthOfFont = 0;
			else
				widthOfFont = texturePtr->TexWidth << 4;
			shiftSize = widthOfFont * difOfNumber;
		}
		//operation on position, @see NumberClass.cpp
		for(i=0;i<numofNumber;i++)
		{
			
			//texturePtr[i].OffsetX = ((s16)p_wptr->WidgetOffsetX) << 4;
			texturePtr[i].OffsetX = (0) << 4;
			if((i <= pointPosLeft) || (nextHitWaitingValue == 1))
				texturePtr[i].OffsetX += widthOfFont * i;
			else{
				texturePtr[i].OffsetX += widthOfFont * i - (widthOfFont)/2;
			}
			texturePtr[i].OffsetX -= shiftSize;
			if(texturePtr[i].FocusedSlice == SPACE)
			{
				texturePtr[i].mTexAttr |= (FORCE_UNDISPLAY);
			}
			else 
			{
				texturePtr[i].TexLeftTileBox = texturePtr[i].OffsetX >> 9;
				texturePtr[i].TexRightTileBox = (texturePtr[i].OffsetX + texturePtr[i].TexWidth * 16) >> 9;
			}
		}

		//next
		if((p_wptr->NumOfLine & 0xff) == 2) //with animation
		{
			if(align == RIGHTALIGN)  
			{
				difOfNumber = 0;
				shiftSize = widthOfFont * difOfNumber;
				shiftSize  = shiftSize;  
			}
			else if(align == CENTERALIGN)  
			{
				difOfNumber = numofNumber - nextNumOfDisTexture;
				shiftSize = widthOfFont * difOfNumber;
				shiftSize  = (shiftSize >> 1);		
				if( (shiftSize & 0x08) != 0)          
				shiftSize += 0x08;

			}
			else if(align ==LEFTALIGN) //左对齐
			{
				difOfNumber = numofNumber - nextNumOfDisTexture;
				shiftSize = widthOfFont * difOfNumber;

			}
			for(i=0;i<numofNumber;i++)
			{
				//pureMaskTexturePtr->OffsetX = ((s16)p_wptr->WidgetOffsetX) << 4;
				//nextTexturePtr[i].OffsetX = ((s16)p_wptr->WidgetOffsetX) << 4;
				pureMaskTexturePtr->OffsetX = (0) << 4;
				nextTexturePtr[i].OffsetX = (0) << 4;
				if((i <= pointPosLeft) || (hitWaitingValue == 1))
					nextTexturePtr[i].OffsetX += widthOfFont * i;
				else  //the right number of '.' needs only to move half the width of font
					nextTexturePtr[i].OffsetX += widthOfFont * i - (widthOfFont + 1)/2;
				nextTexturePtr[i].OffsetX -= shiftSize;

				//change the pureamsk's offset, zuz 20190109
				//pureMaskTexturePtr->OffsetX -= shiftSize;

				if(nextTexturePtr[i].FocusedSlice == SPACE)
				{
					nextTexturePtr[i].mTexAttr |= (FORCE_UNDISPLAY);
				}
				else 
				{
					nextTexturePtr[i].TexLeftTileBox = nextTexturePtr[i].OffsetX >> 9;
					nextTexturePtr[i].TexRightTileBox = (nextTexturePtr[i].OffsetX + nextTexturePtr[i].TexWidth * 16) >> 9;
				}
			}
		}
	}

	return AHMI_FUNC_SUCCESS;

}
#endif

//**************************************
//reponse for Tag.setValue
//widgetCtrl
//15-11: reserved
//11:  whether the button is color button
//10-7: only used in complex button, the number of button. 1-15
//6-5: The mode of button:0 is single button,1 is switch button,2 is complex button
//4-0: type of widget, button is BUTTON_TEX
//**************************************
funcStatus TextureNumberClass::SetValueCtrl(
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
	u16 oldValueinit=0;
	u8  sign;
	u8  numofNumber;
	u8  overflowStyle = 0;
	u8  overflag = 0;
	u8  animationflag = 0;
	u8  unitflag;
	u8  pictureTexOffset = 0;

	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}


	value = ActionPtr->mTagPtr->getValue();

	sign = (p_wptr->WidgetAttr & 0x4000 )?1:0;
	numofNumber = (p_wptr->WidgetAttr >>5) & 0xf;
	oldValueinit = (p_wptr->WidgetAttr & 0x8000);
	overflowStyle = (p_wptr->NumOfLine >> 12) & 0xf;

	unitflag = p_wptr->DOTOPTIONS;

    //the num of number only supports 1-10
	if(numofNumber > 10 || numofNumber < 1){
		ERROR_PRINT("ERROR: don't support such length of number");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_SetValueErr);
	}
	
	maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);
	lowAlarmValue  = (s32)((p_wptr->LowAlarmValueH  << 16) + p_wptr->LowAlarmValueL );
	highAlarmValue = (s32)((p_wptr->HighAlarmValueH << 16) + p_wptr->HighAlarmValueL);


	if(sign)
	{
		if((s32)value > (s32)maxValue)
		{
			ERROR_PRINT("ERROR: TextureNumber widget bindTag value exceeds the boundary");
			u16 CanvasID, SubcanvasID, WidgetID;
			p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
			EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
			//value = (s32)maxValue;
			overflag = 1;
		}
		if((s32)value < (s32)minValue)
		{
			ERROR_PRINT("ERROR: TextureNumber widget bindTag value exceeds the boundary");
			u16 CanvasID, SubcanvasID, WidgetID;
			p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
			EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
			//value = (s32)minValue;
			overflag = 2;
		}
	}
	else
	{
		if((s32)value > (s32)maxValue)
		{
			ERROR_PRINT("ERROR: TextureNumber widget bindTag value exceeds the boundary");
			u16 CanvasID, SubcanvasID, WidgetID;
			p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
			EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
			//value = (u32)maxValue;
			overflag = 1;
		}
		if((s32)value < (s32)minValue)
		{
			ERROR_PRINT("ERROR: TextureNumber widget bindTag value exceeds the boundary");
			u16 CanvasID, SubcanvasID, WidgetID;
			p_wptr->getWidgetIndex(pagePtr, &CanvasID, &SubcanvasID, &WidgetID);
			EM_vLog(EM_enErrWidgetTagValueOverflow,EM_enErrLevelInfo,p_wptr->ATTATCH_PAGE + 1, CanvasID, SubcanvasID, WidgetID, ActionPtr->mTagPtr->mRegID, value);
			//value = (u32)minValue;
			overflag = 2;
		}
	}
    
	//modify currnet value
	//if(animationDuration) //page animation
	//{
	//	p_wptr->CurValueL = (u16)value;
	//	p_wptr->CurValueH = (u16)(value >> 16);
	//}

	if((p_wptr->NumOfLine & 0xff) == 2)
	{
		pictureTexOffset = 2;
	}

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
				pTexturePtr = &(gPagePtr[p_wptr->ATTATCH_PAGE].pBasicTextureList[p_wptr->StartNumofTex + pictureTexOffset]);
				pTexturePtr->OffsetY = 0;
				if(unitflag)
				{
					pTexturePtr = &(gPagePtr[p_wptr->ATTATCH_PAGE].pBasicTextureList[p_wptr->StartNumofTex + pictureTexOffset + 1]);
					pTexturePtr->OffsetY = 0;
				}
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
			//tagchange by Mr.z
			if(p_wptr->TagChangeAction && last_value != value && (p_wptr->TagChangeAction <= pCurrentPage->mActionInstructionsSize))
            {
				actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->TagChangeAction);
				csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
			}
			//if(sign)
			{
				if(p_wptr->EnterLowAlarmAction && (s32)last_value > (s32)lowAlarmValue && (s32)value <= (s32)lowAlarmValue)
				{
					if(p_wptr->EnterLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)( pCurrentPage->pActionInstructions + p_wptr->EnterLowAlarmAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}		
				}
				else if(p_wptr->LeaveLowAlarmAction && (s32)last_value <= (s32)lowAlarmValue && (s32)value > (s32)lowAlarmValue)
				{
					if(p_wptr->LeaveLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->LeaveLowAlarmAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}		
				}
				if(p_wptr->EnterHighAlarmAction && (s32)last_value < (s32)highAlarmValue && (s32)value >= (s32)highAlarmValue)
				{
					if(p_wptr->EnterHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterHighAlarmAction);
						csi_kernel_msgq_put(ActionInstructionQueue,&actionAddr,0,portMAX_DELAY);
					}
				}
				else if(p_wptr->LeaveHighAlarmAction && (s32)last_value >= (s32)highAlarmValue && (s32)value < (s32)highAlarmValue)
				{
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
//					if(p_wptr->EnterLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
//					{
//						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterLowAlarmAction);
//						xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
//					}		
//				}
//				else if(p_wptr->LeaveLowAlarmAction && last_value <= (u32)lowAlarmValue && value > (u32)lowAlarmValue)
//				{
//					if(p_wptr->LeaveLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
//					{
//						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->LeaveLowAlarmAction);
//						xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
//					}		
//				}
//				if(p_wptr->EnterHighAlarmAction && last_value < (u32)highAlarmValue && value >= (u32)highAlarmValue)
//				{
//					if(p_wptr->EnterHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
//					{
//						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterHighAlarmAction);
//						xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);		
//					}
//				}
//				else if(p_wptr->LeaveHighAlarmAction && last_value >= (u32)highAlarmValue && value < (u32)highAlarmValue)
//				{
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
//            //tagchange的支持
            //if((p_wptr->TagChangeAction != 0) && (p_wptr->TagChangeAction <= pCurrentPage->mActionInstructionsSize))
            //{
            //    actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->TagChangeAction);
            //    xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
            //}
            
            /*if(sign)
			{
				if((p_wptr->EnterLowAlarmAction != 0) && (s32)value < (s32)lowAlarmValue)
				{
					if(p_wptr->EnterLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterLowAlarmAction);
						xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
					}		
				}
				if((p_wptr->EnterHighAlarmAction != 0) && (s32)value > (s32)highAlarmValue)
				{
					if(p_wptr->EnterHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterHighAlarmAction);
						xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);		
					}
				}
			}
			else
			{
				if((p_wptr->EnterLowAlarmAction != 0) && value < (u32)lowAlarmValue)
				{
					if(p_wptr->EnterLowAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterLowAlarmAction);
						xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);
					}		
				}
				if((p_wptr->EnterHighAlarmAction != 0) && value > (u32)highAlarmValue)
				{
					if(p_wptr->EnterHighAlarmAction <= pCurrentPage->mActionInstructionsSize)
					{
						actionAddr = (u32)(pCurrentPage->pActionInstructions + p_wptr->EnterHighAlarmAction);
						xQueueSendToBack(ActionInstructionQueue,&actionAddr,portMAX_DELAY);		
					}
				}
			}*/
        }
		//p_wptr->WidgetAttr |= 0x8000;
		//p_wptr->OldValueL = (u16)value;
		//p_wptr->OldValueH = (u16)(value >> 16);

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
funcStatus TextureNumberClass::MouseTouchCtrl(
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
funcStatus TextureNumberClass::KeyboardTouchCtrl(
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
// name： MeterClass::setATag
// set the texture's animation tag
//  
//  @param   WidgetClassPtr p_wptr,   //pointer of the widget
//  @param	 u32 tagValue             //the value of the animation tag
// addtional comments :
//   none
//-----------------------------
funcStatus TextureNumberClass::setATag(
	WidgetClassPtr p_wptr,   
	u16 value
	)
{
	
	s16 preTextureOffsetY; //1.11.4
//	s16 nextTextureOffsetY; //1.11.4
	u16 transitionParamIn; //ratio of animation duration 0.10
	u16 transitionParamOut; // ratio after varying function 0.10
	u16 widgetHeight;
	TextureClassPtr preTexturePtr;
	TagClassPtr  bindTag;
	u32 bindTagValue;
//	u8 numofNumber;
//	u8 pointPosRight;
//	u8 sign;
	u8 unitflag;
//	u8 i;
	u8 pictureTexOffset = 2;
//	s32 maxValue;
//	s32 minValue;
	
	u16 totalDuration = p_wptr->ANIMATION_DURATION;
//	numofNumber = (p_wptr->WidgetAttr >>5) & 0xf;
//	pointPosRight = (p_wptr->WidgetAttr >>9) & 0xf;
//	sign = (p_wptr->WidgetAttr & 0x4000 )?1:0;
	unitflag = p_wptr->DOTOPTIONS;
	widgetHeight = p_wptr->WidgetHeight;
	bindTag = &TagPtr->getTag(p_wptr->BindTagID);
	bindTagValue = (u32)(bindTag->getValue());

	if(totalDuration == 0 || ( (p_wptr->NumOfLine & 0xff) == 1) ) //no animation
		return AHMI_FUNC_SUCCESS;

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
	if(p_wptr -> PreviousTexturePtrFlag == 0) //down
	{
		preTextureOffsetY = ((widgetHeight) * transitionParamOut) >> 6;   //0->h
	}
	else //up
	{
		preTextureOffsetY =  - ( ((widgetHeight) * transitionParamOut) >> 6); //0->-h
	}

	//modify the position
	preTexturePtr = &(gPagePtr[p_wptr->ATTATCH_PAGE].pBasicTextureList[p_wptr->StartNumofTex + pictureTexOffset]);
	preTexturePtr->OffsetY = preTextureOffsetY;

	if(unitflag)
	{
		preTexturePtr = &(gPagePtr[p_wptr->ATTATCH_PAGE].pBasicTextureList[p_wptr->StartNumofTex + pictureTexOffset + 1]);
		preTexturePtr->OffsetY = preTextureOffsetY;
	}

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

		//maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
		//minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);
		//if(sign)
		//{
		//	if((s32)bindTagValue > (s32)maxValue)
		//	{
		//		bindTagValue = (s32)maxValue;
		//	}
		//	if((s32)bindTagValue < (s32)minValue)
		//	{
		//		bindTagValue = (s32)minValue;
		//	}
		//}
		//else
		//{
		//	if((u32)bindTagValue > (u32)maxValue)
		//	{
		//		bindTagValue = (u32)maxValue;
		//	}
		//	if((u32)bindTagValue < (u32)minValue)
		//	{
		//		bindTagValue = (u32)minValue;
		//	}
		//}
		//p_wptr->CurValueL = (u16)bindTagValue;
		//p_wptr->CurValueH = (u16)(bindTagValue >> 16);
	}

	p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
//	taskEXIT_CRITICAL();

	return AHMI_FUNC_SUCCESS;
}

//***********************************
// number to texture for display
//**********************************
funcStatus TextureNumberClass::updateNumberDisplay(
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
    TextureClassPtr unitTexturePtr, texturePtr;
    u64 tens;
	u32 value;
	u32 uValue[5] = {0};
	s32 sValue[5] = {0};
	s32	maxValue;
	s32	minValue;
	s32 curValue;
    s32 j;
	s16 temp;
	s16 xCoordinate;
	s16 TextureOffsetY; //1.11.4
//    u16 widthOfFont;
	u16 showNumWidth;
	u16 TexWidth;
	u16 UnitTexWidth;
//	u16 shiftSize;
	u16 code;
	u8  leadZero;
	u8  sign;
	u8  pointPosRight;
	u8	pointPosLeft;
	u8  numofNumber;
	u8	i;
	u8	showingZero; 
	u8	signFlag;                     
	u8	dataMinus;   
	u8  numOfDisTexture;
	u8  nextNumOfDisTexture;
//	u8  difOfNumber;
	u8 overflow, overflow_next;
	u8 align;
	u8 unitflag;
	u8 overflowStyle;
	u8 movingDir;  //0 moving down, 1 moving up
	u8 numRadix;    // radix 0-ten radix  1-hex radix
	u8 markingMode; // whether to display 0x for hex   0-display  1-no display
	u8 transformMode; // whether to display upper case  0-low  1-up
	u8 markingPos; // the position of sign in texture
	u8 nextMarkingPos;
	u8 radixBase = 10;
	u8 upperLetterOffset = 10;   // upper case relative to number in ASIC 
	u8 lowerLetterOffset = 4;  // lower case relative to number in ASIC
	u8 enableWaiting; // whether enabling waiting value
	s32 waitingValue; // waiting value
	u8 hitWaitingValue = 0; //if enable waiting value, whether current value hit waiting value
	u8 nextHitWaitingValue = 0;//animaton mode, hit waiting value
	u8 pictureTexOffset = 0;
	u8 movedirection = (p_wptr->TimingFun >> 8);//0 for normal, 1 for up, 2 for down

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
	numofNumber = (p_wptr->WidgetAttr >>5) & 0xf;

	align = (p_wptr->NumOfLine >> 8) & 0xf;
	overflowStyle = (p_wptr->NumOfLine >> 12) & 0xf;

	unitflag = p_wptr->DOTOPTIONS;
	if((p_wptr->NumOfLine & 0xff) == 2)
	{
		pictureTexOffset = 2;
	}

	if(numofNumber > 10 || numofNumber < 1){
		ERROR_PRINT("ERROR: don't support such length of number");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetCtrlErr);
	}

	if(/* pointPosRight < 0 || */ pointPosRight > numofNumber - 1){
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

	enableWaiting = p_wptr->TextureNumberWaitingValue & 0x1;
	temp = (s16)(p_wptr->TextureNumberWaitingValueH);
	waitingValue = (temp << 15) | (s16)(p_wptr->TextureNumberWaitingValue>>1);

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

	if((p_wptr->NumOfLine & 0xff) == 2)
	{
		//draw puremask texture
		texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex]);
		status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
		if(status == AHMI_FUNC_MATRIX_ILLEGAL)
		{
			return status;
		}

		//draw puremask texture
		texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + 1]);
		status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
		if(status == AHMI_FUNC_MATRIX_ILLEGAL)
		{
			return status;
		}
	}

	//get first picture texture
	texturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pictureTexOffset]);
	TexWidth = texturePtr->TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512;
	if(unitflag)
	{
		unitTexturePtr = &(pagePtr->pBasicTextureList[p_wptr->StartNumofTex + pictureTexOffset + 1]);
		UnitTexWidth = unitTexturePtr->TexWidth * p_wptr->ADAPTIVE_SCALE_LEVEL / 512;
	}

	#ifdef EMBEDDED
	if((texturePtr < (void*)startOfDynamicPage) || (texturePtr > (void*)endOfDynamicPage))
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_TexturePtrOverrun);
	#endif

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
		return AHMI_FUNC_SUCCESS;
	}

	if((overflowStyle == OVERFLOW_NON_DISPLAY) && (overflow))
	{
		numOfDisTexture = 0;
	}

	if(nextHitWaitingValue)
	{
		numOfDisTexture = 2;
	}

	//judge the assign mode
	if((pointPosRight != 0) && (!nextHitWaitingValue) && ((overflowStyle == OVERFLOW_DISPLAY) || (!overflow)))
	{
		if(unitflag)// if unit display need add num of unit string texture
		{
			showNumWidth = (TexWidth) * (numOfDisTexture - 1) + TexWidth/2 + UnitTexWidth;
		}
		else
		{
			showNumWidth = (TexWidth) * (numOfDisTexture - 1) + TexWidth/2;
		}
	}else
	{
		if(unitflag)// if unit display need add num of unit string texture
		{
			showNumWidth = (TexWidth) * (numOfDisTexture) + UnitTexWidth;
		}
		else
		{
			showNumWidth = (TexWidth) * (numOfDisTexture);
		}
	}
	
	if(align == RIGHTALIGN)   //default right align
	{
		xCoordinate = (showNumWidth > (s16)p_wptr->WidgetWidth) ? 0 : ((s16)p_wptr->WidgetWidth - showNumWidth);
	}
	else if(align == CENTERALIGN)  //middle align
	{
		xCoordinate = (showNumWidth > (s16)p_wptr->WidgetWidth) ? 0 : ((s16)p_wptr->WidgetWidth - showNumWidth)/2;
	}
	else if(align == LEFTALIGN) //left align mode
	{
		xCoordinate = 0;
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
				texturePtr->FocusedSlice = UNDERLINE;
				texturePtr->OffsetX = (xCoordinate) << 4;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}
				xCoordinate += TexWidth;
			}
			else
			{
				texturePtr->FocusedSlice = SPACE;
			}
		}
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
					texturePtr->FocusedSlice = SPACE;
				}
				else
				{
					texturePtr->FocusedSlice = MINUSSIGN;
				}
				if(texturePtr->FocusedSlice == SPACE)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate) << 4;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(nextHitWaitingValue != 1)
				{
					if(pointPosRight != 0)
					{
						if (i == pointPosLeft)
						{
							xCoordinate += TexWidth/2;
						}
						else
						{
							xCoordinate += TexWidth;
						}
					}
					else
					{
						xCoordinate += TexWidth;
					}
				}
				else
				{
					xCoordinate += TexWidth;
				}

				dataMinus = 0;
				signFlag = 1;
				i++;
			}
			//display ' ' before the positive number
			else if( sign && !dataMinus && (leadZero || ( !leadZero && code != 0 ) || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
			//       sign      postive      display '0'    no display '0' and not 0     no display '0' and arriving 2 number before point                 didnot display sign yet
			{
				if(overflow == 1)
				{
					texturePtr->FocusedSlice = SPACE;
					if(texturePtr->FocusedSlice == SPACE)
					{
						continue;
					}
					texturePtr->OffsetX = (xCoordinate) << 4;
					status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
					if(status == AHMI_FUNC_MATRIX_ILLEGAL)
					{
						return status;
					}
					if(nextHitWaitingValue != 1)
					{
						if(pointPosRight != 0)
						{
							if (i == pointPosLeft)
							{
								xCoordinate += TexWidth/2;
							}
							else
							{
								xCoordinate += TexWidth;
							}
						}
						else
						{
							xCoordinate += TexWidth;
						}
					}
					else
					{
						xCoordinate += TexWidth;
					}
				}
				else
				{
					texturePtr->FocusedSlice = SPACE;
				}
				//texturePtr++;

				signFlag = 1;//already display sign
				i++;
			}
			//positve number display ' '
			else if( sign && !dataMinus && (leadZero || ( !leadZero && code != 0 ) ||     (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
				//  sign      positive      display '0'    not display '0' and not zero               not display zero and arriving the left 2 to point
			{
				texturePtr->FocusedSlice = SPACE;
				if(texturePtr->FocusedSlice == SPACE)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate) << 4;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(nextHitWaitingValue != 1)
				{
					if(pointPosRight != 0)
					{
						if (i == pointPosLeft)
						{
							xCoordinate += TexWidth/2;
						}
						else
						{
							xCoordinate += TexWidth;
						}
					}
					else
					{
						xCoordinate += TexWidth;
					}
				}
				else
				{
					xCoordinate += TexWidth;
				}
				//texturePtr++;
				signFlag = 1;
				i++;
			}
		
			// display '0x'
			if(numRadix==1&&markingMode==1&&i==markingPos){

				if (overflow == 1)
				{
					texturePtr->FocusedSlice = SPACE;
				}else
				{
					texturePtr->FocusedSlice = X;
				}
				if(texturePtr->FocusedSlice == SPACE)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate) << 4;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(nextHitWaitingValue != 1)
				{
					if(pointPosRight != 0)
					{
						//dot 0.5 char
						if (i == pointPosLeft)
						{
							xCoordinate += TexWidth/2;
						}
						else
						{
							xCoordinate += TexWidth;
						}
					}
					else
					{
						xCoordinate += TexWidth;
					}
				}
				else
				{
					xCoordinate += TexWidth;
				}
				tens /= radixBase;
				//texturePtr++;
				continue;
			}else if(numRadix==1&&markingMode==1&&i==(markingPos-1)){
				if (overflow == 1)
				{
					texturePtr->FocusedSlice = SPACE;
				}else
				{
					texturePtr->FocusedSlice = ZERO;
				}
				tens /= radixBase;
				if(texturePtr->FocusedSlice == SPACE)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate) << 4;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(nextHitWaitingValue != 1)
				{
					if(pointPosRight != 0)
					{
						//dot 0.5 char
						if (i == pointPosLeft)
						{
							xCoordinate += TexWidth/2;
						}
						else
						{
							xCoordinate += TexWidth;
						}
					}
					else
					{
						xCoordinate += TexWidth;
					}
				}
				else
				{
					xCoordinate += TexWidth;
				}
				//texturePtr++;
				continue;
			}

			//display point
			if(i == pointPosLeft)
			{
				if(overflow == 1)
					texturePtr->FocusedSlice = SPACE;
				else
					texturePtr->FocusedSlice = MPOINT;//???С????

				if(texturePtr->FocusedSlice == SPACE)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate) << 4;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(nextHitWaitingValue != 1)
				{
					if(pointPosRight != 0)
					{
						//dot 0.5 char
						if (i == pointPosLeft)
						{
							xCoordinate += TexWidth/2;
						}
						else
						{
							xCoordinate += TexWidth;
						}
					}
					else
					{
						xCoordinate += TexWidth;
					}
				}
				else
				{
					xCoordinate += TexWidth;
				}
				//texturePtr++;
//				offsetpoint = i;
				i++;
			}

				

			//display leading zeros
			if (!showingZero          && code == 0     && !leadZero        && (i != pointPosLeft - 1))
			//   not display rest '0'    zero       not display leading '0'     didn't arrive the point
			{
				code=SPACE;//show ' '
			}
			//display the '0' before point
			else if(!showingZero && code == 0 && !leadZero && i == pointPosLeft - 1)
			{
				code= ZERO;//display '0'
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


				showingZero = 1;
			}
			// tens =tens /10;
			tens /= radixBase;
			if(overflow == 1)
			{
				texturePtr->FocusedSlice = SPACE;
				if(texturePtr->FocusedSlice == SPACE)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate) << 4;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(nextHitWaitingValue != 1)
				{
					if(pointPosRight != 0)
					{
						//dot 0.5 char
						if (i == pointPosLeft)
						{
							xCoordinate += TexWidth/2;
						}
						else
						{
							xCoordinate += TexWidth;
						}
					}
					else
					{
						xCoordinate += TexWidth;
					}
				}
				else
				{
					xCoordinate += TexWidth;
				}
			}
			else
			{
				if (nextHitWaitingValue)
				{
					texturePtr->FocusedSlice = UNDERLINE; 
				}else{
					texturePtr->FocusedSlice = code;
				}
				if(texturePtr->FocusedSlice == SPACE)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate) << 4;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(nextHitWaitingValue != 1)
				{
					if(pointPosRight != 0)
					{
						//dot 0.5 char
						if (i == pointPosLeft)
						{
							xCoordinate += TexWidth/2;
						}
						else
						{
							xCoordinate += TexWidth;
						}
					}
					else
					{
						xCoordinate += TexWidth;
					}
				}
				else
				{
					xCoordinate += TexWidth;
				}
			}
			//texturePtr++;
		}
	}

	//set unit
	if(unitflag)
	{
		unitTexturePtr->OffsetX = (xCoordinate) << 4;
		status = unitTexturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,unitTexturePtr,p_wptr);
		if(status == AHMI_FUNC_MATRIX_ILLEGAL)
		{
			return status;
		}

		if(nextHitWaitingValue != 1)
		{
			if(pointPosRight != 0)
			{
				//dot 0.5 char
				if (i == pointPosLeft)
				{
					xCoordinate += UnitTexWidth/2;
				}
				else
				{
					xCoordinate += UnitTexWidth;
				}
			}
			else
			{
				xCoordinate += UnitTexWidth;
			}
		}
		else
		{
			xCoordinate += UnitTexWidth;
		}
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
				if(unitflag)
				{
					unitTexturePtr->OffsetY = TextureOffsetY;
				}
			}
			else
			{
				texturePtr->OffsetY = 0;
				if(unitflag)
				{
					unitTexturePtr->OffsetY = 0;
				}
			}
		}
		else if(movingDir == 1)//up
		{
			if(texturePtr->OffsetY != (p_wptr->WidgetHeight << 4))
			{
				TextureOffsetY = texturePtr->OffsetY + (p_wptr->WidgetHeight << 4);
				texturePtr->OffsetY = TextureOffsetY;
				if(unitflag)
				{
					unitTexturePtr->OffsetY = TextureOffsetY;
				}
			}
			else
			{
				texturePtr->OffsetY = 0;
				if(unitflag)
				{
					unitTexturePtr->OffsetY = 0;
				}
			}
		}

		if((overflowStyle == OVERFLOW_NON_DISPLAY) && (overflow_next))
		{
			nextNumOfDisTexture = 0;
		}

		if(hitWaitingValue)
		{
			nextNumOfDisTexture = 2;
		}

		//judge the assign mode
		if((pointPosRight != 0) && (!hitWaitingValue) && ((overflowStyle == OVERFLOW_DISPLAY) || (!overflow_next)))
		{
			if(unitflag)// if unit display need add num of unit string texture
			{
				showNumWidth = (TexWidth) * (nextNumOfDisTexture - 1) + TexWidth/2 + UnitTexWidth;
			}
			else
			{
				showNumWidth = (TexWidth) * (nextNumOfDisTexture - 1) + TexWidth/2;
			}
		}else
		{
			if(unitflag)// if unit display need add num of unit string texture
			{
				showNumWidth = (TexWidth) * (nextNumOfDisTexture) + UnitTexWidth;
			}
			else
			{
				showNumWidth = (TexWidth) * (nextNumOfDisTexture);
			}
		}
	
		if(align == RIGHTALIGN)   //default right align
		{
			xCoordinate = (showNumWidth > (s16)p_wptr->WidgetWidth) ? 0 : ((s16)p_wptr->WidgetWidth - showNumWidth);
		}
		else if(align == CENTERALIGN)  //middle align
		{
			xCoordinate = (showNumWidth > (s16)p_wptr->WidgetWidth) ? 0 : ((s16)p_wptr->WidgetWidth - showNumWidth)/2;
		}
		else if(align == LEFTALIGN) //left align mode
		{
			xCoordinate = 0;
		}

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
					texturePtr->FocusedSlice = UNDERLINE;
					texturePtr->OffsetX = (xCoordinate) << 4;
					status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
					if(status == AHMI_FUNC_MATRIX_ILLEGAL)
					{
						return status;
					}
					xCoordinate += TexWidth;
				}
				else
				{
					texturePtr->FocusedSlice = SPACE;
				}
			}
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
						texturePtr->FocusedSlice = SPACE;
					else
						texturePtr->FocusedSlice = MINUSSIGN;
					if(texturePtr->FocusedSlice == SPACE)
					{
						continue;
					}
					texturePtr->OffsetX = (xCoordinate) << 4;
					status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
					if(status == AHMI_FUNC_MATRIX_ILLEGAL)
					{
						return status;
					}

					if(hitWaitingValue != 1)
					{
						if(pointPosRight != 0)
						{
							if (i == pointPosLeft)
							{
								xCoordinate += TexWidth/2;
							}
							else
							{
								xCoordinate += TexWidth;
							}
						}
						else
						{
							xCoordinate += TexWidth;
						}
					}
					else
					{
						xCoordinate += TexWidth;
					}
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
						texturePtr->FocusedSlice = SPACE;
						if(texturePtr->FocusedSlice == SPACE)
						{
							continue;
						}
						texturePtr->OffsetX = (xCoordinate) << 4;
						status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
						if(status == AHMI_FUNC_MATRIX_ILLEGAL)
						{
							return status;
						}

						if(hitWaitingValue != 1)
						{
							if(pointPosRight != 0)
							{
								//dot 0.5 char
								if (i == pointPosLeft)
								{
									xCoordinate += TexWidth/2;
								}
								else
								{
									xCoordinate += TexWidth;
								}
							}
							else
							{
								xCoordinate += TexWidth;
							}
						}
						else
						{
							xCoordinate += TexWidth;
						}
					}
					else
					{
						texturePtr->FocusedSlice = SPACE;
					}
					//nextTexturePtr++;

					signFlag = 1;
					i++;
				}
				//display ' ' for the first char of positive number
				else if( sign && !dataMinus && (leadZero || ( !leadZero && code != 0 ) || (!leadZero && /*PointPosRight != 0 &&*/ i == pointPosLeft - 2) ) && !signFlag)
				//       sign      postive      display '0'    no display '0' and not 0     no display '0' and arriving 2 number before point                 didnot display sign yet
				{
					texturePtr->FocusedSlice = SPACE;
					if(texturePtr->FocusedSlice == SPACE)
					{
						continue;
					}
					texturePtr->OffsetX = (xCoordinate) << 4;
					status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
					if(status == AHMI_FUNC_MATRIX_ILLEGAL)
					{
						return status;
					}

					if(hitWaitingValue != 1)
					{
						if(pointPosRight != 0)
						{
							//dot 0.5 char
							if (i == pointPosLeft)
							{
								xCoordinate += TexWidth/2;
							}
							else
							{
								xCoordinate += TexWidth;
							}
						}
						else
						{
							xCoordinate += TexWidth;
						}
					}
					else
					{
						xCoordinate += TexWidth;
					}
					//nextTexturePtr++;
					signFlag = 1;
					i++;
				}

				// display '0x'
			if(numRadix==1&&markingMode==1&&i==nextMarkingPos){

				if (overflow_next == 1)
				{
					texturePtr->FocusedSlice = SPACE;
				}else
				{
					texturePtr->FocusedSlice = X;
				}
				if(texturePtr->FocusedSlice == SPACE)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate) << 4;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(hitWaitingValue != 1)
				{
					if(pointPosRight != 0)
					{
						//dot 0.5 char
						if (i == pointPosLeft)
						{
							xCoordinate += TexWidth/2;
						}
						else
						{
							xCoordinate += TexWidth;
						}
					}
					else
					{
						xCoordinate += TexWidth;
					}
				}
				else
				{
					xCoordinate += TexWidth;
				}
				tens /= radixBase;
				//nextTexturePtr++;
				continue;
			}else if(numRadix==1&&markingMode==1&&i==(nextMarkingPos-1)){
				if (overflow_next == 1)
				{
					texturePtr->FocusedSlice = SPACE;
				}else
				{
					texturePtr->FocusedSlice = ZERO;
				}
				tens /= radixBase;
				if(texturePtr->FocusedSlice == SPACE)
				{
					continue;
				}
				texturePtr->OffsetX = (xCoordinate) << 4;
				status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
				if(status == AHMI_FUNC_MATRIX_ILLEGAL)
				{
					return status;
				}

				if(hitWaitingValue != 1)
				{
					if(pointPosRight != 0)
					{
						//dot 0.5 char
						if (i == pointPosLeft)
						{
							xCoordinate += TexWidth/2;
						}
						else
						{
							xCoordinate += TexWidth;
						}
					}
					else
					{
						xCoordinate += TexWidth;
					}
				}
				else
				{
					xCoordinate += TexWidth;
				}
				//nextTexturePtr++;
				continue;
			}

				//show '.'
				if(i == pointPosLeft)
				{
					if(overflow_next == 1)
						texturePtr->FocusedSlice = SPACE;
					else
						texturePtr->FocusedSlice = MPOINT;

					if(texturePtr->FocusedSlice == SPACE)
					{
						continue;
					}
					texturePtr->OffsetX = (xCoordinate) << 4;
					status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
					if(status == AHMI_FUNC_MATRIX_ILLEGAL)
					{
						return status;
					}

					if(hitWaitingValue != 1)
					{
						if(pointPosRight != 0)
						{
							if (i == pointPosLeft)
							{
								xCoordinate += TexWidth/2;
							}
							else
							{
								xCoordinate += TexWidth;
							}
						}
						else
						{
							xCoordinate += TexWidth;
						}
					}
					else
					{
						xCoordinate += TexWidth;
					}
					//nextTexturePtr++;
				//offsetpoint = i;
					i++;
				}

				//not show leading zeros
				if (!showingZero && code == 0 && !leadZero && (i != pointPosLeft - 1))
				{
					code=SPACE;
				}
				//showing the leading zeros
				else if(!showingZero && code == 0 && !leadZero && i == pointPosLeft - 1)
				{
					code= ZERO;
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
					showingZero = 1;
				}
				tens =tens /radixBase;
				if(overflow_next == 1)
				{
					texturePtr->FocusedSlice = SPACE;
					if(texturePtr->FocusedSlice == SPACE)
					{
						continue;
					}
					texturePtr->OffsetX = (xCoordinate) << 4;
					status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
					if(status == AHMI_FUNC_MATRIX_ILLEGAL)
					{
						return status;
					}

					if(hitWaitingValue != 1)
					{
						if(pointPosRight != 0)
						{
							//dot 0.5 char
							if (i == pointPosLeft)
							{
								xCoordinate += TexWidth/2;
							}
							else
							{
								xCoordinate += TexWidth;
							}
						}
						else
						{
							xCoordinate += TexWidth;
						}
					}
					else
					{
						xCoordinate += TexWidth;
					}
				}
				else
				{
					if (hitWaitingValue)
					{
						texturePtr->FocusedSlice = UNDERLINE;
					}else{
						texturePtr->FocusedSlice = code;
					}
					if(texturePtr->FocusedSlice == SPACE)
					{
						continue;
					}
					texturePtr->OffsetX = (xCoordinate) << 4;
					status = texturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,texturePtr,p_wptr);
					if(status == AHMI_FUNC_MATRIX_ILLEGAL)
					{
						return status;
					}

					if(hitWaitingValue != 1)
					{
						if(pointPosRight != 0)
						{
							//dot 0.5 char
							if (i == pointPosLeft)
							{
								xCoordinate += TexWidth/2;
							}
							else
							{
								xCoordinate += TexWidth;
							}
						}
						else
						{
							xCoordinate += TexWidth;
						}
					}
					else
					{
						xCoordinate += TexWidth;
					}
				//nextTexturePtr++;
				}
			}
		}

		//set unit
		if(unitflag)
		{
			unitTexturePtr->OffsetX = (xCoordinate) << 4;
			status = unitTexturePtr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,unitTexturePtr,p_wptr);
			if(status == AHMI_FUNC_MATRIX_ILLEGAL)
			{
				return status;
			}

			if(nextHitWaitingValue != 1)
			{
				if(pointPosRight != 0)
				{
					//dot 0.5 char
					if (i == pointPosLeft)
					{
						xCoordinate += UnitTexWidth/2;
					}
					else
					{
						xCoordinate += UnitTexWidth;
					}
				}
				else
				{
					xCoordinate += UnitTexWidth;
				}
			}
			else
			{
				xCoordinate += UnitTexWidth;
			}
		}

		if(movingDir == 0) //down
		{
			if(texturePtr->OffsetY != -(p_wptr->WidgetHeight << 4))
			{
				TextureOffsetY = texturePtr->OffsetY + (p_wptr->WidgetHeight << 4);
				texturePtr->OffsetY = TextureOffsetY;
				if(unitflag)
				{
					unitTexturePtr->OffsetY = TextureOffsetY;
				}
			}
			else
			{
				texturePtr->OffsetY = 0;
				if(unitflag)
				{
					unitTexturePtr->OffsetY = 0;
				}
			}
		}
		else if(movingDir == 1)//up
		{
			TextureOffsetY = texturePtr->OffsetY - (p_wptr->WidgetHeight << 4);
			texturePtr->OffsetY = TextureOffsetY;
			if(unitflag)
			{
				unitTexturePtr->OffsetY = TextureOffsetY;
			}
		}
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

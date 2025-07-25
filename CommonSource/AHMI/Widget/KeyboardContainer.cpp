////////////////////////////////////////////////////////////////////////////////
//keyboard container
//created by Zzen1sS
//2020/3/10
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "KeyboardContainer.h"
#include "drawImmediately_cd.h"
#include "stdlib.h"

#ifdef AHMI_CORE
extern TagClassPtr			TagPtr;
extern DynamicPageClassPtr  gPagePtr;
extern ProjectScreenOptions gScreenOptions;

extern QueueHandle_t		ActionInstructionQueue;
extern KBState*             gKBState;
extern Hinter              gHinter;
extern StringClassPtr       StringListPtr;
extern u8*					gStringPtr;
extern ConfigInfoClass		ConfigData;
extern EncodingClass*		gEncoder;
//-----------------------------
// name�� DynamicTexClass
// initialize
//  
//
// addtional comments :
//   none
//-----------------------------
KeyboardContainerClass::KeyboardContainerClass()
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
KeyboardContainerClass::~KeyboardContainerClass()
{

}

int coutValue(int value)
{
	int num = 0;
	if(value < 0)
	{
		num++;
		value = -value;
	}
	if (value == 0)
	{
		num ++;
	}else
	{
		while(value!=0)
		{
			num++;
			value = value / 10;
		}
	}
	return num;
}

void updateHints(WidgetClassPtr p_wptr, KBState* curKBState, u8 shouldUpdatePageCount=0)
{
	u32 stringInnerOffset;
	u16 hintStringLen;
	u32 i;
	char hintsCandidates[32] = {0};
	int hintStringID = -1;
	hintStringID = gHinter.getHintStringID(curKBState->hintInputValue);
	if (hintStringID !=-1)
	{
		//match
		if (shouldUpdatePageCount)
		{
			curKBState->hintPagetCount =  gHinter.getHintWordsPages(hintStringID);
		}
		gHinter.getHintWordsByPage(hintStringID,curKBState->hintPage, &stringInnerOffset, &hintStringLen);
		if (hintStringLen>0)
		{
			
			for(i = 0;i < hintStringLen;i++)
			{
				hintsCandidates[i] = *(gStringPtr + StringListPtr[hintStringID].addr + stringInnerOffset + i);
			}

		}
	}else
	{

		//not match
	}
	//show hint
	TagPtr->getTag(p_wptr->KBHINTTAGID).setString(hintsCandidates, 1);
	
}

void showStrForEnter(WidgetClassPtr p_wptr)
{
//	u8 hasMinus = 0;
	u32 tempValue = 0;
	u32 tens = 1;
	u8 curDigit = 0;
	u8 digitIdx = 0;
	int i;
	char showStr[32] = {0};
	int j;
	u8 stringLen;
	s32 curValue;
	u8 integerCount;
	u8 decimalCount;
	u8 mode;
	u8 showPasswd;
	u8 hideCursor;
	KBState* curKBState;

	mode = (p_wptr->SHOWOPTIONS & 0x6) >> 1;
	showPasswd = p_wptr->SHOWOPTIONS & 0x1;
	hideCursor = (p_wptr->SHOWOPTIONS & 0x8) >> 3;
	integerCount = p_wptr->KBINTCOUNT;
	decimalCount = p_wptr->KBDECCOUNT;

	curKBState = gKBState + p_wptr->KBSTATEID;

	if (mode == 2)
	{
		for (i = 0; i < 32; i++)
		{
			showStr[i] = curKBState->stringValue[i];		
		}
	}else if (mode == 1)
	{
		if((decimalCount != 0) &&(integerCount != 0))
		{
			if(curKBState->stringValue[0] == 0)
			{
				curKBState->stringValue[0] = '0';
			}
		}

		//copy kbstate string value to showStr
		for (i = 0; i < 32; i++)
		{
			showStr[i] = curKBState->stringValue[i];
		}
	}
	else
	{
		//num to string
		curValue = curKBState->numValue;
		if (curValue == 0 && showPasswd)
		{
			//show empty

		}else
		{
			if (curValue<0)
			{
//				hasMinus = 1;
				curValue = -curValue;
				showStr[digitIdx++] = '-';
			}
			tempValue = curValue;
			while (tempValue>9)
			{
				tens *= 10;
				tempValue = tempValue/10;
			}

			while (tens > 0)
			{
				curDigit = curValue / tens;
				curValue = curValue % tens;
				tens = tens / 10;
				showStr[digitIdx++] = curDigit + '0';
			}
		}
		
	}

	if (showPasswd)
	{
		//replace num with *
		for (int i = 0; i < 32; i++)
		{
			if (showStr[i]!=0)
			{
				showStr[i] = '*';
			}
		}
	}

	if(!hideCursor)
	{
		//add light sign
		for (int i = 0; i < 32; i++)
		{
			if (showStr[i]==0)
			{
				/*if((i == 0) && (integerCount != 0 || decimalCount != 0))
				{
					showStr[i] = '0';
					showStr[i + 1] = '|';
					break;
				}*/
				showStr[i] = '|';
				break;
			}
		}
	}

	TagPtr->getTag(p_wptr->BindTagID).setString(showStr, 1);
}

void showStr(WidgetClassPtr p_wptr)
{
//	u8 hasMinus = 0;
	u32 tempValue = 0;
	u32 tens = 1;
	u8 curDigit = 0;
	u8 digitIdx = 0;
	int i;
	char showStr[32] = {0};
	int j;
	u8 stringLen;
	s32 curValue;
	u8 integerCount;
	u8 decimalCount;
	u8 mode;
	u8 showPasswd;
	u8 hideCursor;
	KBState* curKBState;
    //set curValue
	/*p_wptr->KBSTATE_CURVALUE_L = ((u32)curValue) & 0xFFFF;
	p_wptr->KBSTATE_CURVALUE_H = ((u32)curValue) >> 16;*/

	mode = (p_wptr->SHOWOPTIONS & 0x6) >> 1;
	showPasswd = p_wptr->SHOWOPTIONS & 0x1;
	hideCursor = (p_wptr->SHOWOPTIONS & 0x8) >> 3;
	integerCount = p_wptr->KBINTCOUNT;
	decimalCount = p_wptr->KBDECCOUNT;

	curKBState = gKBState + p_wptr->KBSTATEID;

	if (mode == 2)
	{

		
		//get hintLen
		for (j=0;j < 32; j++)
		{
			if (curKBState->hintInputValue[j]!=0)
			{

			}else
			{
				break;
			}
		}

		if (j>0)
		{
			//insert '
			stringLen = 0;
			//copy kbstate string value to showStr
			for (i = 0; i < 32; i++)
			{
				if (curKBState->stringValue[i]!=0)
				{
					stringLen += 1;
				}else
				{
					//
					break;
				}
			
			}

			for (i = 0; i < stringLen - j; i++)
			{
				showStr[i] = curKBState->stringValue[i];		
			}
			showStr[i] = '\'';
			i++;
			for (; i < 32; i++)
			{
				showStr[i] = curKBState->stringValue[i-1];		
			}
		}else
		{
			for (i = 0; i < 32; i++)
			{
				showStr[i] = curKBState->stringValue[i];		
			}
		}
		


		
	}else if (mode == 1)
	{
		if((decimalCount != 0) &&(integerCount != 0))
		{
			if(curKBState->stringValue[0] == 0)
			{
				curKBState->stringValue[0] = '0';
			}
		}

		//copy kbstate string value to showStr
		for (i = 0; i < 32; i++)
		{
			showStr[i] = curKBState->stringValue[i];
		}
	}
	else
	{
		//num to string
		curValue = curKBState->numValue;
		if (curValue == 0 && showPasswd)
		{
			//show empty

		}else
		{
			if (curValue<0)
			{
//				hasMinus = 1;
				curValue = -curValue;
				showStr[digitIdx++] = '-';
			}
			tempValue = curValue;
			while (tempValue>9)
			{
				tens *= 10;
				tempValue = tempValue/10;
			}

			while (tens > 0)
			{
				curDigit = curValue / tens;
				curValue = curValue % tens;
				tens = tens / 10;
				showStr[digitIdx++] = curDigit + '0';
			}
		}
		
	}

	

	if (showPasswd)
	{
		//replace num with *
		for (int i = 0; i < 32; i++)
		{
			if (showStr[i]!=0)
			{
				showStr[i] = '*';
			}
		}
	}

	if(!hideCursor)
	{
		//add light sign
		for (int i = 0; i < 32; i++)
		{
			if (showStr[i]==0)
			{
				/*if((i == 0) && (integerCount != 0 || decimalCount != 0))
				{
					showStr[i] = '0';
					showStr[i + 1] = '|';
					break;
				}*/
				showStr[i] = '|';
				break;
			}
		}
	}

	TagPtr->getTag(p_wptr->BindTagID).setString(showStr, 1);
}


funcStatus KeyboardContainerClass::initWidget(
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
funcStatus KeyboardContainerClass::widgetCtrl(
	DynamicPageClassPtr pagePtr,
	WidgetClassPtr p_wptr,			
	ActionTriggerClassPtr ActionPtr,
	u8 u8_pageRefresh				
	)
{
//	s32 value;
//    s32	maxValue;
//	s32	minValue;
//    u32 textAddr;
//    TextureClassPtr texturePtr;
	
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
		ERROR_PRINT("ERROR: when drawing alphaimg widget, the offset\\width\\height exceeds the boundary");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_WidgetSizeErr);
	}
	
	//curString = curTag->getStringValue();
//	s32 curValue = (s32)((p_wptr->KBSTATE_CURVALUE_L) +  ( (p_wptr->KBSTATE_CURVALUE_H) << 16));
	showStr(p_wptr);
	
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
funcStatus KeyboardContainerClass::SetValueCtrl(
	DynamicPageClassPtr pagePtr,
    WidgetClassPtr p_wptr,  
    ActionTriggerClassPtr ActionPtr,
    u8 u8_pageRefresh      
    )
{
	if((NULL == p_wptr) || (NULL == ActionPtr) || NULL == ActionPtr->mTagPtr || NULL == gPagePtr->pBasicTextureList){
		ERROR_PRINT("ERROR: for NULL pointer");
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}

	p_wptr->REFRESH_FLAG |= WidgetValueRefreshFlag;
	p_wptr->REFRESH_FLAG |= WidgetSourceBufferRefreshFlag;
	
	return AHMI_FUNC_SUCCESS;
}

//**************************************
//reponse for Mouse.Touch
//
//**************************************
funcStatus KeyboardContainerClass::MouseTouchCtrl(
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

//set keyCode for this container
funcStatus KeyboardContainerClass::setKB(
	WidgetClassPtr p_wptr, 
	u32 value
)
{
	//s32 curValue = (s32)((p_wptr->KBSTATE_CURVALUE_L) +  ( (p_wptr->KBSTATE_CURVALUE_H) << 16));
	u32 keyValue;
	KBState* curKBState = gKBState + p_wptr->KBSTATEID;

	long long curValue;
	int i,j;
	int nextCharBytes;
	int lastCharBytes = 1;
	char* curStringValue;
	char* curHintInputValue;
	u8 point_flag = 0;
	u8 limitTemp;
	u8 integerCount;
	u8 decimalCount;
	u8 mode = (p_wptr->SHOWOPTIONS & 0x6) >> 1;
	u8 charLimit = p_wptr->CHAR_LIMIT;
	integerCount = p_wptr->KBINTCOUNT;
	decimalCount = p_wptr->KBDECCOUNT;
	/*int maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	int minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);*/
	if (charLimit==0)
	{
		charLimit = 31;
	}

	if (mode == 2)
	{
		//chinese
		curHintInputValue = (char*)curKBState->hintInputValue;
		curStringValue = (char*)curKBState->stringValue;
		switch (value)
		{
			case 0x10:
				//Shift
				if(p_wptr->KBSTATE_SHIFT != 1)
					p_wptr->KBSTATE_SHIFT = 1;
				else
					p_wptr->KBSTATE_SHIFT = 0;
			break;
			case 0xF0:
				//Cancel
				for (i = 0; i < 32; i++)
				{
					curStringValue[i] = 0;
					curHintInputValue[i] = 0;
				}
				curKBState->hintPage = 0;
				updateHints(p_wptr, curKBState, 1);
				   
			break;
			case 0xF1:
				//Enter
				for (i = 0; i < 32; i++)
				{
					curHintInputValue[i] = 0;
				}
				curKBState->hintPage = 0;
				updateHints(p_wptr, curKBState, 1);
			break;
			case 0xF2:
				//Back
				for (i = 0; i < 31; i++)
				{
					nextCharBytes = gEncoder->getNextCharacterBytesFromStr((u8 *)&curStringValue[i], ConfigData.encoding);
					
					if (nextCharBytes<=0)
					{
						break;
					}else
					{
						lastCharBytes = nextCharBytes;
						i = i + nextCharBytes - 1;
					}
				}
				for (i = 0; i < 31; i++)
				{
					if (curStringValue[i]==0)
					{
						break;
					}
				}
				if (i > 0)
				{
					for (j = 0; j < lastCharBytes; j++)
					{
						//delete one char
						curStringValue[i-j-1] = 0;
					}
				}
				//delete hint one char
				for (i = 0; i < 31; i++)
				{
					if (curHintInputValue[i]==0)
					{
						break;
					}
				}
				if (i > 0)
				{
					for (j = 0; j < lastCharBytes; j++)
					{
						//delete one char
						curHintInputValue[i-j-1] = 0;
					}
					////delete one char
					//curHintInputValue[i-1] = 0;
					curKBState->hintPage = 0;
					updateHints(p_wptr, curKBState, 1);
				}
			break;
		default:
			//char count < 31, add one at the end
			for (i = 0; i < 31; i++)
			{
				if (curStringValue[i]==0)
				{
					break;
				}
			}

			lastCharBytes = 0;//lastCharBytes means total element number
			for (j = 0; j < 31; j++)
			{
				nextCharBytes = gEncoder->getNextCharacterBytesFromStr((u8 *)&curStringValue[j], ConfigData.encoding);
				
				if (nextCharBytes<=0)
				{
					break;
				}else
				{
					lastCharBytes++;
					j = j + nextCharBytes - 1;
				}
			}
			if ((i < 31) && (lastCharBytes < charLimit))//charLimit means elementNumberLimit
			{
				char tmp;
				if((p_wptr->KBSTATE_SHIFT == 1) && (value >= 97) && (value <= 122))
					tmp = (value - 0x20);
				else
					tmp = value;
				curStringValue[i] = tmp;
				//update hint
				for (i = 0; i < 31; i++)
				{
					if (curHintInputValue[i]==0)
					{
						break;
					}
				}
				curHintInputValue[i] = tmp;
				curKBState->hintPage = 0;
				updateHints(p_wptr, curKBState, 1);
			}
			break;
		}
	}else if (mode == 1)
	{
		if(integerCount == 0 && decimalCount == 0)
		{
			//ascii
			curStringValue = (char*)curKBState->stringValue;
			switch (value)
			{
				case 0x10:
					//Shift
					if(p_wptr->KBSTATE_SHIFT != 1)
						p_wptr->KBSTATE_SHIFT = 1;
					else
						p_wptr->KBSTATE_SHIFT = 0;
				break;
				case 0xF0:
					//Cancel
					for (i = 0; i < 32; i++)
					{
						curStringValue[i] = 0;
					}
				   
				break;
				case 0xF1:
					//Enter
					// 
				break;
				case 0xF2:
					//Back
					for (i = 0; i < 31; i++)
					{
						if (curStringValue[i]==0)
						{
							break;
						}
					}
					if (i > 0)
					{
						//delete one char
						curStringValue[i-1] = 0;
					}
				break;
			default:
				//char count < 31, add one at the end
				for (i = 0; i < 31; i++)
				{
					if (curStringValue[i]==0)
					{
						break;
					}
				}
				if (i<=charLimit-1)
				{
					char tmp;
					if((p_wptr->KBSTATE_SHIFT == 1) && (value >= 97) && (value <= 122))
						tmp = (value - 0x20);
					else
						tmp = value;
					curStringValue[i] = tmp;
				}
				break;
			}
		}
		else
		{
			//num with point
			curStringValue = (char*)curKBState->stringValue;
			for (i = 0; i < 32; i++)
			{
				if(curStringValue[i] == '.')
				{
					point_flag = 1;
					break;
				}
			}

			if((point_flag == 1) && (value == '.'))
			{
				return AHMI_FUNC_SUCCESS;
			}

			if((curStringValue[0] == '0') && (value != '.') && (!point_flag))
			{
				curStringValue[0] = 0;
			}
			limitTemp = integerCount;
			//if(curStringValue[0] == '-')
			//{
			//	limitTemp = integerCount + 1;
			//}
			
			for (i = 0; i < 32; i++)
			{
				if(curStringValue[i] == '.')
				{
					/*if(curStringValue[0] == '-')
					{
						limitTemp = i + 1 + decimalCount + 1;
					}else*/
					{
						limitTemp = i + 1 + decimalCount;
					}
					break;
				}
			}

			switch (value)
			{
				case 0x2D:
					//"-" return
					return AHMI_FUNC_SUCCESS;

				case 0xF0:
					//Cancel
					for (i = 0; i < 32; i++)
					{
						if(i == 0)
						{
							curStringValue[i] = '0';
							continue;
						}
						curStringValue[i] = 0;
					}
				   
				break;
				case 0xF1:
					//Enter
					// 
				break;
				case 0xF2:
					//Back
					for (i = 0; i < 31; i++)
					{
						if (curStringValue[i]==0)
						{
							break;
						}
					}
					if (i > 0)
					{
						//delete one char
						curStringValue[i-1] = 0;
					}
				break;
			default:
				//char count < 31, add one at the end
				for (i = 0; i < 31; i++)
				{
					if (curStringValue[i]==0)
					{
						break;
					}
				}
				if (i < limitTemp || value == '.')
				{
					if(i == 0 && value == '.')
					{
					}
					else
					{
						curStringValue[i] = value;
					}
				}
				break;
			}
		}
	}
	else
	{
		//number
		curValue = (s32)curKBState->numValue;
		switch (value)
		{
			case 0x30:
			case 0x31:
			case 0x32:
			case 0x33:
			case 0x34:
			case 0x35:
			case 0x36:
			case 0x37:
			case 0x38:
			case 0x39:
				keyValue = value - 0x30;
				if(curValue >= 0){
					curValue = curValue * 10 + keyValue;
				}else{
					curValue =  -((-curValue) * 10 + keyValue);
				}
			break;
			case 0x2E:
				//.
			break;
			case 0x2D:
				//+/-
				curValue = -curValue;
           
			break;
			case 0xF0:
				//Cancel
			   curValue = 0;
			break;
			case 0xF1:
				//Enter
				// 
			break;
			case 0xF2:
				//Back
			   curValue = curValue / 10;
			break;
		default:
			break;
		}
		
		//set kbState
		if (coutValue(curValue)<=charLimit && curValue >= -999999999 && curValue <= 999999999)
		{
			curKBState->numValue = curValue;
		}
		
	}

	if(value == 0xF1)//Enter
		showStrForEnter(p_wptr);
	else
		showStr(p_wptr);
	
	return AHMI_FUNC_SUCCESS;
}

//set idx for this hints
funcStatus KeyboardContainerClass::setKBHint(
	WidgetClassPtr p_wptr, 
	u32 idx
)
{
//	u32 keyValue;
	u32 stringInnerOffset;
	u16 hintStringLen;
	u16 i;
	s16 appendIdx;
	u16 j;

	KBState* curKBState = gKBState + p_wptr->KBSTATEID;

	int hintStringID = -1;

	if (idx == 0)
	{
		return AHMI_FUNC_SUCCESS;
	}

	idx = idx - 1;//button group start from 1

	hintStringID = gHinter.getHintStringID(curKBState->hintInputValue);
	if (hintStringID !=-1)
	{
		//match
		gHinter.getHintWordByPageAndIndex(hintStringID,curKBState->hintPage,idx, &stringInnerOffset, &hintStringLen);
		if (hintStringLen>0)
		{
			//get hintInputLen
			for( j = 0; j < 32; j++ )
			{
				if (curKBState->hintInputValue[j]==0)
				{
					break;
				}
			}
			for( appendIdx = 0; appendIdx < 32; appendIdx++)
			{
				if (curKBState->stringValue[appendIdx] == 0)
				{
					break;
				}
			}
			if (appendIdx - j >=0)
			{
				//valid
			}else
			{
				//error
				j = appendIdx;
			}
			//clear last j char
			for(i = 0; i < j; i++)
			{
				curKBState->stringValue[appendIdx-1-i] = 0;
			}
			appendIdx = appendIdx - j;
			
			for(i = 0;i < hintStringLen;i++)
			{
				curKBState->stringValue[appendIdx + i] = *(gStringPtr + StringListPtr[hintStringID].addr + stringInnerOffset + i);
			}
			//clear hint
			for (i = 0; i < 32; i++)
			{
				
				curKBState->hintInputValue[i] = 0;
			}
			updateHints(p_wptr, curKBState, 1);

			showStr(p_wptr);
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//change hints page by step
funcStatus KeyboardContainerClass::setKBHintsPage(
	WidgetClassPtr p_wptr, 
	u32 step,
	u8 direction
)
{
//	u32 keyValue;
//	u32 stringInnerOffset;
//	u16 hintStringLen;
//	u16 i;
//	s16 appendIdx;
//	u16 j;

	KBState* curKBState = gKBState + p_wptr->KBSTATEID;

	if (direction == 1)
	{
		//next
		if (curKBState->hintPage < curKBState->hintPagetCount-1 )
		{
			curKBState->hintPage++;
			updateHints(p_wptr, curKBState);
		}
	}else
	{
		//prev
		if (curKBState->hintPage > 0)
		{
			curKBState->hintPage--;
			updateHints(p_wptr, curKBState);
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

int transforStringToInt(char* pdata, u8 integerCount, u8 decimalCount)
{
	float p ;
	int n = 0, m = 0, i;
	int int_temp = 1, dec_temp_final = 1;
//	char *temp;

	p = atof(pdata);

	for(i = 1; i <= decimalCount; i++)
	{
		dec_temp_final = 10 * dec_temp_final;
	}

	p = p * dec_temp_final;

	for(i = 1; i <= (integerCount + decimalCount); i++)
	{
		int_temp = 10 * int_temp;
	}
	
	m = (p*10);
	m %= 10;

	if(m >= 5)
	{
		n = p;
		n++;
	}
	else
	{
		n = p;
	}
	n = n % int_temp;

	return n;
}

//get keyboard value from this container
funcStatus KeyboardContainerClass::getKB(
	WidgetClassPtr p_wptr,
	u16 tagId
)
{
	//s32 curValue = (s32)((p_wptr->KBSTATE_CURVALUE_L) +  ( (p_wptr->KBSTATE_CURVALUE_H) << 16));
	//return curValue;
	TagClassPtr targetTag;
	KBState* curKBState;
	u8 mode;
	u8 entermode;
	u8 integerCount;
	u8 decimalCount;
	s32 outValue;

	targetTag = &TagPtr->getTag(tagId);
	if (targetTag == NULL)
	{
		return (EM_enErrRetModularWidget | EM_ErrRetWidget_NULLPointerErr);
	}
	
	curKBState = gKBState + p_wptr->KBSTATEID;

	mode = (p_wptr->SHOWOPTIONS & 0x6) >> 1;
	entermode = (p_wptr->SHOWOPTIONS & 0x10) >> 4;

	int maxValue = (s32)((p_wptr->MaxValueH << 16) + p_wptr->MaxValueL);
	int minValue = (s32)((p_wptr->MinValueH << 16) + p_wptr->MinValueL);

	integerCount = p_wptr->KBINTCOUNT;
	decimalCount = p_wptr->KBDECCOUNT;

	if (mode == 2)
	{
		if(curKBState->hintInputValue[0] == 0)
		{
			targetTag->setString((char*)curKBState->stringValue, 1);
			//clear
			setKB(p_wptr, 0xF0);
		}
	}
	else if (mode == 1)
	{
		if(integerCount == 0 && decimalCount == 0)
		{
			//ascii
			targetTag->setString((char*)curKBState->stringValue, 1);
		}
		else
		{
			outValue = transforStringToInt((char*)curKBState->stringValue, integerCount, decimalCount);
			targetTag->setRawValue((u32)outValue);
		}
		//clear
		setKB(p_wptr, 0xF0);
	}
	else
	{
		//num
		outValue = (s32)curKBState->numValue;
		if (outValue >= minValue && outValue <= maxValue)
		{
			targetTag->setRawValue((u32)outValue);
			
		}

		if(!entermode)
		{
			//clear
			setKB(p_wptr, 0xF0);
		}

		
	}
	
	return AHMI_FUNC_SUCCESS;
}
#endif

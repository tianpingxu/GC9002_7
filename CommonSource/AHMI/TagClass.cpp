////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI Team
//
// Create Date:   2015/11/17
// File Name:     TagClass.c
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Revision 2.01 - modify the tagSetValue function by Chen
// Revision 2.04 - C++, 20160321 by Yu Chunying
// Revision 2.04 - C++, 2018.10.4 modify the setValue function by Yu Chunying
// Additional Comments:
////////////////////////////////////////////////////////////////////////////////
#include "AHMICfgDefinition.h"
#include "publicInclude.h"
#include "publicDefine.h"
#include "AHMIBasicStructure.h"
#include "ButtonClass.h"
#include "ClockClass.h"
#include "KnobClass.h"
#include "MeterClass.h"
#include "NumberClass.h"
#include "OscilloscopeClass.h"
#include "ProgBarClass.h"
#include "SliderClass.h"
#include "TextClass.h"
#include "VideoClass.h"
#include "OnOffTexture.h"
#include "DynamicTexClass.h"
#include "Keyboard.h"
#include "TouchTask.h"
#include "player1c1.h"
#include "semphr.h"
#if defined(EMBEDDED) && defined(VERSION_2)
#include "csi_ahmi.h"
#include "csi_ahmi_hw.h"
#include "AHMITouch.h"
//#include "gc90x_initial.h"

extern uint8_t calibration_ing;
#endif

#ifdef AHMI_DEBUG
#include "trace.h"
#include <iostream>
#endif

#ifdef AHMI_CORE 

extern ConfigInfoClass  ConfigData;
extern TimerClassPtr  TimerPtr;
extern u16             WorkingPageID;
extern DynamicPageClassPtr gPagePtr;
extern TagClassPtr     TagPtr;
extern u8*     TagChangedListPtr;

extern QueueHandle_t   ActionInstructionQueue;
extern ActionProcessorClass gAction;
extern StringClassPtr	gStringTagStringPtr;
extern u8*			   gStringPtr;
extern StringClassPtr  StringListPtr;     // pointer of string
extern u8*             gTagValueSpacePtr;
extern EncodingClass*		gEncoder;
extern IntervalManager gIntervalManager;
extern WaveFilter*		gWaveFilterPtr; //pointer of wave filters
extern WaveFilterStatus* gWaveFilterStatusPtr; //pointer of wave filter status
extern WaveFilterTickArg* gWaveFilterTickArgPtr;
extern uint32_t TouchScreenType;
extern u16 KeyBoard_PageID;
extern u16 TouchCalibration_PageID;
extern u16 ErrorManagement_PageID;
extern u8 windowPressed;
extern u8 MULTI_PAGE_ANIMATION_SWIPE_EXECUTING;
extern uint8_t gPageAnimationFlag;
#ifdef USE_MODBUS_MASTER
extern unsigned int    usMRegHoldBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS];
#endif

extern QueueHandle_t PlayerInstructionQueue;
extern SemaphoreHandle_t xSemaphoreSetValue;
extern SemaphoreHandle_t xSemaphoreSetString;
extern ProjectScreenOptions gScreenOptions;

TagClass::TagClass(void)
{}

TagClass::~TagClass(void)
{}

TagClass& TagClass::getTag(u16 tagID)
{
	u32 registerID;
	//ERROR_PRINT("get tag by custom []");
	if (tagID == SYSTEM_POINTER_TAG)
	{
		registerID = this[SYSTEM_POINTER_TAG].getValue();
		for (int i = 0; i < ConfigData.NumofTags; i++)
		{
			if (this[i].mRegID == registerID)
			{
				//hit target tag
				return this[i];
			}
		}
		return this[0];
	}
	return this[tagID];
}

u32 TagClass::getTagID(u32 regID)
{
	for (int i = 0; i < ConfigData.NumofTags; i++)
	{
		if (TagPtr[i].mRegID == regID)
		{
			//hit target tag
			return i;
		}
	}
	return 0;
}

//void TagClass::initTagStr(u32 tagStrIdx)
//{
//	//init string tag initvalue
//	int initStringIdx = this->mValue;
//	if( TAG_IS_STR(this->mTagType ))
//	{
//		this->mValue = tagStrIdx;
//		if (initStringIdx>=0 &&initStringIdx<ConfigData.NumofStrings)
//		{
//			this->setString((char*)(gStringPtr + StringListPtr[initStringIdx].addr), 0);
//
//		}else{
//			ERROR_PRINT("ERROR in function SetTag: string id out of boundry");
//			return;
//		}
//	}
//	
//}

//-----------------------------
// function: InitTag
// @brief:init tag
// parameters:
// @param1	TagClassPtr Ptr,                             pointer to tag
// @param2	char* name,                                  name of tag
// @param3	u16 type,                                    tag type: TAGTYPE_INT:int; TAGTYPE_STR:string.
// @param4	u32 initValue,								 init value
// @param6	WidgetLinkDataPtr mWidgetLinkDataPtr,		 binding widget
// @param7	u16 mNumOfDynamicPageAction,				 number of page action instruction.
// @param8	PageLinkDataPtr mPageLinkDataPtr,			 linked pages
// @param9	u16 mNumOfCanvasAction,						 number of canvas action instruction.
// @param10	CanvasLinkDataPtr mCanvasLinkDataPtr,		 linked canvas.
// @param11	u8 mLinkTimerID,							 linked timer ID.
// @param12	u8 mLinkTimerVariableID,					 linked timer Variable ID.
// @param13	u16 tagNeedUpdate,							 if update value by modbus.
// @param14	u16 tagRW,									 flag for update value by modbus: TAGREAD:read ,TAGWRITE:write.
// @param15	u16 regID									 the regID of modbus.
//-----------------------------
u32 TagClass::initTag(
	u16 NumOfWidgetLinker,
	u16 NumOfCanvasAction,
	u16 NumOfDynamicPageAction,
	u16  tagID,
	u16 Type,
	u32 RegID,
	u16 addr,
	u16 valueLen,
	u16 elemNum,
	u32 StoreID,
	WidgetLinkDataPtr WidgetLinkDataPtr,
	PageLinkDataPtr PageLinkDataPtr,
	CanvasLinkDataPtr CanvasLinkDataPtr
)
{
	u16 linkElementCount = 0;
	u32 size, i;
	
	this->mTagType				= Type;
	this->mRegID					= RegID;
	this->mAddr					= addr;
	this->mValueLen				= valueLen;
	this->mCurArryElemNum		= elemNum;
	this->mStoreID				= StoreID;
	this->mNumOfElementLinker	= (NumOfWidgetLinker + NumOfCanvasAction + NumOfDynamicPageAction);

    /* The tag linked keyboard. */
	if(tagID == SYSTEM_KEYBOARD_TAG) 
	{
		this->mNumOfElementLinker = 1;
		size = sizeof(struct TagElementLinkData) * (this->mNumOfElementLinker);
		this->pTagELmementLinker = (ElemenLinkDataPtr) csi_kernel_malloc( size, NULL );
		if(this->pTagELmementLinker == NULL && size != 0)
		{
			#ifdef AHMI_DEBUG
			ERROR_PRINT("pTagELmementLinker Initial failed.\r\n");
			#endif
			return (EM_enErrRetModularTag | EM_ErrRetTag_NULLPointerErr);
		}
		this->pTagELmementLinker[0].mElementType = ELEMENT_TYPE_KEYBOARD;
		this->pTagELmementLinker[0].mLinkElementPtr = NULL;
		return AHMI_FUNC_SUCCESS;
	}
    /* You can change current page by change the tag's value. */
	else if(tagID == SYSTEM_PAGE_TAG)
	{
		this->mNumOfElementLinker = 1;
		size = sizeof(struct TagElementLinkData) * (this->mNumOfElementLinker);
		this->pTagELmementLinker = (ElemenLinkDataPtr) csi_kernel_malloc( size, NULL );
		if(this->pTagELmementLinker == NULL && size != 0)
		{
			#ifdef AHMI_DEBUG
			ERROR_PRINT("pTagELmementLinker Initial failed.\r\n");
			#endif
			return (EM_enErrRetModularTag | EM_ErrRetTag_NULLPointerErr);
		}
		this->pTagELmementLinker[0].mElementType = ELEMENT_TYPE_PAGE_GOTO;
		this->pTagELmementLinker[0].mLinkElementPtr = NULL;
		return AHMI_FUNC_SUCCESS;
	}

    /* Init all link information for the tag. */
	if(this->mNumOfElementLinker != 0)
	{
		size = sizeof(struct TagElementLinkData) * (this->mNumOfElementLinker);
		this->pTagELmementLinker = (ElemenLinkDataPtr) csi_kernel_malloc( size, NULL );
		if(this->pTagELmementLinker == NULL && size != 0) 
		{
			#ifdef AHMI_DEBUG
			ERROR_PRINT("error in initializing tag.\r\n");
			#endif
			return (EM_enErrRetModularTag | EM_ErrRetTag_NULLPointerErr);
		}
	
		/* widget link list. */
		for(i = 0; i < NumOfWidgetLinker; i++)
		{
			this->pTagELmementLinker[linkElementCount].mElementType = ELEMENT_TYPE_WIDGET;
			this->pTagELmementLinker[linkElementCount].mLinkElementPtr = (void *)&(WidgetLinkDataPtr[i]);
			linkElementCount++;
		}
	
		/* canvas link list. */
		for(i = 0; i < NumOfCanvasAction; i++)
		{
			this->pTagELmementLinker[linkElementCount].mElementType = ELEMENT_TYPE_CANVAS;
			this->pTagELmementLinker[linkElementCount].mLinkElementPtr = (void *)&(CanvasLinkDataPtr[i]);
			linkElementCount++;
		}
	
		/* page link list. */
		for(i = 0; i < NumOfDynamicPageAction; i++)
		{
			this->pTagELmementLinker[linkElementCount].mElementType = ELEMENT_TYPE_PAGE;
			this->pTagELmementLinker[linkElementCount].mLinkElementPtr = (void *)&(PageLinkDataPtr[i]);
			linkElementCount++;
		}
	}
	else 
		this->pTagELmementLinker = NULL;
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function��getValue
// @brief:return the value of the tag.
// parameters:
// @param:
// @return:value of the tag.
//-----------------------------
u32 TagClass::getValue() const
{
	//return this->mValue;
	if (TAG_IS_INT(this->mTagType) && (this->mAddr != 0))
	{
		//number
		u32 result = 0;
		/*for (int i = 0; i < 4; i++)
		{
			result += (*(gTagValueSpacePtr + this->mAddr + i) << (3-i) * 8);
		}*/
		result =  *((u32*)(gTagValueSpacePtr + this->mAddr));
		return result;
	}
	return 0;
}

#if 0
uint32_t safeFlag = 0;
uint8_t DrawInterruptCount = 0;
//-----------------------------
// function: AHMIEnterSafeSection
// @brief:set value of the tag.
// parameters:
// @param1:v: value.
// @param2:tagID: ID of the tag.
//-----------------------------
void EnterSafeSectionForTag(void)
{
#if 0
	taskENTER_CRITICAL();
    safeFlag++;
	taskEXIT_CRITICAL();
#endif
}

//-----------------------------
// function: AHMIExitSafeSection
// @brief:set value of the tag.
// parameters:
// @param1:v: value.
// @param2:tagID: ID of the tag.
//-----------------------------
extern void AHMIUsrDef_FreshTrigDraw(void);
void ExitSafeSectionForTag(void)
{
#if 0
	taskENTER_CRITICAL();
    safeFlag--;
    
    if((DrawInterruptCount == 1 ) && (safeFlag == 0))
    {
        DrawInterruptCount = 0;
        AHMIUsrDef_FreshTrigDraw();       
    }
	taskEXIT_CRITICAL();
#endif
}
#endif

funcStatus TagClass::setValueTrigger(TagClass* pTag, uint32_t value, bool bNeedSyn)
{
    PLAR_tstAHMICmd stAHMICmd;
    uint8_t front_or_back = 0;

    if(pTag == NULL)
	{
        return (EM_enErrRetModularTag | EM_ErrRetTag_NULLPointerErr);
	}

    memset(&stAHMICmd, 0, sizeof(stAHMICmd));
    stAHMICmd.u32CmdType = enCmdType_SetValue;
    stAHMICmd.u32PageID = WorkingPageID;
    stAHMICmd.u32Para[0] = (uint32_t)pTag;
    stAHMICmd.u32Para[1] = value;
	stAHMICmd.u32Para[2] = (uint32_t)bNeedSyn;

	if(bNeedSyn)
		front_or_back = 1;

    if(gScreenOptions.enable)
	{
		stAHMICmd.screenType = RefreshScreenMain;
		if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, front_or_back, portMAX_DELAY) != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
		}
		if(bNeedSyn)
		{
			csi_kernel_sem_wait( xSemaphoreSetValue, 1000);
		}

		vTaskDelay(30 / portTICK_RATE_MS);
		stAHMICmd.screenType = RefreshScreenSub;
		if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, front_or_back, portMAX_DELAY) != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
		}
		if(bNeedSyn)
		{
			csi_kernel_sem_wait( xSemaphoreSetValue, 1000);
		}
	}
	else
	{
		if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, front_or_back, portMAX_DELAY) != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
		}
		if(bNeedSyn)
		{
			csi_kernel_sem_wait( xSemaphoreSetValue, 1000);
		}
	}
    
    return AHMI_FUNC_SUCCESS;
}

funcStatus TagClass::setStringTrigger(TagClass* pTag, bool bNeedSyn)
{
    PLAR_tstAHMICmd stAHMICmd;
    uint8_t front_or_back = 0;

    if(pTag == NULL)
        return (EM_enErrRetModularTag | EM_ErrRetTag_NULLPointerErr);
    
    memset(&stAHMICmd, 0, sizeof(stAHMICmd));
    stAHMICmd.u32CmdType = enCmdType_SetString;
    stAHMICmd.u32PageID = WorkingPageID;
    stAHMICmd.u32Para[0] = (uint32_t)pTag;
	stAHMICmd.u32Para[1] = (uint32_t)bNeedSyn;

	if(bNeedSyn)
		front_or_back = 1;

    if(gScreenOptions.enable)
	{
		stAHMICmd.screenType = RefreshScreenMain;
		if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, front_or_back, portMAX_DELAY) != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
		}
		if(bNeedSyn)
		{
			csi_kernel_sem_wait( xSemaphoreSetString, 1000);
		}

		vTaskDelay(30 / portTICK_RATE_MS);
		stAHMICmd.screenType = RefreshScreenSub;
		if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, front_or_back, portMAX_DELAY) != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
		}
		if(bNeedSyn)
		{
			csi_kernel_sem_wait( xSemaphoreSetString, 1000);
		}
	}
	else
	{
		if(csi_kernel_msgq_put(PlayerInstructionQueue, &stAHMICmd, front_or_back, portMAX_DELAY) != 0)
		{
			return (EM_enErrRetModularActionProcessor | EM_ErrRetActionProcessor_QueueSendErr);
		}
		if(bNeedSyn)
		{
			csi_kernel_sem_wait( xSemaphoreSetString, 1000);
		}
	}
    
    return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function: setValue
// @brief:set value of the tag.
// parameters:
// @param1:v: value.
// @param2:tagID: ID of the tag.
//-----------------------------
#if defined(EMBEDDED) && defined(VERSION_2)
extern uint32_t AHMIOutputRotation;
extern void SetAHMIOutputRotation(uint32_t rotation);
extern void SetBacklightWaitDuration(uint32_t duration);
extern u8 initAHMI;
#endif
u32 TagClass::setValue(u32 v, u16 tagID, bool bNeedSyn, bool isBeforeLoadAction)
{
	int waveFilterStatusIDLEIdx;
	u8 useWaveFilter;
	u32 tagValue;
	WaveFilterTickArg* arg;
	int frequence;//frequence of wavefilter
	int intervalTime;//1000/frequence
#ifdef AHMI_DEBUG
	char text[100];
#endif
    /* if tagID is greater than or equal to number of Tag. */
	if(tagID >= ConfigData.NumofTags)
	{
		ERROR_PRINT("ERROR in tagSetValue: tagID out of baundary");
		return (EM_enErrRetModularTag | EM_ErrRetTag_TagIDOverrun);
	}

	//check tag type
	if (TAG_IS_STR(mTagType))
	{
		//string tag
		ERROR_PRINT("ERROR in set value to a string tag");
		return (EM_enErrRetModularTag | EM_ErrRetTag_TagTypeErr);
	}
 
    /* if tagID is equal to 0. */
	if(tagID == 0)
    {
		updateTagValueToValueSpace(0);
		return AHMI_FUNC_SUCCESS;
    }
	
	useWaveFilter = 0;
	for (int i = 0; i < ConfigData.NumOfWaveFilterStatus; i++)
	{
		if (gWaveFilterStatusPtr[i].tagID == tagID)
		{
			//should use wavefilter
			
			useWaveFilter = 1;
			
			//check wheter tag is related with a interval
			waveFilterStatusIDLEIdx = getIntervalInUsedByTag(this);
			if(waveFilterStatusIDLEIdx!=-1){
				//tag has a interval related
			}else{
				//has no interval related
				waveFilterStatusIDLEIdx = gIntervalManager.getIDLEInterval();//get an idle position
				if (waveFilterStatusIDLEIdx==-1)
				{
					//no idle
					ERROR_PRINT("no idle wave filter status");
					if(isBeforeLoadAction)
					{
						setRawValue(v);
					}
					else
					{
						setValueTrigger(this, v, bNeedSyn);
					}
					return AHMI_FUNC_SUCCESS;
				}
			}
			
			// set arg
			arg = gWaveFilterTickArgPtr + waveFilterStatusIDLEIdx;
			arg->status = gWaveFilterStatusPtr + i;
			arg->waveFilter = &gWaveFilterPtr[gWaveFilterStatusPtr[i].waveFilterIdx];
			tagValue = getValue();
			if(tagValue < v)
			{
				if((tagValue + arg->waveFilter->getStep()) < v)
				{
					arg->status->last = tagValue + arg->waveFilter->getStep();
					arg->status->temp = tagValue + arg->waveFilter->getStep();
				}
				else
				{
					arg->status->last = tagValue;
					arg->status->temp = tagValue;
				}
			}
			else if(tagValue > v)
			{
				if((tagValue - arg->waveFilter->getStep()) > v)
				{
					arg->status->last = tagValue - arg->waveFilter->getStep();
					arg->status->temp = tagValue - arg->waveFilter->getStep();
				}
				else
				{
					arg->status->last = tagValue;
					arg->status->temp = tagValue;
				}
			}
			else
			{
				arg->status->last = tagValue;
				arg->status->temp = tagValue;
			}
			arg->status->target = v;
			arg->tag = this;
			arg->totalDuration = 0;
			arg->intervalID = -1;
			frequence = arg->waveFilter->getFrequence();
			if (frequence <= 0)
			{
				frequence = 1;
			}
			intervalTime = 1000/frequence;
			arg->intervalID = gIntervalManager.setInterval(waveFilterStatusIDLEIdx,tickWaveFilterStatus,arg,intervalTime);

		}
	}
	if (!useWaveFilter)
	{
		if(isBeforeLoadAction)
		{
			setRawValue(v);
		}
		else
		{
			setValueTrigger(this, v, bNeedSyn);
		}
	}

	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function: setRawValue
// @brief:set value of the tag.
// parameters:
// @param1:v: value.
// @param2:tagID: ID of the tag.
//-----------------------------
u32 TagClass::setRawValue(u32 v)
{
	u32 tagid;
    /* if use modbus. */
#ifdef USE_MODBUS_MASTER
    /* set reg's value.*/
	usMRegHoldBuf[0][this->mRegID]=v;
#endif
#ifdef AHMI_DEBUG
	WriteText(text,"Set tag value %d.\r\n", v);
	ERROR_PRINT(text);
#endif
	updateTagValueToValueSpace(v);
    
	tagid = getTagID(this->mRegID);
	#if defined(EMBEDDED) && defined(VERSION_2)
    /* if tagID is SYSTEM_BACKLIGHT_TAG. */
	if(tagid == SYSTEM_BACKLIGHT_TAG)
    {
        /* backlight value 0~100*/
        if(v > 100)
        {
            v = 100;
        }
        #ifdef EMBEDDED
		/* change backlight */
		csi_ahmi_SetBacklightBrightness(v);
		csi_ahmi_StartBacklight();
		#endif
    }
	/* if tagID is SYSTEM_CONTRAST_TAG. */
	else if(tagid == SYSTEM_CONTRAST_TAG)
    {
		csi_ahmi_SetDisplayContrast(v);
    }
	/* if tagID is SYSTEM_SATURATION_TAG. */
	else if(tagid == SYSTEM_SATURATION_TAG)
    {
		csi_ahmi_SetDisplaySaturation(v);
    }
	/* if tagID is SYSTEM_BRIGHTNESS_TAG. */
	else if(tagid == SYSTEM_BRIGHTNESS_TAG)
    {
		csi_ahmi_SetDisplayBrightness(v);
    }
	/* if tagID is SYSTEM_DisplayRotate_TAG. */
	else if(tagid == SYSTEM_DisplayRotate_TAG)
    {
		if((v % 90 == 0) && (v <= 270))
		{
			#if !defined(CHIP_GC9002) && !defined(CHIP_GC9005)
			if(((AHMIOutputRotation*90 - v) != 180) && ((v - AHMIOutputRotation*90) != 180) && (AHMIOutputRotation*90 != v))
			{
				csi_ahmi_DisableDisplay();
				csi_ahmi_SetDisplayPriParamForRotate();
				csi_ahmi_EnableDisplay();
			}
			#endif
			SetAHMIOutputRotation(v);
			
			if((TouchScreenType > TOUCHSCREENOFF) && (TouchScreenType < TOUCHSCREENTYPEEND))
			{
				AHMITouch_InitDevice();
			}  
			initAHMI = 0;
		}
    }
	/* if tagID is SYSTEM_BacklightDuration_TAG. */
	else if(tagid == SYSTEM_BacklightDuration_TAG)
    {
		SetBacklightWaitDuration(v);
		csi_ahmi_InitBacklightAndTimer();
    }
    
	//Normal Page don't excute set touchscreen calibration tag.
	if((TouchCalibration_PageID != WorkingPageID) && (SYSTEM_TOUCHSCREEN_TAG == tagid))
	{
		return (EM_enErrRetModularTag | EM_ErrRetTag_OpSysTagErr);
	}

	//TouchCalibration_Page don't excute set normal tag actions.
	if((TouchCalibration_PageID == WorkingPageID) && (SYSTEM_TOUCHSCREEN_TAG != tagid) && (SYSTEM_PAGE_TAG != tagid))
	{
		return (EM_enErrRetModularTag | EM_ErrRetTag_OpSysTagErr);
	}
	
	//TouchCalibration_Page && calibration_ing don't excute loadpage.
	if((TouchCalibration_PageID == WorkingPageID) && (SYSTEM_PAGE_TAG == tagid) && (calibration_ing == 1))
	{
		return (EM_enErrRetModularTag | EM_ErrRetTag_OpSysTagErr);
	}
    #endif

	//Normal Page don't excute set error message page number tag.
	if((ErrorManagement_PageID != WorkingPageID) && (SYSTEM_ERROR_MESSAGE_PAGE_NUM_TAG == tagid))
	{
		return (EM_enErrRetModularTag | EM_ErrRetTag_OpSysTagErr);
	}

	//ErrorManagement_Page don't excute set normal tag actions.
	if((ErrorManagement_PageID == WorkingPageID) && (SYSTEM_ERROR_MESSAGE_PAGE_NUM_TAG != tagid))
	{
		return (EM_enErrRetModularTag | EM_ErrRetTag_OpSysTagErr);
	}
	
	//if(MULTI_PAGE_ANIMATION_SWIPE_EXECUTING || gPageAnimationFlag)
	//	return NullRefreshFlag;
	
    return setBindingElement();

}

void TagClass::updateTagValueToValueSpace(u32 v)
{
	if (TAG_IS_INT(mTagType))
	{
		//number
		int i;
		u8* valueArr = (u8*)&v;
		for(i=0;i<4;i++)
		{
			*(gTagValueSpacePtr + mAddr + i) = *(valueArr + i);
		}
	}
}

//-----------------------------
// function: setString
// @brief:set string to the tag.
// parameters:
//   @param1 char*  str         pointer to the string
//-----------------------------
u32 TagClass::setString(char* str, u8 update, bool bNeedSyn, bool isBeforeLoadAction)
{
	uint8_t dif_flag = 0;
	
	/* If str is null. */
	if(NULL == str)
		return (EM_enErrRetModularTag | EM_ErrRetTag_NULLPointerErr);

	/* If TagType is not string. */
	if(!TAG_IS_STR(mTagType)) 
		return (EM_enErrRetModularTag | EM_ErrRetTag_TagTypeErr);
	
	//Normal Page don't excute set error message tag actions.
	if((ErrorManagement_PageID != WorkingPageID) && (SYSTEM_ERROR_MESSAGE_TAG == this->mRegID))
	{
		return (EM_enErrRetModularTag | EM_ErrRetTag_OpSysTagErr);
	}

	//ErrorManagement_Page don't excute set normal tag actions.
	if((ErrorManagement_PageID == WorkingPageID) && (SYSTEM_ERROR_MESSAGE_TAG != this->mRegID))
	{
		return (EM_enErrRetModularTag | EM_ErrRetTag_OpSysTagErr);
	}

	int i;
	u16 strlength = this->mValueLen;
	u8* curStr = gTagValueSpacePtr + this->mAddr;
	for (i = 0; i < strlength; i++)
	{
		if (str[i] != curStr[i])
		{
			dif_flag = 1;
			break;
		}
	}
	
	if(dif_flag)
	{
		for (i = 0; i < strlength; i++)
		{
			if ((str[i]!='\0') || (i == 0))
			{
				*curStr++ = str[i];
			}else{
				break;
			}
		}
	
		for(;i < strlength;i++){
			*curStr++ = '\0';
		}
	
		/* Update binding Elements. */
		if(update)
		{
			if(isBeforeLoadAction)
			{
				this->setBindingElement();
			}
			else
			{
				setStringTrigger(this, bNeedSyn);
			}
//			EnterSafeSectionForTag();
//			this->setBindingElement();
//			ExitSafeSectionForTag();
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function: setStringForStoreStringTag
// @brief:set string to the tag.
// parameters:
//   @param1 char*  str         pointer to the string
//-----------------------------
u32 TagClass::setStringForStoreStringTag(char* str, u8 update)
{
	uint8_t dif_flag = 0;
	
	/* If str is null. */
	if(NULL == str)
		return (EM_enErrRetModularTag | EM_ErrRetTag_NULLPointerErr);

	/* If TagType is not string. */
	if(!TAG_IS_STR(mTagType)) 
		return (EM_enErrRetModularTag | EM_ErrRetTag_TagTypeErr);
	
	//Normal Page don't excute set error message tag actions.
	if((ErrorManagement_PageID != WorkingPageID) && (SYSTEM_ERROR_MESSAGE_TAG == this->mRegID))
	{
		return (EM_enErrRetModularTag | EM_ErrRetTag_OpSysTagErr);
	}

	//ErrorManagement_Page don't excute set normal tag actions.
	if((ErrorManagement_PageID == WorkingPageID) && (SYSTEM_ERROR_MESSAGE_TAG != this->mRegID))
	{
		return (EM_enErrRetModularTag | EM_ErrRetTag_OpSysTagErr);
	}

	int i;
	u16 strlength = this->mValueLen;
	u8* curStr = gTagValueSpacePtr + this->mAddr;
	for (i = 0; i < strlength; i++)
	{
		if (str[i] != curStr[i])
		{
			dif_flag = 1;
			break;
		}
	}
	
	if(dif_flag)
	{
		for (i = 0; i < strlength; i++)
		{
			if ((str[i]!='\0') || (i == 0))
			{
				*curStr++ = str[i];
			}else{
				break;
			}
		}
	
		for(;i < strlength;i++){
			*curStr++ = '\0';
		}
	
		/* Update binding Elements. */
		if(update)
		{
			//EnterSafeSectionForTag();
			this->setBindingElement();
			//ExitSafeSectionForTag();
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function: setString
// @brief:set string to the tag.
// parameters:
//   @param1 char*  str         pointer to the string
//   @param2 uint32_t size      size of source string
//-----------------------------
u32 TagClass::setString(char* str, u16 size, u8 update, bool bNeedSyn, bool isBeforeLoadAction)
{
    int i;
    u8* curStr;
    u16 strLength;
    u16 updateLength;
    uint8_t dif_flag = 0;

	/* If str is null. */
	if(NULL == str)
		return (EM_enErrRetModularTag | EM_ErrRetTag_NULLPointerErr);

	/* If TagType is not string. */
	if(!TAG_IS_STR(mTagType)) 
		return (EM_enErrRetModularTag | EM_ErrRetTag_TagTypeErr);
	
	//Normal Page don't excute set error message tag actions.
	if((ErrorManagement_PageID != WorkingPageID) && (SYSTEM_ERROR_MESSAGE_TAG == this->mRegID))
	{
		return (EM_enErrRetModularTag | EM_ErrRetTag_OpSysTagErr);
	}

	//ErrorManagement_Page don't excute set normal tag actions.
	if((ErrorManagement_PageID == WorkingPageID) && (SYSTEM_ERROR_MESSAGE_TAG != this->mRegID))
	{
		return (EM_enErrRetModularTag | EM_ErrRetTag_OpSysTagErr);
	}
	
	strLength = this->mValueLen;
    
    updateLength = size;
    
    if(strLength < size)
	{
        updateLength = strLength;
	}
    
	curStr = gTagValueSpacePtr + this->mAddr;
    
	for (i = 0; i < updateLength; i++)
	{
		if (str[i] != curStr[i])
		{
			dif_flag = 1;
			break;
		}
	}
	
	if(dif_flag)
	{
		for (i = 0; i < updateLength; i++)
		{
			if ((str[i]!='\0') || (i == 0))
			{
				*curStr++ = str[i];
			}else{
				break;
			}
		}
	
		for(;i < strLength;i++){
			*curStr++ = '\0';
		}
	
		/* Update binding Elements. */
		if(update)
		{
			if(isBeforeLoadAction)
			{
				this->setBindingElement();
			}
			else
			{
				setStringTrigger(this, bNeedSyn);
			}
//			EnterSafeSectionForTag();
//			this->setBindingElement();
//			ExitSafeSectionForTag();
		}
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function: getString
// @brief:get string from the tag.
// parameters:
//   @param1 char*  str         pointer to the string
//-----------------------------
u32 TagClass::getString(char* str)
{
	/* If str is null. */
	if(NULL == str)
		return (EM_enErrRetModularTag | EM_ErrRetTag_NULLPointerErr);

	/* If TagType is not string. */
	if(!TAG_IS_STR(mTagType)) 
		return (EM_enErrRetModularTag | EM_ErrRetTag_TagTypeErr);
	
	int i;
	u16 strlength = this->mValueLen;
	u8* curStr = gTagValueSpacePtr + this->mAddr;
	for (i = 0; i < strlength; i++)
	{
		if ((*curStr) != '\0')
		{
			str[i] = *curStr++;
		}else{
			break;
		}
	}

	for(;i < strlength;i++){
		str[i] = '\0';
	}
	
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function: getStringLength
// @brief:get string length from the tag.
// parameters:
//   @param1 None
//-----------------------------
u16 TagClass::getStringLength(void)
{
	if(!TAG_IS_STR(mTagType))
		return 0;

	return this->mValueLen;
}

u8* TagClass::getStringValue()
{
	if(!TAG_IS_STR(mTagType))
		return 0;

	return (gTagValueSpacePtr + this->mAddr);
}

//-----------------------------
// function: setArray
// @brief:set array v to the tag.
// parameters:
//   @param1 char*  str         pointer to the string
//   @param2 uint32_t size      size of source string
//-----------------------------
void TagClass::setArray(u8* arr,  u8 update, s16 size, u8 foreceArrElemLen)
{
    int i,j;
    u8* curArr;
    u16 arrLength;
    u16 updateLength;
	u8 tagElemLen;
	u8 elemSign;
	u32 value;
	s32 sValue;

	if(( !TAG_IS_ARR(mTagType) ) || (NULL == arr)) 
		return;
	
	//Normal Page don't excute set error message tag actions.
	if((ErrorManagement_PageID != WorkingPageID) && (SYSTEM_ERROR_MESSAGE_TAG == this->mRegID))
	{
		return;
	}

	//ErrorManagement_Page don't excute set normal tag actions.
	if((ErrorManagement_PageID == WorkingPageID) && (SYSTEM_ERROR_MESSAGE_TAG != this->mRegID))
	{
		return;
	}
	tagElemLen = this->getElemByteLength();
	arrLength = this->mValueLen/tagElemLen;
	elemSign = this->getElemSign();
    
    updateLength = size;
    
	if ((s16)updateLength == -1)
	{
		updateLength = arrLength;
	}
	else
	{
		if (foreceArrElemLen != 0)
		{
			updateLength =  updateLength/foreceArrElemLen;
		}
		else
		{
			updateLength = updateLength/tagElemLen;
		}
	}

    if(arrLength < updateLength)
        updateLength = arrLength;

	
	
	
    this->mCurArryElemNum = updateLength;
	curArr = gTagValueSpacePtr + this->mAddr;
    
	
	
	for (i = 0; i < updateLength; i++)
	{
		
		if (foreceArrElemLen != 0 && foreceArrElemLen != tagElemLen)
		{
			if (tagElemLen > foreceArrElemLen)
			{
				for (j = 0; j < tagElemLen; j++)
				{
					if (j+foreceArrElemLen-tagElemLen < 0)
					{
						curArr[i*tagElemLen+j] = 0;
					}
					else
					{
						curArr[i*tagElemLen+j] = arr[i*foreceArrElemLen+j+foreceArrElemLen-tagElemLen];
					}
					
				}
			}
			else
			{
				//get u32 value
				value = 0;
				for(int k = 0; k < foreceArrElemLen; k++)
				{
					value |= (arr[i*foreceArrElemLen+k] << (8 * (foreceArrElemLen - k - 1)));
				}
				if (elemSign == 1)
				{
					//sign
					sValue = value;
					switch(tagElemLen)
					{
						case 1:
							if(sValue > 127)
								value = 127;
							if(sValue < -128)
								value = -128;
						break;
						case 2:
							if(sValue > 65534)
								value = 65534;
							if(sValue < -65535)
								value = -65535;
						break;
					}
				}
				else
				{
					switch(tagElemLen)
					{
						case 1:
							if(value > 0xFF)
								value = 0xFF;
						break;
						case 2:
							if(value > 0xFFFF)
								value = 0xFFFF;
						break;
					}
				}
				

				//write to curArr
				for (j = tagElemLen; j > 0; j--)
				{
					curArr[i*tagElemLen+(tagElemLen - j)] = (value >> (8 * (j - 1)));
				}
			}
		}
		else
		{
			for (j = 0; j < tagElemLen; j++)
			{
				curArr[i*tagElemLen+j] = arr[i*tagElemLen+j];
			}
		}
	}


	/* Update binding Elements. */
	if(update)
	{
		setStringTrigger(this);
//		EnterSafeSectionForTag();
//		this->setBindingElement();
//		ExitSafeSectionForTag();
	}
}

u8 TagClass::getElemByteLength()
{
	return (this->mTagType >> 8) & 0x7F;
}

u8 TagClass::getElemSign()
{
	return (this->mTagType >> 15);
}

u16 TagClass::getArrayLength(void)
{
	if(!TAG_IS_ARR(mTagType))
		return 0;

	return this->mValueLen;
}

u16 TagClass::getCurArryElemNum(void)
{
	if(!TAG_IS_ARR(mTagType))
		return 0;

	return this->mCurArryElemNum;
}

u8* TagClass::getArrayValue(){
	if(!TAG_IS_ARR(mTagType))
		return 0;
	
	return (gTagValueSpacePtr + this->mAddr);
	
}

s32 TagClass::getArrayS32Value(u16 idx)
{
	u32 value;
	u8 sign;
	s8 valueS8;
	s16 valueS16;
	s32 valueS32;
	int j;

	if(!TAG_IS_ARR(mTagType))
		return 0;

	u8 dataByteLen = this->getElemByteLength();
	u16 arrayElemNum = this->getCurArryElemNum();
	if (idx >= arrayElemNum)
	{
		//error, out of range
		return 0;
	}
	u8* data = this->getArrayValue() + dataByteLen * idx;
	sign = this->getElemSign();
	value = 0;
	for (j = 0; j < dataByteLen; j++)
	{
		value = (value << 8) + data[j];
	}
	if (sign)
	{
		switch (dataByteLen)
		{
		case 4:
			valueS32 = value;
			return valueS32;
		case 2:
			valueS16 = (u16)value;
			return valueS16;
		default:
			//1
			valueS8 = (u8)value;
			return valueS8;
			break;
		}
	}
	else
	{
		return (s32)value;
	}
}
#if defined(CHIP_GC9002)
#include "csi_ahmi.h"
extern void SetPartialDrawRefreMsg(RefreshMsg *msg);
#endif
//-----------------------------
// function:setBindingElement
// @brief:Update elements binded to the tag.
//-----------------------------
u32 TagClass::setBindingElement()
{
	
	WidgetClassPtr pLinkedWidgetPtr;
	CanvasClassPtr pLinkedCanvas;
	SubCanvasClassPtr pFocusedSubcanvasPtr;
	ElemenLinkDataPtr pElementLinker;
	WidgetLinkDataPtr pWidgetLinker;
	CanvasLinkDataPtr pCanvasLinker;
	PageLinkDataPtr   pPageLinker  ;
    u32 tagValue;
    u32 result = NullRefreshFlag;
	ActionTriggerClass gWidgetTagTrigger;
	u16 CanvasID, SubcanvasID, WidgetID;
    u16 id;
	u8 u8_widgetRefresh;
	
#ifdef AHMI_DEBUG
	char text[100];
#endif
	
//	taskENTER_CRITICAL();
    /* Check all binding elements one by one. */
	for(id=0;id<this->mNumOfElementLinker;id++)
	{
		pElementLinker = &(this->pTagELmementLinker[id]);

        /* The type of the element is widget. */
		if(pElementLinker->mElementType == ELEMENT_TYPE_WIDGET)
		{
//            taskENTER_CRITICAL();
            
			pWidgetLinker = (WidgetLinkDataPtr)(pElementLinker->mLinkElementPtr);
			if(pWidgetLinker->LinkWidgetPageID == WorkingPageID) 
			{
				gWidgetTagTrigger.mInputType = ACTION_TAG_SET_VALUE;
				gWidgetTagTrigger.mTagPtr = this;
                /* If the widget on the focused subcanvas. */
				pLinkedWidgetPtr = &(gPagePtr[WorkingPageID].pWidgetList[pWidgetLinker->LinkWidgetID]);   //widget
				pLinkedWidgetPtr->getWidgetIndex(&gPagePtr[WorkingPageID], &CanvasID, &SubcanvasID, &WidgetID);
				pLinkedCanvas = &(gPagePtr[WorkingPageID].pCanvasList[pLinkedWidgetPtr->ATTATCH_CANVAS]); //widgetָ���canvas
				pFocusedSubcanvasPtr = &(gPagePtr[WorkingPageID].pSubCanvasList[(pLinkedCanvas->mStartAddrOfSubCanvas) + SubcanvasID]);  //canvasָ���subcanvas
				if(NULL == pFocusedSubcanvasPtr)
				{
//					taskEXIT_CRITICAL();
					return (EM_enErrRetModularTag | EM_ErrRetTag_NULLPointerErr);
				}

				if( (pWidgetLinker->LinkWidgetID) >= (pFocusedSubcanvasPtr->StartAddrOfWidget) && ((pWidgetLinker->LinkWidgetID) < (pFocusedSubcanvasPtr->StartAddrOfWidget + pFocusedSubcanvasPtr->NumOfWidget) ) )//��subcanvas��widget��Χ��
                    u8_widgetRefresh = 1;//Refresh the widget.
                else
                    u8_widgetRefresh = 0;//Do not refresh the widget.
                
 				gWidgetTagTrigger.widgetActionTrigger(&gPagePtr[WorkingPageID],pLinkedWidgetPtr,u8_widgetRefresh);
                
                result |= WidgetNormalRefreshFlag;

			}
            
//            taskEXIT_CRITICAL();
		} /* end of widget linker. */ 
		/* The type of the element is cancas. */
		else if(pElementLinker->mElementType == ELEMENT_TYPE_CANVAS) //CANVAS
		{
//            taskENTER_CRITICAL();
            
			pCanvasLinker = (CanvasLinkDataPtr)(pElementLinker->mLinkElementPtr);
			if(pCanvasLinker->LinkCanvasPageID == WorkingPageID)
			{
				gWidgetTagTrigger.mInputType = ACTION_TAG_SET_VALUE;
				gWidgetTagTrigger.mTagPtr = this;
				pLinkedCanvas = &gPagePtr[WorkingPageID].pCanvasList[pCanvasLinker->LinkCanvasID];
				pLinkedCanvas->CanvasState = 1; //for subcanvas animation
				CanvasActionTrigger(pLinkedCanvas, &gWidgetTagTrigger);
                result |= WidgetNormalRefreshFlag;
				
				#if defined(CHIP_GC9002)	
				if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
				{
					RefreshMsg refreshMsg;
					refreshMsg.mElementType = ANIMATION_REFRESH_CANVAS;
					refreshMsg.mElementPtr.cptr = pLinkedCanvas;
					SetPartialDrawRefreMsg(&refreshMsg);
				}
				#endif
			}
            
//            taskEXIT_CRITICAL();
		}/* end of canvas linker. */
		/* The type of the element is page. */
		else if(pElementLinker->mElementType == ELEMENT_TYPE_PAGE) //page
		{
//            taskENTER_CRITICAL();
            
			pPageLinker = (PageLinkDataPtr)(pElementLinker->mLinkElementPtr);
			if(pPageLinker->LinkDynamicPageID == WorkingPageID)
			{
				PageActionTrigger(&gPagePtr[WorkingPageID],pPageLinker->LinkActionIDInDynamicPage);
			}
            
//            taskEXIT_CRITICAL();
		}/* end of page linker */
        /* The type of the element is keyboard. */
		else if(pElementLinker->mElementType == ELEMENT_TYPE_KEYBOARD)
		{
//            taskENTER_CRITICAL();
            
			tagValue = getValue();
			if(tagValue == AHMI_KEYBOARD_LEFT || tagValue == AHMI_KEYBOARD_UP)
			{
#ifdef AHMI_DEBUG
				WriteText(text,"triggering keyboard left.\r\n");
				ERROR_PRINT(text);
#endif
				keyboardLeft();
			}
			else if(tagValue == AHMI_KEYBOARD_RIGHT || tagValue == AHMI_KEYBOARD_DOWN)
			{
#ifdef AHMI_DEBUG
				WriteText(text,"triggering keyboard right.\r\n");
				ERROR_PRINT(text);
#endif
				keyboardRight();
			}	
			else if(tagValue == AHMI_KEYBOARD_OK)
			{
#ifdef AHMI_DEBUG
				WriteText(text,"triggering keyboard OK.\r\n");
				ERROR_PRINT(text);
#endif
				keyboardEntered();
			}
			else if(tagValue == AHMI_KEYBOARD_OK_PRESS)
			{
#ifdef AHMI_DEBUG
				WriteText(text,"triggering keyboard OK PRESS.\r\n");
				ERROR_PRINT(text);
#endif
				keyboardEnteredPress();
			}
			else if(tagValue == AHMI_KEYBOARD_CLEAR)
			{
#ifdef AHMI_DEBUG
				WriteText(text,"triggering keyboard Clear.\r\n");
				ERROR_PRINT(text);
#endif
				keyboardClear();
			}
			else 
			{
				ERROR_PRINT("ERROR in setting keyboard tag: connot find the keyboard value");
			}
            
            result |= WidgetNormalRefreshFlag;
			
			#if defined(CHIP_GC9002)	
			if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
			{
				RefreshMsg refreshMsg;
				refreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
				refreshMsg.mElementPtr.cptr = pLinkedCanvas;
				SetPartialDrawRefreMsg(&refreshMsg);
			}
			#endif
            
//            taskEXIT_CRITICAL();
		}/* end of keyboard linker. */
        /* The type of the element is GOTO page. */
		else if(pElementLinker->mElementType == ELEMENT_TYPE_PAGE_GOTO)
		{
            
#ifdef EMBEDDED        //touch calibration   
//            taskENTER_CRITICAL();
            
            if((TouchScreenType > TOUCHSCREENOFF) && (TouchScreenType < TOUCHSCREENCAPACITIVE))
            {
                if(this->getValue() == (u32)(TouchCalibration_PageID + 1) || (this->getValue() == (u32)0xFFFFFFFD))  //0xFFFFFFFD = -3
                {
                    updateTagValueToValueSpace(TouchCalibration_PageID + 1);
                    
                    ResetCalibrationflag();
                }
            }
            
//            if((TouchScreenType > TOUCHSCREENOFF) && (TouchScreenType < TOUCHSCREENCAPACITIVE))
//            {
//                if(GetCalibrationflag() != 1)
//                    return;
//            }
            
//            taskEXIT_CRITICAL();
#endif //end touch calibration   
            tagValue = getValue();
//#ifdef PC_SIM
			if((tagValue >= 1) && (tagValue <= ConfigData.NumofPages))
//#else
//			if((tagValue >= 1) && (tagValue <= ConfigData.NumofPages) && (WorkingPageID != (TouchCalibration_PageID)))
//#endif
			{
				gAction.GotoPageExe(tagValue - 1, 0, 0);
                
                result |= PageNormalRefreshFlag;
				
				#if defined(CHIP_GC9002)	
				if(GetAHMIParticalDrawType() != AHMI_PARTIAL_DRAW_NONE)
				{
					RefreshMsg refreshMsg;
					refreshMsg.mElementType = ANIMATION_REFRESH_PAGE;
					refreshMsg.mElementPtr.cptr = pLinkedCanvas;
					SetPartialDrawRefreMsg(&refreshMsg);
				}
				#endif
			}
			else
			{
				if(WorkingPageID != (TouchCalibration_PageID))
				{
					ERROR_PRINT("ERROR when setting page tag binding element: cannot goto this page");
				}
			}
		}/* end of GOTO page linker. */
	} /* end of linker. */
//	taskEXIT_CRITICAL();
	
	return result;
}

//-----------------------------
//function: PageActionTrigger
// @brief:trigger page action
//parameters:
// @param1 PagePtr              pointer to page
// @param2 TriggerAction        address to action    
//-----------------------------
funcStatus TagClass::PageActionTrigger(
		DynamicPageClassPtr Ptr,
		u16 TriggerAction
		)
{
	u32 addr;
	addr = (u32)(Ptr->pActionInstructions + Ptr->pActionStartADDR[TriggerAction]);

	csi_kernel_msgq_put(ActionInstructionQueue,&addr,0,portMAX_DELAY);
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function: CanvasActionTrigger
// @brief:trigger page action
// parameters:
// @param1 mCanvas              pointer to canvas
// @param2 TriggerAction        address to action  
//-----------------------------
funcStatus TagClass::CanvasActionTrigger(
		CanvasClassPtr mCanvas,
		ActionTriggerClassPtr canvasTagTrigger
		)
{
	/* Change focused subvavas by change a spicial tag's value. */
	u32 value;
	if(canvasTagTrigger->mInputType == ACTION_TAG_SET_VALUE)
	{
		value = canvasTagTrigger->mTagPtr->getValue();
//		taskENTER_CRITICAL();
		if(value < mCanvas->mNumOfSubCanvas)
		{
			mCanvas->focusNewSubCanvas((u8)value);
		}
		else
		{
			mCanvas->focusNewSubCanvas((u8)(mCanvas->mNumOfSubCanvas - 1));
		}
//		taskEXIT_CRITICAL();
	}
	return AHMI_FUNC_SUCCESS;
}

void tickWaveFilterStatus(void* _arg){
	WaveFilterTickArg* arg = (WaveFilterTickArg*)(_arg);
	int frequence = arg->waveFilter->getFrequence();
	int intervalTime = 1000/frequence;
	arg->waveFilter->filter(arg->status);
	//set tag
	arg->tag->setValueTrigger(arg->tag, arg->status->last);
	arg->totalDuration += intervalTime;
	if ((arg->totalDuration >= arg->waveFilter->getDuration()) || (arg->status->last == arg->status->target))
	{
		gIntervalManager.clearInterval(arg->intervalID);
	}
}

int getIntervalInUsedByTag(TagClassPtr _tag){
	WaveFilterTickArg* arg;
	for (int i = 0; i < IntervalMaxNum; i++)
	{
		if (gIntervalManager.intervals[i].arg!=NULL)
		{
			arg = (WaveFilterTickArg*)(gIntervalManager.intervals[i].arg);
			if (arg->tag == _tag)
			{
				return i;
			}
		}
	}
	return -1;
	
}
#endif

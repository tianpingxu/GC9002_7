////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMIÐ¡×é³ÉÔ±
//
// Create Date:   2015/11/17
// File Name:     Tagclass.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/17 by...
// Revision 2.04 - C++, 20160321 by Yu Chunying
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef   TAGCLASS__H
#define   TAGCLASS__H

#include "AHMIBasicStructure.h"
#include "publicType.h"
#include "tagClass_cd.h"
#include "tagClass_c1.h"
#include "publicDefine.h"
#ifdef AHMI_CORE

typedef enum
{
    EM_ErrRetTag_NULLPointerErr = 1,
	EM_ErrRetTag_TagIDOverrun,
	EM_ErrRetTag_TagTypeErr,
	EM_ErrRetTag_OpSysTagErr,
	EM_ErrRetTag_QueueSendErr
   
}EM_ErrRetTag;

class TagClass
{
public:
	ElemenLinkDataPtr pTagELmementLinker; //linked elements list.
	u32 mRegID; 				//linked reg, 20160904
	//u32 mValue;
	u32 mStoreID;
	u16 mNumOfElementLinker; 	//number of linked elements.
	u16	mTagType;				//Type, 0: int, 1:string,type[5:1] is the size of string .
	u16 mAddr;
	u16 mValueLen;
	u16 mCurArryElemNum;			//current array element number, only used in arrayTag.

	TagClass();
	~TagClass();

	//void initTagStr(u32 tagStrIdx);//tagStrIdx:idx of tag string to replace mValue
	TagClass& getTag(u16 tagID);
	u32 	getTagID(u32 regID);
	u32		getValue() const;
	u32		setValue(u32 v, u16 tagID, bool bNeedSyn = false, bool isBeforeLoadAction = false);
	u32 	setRawValue(u32 v);
	void	updateTagValueToValueSpace(u32 v);
	u32		setString(char* str, u8 update, bool bNeedSyn = false, bool isBeforeLoadAction = false);//update binding Elements
	u32		setStringForStoreStringTag(char* str, u8 update);
    u32		setString(char* str, u16 size, u8 update, bool bNeedSyn = false, bool isBeforeLoadAction = false);//update binding Elements
    u32 	getString(char* str);
	u16		getStringLength(void);
	u32		setBindingElement();
	u32	initTag(
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
			);
            
	u8 * getStringValue();

	void	setArray(u8* data, u8 update, s16 size = -1, u8 forceDataElemLen = 0);//update binding Elements

	u8 getElemByteLength();
	u8 getElemSign();
	u16 getArrayLength(void);
	u16 getCurArryElemNum(void);
	u8 * getArrayValue();
	s32 getArrayS32Value(u16 idx);

	funcStatus PageActionTrigger(
		DynamicPageClassPtr Ptr,
		u16 TriggerAction
		);

	funcStatus CanvasActionTrigger(
		CanvasClassPtr mCanvas,
		ActionTriggerClassPtr canvasTagTrigger
		);
        
    funcStatus setValueTrigger(TagClass* pTag, uint32_t value, bool bNeedSyn = false);
    
    funcStatus setStringTrigger(TagClass* pTag, bool bNeedSyn = false);

};

typedef TagClass* TagClassPtr;


void tickWaveFilterStatus(void* _arg);

int getIntervalInUsedByTag(TagClassPtr _tag);

#endif

#endif


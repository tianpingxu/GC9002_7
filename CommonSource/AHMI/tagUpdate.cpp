////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI
//
// Create Date:   2015/11/30
// File Name:     tagUpdate.c
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/30
// Revision 2.02 - add the initTagUpdateQueue function by Arthas 20151207
// Revision 2.04 - C++, 20160321 by ???
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#include "publicInclude.h"
#include "AHMIBasicDefine.h"

#define TAG_UPDATE_TIME 20
#ifdef AHMI_CORE
extern  ConfigInfoClass      ConfigData;
extern  TagUpdateClassPtr    TagUpdatePtr;
extern u8*                 TagChangedListPtr;
extern TagClassPtr     TagPtr;


TagUpdateClass::TagUpdateClass()
{}

TagUpdateClass::~TagUpdateClass()
{}

//-----------------------------
//function:initTagUpdateQueue
// @brief:called by draw function
//parameters:
//   @param1 u16 NumofTagUpdateQueue          number of the TagUpdateQueue's element.
//   @param2 u16 NumofTags                    number of the TagUpdateQueue's tag.
//   @param3 TagClassPtr tagPtr               pointer to the tag array.
//   @param4 TagUpdateStructPtr TagUpdatePtr  pointer to the TagUpdate array.
//-----------------------------
void initTagUpdateQueue(u16 NumofTagUpdateQueue,u16 NumofTags, TagClassPtr tagPtr, TagUpdateClassPtr TagUpdatePtr)
{
	int ID;
	int curTag = 0;
	u8 mTagNeedUpdate;

	if((0 == NumofTagUpdateQueue) || (0 == NumofTags) || (NULL == tagPtr) || (NULL == TagUpdatePtr))
		return;

	for(ID = 0; ID < NumofTagUpdateQueue; ID++)
    {
		for(;curTag < NumofTags; curTag++)
		{
			mTagNeedUpdate = tagPtr[curTag].mTagType & TAG_READ_REG;
			if(mTagNeedUpdate)
			{
				if(tagPtr)
				{
					TagUpdatePtr[ID].mRegID = tagPtr[curTag].mRegID;
					TagUpdatePtr[ID].mTagID = curTag;
					curTag++;
					break;
				}
			}
		}
    }

	/* Sort the TagUpdate array. */
	sortTagUpdateClass(NumofTagUpdateQueue,TagUpdatePtr);
}


//-----------------------------
// function: UpdateAllTags.
// @brief: Update all the tags' binding elements.
// parameters:
//   void
//-----------------------------
void UpdateAllTags(void)
{
    uint32_t tagChangedValue = 0;
	u16 tagID = 0;
 
    /* Scan all tags one by one. */
    for(tagID = 0; tagID < ConfigData.NumofTags; tagID++)
	{
//        taskENTER_CRITICAL();

        /* If this tag has be changed. */
		if(TagChangedListPtr[tagID] != 0)
		{
            tagChangedValue = TagChangedListPtr[tagID];

            /* Refresh all the binding elements. */
			TagPtr[tagID].setBindingElement();
            
            /* If value of the tag was not be changed when setBindingElement. */
            if(tagChangedValue == TagChangedListPtr[tagID])
            {
                TagChangedListPtr[tagID] = 0;
            }
            else
            {
                TagChangedListPtr[tagID] = 1;
            }
		}

//        taskEXIT_CRITICAL();
	}
}

//-----------------------------
// function: sortTagUpdateStruct
// @brief: Sort the TagUpdate array by the value of mRegID.
// parameters:
//   @param1 u16 NumofTagUpdateQueue          number of the element.
//   @param2 TagUpdateStructPtr TagUpdatePtr  pointer to the TagUpdate array.
//   created by arthas 20160114
//-----------------------------
void sortTagUpdateClass(u16 NumofTagUpdateQueue, TagUpdateClassPtr TagUpdatePtr)
{
	int i,j;
	u16 min;
	TagUpdateClassPtr pTagUpdateClassTemp;
	TagUpdateClass tagUpdateClassTemp;

    if((0 == NumofTagUpdateQueue) || (NULL == TagUpdatePtr))
		return;

	pTagUpdateClassTemp = &tagUpdateClassTemp;

    /* Bubble Sort with the value of mRegID. */
	for(i = 0; i < NumofTagUpdateQueue - 1; i ++)
	{
		min = i;
		for(j = i + 1; j < NumofTagUpdateQueue; j ++)
		{
			if(TagUpdatePtr[j].mRegID < TagUpdatePtr[min].mRegID)
			{
				min = j;
			}
		}
		if(min != i)
		{
            /* Exchange the position of two element. */
			pTagUpdateClassTemp->mRegID = TagUpdatePtr[i].mRegID;
			TagUpdatePtr[i].mRegID = TagUpdatePtr[min].mRegID;
			TagUpdatePtr[min].mRegID = pTagUpdateClassTemp->mRegID;
			pTagUpdateClassTemp->mTagID = TagUpdatePtr[i].mTagID;
			TagUpdatePtr[i].mTagID = TagUpdatePtr[min].mTagID;
			TagUpdatePtr[min].mTagID = pTagUpdateClassTemp->mTagID;
		}
	}
}

#endif

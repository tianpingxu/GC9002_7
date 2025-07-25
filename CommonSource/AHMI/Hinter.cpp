#include "Hinter.h"
#include <string.h>

extern Hinter         gHinter;
extern HintClass*     gHintClass;
extern ConfigInfoClass ConfigData;
extern StringClassPtr       StringListPtr;
extern u8*					gStringPtr;
extern EncodingClass*		gEncoder;

HintClass::HintClass()
{

}

HintClass::~HintClass()
{

}

void HintClass::initHint(u32 keywordStringID, u32 hintStringID)
{
	this->keywordStringID = keywordStringID;
	this->hintStringID = hintStringID;
}

u8 matchString(u32 targetStringID, u8* sourceStr, u16 len)
{
	u16 i;
	u8 * targetStr = gStringPtr + StringListPtr[targetStringID].addr;
	if (len == 0)
	{
		for (i = 0;i < 32; i++)
		{
			if (sourceStr[i] == 0)
			{
				break;
			}else
			{
				len++;
			}
		}
	}

	if (len > StringListPtr[targetStringID].length)
	{
		return 0;
	}else if (len < StringListPtr[targetStringID].length)
	{
		for (i = 0;i < len; i++)
		{
			if (targetStr[i] != sourceStr[i])
			{
				return 0;
			}
		}
		//remaining all 0
		for (;i < StringListPtr[targetStringID].length; i++)
		{
			if (targetStr[i] != 0)
			{
				return 0;
			}
		}
	}else
	{
		//len equal
		for (i = 0;i < len; i++)
		{
			if (targetStr[i] != sourceStr[i])
			{
				return 0;
			}
		}
	}
	return 1;
}

int Hinter::getHintStringID(u8* curInput, u16 curInputLen)
{
	u16 idx;
	u32 keywordStringID;
	u32 hintStringID;
	u8 i;
	if (curInputLen == 0)
	{
		//calculate
		for (i = 0;i < 32; i++)
		{
			if (curInput[i] == 0)
			{
				break;
			}else
			{
				curInputLen++;
			}
		}
	}
	if (curInputLen == 0)
	{
		return -1;
	}
	for(idx = 0;idx < ConfigData.NumOfHints; idx++)
	{
		keywordStringID = (gHintClass + idx)->keywordStringID;
		hintStringID = (gHintClass + idx)->hintStringID;
		if (matchString(keywordStringID, curInput, curInputLen))
		{
			return hintStringID;
		}
	}
	return -1;
}

u8 Hinter::getHintWordsPages(u16 hintStringID, u8 pageSize)
{
	u16 totalWordCount = getHintWordsCount(hintStringID);
	return (totalWordCount+pageSize-1)/pageSize;
}

//get current hint string unicode num
u16 Hinter::getHintWordsCount(u16 hintStringID)
{
	u16 hintStrLen;
	u16 i;
	int nextCharBytes;
	u16 wordCount = 0;
	hintStrLen = StringListPtr[hintStringID].length;

	for (i = 0; i < hintStrLen; i++)
	{
		nextCharBytes = gEncoder->getNextCharacterBytesFromStr( gStringPtr + StringListPtr[hintStringID].addr + i,ConfigData.encoding);
		
		if (nextCharBytes<=0)
		{
			break;
		}else
		{
			wordCount++;
			i = i + nextCharBytes - 1;
		}
	}
	
	return wordCount;
}

//get current hint u8 by page, return offset and len in string u8 array
int Hinter::getHintWordsByPage(u16 hintStringID, u16 pageID, u32* offsetInString, u16 * len, u8 pageSize)
{
	u32 startAddrOffset;
	u32 endAddrOffset;
	u16 startNum;
	u16 i;
	int nextCharBytes;
	startAddrOffset = 0;
	endAddrOffset = 0;
	startNum = pageID * pageSize;
	for (i = 0; i < startNum; i++)
	{
		nextCharBytes = gEncoder->getNextCharacterBytesFromStr( gStringPtr + StringListPtr[hintStringID].addr + startAddrOffset,ConfigData.encoding);
		if (nextCharBytes<=0)
		{
			//no valid char
			*offsetInString = 0;
			*len = 0;
			return 0;
		}else
		{
			startAddrOffset += nextCharBytes;
		}
	}

	for (i = 0; i < pageSize; i++)
	{
		nextCharBytes = gEncoder->getNextCharacterBytesFromStr( gStringPtr + StringListPtr[hintStringID].addr + startAddrOffset + endAddrOffset,ConfigData.encoding);
		if (nextCharBytes<=0)
		{
			//no valid char
			*offsetInString = startAddrOffset;
			*len = endAddrOffset;

			return 1;
		}else
		{
			endAddrOffset += nextCharBytes;
		}
	}

	*offsetInString = startAddrOffset;
	*len = endAddrOffset;
	return 1;
}

//select current word, get corresponding offset and len in string u8 array
int Hinter::getHintWordByPageAndIndex(u16 hintStringID, u16 pageID, u8 idx, u32* offsetInString, u16 * len,u8 pageSize)
{

	u32 startAddrOffset;
//	u32 endAddrOffset;
	u16 startNum;
	u16 i;
	int nextCharBytes;
	startAddrOffset = 0;
//	endAddrOffset = 0;
	startNum = pageID * pageSize + idx;
	for (i = 0; i < startNum; i++)
	{
		nextCharBytes = gEncoder->getNextCharacterBytesFromStr( gStringPtr + StringListPtr[hintStringID].addr + startAddrOffset,ConfigData.encoding);
		if (nextCharBytes<=0)
		{
			//no valid char
			*offsetInString = 0;
			*len = 0;
			return 0;
		}else
		{
			startAddrOffset += nextCharBytes;
		}
	}

	nextCharBytes = gEncoder->getNextCharacterBytesFromStr( gStringPtr + StringListPtr[hintStringID].addr + startAddrOffset,ConfigData.encoding);
	if (nextCharBytes<=0)
	{
		//no valid char
		*offsetInString = startAddrOffset;
		*len = 0;

		return 0;
	}

	*offsetInString = startAddrOffset;
	*len = nextCharBytes;
	return 1;
}

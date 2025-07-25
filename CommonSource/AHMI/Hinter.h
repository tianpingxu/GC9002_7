#ifndef HINTER__H
#define HINTER__H

#include "publicDefine.h"
#include "publicInclude.h"

class HintClass
{
public:
	
	u32 keywordStringID;
	u32 hintStringID;//addr offset from gString

#ifndef PC_SIM_Gen
	HintClass();
	~HintClass();

	void initHint(u32 keywordStringID, u32 hintStringID);
#endif

};



class Hinter{
public:
	//match hint string by input
	int getHintStringID(u8* curInput, u16 curInputLen=0);

	//get current hint string unicode page count
	u8 getHintWordsPages(u16 hintStringID, u8 pageSize = 5);

	//get current hint string unicode num
	u16 getHintWordsCount(u16 hintStringID);

	//get current hint u8 by page, return offset and len in string u8 array
	int getHintWordsByPage(u16 hintStringID, u16 pageID, u32* offsetInString, u16 * len, u8 pageSize = 5);

	//select current word, get corresponding offset and len in string u8 array
	int getHintWordByPageAndIndex(u16 hintStringID, u16 pageID, u8 idx, u32* offsetInString, u16 * len, u8 pageSize = 5);
	
};



#endif

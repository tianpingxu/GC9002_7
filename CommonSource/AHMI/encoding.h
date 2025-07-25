#ifndef ENCODING__H
#define ENCODING__H

#include "publicDefine.h"

//class EncodingRange{
//public:
//	u32 start;
//	u32 end;
//
//	EncodingRange(u32 _start,u32 _end);
//};

typedef struct _EncodingRang{
	u32 start;
	u32 end;
}EncodingRange;

class FontRangeClass
{
public:
	u16 id;        // id of  string
	u16 rangeNum;
	u32 addr;//addr offset from gFontRange


};

void initEncodingRanges();

class EncodingClass{
public:

	int limitOfEachChunk;//num of char in each chunk

	//get total character by encoding
	int getCharacterNum(u8 encoding,bool full=false);


	//trans u8 mValue[32] to encodable characters by encoding
	void trasnU8ArrayToEncodableArray(u8 encoding,u8* toTransU8Array,u32* outputArray,int limit=32,int outputformat=0);

	//get next valide character bytes
	//-1 invalid
	int getNextCharacterBytesFromStr(u8* str, u8 encoding=ENCODING_ASCII);


	//trans utf-8 code to unicode
	u32 encodeUTF8ToUnicode(u8 * str,int length);

	//combine multi byte code to single u32 code
	//trans utf-8 code to unicode
	u32 getCodePointFromCharacterCode(u8 encoding,u8* str,int length);

	//get index in font lib for character
	u32 getCharacterIndexByCodePoint(u8 encoding,u32 codePoint, int fontRangeID);
};



#endif

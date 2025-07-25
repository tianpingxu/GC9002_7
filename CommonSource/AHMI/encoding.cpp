#include "encoding.h"
#include <string.h>
#ifdef NORMALIZE_ENABLE
	#include "normalizer.h"
#endif

FontRangeClass* FontRangeListPtr; //pointers of font ranges
EncodingRange* gRangesPtr;  //pointer of fontRange ranges

static EncodingRange ASCII_Range[] = {
	{0,127},{0,0}
};

const static EncodingRange GB2312_RANGE[95] = {
{0x0, 0x7f},
{0xa1a1, 0xa1fe},
{0xa2a1, 0xa2fe},
{0xa3a1, 0xa3fe},
{0xa4a1, 0xa4fe},
{0xa5a1, 0xa5fe},
{0xa6a1, 0xa6fe},
{0xa7a1, 0xa7fe},
{0xa8a1, 0xa8fe},
{0xa9a1, 0xa9fe},
{0xaaa1, 0xaafe},
{0xaba1, 0xabfe},
{0xaca1, 0xacfe},
{0xada1, 0xadfe},
{0xaea1, 0xaefe},
{0xafa1, 0xaffe},
{0xb0a1, 0xb0fe},
{0xb1a1, 0xb1fe},
{0xb2a1, 0xb2fe},
{0xb3a1, 0xb3fe},
{0xb4a1, 0xb4fe},
{0xb5a1, 0xb5fe},
{0xb6a1, 0xb6fe},
{0xb7a1, 0xb7fe},
{0xb8a1, 0xb8fe},
{0xb9a1, 0xb9fe},
{0xbaa1, 0xbafe},
{0xbba1, 0xbbfe},
{0xbca1, 0xbcfe},
{0xbda1, 0xbdfe},
{0xbea1, 0xbefe},
{0xbfa1, 0xbffe},
{0xc0a1, 0xc0fe},
{0xc1a1, 0xc1fe},
{0xc2a1, 0xc2fe},
{0xc3a1, 0xc3fe},
{0xc4a1, 0xc4fe},
{0xc5a1, 0xc5fe},
{0xc6a1, 0xc6fe},
{0xc7a1, 0xc7fe},
{0xc8a1, 0xc8fe},
{0xc9a1, 0xc9fe},
{0xcaa1, 0xcafe},
{0xcba1, 0xcbfe},
{0xcca1, 0xccfe},
{0xcda1, 0xcdfe},
{0xcea1, 0xcefe},
{0xcfa1, 0xcffe},
{0xd0a1, 0xd0fe},
{0xd1a1, 0xd1fe},
{0xd2a1, 0xd2fe},
{0xd3a1, 0xd3fe},
{0xd4a1, 0xd4fe},
{0xd5a1, 0xd5fe},
{0xd6a1, 0xd6fe},
{0xd7a1, 0xd7fe},
{0xd8a1, 0xd8fe},
{0xd9a1, 0xd9fe},
{0xdaa1, 0xdafe},
{0xdba1, 0xdbfe},
{0xdca1, 0xdcfe},
{0xdda1, 0xddfe},
{0xdea1, 0xdefe},
{0xdfa1, 0xdffe},
{0xe0a1, 0xe0fe},
{0xe1a1, 0xe1fe},
{0xe2a1, 0xe2fe},
{0xe3a1, 0xe3fe},
{0xe4a1, 0xe4fe},
{0xe5a1, 0xe5fe},
{0xe6a1, 0xe6fe},
{0xe7a1, 0xe7fe},
{0xe8a1, 0xe8fe},
{0xe9a1, 0xe9fe},
{0xeaa1, 0xeafe},
{0xeba1, 0xebfe},
{0xeca1, 0xecfe},
{0xeda1, 0xedfe},
{0xeea1, 0xeefe},
{0xefa1, 0xeffe},
{0xf0a1, 0xf0fe},
{0xf1a1, 0xf1fe},
{0xf2a1, 0xf2fe},
{0xf3a1, 0xf3fe},
{0xf4a1, 0xf4fe},
{0xf5a1, 0xf5fe},
{0xf6a1, 0xf6fe},
{0xf7a1, 0xf7fe},
{0xf8a1, 0xf8fe},
{0xf9a1, 0xf9fe},
{0xfaa1, 0xfafe},
{0xfba1, 0xfbfe},
{0xfca1, 0xfcfe},
{0xfda1, 0xfdfe},
{0xfea1, 0xfefe}
};



static EncodingRange UNICODE_RANGE[] = {
	{0,127},
	{0x4e00,0x9fff}
};

void initEncodingRanges(){
//	GB2312_RANGE[0].start = 0;
//	GB2312_RANGE[0].end = 127;
//	for (int i = 1; i <= 94; i++)
//	{
//		GB2312_RANGE[i].start = ((0xa0 + i)<<8) + 0xa0+1;
//		GB2312_RANGE[i].end = ((0xa0 + i)<<8) + 0xa0+94;
//	}
}

int EncodingClass::getCharacterNum(u8 encoding,bool full){
	if (full)
	{
		switch (encoding)
		{
		case ENCODING_UTF8:
			return 0x9fa5 - 0x4e00 +1 + 127;
//			break;
		case ENCODING_GB2312:
			return 94 * 94 + 127;
//			break;
		case ENCODING_ASCII:
		default:
			return 127;
//			break;
		}
	}else{
		return 127;
	}
	
}



void EncodingClass::trasnU8ArrayToEncodableArray(u8 encoding,u8* toTransU8Array,u32* outputArray,int limit,int outputformat){
	
	int pos = 0;
	int i = 0, j = 0;
	u32 tmp = 0;
#ifdef NORMALIZE_ENABLE
	Normalizer normalizer;
	u32 normalizeCodePoints[480] = {0};
	u16 normalizedLen = 0;
	int ret;
	int resultLimit = limit;
	limit = 480;
#endif
	
	for (i = 0; i < limit; i++)
	{
		int nextCharBytes = getNextCharacterBytesFromStr(&toTransU8Array[pos],encoding);
		if ((nextCharBytes > 0) && (toTransU8Array[pos] != 0))
		{
			outputArray[i] = getCodePointFromCharacterCode(encoding,&toTransU8Array[pos],nextCharBytes);
			pos += nextCharBytes;
		}else{
			break;
		}
	}

#ifdef NORMALIZE_ENABLE
	//do nfc
	if (encoding == ENCODING_UTF8)
	{
		limit = resultLimit;
		ret = normalizer.normalizeNFxC(outputArray,i, normalizeCodePoints, limit, &normalizedLen, NFC);
		if (ret != -1)
		{
			for (i = 0; i < normalizedLen; i++)
			{
				outputArray[i] = normalizeCodePoints[i];
			}
		}
		else
		{
			//error
			
			i = limit;
		}
	}
#endif

	if(outputformat)
	{
		for(j = 0; j < i/2; j++)
		{
			tmp = outputArray[j];
			outputArray[j] = outputArray[i-1-j];
			outputArray[i-1-j] = tmp;
		}
	}

	for (;i<limit;i++){
		outputArray[i] = 0;
	}
}

int EncodingClass::getNextCharacterBytesFromStr(u8* str, u8 encoding){
	int pos = 0;
	int numOfBytes = 0;
	u8 curChar;

	int strLength = 0;
	while(str[pos]!='\0'){
		strLength++;
		pos++;
	}
	pos = 0;
	switch (encoding)
	{
	case ENCODING_UTF8:
		if(str[pos]!='\0'){
			curChar = str[pos];
			
			if      ((curChar & 0x80) == 0)    numOfBytes = 1;
			else if ((curChar & 0xE0) == 0xC0) numOfBytes = 2;
			else if ((curChar & 0xF0) == 0xE0) numOfBytes = 3;
			else if ((curChar & 0xF8) == 0xF0) numOfBytes = 4;
			else return -1;

			if ( strLength  < numOfBytes) {
				return -1;
			}
			for (int i = 1; i < numOfBytes; ++i) {
			  if ((str[i] & 0xC0) != 0x80) {
				return -1;
			  }
			}
			
		}
		//valid utf-8
		
		break;
	case ENCODING_GB2312:
		if (str[pos]!='\0')
		{
			curChar = str[pos];
			if (/* curChar>=0 && */curChar < 128)
			{
				numOfBytes = 1;
			}else
			{
				//2 bytes
				if (strLength<2)
				{
					return -1;
				}
				//0xa1a1 - 0xfefe
				if (!(curChar>=0xa1 && curChar<=0xfe && str[pos+1]>=0xa1 && str[pos+1]<=0xfe))
				{
					return -1;
				}
				numOfBytes = 2;
			}
		}
        break;
	case ENCODING_ASCII:
	default:
		curChar = str[pos];
		if (/* curChar>=0 && */curChar < 128)
		{
			numOfBytes = 1;
		}else{
			return -1;
		}
		break;
	}
	return numOfBytes;

}


u32 EncodingClass::encodeUTF8ToUnicode(u8 * str,int length){
	if (length==1)
	{
		return str[0];
	}else if (length == 2)
	{
		return ((str[0]& 0x1F)<<6) + (str[1] & 0x3F);
	}else if (length == 3)
	{
		return ((str[0]& 0x0F)<<12) + ((str[1] & 0x3F)<<6) +(str[2] & 0x3F);
	}else if (length == 4)
	{
		return ((str[0]& 0x07)<<18) + ((str[1] & 0x3F)<<12) +((str[1] & 0x3F)<<6) +(str[2] & 0x3F);
	}else{
		return 0;
	}
	
}

u32 EncodingClass::getCodePointFromCharacterCode(u8 encoding,u8* str,int length){
	switch (encoding)
	{
	case ENCODING_UTF8:
		return encodeUTF8ToUnicode(str,length);
	case ENCODING_GB2312:
		if (length == 2)
		{
			return (str[0]<<8) + str[1];
		}else if (length == 1)
		{
			return str[0];
		}else{
			return 0;
		}
	case ENCODING_ASCII:
	default:
		if (length == 1)
		{
			return str[0];
		}else{
			return 0;
		}
//		break;
	}

}

u32 EncodingClass::getCharacterIndexByCodePoint(u8 encoding,u32 codePoint, int fontRangeID){
	EncodingRange const* ranges;
	int rangeNum;
	if (fontRangeID == -1)
	{
		switch (encoding)
		{
		case ENCODING_UTF8:
			ranges = UNICODE_RANGE;
			rangeNum = 2;
			break;
		case ENCODING_GB2312:
			ranges = GB2312_RANGE;
			rangeNum = 95;
			break;
		case ENCODING_ASCII:
		default:
			ranges = ASCII_Range;
			rangeNum = 1;
			break;
		}
	}
	else
	{
		ranges = gRangesPtr + FontRangeListPtr[fontRangeID].addr;
		rangeNum = FontRangeListPtr[fontRangeID].rangeNum;
	}
	
	u32 idx = 0;
	u32 idxOfRangeStart = 0;
	for (int i = 0; i < rangeNum; i++)
	{
		if(ranges[i].start > ranges[i].end){
			//invalid range
			return idx;
		}else{
			if (codePoint>=ranges[i].start && codePoint<=ranges[i].end)
			{
				//in this range
				return codePoint - ranges[i].start + idxOfRangeStart;
			}
		}
		//next range
		idxOfRangeStart += ranges[i].end - ranges[i].start + 1;
	}
	//not in ranges
	return idx;
}

#include "normalizer.h"
#include "publicInclude.h"

//UniChar Normalizer::fromCache(Strategy next, u16 cp, u8 needFeature)
//{
//	
//}

int binarySearch(FeatureKeyPair arr[], int size, int target) 
{
    int left = 0;
    int right = size - 1;
    
    while (left <= right) 
	{
        int mid = left + (right - left) / 2; // 计算中间位置

        // 如果找到目标元素，返回索引
		if (arr[mid].key == target)
		{
            return mid;
        }

        // 如果目标元素在左半部分，更新右边界
        if (arr[mid].key > target) 
		{
            right = mid - 1;
        }
        // 如果目标元素在右半部分，更新左边界
        else 
		{
            left = mid + 1;
        }
    }

    // 目标元素不在数组中，返回-1
    return -1;
}

int binarySearch(int arr[TCount][2], int size, int target) 
{
    int left = 0;
    int right = size - 1;
	int i;
	for (i = 0; i <= right; i++)
	{
		if (arr[i][0] == 0)
		{
			right = i;
			break;
		}
	}
    
    while (left <= right) 
	{
        int mid = left + (right - left) / 2; // 计算中间位置

        // 如果找到目标元素，返回索引
		if (arr[mid][0] == target)
		{
            return mid;
        }

        // 如果目标元素在左半部分，更新右边界
        if (arr[mid][0] > target) 
		{
            right = mid - 1;
        }
        // 如果目标元素在右半部分，更新左边界
        else 
		{
            left = mid + 1;
        }
    }

    // 目标元素不在数组中，返回-1
    return -1;
}

//template <typename T, typename K, size_t N>
//int binarySearchT(T (&arr)[N], K target) {
//    int left = 0;
//    int right = N - 1;
//    
//    while (left <= right) {
//        int mid = left + (right - left) / 2;
//        
//        if (arr[mid] == target) {
//            return mid;
//        }
//
//        if (arr[mid] < target) {
//            left = mid + 1;
//        } else {
//            right = mid - 1;
//        }
//    }
//    
//    return -1;  // 如果未找到目标
//}

int fromData(u32 cp, u8 needFeature, UniChar *curChar)
{
	/*var hash = cp & 0xFF00;
      var dunit = UChar.udata[hash] || {};
      var f = dunit[cp];
      return f ? new UChar(cp, f) : new UChar(cp, DEFAULT_FEATURE);*/
	int pos = binarySearch(uData, sizeof(uData)/sizeof(FeatureKeyPair), cp);
	
	curChar->codepoint = cp;
	if (pos != -1)
	{
		curChar->feature = uData[pos].feature;
	}
	else
	{
		curChar->feature = DEFAULT_FEATURE;
	}
	curChar->featureValid = 1;
	return 0;
}


int fromCpOnly(u32 cp, u8 needFeature, UniChar *curChar)
{
	if (needFeature)
	{
		return -1;
	}
	else
	{
		
		curChar->codepoint = cp;
		curChar->featureValid = 0;
		//curChar->feature = NULL_FEATURE;
		return 0;
	}
}

int fromRuleBasedJamo(u32 cp, u8 needFeature, UniChar *curChar)
{
	int j;
	int base;
	int SIndex;
	int TIndex;
    if(cp < LBase || (LBase + LCount <= cp && cp < SBase) || (SBase + SCount < cp))
	{
        return -1;
    }
	curChar->codepoint = cp;
    if(LBase <= cp && cp < LBase + LCount)
	{
        
		base = (cp - LBase) * VCount;
        for (j = 0; j < VCount; ++j)
		{
			curChar->feature.T[j][0] = VBase + j;
			curChar->feature.T[j][1] =  SBase + TCount * (j + base);
        }
		curChar->featureValid = 1;
        return 0;
    }

    SIndex = cp - SBase;
    TIndex = SIndex % TCount;
    
    if(TIndex != 0)
	{
        curChar->feature.S[0] = SBase + SIndex - TIndex;
		curChar->feature.S[1] = TBase + TIndex;
    } else {
     
		curChar->feature.S[0] = LBase + (SIndex / NCount);
		curChar->feature.S[1] = VBase + ((SIndex % NCount) / TCount);
        for (j = 1; j < TCount; ++j)
		{
			curChar->feature.T[j-1][0] = TBase + j;
			curChar->feature.T[j-1][1] =  cp + j;
        }
    }
	curChar->featureValid = 1;
    return 0;
}


int fromCpFilter(u32 cp, u8 needFeature, UniChar *curChar)
{
	if ((cp < 60) || ((13311 < cp) && (cp < 42607)))
	{
		curChar->codepoint = cp;
		curChar->feature = DEFAULT_FEATURE;
		curChar->featureValid = 1;
		return 0;
	}
	else
	{
		return -1;
	}
}

UniChar::UniChar()
{
	this->featureValid = 0;//set to false default
}

void UniChar::copyFrom(UniChar* srcChar)
{
	this->codepoint = srcChar->codepoint;
	this->featureValid = srcChar->featureValid;
	this->feature = srcChar->feature;
}

int UniChar::fromCharCode(u32 cp, u8 needFeature)
{
	int i;
	int ret;
	//set to null
	this->featureValid = 0;

	Strategy strategies[4] = {fromCpFilter, fromCpOnly, fromRuleBasedJamo, fromData};
	for(i=0;i<4;i++)
	{
		ret = strategies[i](cp, needFeature, this);
		if (ret != -1)
		{
			break;
		}
	}
	return ret;
}

u8 UniChar::isHighSurrogate(u32 cp)
{
	if(cp >= 0xD800 && cp <= 0xDBFF)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

u8 UniChar::isLowSurrogate(u32 cp)
{
	if(cp >= 0xDC00 && cp <= 0xDFFF)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int UniChar::prepFeature()
{
	if (this->featureValid == 0)
	{
		this->fromCharCode(this->codepoint, 1);
	}
	return 0;
}

int UniChar::getDecomp(int* S)
{
	int i;
	prepFeature();
	if (this->feature.S[0] == 0)
	{
		return -1;
	}
	else
	{
		for (i = 0; i < SELEMCount; i++)
		{
			S[i] = this->feature.S[i];
		}
		return 0;
	}
}

u8 UniChar::isCompatibility()
{
	prepFeature();
	u32 val;
	if (this->feature.L == 0)
	{
		return 0;
	}
	else
	{
		return (this->feature.L & (1 << 8))>>8;
	}
}

u8 UniChar::isExclude()
{
	prepFeature();
	if (this->feature.L == 0)
	{
		return 0;
	}
	else
	{
		return (this->feature.L & (1 << 9))>>9;
	}
}

u8 UniChar::getCanonicalClass()
{

	prepFeature();
	if (this->feature.L == 0)
	{
		return 0;
	}
	else
	{
		return this->feature.L & 0xff;
	}
}




int UniChar::getComposite(UniChar* compositeChar, UniChar* followingChar)
{
	int pos;
	prepFeature();
	if (this->featureValid == 0)
	{
		return -1;
	}
	else
	{
		pos = binarySearch(this->feature.T, TCount, followingChar->codepoint);
		if (pos != -1)
		{
			compositeChar->fromCharCode(this->feature.T[pos][1], 0);
			return 0;
		}
		else
		{
			return -1;
		}
	}
}

UniCharIterator::UniCharIterator(u32* arr, u16 len)
{
	this->unicodeArray = arr;
	this->unicodeArrayLen = len;
	this->cursor = 0;
}

int UniCharIterator::next(u32* nextCP)
{
	u32 d;
	u32 cp;
	UniChar uniChar;
	if(this->unicodeArrayLen > 0 && this->cursor < this->unicodeArrayLen)
	{
		cp = this->unicodeArray[this->cursor++];
		
		if(uniChar.isHighSurrogate(cp) && this->cursor < this->unicodeArrayLen)
		{
			d = this->unicodeArray[this->cursor];
			if ( uniChar.isLowSurrogate(d))
			{
				cp = (cp - 0xD800) * 0x400 + (d -0xDC00) + 0x10000;
				++this->cursor;
			}
            
		}
		*nextCP = cp;
		return 0;
    } 
	else 
	{
        return -1;
    }
}

RecursDecompIterator::RecursDecompIterator(UniCharIterator* it, u8 cano, UniChar* buf, u16 len)
{
	this->it = it;
	this->canonical = cano;
	this->resBuf = buf;
	this->resBufLen = len;
	this->resBufValidLen = 0;
	
	cursor = 0;
	skipCursor = 0;
	shouldSkipCount = 0;
	lastCP = 0;
}

int RecursDecompIterator::recursiveDecomp(u8 cano, UniChar* uniChar)
{
	int ret;
	int i;
	int decomp[SELEMCount];
	UniChar tempChar;
	ret = uniChar->getDecomp(decomp);

	if (ret != -1 && !(cano!=0 && uniChar->isCompatibility()!=0))
	{
		for (i = 0; i < SELEMCount; i++)
		{
			if (decomp[i]==0)
			{
				break;
			}
			else
			{
				tempChar.fromCharCode(decomp[i], 0);
				ret = recursiveDecomp(cano, &tempChar);
				if (ret == -2)
				{
					//overflow
					return -2;
				}
				else
				{
					//continue
				}
			}
		}
		return 0;
	}
	else
	{
		//set 
		if (shouldSkipCount>0 && skipCursor<shouldSkipCount)
		{
			//should skip this char
			skipCursor++;
			return 0;
		}
		else
		{
			if (resBufValidLen + 1 <= resBufLen)
			{
				resBuf[resBufValidLen] = *uniChar;
				resBufValidLen++;
				return 0;
			}
			else
			{
				//overflow
				return -2;
			}
		}
		
	}
}

int RecursDecompIterator::next(UniChar* nextChar)
{
	int ret;
	UniChar uniChar;
	u32 cp;
	if(resBufValidLen == 0 || cursor == resBufValidLen)
	{
		//consider should skip
		if (shouldSkipCount > 0)
		{
			ret = uniChar.fromCharCode(lastCP, 1);
		}
		else
		{
			ret = it->next(&cp);
			if(ret == -1)
			{
				return -1;
			}
			ret = uniChar.fromCharCode(cp,1);
		}
		
        
		
		resBufValidLen = 0;
		cursor = 0;
        ret = recursiveDecomp(canonical, &uniChar);
		if (ret == -2)
		{
			lastCP = uniChar.codepoint;
			shouldSkipCount += resBufLen;
			skipCursor = 0;
		}
		else
		{
			shouldSkipCount = 0;
			skipCursor = 0;
		}
    }
    *nextChar = resBuf[cursor++];
	return 0;
}

DecompIterator::DecompIterator(RecursDecompIterator* it, UniChar* buf, u16 len)
{
	this->it = it;
	this->resBuf = buf;
	this->resBufLen = len;
	this->resBufValidLen = 0;
	this->cursor = 0;
}

int DecompIterator::next(UniChar* nextChar)
{
	u8 cc;
	u8 cc2;
	UniChar uniChar;
	UniChar* uniChar2;
	int ret;
	int inspt;
	int i;
    if(this->resBufValidLen == 0 || cursor == resBufValidLen)
	{
		cursor = 0;
		resBufValidLen = 0;
        do{
			if (resBufValidLen + 1 == resBufLen)
			{
				//overflow
				//todo, print error
				return -1;
			}
			ret = this->it->next(&uniChar);
			if(ret == -1)
			{
				break;
			}
			cc = uniChar.getCanonicalClass();
			inspt = this->resBufValidLen;
			if(cc != 0)
			{
				for(; inspt > 0; --inspt)
				{
					uniChar2 = this->resBuf+(inspt - 1);
					cc2 = uniChar2->getCanonicalClass();
					if(cc2 <= cc)
					{
						break;
					}
				}
			}
			for (i = this->resBufValidLen+1;i>inspt;i--)
			{
				this->resBuf[i] = this->resBuf[i-1];
			}
			this->resBuf[inspt] = uniChar;
			this->resBufValidLen++;
			
        } while(cc != 0);
    }
 
	if (this->resBufValidLen > 0 && this->cursor < this->resBufValidLen)
	{
		*nextChar = this->resBuf[this->cursor++];
		return 0;
	}
	else
	{
		return -1;
	}
}

CompIterator::CompIterator(DecompIterator* it, u32* buf,u16 bufLen, UniChar* procBuf, u16 procBufLen  )
{
	this->it = it;
	this->resBuf = buf;
	this->resBufLen = bufLen;
	this->resBufValidLen = 0;

	this->procBuf = procBuf;
	this->procBufLen = procBufLen;
	this->procBufValidLen = 0;

	this->cursor = 0;
	this->lastClass = 0;
}

int CompIterator::next(u32* nextCharCP)
{
	UniChar uniChar;
	UniChar* startChar;
	UniChar compositeChar;
	int ret;
	int i;
	u8 cc;
	while(this->resBufValidLen == 0 || this->cursor == this->resBufValidLen)
	{
		this->resBufValidLen = 0;
		this->cursor = 0;

        ret = this->it->next(&uniChar);
        if(ret == -1){
			for(i=0;i<this->procBufValidLen;i++)
			{
				this->resBuf[i] = this->procBuf[i].codepoint;
			}
			this->resBufValidLen = this->procBufValidLen;
			this->procBufValidLen = 0;
			break;
        }
        if(this->procBufValidLen == 0){
			this->lastClass = uniChar.getCanonicalClass();
			this->procBuf[this->procBufValidLen++] = uniChar; 
        } else {
			startChar = this->procBuf;
			ret = startChar->getComposite(&compositeChar, &uniChar);
			cc = uniChar.getCanonicalClass();
			if(ret != -1 && (this->lastClass < cc || this->lastClass == 0)){
				this->procBuf[0] = compositeChar;
			} else {
				if(cc == 0){
					for(i=0;i<this->procBufValidLen;i++)
					{
						this->resBuf[i] = this->procBuf[i].codepoint;
					}
					this->resBufValidLen = this->procBufValidLen;
					this->procBufValidLen = 0;
				}
				this->lastClass = cc;
				this->procBuf[this->procBufValidLen++] = uniChar;
			}
        }
    }
    
	if (this->resBufValidLen > 0 && this->cursor < this->resBufValidLen)
	{
		*nextCharCP = this->resBuf[this->cursor++];
		return 0;
	}
	else
	{
		return -1;
	}
}

int Normalizer::normalizeNFxC(u32* unicodeArr, u16 len, u32* resultArr,u16 resultLimit, u16* resultLen, UnicodeNormalizeType type)
{
	//nfc
	int ret;
	UniCharIterator uniCharIt(unicodeArr, len);
	UniChar recursDecompBuf[4];
	RecursDecompIterator recursDecompIt(&uniCharIt, (type==NFC)?1:0, recursDecompBuf, 4);
	UniChar decompBuf[4];
	DecompIterator decompIt(&recursDecompIt, decompBuf, 4);
	u32 compBuf[4];
	UniChar compProcBuf[4];
	CompIterator compIt(&decompIt, compBuf, 4, compProcBuf,4);
	u32 uniCharCP;
	*resultLen = 0;
	int count = 0;

	switch (type)
	{
	case NFC:
	case NFKC:
		while (compIt.next(&uniCharCP) != -1)
		{
			resultArr[count++] = uniCharCP;
			if (resultLimit>0 && count == resultLimit)
			{
				break;
			}
		}
		break;
	default:
		//invalid
		return -1;
	}
	
	
	*resultLen = count;
	return 0;
}

int Normalizer::normalizeNFxD(u32* unicodeArr, u16 len, u32* resultArr,u16 resultLimit, u16* resultLen, UnicodeNormalizeType type)
{
	//nfc
	int ret;
	UniCharIterator uniCharIt(unicodeArr, len);
	UniChar recursDecompBuf[4];
	RecursDecompIterator recursDecompIt(&uniCharIt, (type==NFC)?1:0, recursDecompBuf, 4);
	UniChar decompBuf[4];
	DecompIterator decompIt(&recursDecompIt, decompBuf, 4);
	
	UniChar uniChar;
	*resultLen = 0;
	int count = 0;
	switch (type)
	{
	case NFD:
	case NFKD:
		while (decompIt.next(&uniChar) != -1)
		{
			resultArr[count++] = uniChar.codepoint;
			if (resultLimit>0 && count == resultLimit)
			{
				break;
			}
		}
		break;
	default:
		//invalid
		return -1;
	}
	
	*resultLen = count;
	return 0;
}

void Normalizer::test()
{
	u32 unicodeArr[4] = {0x212B, 0x1E0B, 0x0323, 0x2126};
	u32 resultArr[4] = {0};
	u16 resultLen = 0;
	this->normalizeNFxC(unicodeArr,4, resultArr,0, &resultLen,NFC);
	//result: 0x00C5, 0x1E0D, 0x0307, 0x03A9
}
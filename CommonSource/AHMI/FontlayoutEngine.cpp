#include "FontlayoutEngine.h"
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "aniamtion.h"
#include "drawImmediately_cd.h"
extern EncodingClass*		gEncoder;
extern ConfigInfoClass  ConfigData;
FontlayoutEngine::FontlayoutEngine
()
{
}

FontlayoutEngine::~FontlayoutEngine
()
{
}

Textinputbox::Textinputbox()
{
}

Textinputbox::~Textinputbox()
{
}

void Textinputbox::initialBox(s16 mx,s16 my,s16 mw,s16 mh)
{
	x = mx;
	y = my;
	w = mw;
	h = mh;
}

void Textinputbox::deleteVertical(s16 mh)
{
	y += mh;
	h -= mh;
}

LineTrcak::LineTrcak()
{
}

LineTrcak::~LineTrcak()
{
}

void LineTrcak::initialLineTrack(s16 mx,s16 my,s16 mlineHeight,s16 mlineInd,s16 mfirstTextIdx,s16 mlastTextIdx)
{
	x = mx;
	y = my;
	lineHeight = mlineHeight;
	lineInd = mlineInd;
	firstTextIdx = mfirstTextIdx;
	lastTextIdx = mlastTextIdx;
	layoutedCount = 0;
}

u8 FontlayoutEngine::getCharacterType(u32 codepoint)
{
	u8 curLetterType;
	if (codepoint<128)
		{
			curLetterType = 0;
		}
		else{
			curLetterType = 1;
		}
	return curLetterType;
}

funcStatus FontlayoutEngine::layoutlinesByOneTexture(
		Textinputbox *mbox,
		ParagraphAttrs paragraphAttrs,
		Span* spans,
		u8 numOfSpans,
		int fontRangeID,
		LineTrcak *mlinetrack,
		WidgetClassPtr p_wptr,
		//TextureClassPtr textureptr,
		u32 *u32p_sourceShift,
		NewMatrixPtr pWidgetMatrix,
		u8 noDraw,
		u8 *texdrawNum
		)
{
	funcStatus status = AHMI_FUNC_SUCCESS;
	s16 curTotalWidth = preCalculateInfoInLine(mbox,paragraphAttrs, spans,numOfSpans, fontRangeID, mlinetrack);
	//align = 0;
	int startXInLine;
	int fontPadding; // = (textureptr->TexWidth - fontsize)/2;
	int i,j;
	u8 hasNext = 0;
	u8 outOfNumber = 0;
	s16 nextSpanIdx;
	s16 nextTextIdx;
	if(paragraphAttrs.direction)
	{
		switch (paragraphAttrs.align)
		{
		case CENTERALIGN :
			startXInLine = mbox->x + paragraphAttrs.indentationLeft + (mbox->w - paragraphAttrs.indentationLeft - paragraphAttrs.indentationRight - curTotalWidth)/2;
			break;
		case LEFTALIGN:
			startXInLine = mbox->x + mbox->w -paragraphAttrs.indentationRight - curTotalWidth;
			break;
		case RIGHTALIGN:
		default:
			startXInLine = mbox->x + paragraphAttrs.indentationLeft;
			break;
		}
	}
	else
	{
		switch (paragraphAttrs.align)
		{
		case CENTERALIGN :
			startXInLine = mbox->x + paragraphAttrs.indentationLeft + (mbox->w - paragraphAttrs.indentationLeft - paragraphAttrs.indentationRight - curTotalWidth)/2;
			break;
		case RIGHTALIGN:
			startXInLine = mbox->x + mbox->w -paragraphAttrs.indentationRight - curTotalWidth;
			break;
		case LEFTALIGN:
		default:
			startXInLine = mbox->x + paragraphAttrs.indentationLeft;
			break;
		}
	}

	if (mlinetrack->lineInd == 0)
	{
		startXInLine += paragraphAttrs.firstLineIndentation;
	}
	startXInLine -= (spans[mlinetrack->firstSpanIdx].textureptr->TexWidth - spans[mlinetrack->firstSpanIdx].fontsize)/2;
	s16 startTextIdx = mlinetrack->firstTextIdx;
	s16 maxTextIdx = mlinetrack->lastTextIdx;

	for (j = mlinetrack->firstSpanIdx; j <= mlinetrack->lastSpanIdx; j++)
	{
		if (outOfNumber)
		{
			break;
		}
		fontPadding = (spans[j].textureptr->TexWidth - spans[j].fontsize)/2;
		if(j == mlinetrack->lastSpanIdx)
		{
			maxTextIdx = mlinetrack->lastTextIdx;
		}else
		{
			maxTextIdx = spans[j].codepointsNum-1;
		}
		for (i = startTextIdx; i <= maxTextIdx; i++)
		{
			u32 code = 0;
			int code_Pos = i;
			if(paragraphAttrs.direction)
			{
				code_Pos = maxTextIdx - i + startTextIdx;
			}
			else
			{
				code_Pos = i;
			}
			code = gEncoder->getCharacterIndexByCodePoint(ConfigData.encoding, spans[j].codepoints[code_Pos], fontRangeID);
			if (code == 0)
			{
				if((fontRangeID != -1) && (spans[j].codepoints[code_Pos] != 0))
				{
					code = '?';
				}
				else
				{
					//no code
					continue;
				}
			}
			else if(code < ' ')
			{
				code = ' ';
			}
			code = code - ' ';

			//check layouted count
			mlinetrack->layoutedCount++;
			if (mlinetrack->layoutedCount > numofNumber)
			{
				outOfNumber = 1;
				break;
			}

			spans[j].textureptr->FocusedSlice = code;
			if (code == 14)
			{
				spans[j].textureptr->OffsetX = (startXInLine + spans[j].fontsize/4) << 4;
			}
			else
			{
				spans[j].textureptr->OffsetX = startXInLine << 4;
			}
		
			spans[j].textureptr->OffsetY = (mbox->y + mlinetrack->lineHeight - spans[j].fontsize-fontPadding)<<4;
			if (code != 0)
			{
				if (!noDraw)
				{
					status = spans[j].textureptr->WriteNewSourceBuffer(u32p_sourceShift,pWidgetMatrix,spans[j].textureptr,p_wptr);
					if(status == AHMI_FUNC_MATRIX_ILLEGAL)
					{
						return AHMI_FUNC_MATRIX_ILLEGAL;
					}
					#if defined(CHIP_GC9002)
					(*texdrawNum) = (*texdrawNum) + 1;
					//SBBatchDraw background is one longsourcebuffer(64bytes), so 28 = (2048 - 64) / 64 - 3, 3 means puremask + textinputBackgroundPic + purecolor
					if((*texdrawNum) >= 28)
						break;
					#endif
				}
				
			}

			int nextcode_Pos = 0;
			if(paragraphAttrs.direction)
			{
				nextcode_Pos = code_Pos - 1;
			}
			else
			{
				nextcode_Pos = code_Pos + 1;
			}
			startXInLine += spans[j].fontsize + calculateNextSpacing(spans[j].codepoints[code_Pos],spans[j].codepoints[nextcode_Pos],spans[j].fullspacing,spans[j].halfspacing);
		}
		if (j+1<numOfSpans)
		{
			curTotalWidth += calculateNextSpacing(spans[j].codepoints[i-1], spans[j+1].codepoints[0],spans[j].fullspacing,spans[j].halfspacing);
		}
		startTextIdx = 0;
	}
	
	/*if (mlinetrack->lastTextIdx < numofNumber-1)
	{
		mlinetrack->firstTextIdx = mlinetrack->lastTextIdx + 1;
		mlinetrack->lineInd++;
			
		mbox->deleteVertical(mlinetrack->lineHeight + spacingBetweenLines);
		mlinetrack->lineHeight = 0;
		layoutlines(mbox,codepoints,mlinetrack,textureptr);
	}*/
	if (!outOfNumber)
	{
		if (mlinetrack->lastSpanIdx >= numOfSpans-1 && mlinetrack->lastTextIdx >= spans[mlinetrack->lastSpanIdx].codepointsNum-1)
		{

		}else if (mlinetrack->lastSpanIdx < numOfSpans-1)
		{
			if (mlinetrack->lastTextIdx < spans[mlinetrack->lastSpanIdx].codepointsNum-1)
			{
				nextSpanIdx = mlinetrack->lastSpanIdx;
				nextTextIdx = mlinetrack->lastTextIdx+1;
			}else
			{
				nextSpanIdx = mlinetrack->lastSpanIdx + 1;
				nextTextIdx = 0;
			}
			hasNext = 1;
		}else
		{
			nextSpanIdx = mlinetrack->lastSpanIdx;
			nextTextIdx = mlinetrack->lastTextIdx + 1;
			hasNext = 1;
		}
	}
	
	if (hasNext)
	{
		mlinetrack->firstSpanIdx = nextSpanIdx;
		mlinetrack->firstTextIdx = nextTextIdx;
		mlinetrack->lineInd += 1;
		mbox->deleteVertical(mlinetrack->lineHeight + paragraphAttrs.spacingBetweenLines);
		mlinetrack->lineHeight = 0;
		status = layoutlinesByOneTexture(mbox,paragraphAttrs,spans,numOfSpans, fontRangeID, mlinetrack,p_wptr,u32p_sourceShift,pWidgetMatrix,noDraw,texdrawNum);
		if(status == AHMI_FUNC_MATRIX_ILLEGAL)
		{
			return status;
		}
	}else
	{
		mbox->deleteVertical(mlinetrack->lineHeight + paragraphAttrs.spacingBetweenLines);
	}
	return AHMI_FUNC_SUCCESS;
}

s16 FontlayoutEngine::preCalculateInfoInLine(
	Textinputbox *mbox,
	ParagraphAttrs paragraphAttrs,
	Span* spans,
	u8 numOfSpans,
	int fontRangeID,
	LineTrcak *mlinetrack)
{
	s16 firstSpanIdx = mlinetrack->firstSpanIdx;
	s16 lastTextIdx = mlinetrack->firstTextIdx;
    s16 curTotalWidth = 0;
    s16 nextTotalWidth = 0;
    s16 lineHeight = 0;
	s16 nextLineHeight;
    s16 singleLineWidth = mbox->w - paragraphAttrs.indentationLeft - paragraphAttrs.indentationRight;
    bool isFirst = true;
	int fontsize;
	u32 *codepoints;

	int fullspacing;
	int halfspacing;
	int i,j;
	for (j = firstSpanIdx; j < numOfSpans; j++)
	{
		codepoints = spans[j].codepoints;
		fontsize = spans[j].fontsize;
		fullspacing = spans[j].fullspacing;
		halfspacing = spans[j].halfspacing;
		
		
		if (fontsize > lineHeight)
		{
			nextLineHeight = fontsize;
		}else
		{
			nextLineHeight = lineHeight;
		}
		
		for (i = lastTextIdx; i < spans[j].codepointsNum; i++)
		{
			u32 code = gEncoder->getCharacterIndexByCodePoint(ConfigData.encoding, codepoints[i], fontRangeID);
			if (code == '\n')
			{
				//change line
				mlinetrack->lastSpanIdx = j;
				mlinetrack->lastTextIdx = i;
				mlinetrack->lineHeight = lineHeight;
				return curTotalWidth ;
			}else if(code == 0)
			{
				//no code
				if((fontRangeID != -1) && (codepoints[i] != 0))
				{
					code = '?';
				}
				else
				{
					continue;
				}
			}else if(code < ' ')
			{
				code = ' ';
			}
			if(isFirst)
			{
				nextTotalWidth = curTotalWidth + fontsize;
				if(mlinetrack->lineInd == 0){
					nextTotalWidth += paragraphAttrs.firstLineIndentation||0;
				}
                    
				isFirst = false;
			 }
			 else
			 {
				nextTotalWidth = curTotalWidth +fontsize + calculateNextSpacing(codepoints[i-1],codepoints[i],fullspacing,halfspacing);
			 }
                
			if(nextTotalWidth > singleLineWidth){
				//need change line
				mlinetrack->lastSpanIdx = j;
				mlinetrack->lastTextIdx = i-1;
				mlinetrack->lineHeight = lineHeight;
				return curTotalWidth ;
            
			}else
			{
				lineHeight = nextLineHeight;
			}
			//spacing
			curTotalWidth = nextTotalWidth;
		}

		if (j+1<numOfSpans)
		{
			curTotalWidth += calculateNextSpacing(codepoints[i-1], spans[j+1].codepoints[0],fullspacing,halfspacing);
		}
		
            
        lastTextIdx = 0;
	}
	
	mlinetrack->lastSpanIdx = j-1;
	mlinetrack->lastTextIdx = i-1;
	mlinetrack->lineHeight = lineHeight;
    return curTotalWidth ;
}

void FontlayoutEngine::preCalculateBoxY(
	Textinputbox *mbox,
	ParagraphAttrs paragraphAttrs,
	Span* spans,
	u8 numOfSpans,
	int fontRangeID,
	LineTrcak *mlinetrack,
	int *totalTextLength
	){
	int	totalLines = 0;
	s16 lastTextIdx = mlinetrack->firstTextIdx;
    s16 curTotalWidth = 0;
    s16 nextTotalWidth = 0;
    s16 lineHeight = 0;
	s16 nextLineHeight;
	int verticalAlign = paragraphAttrs.verticalAlign;
    s16 singleLineWidth = mbox->w - paragraphAttrs.indentationLeft - paragraphAttrs.indentationRight;
	u16 spacingBetweenLines = paragraphAttrs.spacingBetweenLines;
    bool isFirst = true;
	int fontsize;
	u32 *codepoints;
	s16 originOffsetY = mbox->y;

	int fullspacing;
	int halfspacing;
	int i,j;
	for (j = 0; j < numOfSpans; j++)
	{
		codepoints = spans[j].codepoints;
		fontsize = spans[j].fontsize;
		fullspacing = spans[j].fullspacing;
		halfspacing = spans[j].halfspacing;
		
		
		if (fontsize > lineHeight)
		{
			nextLineHeight = fontsize;
		}else
		{
			nextLineHeight = lineHeight;
		}
		
		for (i = lastTextIdx; i < spans[j].codepointsNum; i++)
		{
			u32 code = gEncoder->getCharacterIndexByCodePoint(ConfigData.encoding, codepoints[i], fontRangeID);
			if (code == '\n')
			{
				//change line
				totalLines++;
				curTotalWidth = 0;
				lastTextIdx = 0;
				continue;
			}else if(code == 0)
			{
				//no code
				if((fontRangeID != -1) && (codepoints[i] != 0))
				{
					code = '?';
				}
				else
				{
					continue;
				}
			}else if(code < ' ')
			{
				code = ' ';
			}
			if(isFirst)
			{
				nextTotalWidth = curTotalWidth + fontsize;
				if(mlinetrack->lineInd == 0){
					nextTotalWidth += paragraphAttrs.firstLineIndentation||0;
				}
                    
				isFirst = false;
			 }
			 else
			 {
				nextTotalWidth = curTotalWidth +fontsize + calculateNextSpacing(codepoints[i-1],codepoints[i],fullspacing,halfspacing);
			 }
             
			curTotalWidth = nextTotalWidth;
			if(nextTotalWidth > singleLineWidth){
				totalLines++;
				//need change line
				curTotalWidth = 0;
				lastTextIdx = i;
				i--;
            
			}
			
		}
		if (curTotalWidth<=singleLineWidth) {                    
            totalLines++;
            curTotalWidth = 0;
            lastTextIdx = 0;
        }
		
	}

	*totalTextLength = totalLines*(nextLineHeight+spacingBetweenLines);
	if(verticalAlign==1)
		mbox->y = originOffsetY + (mbox->h - totalLines*(nextLineHeight+spacingBetweenLines))/2;
	if(verticalAlign==2)
		mbox->y = originOffsetY + mbox->h - totalLines*(nextLineHeight+spacingBetweenLines);
	if(verticalAlign==0)
		mbox->y = (originOffsetY + spacingBetweenLines);
}

s16 FontlayoutEngine::calculateNextSpacing(
	u32 formerchar,
	u32 laterchar,
	int fullfontspacing,
	int halffontspacing
	)
{
	if (formerchar == 0 || laterchar == 0)
	{
		return 0;
	}
	int formertype = getCharacterType(formerchar);
	int latertype = getCharacterType(laterchar);
    if(formertype == 0){
        if(latertype == 0){
            return halffontspacing;
        }
		else{
            return (fullfontspacing+halffontspacing)/2;
        }
    }
	else{
        if(latertype == 0){
            return (fullfontspacing+halffontspacing)/2;
        }
		else{
            return fullfontspacing;
        }
    }
}

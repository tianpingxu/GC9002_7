#include <publicType.h>
#include "TextureClass.h"

class Textinputbox
{
public:
	Textinputbox();
	Textinputbox(s16 mx,s16 my,s16 mw,s16 mh);
	~Textinputbox();
	void initialBox(s16 mx,s16 my,s16 mw,s16 mh);
	s16 x,y,w,h;
	void deleteVertical(s16 mh);

private:

};

typedef struct {
	u32* codepoints;
	int codepointsNum;
	int fontsize;
	int fullspacing;
	int halfspacing;
	TextureClassPtr textureptr;//act as fontFamily
} Span;

typedef struct {
	s16 indentationLeft;
	s16 indentationRight;
	s16 firstLineIndentation;
	u16 spacingBetweenLines;
	u8 direction;    //text direction
	u8 align;        //align mode
	int verticalAlign;        //verticalAlign mode
} ParagraphAttrs;

class LineTrcak
{
public:
	LineTrcak();
	LineTrcak(s16 mx,s16 my,s16 mlineHeight,s16 mlineInd,s16 mfirstTextIdx,s16 mlastTextIdx);
	void initialLineTrack(s16 mx,s16 my,s16 mlineHeight,s16 mlineInd,s16 mfirstTextIdx,s16 mlastTextIdx);
	~LineTrcak();
	s16 x, y,lineHeight,lineInd, firstSpanIdx, lastSpanIdx, firstTextIdx,lastTextIdx, layoutedCount;
   
private:

};
class FontlayoutEngine

{
public:FontlayoutEngine();
	~FontlayoutEngine();
	
	/*int fontsize;
	int fullspacing;
	int halfspacing;*/
	u16 numofNumber;
	

	u8 getCharacterType(
		u32 codepoint
		);


	funcStatus layoutlinesByOneTexture(
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
		);

	s16 calculateNextSpacing(
		u32 formerchar,
		u32 laterchar,
		int fullfontspacing,
		int halffontspacing
		);

	s16 preCalculateInfoInLine(
		Textinputbox *mbox,
		ParagraphAttrs paragraphAttrs,
		Span* spans,
		u8 numOfSpans,
		int fontRangeID,
		LineTrcak *mlinetrack
		);

	void preCalculateBoxY(
		Textinputbox *mbox,
		ParagraphAttrs paragraphAttrs,
		Span* spans,
		u8 numOfSpans,
		int fontRangeID,
		LineTrcak *mlinetrack,
		int *totalTextLength
		);

private:

};


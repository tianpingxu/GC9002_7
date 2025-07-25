//created by Zzen1sS
//2020/4/2
#ifndef KBSTATE__H
#define KBSTATE__H

#include "publicDefine.h"





typedef struct{
	u32 numValue;
	u8 stringValue[32];
	u8 hintPage; //for chinese
	u8 hintPagetCount; //for chinese
	u8 hintInputValue[32]; //for chinese
	u32 hintCandidates[5]; //for chinese
}KBState;

typedef struct{
	u32 currentValue;
	u16 tagID;
	u8 mode;
	u8 state;
	u8 statebyItem;
	u16 curMenuItemSequence[32];
}MenuState;



#endif // !WAVEFILTER__H

#ifndef DELAYARG__H
#define DELAYARG__H
#include "publicDefine.h"
typedef struct{
	int intervalID;
	
	u32 instructionAddr;

	u16 workingPageId;
}DelayArg;
#endif 

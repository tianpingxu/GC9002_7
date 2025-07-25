////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMID
//
// Create Date:   2015/11/30
// File Name:     tagUpdate.c
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2015/11/30
// Revision 2.04 - C++, 20160321 by ¨®¨²¡äo¨®a
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////
#ifndef PC_SIM_Gen
#include "FreeRTOS.h"
#endif
#include "publicType.h"
#include "tagUpdateClass_c1.h"
#include "publicDefine.h"

#ifndef  TAGUPDATE__H
#define  TAGUPDATE__H

#ifdef AHMI_CORE
class TagUpdateClass
{
public:
	u32 mRegID;         //reg?¨¤¡§oo?
	u16 mTagID;         //tag?¨¤¡§oo?
#ifndef PC_SIM_Gen
	TagUpdateClass();
	~TagUpdateClass();
#endif
};

typedef class TagUpdateClass* TagUpdateClassPtr;

void initTagUpdateQueue(u16 NumofTagUpdateQueue,u16 NumofTags, TagClassPtr tagPtr, TagUpdateClassPtr TagUpdatePtr);
void sortTagUpdateClass(u16 NumofTagUpdateQueue,TagUpdateClassPtr TagUpdatePtr);
void UpdateAllTags(void);

#endif

#endif


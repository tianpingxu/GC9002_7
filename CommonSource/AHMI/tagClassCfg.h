////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2016/03/21
// File Name:     TextureClass.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef TAGCLASSCFG__H
#define TAGCLASSCFG__H

#include "publicType.h"
#include "publicDefine.h"
#include "TagClass.h"

#ifdef AHMI_CORE
class TagClassCfg
{
public:
	u32 mRegID;                  //linked reg
	//u32 mValue;                  //the intial value of tag
	u32 mStoreID;                 //the ID of tag which need store into flash
	u16 mAddr;
	u16 mValueLen;               //value length
	u16 mCurArryElemNum;			//current array element number, only used in arrayTag.
	u16	mTagType;                //Type:0:int;1:string
	u16  NumOfWidgetLinker;       //number of linked elements.
	u16  NumOfDynamicPageAction;  //number of linked dynamicPage action.
	u16  NumOfCanvasAction;       //linked canvas actions num
};
#endif

#endif

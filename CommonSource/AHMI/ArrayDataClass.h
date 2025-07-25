////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:      Zzen1sS
//
// Create Date:   2022/11/16
// File Name:     ArrayDataClass.cpp
// Project Name:  
// 
//
//

// 
////////////////////////////////////////////////////////////////////////////////

#ifndef   ArrayDataClass__H
#define   ArrayDataClass__H

#include "AHMIBasicStructure.h"
#include "publicType.h"
#include "publicDefine.h"
#ifdef AHMI_CORE


class ArrayDataClass
{
public:
	u16 id;        // id of  array data class
	u16 length;
	u32 addr;//addr offset from gString

#ifndef PC_SIM_Gen
	ArrayDataClass();
	~ArrayDataClass();

	void initArrayData(u16 id, u16 length, u8 *value);
#endif

};

typedef ArrayDataClass* ArrayDataClassPtr;

#endif

#endif


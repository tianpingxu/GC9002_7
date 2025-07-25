////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     
//
// Create Date:   2017/8/10
// File Name:     StringVector.cpp
// Project Name:  
// 
// Revision:1.0.0
// Additional Comments:
// StringClass is used to store string encoding parsed from the IDE, which is stored as an array
// For example, according to UTF-8 encoding, IDE will parse 'ab' into array [97,98].
// The length of the string array is unified to 32, that is to say, it can store up to 32 characters at most.
//
//
// create by lixiang
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef   StringCLASS__H
#define   StringCLASS__H

#include "AHMIBasicStructure.h"
#include "publicType.h"
#include "publicDefine.h"
#ifdef AHMI_CORE


class StringClass
{
public:
	u16 id;        // id of  string
	//u8 value[STRINGMAXSIZE];  // value of string, 32 most
	u16 length;
	u32 addr;//addr offset from gString

#ifndef PC_SIM_Gen
	StringClass();
	~StringClass();

	void initString(u16 id, u16 length, u8 *value);
#endif

};

typedef StringClass* StringClassPtr;

#endif

#endif


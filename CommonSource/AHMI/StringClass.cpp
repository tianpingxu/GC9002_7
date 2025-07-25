////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     lixiang
//
// Create Date:   2018/8/10
//
// 
// StringClass is used to store string encoding parsed from the IDE, which is stored as an array
// For example, according to UTF-8 encoding, IDE will parse 'ab' into array [97,98].
// The length of the string array is unified to 32, that is to say, it can store up to 32 characters at most.
//
// Revision:1.0.0
// 
////////////////////////////////////////////////////////////////////////////////

#include "publicInclude.h"
#include "publicDefine.h"

StringClass::StringClass(void)
{}

StringClass::~StringClass(void)
{}

//-----------------------------
// function: initString
// @brief: Copy data from a buffer to a tag binding address.
// parameters:
//  @param1:id          ID of a tag.
//  @param1:value       pointer to a buffer.    
//-----------------------------
void StringClass::initString(u16 id, u16 length, u8 *value){
	this->id = id;
	this->length = length;
}


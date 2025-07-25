////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     Zzen1sS
//
// Create Date:   2022/11/16
//
// 
// 
////////////////////////////////////////////////////////////////////////////////

#include "publicInclude.h"
#include "publicDefine.h"

ArrayDataClass::ArrayDataClass(void)
{}

ArrayDataClass::~ArrayDataClass(void)
{}

//-----------------------------
// function: initArrayData
// @brief: Copy data from a buffer to a tag binding address.
// parameters:
//  @param1:id          ID of a tag.
//  @param1:value       pointer to a buffer.    
//-----------------------------
void ArrayDataClass::initArrayData(u16 id, u16 length, u8 *value){
	this->id = id;
	this->length = length;
}


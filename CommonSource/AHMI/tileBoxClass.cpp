////////////////////////////////////////////////////////////////////////////////
// Company:		 Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     tileBoxClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi
// Additional Comments:
//    
////////////////////////////////////////////////////////////////////////////////
#include "tileBoxClass.h"
#include "publicDefine.h"
#include "publicInclude.h"

#ifdef AHMI_CORE

TileBoxClass::TileBoxClass(void)
{}

TileBoxClass::~TileBoxClass(void)
{}

//-----------------------------
// function name£º  adjustSourceBuffer
// Adjust SourceBuffer
// param list£º
//  @param1 none
// Remarks(Revision between versions):
// ...
//-----------------------------
funcStatus TileBoxClass::adjustSourceBuffer()
{
	if(this->LeftBox < 0)
		this->LeftBox = 0;
	if(this->TopBox < 0)
		this->TopBox = 0;
	if(this->RightBox > (s8)((TILE_NUM_X) - 1))
		this->RightBox = (s8)((TILE_NUM_X) - 1);
	if(this->ButtomBox > (s8)((TILE_NUM_Y) - 1))
		this->ButtomBox = (s8)((TILE_NUM_Y) - 1);
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name£º  sourceBoxCopy
// Copy input tile box
// param list£º
//  @param1 TileBoxClassPtr   pSourceBox    input tile box class pointer
// Remarks(Revision between versions):
// ...
//-----------------------------
funcStatus TileBoxClass::sourceBoxCopy(TileBoxClassPtr pSourceBox)
{
	this->LeftBox   = pSourceBox->LeftBox   ;
	this->RightBox  = pSourceBox->RightBox  ;
	this->TopBox    = pSourceBox->TopBox    ;
	this->ButtomBox = pSourceBox->ButtomBox ;
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name£º  sourceBoxOr
// tile boxes make | only merge when the two bounding boxes contain the same area.
// param list£º
//  @param1 TileBoxClassPtr   pSourceBox    input tile box class pointer
// Remarks(Revision between versions):
// ...
//-----------------------------
funcStatus TileBoxClass::sourceBoxOr(TileBoxClassPtr pSourceBox)
{
	if(pSourceBox->LeftBox < this->LeftBox)
		this->LeftBox = pSourceBox->LeftBox;
	if(pSourceBox->RightBox > this->RightBox)
		this->RightBox = pSourceBox->RightBox;
	if(pSourceBox->TopBox < this->TopBox)
		 this->TopBox = pSourceBox->TopBox;
	if(pSourceBox->ButtomBox >  this->ButtomBox)
		 this->ButtomBox = pSourceBox->ButtomBox;
	return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name£º  sourceBoxOverlap
// Determines whether two tile boxes have intersecting parts, and then returns AHMI_FUNC_SUCCESS
// param list£º
//  @param1 TileBoxClassPtr   pSourceBox    input tile box class pointer
// Remarks(Revision between versions):
// ...
//-----------------------------
funcStatus TileBoxClass::sourceBoxOverlap(TileBoxClassPtr pSourceBox)
{
	if(
		pSourceBox->LeftBox  >  this->RightBox  || 
		pSourceBox->RightBox <  this->LeftBox   || 
		pSourceBox->TopBox   >  this->ButtomBox ||
		pSourceBox->ButtomBox < this->TopBox 
		)
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_TileBoxOverrun);
	else 
		return AHMI_FUNC_SUCCESS;
}

//-----------------------------
// function name£º  sourceBoxContain
// Determines whether the tile box contains input bounding boxes and returns AHMI_FUNC_SUCCESS.
// param list£º
//  @param1 TileBoxClassPtr   pSourceBox    input tile box class pointer
// Remarks(Revision between versions):
// ...
//-----------------------------
funcStatus TileBoxClass::sourceBoxContain(TileBoxClassPtr pSourceBox)
{
	if(
		pSourceBox->LeftBox   >= this->LeftBox  && 
		pSourceBox->RightBox  <= this->RightBox && 
		pSourceBox->TopBox    >= this->TopBox   &&
		pSourceBox->ButtomBox <= this->ButtomBox 
		)
		return AHMI_FUNC_SUCCESS;
	else
		return (EM_enErrRetModularTexture | EM_ErrRetTexture_TileBoxOverrun);
}

#endif

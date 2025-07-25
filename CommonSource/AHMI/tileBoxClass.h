////////////////////////////////////////////////////////////////////////////////
// Company:		 Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     tileBoxClass.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi 
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef TILEBOXCLASS__H
#define TILEBOXCLASS__H

#include "publicType.h"
#include "publicDefine.h"
#include "refreshQueueHandler.h"

#ifdef AHMI_CORE

class TileBoxClass;
class NewMatrix;

typedef TileBoxClass* TileBoxClassPtr;
typedef NewMatrix*  NewMatrixPtr;

class TileBoxClass
{
public:	
	//variables
	s8 LeftBox;  //left tile box
	s8 TopBox;   //top tile box
	s8 RightBox; //right tile box
	s8 ButtomBox;//bottom tile box

#ifndef PC_SIM_Gen
	//fuctions
	TileBoxClass(void);
	~TileBoxClass(void);

	//-----------------------------
	// function name£º  adjustSourceBuffer
	// Adjust SourceBuffer
	// param list£º
	//  @param1 none
	// Remarks(Revision between versions):
	// ...
	//-----------------------------
	funcStatus adjustSourceBuffer();

	//-----------------------------
	// function name£º  sourceBoxCopy
	// Copy input tile box
	// param list£º
	//  @param1 TileBoxClassPtr   pSourceBox    input tile box class pointer
	// Remarks(Revision between versions):
	// ...
	//-----------------------------
	funcStatus sourceBoxCopy(TileBoxClassPtr pSourceBox);

	//-----------------------------
	// function name£º  sourceBoxOr
	// tile boxes make | only merge when the two bounding boxes contain the same area.
	// param list£º
	//  @param1 TileBoxClassPtr   pSourceBox    input tile box class pointer
	// Remarks(Revision between versions):
	// ...
	//-----------------------------
	funcStatus sourceBoxOr(TileBoxClassPtr pSourceBox);

	//-----------------------------
	// function name£º  sourceBoxOverlap
	// Determines whether two tile boxes have intersecting parts, and then returns AHMI_FUNC_SUCCESS
	// param list£º
	//  @param1 TileBoxClassPtr   pSourceBox    input tile box class pointer
	// Remarks(Revision between versions):
	// ...
	//-----------------------------
	funcStatus sourceBoxOverlap(TileBoxClassPtr pSourceBox);

	//-----------------------------
	// function name£º  sourceBoxContain
	// Determines whether the tile box contains input bounding boxes and returns AHMI_FUNC_SUCCESS.
	// param list£º
	//  @param1 TileBoxClassPtr   pSourceBox    input tile box class pointer
	// Remarks(Revision between versions):
	// ...
	//-----------------------------
	funcStatus sourceBoxContain(TileBoxClassPtr pSourceBox);
	
#endif
};

#endif

#endif

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:     AHMI
//
// Create Date:   2016/03/21
// File Name:     CanvasClass.cpp
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 3.00 - File Created 2017/05/17 by  Zhou Yuzhi
// Additional Comments:
//   
// 
////////////////////////////////////////////////////////////////////////////////
#include "publicInclude.h"
#include "AHMIBasicDefine.h"
#include "drawImmediately_cd.h"
#include "profiling.h"

extern u32 gFrameCount ;
extern TagClassPtr     TagPtr;

funcStatus setFrameTagValue(u32 value)
{
//	TagPtr[SYSTEM_FRAME_RATE_TAG].setValue(gFrameCount, SYSTEM_FRAME_RATE_TAG);
	return AHMI_FUNC_SUCCESS;
}


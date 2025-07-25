////////////////////////////////////////////////////////////////////////////////
// Company:		 Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     DynamicPage.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef SUBCANVASCLASS__H
#define SUBCANVASCLASS__H

#include "publicType.h"
#include "publicDefine.h"

#ifdef AHMI_CORE

class CanvasClass;
typedef CanvasClass* CanvasClassPtr;

typedef enum
{
    EM_ErrRetSubcanvas_NULLPointerErr = 1,
    EM_ErrRetSubcanvas_DurationOverrun,
	EM_ErrRetSubcanvas_AnimationTypeErr
   
}EM_ErrRetSubcanvas;

class SubCanvasClass
{
public:
	//variables
	NewMatrix mSubCanvasMatrix; //matrix
	u16 contentWidth;
	u16 contentHeight;
	s16 contentOffsetX;
	s16 contentOffsetY;
	s16 mWidth;
	s16 mHeight;
	s16 mOffsetX;       //integer
	s16 mOffsetY;       //integer
    s16 mZoomX;         //zoom scale X
    s16 mZoomY;         //zoom scale X
    s16 mRotateCenterX; //Rotate center pointer X
    s16 mRotateCenterY; //Rotate center pointer Y
    s16 mRotateAngle;   //Rotate angle
    s16 mProjectionX;   //Projection angle X
    s16 mProjectionY;   //Projection angle Y
	u16 NumOfWidget;      //number of widget
	u16 StartAddrOfWidget;//The array subscript of the initial widget.
	u16 NumofTouchWidget; //number of touch widget
	u16 StartAddrOfTouchWidget; //The array subscript of the touch widget.
	u16 mATag;
	s16 animationStartOffset;
	u8 shouldShow;
	u8 attachCanvas;
	u8 startAddrOfSubCanAction; //Initial instruction address
	u8 shouldShowScrollBar;     //1 for show, 0 for not show
	u8 scrollWidgetScalePadding; //0 - 50, widget scale padding when scrol
	u8 scrollWidgetScaleBorderRadius; //widget scale border radius
	u8 videoExist;				//1 for video exist, 0 for video do not exist
	u8 reserved2;

#ifndef PC_SIM_Gen

	//functions
	SubCanvasClass(void);
	~SubCanvasClass(void);

	//-----------------------------
	// name£º  initSubCanvas
	// init SubCanvas matrix & mATag
	//  
	// @param1 none
	// addtional comments :
	// ...
	//-----------------------------
	funcStatus initSubCanvas(void);

	//-----------------------------
	// name£º  initWidgetInSubCanvas
	// init SubCanvas widget value
	//  
	// @param1 none
	// addtional comments :
	// ...
	//-----------------------------
	funcStatus initWidgetValueInSubCanvas(
		DynamicPageClassPtr pageptr
		);

	//-----------------------------
	// name   setAtag
	// set animation tag
	//  
	// @param1 u16 value  this value is between 0 and totalframe
	// @param2 SubCanvasClassPtr lastScptr last shown subCanvas
	// addtional comments :
	// ...
	//-----------------------------
	u32 setAtag(u16 value, SubCanvasClassPtr lastScptr = NULL);

	//-----------------------------
	// name   animationFinishCallback
	// process animation finish
	//  
	// @param1 SubCanvasClassPtr lastScptr last shown subCanvas
	// addtional comments :
	// ...
	//-----------------------------
	u32 animationFinishCallback(SubCanvasClassPtr lastScptr = NULL);

	funcStatus CalculateMatrix(NewMatrix* pSubCanvasMatrix);

	void SetSubCanvasMatrixRefreshFlag(void);

#endif
};

typedef SubCanvasClass* SubCanvasClassPtr;

#endif

#endif

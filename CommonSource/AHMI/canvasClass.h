////////////////////////////////////////////////////////////////////////////////
// Company:		 Graphichina Inc
//
// Create Date:   2016/03/21
// File Name:     canvasClass.h
// Project Name:  AHMISimulator
// 
// Revision:
// Revision 2.00 - File Created 2016/03/21 by Zhou Yuzhi
// Additional Comments:
//    
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef CANVASCLASS__H
#define CANVASCLASS__H

#include "publicType.h"
#include "publicDefine.h"
#include "canvasClass_c1.h"
#include "Easing.h"

#ifdef AHMI_CORE

extern TagClassPtr			TagPtr;

typedef enum
{
    EM_ErrRetCanvas_NULLPointerErr = 1,
    EM_ErrRetCanvas_DrawWidgetErr,
    EM_ErrRetCanvas_CreateAnimationErr,
    EM_ErrRetCanvas_DurationOverrun,
	EM_ErrRetCanvas_TriggerAnimationErr
   
}EM_ErrRetCanvas;

/**
*  draw canvas class
*/
class CanvasClass
{
public:
	//±‰¡ø
	NewMatrix mCanvasMatrix;
	int ReleasingSpeed;
	u32 OldTickForCanvasSwipe;
    u8* pCanvasActionStartADDR;
	u16 mCanvaAttr;     //reserved
	u16 mwidth;         //integer
	u16 mheight;        //integer
	s16 moffsetX;       //integer
	s16 moffsetY;       //integer
    s16 mZoomX;         //zoom scale X
    s16 mZoomY;         //zoom scale X
    s16 mRotateCenterX; //Rotate center pointer X
    s16 mRotateCenterY; //Rotate center pointer Y
	s16 mScaleCenterX; //Scale center pointer X
    s16 mScaleCenterY; //Scale center pointer Y
    s16 mRotateAngle;   //Rotate angle
    s16 mProjectionX;   //Projection angle X
    s16 mProjectionY;   //Projection angle Y
	u16 mStartAddrOfSubCanvas;
	u16 mNumOfCanvasAction;
	u16 bindTagID;   //Bound tag label
	u16 subcanvasAnimationDuration;
	u16 constSubcanvasAnimationDuration;
	u16 mATag;                           //animation tag
	u16 mRefreshType;			//canvas refresh type
	u16 pressedPosX;
	u16 pressedPosY;
	u16 holdPosX;
	u16 holdPosY;
	u8  mCanvasID;   //canvas label
	u8  mFocusedCanvas; //current subcanvas
	u8  mNumOfSubCanvas;
	u8 animationType;                    //Animation type of child canvas switching
	u8 timingFun;
	//u8  subcanvasAnimationFrame;       //Subcanvas switching animation time
	u8 startAddrOfCanvasCustomAnimation; // the start address of canvas animation for custom.
	u8 numOfCanvasCustomAnimation;       // the number of the animation
	u8 curCustomAnimationPtr;            // the current runing custom animation, -1 indicating that there is no animation running
	u8 pressFlag;
	u8 CanvasState;// 0 for initial, 1 for swipe or subcanvas animation, 2 for drag
	u8 pressInEdges; //press point in edges, 0:top 1:bottom 2:left 3:right
	u8 pullState; //bit 0: horizontal isPulling, bit 1:vertical isPulling, bit 2: 0 forward|1 back
	u8 subCanvasWillLoadIdx;
	u8 ReleasingAnimationFlag;
	//s16 aniamtionStartOffset;
	

#ifndef PC_SIM_Gen
	//function
	CanvasClass(void);
	~CanvasClass(void);

	//-----------------------------
	// name£∫ loadSubCanvasWithAnimation
	// load new subcanvas with animation
	//
	// @param1 SubCanvasClassPtr pCurSubCanvas
	// @param2 SubCanvasClassPtr pNextSubCanvas
	// additional comments
	//   none
	//-----------------------------
	u32 loadSubCanvasWithAnimation(
		SubCanvasClassPtr pCurSubCanvas,
		SubCanvasClassPtr pNextSubCanvas
		);

	//-----------------------------
	// name£∫ focusNewSubCanvas
	// load new subcanvas
	//
	// @param1 u8 subCanvasID
	// additional comments
	//   none
	//-----------------------------
	funcStatus focusNewSubCanvas(
		u8 subCanvasID
		);

	//-----------------------------
	// name£∫ drawCanvas
	// draw canvas function
	//
	// @param1 TileBoxClassPtr tileBox : sourcebox class
	// @param2 u32 *sourceShift : sourcebuffer pointer
	// @param3 u8  pageEnable : whether refresh total page
	// @param4 matrixClassPtr pPageMatrix : additional matrix pointer
	// @param5 SubCanvasClassPtr scptr : subcanvas pointer
	// @param6 u8 RefreshType : the type of refresh
	// @param7 u8 staticTextureEn : whether draw into the static texture buffer
	// additional comments
	//   none
	//-----------------------------
	u32 drawCanvas(
		TileBoxClassPtr tileBox , //sourcebox class
		u32 *sourceShift        , //sourcebuffer pointer
		u8 pageEnable           ,  //whether refresh total page
		DynamicPageClassPtr pagetr, //additional matrix pointer
		SubCanvasClassPtr scptr   , //subcanvas pointer
		u8 RefreshType            , //the type of refresh
		u8 staticTextureEn        //whether draw into the static texture buffer
		);

	funcStatus CalculateMatrix(NewMatrix* curCanvasMatrix);

	funcStatus CalculateROIPoint(NewMatrix* curCanvasMatrix, PointClass* p0, PointClass* p1, PointClass* p2, PointClass* p3);

	void SetCanvasMatrixRefreshFlag(void);

	void updateSubCanvasListOffset(s16 offset, u8 direction, u8 isReset = 0); //horizontal 0, vertical 1

	void updateSubCanvasOffset(s16 offsetX, s16 offsetY);

	u8 calEdgesPressed(s16 x, s16 y);

	void LoadNextSubCanvasAfterReleasingAnimation(void);

	//-----------------------------
	// name£∫ initCanvas
	// initial canvas
	//
	// @param1 none
	// additional comments
	//   none
	//-----------------------------
	funcStatus initCanvas(void);

	//-----------------------------
	// name£∫ setATag
	// set the animation tag, compute the matrix
	// canvas animation is the custom animation for canvas
	//  
	//     @param1 u8 value
	// addtional comments :
	//   none
	//-----------------------------
	u32 setATag(u16 value);

	//-----------------------------
	// name£∫ computeCanvasMatrix
	// compute the matrix of canvas
	// When page exists scaling,the offset of canvas relative to page should be multiplied by the scaling factor of page.
	// Finally, the offset of canvas is offset relative to moffset.
	//  
	//     @param1 matrixClassPtr pPageMatrix pointer
	//     @param2 matrixClassPtr curCanvasMatrix pointer
	// addtional comments :
	//   this matrix is inverse matrix
	//-----------------------------
	funcStatus computeCanvasMatrix(NewMatrixPtr pPageMatrix, NewMatrixPtr curCanvasMatrix);

	//-----------------------------
	// name£∫ triggerAnimation
	//trigger canvas custom animation
	//  
	//     @param1 u8 animationID
	// addtional comments :
	//   none
	//-----------------------------
	u32 triggerAnimation(u8 animationID);

	u32 mouseTouchCtrl(
		ActionTriggerClassPtr ActionPtr,
		PIDPoint* pPressPoint,
		PIDPoint* pReleasePoint,
		PIDPoint* pAbsolutePressPoint,
		PIDPoint* pAbsoluteReleasePoint,
		u8 u8_pageRefresh      
	);

#endif

};

#define  CanvasMatrixRefreshFlag	(0x02)

#endif

#endif
